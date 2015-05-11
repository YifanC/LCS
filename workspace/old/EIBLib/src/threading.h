#ifndef __THREADING__H__
#define __THREADING__H__

/*! \file threading.h
    \brief This header file declares the necessary functions and
    data types for the platform-independant threading implementation.
    
    This header file declares the necessary functions and
    data types for the platform-independant threading implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose threading layer.
*/

#include "eib7.h"

#ifdef Win32
#include "win32_threading.h"
#elif  Linux
#include "linux_threading.h"
#else
#error No valid platform defined for build.
#endif

typedef void (*EIB_ThreadFunc)(void *userdata);


typedef enum EIB_THREAD_TYPE 
{
  EIB_THREAD_NONE = 0,  /*!< */
  EIB_THREAD_SRT = 1,   /*!< */
  EIB_THREAD_REC = 2,   /*!< */
  EIB_THREAD_STRM = 3   /*!< */
}EIB_THREAD_TYPE;



/*! Starts a function as a new thread.
  \param threadfunc the function to be started as a thread
  \param userdata a pointer to user data that should be passed to the userdata parameter of threadfunc
  \param threadhandle the operating system specific thread handle will be returned herein
  \return EIB_Success if the thread could be started successfully
*/

EIB7_ERR EIB_StartThread(EIB_ThreadFunc threadfunc, void *userdata, EIB_THREAD *threadhandle);

/*! Waits for a thread to finish.
  \param threadhandle the operating system specific thread handle
  \return EIB_Success if the thread could be stopped successfully
*/

EIB7_ERR EIB_WaitThread(EIB_THREAD *threadhandle);

/*! Initializes a lock for the thread-safety mechanism.
  \param lock a pointer to a memory location which receives the thread lock handle
  \return EIB_Success if the thread lock could be initialized
*/

EIB7_ERR EIB_InitThreadLock(EIB_LOCK *lock);

/*! Acquires the lock to the thread
  \param lock a pointer to the lock handle
  \return EIB_Success if the thread lock could be Acquired
*/

EIB7_ERR EIB_AcquireThreadLock(EIB_LOCK *lock);

/*! Releases the lock to the thread
  \param lock a pointer to the lock handle
  \return EIB_Success if the thread lock could be released
*/

EIB7_ERR EIB_ReleaseThreadLock(EIB_LOCK *lock);

/*! Frees the lock to the thread
  \param lock a pointer to the lock handle
  \return EIB_Success if the thread lock could be freed
*/

EIB7_ERR EIB_FreeThreadLock(EIB_LOCK *lock);

/*! Sleeps for the given time
  \param ms sleep time in milliseconds
*/

void EIB_Sleep(int ms);

/*! Records the current time in starttime. This time can then be used
  with #EIB_TimeElapsed to check for timeout conditions
  \param starttime a pointer to an EIB_TIME structure which receives the current time
  \return void
*/
void EIB_GetStartTime(EIB_TIME *starttime);

/*! Calculates the time difference (in milliseconds) between the crurrent time and
  previously recorded start time (by #EIB_GetStartTime)
  \param starttime a pointer to an EIB_TIME structure which holds the start time
  \return the number of milliseconds since the start time
*/
int EIB_TimeElapsed(EIB_TIME *starttime);

#endif /*__THREADING__H__*/
