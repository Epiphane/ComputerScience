#include <stdio.h>

int main() {
#include "Counter.h"
#if PRINT == 1
   printf("%d includes\n", counter);
#endif

#include "Counter.h"
#if PRINT == 1
   printf("%d includes\n", counter);
#endif

#include "Counter.h"
#if PRINT == 1
   printf("%d includes\n", counter);
#endif

#include "Counter.h"
#if PRINT == 1
   printf("%d includes\n", counter);
#endif

#include "Counter.h"
#if PRINT == 1
   printf("%d includes\n", counter);
#endif
}
