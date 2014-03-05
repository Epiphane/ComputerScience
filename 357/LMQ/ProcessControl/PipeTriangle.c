#include <stdio.h>
#include <unistd.h>

#define CCOUNT 3

/* Send our pid on "out" for the next process in the triangle to receive. Run
 * a "bucket brigade", getting a pid from the prior process in the triangle  
 * via "in", and passing it along, so that all processes see each pid once.
 * Total up the pids and send them to the parent via "report". */
void SendTotal(int report, int in, int out, int pid) {
   int i, total = pid;
   
   for(i = 1; i < CCOUNT; i ++) {
      write(out, &pid, sizeof(pid));
      
      read(in, &pid, sizeof(pid));
      total += pid;
   }

   write(report, &total, sizeof(total));

   close(out);
   /* Check that there is no lingering data on "in" */
   if (0 != read(in, &pid, sizeof(pid)))
      printf("Missing EOF on pipe.");

   /* Relevant cleanup */
   close(out);
   close(report);
}

int main () {
   int i, pid, proc, checkTotal, pidTotal;

   // Pipes to parent, and between three children A, B, C
   int reportFds[2], AtoB[2], BtoC[2], CtoA[2]; 
   
   pipe(reportFds);
   pipe(CtoA);
   pipe(AtoB);

   if (!fork()) { /* Child A */
      close(CtoA[1]);

      SendTotal(reportFds[1], CtoA[0], AtoB[1], getpid());
   }
   else {
      /* Set up for child B */
      close(AtoB[1]);
      pipe(BtoC);

      if (!fork()) { // Do child B
         close(CtoA[1]);

         SendTotal(reportFds[1], AtoB[0], BtoC[1], getpid());
      }
      else {
         /* Set up for child C */
         close(BtoC[1]);

         if (!fork()) { // Do child C
            close(reportFds[0]);
            SendTotal(reportFds[1], BtoC[0], CtoA[1], getpid());
         }
         else {
            close(CtoA[1]);
            close(reportFds[1]);

            for (i = pidTotal = 0; i < CCOUNT; i++) {
               pidTotal += wait(NULL);
               printf("Child exits..\n");
            }
            for (i = 0; i < CCOUNT; i++)
               if (0 < read(reportFds[0], &checkTotal, sizeof(checkTotal))
                && checkTotal == pidTotal)
                  printf("Good report\n");

            if (0 == read(reportFds[0], &checkTotal, sizeof(checkTotal)))
               printf("Good EOF\n");

            close(reportFds[0]);
         }
      }
   }

   return 0;
}
