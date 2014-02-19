#include <stdio.h>

#define Test1(x) {       \
   if (x > x+1)          \
      printf("Test1\n"); \
}
     
#define Test2(x) {       \
   if (x != x)           \
      printf("Test2\n"); \
}

#define Test3(x) {       \
   if (x && !(x))        \
      printf("Test3\n"); \
}

int main() {
   Test1(1 < 0);
   Test2(0 != 2);
   Test3(1 || 0);
}
