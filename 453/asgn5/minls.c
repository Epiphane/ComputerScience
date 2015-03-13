#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

void helpAndExit() {
   printf("usage: minls [ -v ] [ -p num [ -s num ] ] imagefile [ path ]\n"
          "Options:\n"
          "-p part    --- select partition for filesystem (default: none)\n"
          "-s sub     --- select subpartition for filesystem (default: none)\n"
          "-h help    --- print usage information and exit\n"
          "-v verbose --- increase verbosity level\n");
   exit(1);
}

int main(int argc, char *argv[]) {
   int partition = -1;
   int subpartition = -1;
   char *imagefile, *path = "/";

   argv ++; argc --; // Move onto first argument
   if (argc <= 0 || strcmp(*argv, "-h") == 0) {
      helpAndExit();
   }
   else {
      if (strcmp(*argv, "-v") == 0) {
         set_verbose(1);
         argv ++; argc --; // Move onto next argument
      }

      // Make sure there are more args
      if (argc <= 0)
         helpAndExit();

      if (strcmp(*argv, "-p") == 0) {
         argv ++; argc --; // Next argument
         
         sscanf(*argv, "%d", &partition);
         argv ++; argc --; // Next argument

         if (argc > 0 && strcmp(*argv, "-s") == 0) {
            argv ++; argc --; // Next argument
            
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
      open_fs(imagefile, partition, subpartition);
      print_file(0, path, TRUE);
   }

   return 0;
}
