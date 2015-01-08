/*
 * Malloc custom library
 * CPE 453 Assignment 1
 *
 * Thomas Steinke (tsteinke@calpoly.edu)
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "malloc.h"

typedef struct Block {
  size_t size;
  struct Block *nextBlock;
} Block;

Block *firstBlock = NULL;

// Default page size is 64kb
const int PAGE_SIZE = 64 * 1024;

void *allocPage(int pageSize) {
  void *success = sbrk(pageSize);

  return success;
}

void *calloc(size_t nmemb, size_t size) {
  return 0;
}

void *malloc(size_t size) {
  if(!firstBlock) {
    
  }

  return 0;
}

void free(void *ptr) {

}

void *realloc(void *ptr, size_t size) {
  return 0;
}
