#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM1 6
#define DIM2 8
#define NAME_LEN 20

#define qsort NOT_ALLOWED


typedef struct Name {
   char first[NAME_LEN+1];
   char last[NAME_LEN+1];
} Name;

// Support functions NameLess, NameGreater, DblLess, and StrLess
int NameLess(void *name1, void *name2) {
   Name *p1 = (Name *)name1;
   Name *p2 = (Name *)name2;
   int comp = strcmp(p2->last, p1->last);
   if(!comp)
      comp = strcmp(p2->first, p1->first);

   return comp;
}

int NameGreater(void *name1, void *name2) {
   Name *p1 = (Name *)name1;
   Name *p2 = (Name *)name2;

   return strcmp(p1->last, p2->last) || strcmp(p1->first, p2->first); 
}

int DblLess(void *dbl1, void *dbl2) {
   return *(double *)dbl1 < *(double *)dbl2;
}

int StrLess(void *str1, void *str2) {
   return strcmp(*((char **)str2), *((char **)str1));
}
 
// Function GenInsertSort
void GenInsertSort(void *list, int dim, int size, int (*compare) (void *, void *)) {
   char *cursor, *listHead, *temp;
   int ndx;
   
   temp = malloc(size);
   cursor = listHead = (char *)list;

   for(ndx = size; ndx < size * dim; ndx += size) {
      cursor = listHead+ndx;
      memcpy(temp, cursor, size);
      while(cursor > listHead && compare(temp, cursor - size) > 0) {
         memcpy(cursor, cursor - size, size);
         cursor -= size;
      }
      memcpy(cursor, temp, size);
   }
}

void main() {
   Name names[DIM1] = {{"John", "Smith"}, {"Jane", "Smith"}, {"Bill", "Jones"},
      {"Sue", "Johnson"}, {"Susan", "Johnson"}, {"Jim", "Taylor"}
   };
   double vals[DIM2] = {1.1, -2.2, 3.3, -4.2, 5.5, -6.6, 7.7, -8.8};
   char *words[DIM2]
    = {"alpha", "beta", "omega", "pi", "phi", "gamma", "delta", "epsilon" };
   int i;
   
   GenInsertSort(names, DIM1, sizeof(Name), NameLess);
   for (i = 0; i < DIM1; i++)
      printf("%s %s\n", names[i].first, names[i].last);
          
   GenInsertSort(names, DIM1, sizeof(Name), NameGreater);   
   for (i = 0; i < DIM1; i++)
      printf("%s %s\n", names[i].first, names[i].last);

   GenInsertSort(vals, DIM2, sizeof(double), DblLess);
   for (i = 0; i < DIM2; i++)
      printf("%f ", vals[i]);
   printf("\n");
   
   GenInsertSort(words, DIM2, sizeof(char *), StrLess);
   for (i = 0; i < DIM2; i++)
      printf("%s ", words[i]);
   printf("\n");
}
