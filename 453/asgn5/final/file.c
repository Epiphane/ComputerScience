/*
 * minget and minls - Minix file management 
 *
 * CPE 453 Assignment 5
 *
 * Author: Thomas Steinke (tsteinke@calpoly.edu)
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>

#include "file.h"

FILE *fs;
uint32_t fs_lFirst;
uint32_t fs_size;
size_t fs_imap, fs_zmap, fs_inodes, fs_zones;
size_t zone_size;
struct superblock fs_superblock;
struct inode root;

int verbose = 0;

/* Verbosity output */
void print_superblock(struct superblock *superblock);
void print_inode(struct inode *inode);

/* Error checking */
void check_mbr_sig(long offset);
void check_partition(struct partition *partition, const char *prefix, int n);

/* Set the verbosity of the program */
void set_verbose(int _verbose) {
   verbose = _verbose;
}

/* Send an error and exit the program */
void errorAndExit(const char *err, int debug) {
   fprintf(stderr, err, debug, strerror(errno));
   exit(1);
}

/* Open the file system/possible partitions */
void open_fs_part(int part, int subpart);
void open_fs(char *path, int part, int subpart) {
   size_t res;

   fs = fopen(path, "r");
   if (fs == NULL) {
      fprintf(stderr, "Error opening %s: %s\n", path, strerror(errno));
      exit(1);
   }

   if (part == -1) { // NO partition
      fs_lFirst = 0;
   }
   else {
      open_fs_part(part, subpart);
   }

   // Read in superblock!
   if (fseek(fs, SECTOR_SIZE * fs_lFirst + SUPERBLOCK_LOCATION, SEEK_SET) != 0)
      errorAndExit("Error seeking to superblock location %d: %s\n", 
                   fs_lFirst + SUPERBLOCK_LOCATION);
   res = fread(&fs_superblock, 1, sizeof(struct superblock), fs);
   if (res != sizeof(struct superblock))
      errorAndExit("Superblock only read %d bytes.\n", res);

   // Confirm it's a minix filesystem
   if (fs_superblock.magic != MINIX_MAGIC_NUMBER)
      errorAndExit("Partition not a Minix filesystem (magic number: %d)\n", 
                   fs_superblock.magic);

   if (verbose)
      print_superblock(&fs_superblock);

   // Set references to each section
   fs_imap = SECTOR_SIZE * fs_lFirst +
      fs_superblock.blocksize * IMAP_BLOCK_OFFSET;
   fs_zmap = fs_imap + fs_superblock.blocksize * fs_superblock.i_blocks;
   fs_inodes = fs_zmap + fs_superblock.blocksize * fs_superblock.z_blocks;
   zone_size = fs_superblock.blocksize << fs_superblock.log_zone_size;
   fs_zones = SECTOR_SIZE * fs_lFirst + zone_size * fs_superblock.firstdata;
}

/* Open a partition of a file system */
void open_fs_part(int part, int subpart) {
   struct partition partitions[MAX_PARTITIONS];
   size_t res;

   check_mbr_sig(0);

   // Load partition table entries
   if (fseek(fs, PARTITION_TABLE, SEEK_SET) != 0) {
      fprintf(stderr, "Error loading partition table: %s\n", strerror(errno));
      exit(1);
   }
   res = fread(partitions, sizeof(struct partition), 4, fs);
   if (res != MAX_PARTITIONS) {
      fprintf(stderr, "Only read %ld partitions\n", res);
      exit(1);
   }

   // Check partition
   check_partition(&partitions[part], "p", part);

   // Partition table is good!
   fs_lFirst = partitions[part].lFirst;
   fs_size = partitions[part].size;

   if (subpart != -1) {
      // Make sure this partition is good
      check_mbr_sig(SECTOR_SIZE * fs_lFirst);

      // Seek to beginning of this partition
      if (fseek(fs, SECTOR_SIZE * fs_lFirst + PARTITION_TABLE, SEEK_SET)) {
         fprintf(stderr, "Error seeking to partition %d: %s\n", 
            part, strerror(errno));
         exit(1);
      }
      res = fread(partitions, sizeof(struct partition), 4, fs);
      if (res != MAX_PARTITIONS) {
         fprintf(stderr, "Only read %ld partitions\n", res);
         exit(1);
      }

      // Make sure subpart is good
      check_partition(&partitions[subpart], "sub", subpart);

      // Partition table is good!
      fs_lFirst = partitions[subpart].lFirst;
      fs_size = partitions[subpart].size;
   }
}

