/*
 * Dining Philosophers problem
 *
 * CPE 453 Assignment 3
 *
 * Author: Thomas Steinke (tsteinke@calpoly.edu)
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#define NUM_PHILOSOPHERS 5
#define NONE 0
#define EAT 1
#define THINK 2

// Width of a print: | ----- Think (14)
#define SECTION_WIDTH 14

void dawdle();
void *philosopher(void *id);
void print_philosophers();
void print_line();

typedef struct Philosopher {
   int left, right, state;
} Philosopher;

Philosopher philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_mutex_t printing;

void lock_printing() {
   if(pthread_mutex_lock(&printing) != 0) {
      fprintf(stderr, "Locking printer: %s\n", strerror(errno));
      exit(-1);
   }
}

void unlock_printing() {
   if(pthread_mutex_unlock(&printing) != 0) {
      fprintf(stderr, "Unlocking printer: %s\n", strerror(errno));
      exit(-1);
   }
}

int main(int argc, char *argv[]) {
   int i;

   int         id[NUM_PHILOSOPHERS]; // Individual identifier
   pthread_t   childid[NUM_PHILOSOPHERS]; // Activations for each child

   // Initialize mutex on printing
   if(pthread_mutex_init(&printing, NULL) != 0) {
      fprintf(stderr, "Printing mutex: %s\n", strerror(errno));
      exit(-1);
   }

   // Initialize info for philosophers
   // and the fork on their "left"
   for(i = 0; i < NUM_PHILOSOPHERS; i ++) {
      id[i] = i;
      philosophers[i].left = philosophers[i].right = 0;
      philosophers[i].state = NONE;

      if(pthread_mutex_init(&forks[i], NULL) != 0) {
         fprintf(stderr, "Fork %d: %s\n", i, strerror(errno));
         exit(-1);
      }
   }

   // Output the head of the page
   lock_printing();
   print_line();
   for(i = 0; i < NUM_PHILOSOPHERS; i ++) {
      int padding_width = (SECTION_WIDTH - 2) / 2;
      printf("|%*c%c%*c", padding_width, ' ', 'A' + i, padding_width, ' ');
   } 
   printf("|\n");
   print_line();
   unlock_printing();

   for(i = 0; i < NUM_PHILOSOPHERS; i ++) {
      int res;
      res = pthread_create(&childid[i], // Where to put identifier
                           NULL,        // No special options
                           philosopher,       // function child()
                           (void *) (&id[i])); // id[i]
      
      if(res == -1) { // Error?
         fprintf(stderr, "Child %d: %s\n", i, strerror(errno));
         exit(-1);
      }
   }

   // Wait for all children to finish
   for(i = 0; i < NUM_PHILOSOPHERS; i ++) {
      pthread_join(childid[i], NULL);
      printf("Parent: child %d exited.\n", i);
   }

   // Put away all the forks
   for(i = 0; i < NUM_PHILOSOPHERS; i ++) {
      if(pthread_mutex_destroy(&forks[i]) != 0) {
         fprintf(stderr, "Fork %d: %s\n", i, strerror(errno));
         exit(-1);
      }
   }

   return 0;
}

void print_line() {
   int i, j;
   for(i = 0; i < NUM_PHILOSOPHERS; i ++) {
      printf("|");
      for(j = 0; j < SECTION_WIDTH - 1; j ++)
         printf("=");
   }
   printf("|\n");
}

void print_philosophers() {
   int i, j;
   for(i = 0; i < NUM_PHILOSOPHERS; i ++) {
      printf("| ");
      
      // Print forks
      for(j = 0; j < NUM_PHILOSOPHERS; j ++) {
         if((j == i      && philosophers[i].left) ||
            (j == i + 1  && philosophers[i].right)) {
            printf("%d", j);
         }
         else
            printf("-");
      }

      if(philosophers[i].state == NONE)
         printf("       ");
      else if(philosophers[i].state == EAT)
         printf(" Eat   ");
      else if(philosophers[i].state == THINK)
         printf(" Think ");
   }

   printf("|\n");
}

/*
 * Executed as the body of each child thread.
 *
 * INPUT: Pointer to an integer, its ID.
 */
void *philosopher(void *id) {
   int whoami = *(int *)id;

   printf("Child %d (%d):    Hello. \n", whoami, (int) getpid());
   printf("Child %d (%d):    Goodbye. \n\n", whoami, (int) getpid());

   return NULL;
}

/*
 * Sleep for a random amount of time between 0 and 999 milliseconds.
 *
 * nanosleep() is from -lrt
 */
void dawdle() {
   struct timespec tv;
   int msec = (int) (((double) random() / INT_MAX) * 1000);

   tv.tv_sec = 0;
   tv.tv_nsec = 1000000 * msec;
   if(nanosleep(&tv, NULL) == -1) {
      perror("nanosleep");
   }
}
