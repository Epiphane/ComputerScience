#include <stdio.h>

void SetupAll(int *p, int **pp, int ***ppp, int ****pppp) {
   *pp = p;
   *ppp = pp;
   *pppp = ppp;
}

int main() {
   int i = 42, *p, **pp, ***ppp;
   
   SetupAll(&i, &p, &pp, &ppp);
   if (ppp == &pp && pp == &p && p == &i)
      printf("%u %u %u %u\n", ***ppp, **pp, *p, i);
}