void find_file_r(struct inode *file, struct inode *base, char *path) {
   struct inode sub_file;
   size_t res, flen;
   struct direntry *file_entries;
   int i;

   // Skip leading /
   if (*path == '/')
      path ++;

   // If there's no more path, we're done!
   if (strlen(path) == 0) {
      memcpy(file, base, sizeof(struct inode));
      if (verbose)
         print_inode(file);
   }
   else {
      // If it's not a directory, we got a problem
      if ((base->mode & DIRECTORY) == 0) {
         fprintf(stderr, "Base not a directory: %s\n", path);
         exit(1);
      }

      // Otherwise check all sub-files
      file_entries = (struct direntry *)malloc(base->size);
      read_file((char *)file_entries, base);

      // Get the next slash if there is one
      flen = strcspn(path, "/");
      if (flen > MAX_FILENAME_LEN)
         errorAndExit("Filename too long: %d\n", flen);

      for (i = 0; i < base->size / sizeof(struct direntry); i ++) {
         // If inode is 0, then skip it
         if (file_entries[i].inode == 0)
            continue;

         if (flen == strlen((const char *)file_entries[i].name) &&
             !strncmp(path, (const char *)file_entries[i].name, flen)) {
            // Seek to the inode position
            long offset = fs_inodes + INODE_SIZE * (file_entries[i].inode - 1);
            if (fseek(fs, offset, SEEK_SET))
               errorAndExit("Error seeking to inode loc %d: %s\n", offset);
      
            res = fread(&sub_file, 1, sizeof(struct inode), fs);
            if (res != sizeof(struct inode))
               errorAndExit("inode only read %d bytes. Err: %s\n", res);
         
            // Move path pointer forward
            path += flen;
            find_file_r(file, &sub_file, path);
            return;
         }
      }

      // Nothing found
      printf("%s not found.\n", path);
      exit(1);
   }
}

/* Find the inode for a file */
void find_file(struct inode *file, char *path) {
   size_t res;

   // Read root inode
   if(fseek(fs, fs_inodes, SEEK_SET))
      errorAndExit("Error seeking to root inode location %d: %s\n", fs_inodes);

   res = fread(&root, 1, sizeof(struct inode), fs);
   if (res != sizeof(struct inode))
      errorAndExit("Root inode only read %d bytes.\n", res);

   memcpy(file, &root, sizeof(struct inode));

   find_file_r(file, &root, path);
}

void print_mode(int fd, uint16_t mode) {
   dprintf(fd, "%c%c%c%c%c%c%c%c%c%c", 
           mode & DIRECTORY ? 'd' : '-',
           mode & P_OWNER_READ ? 'r' : '-',
           mode & P_OWNER_WRITE ? 'w' : '-',
           mode & P_OWNER_EXEC ? 'x' : '-',
           mode & P_GROUP_READ ? 'r' : '-',
           mode & P_GROUP_WRITE ? 'w' : '-',
           mode & P_GROUP_EXEC ? 'x' : '-',
           mode & P_OTHER_READ ? 'r' : '-',
           mode & P_OTHER_WRITE ? 'w' : '-',
           mode & P_OTHER_EXEC ? 'x' : '-');
}

