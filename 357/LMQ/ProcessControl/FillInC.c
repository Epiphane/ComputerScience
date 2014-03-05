#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
   char **ptr;
   
   for (ptr = argv+1; *ptr; ptr++)
      printf("%s ", *ptr);
   printf("\n");

   if (argc > 3) {
      *--ptr = NULL;
      *++argv = *(argv - 1);
      execv(*argv, argv);
   }
   printf("Done!");
      
   return 0;   
}
