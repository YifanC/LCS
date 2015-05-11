/*! \file win32_commtcp.c
    \brief This source file implements the necessary functions and
    data types for the WIN32 socket implementation.
    
    This header file implements the necessary functions and
    data types for the WIN32 socket implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose socket layer.
*/

#include <stdlib.h>
#include <stdio.h>

#include "win32_commtcp.h"
#include "commtcp.h"
#include "definitions.h"

#define DEBUG 0

static EIB7_ERR InitWSA(void)
{
  static int init = 0;
  int err = 0;
  
  if(init == 0)
  {
      WORD wVersionRequested;
      WSADATA wsaData;

      init++;

      wVersionRequested = MAKEWORD( 2, 2 );
      err = WSAStartup( wVersionRequested, &wsaData );
  }

  if(err != 0)
    return EIB7_CantInitWinSock;

  return EIB7_NoError;
}

EIB7_ERR EIB_GetHostIP(const char *hostname, unsigned long *ip)
{
  EIB7_ERR err;
  struct hostent *hostentry;

  err = InitWSA();
  if(err != EIB7_NoError) 
     return err;

  /* Try to resolve hostname */
  hostentry = gethostbyname(hostname);
  if(hostentry == NULL)
    {
      /*WSACleanup(); */   /* cleanup will be done while DLL unloading */
      return EIB7_HostNotFound;
    }

  *ip = ntohl(*(u_long *) hostentry->h_addr_list[0]);

  return EIB7_NoError;
}


EIB7_ERR EIB_InitSocket(EIB_SOCKET *sock)
{
   *sock = INVALID_SOCKET;

   return EIB7_NoError;
}
  

EIB7_ERR EIB_OpenSocket(unsigned long ip, EIB_SOCKET *sock, unsigned long timeout)
{
  EIB7_ERR err;
  BOOL bOptVal = TRUE;
  int iOptVal = TCP_DEFAULT_TIMEOUT;
  struct sockaddr_in addr;
  
  err = InitWSA();
  if(err != EIB7_NoError) 
     return err;

  /* fill the adress structure */
  
  memset(&addr, 0, sizeof(addr)); 
  addr.sin_family = AF_INET;
  addr.sin_port = htons(TCP_DEFAULT_PORT);
  addr.sin_addr.s_addr = htonl(ip);

  /* open a socket and connect to the device */

  *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(*sock == INVALID_SOCKET)
    {
      return EIB7_CantOpenSocket;
    }

  iOptVal = timeout;
  
  setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&iOptVal, sizeof(iOptVal));
  setsockopt(*sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&iOptVal, sizeof(iOptVal));

  if(connect(*sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
      closesocket(*sock);
      return EIB7_CantConnect;
    }

  /* set some socket options for the default behaviour */
  setsockopt(*sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bOptVal, sizeof(bOptVal));
  /*  iOptVal = TCP_DEFAULT_TIMEOUT; */
  setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&iOptVal, sizeof(iOptVal));
  setsockopt(*sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&iOptVal, sizeof(iOptVal));
  
  return EIB7_NoError;
}


EIB7_ERR EIB_SendSocket(EIB_SOCKET sock, const unsigned char *data, size_t len)
{
  int result;

  do
    {
      result = send(sock, (const char*)data, (len & 0xFFFFFFFF), 0);
    }
  while(result == WSAEINPROGRESS);

  if(result == SOCKET_ERROR)
    {
      switch(WSAGetLastError())
	{
	case WSAECONNRESET:
	  return EIB7_ConnReset;
	  break;
	case WSAETIMEDOUT:
	  return EIB7_ConnTimeout;
	  break;
	default:
	  return EIB7_SendError;
	}
    }

  return EIB7_NoError;
}


EIB7_ERR EIB_ReceiveSocket(EIB_SOCKET sock, unsigned char *data, int *len)
{
  int result;
  
  do
    {
      result = recv(sock, (char*)data, *len, 0);
    }
  while(result == WSAEINPROGRESS);

  if(result == SOCKET_ERROR)
    {
      *len = 0;
      switch(WSAGetLastError())
	{
	case WSAECONNRESET:
	  return EIB7_ConnReset;
	  break;
	case WSAETIMEDOUT:
	  return EIB7_ConnTimeout;
	  break;
	default:
	  return EIB7_ReceiveError;
	}
    }

  *len = result;
  
  return EIB7_NoError;
}


EIB7_ERR EIB_CloseSocket(EIB_SOCKET *sock)
{
  /* No error checking here, this should always work. Anyway, what would
     we do if it didn't work? */
  
  if(*sock != INVALID_SOCKET)
    closesocket(*sock);

  *sock = INVALID_SOCKET;
  
  return EIB7_NoError;
}

