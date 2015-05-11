
#ifndef __EIB7_TYPES_H__
#define __EIB7_TYPES_H__

/*!
  \file eib7_types.h
  \brief This file defines the central data structures for the EIB7 driver.

*/

#include "eib7.h"
#include "definitions.h"
#include "cmdtcp.h"
#include "threading.h"
#include "fifo.h"

/*! This structure defines the necessary functions to perform a command on
   an EIB device independant of it's electrical interface */
typedef struct EIB_CommandInterface
{
  
/*! Initialize the command structure for a command to be executed
  \param command the index of the command entry in the Commands array
  \param cmddata a pointer which receives a pointer to the command structure
  \return EIB7_NoError if command was executed successfully
*/
  EIB7_ERR (*InitializeCommand)(int command, void **cmddata);

/*! Free a command structure after use.
  \param cmddata a pointer to the command data structure 
  \return EIB7_NoError if command was executed successfully
*/
  EIB7_ERR (*FreeCommand)(void *cmddata);

/*! Set an argument in the command structure.
  \param cmddata a pointer to the command data structure
  \param index the index of the parameter in the command
  \param value a pointer to the data variable for the parameter.
  For integer data, it is always assumed to point to an 4-Byte data type
  \param size the size of the parameter data. Only valid for parameters with variable size
  \return EIB7_NoError if the argument could be placed in the command
*/
  EIB7_ERR (*SetParameter)(void *cmddata, int index, void *value, int size);

/*! Get a response data field from the command structure
  \param cmddata a pointer to the command data structure
  \param index the index of the result field in the command
  \param value a pointer to the data variable for the parameter.
  For integer data, it is always assumed to point to an 4-Byte data type
  \param size the size of the parameter data. Only valid for parameters with variable size
  \param offset the offset of the parameter data 'page'. Only valid for repeating result fields, e.g. connection info
  \return EIB7_NoError if the result could be retrieved
*/
  EIB7_ERR (*GetParameter)(void *cmddata, int index, void *value, int size, int offset);

/*! Execute a command on the EIB device, and awaits the response. Sets the appropriate
  error information.
  \param conn the pointer to the communication object for this device (in case of cmdtcp, a pointer to an EIB_SOCKET)
  \param seqno the sequence number for the command
  \param cmddata a pointer to the command structure
  \param resplen the size of the response data
  \return EIB7_NoError if command was executed successfully
*/

  EIB7_ERR (*ExecuteCommand)(void *conn, int seqno, void *cmddata, int *resplen);

}EIB_CommandInterface;

/*! The maximum number of data fields in a position data structure */
#define MAX_POSITION_DATA_FIELDS 32

/*! The default number of regions within a data packet */
#define DEFAULT_NUM_OF_DATA_REGIONS   5

/*! The number of axis in one EIB device */
#define NUM_AXIS_PER_EIB 4

/*! The number of ports in one EIB device */
#define NUM_PORTS_PER_EIB 8

/*! The number of input ports in one EIB device */
#define NUM_INPUTS_PER_EIB 4

/*! This is the data for an axis handle */
typedef struct
{
  EIB7_HANDLE eib; /*!< The EIB7_HANDLE to which this axis belongs */
  int channel;     /*!< The index of the axis (aka 'channel'). Channels start with 0 */
}EIB_Axis;

/*! This is the data for an I/O handle */
typedef struct
{
  EIB7_HANDLE eib; /*!< The EIB7_HANDLE to which this port belongs */
  int port;        /*!< The index of the port. Indicesstart with 0 */
  int output;      /*!< True, if this port is an output port, false otherwise */
}EIB_Port;

/*! The structure that defines a field in a position data record. */
typedef struct EIB_DataFieldDescription
{
  EIB7_PositionDataField type; /*!< the field's type */
  int size;                    /*!< the field's size in the converted structure */
  int size_raw;                /*!< the field's size in the raw structure */
  int offset;                  /*!< the field's offset. This must be calculated using #EIB_CalculateOffsets
				 after the field list in the EIB data structure has been changed !*/
  int offset_raw;              /*!< the field's offset in the raw structure. This must be calculated using
				 #EIB_CalculateOffsets after the field list in the EIB data structure has
				 been changed !*/
  struct EIB_DataFieldDescription* next; /* pointer to next EIB_DataFieldDescription */

}EIB_DataFieldDescription;


typedef struct EIB_DataRegionDescription
{
   EIB7_DataRegion type;                     /* type of this region */
   unsigned long size;                       /* number of fields in this region */
   EIB_DataFieldDescription* firstfield;     /* pointer to the first field in this region */
   struct EIB_DataRegionDescription* next;   /* pointer to next EIB_DataRegionDescription */

} EIB_DataRegionDescription;

typedef struct
{
   unsigned long size;                       /* number of regions in this packet */
   EIB_DataRegionDescription* firstregion;   /* pointer to first region in this packet */
} EIB_DataPacketDescription;


/*! This is the main EIB7 data struct. It holds all information necessary for one EIB connection */
typedef struct EIB_Data
{
  EIB7_HANDLE handle;        /*!< The handle for this data structure, for self-reference */
  EIB_CommandInterface *cif; /*!< The communication interface for this device */
  union
  {
    EIB_TCPconn tcpconn;
  }conndata;                 /*!< If this is a TCP/IP connection, this field holds a EIB_TCPconn structure */
  void *conn;                /*!< This ptr is initialized to point to the used union field inside this tructure */
  enum
    {
      EIB_TCPConnection
    } conntype;
  
  EIB_LOCK   threadlock;     /*!< The thread lock used for this device, to avoid race
			       conditions and command interruption */
  int seqno;                 /*!< The sequence number of the current command */
  int master;                /*!< TRUE if this connection is the master connection */

  EIB_LOCK   fifolock;       /*!< Another lock lock used for access to the real-time fifo only */
  EIB_THREAD thread;         /*!< The threadhandle for the soft realtime thread */
            /*!< the type of the thread currently running */
  EIB_THREAD_TYPE threadtype;
  long fifosize;             /*!< The size of the FIFO to be created */
  int threadrunning;         /*!< TRUE if the thread is started */
  int threadendflag;         /*!< TRUE if the thread should finish */
  
  EIB7OnDataAvailable callback; /*!< The callback function for incoming soft realtime data */
  void *callbackdata;        /*!< the userdata pointer for the callback */
  int threshold;             /*!< The number of packets that have to be available in the FIFO before calling the callback function */
  int maxudp;                /*!< The maximum number of bytes in the UDP receive buffer since the last call to EIB7GetUDPMaxBytes() */
  EIB_FIFO fifo;             /*!< The FIFO for the soft realtime mode */

  EIB_DataPacketDescription pkt_head;
                             /*!< The list of all fields in a position data record for this EIB, for each region */
  int posdatasize;           /*!< The size of the position data record */
  int posdatasize_raw;       /*!< The size of the raw position data record */
  int posdata_allign;        /*!< This allignment specifies the basis for the size of the position data record */

  EIB_Axis axis[NUM_AXIS_PER_EIB];  /*!< The data for the axis of the EIB */
  EIB_Port port[NUM_PORTS_PER_EIB]; /*!< The data for the I/O ports of the EIB */
  
}EIB_Data;




#endif /*__EIB7_TYPES_H__*/
