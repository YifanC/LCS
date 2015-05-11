/*! \file linux_commtcp.c
    \brief This source file implements the necessary functions and
    data types for the LINUX socket implementation.
    
    This header file implements the necessary functions and
    data types for the LINUX socket implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose socket layer.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h> 
#include <sys/ioctl.h>
#include <sys/select.h>
#include <netinet/tcp.h> 
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "commtcp.h"
#include "definitions.h"

#define DEBUG 0

int EIB_SetSocketTimeout(EIB_SOCKET socket, unsigned long timeout);


EIB7_ERR EIB_GetHostIP(const char *hostname, unsigned long *ip)
{
  struct hostent *hostentry;

  /* Try to resolve hostname */
  hostentry = (void *)gethostbyname(hostname);
  if(hostentry == NULL)
    {
      return EIB7_HostNotFound;
    }

  *ip = ntohl(*(u_long *) hostentry->h_addr_list[0]);

  return EIB7_NoError;
}


EIB7_ERR EIB_InitSocket(EIB_SOCKET *sock)
{
   *sock = -1;

   return EIB7_NoError;
}

  
EIB7_ERR EIB_OpenSocket(unsigned long ip, EIB_SOCKET *sock, unsigned long timeout)
{
  int err;
  int bOptVal = 1;
  struct sockaddr_in addr;
  
  /* fill the adress structure */
  
  memset(&addr, 0, sizeof(addr)); 
  addr.sin_family = AF_INET;
  addr.sin_port = htons(TCP_DEFAULT_PORT);
  addr.sin_addr.s_addr = htonl(ip);

  /* open a socket and connect to the device */

  *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(*sock < 0)
    {
      return EIB7_CantOpenSocket;
    }
  
  err = EIB_SetSocketTimeout(*sock,timeout);
  if(err)
	  return EIB7_IllegalParameter;

  if(connect(*sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
      close(*sock);
      return EIB7_CantConnect;
    }

  /* set some socket options for the default behaviour */
  setsockopt(*sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bOptVal, sizeof(bOptVal));
  /* iOptVal = TCP_DEFAULT_TIMEOUT; */
  err = EIB_SetSocketTimeout(*sock,timeout);
  if(err)
	  return EIB7_IllegalParameter;
  
  return EIB7_NoError;
}


EIB7_ERR EIB_SendSocket(EIB_SOCKET sock, const unsigned char *data, size_t len)
{
  int result;

  result = send(sock, data, (len & 0xFFFFFFFF), 0);

  if(result < 0)
    {
      switch(errno)
	{
	case ECONNRESET:
	  return EIB7_ConnReset;
	  break;
	case ETIMEDOUT:
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
  
  result = recv(sock, data, *len, 0);

  if(result < 0)
    {
      *len = 0;
      switch(errno)
	{
	case ECONNRESET:
	  return EIB7_ConnReset;
	  break;
	case ETIMEDOUT:
	  return EIB7_ConnTimeout;
	  break;
	default:
	  return EIB7_ReceiveError;
	}
    }

  *len = (int)result;
  
  return EIB7_NoError;
}


EIB7_ERR EIB_CloseSocket(EIB_SOCKET *sock)
{
  /* No error checking here, this should always work. Anyway, what would
     we do if it didn't work? */
  
  if(*sock != -1)
    close(*sock);
  *sock = -1;
  
  return EIB7_NoError;
}

EIB7_ERR EIB_OpenUDP(EIB_SOCKET tcpsock, EIB_SOCKET *udpsock, unsigned long *ip, unsigned long *port)
{
  struct sockaddr_in addr;
  unsigned int sz;
  unsigned long newport;
  
  /* open a socket */

  *udpsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(*udpsock < 0)
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

  for(newport = 1024; newport<32768; newport++)
    {
      addr.sin_port = htons((unsigned short)newport);
      
      if(bind(*udpsock, (struct sockaddr *)&addr, sz) >= 0)
	{
	  int buffsize;
	  
#if 0
	  unsigned long nonblock;
	  
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
  close(*udpsock);
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

  select(udpsocket + 1, &fds, NULL, NULL, &timeout);

  return FD_ISSET(udpsocket, &fds);
}

int EIB_SendUDP(EIB_SOCKET udpsocket, void *data, int length)
{

   if(send(udpsocket, data, length, 0) != length)
      return -1;

   return 0;
}

int EIB_ReceiveUDP(EIB_SOCKET udpsocket, void *data, int size, int *bytes, int *rcv)
{
  unsigned long avail = 0;
#if DEBUG
  static int last_counter = -1;
#endif
  
  /* determine if enough data can be read */
  if(ioctl(udpsocket, FIONREAD, &avail) < 0)
  {
#if DEBUG
    printf("UDP: error while calling ioctl\n");
#endif
    
    return 0;
  }
  
  /* set some debugging statistics data */
  if(bytes) *bytes = avail;
  
  if(avail > 0)
    {
      int ret;
 
      ret = recv(udpsocket, data, MIN(size, (int)avail), 0);      
      if(ret < 0)
	{
#if DEBUG
	  int error;

	  error = errno;
	  
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
	  
	  printf("UDP returned: ret=%d avail=%d\n", ret, avail);
#endif	  
	  return 1;
	}
#if DEBUG
      else 
	printf("\nUDP: no data returned!\n");
#endif	  
      
    }
  
  return 0;
}

EIB7_ERR EIB_OpenTCPDataSocket(unsigned long ip, unsigned long port, EIB_SOCKET *sock)
{
  int bOptVal = 1;
  struct sockaddr_in addr;
  
  /* fill the adress structure */
  
  memset(&addr, 0, sizeof(addr)); 
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port & 0xFFFF);
  addr.sin_addr.s_addr = htonl(ip);

  /* open a socket and connect to the device */

  *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(*sock < 0)
    {
      return EIB7_CantOpenSocket;
    }

  if(connect(*sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
      close(*sock);
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
   unsigned int sz;

   /* open a socket */
   *udpsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if(*udpsock < 0)
   {
      return EIB7_CantOpenSocket;
   }

   /* fill the adress structure */
   sz = sizeof(addr);
   getpeername(tcpsocket, (struct sockaddr *)&addr, &sz);
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port & 0xFFFF);

   if(connect(*udpsock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
   {
      close(*udpsock);
      return EIB7_CantConnect;
   }

   buffsize = 128 * 1024;
   setsockopt(*udpsock, SOL_SOCKET, SO_RCVBUF, (void *)&buffsize, sizeof(buffsize));

   return EIB7_NoError;
}


int EIB_SetSocketTimeout(EIB_SOCKET socket, unsigned long timeout)
{
  int err;
  struct timeval TimeStruct;
  
  TimeStruct.tv_sec  =  timeout / 1000;
  TimeStruct.tv_usec = (timeout % 1000) * 1000;

  err = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (void*)&TimeStruct, sizeof(TimeStruct));
  if(err == 0)
	  err = setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (void*)&TimeStruct, sizeof(TimeStruct));

  return err != 0 ? 1 : 0;
}

