#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 20

typedef struct Node {
   char word[LEN+1];
   struct Node *next;
} Node;

int main() {
   char input[LEN];
   Node *head, *cursor, *newNode;

   head = calloc(1, sizeof(Node));

   while(scanf("%s ",input) != EOF) {
      newNode = malloc(sizeof(Node));
      strcpy(newNode->word, input);
      
      cursor = head;
      while(cursor->next && strcmp(input,cursor->next->word) > 0)
         cursor = cursor->next;

      newNode->next = cursor->next;
      cursor->next = newNode;
   }

   while(head = head->next) {
      printf("%s",head->word);
      if(head->next)
         printf(" ");
      else
         printf("\n");
   }
}
