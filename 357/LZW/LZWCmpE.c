#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include "LZWCmp.h"
#include "SmartAlloc.h"

#define INIT_NUMBITS 9
#define INIT_MAXCODE 511
#define INIT_TREESIZE 256
#define EOD 256
#define BITS_PER_SINK 32
#define MAX_PCODE_SIZE 1024

void TreeInsert(TreeNode *root, void *cst, int toInsert);
int TreeContains(TreeNode **root, void *cst, Code code);
TreeNode *TreeNodeInit();
void TreePrint(TreeNode *root, void *cst);
void InOrder(TreeNode *root, void *cst);
void TreeInit(LZWCmp *cmp);

void TreeFree(TreeNode *root);
void PostOrderFree(TreeNode *root);
void FreeNode(TreeNode *toFree);
void RecycleDictionary(LZWCmp *cmp, UChar sym);

int PackBits(LZWCmp *cmp, int bits);

static TreeNode *FreeList = NULL;

void LZWCmpInit(LZWCmp *cmp, CodeSink sink, void *sinkState, int recycleCode,
 int traceFlags) {
   cmp->sink = sink;
   cmp->sinkState = sinkState;
   cmp->recycleCode = recycleCode;
   cmp->traceFlags = traceFlags;
   cmp->numBits = INIT_NUMBITS;
   cmp->maxCode = INIT_MAXCODE;
   cmp->bitsUsed = 0;

   cmp->curLoc = NULL;

   TreeInit(cmp);

   cmp->pCode.size = 1;
  
   cmp->pCode.data = (char *) malloc(MAX_PCODE_SIZE); 
   cmp->nextInt = 0;
   cmp->curCode.size = MAX_PCODE_SIZE;
} 

void LZWCmpEncode(LZWCmp *cmp, UChar sym) {
   static int newCodeNdx, recycleReady;
   int recycled = 0;
   UChar *newData;

   if (cmp->curLoc == NULL) {
      cmp->curLoc = cmp->root;
      for (newCodeNdx = 0; newCodeNdx < sym; newCodeNdx++) 
         cmp->curLoc = cmp->curLoc->right;

      *(cmp->pCode.data) = sym;
      cmp->pCodeLimit = cmp->curLoc->cNum;
   }
   else {
      *(cmp->pCode.data + cmp->pCode.size) = sym;
      cmp->pCode.size++;

      if (cmp->pCode.size >= cmp->curCode.size) {
         free(cmp->pCode.data);
         cmp->pCode.data = malloc(cmp->curCode.size + MAX_PCODE_SIZE);
         cmp->curCode.size += MAX_PCODE_SIZE;
      }

      if (TreeContains(&cmp->curLoc, cmp->cst, cmp->pCode)) {
         cmp->pCodeLimit = cmp->curLoc->cNum;
      }
      else {
         newCodeNdx = ExtendCode(cmp->cst, cmp->pCodeLimit);
         SetSuffix(cmp->cst, newCodeNdx, sym);
         TreeInsert(cmp->root, cmp->cst, newCodeNdx);

         PackBits(cmp, cmp->pCodeLimit);

         if (newCodeNdx == cmp->recycleCode) {
            RecycleDictionary(cmp, sym);

            for (newCodeNdx = 0; newCodeNdx < sym; newCodeNdx++) 
               cmp->curLoc = cmp->curLoc->right;

            *(cmp->pCode.data) = sym;
            cmp->pCode.size = 1;
            cmp->pCodeLimit = cmp->curLoc->cNum;
         }

         if (!recycled && newCodeNdx == cmp->maxCode + 1) {
            cmp->maxCode = cmp->maxCode << 1;
            cmp->maxCode++;
            cmp->numBits++;
            if (cmp->traceFlags & TRACE_BUMPS)
               printf("Bump numBits to %d\n", cmp->numBits);
         }  

         if (!recycled) {
            cmp->curLoc = cmp->root;

            *(cmp->pCode.data) = sym;
            cmp->pCode.size = 1;
            cmp->pCodeLimit = sym;
         }
      }
   }
}

void RecycleDictionary(LZWCmp *cmp, UChar sym) {
   cmp->numBits = INIT_NUMBITS;
   cmp->maxCode = INIT_MAXCODE;

   FreeCode(cmp->cst, 0);
   DestroyCodeSet(cmp->cst);
   TreeFree(cmp->root);

   TreeInit(cmp);
   cmp->curLoc = cmp->root;

   if (cmp->traceFlags & TRACE_RECYCLES)
      printf("Recycling dictionary...\n");

   *(cmp->pCode.data) = sym;
   cmp->pCode.size = 1;
   cmp->pCodeLimit = cmp->curLoc->cNum;
}

/* PackBits inputs bits via its |bits| parameter and calls CodeSink
 * once 32 bits worth of data has been passed into it. */
