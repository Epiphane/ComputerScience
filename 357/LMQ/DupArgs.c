#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SmartAlloc.h"

void printArgs(char **args) {
   while (*args)
      printf("%s\n", *args++);
}

void burnArgs(char **args, int argc) {
   while (argc--) {
      **args = 'X';
      *args++ = NULL;
   }
}

void copyArgs(char **argv, char ***dupArgs) {
   char **newArgs;
   int i = EOF;
   while(*(argv + ++i))
      ;

   newArgs = *dupArgs = calloc(i, sizeof(argv));
   while(*++argv) {
      *newArgs = malloc(strlen(*argv)+1);
      strcpy(*newArgs++, *argv);
   }
}

void main(int argc, char **argv) {
   char **dupArgs;
   
   copyArgs(argv, &dupArgs);
   burnArgs(argv, argc);
   printArgs(dupArgs);
   printf("Space: %d\n", report_space());
}
