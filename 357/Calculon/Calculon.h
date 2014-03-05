#define NO_SUITE -1
#define NO_DIR -2
#define FAILED_RUN 1

#define MAX_SUITE_LINE 100

int RunSuite(FILE *suite, char *directory);
char *CreateProgram(char *infoLine);

void DontScrollDown();
