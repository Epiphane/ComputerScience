#include <stdio.h>

int main() {
  int value1 = 10;
  char string[] = "Hi";
  int value2 = 50;
  printf("Working - %x, %x, %x\n",&value1,string,&value2);
  printf("The string is %s\n",string);
  printf("%s - located at %x\n",string,string);

  strcpy(string, "Yo");
  printf("%s - located at %x\n",string,string);

  strcpy(string, "A");
  printf("%s - located at %x\n",string,string);

  strcpy(string, "Silver!!!!!!!!!!!!!!!!!");
  printf("%s - located at %x\n",string,string);

  printf("%d - located at %x\n",value1,&value1);
  printf("%d - located at %x\n",value2,&value2);
}
