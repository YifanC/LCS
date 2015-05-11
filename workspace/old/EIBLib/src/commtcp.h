#ifndef __COMMTCP__H__
#define __COMMTCP__H__

/*! \file commtcp.h
    \brief This header file declares the necessary functions and
    data types for the platform-independant socket implementation.
    
    This header file declares the necessary functions and
    data types for the platform-independant socket implementation.
    All functions herein are tailored for use in the EIB7 driver only.
    This is not a general-purpose socket layer.
*/

#include "eib7.h"

#ifdef Win32
#include "win32_commtcp.h"
#elif  Linux
#include "linux_commtcp.h"
#else
#error No valid platform defined for build.
#endif 

/*! Resolves a host name (either as ASCII hostname, e.g. 'eib7.network.domain', 'eib7' or as
  IP address string , e.g. '192.168.1.2) to an unsigned long in host byte order.
  \param hostname contains either the ip-address or the hostname of the EIB7 as a string
  \param ip pointer to an unsigned long which receives the IP address
  \return EIB_Success if the name could be resolved
*/

EIB7_ERR EIB_GetHostIP(const char *hostname, unsigned long *ip);

/*! Initialize the socket to an invalid value.
  \param socket the referenced socket will be set to invalid
  \return EIB_Success
*/

EIB7_ERR EIB_InitSocket(EIB_SOCKET *socket);

/*! Opens a TCP/IP socket to the EIB hardware. Returns an error if the connection could not
  be opened.
  \param ip contains the ip-address in host byte order
  \param socket if the call is successful, the socket handle is returned in this parameter
  \param timeout the timeout (in ms) that should be waited before the connection attempt is deemed a failure
  \return EIB_Success if the socket could be opened
*/

EIB7_ERR EIB_OpenSocket(unsigned long ip, EIB_SOCKET *socket, unsigned long timeout);

/*! Sends data to the EIB device
  \param socket the socket handle to the device
  \param data the pointer to the data to be sent
  \param len the number of bytes to send
  \return EIB_Success if all bytes have been sent successfully
*/

EIB7_ERR EIB_SendSocket(EIB_SOCKET socket, const unsigned char *data, size_t len);

/*! Reads data from the EIB device
  \param socket the socket handle to the device
  \param data the pointer to the data buffer, where the received bytes will be copied into
  \param len the size of the input buffer. On return, the actual number of received bytes will be put here
  \return EIB_Success if the call ends successful. The caller must examine the len output to check
  whether all data has been received successfully
*/

EIB7_ERR EIB_ReceiveSocket(EIB_SOCKET socket, unsigned char *data, int *len);

/*! Closes the socket to the EIB hardware.
  \param socket the socket handle to the device
  \return EIB_Success if the call ends successful.
*/

EIB7_ERR EIB_CloseSocket(EIB_SOCKET *socket);

/*! Open a UDP socket to receive data on, and return the IP address and port
  for the EIB's destination network address. The socket can be closed with #EIB_CloseSocket
  \param tcpsocket the socket handle to the device
  \param udpsocket pointer to a variable to receive the UDP socket handle
  \param ip pointer to an unsigned long which receives the IP address
  \param port pointer to an unsigned long which receives the port number
  \return EIB7_NoError if the UDP port could be opened
*/

EIB7_ERR EIB_OpenUDP(EIB_SOCKET tcpsocket, EIB_SOCKET *udpsocket, unsigned long *ip, unsigned long *port);

/*! Wait for UDP data to be available at the sepcified socket
  \param udpsocket the UDP socket handle
  \param microseconds maximum time to wait in microseconds
  \return
  - 0 if no data is available (timeout)
  - != 0 if data is available
*/
int EIB_WaitForUDPData(EIB_SOCKET udpsocket, int microseconds);


/*! Send one UDP data packet. 
  \param udpsocket the UDP socket handle
  \param data pointer to the transmit buffer
  \param length number of bytes to transmit
  \return 0 if successfull
*/
int EIB_SendUDP(EIB_SOCKET udpsocket, void *data, int length);


/*! Receive exactly one UDP data packet. This call does not wait, it returns immediately wether or
  not a UDP datagram is available
  \param udpsocket the UDP socket handle
  \param data pointer to the receive buffer
  \param size receive buffer size
  \param bytes pointer to a variable that receives the number of bytes available at the UDP port (may
  be NULL)
  \param rcv number of bytes received
  \return 0 if no packet was received, != 0 if a packet was received
*/

int EIB_ReceiveUDP(EIB_SOCKET udpsocket, void *data, int size, int *bytes, int *rcv);


/*! Opens a TCP/IP socket to the EIB hardware. Returns an error if the connection could not
  be opened.
  \param ip contains the ip-address in host byte order
  \param port contains the TCP port number in host byte order
  \param socket if the call is successful, the socket handle is returned in this parameter
  \return EIB_Success if the socket could be opened
*/

EIB7_ERR EIB_OpenTCPDataSocket(unsigned long ip, unsigned long port, EIB_SOCKET *sock);


/*! Opens a UDP/IP socket to the EIB hardware. Returns an error if the connection could not
  be opened.
  \param ip contains the ip-address in host byte order
  \param port contains the UDP port number in host byte order
  \param socket if the call is successful, the socket handle is returned in this parameter
  \return EIB_Success if the socket could be opened
*/

EIB7_ERR EIB_OpenUDPDataSocket(EIB_SOCKET tcpsocket, unsigned long port, EIB_SOCKET *udpsock);


/*! Set the socket timeout to the given value.
	\param socket the socket handle
	\param timeout the timeout in ms
	\return 0 if the call was successful, 1 on error
*/
int EIB_SetSocketTimeout(EIB_SOCKET socket, unsigned long timeout);

#endif /*__COMMTCP__H__*/
