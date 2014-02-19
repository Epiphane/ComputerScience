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

/* RemoveRange removes, and frees, the nodes from index "start" (inclusive) to
 * index "end" (exclusive) from the list, with nodes numbered from 0. DO NOT
 * assume that start and end are valid indices. Leave the list unchanged if
 * start is negative, if end is greater than the number of nodes in the list, or
 * if start is greater than end. Return the head pointer of the list after the
 * removal (which might not be the same as before)
 */
Node *RemoveRange(Node *head, int start, int end) {
   Node *temp;
   Node *startPtr, *cursor, *endPtr;
   int newHead = start;

   startPtr = endPtr = head;

   if(start >= 0 && end > start) {
      while(endPtr && --end)
         endPtr = endPtr->next;

      while(startPtr && --start > 0)
         startPtr = startPtr->next;

      cursor = startPtr->next;
      // If we actually got to our node
      if(end <= 0) {
         while(cursor != endPtr) {
            temp = cursor->next;
            free(cursor);
            cursor = temp;
         }
         startPtr->next = cursor->next;
         free(cursor);
      }
   }

   if(!newHead) {
      temp = startPtr->next;
      free(head);
      head = temp;
   }

   return head;
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
   int numNodes, data, start, end;

   scanf("%d", &numNodes);
   while (numNodes--) {
      scanf("%d", &data);
      head = Add(data, head);
   }
   
   scanf("%d", &start);
   scanf("%d", &end);

   head = RemoveRange(head, start, end);
   PrintAll(head);
   FreeAll(head);
	
   printf("Unfreed space: %d\n", report_space());
   return 0;
}
