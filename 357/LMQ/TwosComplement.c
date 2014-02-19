#include <stdio.h>
#include <string.h>

void TwosComplement(char *binary) {
   char *cursor = binary;
   // Move to the end
   while(*++cursor)
      ;

   // Move to the first 0 that shouldn't be swapped
   while(*--cursor == '0')
      ;

   // Cursor is now pointing at the least significant 1
   // Replace everything up to that.
   while(binary < cursor) {
      *binary = *binary == '0' ? '1' : '0';
      binary ++;
   }
}

int main() {
   char number[100];
   
   while (EOF != scanf("%99s", number)) {
      TwosComplement(number);
      printf("%s\n", number);
   }
}
