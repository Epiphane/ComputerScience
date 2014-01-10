#include <stdio.h>
#define MAX 100

// One pair is "less" than another if either its val1 is less than the other's val1, or if
// val1's are tied, then if its val2 is less than the other's val2.
typedef struct {
   int val1;
   int val2;
} Pair;

// Swap takes two pairs, and makes sure the first is less than or equal to the second, 
// exchanging their values if needed.
void Swap(Pair *p1, Pair *p2) {
   // Swap the two if val1 is smaller for the second one,
   // Or val1 is equal and val2 is smaller for the second
   if(p2->val1 < p1->val1 || p2->val1 == p1->val1 && p2->val2 < p1->val2) {
      Pair temp = *p1;
      *p1 = *p2;
      *p2 = temp;
   }
}

// SortThree takes three Pairs, and changes their values so that the first one has the lowest
// value, the second the next lowest, and the third the highest. (Start by putting the first
// two in order via a Swap call, then work the third one into the order by two more calls of Swap.)
void SortThree(Pair *p1, Pair *p2, Pair *p3) {
   Swap(p1, p2);
   Swap(p2, p3);
   Swap(p1, p2);
}

int main() {
   Pair pair1 = {4, 2}, pair2 = {3, 2}, pair3 = {4, 1};

   // Call SortThree for pair1, pair2, and pair3
   SortThree(&pair1, &pair2, &pair3);
   
   printf("pair1: %d %d  pair2: %d %d  pair3: %d %d\n", pair1.val1, pair1.val2, pair2.val1,
      pair2.val2, pair3.val1, pair3.val2);
                   
   // Now call SortThree for pair3, pair2, pair1, so that they're in descending order.
   SortThree(&pair3, &pair2, &pair1);
   printf("pair1: %d %d  pair2: %d %d  pair3: %d %d\n", pair1.val1, pair1.val2, pair2.val1,
      pair2.val2, pair3.val1, pair3.val2);
}
