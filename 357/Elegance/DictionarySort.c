#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 20

typedef struct Node {
   char word[LEN+1];
   struct Node *next;
} Node;
