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
  struct Block *next;
  struct Block *prev;
} Block;

Block *firstBlock = NULL;
void *heapEnd = NULL;

// Default page size is 64kb
const int PAGE_SIZE = 64 * 1024;

void debugPointer(void *ptr, size_t size) {
  printf("(ptr=%08x, size=%d)\n", ptr, size);
}

void *allocPage(int pageSize) {
  void *base = sbrk(pageSize);

  if(errno) {
    fprintf(stderr, "Memory allocation failed: %s", strerror(errno));
    return NULL;
  }

  // Keep track of where the page ends now
  heapEnd = base + pageSize;

  return base;
}

void setHeapTo(void *_heapEnd) {
  brk(_heapEnd);

  if(errno) {
    fprintf(stderr, "Memory deallocation failed: %s", strerror(errno));
    return;
  }

  heapEnd = _heapEnd;
}

/* Memory functions */
Block *alloc(size_t size) {
  Block *cursor = firstBlock;
  Block *newBlock;
  void *oldHeapEnd = heapEnd; // Keep track of the "before" size
  if(!firstBlock) {
    // Alloc our first page of memory
    cursor = firstBlock = allocPage(PAGE_SIZE);
    if(firstBlock == NULL) {
      // If there was an error, return null and set errno
      errno = ENOMEM;
      return NULL;
    }

    // Initialize first Block
    firstBlock->size = 0;
    firstBlock->next = NULL;
    firstBlock->prev = NULL;
  }

  // Now, look through blocks for a space our size
  int foundSpot = 0;
  while(cursor->next) {
    // Attempt to place block after this one
    newBlock = (void *) cursor + sizeof(Block) + cursor->size;
    if(newBlock + size < cursor->next) {
      newBlock->size = size;

      newBlock->prev = cursor;
      newBlock->next = cursor->next;
      
      cursor->next->prev = newBlock;
      cursor->next = newBlock;
      foundSpot = 1; // Mark that we're done!
    }
    // Otherwise, continue searching...
    cursor = cursor->next;
  }

  if(!foundSpot) {
    // If we get here, no suitable space has been found. Stick it after the end
    cursor->next = newBlock = (void *) cursor + sizeof(Block) + cursor->size;
    newBlock->size = size;
    newBlock->prev = cursor;
    newBlock->next = NULL;

    while((void *) newBlock + newBlock->size > heapEnd) {
      void *success = allocPage(PAGE_SIZE);
      if(success == NULL) {
        // If we had an issue, free all the pages we allocated for this
        setHeapTo(oldHeapEnd);

        // If there was an error, return null and set errno
        errno = ENOMEM;
        return NULL;
      }
    }
  }

  return newBlock;
}

void *calloc(size_t nmemb, size_t size) {
  Block *block = alloc(nmemb * size);
  void *memory = (void *) block + sizeof(Block);

  memset(memory, 0, nmemb * size);

  if(getenv("DEBUG_MALLOC")) {
    printf("MALLOC: calloc(%ld, %ld)\t=> ", nmemb, size);
    debugPointer(memory, block->size);
  }
  return memory;
}

void *malloc(size_t size) {
  Block *block = alloc(size);
  void *memory = (void *) block + sizeof(Block);

  if(getenv("DEBUG_MALLOC")) {
    printf("MALLOC: malloc(%ld)\t=> ", size);
    debugPointer(memory, block->size);
  }
  return memory;
}

void *realloc(void *ptr, size_t size) {
  // Pfft, you can't free before you even allocate anything!
  if(!firstBlock) {
    errno = EFAULT;
    return ptr;
  }

  // Set up a pointer to our data header
  Block *block = ptr - sizeof(Block);
  size_t oldSize = block->size;

  void *memory = ptr;
  // If we have space, just change the size
  if(size <= oldSize || ptr + oldSize < block->next) {
    block->size = size;
  }
  // Otherwise, we'll need a larger spot
  else {
    // Free my block
    free(block);
    // Get new spot and copy data to it
    block = alloc(size);
    memory = (void *) block + sizeof(Block);
    memcpy(memory, ptr, oldSize);

    // Fix pointer
    ptr = memory;
  }

  if(getenv("DEBUG_MALLOC")) {
    printf("MALLOC: realloc(%08x, %ld)\t=> ", ptr, size);
    debugPointer(memory, block->size);
  }

  return ptr;
}

void free(void *ptr) {
  // Pfft, you can't free before you even allocate anything!
  if(!firstBlock) {
    errno = EFAULT;
    return;
  }

  // Set up a pointer to our data header
  Block *block = ptr - sizeof(Block);

  // Erase me from everybody's memory
  block->prev->next = block->next;
  if(block->next)
    block->next->prev = block->prev;

  if(getenv("DEBUG_MALLOC"))
    printf("MALLOC: free(%08x)\n", ptr);
}
