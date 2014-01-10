#include <stdio.h>

int main() {
   int res, input, lastInput, numSequences = 0, longestSequence = 0, thisLength;
   res = scanf("%d", &lastInput);
   while(res == 1) {
      numSequences ++;
      thisLength = 1;
      while(1 == (res = scanf(" %d",&input)) && input >= lastInput) {
         lastInput = input;
         thisLength ++;
      }
      lastInput = input;
      if(thisLength > longestSequence)
         longestSequence = thisLength;
   }
   
   if(numSequences == 1)
      printf("There is one sorted subsequence, ");
   else
      printf("There are %d sorted subsequences, ", numSequences);

   if(longestSequence == 1)
      printf("the longest of which has one integer.\n");
   else
      printf("the longest of which has %d integers.\n", longestSequence);

   return 0;
}
