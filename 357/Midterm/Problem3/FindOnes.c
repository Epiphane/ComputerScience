#include <stdio.h>
#include <assert.h>

int numMatches(int val, int bits) {
   unsigned mask;
   int numMatches;
   int shift;

   mask = (1 << bits) - 1;

   numMatches = (val & mask) == mask;

   // Move to front now
   mask = mask << 32 - bits;
   while(!(mask % 2)) {
      numMatches += (val & mask) == mask;
      mask = mask >> 1;
   }

   return numMatches;
}

int main() {
   int val;
   int bits;

   while(EOF != scanf(" %X",&val)) {
      scanf(" %d",&bits);
      assert(bits > 0 && bits < 32);
      
      printf("%X has %d spots with %d 1-bits in a row\n",val,numMatches(val,bits),bits);
   }
   return 0;
}
