#include <stdio.h>
#include "SmartAlloc.h"

typedef struct Node
{
   struct Node *next;
   int data;
} Node;

// Begin code

void ReadIntegers(Node **pOdd, Node **pEven) {
   int inputNumber;
   Node *inputNode;
   Node **list, *prev, *cursor;

   *pOdd = *pEven = NULL;
   
   // Continue until you get 0
   while(scanf("%d",&inputNumber) && inputNumber) {
      inputNode = (Node *) malloc(sizeof(Node));
      
      // Initialize the cursor
      list = inputNumber % 2 ? pOdd : pEven;

      // Option 1
      inputNode->next = *list;
      // Put the new, blank node at the beginning
      cursor = *list = inputNode;
      while(cursor->next && cursor->next->data < inputNumber) {
         cursor->data = cursor->next->data;
         cursor = cursor->next;
      }
      cursor->data = inputNumber;
   }
}

// End code

int main() {
   Node *odd, *even;
   Node *temp;
   
   ReadIntegers(&odd, &even);
   
   temp = odd;
   printf("Odd values:");
   while (temp) {
      printf(" %d", temp->data);
      temp = temp->next;
   }
   
   temp = even;
   printf("\nEven values:");
   while (temp) {
      printf(" %d", temp->data);
      temp = temp->next;
   }
   printf("\nUsed %d bytes.\n", report_space());
}
