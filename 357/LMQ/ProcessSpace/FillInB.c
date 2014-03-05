#include <stdio.h>

int *f(int x) {
   int data = x;
   
   return x ? f(x-1) : &data;
}

int main() {
   int prm = 42;
   int *v1 = f(prm++);
   int *v2 = f(prm++);
   int *v3 = f(prm++);
   
   f(prm*2);

   printf("%d %d %d\n", *v1, *v2, *v3);
}
