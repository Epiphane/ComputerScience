#include <stdio.h>

#define DIM 8

int IsLess(double v1, double v2) {
   return v1 < v2;
}

int IsGreater(double v1, double v2) {
   return v1 > v2;
}

int IsAbsLess(double v1, double v2) {
   if(v1 < 0) v1 *= -1;
   if(v2 < 0) v2 *= -1;
   return v1 < v2;
}

// Iterate through "vals" (by incrementing vals itself for conciseness) to
// find the best value, as defined by parameter "better".  Assume "dim"
// is at least 1.  Careful definition of "better" will be essential.
double FindBest(double vals[], int dim, int (*better)(double, double)) {
   double best, *end = vals + dim;
   
   best = *vals;
   while(--dim) 
      if(better(*++vals, best))
         best = *vals;

   return best;
}

int main() {
   double vals[DIM];
   int i;
   
   for (i = 0; i < DIM; i++)
      scanf("%lf", vals+i);
     
   printf("Min value %0.3f\n", FindBest(vals, DIM, IsLess));
   printf("Max value %0.3f\n", FindBest(vals, DIM, IsGreater));
   printf("Lowest absolute value %0.3f\n", FindBest(vals, DIM, IsAbsLess));
}

