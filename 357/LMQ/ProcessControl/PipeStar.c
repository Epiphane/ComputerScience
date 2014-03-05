#include <stdio.h>
#include <unistd.h>

#define PROCS 5

int main () {
   int pid, proc, checkTotal, pidTotal = 0, reportFds[2];
   
   pipe(reportFds);

   /* Create PROCS children, call them child 0 through child PROCS-1 */
   for (proc = 0; proc < PROCS; proc++) {
      if (!(pid = fork())) {
         pid = getpid();
         write(reportFds[1], &pid, sizeof(pid));
         close(reportFds[1]);
         close(reportFds[0]);
      }
      else {
         pidTotal += pid;
      }
   }

   close(reportFds[1]);
   
   for (proc = 0; proc < PROCS; proc++)
      if (0 < read(reportFds[0], &pid, sizeof(pid))) {
         printf("Good read\n");
         pidTotal -= pid;
      }

   if (pidTotal == 0 && 0 == read(reportFds[0], &pid, sizeof(pid)))
         printf("Good report and EOF\n");

   return 0;
}
