#include <stdio.h>
#include <stdarg.h>

/* Accept a format string comprised only of the letters c d i s and f for char,
 * double, int, short, and float, respectively.  Accept as many variadic
 * parameters after that format string as there are letters in the string.
 * Assume the actual parameters in the main were of the indicated types 
 * (though passed variadically).  Total the parameters and return the total
 * (use the ASCII value of any char parameter. */
float addEm(char *format, ...) {
   va_list numbers;
   float total = 0;
   char argType;
   char **firstArg = &format;

   va_start(numbers, *++firstArg);
   while(argType = *format++) {
      switch(argType) {
      case 'c':
      case 's':
      case 'i':
         total += va_arg(numbers, int);
         break;
      case 'd':
      case 'f':
         total += va_arg(numbers, double);
         break;
      }
   }

   va_end(numbers);

   return total;
}

int main() {
   double dVal;
   float fVal;
   int iVal;
   short sVal;
   char cVal;
  
   scanf("%d %hd %c %f %lf", &iVal, &sVal, &cVal, &fVal, &dVal);
  
   printf("Total is %0.2f or %0.2f if you add 42\n",
    addEm("fdisc", fVal, dVal, iVal, sVal, cVal),
    addEm("siccddf", sVal, iVal, cVal, 42, dVal, 0.0, fVal));
}
