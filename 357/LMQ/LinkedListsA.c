#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
   int data;          // Could put any kind of data here
   struct Node *next; // Self-referential requires "struct"
} Node;

// Insert a new node as the second one on the list, if there are is already a node, or 
// as the only node on the list, otherwise.
Node *AddSecond(Node *head, int data) {
   Node *temp;
   
   temp = malloc(sizeof(Node));
   temp->data = data;
   if(head) {
      temp->next = head->next;
      head->next = temp;
   }
   else {
      temp->next = NULL;
      head = temp;
   }

   return head;
}

void PrintAll(char *tag, Node *head) {
   Node *temp;
   
   printf("%s:", tag);
   for (temp = head; temp != NULL; temp = temp->next)
      printf(" %d", temp->data);
   printf("\n");
}

int main() {
   Node *head = NULL;
   
   head = AddSecond(head, 10);
   head = AddSecond(head, 20);
   head = AddSecond(head, 15);
   head = AddSecond(head, 12);
   
   PrintAll("After insert", head);
}
