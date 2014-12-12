#include "pthread_compat.h"

int 
pthread_cond_init (pthread_cond_t *cv, const pthread_condattr_t *attr)
{

  // Initialize the count to 0.
  cv->waiters_count_ = 0;

  cv->events_[SIGNAL] = CreateEvent (NULL,  // no security
                                     FALSE, // auto-reset event
                                     FALSE, // non-signaled initially
                                     NULL); // unnamed

  // Create a manual-reset event.
  cv->events_[BROADCAST] = CreateEvent (NULL,  // no security
                                        TRUE,  // manual-reset
                                        FALSE, // non-signaled initially
                                        NULL); // unnamed
  return 0;
}


int 
pthread_cond_wait (pthread_cond_t *cv,
                   pthread_mutex_t *external_mutex)
{
  int result;
  int last_waiter;
  // Avoid race conditions.
  EnterCriticalSection (&cv->waiters_count_lock_);
  cv->waiters_count_++;
  LeaveCriticalSection (&cv->waiters_count_lock_);

  // It's ok to release the <external_mutex> here since Win32
  // manual-reset events maintain state when used with
  // <SetEvent>.  This avoids the "lost wakeup" bug...
  LeaveCriticalSection (external_mutex);

  // Wait for either event to become signaled due to <pthread_cond_signal>
  // being called or <pthread_cond_broadcast> being called.
  result = WaitForMultipleObjects (2, ev->events_, FALSE, INFINITE);

  EnterCriticalSection (&cv->waiters_count_lock_);
  cv->waiters_count_--;
  last_waiter =
    result == WAIT_OBJECT_0 + BROADCAST 
    && cv->waiters_count_ == 0;
  LeaveCriticalSection (&cv->waiters_count_lock_);

  // Some thread called <pthread_cond_broadcast>.
  if (last_waiter)
    // We're the last waiter to be notified or to stop waiting, so
    // reset the manual event. 
    ResetEvent (cv->events_[BROADCAST]); 

  // Reacquire the <external_mutex>.
  EnterCriticalSection (external_mutex, INFINITE);

  return 0;
}

int
pthread_cond_signal (pthread_cond_t *cv)
{
  int have_waiters;
  // Avoid race conditions.
  EnterCriticalSection (&cv->waiters_count_lock_);
  have_waiters = cv->waiters_count_ > 0;
  LeaveCriticalSection (&cv->waiters_count_lock_);

  if (have_waiters)
    SetEvent (cv->events_[SIGNAL]);
    
  return 0;
}

int 
pthread_cond_broadcast (pthread_cond_t *cv)
{
  int have_waiters;
  // Avoid race conditions.
  EnterCriticalSection (&cv->waiters_count_lock_);
  have_waiters = cv->waiters_count_ > 0;
  LeaveCriticalSection (&cv->waiters_count_lock_);

  if (have_waiters)
    SetEvent (cv->events_[BROADCAST]);
    
  return 0;
}