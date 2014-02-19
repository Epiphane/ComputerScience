#include <stdio.h>

int main() {
   int x = 1, y = 5, z = 23;
   char *p = "hello";
   int b = 23, c = 19, i = 13, s1 = 10, s2 = 7;
   int j = 20;

   printf("%d vs. %d\n", (y = ((x & y) || (x | y && z))) == 0, 
(y = x & y || x | y && z) == 0);

   printf("%d vs. %d\n", x = (( x + y) ? (*(p++) & b) ^ c : i << (s1 + s2) ) + 1, 
            x =  x + y ? *p++ & b ^ x : i << s1 + s2 + 1);

   printf("%.2f vs %.2f\n", (float)(i << ( (j ^ i) << 4)),  (float)(i << (( j ^ i )<< 4)));
}
