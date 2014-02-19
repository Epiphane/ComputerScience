#include <stdio.h>
#define MAX_SCORE 100

int main() {
   int score;
   int scores, max, min, total, countOfMode;
   int count[MAX_SCORE+1] = {0};

   min = MAX_SCORE;
   countOfMode = scores = max = total = score = 0;

   // Scan until EOF, insert scores
   while(EOF != scanf("%d",&score)) {
      total += score;
      if(score > max)
         max = score;
      if(score < min)
         min = score;

      if(++count[score] > countOfMode)
         countOfMode = count[score];

      scores++;
   }

   if(scores) {
      printf("Min: %d\nMax: %d\nMean: %.2f\nMode: [%d]",min,max,(double)total/scores,countOfMode);
      for(score = 0; score <= MAX_SCORE; score++)
         if(count[score] == countOfMode) {
            printf(" %d",score);
         }

      printf("\n");
   }
}
