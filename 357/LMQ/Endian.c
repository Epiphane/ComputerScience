#include <stdio.h>

#define BYTE 8
#define BYTE3 24

int main() {
   unsigned short s;
   int i;
   unsigned mask;

   scanf("%hx %x", &s, &i);

   mask = (1 << BYTE) - 1;
   s = s << BYTE & ~mask | s >> BYTE;
   
   i = i << BYTE3 | i >> BYTE3 & mask | i >> BYTE & mask << BYTE | i << BYTE & mask << BYTE * 2;

   //i = (i << BYTE & ~mask | i >> BYTE & mask) & mask << BYTE | i >> BYTE3 ^ ~mask >> BYTE ^ ~i << BYTE3;

   printf("%X %X", s, i);
}
