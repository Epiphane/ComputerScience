#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "LZWCmp.h"
#include "SmartAlloc.h"

void LZWFreeTree(void *cst, TreeNode *node);
void LZWInitTree(LZWCmp *cmp);
void LZWCmpEncode(LZWCmp *cmp, UChar sym);

static TreeNode *FreeList = NULL;

void LZWFreeTree(void *cst, TreeNode *node) {
   if (node->right)
      LZWFreeTree(cst, node->right);
   if (node->left)
      LZWFreeTree(cst, node->left);
   
   FreeCode(cst, node->cNum);
   node->right = FreeList;
   FreeList = node;
}

TreeNode *NewNode() {
   TreeNode *result;
   
   if (FreeList) {
      result = FreeList;
      FreeList = FreeList->right;
      result->right = result->left = NULL;
   }
   else
      result = calloc(1, sizeof(TreeNode));  

   return result;
}

TreeNode *LZWAddCode(LZWCmp *cmp, UChar sym) {
   TreeNode *result = NewNode();

   result->cNum = ExtendCode(cmp->cst, cmp->curLoc->cNum);
   GetCode(cmp->cst, result->cNum);
   SetSuffix(cmp->cst, result->cNum, sym);

   return result;
}

void PrintTreeInOrder(LZWCmp *cmp, TreeNode *node) {
   Code thisCode;
   int index = 0;
   
   if (node) {
      PrintTreeInOrder(cmp, node->left);
      
      thisCode = GetCode(cmp->cst, node->cNum);
      printf("|%d", thisCode.data[index++]);
      while (index < thisCode.size)
         printf(" %d", thisCode.data[index++]);
      FreeCode(cmp->cst, node->cNum);

      PrintTreeInOrder(cmp, node->right);
   }
}

void LZWResetTree(LZWCmp *cmp) {
   LZWFreeTree(cmp->cst, cmp->root);
   DestroyCodeSet(cmp->cst);
   cmp->cst = CreateCodeSet(cmp->recycleCode + 1);
   LZWInitTree(cmp);
}

void LZWInitTree(LZWCmp *cmp) {
   int charNum;
   TreeNode *loc;
   
   loc = cmp->root = NewNode();
   loc->cNum = NewCode(cmp->cst, 0);
   
   for (charNum = 1; charNum < NUM_SYMS; charNum++) {
      loc->right = NewNode();
      loc = loc->right;
      loc->cNum = NewCode(cmp->cst, charNum);
      GetCode(cmp->cst, charNum);
   }

   NewCode(cmp->cst, 0);
   
   cmp->numBits = 0;
   while (1 << cmp->numBits++ < NUM_SYMS)
      ;
  
   cmp->maxCode = (1 << cmp->numBits) - 1;
}

void LZWCmpInit(LZWCmp *cmp, CodeSink sink, void *sinkState, int recycleCode,
 int traceFlags) {
   cmp->sink = sink;
   cmp->sinkState = sinkState;
   cmp->recycleCode = recycleCode;
   cmp->traceFlags = traceFlags;
   cmp->pCodeLimit = cmp->bitsUsed = 0;
   cmp->pCode.size = SIZE_INCR;
   cmp->curLoc = NULL;
   
   cmp->cst = CreateCodeSet(recycleCode + 1);
   LZWInitTree(cmp);
   
   cmp->pCode.data = malloc(SIZE_INCR);

}

int CompareCodes(LZWCmp *cmp, Code code2) {
   int s1 = cmp->pCodeLimit, s2 = code2.size;
   unsigned char *c1 = cmp->pCode.data - 1, *c2 = code2.data - 1;
   
   while (s1-- && s2-- && *++c1 == *++c2)
      ; 
   
   if (++s1)
      s1--;

   assert(cmp->pCode.data <= c1); 

   return (*c1 == *c2) ? s1 - s2 : *c1 - *c2;
}

