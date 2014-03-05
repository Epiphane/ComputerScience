#include <stdio.h>

int charCount(char str[], char c);

int main()
{
   char word[20], search, result;

   printf("Input a word: ");
   scanf("%s", word);
   printf("Character to search for: ");
   scanf(" %c", &search);

   result = charCount(word, search);

   printf("Occurs %d times!\n", result);
 
   return 0;
}

int charCount(char *str, char c)
{
   int result;

   if (*str == 0)
      result = 0;
   else if (*str == c)
      result = 1 + charCount(str+1, c);
   else
      result = charCount(str+1, c);
   
   return result;
}
   