void read_file_async(int fd, struct inode *file) {
}
void read_file(char *buffer, struct inode *file) {
   unsigned int zone = 0, d_zone = 0;
   long buf_pos = 0;;
   uint32_t *ind_buffer, *double_buffer;
   uint32_t **double_buffers;
   size_t bytes_to_read, bytes_read;
   long offset;         
   size_t zones_per_zone = zone_size / sizeof(uint32_t);

   // Load ind buffer 
   if (file->indirect) {
      ind_buffer = (uint32_t *)malloc(zone_size);
      offset = zone_size * (file->indirect - fs_superblock.firstdata);
      
      if(fseek(fs, fs_zones + offset, SEEK_SET)) {
         fprintf(stderr, "Error seeking to  %ld: %s\n", 
            offset, strerror(errno));
         exit(1);
      }
      bytes_read = fread(ind_buffer, 1, zone_size, fs);
      if (bytes_read != zone_size) {
         fprintf(stderr, "Only read %ld bytes of ind zone\n", bytes_read);
         exit(1);
      }
   }
   if (file->two_indirect) {
      double_buffer = (uint32_t *)calloc(zones_per_zone, sizeof(uint32_t));
      double_buffers = (uint32_t **)calloc(zones_per_zone, sizeof(uint32_t *));
      offset = zone_size * (file->two_indirect - fs_superblock.firstdata);
      
      if(fseek(fs, fs_zones + offset, SEEK_SET)) {
         fprintf(stderr, "Error seeking to  %ld: %s\n", 
            offset, strerror(errno));
         exit(1);
      }
      bytes_read = fread(double_buffer, 1, zone_size, fs);
      if (bytes_read != zone_size) {
         fprintf(stderr, "Only read %ld bytes of ind2 zone\n", bytes_read);
         exit(1);
      }

      for(d_zone = 0; d_zone < zones_per_zone; d_zone ++) {
         if (double_buffer[d_zone]) {
            double_buffers[d_zone] = (uint32_t *)calloc(zones_per_zone, 
               sizeof(uint32_t));
            offset = zone_size * (double_buffer[d_zone] - 
               fs_superblock.firstdata);
            
            if(fseek(fs, fs_zones + offset, SEEK_SET)) {
               fprintf(stderr, "Error seeking to  %ld: %s\n", offset, 
                  strerror(errno));
               exit(1);
            }
            bytes_read = fread(double_buffers[d_zone], 1, zone_size, fs);
            if (bytes_read != zone_size) {
               fprintf(stderr, "Only read %ld bytes of ind2 zone\n", 
                  bytes_read);
               exit(1);
            }
         }
      }
   }

   zone = 0;
   while (buf_pos < file->size) {
      bytes_to_read = file->size > zone_size ? zone_size : file->size;
      if (buf_pos + bytes_to_read > file->size)
         bytes_to_read = file->size - buf_pos;

      if (zone < DIRECT_ZONES) {
         offset = zone_size * (file->zone[zone] - fs_superblock.firstdata);

         if (file->zone[zone] == 0) {
            memset(&buffer[buf_pos], 0, bytes_to_read);
            buf_pos += bytes_to_read;
            bytes_to_read = 0;
         }
      }
      // Is it in the first indirect?
      else if(zone < DIRECT_ZONES + zones_per_zone) {
         // Make sure indirect exists
         if (file->indirect == 0) {
            memset(&buffer[buf_pos], 0, bytes_to_read);
            buf_pos += bytes_to_read;
            bytes_to_read = 0;
         }
         else {
            offset = zone_size * (ind_buffer[zone - DIRECT_ZONES] -
               fs_superblock.firstdata);

            if (ind_buffer[zone - DIRECT_ZONES] == 0) {
               memset(&buffer[buf_pos], 0, bytes_to_read);
               buf_pos += bytes_to_read;
               bytes_to_read = 0;
            }
         }
      }
      else if (file->two_indirect && zone < DIRECT_ZONES + (zones_per_zone + 1) 
         * zones_per_zone) {
         d_zone = zone - (DIRECT_ZONES + zones_per_zone);

         if (double_buffers[d_zone / zones_per_zone] == 0) {
            memset(&buffer[buf_pos], 0, bytes_to_read);
            buf_pos += bytes_to_read;
            bytes_to_read = 0;
         }
         else {
            offset = double_buffers[d_zone / zones_per_zone]
               [d_zone % zones_per_zone];

            if (offset == 0) {
               memset(&buffer[buf_pos], 0, bytes_to_read);
               buf_pos += bytes_to_read;
               bytes_to_read = 0;
            }

            offset = zone_size * (offset - fs_superblock.firstdata);
         }
      }
      else {
         memset(&buffer[buf_pos], 0, bytes_to_read);
         buf_pos += bytes_to_read;
         bytes_to_read = 0;
      }

      if (bytes_to_read) {
         if(fseek(fs, fs_zones + offset, SEEK_SET)) {
            fprintf(stderr, "Error on zone %d seeking to loc %d: %s\n", zone,
               file->zone[zone], strerror(errno));
            exit(1);
         }
         bytes_read = fread(&buffer[buf_pos], 1, bytes_to_read, fs);

         if (bytes_read != bytes_to_read) {
            fprintf(stderr, "Didn't read enough bytes for zone %d: %ld / %ld\n",
               zone, bytes_read, bytes_to_read);
            exit(1);
         }

         buf_pos += bytes_read;
      }
      zone ++;
   }
}

void print_inode_line(int fd, struct inode *file, unsigned char *name) {
   print_mode(fd, file->mode);

   dprintf(fd, " %9d %s\n", file->size, name);
}

