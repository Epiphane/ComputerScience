#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Calculon.h"

int ParseSuite(FILE *suite, char *suiteD, char *directory, Program **programs) {
   Program *currentProgram;
   Test *lastTest, *temp;
   char buffer[MAX_SUITE_LINE];
   int broken, makefile;

   broken = makefile = 0;

   currentProgram = *programs = NULL;

   if (access("Makefile", F_OK) != -1) {
      makefile = 1;
      if (MoveTestFile("Makefile", suiteD, directory))
         fprintf(stderr, "Call Failed: cp Makefile %s\n", directory);
   }

   while (!broken && fgets(buffer, MAX_SUITE_LINE, suite)) {
      if (*buffer == 'P') {
         if (currentProgram) {
            currentProgram->next = ParseProgram(buffer + 2, 
             suiteD, directory, makefile);
            currentProgram = currentProgram->next;
         }
         else
            currentProgram = *programs = ParseProgram(buffer + 2, 
             suiteD, directory, makefile);
            
         if (!currentProgram)
            broken = BAD_PARSE;
      }
      else {
         temp = ParseTest(buffer + 2, currentProgram->name, suiteD, directory);
         if (currentProgram->firstTest)
            lastTest->next = temp;
         else
            currentProgram->firstTest = temp;

         lastTest = temp;

         currentProgram->tests++;
      }
   }
   
   return 0;
}

Program *ParseProgram(char *line, char *suite, char *directory, int makefile) {
   int numFiles;
   char *token;
   Program *program;
   File *temp;

   program = calloc(1, sizeof(Program));
   program->makefile = makefile;

   token = strtok(line, " \n");
   while (program && token) {
      if (!program->name) {
         program->name = malloc(strlen(token) + 2 + 1);
         strcpy(program->name, "./");
         strcat(program->name, token);
      }
      else {
         if (MoveTestFile(token, suite, directory))
            program = FreeProgram(program);
         else {
            temp = malloc(sizeof(File));
            temp->next = program->compileFiles;
            strcpy(temp->fileName, token);
            program->compileFiles = temp;
         }
      }

      token = strtok(NULL, " \n");
   }   

   return program;
}

Test *ParseTest(char *line, char *programName, char *suite, char *directory) {
   char *token;
   Test *test;

   test = calloc(1, sizeof(Test));
   test->argv = ArgvCreate(MAX_ARGS);

   token = strtok(line, " \n");
   while (test && token) {
      if (!test->input) {
         test->input = malloc(strlen(token) + 1);
         strcpy(test->input, token);
         MoveTestFile(token, suite, directory);
      }
      else if (!test->output) {
         test->output = malloc(strlen(token) + 1);
         strcpy(test->output, token);
         MoveTestFile(token, suite, directory);
      }
      else if (!test->timeout) {
         sscanf(token, "%d", &test->timeout);
      }
      else {
         ArgvAddArg(test->argv, token);
      }

      token = strtok(NULL, " \n");
   }

   return test;
}

int MoveTestFile(char *file, char *fromDirectory, char *toDirectory) {
   char *fullFile = malloc(strlen(file) + strlen(fromDirectory) + 2);
   int failure = 0;

   strcpy(fullFile, file);
   
   if (failure = access(file, F_OK) == -1) {
      printf("Could not find required test file '%s'\n", file);
      exit(1);
   }
   else {
      if (!fork()) {
         execlp("cp", "cp", file, toDirectory, NULL);
         fprintf(stderr, "Call Failed: cp %s %s\n", file, toDirectory);
      }
      else
         wait(&failure);
   }

   return failure;
}
