#include <stdio.h>

int main() {
   int strange = 2147483648;

   printf("%d %d\n",strange,-strange);

   if(strange != 0 && strange == -strange)
      printf("I am strange\n");
}
