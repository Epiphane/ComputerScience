#include <stdio.h>
#include "file.h"

int main(int argc, char *argv[]) {

   printf("Size of a partition entry: %ld\n", sizeof(struct partition));
   return 0;
}
