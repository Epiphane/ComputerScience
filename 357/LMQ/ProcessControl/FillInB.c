#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
   int i;
   
   for (i = 0; i < 5 && !fork(); i++) {
      fflush(stdout);
      printf("%d ", i);
   }
 
   wait(NULL);

   printf("\n");

   return 0;
}