void print_dir(int fd, struct inode *dir) {
   struct direntry *file_entries;
   struct inode file;
   size_t res;

   read_file((char *)file_entries, dir);
      
      while (read(file_reader[0], &file_entry, sizeof(struct direntry)) > 0) {
         // If inode is 0, then skip it
         if (file_entry.inode == 0)
            continue;

         // Seek to the inode position
         long offset = fs_inodes + INODE_SIZE * (file_entry.inode - 1);
         if (fseek(fs, offset, SEEK_SET))
            errorAndExit("Error seeking to inode location %d: %s\n", offset);

         res = fread(&file, 1, sizeof(struct inode), fs);
         if (res != sizeof(struct inode))
            errorAndExit("inode only read %d bytes. Err: %s\n", res);

         print_inode_line(fd, &file, file_entry.name);
      }
}

/* Print the contents of a file (or directory) */
void print_file(int fd, char *path, int directory) {
   // Zone-size buffer to read the file
   struct inode file;

   find_file(&file, path);
   // Is it a directory?
   if (directory) {
      if (file.mode & DIRECTORY) {
         dprintf(fd, "%s:\n", path);
         print_dir(fd, &file);
      }
      else {
         print_inode_line(fd, &file, (unsigned char *)path);         
      }
   }
   else if (file.mode & REGULAR_FILE && !directory) {
      read_file(fd, &file);
   }
   else {
      fprintf(stderr, "%s is a directory!\n", path);
      exit(1);
   }
}

void print_inode(struct inode *inode) {
   int i;
   dprintf(0, "\nFile inode:\n"
              "  uint16_t mode  \t0x%04x (", inode->mode);
   print_mode(0, inode->mode);
   dprintf(0, ")\n"
              "  uint16_t links \t%d\n"
              "  uint16_t uid   \t%d\n"
              "  uint16_t gid   \t%d\n"
              "  uint32_t size  \t%d\n"
              "  int32_t atime  \t%d --- %s\n"
              "  int32_t mtime  \t%d --- %s\n"
              "  int32_t ctime  \t%d --- %s\n"
              "\n  Direct zones:\n",
              inode->links,
              inode->uid, inode->gid, inode->size,
              inode->atime, ctime((time_t *)&inode->atime), 
              inode->mtime, ctime((time_t *)&inode->mtime),
              inode->ctime, ctime((time_t *)&inode->ctime));
   for(i = 0; i < DIRECT_ZONES; i ++) {
      printf("               zone[%d]  =  %d\n", i, inode->zone[i]);
   }
   printf("  uint32_t indirect\t   %d\n"
          "  uint32_t double\t   %d\n",
          inode->indirect, inode->two_indirect);
}

void print_superblock(struct superblock *superblock) {
   printf("\nSuperblock contents:\n"
          "Stored Fields:\n"
          "  ninodes:\t %d\n"
          "  i_blocks:\t %d\n"
          "  z_blocks:\t %d\n"
          "  firstdata:\t %d\n"
          "  log_zone_size\t %d (zone size: %d)\n"
          "  max_file:\t %u\n"
          "  magic:\t %04x\n"
          "  zones:\t %d\n"
          "  blocksize:\t %d\n"
          "  subversion:\t %d\n",
          superblock->ninodes, 
          superblock->i_blocks, 
          superblock->z_blocks, 
          superblock->firstdata, 
          superblock->log_zone_size, 
          superblock->blocksize << superblock->log_zone_size,
          superblock->max_file, 
          superblock->magic, 
          superblock->zones, 
          superblock->blocksize, 
          superblock->subversion); 
}

/* Error Checking */
void check_mbr_sig(long offset) {
   uint8_t signature[2];

   // Check partition table before proceeding
   if (fseek(fs, offset + PARTITION_TABLE_SIG, SEEK_SET) != 0) {
      fprintf(stderr, "Error checking parition table: %s\n", strerror(errno));
      exit(1);
   }
   fread(signature, sizeof(uint8_t), PARTITION_TABLE_SIG_LENGTH, fs);
   if (feof(fs) || ferror(fs)) {
      fprintf(stderr, "Error reading partition table signature\n");
      exit(1);
   }
   if (signature[0] != BOOT_SECTOR_510 || signature[1] != BOOT_SECTOR_511) {
      fprintf(stderr, "Partition table signature not correct: 0x");
      fprintf(stderr, "%02X%02X\n", signature[0], signature[1]);
      exit(1);
   }
}

void check_partition(struct partition *partition, const char *prefix, int n) {
   if (partition->type != MINIX_PARTITION_TYPE) {
      fprintf(stderr, "Partition %s%d is not for Minix. Type=%02x\n", prefix, n,
         partition->type);
      exit(1);
   }
}
