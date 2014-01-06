#include <stdio.h>

// GetTwoInts function fills two int values by scanning from input, and returns
// a result like that of scanf
int GetTwoInts(int *i1, int *i2) {
   printf("Enter two ints: ");
   return scanf("%d %d", i1, i2);
}

// FindMinMax function call fills in two int values for min and max of two
// other int values.
int FindMinMax(int i1, int i2, int *min, int *max) {
   if(i1 > i2) {
      *min = i2;
      *max = i1;
   }
   else {
      *min = i1;
      *max = i2;
   }
   return 0;
}

// Sort is passed two ints and puts them in increasing order.  Calls
// FindMinMax.
int Sort(int *i1, int *i2) {
   return FindMinMax(*i1, *i2, i1, i2);
}

int main() {
   int i1, i2, min, max;
   while (EOF != GetTwoInts(&i1, &i2)) {
      FindMinMax(i1, i2, &min, &max);
      printf("Min/max of %d %d. Min: %d  Max: %d\n", i1, i2, min, max); 
      Sort(&i1, &i2);
      printf("In sorted order: %d %d\n", i1, i2);
   }
}
