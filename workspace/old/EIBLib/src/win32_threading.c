
/*! \file win32_threading.c
    \brief This source file implements the necessary functions and
    data types for the WIN32 threading implementation.
    
    This header file implements the necessary functions and
    data types for the WIN32 threading implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose threading layer.
*/

#include <windows.h>
#include "eib7.h"
#include "threading.h"

struct threadargs
{
  EIB_ThreadFunc threadfunc;
  void *userdata;
};

static DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
  struct threadargs *ta;

  ta = (struct threadargs *)lpParameter;

  ta->threadfunc(ta->userdata);

  ExitThread(0);

  return 0;
}

EIB7_ERR EIB_StartThread(EIB_ThreadFunc threadfunc, void *userdata, EIB_THREAD *threadhandle)
{
  struct threadargs *ta;

  ta = (struct threadargs *)malloc(sizeof(struct threadargs));

  if(ta == NULL)
    return EIB7_OutOfMemory;
  
  ta->threadfunc = threadfunc;
  ta->userdata = userdata;

  threadhandle->hdl = CreateThread(NULL, 0, ThreadProc, ta, 0, NULL);
  if(threadhandle->hdl == NULL)
    return EIB7_CantStartThread;

  if(threadhandle->threadargs)
     free(threadhandle->threadargs);
  threadhandle->threadargs = (void*)ta;

  SetThreadPriority(threadhandle->hdl, THREAD_PRIORITY_TIME_CRITICAL);
  
  return EIB7_NoError;
}

EIB7_ERR EIB_WaitThread(EIB_THREAD *threadhandle)
{
  struct threadargs *ta;

  WaitForSingleObject(threadhandle->hdl, INFINITE);
  CloseHandle(threadhandle->hdl);
  threadhandle->hdl = NULL;

  ta = (struct threadargs*)(threadhandle->threadargs);
  free(ta);
  threadhandle->threadargs = NULL;

  return EIB7_NoError;
}

EIB7_ERR EIB_InitThreadLock(EIB_LOCK *lock)
{
  InitializeCriticalSection(lock);
  
  return EIB7_NoError;
}

EIB7_ERR EIB_AcquireThreadLock(EIB_LOCK *lock)
{
  EnterCriticalSection(lock);
  
  return EIB7_NoError;
}

EIB7_ERR EIB_ReleaseThreadLock(EIB_LOCK *lock)
{
  LeaveCriticalSection(lock);

  return EIB7_NoError;
}

EIB7_ERR EIB_FreeThreadLock(EIB_LOCK *lock)
{
  DeleteCriticalSection(lock);

  return EIB7_NoError;
}

void EIB_Sleep(int ms)
{
  Sleep(ms);
}

void EIB_GetStartTime(EIB_TIME *starttime)
{
   if(starttime == NULL)
      return;

   #pragma warning( suppress : 4996)
  _ftime(starttime);
}


int EIB_TimeElapsed(EIB_TIME *starttime)
{
  EIB_TIME now;
  __time64_t temp;

  if(starttime == NULL)
      return 0;

  #pragma warning( suppress : 4996)
  _ftime(&now);

  temp = (now.time - starttime->time) * 1000 +
    ((int)now.millitm - (int)starttime->millitm);

  return (int)(temp & 0xFFFFFFFF);
}

