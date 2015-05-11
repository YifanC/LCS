#ifndef __WIN32_THREADING__H__
#define __WIN32_THREADING__H__

/*! \file win32_commtcp.h
    \brief This header file declares the necessary functions and
    data types for the WIN32 threading implementation.
    
    This header file declares the necessary functions and
    data types for the WIN32 threading implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose threading layer.
*/

#include <windows.h>
#include <sys/timeb.h>
#include <time.h>

/*! An EIB_THREAD contains some information for the thread*/
typedef struct EIB_THREAD
{
  HANDLE hdl;   /*!< The thread's WIN32 HANDLE */
  void* threadargs;     /*!< arguments for the thread */
} EIB_THREAD;
  

/*! An EIB_LOCK is a CRITICAL_SECTION in WIN32 */
typedef CRITICAL_SECTION EIB_LOCK;

/*! An EIB_TIME is a struct _timeb */
typedef struct _timeb EIB_TIME;

#endif /*__WIN32_THREADING__H__*/
