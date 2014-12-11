#ifndef PTHREAD_COMPAT_H
#define PTHREAD_COMPAT_H

#include <windows.h>
#include <process.h>

typedef HANDLE pthread_mutex_t;

typedef int pthread_condattr_t;

typedef struct
{
  int waiters_count_;
  // Number of waiting threads.

  CRITICAL_SECTION waiters_count_lock_;
  // Serialize access to <waiters_count_>.

  HANDLE sema_;
  // Semaphore used to queue up threads waiting for the condition to
  // become signaled. 

  HANDLE waiters_done_;
  // An auto-reset event used by the broadcast/signal thread to wait
  // for all the waiting thread(s) to wake up and be released from the
  // semaphore. 

  size_t was_broadcast_;
  // Keeps track of whether we were broadcasting or signaling.  This
  // allows us to optimize the code if we're just signaling.
} pthread_cond_t;

void pthread_cond_init (pthread_cond_t *cv, const pthread_condattr_t *attr);

void pthread_cond_wait (pthread_cond_t *cv, pthread_mutex_t *external_mutex);

void pthread_cond_signal (pthread_cond_t *cv);
  
void pthread_cond_broadcast (pthread_cond_t *cv);
  
#endif