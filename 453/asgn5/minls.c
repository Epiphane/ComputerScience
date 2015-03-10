#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

void helpAndExit() {
   printf("usage: minls [ -v ] [ -p num [ -s num ] ] imagefile [ path ]\n"
          "Options:\n"
          "-p part\t--- select partition for filesystem (default: none)\n"
          "-s sub\t--- select subpartition for filesystem (default: none)\n"
          "-h help\t--- print usage information and exit\n"
          "-v verbose\t--- increase verbosity level\n");
   exit(0);
}

int main(int argc, char *argv[]) {
   int verbose = 0;
   int partition = -1;
   int subpartition = -1;
   char *imagefile, *path = "/";
   struct inode *file;

   argv ++; argc --; // Move onto first argument
   if (argc <= 0 || strcmp(*argv, "-h") == 0) {
      helpAndExit();
   }
   else {
      if (strcmp(*argv, "-v") == 0) {
         verbose = 1;
         argv ++; argc --; // Move onto next argument
      }

      // Make sure there are more args
      if (argc <= 0)
         helpAndExit();

      if (strcmp(*argv, "-p") == 0) {
         sscanf(*argv, "%d", &partition);
         argv ++; argc --; // Next argument

         if (argc > 0 && strcmp(*argv, "-s") == 0) {
            sscanf(*argv, "%d", &subpartition);
            argv ++; argc --; // Next argument
         }
      }

      if (argc <= 0)
         helpAndExit();

      imagefile = *argv;
      argv ++; argc --;
      
      if (argc > 0)
         path = *argv;

      // Read image file
      open_fs(imagefile, partition, subpartition, verbose);
      file = find_file(path);
   }

   return 0;
}
