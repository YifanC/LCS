#ifndef __CMDTCP_H__
#define __CMDTCP_H__

/*! \file cmdtcp.h
    \brief This file implements the network-based command handling.
    
    This file implements the network-based command handling. All commands are
    defined as data, complete with all information needed to access the return values.
    This is done with the idea in mind to be able to switch to a different command
    mechanism (e.g. direct PCI-E bus interface) easily.
*/

#include "commtcp.h"


#define MAXPARAMS 8   /*!< Maximum number of parameters for one command */
#define MAXRESULTS 16 /*!< Maximum number of result fields for one command */


/*! The possible data types for any parameter or result field. */
typedef enum 
  {
    TCP_PT_Data, /*!< Opaque binary data */
    TCP_PT_U8,
    TCP_PT_U16,
    TCP_PT_U24,  /*!< 3-Byte flags field */
    TCP_PT_U32,
    TCP_PT_POS,  /*!< 6-Byte encoder position */
    TCP_PT_AUXPOS, /*!< 4-Byte encoder position for auxiliary axis */
    TCP_PT_Undef /*!< The data type is undefined. Should not be used */
  }
  TCP_ParamType;


/*! This struct describes one parameter or result field. */

typedef struct 
{
  TCP_ParamType type;     /*!< data type of the parameter */
  int offset;             /*!< byte position of this field (from start of parameters) */
  int size;               /*!< size (in bytes) of this field. -1 means variable length */
  char *name;             /*!< name of this field. Very useful for debugging */
}TCP_Parameter;

/*! This struct defines all TCP/IP commands for the EIB7 */
typedef struct 
{
  char *name;                        /*!< the name of this command. Very useful for debugging */
  unsigned long opcode;              /*!< the command opcode (e.g. 0x64 for close link) */
  int numparams;                     /*!< the number of input parameters for this command */
  TCP_Parameter param[MAXPARAMS];    /*!< the actual input parameter descriptions */
  int numresults;                    /*!< the number of results for this command. If the value is ngative, don't wait for an answer (e.g. Reset command) */
  TCP_Parameter result[MAXRESULTS];  /*!< the actual result field descriptions */
}TCP_Command;

#define EMPTY_PARAM { TCP_PT_Undef, 0, 0, "Unused" }



extern TCP_Command Commands[];



/*! This struct holds the socket and the connection status for a TCP/IP connection to an EIB */
typedef struct
{
  EIB_SOCKET sock;
  EIB_SOCKET udpsock;
  EIB_SOCKET tcpdatasocket;
  EIB7_CONN_STATUS status;
}EIB_TCPconn;

/*! This call returns a command interface for the TCP/IP connection
 \returns a pointer to an EIB_CommandInterface structure
*/
struct EIB_CommandInterface *EIB_GetTCPCommandInterface();

/*! The soft real time handler for a network connection. This routine
  is started as a separate thread when the EIB is switched to soft realtime
  \param userdata a pointer to an EIB_Data structure
*/
void EIB_SoftRealtimeHandlerUDP(void *userdata);


/*! The recording handler for a network connection. This routine
  is started as a separate thread when the recording data is transferred
  from the EIB to the host
  \param userdata a pointer to an EIB_Data structure
*/
void EIB_RecordingHandlerTCP(void* userdata);


/*! The streaming handler for a network connection. This routine
  is started as a separate thread when the EIB is switched to streaming mode
  \param userdata a pointer to an EIB_Data structure
*/
void EIB_StreamingHandlerUDP(void* userdata);



#endif
