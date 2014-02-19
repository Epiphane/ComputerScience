#include <stdio.h>
#include <string.h>

#include "LZWCmp.h"
#include "SmartAlloc.h"

#define EXT_LENGTH 3
#define MAX_DICTIONARY_SIZE 4096
#define INTS_PER_LINE 8

typedef struct FileState {
   FILE *fp;
   int numInts;
} FileState;

void CodeSinker(void *state, UInt code, int done) {
   FileState *file = (FileState *)state;
  
   if (!done) { 
      fprintf(file->fp, "%08X", code);

      if (--file->numInts)
         fprintf(file->fp, " ");
      else {
         fprintf(file->fp, "\n");
         file->numInts = INTS_PER_LINE;
      }
   }
   else {
      fprintf(file->fp, "\n");
   }
}

int ReadFlag(char *arg, int *traceFlags, int *reportSpace) {
   int newFlag = FALSE;   
   int isFlag = *arg == '-';

   while (*++arg && isFlag) {
      if (*arg == 't')
         newFlag |= TRACE_TREE;
      else if (*arg == 'c')
         newFlag |= TRACE_CODES;
      else if (*arg == 'b')
         newFlag |= TRACE_BUMPS;
      else if (*arg == 'r')
         newFlag |= TRACE_RECYCLES;
      else if (*arg == 's')
         *reportSpace = TRUE;
   }
   *traceFlags |= newFlag;

   return isFlag;
}

int main(int argc, char *argv[]) {
   LZWCmp cmp;
   int reportSpace, traceFlags, nextChar;
   FILE *fileToZip;
   char *destinationName;
   FileState destination;

   reportSpace = traceFlags = FALSE;
   
   while (--argc && ReadFlag(*++argv, &traceFlags, &reportSpace))
      ;

   while (argc--) {
      if (fileToZip = fopen(*argv, "r")) {
         destinationName = malloc(strlen(*argv) + EXT_LENGTH);
         strcpy(destinationName, *argv++);
         strcat(destinationName, ".Z");

         destination.fp = fopen(destinationName, "w");
         destination.numInts = INTS_PER_LINE;

         LZWCmpInit(&cmp, CodeSinker, &destination, 
          MAX_DICTIONARY_SIZE, traceFlags);
         
         while ((nextChar = fgetc(fileToZip)) != EOF) {
            LZWCmpEncode(&cmp, nextChar);
         }
         
         LZWCmpStop(&cmp);
         if (reportSpace)
            printf("Space after LZWCmpStop for %s: %d\n", 
             *(argv - 1), report_space());      
         
         LZWCmpDestruct(&cmp);
            
         free(destinationName);
         fclose(destination.fp);
         fclose(fileToZip);
      }
      else
         printf("Cannot open %s\n", *argv);
   }

   LZWCmpClearFreelist();
   
   if (reportSpace) // flag that results from the -s commandline option
      printf("Final space: %ld\n", report_space());

   return 0;
}
