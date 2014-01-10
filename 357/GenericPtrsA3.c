#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IDIM 10
#define DDIM 8
#define FDIM 6

// ScanArray is passed an array of some type scanf-able with a single
// format specifier e.g. "%d" or "%c".  It's also passed information on
// the array's dimension, and the size of each element, plus a format
// string.  It runs scanf to read in each element of the array, and returns.
// It does no error checking.  You can pass fmt directly to scanf thus: 
// scanf(fmt, ...  to use the format string that was passed from the main.
// (More details on format strings and char pointers will come in the next
// topic.)
void scanArray(void *arr, int dim, int size, char *fmt) {
   char *ndx = arr;
   for(; dim--; ndx += size) {
      scanf(fmt, ndx);
   }
}

int main() {
   int ndx, iArr[IDIM];
   double dArr[DDIM];
   float fArr[FDIM];
     
   scanArray(iArr, IDIM, sizeof(int), "%d");
   scanArray(dArr, DDIM, sizeof(double), "%lf");
   scanArray(fArr, FDIM, sizeof(float), "%f");
      
   printf("iArr:");
   for (ndx = 0; ndx < IDIM; ndx++)
      printf(" %d", iArr[ndx]);
   
   printf("\ndArr:");
   for (ndx = 0; ndx < DDIM; ndx++)
      printf(" %0.2f", dArr[ndx]);
                        
   printf("\nfArr:");
   for (ndx = 0; ndx < FDIM; ndx++)                          
      printf(" %0.2f", fArr[ndx]); 
                                
   printf("\n");
}
