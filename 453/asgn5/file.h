
/*
 * minget and minls - Minix file management 
 *
 * CPE 453 Assignment 5
 *
 * Author: Thomas Steinke (tsteinke@calpoly.edu)
 */
#include <stdio.h>
#include <stdint.h>

/* Partition table entry */
#define MAX_PARTITIONS 4
#define PARTITION_TABLE 0x1BE
#define PARTITION_TABLE_SIG 510
#define PARTITION_TABLE_SIG_LENGTH 2
#define MINIX_PARTITION_TYPE 0x81
#define BOOT_SECTOR_510 0x55
#define BOOT_SECTOR_511 0xAA
#define SECTOR_SIZE 512
struct partition {
   uint8_t bootind; /* Boot magic number 0x80 if bootable */
   uint8_t start_head; /* Start of partition in CHS */
   uint8_t start_sec;
   uint8_t start_cyl;
   uint8_t type; /* Type of partition */
   uint8_t end_head; /* End of partition in CHS */
   uint8_t end_sec;
   uint8_t end_cyl;
   uint32_t lFirst; /* First sector (LBA addressing) */
   uint32_t size; /* Size of partition in sectors */
};

/* Superblock format */
#define SUPERBLOCK_LOCATION 1024
#define MINIX_MAGIC_NUMBER 0x4D5A
#define MINIX_MAGIC_NUMBER_REVERSE 0x5A4D
#define IMAP_BLOCK_OFFSET 2
struct superblock { /* Minix Version 3 Superblock
                     * this structure found in fs/super.h
                     * * in minix 3.1.1
                     */
   /* on disk. These fields and orientation are non–negotiable */
   uint32_t ninodes; /* number of inodes in this filesystem */
   uint16_t pad1; /* make things line up properly */
   int16_t i_blocks; /* # of blocks used by inode bit map */
   int16_t z_blocks; /* # of blocks used by zone bit map */
   uint16_t firstdata; /* number of first data zone */
   int16_t log_zone_size; /* log2 of blocks per zone */
   int16_t pad2; /* make things line up again */
   uint32_t max_file; /* maximum file size */
   uint32_t zones; /* number of zones on disk */
   int16_t magic; /* magic number */
   int16_t pad3; /* make things line up again */
   uint16_t blocksize; /* block size in bytes */
   uint8_t subversion; /* filesystem sub–version */
};

/* Inode format */
#define DIRECT_ZONES 7
#define INODE_SIZE 64
#define DIRECTORY_SIZE 64
struct inode {
   uint16_t mode; /* mode */
   uint16_t links; /* number or links */
   uint16_t uid;
   uint16_t gid;
   uint32_t size;
   int32_t atime;
   int32_t mtime;
   int32_t ctime;
   uint32_t zone[DIRECT_ZONES];
   uint32_t indirect;
   uint32_t two_indirect;
   uint32_t unused;
};

/* Directory entry */
#define MAX_FILENAME_LEN 60
struct direntry {
   uint32_t inode;
   unsigned char name[MAX_FILENAME_LEN];
};

/* File types */
#define FILE_TYPE_MASK 0170000
#define REGULAR_FILE   0100000
#define DIRECTORY      0040000
#define P_OWNER_READ   0000400
#define P_OWNER_WRITE  0000200
#define P_OWNER_EXEC   0000100
#define P_GROUP_READ   0000040
#define P_GROUP_WRITE  0000020
#define P_GROUP_EXEC   0000010
#define P_OTHER_READ   0000004
#define P_OTHER_WRITE  0000002
#define P_OTHER_EXEC   0000001

/* Functions defined in file.c */
void open_fs(char *name, int partition, int subpartition, int verbose);
struct inode *find_file(char *path);
int read_file(char *buf, struct inode *file, unsigned int zone);
