#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FORKS 30

int main() {
   int i, pid, status;
   
   for (i = 0; i < FORKS; i++) {
      if (!fork()) {
         if (!(i%10))
           sleep(2);
         printf("%d\n", i);
         i = FORKS; 
      }
      else
         wait(&status);
   }
   
   return 0;
}