int PackBits(LZWCmp *cmp, int bits) {
   int bitNdx, mask;

   if (cmp->traceFlags & TRACE_CODES)
      printf("Sending code %d\n", bits);

   if (cmp->traceFlags & TRACE_TREE)
      TreePrint(cmp->root, cmp->cst);

   cmp->bitsUsed += cmp->numBits;
   if (cmp->bitsUsed >= BITS_PER_SINK) {
      cmp->nextInt += bits >> (cmp->bitsUsed - BITS_PER_SINK);
      cmp->sink(cmp->sinkState, cmp->nextInt, 0);

      mask = (1 << cmp->bitsUsed - BITS_PER_SINK) - 1;
      cmp->nextInt = bits & mask; 
      cmp->bitsUsed -= BITS_PER_SINK;
      cmp->nextInt <<= BITS_PER_SINK - cmp->bitsUsed;

      return 1;
   }
   else {
      cmp->nextInt += bits << (BITS_PER_SINK - cmp->bitsUsed);
      return 0;
   }
}

void LZWCmpStop(LZWCmp *cmp) {
   if (cmp->curLoc == NULL) {
      PackBits(cmp, EOD);
      cmp->sink(cmp->sinkState, cmp->nextInt, 0);
   }
   else {
      PackBits(cmp, cmp->pCodeLimit);
      if (!PackBits(cmp, EOD)) {
         cmp->sink(cmp->sinkState, cmp->nextInt, 0);
      }
   }
   
   FreeCode(cmp->cst, 0);
   
   TreeFree(cmp->root);
   cmp->sink(cmp->sinkState, 0, 1);
}

void LZWCmpDestruct(LZWCmp *cmp) {
   DestroyCodeSet(cmp->cst);

   free(cmp->pCode.data);
}

/* Clear freelist.  This method is static because the freelist is shared
 * among all LZWCmp objects, and should be cleared only when all LZWCmp
 * objects are destroyed. */
void LZWCmpClearFreelist() {
   TreeNode *node;

   while (FreeList) {
      node = FreeList;
      FreeList =  node->right;
      free(node);
   }
}

void TreeInsert(TreeNode *root, void *cst, int toInsert) {
   int cmp;
   TreeNode *prevNode;

   while (root) {
      prevNode = root;
      cmp = CodeCmp(GetCode(cst, toInsert), GetCode(cst, root->cNum));
      if (cmp > 0)
         root = root->right;
      else if (cmp < 0)
         root = root->left;
      else {
         return;
      }
   }

   root = TreeNodeInit();
   root->cNum = toInsert;

   if (cmp > 0)
      prevNode->right = root;
   else if (cmp < 0)
      prevNode->left = root;
}

int TreeContains(TreeNode **cursor, void *cst, Code code) {
   int cmp;
   Code curCode;
   TreeNode *prevNode;

   while (*cursor) {
      prevNode = *cursor;
      curCode = GetCode(cst, (*cursor)->cNum);
      cmp = CodeCmp(code, curCode);

      if (cmp > 0)
         *cursor = (*cursor)->right;
      else if (cmp < 0)
         *cursor = (*cursor)->left; 
      else {
         return 1;
      }  
   }

   *cursor = prevNode;
   return 0;
}

void TreePrint(TreeNode *root, void *cst) {
   printf("|");
   InOrder(root, cst);
   printf("\n\n");
}

void InOrder(TreeNode *root, void *cst) {
   int ndx;
   Code code;

   if (root) {
      InOrder(root->left, cst);
      code = GetCode(cst, root->cNum);
      for (ndx = 0; ndx < code.size; ndx++) {
         if (ndx > 0)
            printf(" ");
         printf("%d", *(code.data + ndx));
      }
      printf("|");
      InOrder(root->right, cst);
   }
}

/* Initialize a new TreeNode struct and set its values to 0 */
TreeNode *TreeNodeInit() {
   TreeNode *newNode;

   if (FreeList) {
      newNode = FreeList;
      FreeList =  newNode->right;
   }
   else {
      newNode = (TreeNode *) calloc(1, sizeof(TreeNode));
   }
   newNode->left = newNode->right = 0;
   newNode->cNum = 0;

   return newNode;
}

void TreeInit(LZWCmp *cmp) {
   int ndx;

   cmp->cst = CreateCodeSet(cmp->recycleCode + 1);
   
   cmp->root = TreeNodeInit();
   for (ndx = 0; ndx < INIT_TREESIZE; ndx++) {
      NewCode(cmp->cst, ndx);
      TreeInsert(cmp->root, cmp->cst, ndx);
   }

   assert(NewCode(cmp->cst, -1) == EOD);
   TreeInsert(cmp->root, cmp->cst, EOD);
}

void TreeFree(TreeNode *root) {
   PostOrderFree(root);
}

void PostOrderFree(TreeNode *root) {
   if (root) {
      PostOrderFree(root->left);
      PostOrderFree(root->right);
      FreeNode(root);
   }
}

void FreeNode(TreeNode *toFree) {
   toFree->right = FreeList;
   FreeList = toFree;
}

int CodeCmp(Code code1, Code code2) {
   int minLength = code1.size < code2.size ? code1.size : code2.size;
   int ndx, diff;

   for (ndx = 0; ndx < minLength; ndx++) {
      if (diff = *(code1.data + ndx) - *(code2.data + ndx))
         return diff;
   }

   return code1.size - code2.size;
}
