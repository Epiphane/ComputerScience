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
#include <stdlib.h>

#include "malloc.h"

// Default page size is 64kb
#define PAGE_SIZE 64 * 1024
// Pointers must line up mod 16
#define ALIGNMENT 0xf

typedef struct Block {
  size_t size;
  struct Block *next;
} Block;

/* Globals keeping track of the start of the heap and the end */
Block *firstBlock = NULL;
void *heapEnd = NULL;

/* Prints information about a pointer */
void debugPointer(void *ptr, size_t size) {
  fprintf(stderr, "(ptr=%p, size=%zu)\n", ptr, size);
}

/*
 * Allocated PAGE_SIZE more space from the heap, and returns a pointer
 * to this new section of memory.
 *
 * Also updates heapEnd (global) to keep track of the new end of the heap.
 */
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

/*
 * Arbitrarily moves heapEnd. Useful for un-allocating heap space if malloc
 * fails.
 */
void setHeapTo(void *_heapEnd) {
  brk(_heapEnd);

  if(errno) {
    fprintf(stderr, "Memory deallocation failed: %s", strerror(errno));
    return;
  }

  heapEnd = _heapEnd;
}

/* Memory functions */
/*
 * The 'brains' behind malloc.
 * Finds an unallocated space in the heap, allocates it, and returns
 * a pointer to the head of the data Block.
 *
 * NOTE: This is not a pointer to the data itself. Functions that use this
 * must be sure to increment by sizeof(Block) to get the data section.
 */
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
  }

  // Now, look through blocks for a space our size
  int foundSpot = 0;
  while(cursor->next && !foundSpot) {
    // Attempt to place block after this one
    newBlock = (void *) cursor + sizeof(Block) + cursor->size;
   
    // Fix alignment
    void *memory = (void *) newBlock + sizeof(Block);
    if((long) memory & ALIGNMENT) {
      memory = (void *) (((long) memory + ALIGNMENT) & ~ALIGNMENT);
      newBlock = memory - sizeof(Block);
    }

    if(newBlock + size < cursor->next) {
      newBlock->size = size;

      newBlock->next = cursor->next;
      
      cursor->next = newBlock;
      foundSpot = 1; // Mark that we're done!
    }
    // Otherwise, continue searching...
    cursor = cursor->next;
  }

  if(!foundSpot) {
    // If we get here, no suitable space has been found. Stick it after the end
    cursor->next = newBlock = (void *) cursor + sizeof(Block) + cursor->size;

    // Fix alignment
    void *memory = (void *) newBlock + sizeof(Block);
    if((long) memory & ALIGNMENT) {
      memory = (void *) (((long) memory + ALIGNMENT) & ~ALIGNMENT);
      cursor->next = newBlock = memory - sizeof(Block);
    }
   
    while((void *) newBlock + size + sizeof(Block) >= heapEnd) {
      void *success = allocPage(PAGE_SIZE);
      if(success == NULL) {
        // If we had an issue, free all the pages we allocated for this
        setHeapTo(oldHeapEnd);

        // If there was an error, return null and set errno
        errno = ENOMEM;
        return NULL;
      }
    }

    newBlock->size = size;
    newBlock->next = NULL;
  }

  return newBlock;
}

/*
 * Allocate space for n members of size size, and initialize space to 0
 */
void *calloc(size_t nmemb, size_t size) {
  Block *block = alloc(nmemb * size);
  void *memory = (void *) block + sizeof(Block);

  memset(memory, 0, nmemb * size);

  if(getenv("DEBUG_MALLOC")) {
    fprintf(stderr, "MALLOC: calloc(%zu, %zu)\t\t=> ", nmemb, size);
    debugPointer(memory, block->size);
  }
  return memory;
}

/*
 * Allocate space (uninitialized)
 */
void *malloc(size_t size) {
  Block *block = alloc(size);
  void *memory = (void *) block + sizeof(Block);

  if(getenv("DEBUG_MALLOC")) {
    fprintf(stderr, "MALLOC: malloc(%zu)\t\t=> ", size);
    debugPointer(memory, block->size);
  }
  return memory;
}

/*
 * Given a pointer to data space, reallocate it to fit a different data size
 */
void *realloc(void *ptr, size_t size) {
  if(ptr == NULL) {
    return malloc(size);
  }
  if(size == 0) {
    free(ptr);
  }

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
  if(size <= oldSize ||
     (block->next == NULL && ptr + sizeof(Block) + oldSize < heapEnd) ||
     (Block *) (ptr + sizeof(Block) + oldSize) < block->next) {
    block->size = size;
  }
  // Otherwise, we'll need a larger spot
  else {
    // Free my block
    free(ptr);
    
    // Get new spot and copy data to it
    block = alloc(size);
    memory = (void *) block + sizeof(Block);
    memcpy(memory, ptr, oldSize);

    // Fix pointer
    ptr = memory;
  }

  if(getenv("DEBUG_MALLOC")) {
    fprintf(stderr, "MALLOC: realloc(%p, %zu)\t=> ", ptr, size);
    debugPointer(memory, block->size);
  }

  return ptr;
}

/*
 * Free allocated data
 */
void free(void *ptr) {
  if(ptr == NULL)
    return;

  // Pfft, you can't free before you even allocate anything!
  if(!firstBlock) {
    errno = EFAULT;
    return;
  }

  // Set up a pointer to our data header
  Block *block = ptr - sizeof(Block);

  // Set up a cursor to look for this block (must have been alloced before)
  Block *cursor = firstBlock;
  while(cursor->next && cursor->next < block) {
    cursor = cursor->next;
  }

  // Found our block
  if(cursor->next == block) {
    cursor->next = block->next;
  }

  // Erase me from everybody's memory
  if(getenv("DEBUG_MALLOC"))
    fprintf(stderr, "MALLOC: free(%p)\n", ptr);
}
