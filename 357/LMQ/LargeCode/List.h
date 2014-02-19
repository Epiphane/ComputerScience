#ifndef LIST_H
#define LIST_H

typedef struct List List;

typedef enum {LIST_AT_START, LIST_AT_END} ListAddType;

List *ListCreate();
void ListDestroy(List *list);
int ListGetCount(List *list);
void ListAdd(List *list, double toAdd, ListAddType where);
int ListDrop(List *list, double toDrop);

// Iterators
void ListStart(List *list);
int ListHasNext(List *list);
double ListCurrent(List *list);
void ListAdvance(List *list);

void ListClearFreelist();
void ListDestroyAll();

#endif
