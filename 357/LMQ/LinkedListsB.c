#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
   int data;          // Could put any kind of data here
   struct Node *next; // Self-referential requires "struct"
} Node;

Node *AddAtEnd(Node *head, int data) {
   Node *node, *temp;
   
   temp = calloc(1, sizeof(Node));
   temp->data = data;
   if(node = head) {
      for(; node->next; node = node->next)
         ;
      
      node->next = temp;
   }
   else
      head = temp;

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
   
   head = AddAtEnd(head, 40);
   head = AddAtEnd(head, 30);
   head = AddAtEnd(head, 20);
   head = AddAtEnd(head, 50);
   head = AddAtEnd(head, 10);
   
   PrintAll("After insert", head);
}
