#include <stdio.h>
#include "List.h"
#include "SmartAlloc.h"

int main() {
   List **lists;
   int numLists, list;
   char command;
   double value;
   
   scanf("%d", &numLists); // Find out how many lists we want
   lists = calloc(sizeof(List *), numLists);
   
   // Create lists, and put a sample value on each
   for (list = 0; list < numLists; list++) {
      lists[list] = ListCreate();
      ListAdd(lists[list], 42.0, LIST_AT_START);
   }
   
   // Hand-delete selected ones, but not all
   while (EOF != scanf("%d", &list)) {\
      numLists--;
      ListDestroy(lists[list]);
   }
   
   if (numLists)
      ListDestroyAll();    // Destroy all still-existing lists
   ListClearFreelist(); // Burn the Node freelist
   free(lists);         // Free our list pointer block
   
   printf("%d bytes left\n", report_space());  // Should be 0
}
