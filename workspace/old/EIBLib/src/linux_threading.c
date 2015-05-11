
/*! \file linux_threading.c
    \brief This source file implements the necessary functions and
    data types for the Linux threading implementation.
    
    This header file implements the necessary functions and
    data types for the Linux threading implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose threading layer.
*/

#include <sched.h>
#include <string.h>
#include "eib7.h"
#include "threading.h"
#include "unistd.h"

struct threadargs
{
  EIB_ThreadFunc threadfunc;
  void *userdata;
};

static void *ThreadProc(void *lpParameter)
{
  struct threadargs *ta;

  ta = (struct threadargs *)lpParameter;

  ta->threadfunc(ta->userdata);

  return NULL;
}

EIB7_ERR EIB_StartThread(EIB_ThreadFunc threadfunc, void *userdata, EIB_THREAD *threadhandle)
{
  struct threadargs *ta;
  struct sched_param sp;
  
  ta = (struct threadargs *)malloc(sizeof(struct threadargs));

  if(ta == NULL)
    return EIB7_OutOfMemory;
  
  ta->threadfunc = threadfunc;
  ta->userdata = userdata;

  if(pthread_create(&threadhandle->thread, NULL, ThreadProc, ta))
    {
      free(ta);
      return EIB7_CantStartThread;
    }
  
  if(threadhandle->threadargs)
     free(threadhandle->threadargs);
  threadhandle->threadargs = (void*)ta;

  memset(&sp, 0, sizeof(sp));
  sp.sched_priority = sched_get_priority_max(SCHED_RR);
  pthread_setschedparam(threadhandle->thread, SCHED_RR, &sp);
  
  return EIB7_NoError;
}

EIB7_ERR EIB_WaitThread(EIB_THREAD *threadhandle)
{
  void *ret;
  struct threadargs *ta;
  
  pthread_join(threadhandle->thread, &ret);

  ta = (struct threadargs*)(threadhandle->threadargs);
  free(ta);
  threadhandle->threadargs = NULL;

  return EIB7_NoError;
}

EIB7_ERR EIB_InitThreadLock(EIB_LOCK *lock)
{
  pthread_mutex_init(lock, 0);
  
  return EIB7_NoError;
}

EIB7_ERR EIB_AcquireThreadLock(EIB_LOCK *lock)
{
  pthread_mutex_lock(lock);
  
  return EIB7_NoError;
}

EIB7_ERR EIB_ReleaseThreadLock(EIB_LOCK *lock)
{
  pthread_mutex_unlock(lock);

  return EIB7_NoError;
}

EIB7_ERR EIB_FreeThreadLock(EIB_LOCK *lock)
{
  pthread_mutex_destroy(lock);

  return EIB7_NoError;
}

void EIB_Sleep(int ms)
{
  usleep(ms * 1000);
}

void EIB_GetStartTime(EIB_TIME *starttime)
{
  if(starttime == NULL)
  	return;
  	
  ftime(starttime);
}


int EIB_TimeElapsed(EIB_TIME *starttime)
{
  EIB_TIME now;

  ftime(&now);

  return (int)(now.time - starttime->time) * 1000 +
    ((int)now.millitm - (int)starttime->millitm);
}

