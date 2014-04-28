#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "Calculon.h"

#define NEW_FILE 0644
#define TEN 10
#define SAFERUN 192
#define TIMEOUT 1
#define RUNTIME 200

void RunSuite(Program *firstProgram) {
   while (firstProgram) {
      if (mkdir("run", S_IRUSR | S_IWUSR | S_IXUSR)) {
         fprintf(stderr, "Error: Could not create directory run\n");
      }
      else {
         if (CompileProgram(firstProgram))
            RunTests(firstProgram);

         chdir("..");
      
         RemoveTempDirectory("run");
      }

      firstProgram = FreeProgram(firstProgram);
   }
}

int CompileProgram(Program *program) {
   File *file;
   
   void *compileArgs = ArgvCreate(MAX_ARGS);
   int failure = 0;

   ArgvAddArg(compileArgs, "gcc");

   while (file = program->compileFiles) {
      ArgvAddArg(compileArgs, file->fileName);
      
      if (!fork()) {
         failure = execlp("cp", "cp", file->fileName, "run", NULL);
         fprintf(stderr, "Error: Could not run cp %s run: %s\n", 
          file->fileName, strerror(failure));
         exit(0);
      }
      else
         wait(NULL);

      program->compileFiles = file->next;
      free(file);
   }

   chdir("run");
   if (program->makefile) {
      
      if (!fork()) {
         dup2(1, 2);
         execlp("make", "make", "-s", "-f", "../Makefile", 
          program->name + 2, NULL);
      }
      else {
         wait(&failure);
         if (failure)
            printf("Failed: make %s\n", program->name + 2);
      }
   }

   else {
      ArgvAddArg(compileArgs, "-o");
      ArgvAddArg(compileArgs, program->name + 2);

      if (!fork()) {
         dup2(1, 2);
         execvp("gcc", ArgvReport(compileArgs)); 
      }
      else {
         wait(&failure);
         if (failure)
            printf("Failed: %s\n", ArgvStringReport(compileArgs));
      }
   }

   compileArgs = ArgvFree(compileArgs);   
   
   return !failure;
}   

int RunTests(Program *program) {
   int pass, testNum, timeArg, clockArg, lastArg;
   void *safeRunArgs = ArgvCreate(MAX_ARGS);
   
   pass = testNum = program->firstTest != NULL;

   ArgvAddArg(safeRunArgs, "/home/grade_cstaley/bin/SafeRun");
   ArgvAddArg(safeRunArgs, "-i");
   ArgvAddArg(safeRunArgs, "-p30");
   timeArg = ArgvAddArg(safeRunArgs, "-t1000");
   clockArg = ArgvAddArg(safeRunArgs, "-t10000");
   lastArg = ArgvAddArg(safeRunArgs, program->name);

   while (program->firstTest) {
      ArgvChangeSRArg(safeRunArgs, timeArg, "-t", program->firstTest->timeout);
      ArgvChangeSRArg(safeRunArgs, clockArg, "-T", 
       program->firstTest->timeout * TEN);

      pass &= RunTest(program->name, program->firstTest, 
       safeRunArgs, lastArg, testNum++);
      
      program->firstTest = FreeTest(program->firstTest);
   }
   
   if (pass)
      printf("%s %d of %d tests passed.\n", program->name + 2, 
       program->tests, program->tests);

   return pass;
}

int RunTest(char *pName, Test *test, void *safeRun, int lastArg, int testNum) {
   int input, output, fail;
   char **temp;
   int safeRunStatus, diffFail, timeout, runtimeError;

   ArgvAddArgs(safeRun, lastArg + 1, ArgvReport(test->argv));
 
   chdir("..");
   if (!fork()) {
      execlp("cp", "cp", test->input, "run", NULL);
   }
   else {
      wait(NULL);
      if (!fork()) {
         execlp("cp", "cp", test->output, "run", NULL);
      }
      else
         wait(NULL);
   }
   chdir("run");

   input = open(test->input, O_RDONLY);   
   if (input >= 0) {
      if (!fork()) {
         output = open("test.output.temp", O_WRONLY | O_CREAT | O_TRUNC,
          NEW_FILE);
         
         dup2(input, 0);
         close(input);
         dup2(output, 1);
         dup2(output, 2);
         close(output);

         temp = ArgvReport(safeRun);
         output = execv(*temp, temp);
         fprintf(stderr, "Error calling SafeRun: %s\n", strerror(output));
         exit(1);
      }
      else {
         close(input);

         wait(&safeRunStatus);

         if (!fork()) {
            output = open("diffRes", O_WRONLY | O_CREAT | O_TRUNC, NEW_FILE);
           
            dup2(output, 1);
            close(output);

            execlp("diff", "diff", test->output, "test.output.temp", NULL);
            fprintf(stderr, "Error calling diff %s test.output.temp\n",
             test->output);
            exit(1);
         }
         else {
            wait(&diffFail);
         }
      }
   }
   else {
      PrintTestIssues(input, -1);
   }

   fail = TestFailure(diffFail, WEXITSTATUS(safeRunStatus), pName + 2, testNum);
   return !fail;
}

int TestFailure(int diffFail, int safeRun, char *pName, int testNum) {
   int comma = 0;
 
   if (diffFail | safeRun) {
      printf("%s test %d failed: ", pName, testNum);
      
      if (diffFail) {
         printf("diff failure");
         comma++;
      }      

      if (safeRun) {
         if (comma++)
            printf(", ");
         
         printf("runtime error");
      }

      if (safeRun & TIMEOUT && safeRun & SAFERUN) {
         if (comma++)
            printf(", ");
         
         printf("timeout");
      }
      
      printf("\n");
   }

   return diffFail | safeRun;
}

void PrintTestIssues(int input, int output) {
   if (input < 0)
      fprintf(stderr, "Error opening input for reading.\n");
   else 
      close(input);
}

Program *FreeProgram(Program *program) {
   Program *next = program->next;

   if (program->name)
      free(program->name);

   while (program->firstTest)
      program->firstTest = FreeTest(program->firstTest);

   free(program);
   return next;
}

Test *FreeTest(Test *test) {
   Test *next = test->next;

   free(test->output);

   return next;
}
