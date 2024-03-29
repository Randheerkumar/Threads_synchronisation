#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef __cplusplus
extern "C"{
#endif
#ifndef _POSIX_PTHREAD_SEMANTICS
#define _POSIX_PTHREAD_SEMANTICS
#endif

/*
 Note: this library requires you to link with the posix threads
 library (-lpthread) and the real time library (-lrt) {for
 nanosleep}.

   c++ -D_POSIX_PTHREAD_SEMANTICS main.cc thread.c -lpthread -lrt
   or 
   g++ -Wall -D_POSIX_PTHREAD_SEMANTICS main.cc thread.c -lpthread -lrt
*/

#include <pthread.h>
#include <unistd.h>

typedef pthread_mutex_t smutex_t;
typedef pthread_cond_t scond_t;
typedef pthread_t thread_t;


void init(int n,int p);

/*
 * API for managing threads
 *
 * First the simplified version where the called
 * function takes an int as an argument and returns
 * an int when it's done.
 *
 * Then the more flexible version where the argument
 * and return are arbitrary pointers.
 */
void thread_create(thread_t *thrd,
		    void (*start_routine)(int), 
		    int arg_to_start_routine);
void thread_yield();
long thread_join(thread_t thrd);
void thread_exit(int ret);

void thread_create_p(thread_t *thrd,
                      void *(*start_routine)(void*), 
                      void *arg_to_start_routine);
void *thread_join_p(thread_t thrd);
void thread_exit_p(void *ret);

/*
 * WARNING:
 * Do not use sleep for synchronizing threads that 
 * should be waiting for events (using condition variables)!
 * Sleep should only be used to wait for a specified amount
 * of time! (If you find yourself looping on a predicate
 * and calling sleep in the loop, you probably are using
 * it incorrectly! We will deduct points from your grade
 * if you do this!
 */
void thread_sleep(unsigned int seconds, unsigned int nanoseconds);



/*
 * API for mutex locks
 */
void smutex_init(smutex_t *mutex);
void smutex_destroy(smutex_t *mutex);
void smutex_lock(smutex_t *mutex);
void smutex_unlock(smutex_t *mutex);

/*
 * API for condition variables
 */
void scond_init(scond_t *cond);
void scond_destroy(scond_t *cond);

/*
 * Condition variables are always associated with state
 * variables that you access before signalling, broadcasting,
 * or waiting. To access the state variable, you must hold
 * the associated mutex. To help enforce this, you 
 * are required to hold the mutex and pass it in as an
 * argument to these functions.
 */
void scond_signal(scond_t *cond, smutex_t *mutex);
void scond_broadcast(scond_t *cond, smutex_t *mutex);
void scond_wait(scond_t *cond, smutex_t *mutex);



#ifdef __cplusplus
} /* extern C */
#endif

#endif

