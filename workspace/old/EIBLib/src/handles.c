/*! \file handles.c
    \brief This source file implements the necessary functions and
    data types for handling integer handles to memory data pointers.
    
    This functionality shall make use of undefined pointers by users
    nearly impossible.
*/

#include <stdlib.h>
#include "threading.h"
#include "eib7.h"

/*! The max. number of simultaneous open handles */
#define MAXHANDLES 8192

/* The array holding all handle's information */
static struct 
{
  int master; /*!< If -1, this handle is a master handle. If >= 0, it is a slave to the handle indicated */
  void *ptr;  /*!< the actual pointer to the data area */
}Handle[MAXHANDLES];

static int Initialized = 0;
static int NextHandle = 0;

static EIB_LOCK lock;

EIB7_ERR EIB_ReserveHandle(int *handle, void *data)
{
  int i;
  int hdl;
  
  if(++Initialized == 1)
    {
      EIB_InitThreadLock(&lock);
      
      EIB_AcquireThreadLock(&lock);

      for(i = 0; i < MAXHANDLES; i++)
	{
	  Handle[i].master = -1;
	  Handle[i].ptr = NULL;
	}
        
      Initialized = 1;
    }
  else
    EIB_AcquireThreadLock(&lock);
  
  /* find a free handle */
  hdl = -1;
  for(i = NextHandle; hdl < 0 && i < MAXHANDLES; i++)
    if(Handle[i].ptr == NULL)
      {
	hdl = i;
      }
  /* try again from 0 (turnaround), if none found */
  for(i = 0; hdl < 0 && i < NextHandle; i++)
    if(Handle[i].ptr == NULL)
      {
	hdl = i;
      }

  if(hdl < 0)
    {
      EIB_ReleaseThreadLock(&lock);
      return EIB7_OutOfMemory;
    }
  
  NextHandle = hdl + 1;
  
  Handle[hdl].ptr = data;

  if(Handle[hdl].ptr == NULL)
    {
      EIB_ReleaseThreadLock(&lock);
      return EIB7_OutOfMemory;
    }
  
  *handle = hdl + 1;
  
  EIB_ReleaseThreadLock(&lock);
  return EIB7_NoError;
}

#define CHECK_HANDLE(h)							\
  if(h < 0 || h >= MAXHANDLES || Handle[h].ptr == NULL)			\
    {									\
      EIB_ReleaseThreadLock(&lock);					\
      return EIB7_InvalidHandle;					\
    }									\
  									\
  if(Handle[h].master >= 0 && Handle[Handle[h].master].ptr == NULL)	\
    {									\
      EIB_ReleaseThreadLock(&lock);					\
      return EIB7_InvalidHandle;					\
    }									\
  
EIB7_ERR EIB_ReserveChildHandle(int master, int *handle, void *data)
{
  EIB7_ERR error;
  
  if(!Initialized)
    return EIB7_InvalidHandle;

  master--;

  CHECK_HANDLE(master);

  error = EIB_ReserveHandle(handle, data);
  
  if(error != EIB7_NoError)
    {
      EIB_ReleaseThreadLock(&lock);
      return error;
    }

  Handle[*handle - 1].master = master;
  
  return EIB7_NoError;
}


EIB7_ERR EIB_RetrieveHandlePtr(int handle, void **data)
{
  if(!Initialized)
    return EIB7_InvalidHandle;
  
  handle--;

  EIB_AcquireThreadLock(&lock);
  
  CHECK_HANDLE(handle);
  
  *data = Handle[handle].ptr;

  EIB_ReleaseThreadLock(&lock);					
  return EIB7_NoError;
}

EIB7_ERR EIB_FreeHandle(int handle)
{
  int i;
  
  if(!Initialized)
    return EIB7_InvalidHandle;

  handle--;

  EIB_AcquireThreadLock(&lock);

  CHECK_HANDLE(handle);
  
  Handle[handle].ptr = NULL;
  Handle[handle].master = -1;

  /* mark all slaves as invalid */
  for(i = 0; i < MAXHANDLES; i++)
    {
      if(Handle[i].master == handle)
	{
	  Handle[i].master = -1;
	  Handle[i].ptr = NULL;
	}
    }
  
  EIB_ReleaseThreadLock(&lock);					
  return EIB7_NoError;
}

