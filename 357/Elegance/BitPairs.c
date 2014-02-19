#include <stdio.h>

#define BITS_PER_SHORT 16
#define TWO_ONES 98304

void main() {
   int numPairs, lastNum;
   numPairs = lastNum = 0;

   unsigned short thisNum, shift;
   
   while(EOF != scanf("%hd", &thisNum)) {
      lastNum |= thisNum;
      for(shift = BITS_PER_SHORT; shift--;) {
         numPairs += (lastNum & TWO_ONES) == TWO_ONES;
         lastNum *= 2;
      }
   }

   printf("%d pairs\n",numPairs);
}
