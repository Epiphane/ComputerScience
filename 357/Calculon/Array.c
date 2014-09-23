#include <stdlib.h>
#include <string.h>
#include "Calculon.h"

typedef struct ArgArray {
   int numArgs;
   int maxArgs;
   char *args;
   
   int calls;
   char **argv;
   int strCalls;
   char *argvString;
} ArgArray;

void DoubleArgs(ArgArray *argv);
void ArgvFreeArgv(ArgArray *argv);

void *ArgvCreate(int maxArgs) {
   ArgArray *argv = calloc(1, sizeof(ArgArray));
   
   argv->maxArgs = maxArgs;

   argv->args = calloc(maxArgs, ARG_LENGTH * sizeof(char));
   argv->argv = calloc(argv->maxArgs, sizeof(char *));

   return argv;
}

int ArgvAddArg(void *argArray, char *arg) {
   ArgArray *argv = (ArgArray *) argArray;

   if (argv->numArgs == argv->maxArgs - 1)
      DoubleArgs(argv);

   argv->calls = argv->strCalls = 0;

   strcpy(argv->args + ARG_LENGTH * argv->numArgs, arg);

   return argv->numArgs++;
}

int ArgvAddArgs(void *argArray, int start, char **args) {
   int ndx;
   ArgArray *argv = (ArgArray *) argArray;

   argv->numArgs = start;

   while (*args) {
      ndx = ArgvAddArg(argArray, *args++);
   }

   return ndx;
}

void ArgvChangeSRArg(void *argArray, int argNum, char *arg, int value) {
   ArgArray *argv = (ArgArray *) argArray;

   argv->calls = argv->strCalls = 0;

   sprintf(argv->args + ARG_LENGTH * argNum, "%s%d", arg, value);
}

void ArgvChangeArg(void *argArray, int argNum, char *arg) {
   ArgArray *argv = (ArgArray *) argArray;

   argv->calls = argv->strCalls = 0;
   
   strcpy(argv->args + ARG_LENGTH * argNum, arg);
}

void DoubleArgs(ArgArray *argv) {
   char *oldArgs = argv->args;

   argv->args = calloc(argv->maxArgs * 2, ARG_LENGTH * sizeof(char));
   argv->argv = calloc(argv->maxArgs * 2 + 1, sizeof(char *));
   memcpy(argv->args, oldArgs, argv->maxArgs * ARG_LENGTH * sizeof(char));
   
   free(oldArgs);

   argv->maxArgs *= 2;
}

void *ArgvFree(void *argArray) {
   ArgArray *argv = (ArgArray *) argArray;

   free(argv->argv); 
   free(argv->argvString);
   free(argv->args);
   free(argv);

   return NULL;
}

char **ArgvReport(void *argArray) {
   int ndx;
   ArgArray *argv = (ArgArray *) argArray;
   
   if (!argv->calls++) {
      for (ndx = 0; ndx < argv->numArgs; ndx++) {
         argv->argv[ndx] = argv->args + ndx * ARG_LENGTH;
      }
      argv->argv[ndx] = NULL;
   }

   return argv->argv;
}

char *ArgvStringReport(void *argArray) {
   int argsLength, ndx;
   char *tmp;
   ArgArray *argv = (ArgArray *) argArray;

   if (!argv->strCalls++) {
      argsLength = 0;
      for (ndx = 0; ndx < argv->numArgs; ndx++) {
         argsLength += strlen(argv->args + ndx * ARG_LENGTH) + 1;
      }
      
      tmp = argv->argvString = malloc(argsLength);   
      
      for (ndx = 0; ndx < argv->numArgs; ndx++) {
         if (ndx)
            tmp[-1] = ' '; 
         
         strcpy(tmp, argv->args + ndx * ARG_LENGTH);
         tmp += strlen(tmp) + 1;
      }
      argv->argv[ndx] = NULL;
   }

   return argv->argvString;
}
