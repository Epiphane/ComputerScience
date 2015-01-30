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

scheduler RoundRobin;
scheduler currentScheduler = NULL;
thread lwp_tlist = NULL, currentThread = NULL;
tid_t  next_tid  = 0;

void RoundRobin_init() {
   currentThread = NULL;
}
void RoundRobin_shutdown() {}
void RoundRobin_admit(thread new) {
   if(!lwp_tlist) {
      // If empty list, make this the list
      lwp_tlist = new->tprev = new->tnext = new;
      lwp_tlist->tlist_next = NULL;
   }
   else {
      // Otherwise, do this:
      // lwp_tlist: new -> (old lwp_tlist)
      // schedule: lwp_tlist -> new -> (lwp_tlist->tnext)
      new->tprev = lwp_tlist;
      new->tnext = lwp_tlist->tnext;

      lwp_tlist->tnext->tprev = new;
      lwp_tlist->tnext = new;

      new->tlist_next = lwp_tlist;
      lwp_tlist = new;
   }
}

void RoundRobin_remove(thread victim) {
   if(victim == lwp_tlist) {
      // If it's the head of the list, change it
      lwp_tlist = lwp_tlist->tlist_next;
   }
   else {
      // erase from lwp_tlist
      thread cursor = lwp_tlist;
      while(cursor->tlist_next != victim)
         cursor = cursor->tlist_next;

      cursor->tlist_next = victim->tlist_next;
   }

   // Remove from schedule
   victim->tprev->tnext = victim->tnext;
   victim->tnext->tprev = victim->tprev;
}

tid_t RoundRobin_next() {
   // Start at the first node
   if(!currentThread)
      currentThread = lwp_tlist;

   currentThread = currentThread->tnext;
   return currentThread->tid;
}

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

/*
 * Create a new lightweight process which calls the `function` with the
 * given argument. The new process's stack will be `stacksize` words.
 *
 * Returns the (lightweight) thread id of the new thread or -1 on error
 */
tid_t lwp_create(lwpfun function, void *argument, size_t stacksize) {
   context *thread = malloc(sizeof(context));
   
   // Initialize thread
   thread->stack = malloc(stacksize * sizeof(unsigned long));
   thread->stacksize = stacksize;
   thread->tid = next_tid++;

   // Add it to scheduler
   if(!currentScheduler) {
      currentScheduler = RoundRobin = calloc(1, sizeof(struct scheduler));
      RoundRobin->admit = &RoundRobin_admit;
      RoundRobin->remove = &RoundRobin_remove;
      RoundRobin->next = &RoundRobin_next;
   }

   currentScheduler->admit(thread);

   //SetSP(thread->stack);
   save_context(&thread->state);

   // Put return address (function) at the bottom of the stack
   // which grows large->small APPARENTLY
   unsigned long *bottom = (void *)thread->stack + thread->stacksize * sizeof(unsigned long);
   *bottom = (unsigned long) function;
   *(bottom - 1) = thread->state.rbp;

   thread->state.rsp = bottom;
   thread->state.rbp = bottom - 1;
   thread->state.rdi = argument;
   
   return thread->tid;
}

/*
 * Returns the tid of the calling LWP. Returns undefined if not called by a LWP
 */
tid_t lwp_getid() {
   if(currentThread) {
      return currentThread->tid;
   }
   return -1;
}

/*
 * Yields control to another LWP. Which process to start depends on the
 * scheduler. Saved the current context, picks the next one, restores that
 * thread's context, and returns
 */
void lwp_yield() {
   // Save old thread's context
   save_context(&currentThread->state);
   currentThread = getThread(currentScheduler->next());
   
   // Load new thread's context
   load_context(&currentThread->state);
}

/*
 * Terminates the current LWP and frees its resources. Calls sched->next()
 * to get the next thread. If no other threads, calls lwp_stop();
 */
void lwp_exit_real(thread old) {
   // Free the old stack
   free(old->stack);
   free(old);
   load_context(&currentThread->state);
}

void lwp_exit() {
   thread victim = currentThread;
  
   // Get new thread from scheduler
   currentThread = getThread(currentScheduler->next());
   currentScheduler->remove(victim);
  
   if(!lwp_tlist) // No threads
      lwp_stop();
   else // More threads
      lwp_exit_real(victim);
}

/*
 * Starts the LWP system. Saves the original context, picks an LWP, and starts
 * it. If no LWPs, returns immediately.
 */
rfile mainThread;
void lwp_start() {
   if(lwp_tlist) {
      // Save original
      GetSP(mainThread.rsp);
      save_context(&mainThread);

      // Load first thread
      currentThread = getThread(currentScheduler->next());
      load_context(&currentThread->state);
   }
}

/*
 * Stops the LWP system, restores the original context. Thread processing will
 * be restored by a call to lwp_start, if applicable
 */
void lwp_stop() {
   // Restore main thread
   SetSP(mainThread.rsp);
   load_context(&mainThread);
}

/*
 * Causes the LWP package to use the function scheduler to choose its next
 * process. Transfers all threads from the old scheduler to the new one in
 * next() order.
 *
 * If scheduler is NULL or has never been set, default to round robin
 */
void lwp_set_scheduler(scheduler newScheduler) {
   // Initialize scheduler
   if(newScheduler->init)
      newScheduler->init();

   tid_t child_tid = currentScheduler->next();
   thread child;
   thread orphans = NULL;
   while(child_tid != NO_THREAD) {
      // Move child to new scheduler
      child = getThread(child_tid);
      currentScheduler->remove(child);
      child->tlist_next = orphans;

      child_tid = currentScheduler->next();
   }

   while(orphans) {
      newScheduler->admit(orphans);
      orphans = orphans->tlist_next;
   }

   // Swap schedulers
   if(currentScheduler->shutdown)
      currentScheduler->shutdown();
   currentScheduler = newScheduler;
}
