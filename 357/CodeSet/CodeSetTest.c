#include <stdio.h>
#include "SmartAlloc.h"
#include "CodeSet.h"

int main() {
   char command;
   void *cs[3];  // Array of three void pointers
   int ndx, numCodes, val, set, depth;
   Code code;
   
  while (EOF != scanf(" %c", &command)) {
      switch (command) {
      case 'C': // create new codeSet     C <set> <maxCodes>
         scanf("%d%d", &set, &numCodes);
         cs[set] = createCodeSet(numCodes);
         printf("Set %d initialized\n", set);
         break;
      case 'N': // add new code           N <set> <value>
         scanf("%d%d", &set, &val);
         printf("New code in set %d at index %d\n", set, newCode(cs[set], val));
         break;
      case 'E': // extend existing code   E <set> <codeIndex>
         scanf("%d%d", &set, &val);
         printf("Added extended code %d\n", extendCode(cs[set], val));
         break;
      case 'S': // set suffix             S <set> <codeIndex> <suffix>
         scanf("%d%d%d", &set, &val, &depth); 
         setSuffix(cs[set], val, depth);
         break;
      case 'G': // get code               G <set> <codeIndex>
         scanf("%d%d", &set, &val);
         printf("Code %d in set %d has value:",val,set);
         code = getCode(cs[set], val);
         char *byte = code.data; 
         for(; byte < code.data + code.size; byte ++) {
            printf(" %d",*byte);
         }
         printf("\n");
         break;
      case 'F': // Free code <set>
         scanf("%d%d", &set, &val);
         freeCode(cs[set], val);
         break;
      case 'R': // Report current space
         printf("Current space: %d\n", report_space());
         break;
      case 'X': // Add many codes in prefix order  X <set> <depth>
         scanf("%d%d", &set, &depth);
         ndx = newCode(cs[set], 'X');
         while (--depth) {
            ndx = extendCode(cs[set], ndx);
            setSuffix(cs[set], ndx, 'X');
         }
         break;
      case 'D': // Destroy existing code set  D <set>
         scanf("%d", &set);
         destroyCodeSet(cs[set]);
         printf("Set %d destroyed\n", set);
      }
   }
}
