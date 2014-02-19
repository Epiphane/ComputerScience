#include <stdio.h>

int Sum(int term1, ...) {
   int *term = &term1;
   int sum = 0;

   while(*term) {
      sum += *term++;
   }

   return sum;
}

int main() {
   printf("%d %d %d %d\n", Sum(0), Sum(3, 5, 6, 7, 0),
    Sum(7, 2, 42, 3, 5, -4, 0), Sum(-1, 9, 12, 123, -213, 42, 7, 2, 0));
}
