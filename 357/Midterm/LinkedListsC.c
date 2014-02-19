#include <stdio.h>
#include <stdlib.h>
#include "SmartAlloc.h"

typedef struct Node {
   int data;          // Could put any kind of data here
   struct Node *next; // Self-referential requires "struct"
} Node;

Node *Add(int data, Node *oldHead) {
   Node *rtn = malloc(sizeof(Node));
   
   rtn->data = data;
   rtn->next = oldHead;
   return rtn;
}

/* RemoveIndex removes, and frees, the node at index "ndx" from the list, 
with nodes numbered from 0. You may assume that ndx is not negative, and does
not go past the last node of the list. Return the head pointer of the
list after the removal (which might not be the same as before)*/
Node *RemoveIndex(Node *head, int ndx) {
   Node *oldHead = head;
   Node *temp;

   if(ndx) {
      while(--ndx) {
         head = head->next;
      }

      temp = head->next;
      head->next = temp->next;
      free(temp);
   }
   else {
      oldHead = head->next;
      free(head);
   }

   return oldHead;
}

void PrintAll(Node *head) {
   Node *temp;
   
   for (temp = head; temp != NULL; temp = temp->next)
      printf("%d%c", temp->data, temp->next ? ' ' : '\n');
}

void FreeAll(Node *head) {
   Node *temp;
   
   while (head != NULL) {
      temp = head->next;
           free(head);
      head = temp;
   }
}

int main() {
   Node *head = NULL;
   int numNodes, data;

   scanf("%d", &numNodes);
   while (numNodes--) {
      scanf("%d", &data);
      head = Add(data, head);
   }

   while (EOF != scanf("%d", &data)) {
      head = RemoveIndex(head, data);
   }

   PrintAll(head);    
   FreeAll(head);       
   printf("Unfreed space: %d\n", report_space());
}
