#include <stdio.h>
#include "malloc.h"

typedef struct Object {
  int x, y, z;
  char letter;
} Object;

typedef struct LargeObject {
  char buffer[1024];
} LargeObject;

int main(int argc, char *argv[]) {

  LargeObject *obj1 = malloc(sizeof(LargeObject));
  Object *obj2 = malloc(sizeof(Object));
  LargeObject *obj3 = malloc(sizeof(LargeObject));
  Object *obj4 = malloc(sizeof(Object));

  free(obj4);
  LargeObject *obj5 = malloc(sizeof(LargeObject));

  free(obj1);
  free(obj2);
  free(obj3);
  free(obj5);

  return 0;
}