void LZWSendCode(LZWCmp *cmp, UInt code) {
   UInt mask = (1 << cmp->numBits + 1) - 1;
   int shiftBack;
   int intSize = sizeof(UInt) * BITS_PER_BYTE;

   if (cmp->traceFlags & TRACE_CODES)
      printf("Sending code %d\n", code);
   if (cmp->traceFlags & TRACE_TREE) {
      PrintTreeInOrder(cmp, cmp->root);
      printf("|\n\n");
   }

   cmp->bitsUsed += cmp->numBits;

   if (cmp->bitsUsed > intSize) {
      cmp->bitsUsed -= intSize;
      cmp->nextInt = cmp->nextInt << cmp->numBits - cmp->bitsUsed;
      cmp->nextInt |= code >> cmp->bitsUsed;

      cmp->sink(cmp->sinkState, cmp->nextInt, FALSE);
      cmp->nextInt = code;
   }
   else {
      cmp->nextInt <<= cmp->numBits;
      cmp->nextInt |= code;

   }
   
   if (code == NUM_SYMS)
      cmp->sink(cmp->sinkState, cmp->nextInt << intSize - cmp->bitsUsed, FALSE);
}

void LZWSendCurrentCode(LZWCmp *cmp) {
   int lastChar = cmp->pCode.data[--cmp->pCodeLimit];
   
   LZWSendCode(cmp, cmp->curLoc->cNum);
   
   cmp->pCodeLimit = sizeof(UChar);
   *cmp->pCode.data = lastChar;

   cmp->curLoc = cmp->root;
}

/* Encode sym, moving down the tree.
 * If no match is found, add a leaf and sink the current code.
 */
void LZWCmpEncode(LZWCmp *cmp, UChar sym) {
   int compare, leafAdded = FALSE;
   char *temp;
   UChar lastChar;
   TreeNode *loc;

   if (!cmp->curLoc) 
      cmp->curLoc = cmp->root;

   loc = cmp->curLoc;
   cmp->curCode = GetCode(cmp->cst, loc->cNum);

   if (cmp->pCode.size == cmp->pCodeLimit) {
      temp = malloc(cmp->pCode.size + SIZE_INCR);
      memcpy(temp, cmp->pCode.data, cmp->pCode.size);
      cmp->pCode.size += SIZE_INCR;
      
      free(cmp->pCode.data);
      cmp->pCode.data = temp;
   }
      
   cmp->pCode.data[cmp->pCodeLimit++] = sym;

   while (!leafAdded && (compare = CompareCodes(cmp, cmp->curCode))) {
      FreeCode(cmp->cst, loc->cNum);
      if (compare > 0) {
         if (!loc->right) {
            loc->right = LZWAddCode(cmp, sym);
            leafAdded = TRUE;
         }

         loc = loc->right;
      }
      else {
         if (!loc->left) {
            loc->left = LZWAddCode(cmp, sym);
            leafAdded = TRUE;
         }

         loc = loc->left;
      }
      cmp->curCode = GetCode(cmp->cst, loc->cNum);
   }
   FreeCode(cmp->cst, loc->cNum);
   
   if (leafAdded) {
      LZWSendCurrentCode(cmp);
         
      if (loc->cNum > cmp->maxCode) {
         if (loc->cNum == cmp->recycleCode) {   
            if (cmp->traceFlags & TRACE_RECYCLES)
               printf("Recycling dictionary...\n"); 
            LZWResetTree(cmp);
            cmp->curLoc = cmp->root;
         }
         else { 
            cmp->numBits++;
            
            cmp->maxCode = (1 << cmp->numBits) - 1;
            if (cmp->traceFlags & TRACE_BUMPS)
               printf("Bump numBits to %d\n", cmp->numBits);
         }
      }

      while (compare = CompareCodes(cmp, 
       GetCode(cmp->cst, cmp->curLoc->cNum))) {
         FreeCode(cmp->cst, cmp->curLoc->cNum);
         cmp->curLoc = compare > 0 ? cmp->curLoc->right : cmp->curLoc->left;
      }
      FreeCode(cmp->cst, cmp->curLoc->cNum);
   }
   else
      cmp->curLoc = loc;
}

void LZWCmpStop(LZWCmp *cmp) {
   if (cmp->curLoc) {
      LZWSendCode(cmp, cmp->curLoc->cNum);
   }
   LZWSendCode(cmp, NUM_SYMS);
   LZWFreeTree(cmp->cst, cmp->root);
   
   cmp->sink(cmp->sinkState, 0, TRUE);
}

void LZWCmpDestruct(LZWCmp *cmp) {
   if (cmp->pCode.data)
      free(cmp->pCode.data);
   
   DestroyCodeSet(cmp->cst);
}

void LZWCmpClearFreelist() {
   TreeNode *temp;   
   
   while (temp = FreeList) {
      FreeList = temp->right;
      free(temp);
   }
}
