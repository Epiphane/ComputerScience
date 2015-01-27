/*
 * lwp.c (libPLN)
 * Lightweight process management
 *
 * CPE 453 Assignment 2
 *
 * Author: Thomas Steinke (tsteinke@calpoly.edu)
 */

#include "lwp.h"
#include <stdlib.h>
#include <stdio.h>

void RoundRobin_init() {}
void RoundRobin_shutdown() {}
void RoundRobin_admit(thread new) {
   printf("WHAT");
}

void RoundRobin_remove(thread victim) {

}

tid_t RoundRobin_next() {
   return 0;
}

scheduler RoundRobin;
scheduler currentScheduler = NULL;

/* Given a thread id, fetch a pointer to its context */
thread getThread(tid_t target) {
   thread cursor = lwp_tlist;  // Cursor for moving through list

   // Check each thread
   while(cursor) {
      if(cursor->tid == target) {
         return cursor;        // Found it!
      }

      // Else move forward
      cursor = cursor->tlist_next;
   }

   // None found
   return NULL;
}

thread lwp_tlist = NULL;
tid_t  next_tid  = 0;

/*
 * Create a new lightweight process which calls the `function` with the
 * given argument. The new process's stack will be `stacksize` words.
 *
 * Returns the (lightweight) thread id of the new thread or -1 on error
 */
tid_t lwp_create(lwpfun function, void *argument, size_t stacksize) {
   context *thread = malloc(sizeof(context));
   
   // Initialize thread
   thread->stack = malloc(stacksize);
   thread->stacksize = stacksize;
   thread->tid = next_tid++;

   // Add it to list
   thread->tlist_next = lwp_tlist;
   lwp_tlist = thread;

   // Add it to scheduler
   if(!currentScheduler) {
      currentScheduler = RoundRobin = calloc(1, sizeof(RoundRobin));
      RoundRobin->admit = &RoundRobin_admit;
      RoundRobin->remove = &RoundRobin_remove;
      RoundRobin->next = &RoundRobin_next;
   }

   currentScheduler->admit(thread);

   return thread->tid;
}

/*
 * Returns the tid of the calling LWP. Returns undefined if not called by a LWP
 */
tid_t lwp_getid();

/*
 * Yields control to another LWP. Which process to start depends on the
 * scheduler. Saved the current context, picks the next one, restores that
 * thread's context, and returns
 */
void lwp_yield();

/*
 * Terminates the current LWP and frees its resources. Calls sched->next()
 * to get the next thread. If no other threads, calls lwp_stop();
 */
void lwp_exit();

/*
 * Starts the LWP system. Saves the original context, picks an LWP, and starts
 * it. If no LWPs, returns immediately.
 */
void lwp_start();

/*
 * Stops the LWP system, restores the original context. Thread processing will
 * be restored by a call to lwp_start, if applicable
 */
void lwp_stop();

/*
 * Causes the LWP package to use the function scheduler to choose its next
 * process. Transfers all threads from the old scheduler to the new one in
 * next() order.
 *
 * If scheduler is NULL or has never been set, default to round robin
 */
void lwp_set_scheduler(scheduler newScheduler) {
   printf("ohmy\n");

   newScheduler->init();

   tid_t child_tid = currentScheduler->next();
   thread child;
   while(child_tid != NO_THREAD) {
      // Move child to new scheduler
      child = getThread(child_tid);
      newScheduler    ->admit(child);
      currentScheduler->remove(child);

      child_tid = currentScheduler->next();
   }

   // Swap schedulers
   currentScheduler->shutdown();
   currentScheduler = newScheduler;
}
