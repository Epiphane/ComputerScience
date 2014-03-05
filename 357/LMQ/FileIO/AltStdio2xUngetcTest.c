#include "AltStdio.h"

int main() {
   int v1, v2, v3;
   
   v1 = altGetchar();
   v2 = altGetchar();
   while (EOF != (v3 = altGetchar())) {
      altPutchar(v1);
      altPutchar(v2);
      altPutchar(v3);
      altUngetc(v1, altStdin);
      altUngetc(v2, altStdin);
      v1 = altGetchar();
      v2 = altGetchar();
   }
   altFflush(altStdout);
}
