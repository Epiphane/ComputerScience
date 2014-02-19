#include <stdio.h>

#define Swap(x, y) {\
   int *__tX, __t = *(__tX = &(x));\
   int *__tY = &(y);\
\
   *__tX = *__tY;\
   *__tY = __t;\
}

int main() {
   int vals[] = {10, 4, 20000000, 1, 10000000, 0}, i = 3, j = 1;
   
   Swap(vals[++i], vals[++j]);
   Swap(vals[i], i);
   Swap(vals[j], j);
   Swap(vals[vals[5]], vals[5]);
   Swap(vals[3], vals[vals[3]]);
   
   printf("i: %d j: %d vals:", i, j);
   for (i = 0; i < 6; i++)
      printf(" %d", vals[i]);
   printf("\n");
}
