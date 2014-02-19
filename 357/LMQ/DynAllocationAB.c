#include <stdio.h>
#include "SmartAlloc.h"

int main() {
   int *p1, *p2, total = 0;
   
   p2 = malloc(sizeof(int));
   do {
      p1 = p2;
      p2 = malloc(sizeof(int));
      
      free(p1);
      scanf("%d",p2);
      total += *p2;
   } while (total < 20);
   free(p2);
   
   printf("Total: %d Final space: %d\n", total, report_space());

}
