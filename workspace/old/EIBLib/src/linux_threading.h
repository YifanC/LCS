#ifndef __LINUX_THREADING__H__
#define __LINUX_THREADING__H__

/*! \file linux_threading.h
    \brief This header file declares the necessary functions and
    data types for the LINUX threading implementation.
    
    This header file declares the necessary functions and
    data types for the LINUX threading implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose threading layer.
*/

#include <pthread.h>
#include <sys/timeb.h>
#include <time.h>

/*! An EIB_THREAD is a structure containing the the windows thread ID and a CRITICAL_SECTION
 for the thread lock */
typedef struct
{
  pthread_t thread;
  void* threadargs;     /*!< arguments for the thread */
}EIB_THREAD;

/*! An EIB_LOCK is a CRITICAL_SECTION in LINUX */
typedef pthread_mutex_t EIB_LOCK;

/*! An EIB_TIME is a struct timeb */
typedef struct timeb EIB_TIME;

#endif /*__LINUX_THREADING__H__*/
