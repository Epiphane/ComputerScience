/*
 * pipeit.c
 *
 * CPE 453 Lab 1
 * Executes the command 'ls | sort -r > outfile'
 *
 * Thomas Steinke (tsteinke)
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
   int ls[2];
   int outfile;
   pipe(ls);

   // Child runs 'ls'
   if(!fork()) {
      dup2(ls[1], 1); // Copy pipe output to stdout
      close(ls[1]);   // And then close it
      close(ls[0]);   // Close pipe input

      execlp("ls", "ls", NULL);
      fprintf(stderr, "ERROR: ls call failed\n");
      return 1;
   }
   // Parent waits
   else {
      dup2(ls[0], 0); // Copy pipe input to stdin
      close(ls[0]);   // And then close it
      close(ls[1]);   // Close pipe output

      // Open outfile
      outfile = open("outfile", O_CREAT | O_TRUNC | O_RDWR, "w");
      if(errno) {
         printf("Error opening outfile: %s\n", strerror(errno));
         
         wait(NULL);   
         return 1;
      }

      dup2(outfile, 1); // Copy outfile to stdout
      close(outfile);   // Stop writing to outfile
      if(errno) {
         printf("Error dup2-ing outfile: %s\n", strerror(errno));
         
         wait(NULL);   
         return 1;
      }

      execlp("sort", "sort", "-r", NULL);
      fprintf(stderr, "ERROR: sort -r call failed\n");
      
      wait(NULL);
   }

   return 0;
}
