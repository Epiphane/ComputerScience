#include <stdio.h>

#define FixMacro(m, x, y) (m((x), (y)))

#define BadMax(x, y) x > y ? x : y
#define BadProd(x, y) x * y
#define BadSqrDist(x, y) x*x + y*y

#define GoodMax(x, y) FixMacro(BadMax, x, y)
#define GoodProd(x, y) FixMacro(BadProd, x, y)
#define GoodSqrDist(x, y) FixMacro(BadSqrDist, x, y)

int main() {
   int x, y;
   
   printf("Bad: %d %d %d\n", BadMax(2, 1) + 3, BadProd(2+3, 3+4),
    BadSqrDist(1+2, 5-1) + 1);
   printf("Good: %d %d %d\n", GoodMax(2, 1) + 3, GoodProd(2+3, 3+4),
    GoodSqrDist(1+2, 5-1) + 1);
}
