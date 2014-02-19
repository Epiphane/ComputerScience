#include <stdio.h>

#define F3

#define level 5
#undef F3
#define F2
#define F1

int main() {
#if !defined(F1) || level < 4
   printf("Alpha\n");
#else
#if defined(F2) && !defined(F3)
   printf("Beta\n");
#endif
#ifdef F1
   printf("Gamma\n");
#endif
#endif
}
