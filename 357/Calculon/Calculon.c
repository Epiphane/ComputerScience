#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Calculon.h"

int main(int argc, char *argv[]) {
   FILE *suite;  
   char temp[MAX_SUITE_LINE];
   int status;   

   suite = fopen(argv[1], "r");
   if(suite < 0) {
      fprintf(stderr, "Error opening %s\n", argv[1]);
      status = NO_SUITE;
   }

   sprintf(temp, ".%d", getpid());
   if(mkdir(temp, S_IRUSR | S_IWUSR | S_IXUSR)) {
      fprintf(stderr, "Error creating directory %s\n", temp);
      status = NO_DIR;
   }   
   else
      status = RunSuite(suite, temp);
   
   //if(!status)
      DontScrollDown();

   return status;
}

/*
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~*/
void DontScrollDown() {
   fprintf(stderr, "        .                            .                      .         \n");
   fprintf(stderr, ".                  .             -)------+====+       .               \n");
   fprintf(stderr, "                         -)----====    ,'   ,'   .                 .  \n");
   fprintf(stderr, "            .                  `.  `.,;___,'                .         \n");

   fprintf(stderr, "                                `, |____l_\\                          \n");
   fprintf(stderr, "                 _,.....------c==]\"\"______ |,,,,,,.....____ _        \n");
   fprintf(stderr, " .      .       \"-:______________  |____l_|]'''''''''''       .     .\n");
   fprintf(stderr, "                               ,'\"\",'.   `.                          \n");
   fprintf(stderr, "      .                 -)-----====   `.   `.              LS        \n");

   fprintf(stderr, "                  .            -)-------+====+       .            .\n");
}
