#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "SmartAlloc.h"
#include "CodeSet.h"

typedef struct CodeEntry {
   struct CodeEntry *prior;
   char *activeCode;
   int calls;
   char suffix;
} CodeEntry;

typedef struct CodeSet {
   CodeEntry *codes;
   int numCodes;
   int derp;
} CodeSet;

/* Allocate, initialize, and return a CodeSet object, via void *
 * The CodeSet will have room for |numCodes| codes, though it will
 * initially be empty. */ 
void *createCodeSet(int numCodes) {
   CodeSet *codeSet = calloc(1, sizeof(CodeSet));
   codeSet->codes = calloc(numCodes, sizeof(CodeEntry));

   return codeSet;
}

/* Add a new 1-byte code to |codeSet|, returning its index, with
 * the first added code having index 0.  The new code's byte is
 * equal to |val|.  Assume (and assert if needed) that there
 * is room in the |codeSet| for a new code. */
int newCode(void *codeSet, char val) {
   CodeSet *set = (CodeSet *)codeSet;

   CodeEntry *newCode = set->codes + set->numCodes;
   newCode->suffix = val;
   
   return set->numCodes++;
}

/* Create a new code by copying the existing code at index
 * |oldCode| and extending it by one zero-valued byte.  Any
 * existing code might be extended, not just the most recently
 * added one. Return the new code's index.  Assume |oldCode| 
 * is a valid index and that there is enough room for a new code. */
int extendCode(void *codeSet, int oldCode) {
   CodeSet *set = (CodeSet *)codeSet;

   CodeEntry *newCode = set->codes + set->numCodes;
   newCode->prior = set->codes + oldCode;
   
   return set->numCodes++;
}

/* Set the final byte of the code at index |code| to |suffix|.  
 * This is used to override the zero-byte added by extendCode. 
 * If the code in question has been returned by a getCode call,
 * and not yet freed via freeCode, then the changed final byte
 * will also show in the Code data that was returned from getCode.*/
void setSuffix(void *codeSet, int code, char suffix) {
   CodeSet *set = (CodeSet *)codeSet;
   
   CodeEntry *entry = set->codes + code;

   entry->suffix = suffix;
}

/* Return the code at index |code| */
Code getCode(void *codeSet, int code) {
   CodeSet *set = (CodeSet *)codeSet;
   CodeEntry *cursor, *thisCode;
   char *codeData;
   int size = 0;
   Code result;  

   thisCode = set->codes + code;
 
   // Calculate size of the code
   for(cursor = set->codes + code; cursor; cursor = cursor->prior)
      size ++;

   if(!thisCode->calls++)
      thisCode->activeCode = malloc(size);
   result.size = size;
   for(cursor = set->codes + code; size; cursor = cursor->prior) {
      // Set this part of the code
      thisCode->activeCode[--size] = cursor->suffix;
   }
   result.data = thisCode->activeCode;

   return result;
}

/* Mark the code at index |code| as no longer needed, until a new
 * getCode call is made for that code. */
void freeCode(void *codeSet, int code) {
   CodeSet *set = (CodeSet *)codeSet;
   CodeEntry *thisCode = set->codes + code;

   if(!--thisCode->calls)
      free(thisCode->activeCode);
}

/* Free all dynamic storage associated with |codeSet| */
void destroyCodeSet(void *codeSet) {
   CodeSet *set = (CodeSet *)codeSet;
   CodeEntry *entry = set->codes + set->numCodes;

   // Free all
   while(entry-- > set->codes)
      if(entry->calls)   
         free(entry->activeCode);
 
   free(set->codes); 
   free(set);
}
