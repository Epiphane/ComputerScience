#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "Calculon.h"

#define GCC_LEN 4

typedef struct CompileFile {
   char name[MAX_SUITE_LINE];
   struct CompileFile *next;
} CompileFile;

int RunSuite(FILE *suite, char *directory) {
   char buffer[MAX_SUITE_LINE];
   char *progName, **inputFiles;
   int broken = 0;

   progName = NULL;
   while (!broken && fgets(buffer, MAX_SUITE_LINE, suite)) {
      if (*buffer == 'P') {
         if (progName)
            free(progName);
         
         if(!(progName = CreateProgram(buffer + 2)))
            broken = 1;
      }
      else {

      }
   }
   
   return broken;
}

char *CreateProgram(char *infoLine) {
   char *token, **progFiles;
   char *progName = NULL;
   int numFiles = 2;
   CompileFile *progFile = NULL, *temp;


   token = strtok(infoLine, " \n");
   while (token != NULL) {
      if (progName) {
         temp = malloc(sizeof(CompileFile));
         
         if (access(token, F_OK) == -1) {
            printf("Could not find requred test file '%s'\n", token);
            return NULL;
         }        
         else {
         
         }

         strcpy(temp->name, token);
         temp->next = progFile;
         progFile = temp;
         numFiles ++;
      }
      else {
         progName = malloc(strlen(token) + 1);
         strcpy(progName, token);
      }

      token = strtok(NULL, " \n");
   }

   progFiles = calloc(numFiles--, sizeof(char *));
   while (temp = progFile) {
      progFiles[--numFiles] = malloc(strlen(progFile->name) + 1);
      strcpy(progFiles[numFiles], progFile->name);
    
      progFile = progFile->next;
      free(temp);
   }

   *progFiles = malloc(GCC_LEN * sizeof(char));
   strcpy(*progFiles, "gcc");

   if (!fork()) {
      execvp("gcc",progFiles); 
   }

   return progName;
}
