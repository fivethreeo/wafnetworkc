#ifndef PTHREAD_COMPAT_H
#define PTHREAD_COMPAT_H

#include <windows.h>
#include <process.h>


typedef CRITICAL_SECTION pthread_mutex_t;

typedef int pthread_condattr_t;

  enum {
    SIGNAL = 0,
    BROADCAST = 1,
    MAX_EVENTS = 2
  };
typedef struct
{
  u_int waiters_count_;
  // Count of the number of waiters.
  
  CRITICAL_SECTION waiters_count_lock_;
  // Serialize access to <waiters_count_>.


  HANDLE events_[MAX_EVENTS];
  // Signal and broadcast event HANDLEs.
} pthread_cond_t;

int pthread_cond_init (pthread_cond_t *cv, const pthread_condattr_t *attr);

int pthread_cond_wait (pthread_cond_t *cv, pthread_mutex_t *external_mutex);

int pthread_cond_signal (pthread_cond_t *cv);
  
int pthread_cond_broadcast (pthread_cond_t *cv);
  
#endif