EIB7_ERR EIB_OpenUDP(EIB_SOCKET tcpsock, EIB_SOCKET *udpsock, unsigned long *ip, unsigned long *port)
{
  struct sockaddr_in addr;
  int sz;
  unsigned long newport;
  
  /* open a socket */

  *udpsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(*udpsock == INVALID_SOCKET)
    {
      return EIB7_CantOpenSocket;
    }

  /* to be sure that the IP address used as destination address
     for the EIB's UDP data, we use the address assigned to the
     TCP/IP connection */

  sz = sizeof(addr);
  getsockname(tcpsock, (struct sockaddr *)&addr, &sz);
  *ip = ntohl(addr.sin_addr.s_addr);
  
  /* Now, iterate over the ports to find a free one */

  for(newport = 1024; newport < 32768; newport++)
    {
      addr.sin_port = htons((unsigned short)newport);

      if(bind(*udpsock, (struct sockaddr *)&addr, sz) != SOCKET_ERROR)
	{
	  int buffsize;
	  
#if 0
	  /* set the socket to non-blocking operation */
	  nonblock = 1;
	  ioctlsocket(*udpsock, FIONBIO, &nonblock);
#endif

	  buffsize = 128 * 1024;
	  setsockopt(*udpsock, SOL_SOCKET, SO_RCVBUF, (void *)&buffsize, sizeof(buffsize));
	  
     *port = newport;

	  return EIB7_NoError;
	}
    }

  /* we could not bind the socket */
  closesocket(*udpsock);
  *port = newport;

  return EIB7_CantOpenSocket;
}

int EIB_WaitForUDPData(EIB_SOCKET udpsocket, int us)
{
  fd_set fds;
  struct timeval timeout;

  timeout.tv_sec = 0;
  timeout.tv_usec = us;
  
  FD_ZERO(&fds);
  FD_SET(udpsocket, &fds);

  select(1, &fds, NULL, NULL, &timeout);

  return FD_ISSET(udpsocket, &fds);
}


int EIB_SendUDP(EIB_SOCKET udpsocket, void *data, int length)
{

   if(send(udpsocket, (char*)data, length, 0) == SOCKET_ERROR)
      return -1;

   return 0;
}


int EIB_ReceiveUDP(EIB_SOCKET udpsocket, void *data, int size, int *bytes, int *rcv)
{
  unsigned long avail;
  static int last_counter = -1;

  avail = 0;
  *rcv = 0;

  /* determine if enough data can be read */
  if(ioctlsocket(udpsocket, FIONREAD, &avail) != 0)
  {
	 if(bytes) *bytes = 0;

	 return 0;
  }
  
  /* set some debugging statistics data */
  if(bytes) *bytes = avail;
  
  if(avail > 0)
    {
      int ret;

      ret = recv(udpsocket, (char*)data, MIN(size, (int)avail), 0);  /* avial must be > 0 */
      if(ret == SOCKET_ERROR)
	{
	  int error;
	  error = WSAGetLastError();
#if DEBUG
	  if(error != 10035)
	    printf("\nSocket error : %d, %d\n", error, avail);
#endif
	  return 0;
	}

      
      *rcv = ret;
      
      if(ret > 0)
	{
#if DEBUG
	  int counter;

	  counter = *(unsigned short *)data;
	  
	  if(last_counter >= 0 && counter != last_counter + 1 && last_counter != 65535 && counter != 0)
	    printf("\nUDP: Invalid counter increment in data  (old = %d, new = %d)\n", last_counter, counter);

	  last_counter = counter;
#endif	  
	  return 1;
	}
#if DEBUG
      else printf("\nUDP: no data returned!\n");
#endif	  
      
    }
  
  return 0;
}

EIB7_ERR EIB_OpenTCPDataSocket(unsigned long ip, unsigned long port, EIB_SOCKET *sock)
{
  BOOL bOptVal = TRUE;
  struct sockaddr_in addr;
  
  /* fill the adress structure */
  
  memset(&addr, 0, sizeof(addr)); 
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port & 0xFFFF);
  addr.sin_addr.s_addr = htonl(ip);

  /* open a socket and connect to the device */

  *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(*sock == INVALID_SOCKET)
    {
      return EIB7_CantOpenSocket;
    }

  if(connect(*sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
      closesocket(*sock);
      return EIB7_CantConnect;
    }

  /* set some socket options for the default behaviour */
  setsockopt(*sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bOptVal, sizeof(bOptVal));
  
  return EIB7_NoError;
}


EIB7_ERR EIB_OpenUDPDataSocket(EIB_SOCKET tcpsocket, unsigned long port, EIB_SOCKET *udpsock)
{
   struct sockaddr_in addr;
   int buffsize;
   int sz;

   /* open a socket */
   *udpsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if(*udpsock == INVALID_SOCKET)
   {
      return EIB7_CantOpenSocket;
   }

   /* fill the adress structure */
   sz = sizeof(addr);
   getpeername(tcpsocket, (struct sockaddr *)&addr, &sz);
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port & 0xFFFF);

   if(connect(*udpsock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
   {
      closesocket(*udpsock);
      return EIB7_CantConnect;
   }

   buffsize = 128 * 1024;
   setsockopt(*udpsock, SOL_SOCKET, SO_RCVBUF, (void *)&buffsize, sizeof(buffsize));

   return EIB7_NoError;
}



int EIB_SetSocketTimeout(EIB_SOCKET socket, unsigned long timeout)
{
  int err;
  int iOptVal = TCP_DEFAULT_TIMEOUT;

  iOptVal = timeout;
  
  err = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&iOptVal, sizeof(iOptVal));
  if(err == 0)
	  err = setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&iOptVal, sizeof(iOptVal));

  return err != 0 ? 1 : 0;
}
