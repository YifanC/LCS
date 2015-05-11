#ifndef __WIN32_COMMTCP__H__
#define __WIN32_COMMTCP__H__

/*! \file win32_commtcp.h
    \brief This header file declares the necessary functions and
    data types for the WIN32 socket implementation.
    
    This header file declares the necessary functions and
    data types for the WIN32 socket implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose socket layer.
*/

#include <winsock2.h>
#include <ws2tcpip.h>

/*! An EIB_SOCKET maps to the Winsock32 SOCKET type */
typedef SOCKET EIB_SOCKET;


#endif /*__WIN32_COMMTCP__H__*/
