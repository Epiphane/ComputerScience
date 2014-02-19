#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SmartAlloc.h"

#define IDIM1 10
#define IDIM2 4

// Function |insert| inserts block of ints |toInsert|, having |dim2| elements, into the
// middle of block of ints |blk|, which has |dim1| elements.  The inserted block starts at 
// offset |offset|.  Data in |blk| are moved down to make room; no data is lost.  
// |insert| makes a larger block to accommodate the inserted data, and returns a pointer 
// to this larger block, which the caller is expected to use instead of |blk|.
int *insert(int *blk, int *toInsert, int dim1, int dim2, int offset) {
   int *to, *finalBlk;
   to = finalBlk = malloc(sizeof(int) * dim1 * dim2);
   
   memcpy(to, blk, sizeof(int) * offset);
   to += offset;
   memcpy(to, toInsert, sizeof(int) * dim2);
   memcpy(to + dim2, blk + offset, sizeof(int) * dim1); 

   free(blk);
   return finalBlk;
}

// End code

void scanInts(int *arr, int size) {
   int i;
   
   for (i = 0; i < size; i++)
      scanf("%d", arr+i);
}

void printInts(char *name, int *arr, int size) {
   int i;
   
   printf("%s:", name);
   for (i = 0; i < size; i++)
      printf(" %d", arr[i]);
   printf("\n");
}

int main() {
   int *iArr1 = malloc(sizeof(int) * IDIM1);
   int *iArr2 = malloc(sizeof(int) * IDIM2);
   
   scanInts(iArr1, IDIM1);
   scanInts(iArr2, IDIM2);
   iArr1 = insert(iArr1, iArr2, IDIM1, IDIM2, IDIM1/2);
   iArr2 = insert(iArr2, iArr1, IDIM2, IDIM1 + IDIM2, IDIM2/2);

   printInts("iArr1", iArr1, IDIM1 + IDIM2);
   printInts("iArr2", iArr2, IDIM1 + 2*IDIM2);
   
   free(iArr1);
   free(iArr2);
   printf("Final space: %d\n", report_space());
}
