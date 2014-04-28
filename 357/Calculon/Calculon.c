#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Calculon.h"

int main(int argc, char *argv[]) {
   FILE *suite;  
   char temp[MAX_SUITE_LINE], *suiteDir;
   int status = 0, ndx = 0;
   Program *programs;

   sprintf(temp, ".%d", getpid());
   if (mkdir(temp, S_IRUSR | S_IWUSR | S_IXUSR)) {
      fprintf(stderr, "Error: Could not create directory %s\n", temp);
      status = NO_DIR;
   }   
   else {
      if (argc < 2) {
         fprintf(stderr, "Error: Not enough arguments\n");
         status = NO_ARG;
      }
      else {
         suite = fopen(suiteDir = argv[1], "r");
         if (suite < 0) {
            fprintf(stderr, "Error: Could not open %s\n", argv[1]);
            status = NO_SUITE;
         }

         while (suiteDir[ndx] && suiteDir[ndx] != '.')
            ndx++;
         suiteDir[ndx] = 0;
         
         status = ParseSuite(suite, suiteDir, temp, &programs);
      
         if (chdir(temp))
            fprintf(stderr, "Call Failed: chdir(%s)\n", temp);
         else {
            RunSuite(programs);
            if (chdir(".."))
               fprintf(stderr, "Call Failed: chdir(..)\n");
         }
      } 

      RemoveTempDirectory(temp);
   }

   //DontScrollDown();

   return status;
}

void RemoveTempDirectory(char *directory) {
   char temp[MAX_SUITE_LINE];
   DIR *tempDir;
   struct dirent *file;   

   if (chdir(directory))
      fprintf(stderr, "Call Failed (RTD): chdir(%s)\n", directory);
   else {
      tempDir = opendir(".");
      
      while (file = readdir(tempDir)) {
         unlink(file->d_name);
      }
      closedir(tempDir);   

      if (chdir(".."))
         fprintf(stderr, "Call Failed: chdir(..)\n");
      if (rmdir(directory))
         fprintf(stderr, "Call Failed: rmdir(%s)\n", directory);
   }
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
   fprintf(stderr, "                                      ");
   fprintf(stderr, "            ____________          \n");
   fprintf(stderr, "                                   --)");
   fprintf(stderr, "-----------|____________|         \n");
  
   fprintf(stderr, "                                      ");
   fprintf(stderr, "        ,'       ,'               \n");
   fprintf(stderr, "                -)------========      ");
   fprintf(stderr, "      ,'  ____ ,'                 \n");
   fprintf(stderr, "                         `.    `.     ");
   
   fprintf(stderr, "    ,'  ,'__ ,'                   \n");
   fprintf(stderr, "                           `.    `.   ");
   fprintf(stderr, "  ,'       ,'                     \n");
   fprintf(stderr, "                             `.    `._");
   fprintf(stderr, ",'_______,'__                     \n");

   fprintf(stderr, "                               [._ _| ");
   fprintf(stderr, "^--      || |                     \n");
   fprintf(stderr, "                       ____,...-----|_");
   fprintf(stderr, "_________ll_|\\                    \n");
   fprintf(stderr, "      ,.,..-------\"\"\"\"\"     \"---");

   fprintf(stderr, "-'                 ||                   \n");
   fprintf(stderr, "  .-\"\"  |===========================");
   fprintf(stderr, " ______________ |                   \n");
   fprintf(stderr, "   \"-...l_______________________    |");
   fprintf(stderr, "  |'      || |_]                   \n");

   fprintf(stderr, "                                [`-.|_");
   fprintf(stderr, "_________ll_|                     \n");
   fprintf(stderr, "                              ,'    ,'");
   fprintf(stderr, " `.        `.                     \n");
   fprintf(stderr, "                            ,'    ,'  ");

   fprintf(stderr, "   `.    ____`.                   \n");
   fprintf(stderr, "                -)---------========   ");
   fprintf(stderr, "     `.  `.____`.                 \n");
   fprintf(stderr, "                                      ");
   fprintf(stderr, "       `.        `.               \n");

   fprintf(stderr, "    By Unknown                        ");
   fprintf(stderr, "         `.________`.             \n");
   fprintf(stderr, "    Patched by LS              --)----");
   fprintf(stderr, "---------|___________|            \n");
   fprintf(stderr, "\n");
}
