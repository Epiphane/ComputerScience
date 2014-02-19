#include <stdio.h>

int main() {
   int i1 = 10, i2 = 20, i3 = 30, *iPtr1 = &i1, *iPtr2 = &i2, *iPtr3;
   
   // 10 20 30 20 10 20
   iPtr3 = iPtr1 = iPtr2;
   iPtr2 = &i1;

   printf("%d %d %d %d %d %d\n", i1, i2, i3, *iPtr1, *iPtr2, *iPtr3);

   return 0;
}
