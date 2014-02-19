#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LZWCmp.h"
#include "SmartAlloc.h"

#define DOT_Z ".Z\0"
#define DOT_Z_SIZE 3
#define RECYCLE_CODE 4096
#define EOD 256
#define INTS_PER_LINE 8

#define TRACE_SIZE 0x10

void FileSink(void *outFile, UInt code, int done);
void ProcessFile(char *fileName, int traces, LZWCmp *cmp);

typedef struct FilePosn {
   FILE *fp;
   int numWritten;
} FilePosn;

int main(int argc, char **argv) {
   char **files;
   int argNum, traces = 0;
   LZWCmp cmp;

   for (argNum = 1; argNum < argc; argNum++) {
      if (argv[argNum][0] == '-') {
         if (strchr(argv[argNum], 't'))
            traces |= TRACE_TREE;

         if (strchr(argv[argNum], 'b'))
            traces |= TRACE_BUMPS;

         if (strchr(argv[argNum], 'r'))
            traces |= TRACE_RECYCLES;

         if (strchr(argv[argNum], 'c'))
            traces |= TRACE_CODES;

         if (strchr(argv[argNum], 's'))
            traces |= TRACE_SIZE;
      }
      else {
         ProcessFile(argv[argNum], traces, &cmp);
      }
   }

   LZWCmpClearFreelist();
   if (traces & TRACE_SIZE)
      printf("Final space: %lu\n", report_space());

   return 0;
}

void ProcessFile(char *fileName, int traces, LZWCmp *currCmp) {
   CodeSink sink = &FileSink;
   UChar data;
   FILE *pFile, *cmpFile;
   FilePosn cmpFilePosn;
   char *cmpFileName = malloc(strlen(fileName) + DOT_Z_SIZE);

   strcpy(cmpFileName, fileName);
   strcat(cmpFileName, DOT_Z);
   cmpFile = fopen(cmpFileName, "w");

   cmpFilePosn.fp = cmpFile;
   cmpFilePosn.numWritten = 0;

   pFile = fopen(fileName, "r");
   if (!pFile) {
      printf("Cannot open %s\n", fileName);
   }
   else {
      LZWCmpInit(currCmp, sink, &cmpFilePosn, RECYCLE_CODE, traces); 

      while (fread(&data, sizeof(char), 1, pFile)) {
         LZWCmpEncode(currCmp, data);
      }

      LZWCmpStop(currCmp);
      if (traces & TRACE_SIZE)
         printf("Space after LZWCmpStop for %s: %lu\n", fileName, 
          report_space());
      LZWCmpDestruct(currCmp);
   }

   free(cmpFileName);
}

/* |FileSink| takes in the compressed data, one UInt at a time.
 * It then outputs to the correct .Z file in Hex format, with
 * eight characters separated by a space.
 */
void FileSink(void *outFile, UInt code, int done) {
   FilePosn *posn = (FilePosn *) outFile;

   if (done) {
      fprintf(posn->fp, "\n");
   }
   else {
      fprintf(posn->fp, "%08X", code);
      if (++posn->numWritten == INTS_PER_LINE) {
         fprintf(posn->fp, "\n");
         posn->numWritten = 0;
      }
      else
         fprintf(posn->fp, " ");
   }
}
