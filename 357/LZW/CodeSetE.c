#include <stdlib.h>
#include <string.h>
#include "SmartAlloc.h"
#include "CodeSet.h"

typedef struct CodeEntry {
   struct CodeEntry *prev;
   char *exportedLink;
   char *associatedBlock;
   char data;
   unsigned short usedBy;
} CodeEntry;

typedef struct CodeSet {
   CodeEntry *entries;
   int ndx;
   int size;
} CodeSet;

/* Allocate, initialize, and return a CodeSet object, via void *
 * The CodeSet will have room for |numCodes| codes, though it will
 * initially be empty. */ 
void *CreateCodeSet(int numCodes) {
   CodeSet *result;

   result = malloc(sizeof(CodeSet));
   result->entries = malloc(numCodes * sizeof(CodeEntry));
   result->ndx = 0;
   result->size = numCodes;

   return result;
}

/* Add a new 1-byte code to |codeSet|, returning its index, with
 * the first added code having index 0.  The new code's byte is
 * equal to |val|.  Assume (and assert if needed) that there
 * is room in the |codeSet| for a new code. */
int NewCode(void *codeSet, char val) {  
   int result;
   CodeSet *currSet = (CodeSet *) codeSet;
   CodeEntry *newEntry = currSet->entries + currSet->ndx;

   newEntry->data = val;
   newEntry->prev = NULL;
   newEntry->exportedLink = NULL;
   newEntry->usedBy = 0;

   return currSet->ndx++;
}

/* Create a new code by extending the existing code at index
 * by one zero-valued byte, using the linked structure from spec.  Any
 * existing code might be extended, not just the most recently
 * added one. Return the new code's index.  Assume |oldCode| 
 * is a valid index, and that there is enough room for 
 * a new code, but assert both of these conditions as well */
int ExtendCode(void *codeSet, int oldCode) {
   CodeSet *currSet = (CodeSet *) codeSet;
   CodeEntry *oldEntry = currSet->entries + oldCode;
   CodeEntry *newEntry = currSet->entries + currSet->ndx;

   newEntry->data = 0;
   newEntry->prev = oldEntry;
   newEntry->exportedLink = NULL;
   newEntry->usedBy = 0;

   return currSet->ndx++;
}

/* Set the final byte of the code at index |code| to |suffix|.  
 * This is used to override the zero-byte added by extendCode. 
 * If the code in question has been returned by a getCode call,
 * and not yet freed via freeCode, then the changed final byte
 * will also show in the Code data that was returned from getCode.*/
void SetSuffix(void *codeSet, int code, char suffix) {   
   CodeSet *currSet = (CodeSet *) codeSet;
   CodeEntry *entry = currSet->entries + code;

   entry->data = suffix;
   if (entry->exportedLink)
      *(entry->exportedLink) = suffix;
}

/* Return the code at index |code| */
Code GetCode(void *codeSet, int code) {  
   CodeSet *currSet = (CodeSet *) codeSet;
   CodeEntry *entry = currSet->entries + code;
   CodeEntry *cursor = entry;
   Code result;
   int size = 1;

   // Figure out the length of the code we were just asked to get
   while (cursor->prev) {
      size++;
      cursor = cursor->prev;
   }
   result.size = size;

   // Check if the CodeEntry is already in use
   if (entry->usedBy) {
      // Grab the already existing code block
      result.data = entry->associatedBlock;
   }
   else {
      // Allocate a new space to hold the NEW Code data   
      cursor = entry;
      result.data = malloc(size);

      while (size--) {
         *(result.data + size) = cursor->data;
         cursor->exportedLink = result.data + size;
         cursor = cursor->prev;
      }

      // Tell the CodeEntry where its associated Code is
      entry->associatedBlock = result.data;
   }

   // Inform the CodeEntry that it has just been USED
   entry->usedBy++;

   return result;
}

/* Mark the code at index |code| as no longer needed, until a new
 * getCode call is made for that code. */
void FreeCode(void *codeSet, int code) {
   CodeSet *currSet = (CodeSet *) codeSet;
   CodeEntry *cursor = currSet->entries;
   int i;

   for (i = 0; i < currSet->ndx; i++) {
      if (cursor->usedBy)
         free(cursor->associatedBlock);
      
      cursor++;
   }
}

/* Free all dynamic storage associated with |codeSet| */
void DestroyCodeSet(void *codeSet) { 
   CodeSet *currSet = (CodeSet *) codeSet;
   CodeEntry *cursor = currSet->entries;

   free(currSet->entries);   
   free(codeSet);
}
