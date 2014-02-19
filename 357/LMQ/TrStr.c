#include <stdio.h>

#define strchr
#define strlen

void TrStr(char *string, char* replace) {
	char *cursor;
	for(; *string; string ++) {
		for(cursor = replace; *cursor && *string != *cursor++; cursor ++)
			;
		
		if(*cursor)
			*string = *cursor;
	}
}

int main() {
   char str1[20], str2[20], *test = "";

   TrStr(test, "abcd");
   printf("Empty translates to |%s|\n", test);

   while (EOF != scanf("%s %s", str1, str2)) {
      TrStr(str1, "");
      printf("%s -> ", str1);
      TrStr(str1, str2);
      printf("%s\n", str1);
   }
   
   return 0;
}
