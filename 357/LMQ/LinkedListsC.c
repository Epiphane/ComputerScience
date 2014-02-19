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

// Remove the second node from the list, or if there are not two nodes, leave the list unchanged.
void RemoveSecond(Node *head) {
   Node *temp;

   if(head && head->next){
      temp = head->next;
      head->next = temp->next;   
      free(temp);
   }
}

void PrintAll(char *tag, Node *head) {
   Node *temp;
   
   printf("%s:", tag);
   for (temp = head; temp != NULL; temp = temp->next)
      printf(" %d", temp->data);
   printf("\n");
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
   
   RemoveSecond(head);
   PrintAll("Step 1", head);
   head = Add(10, head);
   RemoveSecond(head);
   PrintAll("Step 2", head);
   head = Add(20, head);
   RemoveSecond(head);
   PrintAll("Step 3", head);
   head = Add(40, Add(30, Add(20, head)));
   RemoveSecond(head);
   PrintAll("Step 4", head);
   
   FreeAll(head);       
   printf("Unfreed space: %d\n", report_space());
}
