#include <stdio.h>

#define NO_ARG -3
#define NO_SUITE -1
#define NO_DIR -2
#define FAILED_RUN 1
#define NO_TEST_FILE 2
#define BAD_PARSE 3

#define MAX_SUITE_LINE 100

#define EXTRA_GCC_ARGS 4
#define GCC_LEN 4

#define ARG_LENGTH 32
#define MAX_ARGS 10

/* Calculon.c Functions */
void RemoveTempDirectory(char *directory);
void DontScrollDown();

typedef struct Test {
   void *argv;
   char *input;
   char *output;
   int timeout;
   struct Test *next;
} Test;

typedef struct File {
   char fileName[ARG_LENGTH];
   struct File *next;
} File;

typedef struct Program {
   struct File *compileFiles;
   char *name;
   int makefile;
   int tests;
   struct Test *firstTest;
   struct Program *next;
} Program;

/* Array.c Functions */
void *ArgvCreate(int maxArgs);
int ArgvAddArg(void *argArray, char *arg);
int ArgvAddArgs(void *argArray, int start, char **args);
void ArgvChangeArg(void *argArray, int argNum, char *arg);
void ArgvChangeSRArg(void *argArray, int argNum, char *arg, int value);
void *ArgvFree(void *argArray);

char **ArgvReport(void *argArray);
char *ArgvStringReport(void *argArray);

/* RunSuite.c Functions */
void RunSuite(Program *firstProgram);
int RunTests(Program *program);
int RunTest(char *pName, Test *test, void *safeRun, int lastArg, int testNum);
void PrintTestIssues(int input, int output);
int TestFailure(int diffFail, int safeRun, char *pName, int testNum);
Program *FreeProgram(Program *program);
Test *FreeTest(Test *test);
int CompileProgram(Program *program);

/* Parse.c Functions */
int ParseSuite(FILE *suite, char *suiteD, char *directory, Program **programs);
Program *ParseProgram(char *line, char *suite, char *directory, int makefile);
Test *ParseTest(char *line, char *programName, char *suite, char *directory);
int MoveTestFile(char *file, char *fromDir, char *toDir);

