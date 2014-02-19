#include <stdio.h>
#include <stdlib.h>

#define NUM_VALS 26
#define NUM_CHARS 11
#define MAX_DIFF 7
#define BITS_PER_CHAR 7
#define LSBS 3

void EmbedWatermark(short *vals, char *msg) {
   unsigned mask = (1 << LSBS) - 1;
   int charNum, offset;
   short *head;
 
   charNum = 0;
   while(charNum < NUM_CHARS) {
      if(!(offset = charNum++ % LSBS))
         head = vals++;
      
      *vals = *vals++ & ~mask | *msg & mask;
      *vals = *vals++ & ~mask | *msg >> LSBS & mask;
      *head = *head & ~(1 << offset) | *msg++ >> BITS_PER_CHAR - 1 << offset;
   }

   return;
   for(offset = charNum = 0; charNum < NUM_CHARS;) {
      head = vals++;
      do {
         *vals = *vals++ & ~mask | *msg & mask;
         *vals = *vals++ & ~mask | *msg >> LSBS & mask;
         *head = *head & ~(1 << offset) | *msg++ >> BITS_PER_CHAR - 1 << offset;
      } while(offset = ++charNum % LSBS);
   }
}

void PrintWatermark(short *copy) {
   unsigned mask = (1 << LSBS) - 1;
   char thisChar;
   int charNum;
   short headNdx;

   for(charNum = 0; charNum < NUM_CHARS; charNum ++) {
      if(charNum % LSBS == 0) {
         copy++;
         headNdx = 2;
      }
      thisChar = *copy++ & mask; 
      thisChar |= (*copy++ & mask) << LSBS | (*(copy-headNdx) >> charNum % 3 & 1) << BITS_PER_CHAR - 1;
      headNdx += 2;
      printf("%c", thisChar);
   }
   printf("\n");
}

int main() {
   short *vals = calloc(NUM_VALS, sizeof(short));
   char msg[NUM_CHARS];
   int i, diff;
   short copy[NUM_VALS];
   
   for (i = 0; i < NUM_CHARS; i++)
      msg[i] = getchar();
      
   for (i = 0; i < NUM_VALS; i++) {
      scanf("%hd", &vals[i]);
      copy[i] = vals[i];
   }
   
   EmbedWatermark(vals, msg);
   for (i = 0; i < NUM_CHARS; i++)
      msg[i] = 0;

   for (i = 0; i < NUM_VALS; i++) {
      diff = vals[i] - copy[i];
      if (diff > MAX_DIFF || diff < -MAX_DIFF)
         printf("Val %d differs too much\n", i);
      copy[i] = vals[i];
   }
   
   PrintWatermark(copy);
}
