/*
 * minget and minls - Minix file management 
 *
 * CPE 453 Assignment 5
 *
 * Author: Thomas Steinke (tsteinke@calpoly.edu)
 */

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include "file.h"

void helpAndExit() {
   printf("usage: "
          "minget [ -v ] [ -p num [ -s num ] ] imagefile srcpath [ dstpath ]\n"
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
   int output = 0;
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

      path = *argv;
      argv ++; argc --;
      
      if (argc > 0) {
         output = open(*argv, O_CREAT | O_TRUNC | O_WRONLY);
         if (output == -1) {
            fprintf(stderr, "Error opening %s: %s\n", *argv, strerror(errno));
            return 1;
         }
      }

      // Read image file
      open_fs(imagefile, partition, subpartition);
      print_file(output, path, FALSE);
   }

   return 0;
}
