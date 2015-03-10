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
#include <time.h>
#include <stdint.h>

#include "file.h"

FILE *fs;
uint32_t fs_lFirst;
uint32_t fs_size;
size_t fs_imap, fs_zmap, fs_inodes, fs_zones;
struct superblock fs_superblock;
struct inode root;

void errorAndExit(const char *err, int debug) {
   fprintf(stderr, err, debug, strerror(errno));
   exit(1);
}

void print_superblock(struct superblock *superblock);
void print_inode(struct inode *inode);

void open_fs(char *path, int part, int subpart, int verbose) {
   size_t res;

   fs = fopen(path, "r");
   if (fs == NULL) {
      fprintf(stderr, "Error opening %s: %s\n", path, strerror(errno));
      exit(1);
   }

   if (part == -1) { // NO partition
      fs_lFirst = 0;
      fs_size = 0; // TODO does it matter?
   }
   else {
      printf("I don't know how to read partitions cause I'm dumb!\n");
   }

   // Read in superblock!
   if (fseek(fs, fs_lFirst + SUPERBLOCK_LOCATION, SEEK_SET) != 0)
      errorAndExit("Error seeking to superblock location %d: %s\n", 
                   fs_lFirst + SUPERBLOCK_LOCATION);
   res = fread(&fs_superblock, 1, sizeof(struct superblock), fs);
   if (res != sizeof(struct superblock))
      errorAndExit("Superblock only read %d bytes.\n", res);

   // Confirm it's a minix filesystem
   if (fs_superblock.magic != MINIX_MAGIC_NUMBER)
      errorAndExit("Partition not a Minix filesystem (magic number: %d)\n", 
                   fs_superblock.magic);

   if (verbose) {
      print_superblock(&fs_superblock);
   }

   // Set references to each section
   fs_imap = fs_lFirst + fs_superblock.blocksize * IMAP_BLOCK_OFFSET;
   fs_zmap = fs_imap + fs_superblock.blocksize * fs_superblock.i_blocks;
   fs_inodes = fs_zmap + fs_superblock.blocksize * fs_superblock.z_blocks;
   fs_zones = fs_inodes + fs_superblock.ninodes * INODE_SIZE;
   if(fseek(fs, fs_inodes, SEEK_SET))
      errorAndExit("Error seeking to root inode location %d: %s\n", fs_inodes);
}

struct inode *find_file(char *path) {
   size_t res;
   struct inode *file = malloc(sizeof(file));

   // Read root inode
   res = fread(&root, 1, sizeof(struct inode), fs);
   if (res != sizeof(struct inode))
      errorAndExit("Root inode only read %d bytes.\n", res);

   print_inode(&root);

   return file;
}

void open_fs_part(char *path, int part, int subpart) {
   struct partition partitions[MAX_PARTITIONS];
   uint8_t signature[PARTITION_TABLE_SIG_LENGTH];

   // Check partition table before proceeding
   if (fseek(fs, PARTITION_TABLE + PARTITION_TABLE_SIG, SEEK_SET) != 0) {
      fprintf(stderr, "Error checking parition table: %s\n", strerror(errno));
      exit(1);
   }
   fread(signature, sizeof(uint8_t), PARTITION_TABLE_SIG_LENGTH, fs);
   printf("%ld\n", PARTITION_TABLE_SIG + PARTITION_TABLE);
   if (feof(fs) || ferror(fs)) {
      fprintf(stderr, "Error reading partition table signature\n");
      exit(1);
   }
   if (signature[0] != BOOT_SECTOR_510 || signature[1] != BOOT_SECTOR_511) {
      fprintf(stderr, "Partition table signature not correct: 0x");
      fprintf(stderr, "%02X%02X\n", signature[0], signature[1]);
      //exit(1);
   }

   // Load first partition table entry
   if (fseek(fs, PARTITION_TABLE, SEEK_SET) != 0) {
      fprintf(stderr, "Error loading partition table: %s\n", strerror(errno));
      exit(1);
   }
   fread((void *)&partitions[0], sizeof(struct partition), 0, fs);

   // Make sure this is a Minix filesystem
   if (partitions[0].type != MINIX_PARTITION_TYPE) {
      fprintf(stderr, "First partition is not a Minix partition\n");
      exit(1);
   }

   // Partition table is good!
   fs_lFirst = partitions[0].lFirst;
   fs_size = partitions[0].size;

   fseek(fs, fs_lFirst + 510, SEEK_SET);
   fread(signature, sizeof(uint8_t), PARTITION_TABLE_SIG_LENGTH, fs);
   printf("%ld\n", PARTITION_TABLE_SIG + PARTITION_TABLE);
   if (feof(fs) || ferror(fs)) {
      fprintf(stderr, "Error reading partition table signature\n");
      exit(1);
   }
   if (signature[0] != BOOT_SECTOR_510 || signature[1] != BOOT_SECTOR_511) {
      fprintf(stderr, "Partition table signature not correct: 0x");
      fprintf(stderr, "%02X%02X\n", signature[0], signature[1]);
      //exit(1);
   }
}

int read_file(char *buf, struct inode *file, unsigned int zone) {
   return 0;
}

void print_inode(struct inode *inode) {
   int i;
   printf("\nFile inode:\n"
          "  uint16_t mode  \t0x%04x (%s)\n"
          "  uint16_t links \t%d\n"
          "  uint16_t uid   \t%d\n"
          "  uint16_t gid   \t%d\n"
          "  uint32_t size  \t%d\n"
          "  int32_t atime  \t%d --- %s\n"
          "  int32_t mtime  \t%d --- %s\n"
          "  int32_t ctime  \t%d --- %s\n"
          "\n  Direct zones:\n",
          inode->mode, "drwxrwxrwxNO", inode->links,
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
          superblock->blocksize << fs_superblock.log_zone_size,
          superblock->max_file, 
          superblock->magic, 
          superblock->zones, 
          superblock->blocksize, 
          superblock->subversion); 
}
