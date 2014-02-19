#include <stdio.h>

float RunningAverage(float val) {
   static int numVals = 0;
   static float currentTotal = 0;

   currentTotal += val;
   return currentTotal / ++numVals;
}

int main() {
   double val;
   
   while (EOF != scanf("%lf", &val))
      printf("Running average %0.3f\n", RunningAverage(val));
}
