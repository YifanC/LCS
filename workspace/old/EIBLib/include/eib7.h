#ifndef __EIB7__H__
#define __EIB7__H__

/*! \file eib7.h
    \brief This header file declares the public functions and data
    types for eib7 driver interface.

    Detailed description.
*/

#include <stdlib.h>

#ifdef __cplusplus
extern "C" 
{
#endif
  
/*! This enum lists all possible result codes for the driver interface */
typedef enum EIB7_ERR 
{
  EIB7_NoError = 0,                  /*!< Call was successful */
  EIB7_InvEnDatCmdCode = 0x50000001, /*!< Invalid command code for the EnDat command interface */
  EIB7_InvEnDatClock   = 0x50000002, /*!< Invalid EnDat clock */
  EIB7_EnDat22NotSupp  = 0x50000003, /*!< EnDat 2.2 is not supported by this encoder */
  EIB7_EnDatErrII      = 0x50000004, /*!< EnDat error type II */
  EIB7_EnDatIfBusy     = 0x50000005, /*!< EnDat interface busy */
  EIB7_EnDatXmitErr    = 0x50000006, /*!< EnDat transmission error (encoder may be not connected) */
  EIB7_MonotimeErr     = 0x50000007, /*!< invalid monotime for this encoder or EnDat clock */
  EIB7_InvInterface    = 0x60000001, /*!< invalid interface type (1Vss, EnDat) */
  EIB7_InvChannel      = 0x60000002, /*!< invalid channel */
  EIB7_NotInitialized  = 0x60000003, /*!< not initialised */
  EIB7_AccNotAllowed   = 0x60000004, /*!< access not allowed(e.g. Soft Realtime Mode enabled, EnDat master already in use, ...) */
  EIB7_InvEncoder      = 0x60000005, /*!< invalid encoder type (rotary/linear) */
  EIB7_InvRefMarkOpt   = 0x60000006, /*!< invalid reference mark option */
  EIB7_InvInterpolOpt  = 0x60000007, /*!< invalid interpolation option */
  EIB7_InvDistCodeRef  = 0x60000008, /*!< invalid option for distance coded reference marks */
  EIB7_ConfOptIncons   = 0x60000009, /*!< configuration options inconsistent */
  EIB7_FuncNotSupp     = 0x6000000A, /*!< function not supported */
  EIB7_CantClearEnc    = 0x6000000B, /*!< can not clear encoder supply error */
  EIB7_ParamInvalid    = 0x6000000C, /*!< parameter not valid */
  EIB7_EncPwrSuppErr   = 0x6000000D, /*!< encoder power supply error */
  EIB7_CantChIncInpTrm = 0x6000000E, /*!< can not change incremental input termination */
  EIB7_InvalidOpMode   = 0x60001001, /*!< operation mode not supported */
  EIB7_OpModeActive    = 0x60001002, /*!< operation mode is already active */
  EIB7_OpModeBlocked   = 0x60001003, /*!< operation mode blocked */
  EIB7_InvalidIPAddr   = 0x60001004, /*!< IP address invalid */
  EIB7_PacketTooLong   = 0x60002001, /*!< data packet too long */
  EIB7_InvalidPacket   = 0x60002002, /*!< data packet not valid */
  EIB7_RecDataReadErr  = 0x60003001, /*!< can not read recording data from memory */
  EIB7_CantLatchPos    = 0x70000001, /*!< can not latch position */
  EIB7_CantLatchDist   = 0x70000002, /*!< can not latch distance coded reference marks */
  EIB7_TrgNotEn        = 0x80000001, /*!< trigger output is not enabled */
  EIB7_TrgNotConf      = 0x80000002, /*!< trigger input/output is not configured as logical I/O */
  EIB7_TimerTrgEn      = 0x80000003, /*!< Timer Trigger is enabled */
  EIB7_ExtTrgEn        = 0x80000004, /*!< External Trigger is enabled */
  EIB7_InvTrgPeriod    = 0x80000005, /*!< invalid Timer Trigger period */
  EIB7_ICTrgEn         = 0x80000006, /*!< interval counter trigger enabled */
  EIB7_MasterTrgInv    = 0x80000007, /*!< master trigger invalid */
  EIB7_FirmwNotAvail   = 0x90000001, /*!< firmware not available */
  EIB7_CantRdSer       = 0x90000002, /*!< can not read serial number */
  EIB7_CantRdIdent     = 0x90000003, /*!< can not read ident. number */
  EIB7_NoCustNetwork   = 0x90000004, /*!< no custom network parameter available */
  EIB7_CantSaveCustNW  = 0x90000005, /*!< custom network parameter can not be saved */
  EIB7_CantSaveDHCP    = 0x90000006, /*!< can not save DHCP timeout */
  EIB7_DHCPTimeoutInv  = 0x90000007, /*!< DHCP timeout invalid */
  EIB7_HostnameTooLong = 0x90000008, /*!< hostname is to long/short */
  EIB7_HostnameInvalid = 0x90000009, /*!< hostname invalid */
  EIB7_CantSaveHostn   = 0x9000000A, /*!< can not save hostname */
  EIB7_CantRestDefHn   = 0x9000000B, /*!< can not restore default hostname */
  EIB7_CantRdHostname  = 0x9000000C, /*!< can not read hostname */
  EIB7_CantSetStatusLED= 0x90000031, /*!< can not set status LED */
  EIB7_NoDestIP        = 0x90000041, /*!< no destination IP for Soft Realtime Mode */
  EIB7_DestIPUnreach   = 0x90000042, /*!< destination IP for Soft Realtime Mode unreachable */
  EIB7_InvIPAddress    = 0x90000043, /*!< invalid IP address or port number */
  EIB7_SoftRTEn        = 0x90000044, /*!< Soft Realtime Mode already enabled */
  EIB7_CantTermConn    = 0x90000051, /*!< can not terminate connection */
  EIB7_CantTermSelf    = 0x90000052, /*!< can not terminate myself */
  EIB7_InvTSPeriod     = 0x90000061, /*!< invalid timestamp period */
  EIB7_BroadcastInv    = 0x90000062, /*!< use of broadcast address is not allowed */
  EIB7_PortNoInv       = 0x90000063, /*!< invalid port number */

  EIB7_CantInitWinSock = 0xA0000001, /*!< WSAStartup() call failed on WIN32 */
  EIB7_HostNotFound    = 0xA0000002, /*!< gethostbyname() could not resolve hostname */
  EIB7_CantOpenSocket  = 0xA0000003, /*!< the socket could not be created */
  EIB7_CantConnect     = 0xA0000004, /*!< could not connect the socket to the device */
  EIB7_SendError       = 0xA0000005, /*!< Error ocurred while sending data to the device */
  EIB7_ReceiveError    = 0xA0000006, /*!< Error ocurred while receiving data from the device */
  EIB7_OutOfMemory     = 0xA0000007, /*!< Required memory could not be allocated */
  EIB7_IllegalParameter= 0xA0000008, /*!< The parameter is invalid for this command */
  EIB7_InvalidResponse = 0xA0000009, /*!< The command response of the device is invalid */
  EIB7_InvalidHandle   = 0xA000000A, /*!< The handle passed to any EIB7* function is invalid */
  EIB7_ConnReset       = 0xA000000B, /*!< The connection was reset by the peer */
  EIB7_ConnTimeout     = 0xA000000C, /*!< The network request has timed out */
  EIB7_BufferTooSmall  = 0xA000000D, /*!< The destination memory size is too small */
  EIB7_CantStartThread = 0xA000000E, /*!< Cannot start the soft realtime thread */
  EIB7_ElementSizeInv  = 0xA000000F, /*!< FIFO element size mismatch */
  EIB7_FIFOEmpty       = 0xA0000010, /*!< The FIFO is empty, cannot retrieve element */
  EIB7_FieldNotAvail   = 0xA0000011, /*!< The data field requested is not available for the current connection */
  EIB7_IFVersionInv    = 0xA0000012, /*!< Invalid interface version */
  EIB7_FIFOOverflow    = 0xA0000013, /*!< The FIFO had an overflow, some data packets are lost */
  EIB7_PortDirInv      = 0xA0000014, /*!< The I/O port has the wrong direction for this operation */
} EIB7_ERR;

typedef int EIB7_HANDLE;

typedef int EIB7_AXIS;

typedef int EIB7_IO;

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(_WIN32_WCE)
	/* _WIN32 is defined even if _WIN64 is also defined */
  #ifdef _WIN64
  typedef __int64 ENCODER_POSITION;
  #else
  typedef __int64 ENCODER_POSITION;
  #endif /* _WIN64 */
#else
  typedef long long ENCODER_POSITION;
#endif

/*! The callback function definition for incoming data. This callback is called in soft realtime
  mode when data from the EIB arrives. It can be set with #EIB7SetDataCallback.
  Be aware, that this callback is invoked in a different thread
  from the main program, and thus can 'interrupt' the main program.
  The developer is responsible for proper locking.
  All EIB7* calls can be made safely from this callback, without locking, except for:
  - #EIB7Open()
  - #EIB7Close()
  - #EIB7SelectMode()
  \param eib handle of the EIB
  \param cnt number of new FIFO entries at time of call
  \param data pointer to user data
*/
typedef void (*EIB7OnDataAvailable)(EIB7_HANDLE eib, unsigned long cnt, void* data);

/*! This enum defines the possible data fields in a position data record. The fields
  marked global are sent only once in each position data message, the fields marked
  per encoder are sent 4 times, once for each encoder channel */

typedef enum 
{
  EIB7_PDF_EndOfList      = 0,      /*!< This field type marks the end of the list of data fields */
  EIB7_PDF_TriggerCounter = 0x0001, /*!< Trigger counter of the EIB (global) */ 
  EIB7_PDF_StatusWord     = 0x0002, /*!< per encoder */
  EIB7_PDF_PositionData   = 0x0004, /*!< per encoder */
  EIB7_PDF_Timestamp      = 0x0008, /*!< per encoder */
  EIB7_PDF_ReferencePos   = 0x0010, /*!< per encoder */
  EIB7_PDF_DistCodedRef   = 0x0020, /*!< per encoder */
  EIB7_PDF_Analog         = 0x0040, /*!< per encoder */
  EIB7_PDF_EnDat_AI1      = 0x0080, /*!< per encoder */
  EIB7_PDF_EnDat_AI2      = 0x0100  /*!< per encoder */
}EIB7_PositionDataField;


#define EIB7_PDF_MINVAL 1
#define EIB7_PDF_MAXVAL 0x0100



/*! This enum defines from which region in the realtime data the data field will be read */
typedef enum
{
  EIB7_DR_Global          = 0,    /*!< The field is in the global data area */ 
  EIB7_DR_Encoder1        = 1,    /*!< The field is in the encoder 1 region */
  EIB7_DR_Encoder2        = 2,    /*!< The field is in the encoder 2 region */
  EIB7_DR_Encoder3        = 3,    /*!< The field is in the encoder 3 region */
  EIB7_DR_Encoder4        = 4,    /*!< The field is in the encoder 4 region */
  EIB7_DR_AUX             = 9     /*!< The field is in the auxiliary axis region */
}EIB7_DataRegion;

#define EIB7_DR_MAXVAL 9

/*! This call delivers the size and the pointer of a data field from the position data.
  This call works for converted data only. For raw data, see #EIB7GetDataFieldPtrRaw.
  \param eib the handle to the device
  \param data the pointer to the position data or the encoder data
  \param region the data region in which to look up the field
  \param type the field to look up
  \param field a pointer to a pointer which receives the field's start address
  \param size a pointer to an integer which receives the field's size in bytes
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_FieldNotAvail if the field is not available in the position data
*/
EIB7_ERR EIB7GetDataFieldPtr(EIB7_HANDLE eib, void *data, EIB7_DataRegion region, EIB7_PositionDataField type, void **field, unsigned long *size);

/*! This call delivers the size and the pointer of a data field from the position data.
  This call works for raw data only. For converted data, see #EIB7GetDataFieldPtr.
  \param eib the handle to the device
  \param data the pointer to the position data or the encoder data
  \param region the data region in which to look up the field
  \param type the field to look up
  \param field a pointer to a pointer which receives the field's start address
  \param size a pointer to an integer which receives the field's size in bytes
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_FieldNotAvailable if the field is not available in the position data
*/
EIB7_ERR EIB7GetDataFieldPtrRaw(EIB7_HANDLE eib, void *data, EIB7_DataRegion region, EIB7_PositionDataField type, void **field, unsigned long *size);

/*! Returns error information for the given error code
  \param code the error code to retrieve information for
  \param mnemonic the string pointer where to store the error's name
  \param mnemlen the size of the buffer for the mnemonic (should at least be 16 bytes)
  \param message the string pointer where to store the error's full description
  \param msglen the size of the buffer for the message (should at least be 256 bytes)
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIB7GetErrorInfo(EIB7_ERR code, char *mnemonic, unsigned long mnemlen, char *message, unsigned long msglen);

/*! Resolves a host name (either as ASCII hostname, e.g. 'eib7.network.domain', 'eib7' or as
  IP address string , e.g. '192.168.1.2) to an unsigned long in host byte order.
  \param hostname contains either the ip-address or the hostname of the EIB7 as a string
  \param ip pointer to an unsigned long which receives the IP address
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIB7GetHostIP(const char *hostname, unsigned long *ip);

/*! Opens a TCP/IP socket to the EIB7 hardware. Returns an error if the connection could not
  be opened.
  \param ip contains the ip-address in host byte order
  \param eib if the call is successful, the handle to the eib connection object is placed herein
  \param timeout the timeout (in ms) that should be waited before the connection attempt is deemed a failure
  \param ident pointer to destination memory for the ident string (should be at least 9 bytes). If this value is != NULL, then the current firmware version of the EIB7 device is reported here
  \param len size of destination memory inclusive the zero byte for the C string 
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_IFVersionInv if the interface version of the EIB is incompatible to that of the driver
  - EIB7_CantOpenSocket if the socket could not be opened successfully 
  - EIB7_CantConnect if the socket could not connect to the EIB (EIB turned off, EIB unreachable)
*/

EIB7_ERR EIB7Open(unsigned long ip, EIB7_HANDLE *eib, long timeout, char *ident, unsigned long len);

/*! Closes the connection to the EIB7 hardware. All former opened child handles (axis, I/O)
  are closed as well.
  \param eib the handle to the device
  \return Some possible return codes are: 
  - EIB7_NoError if the socket could be opened
  - EIB7_InvalidHandle if the EIB handle is not valid
*/

EIB7_ERR EIB7Close(EIB7_HANDLE eib);

/*! The timeout for the TCP connection will be set to the new value. 
	The timeout will be used for all further commands.
  \param eib the handle to the device
  \param timeout the new timeout value in milliseconds. If the value specified
	is less than 100, the a timeout value of 100ms is used.
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_InvalidParameters if the EIB is not a TCP/IP based connection
*/

EIB7_ERR EIB7SetTimeout(EIB7_HANDLE eib, long timeout);

/*! The possible states for a connection represented by an EIB_HANDLE */

typedef enum EIB7_CONN_STATUS
{
  EIB7_CS_Connected           = 0, /*!< TCP connection established */
  EIB7_CS_Closed              = 1, /*!< TCP connection not established */
  EIB7_CS_Timeout             = 2, /*!< Timeout occurred */
  EIB7_CS_ConnectionReset     = 3, /*!< Connection Reset by the EIB */
  EIB7_CS_TransmissionError   = 4  /*!< e.g. socket layer error, ... */
}EIB7_CONN_STATUS;

/*! Returns the status of the connection for an EIB_HANDLE.
   \param eib the handle to the device
   \param status a pointer to the EIB7_CONN_STATUS result
   \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/

EIB7_ERR EIB7GetConnectionStatus(EIB7_HANDLE eib, EIB7_CONN_STATUS* status);


/*! Returns the number of axes in the EIB.
    \param eib handle for the EIB
    \param dsub (return value) number of axes with D-Sub connector
    \param res1 reserved
    \param res2 reserved
    \param res3 reserved
    \return Some possible return codes are: 
    - EIB7_NoError if the command executed without error
    - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIB7GetNumOfAxes(EIB7_HANDLE eib, unsigned long* dsub, unsigned long* res1, 
                          unsigned long* res2, unsigned long* res3);


/*! Returns one handle for every axis of the EIB. The number of axes depends on the type of the EIB.
    If the length of the set is smaller than the number of axes, only handles to the first N axes are returned.
    \param eib handle for the EIB
    \param set pointer to the first element of a set for the handles
    \param size maximum number of handles within the set
    \param len (return value) number of valid handles within the set
    \return Some possible return codes are: 
    - EIB7_NoError if the command executed without error
    - EIB7_InvalidHandle if the EIB handle is not valid
    - EIB7_OutOfMemory if not enough memory was available
*/
EIB7_ERR EIB7GetAxis(EIB7_HANDLE eib, EIB7_AXIS* set, unsigned long size, unsigned long* len);

/*! Returns one handle for every I/O port of the EIB. The number of ports depends on the type of the EIB.
    If the length of the sets is smaller than the number of ports, only handles to the first N ports are returned.
    \param eib handle for the EIB
    \param iset pointer to the first element of a set for the input handles
    \param isize maximum number of handles within the input handle set
    \param ilen (return value) number of valid handles within the input handle set
    \param oset pointer to the first element of a set for the output handles
    \param osize maximum number of handles within the output handle set
    \param olen (return value) number of valid handles within the output handle set
    \return Some possible return codes are: 
    - EIB7_NoError if the command executed without error
    - EIB7_InvalidHandle if the EIB handle is not valid
    - EIB7_OutOfMemory if not enough memory was available
*/
EIB7_ERR EIB7GetIO(EIB7_HANDLE eib,
		   EIB7_IO* iset, unsigned long isize, unsigned long* ilen,
		   EIB7_IO* oset, unsigned long osize, unsigned long* olen);

/*! The possible operating modes for the EIB */
typedef enum EIB7_OPERATING_MODE
{
  EIB7_OM_Polling = 0,           /*!< */
  EIB7_OM_SoftRealtime = 1,      /*!< */
  EIB7_OM_Streaming = 2,         /*!< */
  EIB7_OM_RecordingSingle = 3,   /*!< */
  EIB7_OM_RecordingRoll = 4      /*!< */
}EIB7_OPERATING_MODE;


#define EIB7_OM_MINVAL 0
#define EIB7_OM_MAXVAL 4

/*! Selects the operation mode of the EIB (Polling, Soft Realtime, Streaming, Recording). 
    The master connection is required
  to switch the mode.
  \param eib the handle for the EIB
  \param mode the operating mode
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_AccNotAllowed if another host uses the EIB in a operation mode except Polling
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_CantOpenSocket if the UDP socket could not be created
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_CantStartThread if the soft-realtime thread could not be started
  - EIB7_InvalidOpMode if the EIB does not support the operation mode
  - EIB7_OpModeActive if the operation mode is already active
  - EIB7_OpModeBlocked if any resource is locked
  - EIB7_InvalidIPAddr if the IP address for Soft Realtime mode is invalid

*/
EIB7_ERR EIB7SelectMode(EIB7_HANDLE eib, EIB7_OPERATING_MODE mode);


/*! Section of the data packet. One data packet can contain several sections. */
typedef struct
{
  EIB7_DataRegion region;                    /*!< The region in the data packet (global or encoder) */
  unsigned long items;                       /*!< The items within this region */

} EIB7_DataPacketSection;


/*! Configures a section of the data packet configuration.
  \param packet pointer to the packet configuration data (#EIB7_DataPacketSection)
  \param index 0 based index for the section within the packet configuration data
  \param region the region added to the data packet
  \param items the items within the region (#EIB7_PositionDataField)
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_IllegalParameter if the parameter is not valid

*/
EIB7_ERR EIB7AddDataPacketSection(EIB7_DataPacketSection* packet, unsigned long index, 
                                                   EIB7_DataRegion region, unsigned long items);


/*! Configures the data packet for the operation modes "Soft Realtime", "Recording", "Streaming".
  \param eib the handle for the EIB
  \param packet pointer to the packet configuration data (#EIB7_DataPacketSection)
  \param size number of valid entries in the packet configuration data
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_AccNotAllowed if the master interface could not be claimed
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_PacketTooLong if the packet configuration is too long
  - EIB7_InvalidPacket if the packet configuration is not valid
  - EIB7_IllegalParameter if the parameter is not valid

*/
EIB7_ERR EIB7ConfigDataPacket(EIB7_HANDLE eib, EIB7_DataPacketSection* packet, unsigned long size);


/*! An enum defining the enable or disable constants */
typedef enum EIB7_MODE 
{
  EIB7_MD_Disable = 0, /*!< */
  EIB7_MD_Enable = 1   /*!< */
}EIB7_MODE;

#define EIB7_MD_MINVAL 0
#define EIB7_MD_MAXVAL 1


/*! Starts or stops transmitting the recorded data from the EIB. The operation mode of the
    EIB must be set to #EIB7_OM_Polling.
  \param eib the handle for the EIB
  \param mode disable or enable the data transmission
  \param offset the first byte transmitted from the EIB
  \param length the number of bytes transmitted (0xFFFFFFFF = all data)
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_AccNotAllowed if the master interface could not be claimed
  - EIB7_OpModeBlocked if the operation mode of the EIB is not #EIB7_OM_Polling 
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_CantOpenSocket if the UDP socket could not be created
  - EIB7_CantStartThread if the thread could not be created
  - EIB7_RecDataReadErr if the data could not be read from memory
  - EIB7_IllegalParameter if the parameter is not valid (offset out of range)

*/
EIB7_ERR EIB7TransferRecordingData(EIB7_HANDLE eib, EIB7_MODE mode, unsigned long offset, unsigned long length);


/*! Gets the status of the recording memory of the EIB. The status information is invalid if
    the function was not executed successfully.
  \param eib the handle for the EIB
  \param length pointer to a variable to receive the number of samples currently stored in the memory
  \param status pointer to a variable to receiv the status of the recording mode
  \param progress pointer to a variable to receive the progress of the data transmission in percent
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available

*/
EIB7_ERR EIB7GetRecordingStatus(EIB7_HANDLE eib, unsigned long* length, unsigned long* status, unsigned long* progress);


/*! Gets the status of the streamin FIFO of the EIB. The status information is invalid if
    the function was not executed successfully.
  \param eib the handle for the EIB
  \param length pointer to a variable to receive the number of bytes currently stored in the fifo
  \param max pointer to a variable to receiv the maximum number of bytes in the fifo since start of streaming mode
  \param size pointer to a variable to receive the size of the fifo in bytes
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available

*/
EIB7_ERR EIB7GetStreamingStatus(EIB7_HANDLE eib, unsigned long* length, unsigned long* max, unsigned long* size);


/*! Gets the size of the memory for the recording mode. The size is specified as
      the maximum number of data packets in the memory. This information is only valid
      after configuring the data packet.
  \param eib the handle for the EIB
  \param size pointer to a variable to receive the size of the memory as a number of data packets
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_InvalidPacket if no packet configuration is available

*/
EIB7_ERR EIB7GetRecordingMemSize(EIB7_HANDLE eib, unsigned long* size);


/*! Sets the network settings for the EIB network interface.
  \param eib the handle for the EIB
  \param ip the destination ip address
  \param netmask the netmask for the EIB network interface
  \param gateway the ip address of the default gateway for the eib
  \param dhcp this flag specifies, wether DHCP should be turned on in the EIB 
  \param timeout the timeout for the DHCP client in seconds
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_CantSaveCustNW custom network parameter can not be saved 
  - EIB7_CantSaveDHCP can not save DHCP timeout 
  - EIB7_DHCPTimeoutInv DHCP timeout invalid 

*/
EIB7_ERR EIB7SetNetwork(EIB7_HANDLE eib, unsigned long ip, unsigned long netmask, unsigned long gateway,
			EIB7_MODE dhcp, unsigned long timeout);

/*! Retrieves the network settings for the EIB network interface.
  \param eib the handle for the EIB
  \param ip pointer to a variable that receives the destination ip address
  \param netmask pointer to a variable that receives the netmask
  \param gateway pointer to a variable that receives the ip address of the default gateway
  \param dhcp pointer to a variable that receives the DHCP status 
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_NoCustNetwork no custom network parameter available
*/
EIB7_ERR EIB7GetNetwork(EIB7_HANDLE eib, unsigned long *ip, unsigned long *netmask, unsigned long *gateway,
			EIB7_MODE *dhcp);

/*! Set the hostname for the EIB. The string must not be longer than 32 bytes including the
  terminating zero byte. If it is longer, it will be truncated.
  \param eib the handle for the EIB
  \param hostname a pointer to the new hostname string. If this parameter is a NULL-pointer
  or it points to an empty string then the default hostname will be restored
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_HostnameTooLong hostname is to long/short 
  - EIB7_HostnameInvalid hostname invalid 
  - EIB7_CantSaveHostn  can not save hostname 
  - EIB7_CantRestDefHn  can not restore default hostname 
*/
EIB7_ERR EIB7SetHostname(EIB7_HANDLE eib, const char *hostname);

/*! Get the hostname from the EIB. 
  \param eib the handle for the EIB
  \param hostname a pointer to the string that will receive the hostname
  \param len the size of the string in bytes
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_CantRdHostname  can not read hostname 
*/
EIB7_ERR EIB7GetHostname(EIB7_HANDLE eib, char *hostname, unsigned long len);

/*! Get the serial number of the EIB as C string
  \param eib the handle for the EIB
  \param serial pointer to destination memory for the serial number
  \param len size of destination memory inclusive the zero byte for the C string
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_CantRdSer can not read serial number
*/
EIB7_ERR EIB7GetSerialNumber(EIB7_HANDLE eib, char *serial, unsigned long len);

/*! Get the device ID of the EIB as C string
  \param eib the handle for the EIB
  \param ident pointer to destination memory for the device ID
  \param len size of destination memory inclusive the zero byte for the C string
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_CantRdIdent can not read ident. number
*/
EIB7_ERR EIB7GetIdentNumber(EIB7_HANDLE eib, char *ident, unsigned long len);

/*! Get the MAC address of the EIB ethernet interface in binary form
  \param eib the handle for the EIB
  \param mac to destination memory for the MAC address (>=6 byte free memory space)
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7GetMAC(EIB7_HANDLE eib, unsigned char* mac);

/*! This enum is used to select the firmware on which to operate */
typedef enum EIB7_FIRMWARE
{
  EIB7_FW_CurrentlyBooted = 0, /*!< */
  EIB7_FW_Factory         = 1, /*!< */
  EIB7_FW_User            = 2, /*!< */
}EIB7_FIRMWARE;

#define EIB7_FW_MINVAL 0
#define EIB7_FW_MAXVAL 2

/*! Get the versionID number of the EIB's firmware as a C-String.
  \param eib the handle for the EIB
  \param ident pointer to destination memory for the ident string (should be at least 9 bytes)
  \param len size of destination memory inclusive the zero byte for the C string 
  \param select an enum of type EIB_Firmware to elect the firmware to retrieve the version for
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_BufferTooSmall The destination memory size is too small
*/
EIB7_ERR EIB7GetVersion(EIB7_HANDLE eib, char * ident, unsigned long len, EIB7_FIRMWARE select);

/*! This enum specifies the current boot mode of the EIB*/
typedef enum EIB7_BOOT_MODE
{
  EIB7_BM_User        = 0, /*!< User Firmware, User Settings */
  EIB7_BM_FactoryUser = 1, /*!< Factory Firmware, User Settings */
  EIB7_BM_FactoryDefault = 2, /*!< Factory Firmware, Default Settings */
}EIB7_BOOT_MODE;


/*! Get the boot mode of the last reboot.
  \param eib the handle for the EIB
  \param mode pointer to an EIB7_BOOT_MODE enum 
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7GetBootMode(EIB7_HANDLE eib, EIB7_BOOT_MODE *mode);

/*! This enum specifies the status of the last update since reboot */
typedef enum EIB7_UPDATE_STATUS
{
  EIB7_US_NoUpdate            = 0, /*!< No update since reboot */
  EIB7_US_UpdateFailed        = 1, /*!< */
  EIB7_US_UpdateSuccessful    = 2, /*!< */
  EIB7_US_VersionIncompatible = 3 /*!< */
}EIB7_UPDATE_STATUS;

/*! Get the status of the last update attempt since last reboot.
  \param eib the handle for the EIB
  \param status pointer to an EIB_UpdateStatus enum
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIB7UpdateState(EIB7_HANDLE eib, EIB7_UPDATE_STATUS *status);

/*! The connection information for a single TCP/IP connection */
typedef struct EIB7_CONN_INFO 
{
  unsigned long  id;
  unsigned long  local_ip;
  unsigned short local_port;
  unsigned long  remote_ip;
  unsigned short remote_port;
}EIB7_CONN_INFO;

/*! Get the number of currently open connections on the EIB
  \param eib the handle for the EIB
  \param cnt pointer to a variable that receives the number of open connections
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7GetNumberOfOpenConnections(EIB7_HANDLE eib, unsigned long * cnt);

/*! Get information on all currently open connections to the EIB
  \param eib the handle for the EIB
  \param info pointer to the first element of a set of type EIB7_CONN_INFO[ ].
  \param size maximum number of entries in the set
  \param cnt pointer to a variable that receives the number of open connections
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7ConnectionInfo(EIB7_HANDLE eib, EIB7_CONN_INFO* info,
			    unsigned long size, unsigned long * cnt);

/*! Request the EIB to terminate an open TCP connection with a TCP reset. The
   EIB does not close the connection which is used to send this command.
  \param eib the handle for the EIB
  \param id ID of the open connection
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
  
EIB7_ERR EIB7TerminateConnection(EIB7_HANDLE eib, unsigned long id);

/*! Get the clock ticks per microsecond of the Timestamp timer.
  \param eib the handle for the EIB
  \param ticks pointer to a variable that receives the clock ticks per microsecond
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/


EIB7_ERR EIB7GetTimestampTicks(EIB7_HANDLE eib, unsigned long * ticks);

/*! Set the Timestamp period in clock ticks.
  \param eib the handle for the EIB
  \param period period of the Timestamp timer in clock ticks,. This value must be > 0.
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvTSPeriod invalid timestamp period 
*/

EIB7_ERR EIB7SetTimestampPeriod(EIB7_HANDLE eib, unsigned long period);

/*! Reset the Timestamp counter to zero
  \param eib the handle for the EIB
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7ResetTimestamp(EIB7_HANDLE eib);

/*! Get the clock ticks per microsecond of the Timer Trigger timer.
  \param eib the handle for the EIB
  \param ticks pointer to a variable that receives the clock ticks per microsecond
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/

EIB7_ERR EIB7GetTimerTriggerTicks(EIB7_HANDLE eib, unsigned long * ticks);

/*! Set the Timer Trigger period in clock ticks.
  \param eib the handle for the EIB
  \param period period of the Timer Trigger timer in clock ticks,. This value must be > 0.
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvTrgPeriod invalid Timer Trigger period 
*/
EIB7_ERR EIB7SetTimerTriggerPeriod(EIB7_HANDLE eib, unsigned long period);


/*! Specifies the mode for the interval counter trigger */
typedef enum EIB7_IntervalCounterMode
{
  EIB7_ICM_Disable       = 0,/*!< */
  EIB7_ICM_Single        = 1,/*!< */
  EIB7_ICM_Periodic      = 2 /*!< */
}EIB7_IntervalCounterMode;

#define EIB7_ICM_MINVAL 0
#define EIB7_ICM_MAXVAL 2

/*! Specifies the start position for the interval counter trigger */
typedef enum EIB7_IntervalCounterStart
{
  EIB7_ICS_Current       = 0,/*!< */
  EIB7_ICS_StartPos      = 1 /*!< */
}EIB7_IntervalCounterStart;

#define EIB7_ICS_MINVAL 0
#define EIB7_ICS_MAXVAL 2


/*! Set interval counter trigger
  \param eib the handle for the EIB
  \param mode operation mode
  \param start start at current position or at start position
  \param startpos start position
  \param period period for periodic trigger
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid mode or start option 
*/
EIB7_ERR EIB7SetIntervalCounterTrigger(EIB7_HANDLE eib, EIB7_IntervalCounterMode mode, 
                                 EIB7_IntervalCounterStart start, unsigned long startpos, unsigned long period);


/*! Specifies the interpolation factor for the interval counter */
typedef enum EIB7_IntervalCounterIPF
{
  EIB7_ICF_1x       = 0,/*!< */
  EIB7_ICF_2x       = 1,/*!< */
  EIB7_ICF_4x       = 2,/*!< */
  EIB7_ICF_5x       = 3,/*!< */
  EIB7_ICF_10x      = 4,/*!< */
  EIB7_ICF_20x      = 5,/*!< */
  EIB7_ICF_25x      = 6,/*!< */
  EIB7_ICF_50x      = 7,/*!< */
  EIB7_ICF_100x     = 8 /*!< */

}EIB7_IntervalCounterIPF;

#define EIB7_ICF_MINVAL 0
#define EIB7_ICF_MAXVAL 8

/*! Specifies the edge resolution for the interval counter */
typedef enum EIB7_IntervalCounterEdge
{
  EIB7_ICE_1x       = 0,/*!< */
  EIB7_ICE_2x       = 1,/*!< */
  EIB7_ICE_4x       = 2 /*!< */
}EIB7_IntervalCounterEdge;

#define EIB7_ICE_MINVAL 0
#define EIB7_ICE_MAXVAL 2


/*! Set interval counter interpolation
  \param eib the handle for the EIB
  \param ipf interpolation factor
  \param edge edge resolution
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid interpolation factor or edge resolution
*/
EIB7_ERR EIB7SetIntervalCounterInterpolation(EIB7_HANDLE eib, EIB7_IntervalCounterIPF ipf, 
                                             EIB7_IntervalCounterEdge edge);


/*! Specifies the trigger source for the trigger output */
typedef enum EIB7_OutputTriggerSrc
{
  EIB7_OT_TrgInSync       = 0,/*!< */
  EIB7_OT_TrgInAsync      = 1,/*!< */
  EIB7_OT_TrgSW1          = 2,/*!< */
  EIB7_OT_TrgSW2          = 3,/*!< */
  EIB7_OT_TrgSW3          = 4,/*!< */
  EIB7_OT_TrgSW4          = 5,/*!< */
  EIB7_OT_TrgRImaskedCH1  = 6,/*!< */
  EIB7_OT_TrgIC           = 7,/*!< */
  EIB7_OT_TrgPuls         = 8,/*!< */
  EIB7_OT_TrgTimer        = 9 /*!< */
}EIB7_OutputTriggerSrc;

#define EIB7_OT_MINVAL 0
#define EIB7_OT_MAXVAL 9


/*! Set trigger source for trigger output
  \param io handle for the trigger output
  \param src trigger source
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the IO handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid trigger source
*/
EIB7_ERR EIB7OutputTriggerSource(EIB7_IO io, EIB7_OutputTriggerSrc src);


/*! Specifies the trigger source for the axis */
typedef enum EIB7_AxisTriggerSrc
{
  EIB7_AT_TrgInput1       = 0,/*!< */
  EIB7_AT_TrgInput2       = 1,/*!< */
  EIB7_AT_TrgInput3       = 2,/*!< */
  EIB7_AT_TrgInput4       = 3,/*!< */
  EIB7_AT_TrgSW1          = 4,/*!< */
  EIB7_AT_TrgSW2          = 5,/*!< */
  EIB7_AT_TrgSW3          = 6,/*!< */
  EIB7_AT_TrgSW4          = 7,/*!< */
  EIB7_AT_TrgRI           = 8,/*!< */
  EIB7_AT_TrgRImaskedCH1  = 9,/*!< */
  EIB7_AT_TrgIC           = 10,/*!< */
  EIB7_AT_TrgPuls         = 11,/*!< */
  EIB7_AT_TrgTimer        = 12 /*!< */
}EIB7_AxisTriggerSrc;

#define EIB7_AT_MINVAL 0
#define EIB7_AT_MAXVAL 12


/*! Set trigger source for the axis
  \param axis handle for the axis
  \param src trigger source
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the AXIS handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid trigger source
*/
EIB7_ERR EIB7AxisTriggerSource(EIB7_AXIS axis, EIB7_AxisTriggerSrc src);

/*! Set trigger source for the auxiliary axis
  \param eib the handle for the EIB
  \param src trigger source
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid trigger source
*/
EIB7_ERR EIB7AuxAxisTriggerSource(EIB7_HANDLE eib, EIB7_AxisTriggerSrc src);


/*! Set master trigger source
  \param eib the handle for the EIB
  \param src trigger source
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid trigger source
*/
EIB7_ERR EIB7MasterTriggerSource(EIB7_HANDLE eib, EIB7_AxisTriggerSrc src);


/*! Trigger Sources */
typedef enum EIB7_TriggerSource
{
  EIB7_TS_TrgInput1       = 0x00000001,/*!< */
  EIB7_TS_TrgInput2       = 0x00000002,/*!< */
  EIB7_TS_TrgInput3       = 0x00000004,/*!< */
  EIB7_TS_TrgInput4       = 0x00000008,/*!< */
  EIB7_TS_TrgRI1          = 0x00000010,/*!< */
  EIB7_TS_TrgRI2          = 0x00000020,/*!< */
  EIB7_TS_TrgRI3          = 0x00000040,/*!< */
  EIB7_TS_TrgRI4          = 0x00000080,/*!< */
  EIB7_TS_TrgRImaskedCH1  = 0x00000100,/*!< */
  EIB7_TS_TrgIC           = 0x00000200,/*!< */
  EIB7_TS_TrgPuls         = 0x00000400,/*!< */
  EIB7_TS_TrgTimer        = 0x00000800,/*!< */
  EIB7_TS_All             = 0xFFFFFFFF
}EIB7_TriggerSource;


/*! Enables or disables the trigger sources
  \param eib the handle for the EIB
  \param mode enable or disable sources
  \param src trigger sources
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid trigger source
*/
EIB7_ERR EIB7GlobalTriggerEnable(EIB7_HANDLE eib, EIB7_MODE mode, unsigned long src);


/*! Specifies the start signal for the puls counter */
typedef enum EIB7_PulsCounterStart
{
  EIB7_PS_TrgInput1       = 0,/*!< */
  EIB7_PS_TrgInput2       = 1,/*!< */
  EIB7_PS_TrgInput3       = 2,/*!< */
  EIB7_PS_TrgInput4       = 3,/*!< */
  EIB7_PS_TrgRI1          = 4,/*!< */
  EIB7_PS_TrgRI2          = 5,/*!< */
  EIB7_PS_TrgRI3          = 6,/*!< */
  EIB7_PS_TrgRI4          = 7,/*!< */
  EIB7_PS_TrgSW1          = 8,/*!< */
  EIB7_PS_TrgSW2          = 9,/*!< */
  EIB7_PS_TrgSW3          = 10,/*!< */
  EIB7_PS_TrgSW4          = 11,/*!< */
  EIB7_PS_TrgRImaskedCH1  = 12, /*!< */
  EIB7_PS_TrgIC           = 13 /*!< */
}EIB7_PulsCounterStart;

#define EIB7_PS_MINVAL 0
#define EIB7_PS_MAXVAL 13


/*! Specifies the trigger signal for the puls counter */
typedef enum EIB7_PulsCounterTrigger
{
  EIB7_PT_TrgInput1       = 0,/*!< */
  EIB7_PT_TrgInput2       = 1,/*!< */
  EIB7_PT_TrgInput3       = 2,/*!< */
  EIB7_PT_TrgInput4       = 3,/*!< */
  EIB7_PT_TrgRI1          = 4,/*!< */
  EIB7_PT_TrgRI2          = 5,/*!< */
  EIB7_PT_TrgRI3          = 6,/*!< */
  EIB7_PT_TrgRI4          = 7,/*!< */
  EIB7_PT_TrgRImaskedCH1  = 8, /*!< */
  EIB7_PT_TrgIC           = 9, /*!< */
  EIB7_PT_TrgTimer        = 10, /*!< */
}EIB7_PulsCounterTrigger;

#define EIB7_PT_MINVAL 0
#define EIB7_PT_MAXVAL 10


/*! Configuration for the puls counter
  \param eib the handle for the EIB
  \param start start signal for puls counter
  \param trigger trigger source
  \param count number of trigger pulses
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the AXIS handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid parameter
*/
EIB7_ERR EIB7ConfigPulsCounter(EIB7_HANDLE eib, EIB7_PulsCounterStart start, 
                               EIB7_PulsCounterTrigger trigger, unsigned long count);


/*! Get the clock ticks per microsecond for the trigger input delay
  \param eib the handle for the EIB
  \param ticks pointer to a variable that receives the clock ticks per microsecond
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the IO handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7GetTriggerDelayTicks(EIB7_HANDLE eib, unsigned long* ticks);


/*! Set the delay for trigger inputs
  \param io handle for the trigger input
  \param dly delay
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the IO handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid parameter
*/
EIB7_ERR EIB7SetTriggerInputDelay(EIB7_IO io, unsigned long dly);


/*! Specifies the active edge of the reference signal for triggering */
typedef enum EIB7_RITriggerEdge
{
  EIB7_RI_Rising        = 0,/*!< */
  EIB7_RI_Falling       = 1,/*!< */
  EIB7_RI_Both          = 2 /*!< */
}EIB7_RITriggerEdge;

#define EIB7_RI_MINVAL 0
#define EIB7_RI_MAXVAL 2


/*! Set the active edge of the reference signal for triggering
  \param axis handle for the axis
  \param edge trigger edge
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the AXIS handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid parameter
*/
EIB7_ERR EIB7SetRITriggerEdge(EIB7_AXIS axis, EIB7_RITriggerEdge edge);


/*! Set the active edge of the reference signal for triggering for the auxiliary axis
  \param axis handle for the axis
  \param edge trigger edge
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the AXIS handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid invalid parameter
*/
EIB7_ERR EIB7AuxSetRITriggerEdge(EIB7_HANDLE eib, EIB7_RITriggerEdge edge);

/*! Reset the Trigger counter to zero
  \param eib the handle for the EIB
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7ResetTriggerCounter(EIB7_HANDLE eib);

/*! Enable or disable the termination resistors for the incremental encoder inputs
  \param eib the handle for the EIB
  \param mode an EIB7_MODE to enable or disable the termination
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7EnableIncrementalTermination(EIB7_HANDLE eib, EIB7_MODE mode);


/*! Specifies the software trigger source */
typedef enum EIB7_SWTrigger
{
  EIB7_ST_SWtrigger1        = 1,/*!< */
  EIB7_ST_SWtrigger2        = 2,/*!< */
  EIB7_ST_SWtrigger3        = 3,/*!< */
  EIB7_ST_SWtrigger4        = 4 /*!< */
}EIB7_SWTrigger;

#define EIB7_ST_MINVAL 0
#define EIB7_ST_MAXVAL 4

/*! Generates a software trigger event that causes the EIB to send data via soft realtime link. 
  \param eib the handle for the EIB
  \param source trigger source
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7SoftwareTrigger(EIB7_HANDLE eib, unsigned long source);

/*! Reset the EIB. After this call, all connections will be closed.
  Please note, that a call to #EIB7Close() is necessary afterwards, to
  avoid memory leakage and safely stop the soft-rt thread, if running.
  \param eib the handle for the EIB
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7Reset(EIB7_HANDLE eib);

/*! Enable or disable blinking mode of the LED on the front panel
  trigger is already activated
  \param eib the handle for the EIB
  \param mode an EIB7_MODE to enable or disable blinking
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_CantSetStatusLED can not set status LED
*/
EIB7_ERR EIB7Identify(EIB7_HANDLE eib, EIB7_MODE mode);

/*! Copy data from the soft-realtime FIFO to destination memory. If the FIFO contains less
  than cnt entries only the available entries will be copied. The functions waits for at
  least one entry if none are available, but for max. timeout ms. All data fields are
  little-endian.
  \param eib the handle for the EIB
  \param data pointer to destination memory
  \param cnt number of requested entries (>=0)
  \param entries (return value) number of copied FIFO entries
  \param timeout Timeout value in milliseconds. -1 means infinite.
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
  - EIB7_FIFOEmpty if no data was in the FIFO
  - EIB7_ElementSizeInv if the data size of the FIFO entry does not match (internal error)
  - EIB7_FIFOOverflow if the FIFO overflowed, so that data was lost. Please note, that
  valid data may be returned in that case. Check the 'entries' parameter.
*/
EIB7_ERR EIB7ReadFIFODataRaw(EIB7_HANDLE eib, void* data, unsigned long cnt,
			     unsigned long* entries, long timeout);

/*! Copy data from the soft-realtime FIFO to destination memory. If the FIFO contains less
  than cnt entries only the available entries will be copied. The functions waits for at
  least one entry if none are available, but for max. timeout ms. This function converts the
  6-Byte raw encoder positions into 8-Byte ENCODER_POSITION values. 
  \param eib the handle for the EIB
  \param data pointer to destination memory
  \param cnt number of requested entries (>=0)
  \param entries (return value) number of copied FIFO entries
  \param timeout Timeout value in milliseconds. -1 means infinite.
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
  - EIB7_FIFOEmpty if no data was in the FIFO
  - EIB7_ElementSizeInv if the data size of the FIFO entry does not match (internal error)
  - EIB7_FIFOOverflow if the FIFO overflowed, so that data was lost. Please note, that
  valid data may be returned in that case. Check the 'entries' parameter.
*/
EIB7_ERR EIB7ReadFIFOData(EIB7_HANDLE eib, void* data, unsigned long cnt,
			  unsigned long* entries, long timeout);


/*! Clear all data currently in the soft-realtime FIFO.
  \param eib the handle for the EIB
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
  - EIB7_InvalidHandle if the EIB handle is not valid
*/

EIB7_ERR EIB7ClearFIFO(EIB7_HANDLE eib);

/*! Get the current size (in bytes) of the soft-realtime FIFO.
  \param eib the handle for the EIB
  \param size pointer to a variable that receives the current FIFO size
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
  - EIB7_InvalidHandle if the EIB handle is not valid
*/

EIB7_ERR EIB7GetFIFOSize(EIB7_HANDLE eib, unsigned long *size);

/*! Set the size (in bytes) of the soft-realtime FIFO. All data currently in
  the FIFO is discarded. If the soft-realtime mode is active, the size will not be set.
  \param eib the handle for the EIB
  \param size the requested FIFO size in bytes. 
  \return Some possible return codes are: 
  - EIB7_NoError if the call returns successfully
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_SoftRTEn if soft-realtime mode is active
*/

EIB7_ERR EIB7SetFIFOSize(EIB7_HANDLE eib, unsigned long size);

/*! This call returns the size of a converted position data record.
  \param eib the handle for the EIB
  \param size pointer to a variable that receives the actual size
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIB7SizeOfFIFOEntry(EIB7_HANDLE eib, unsigned long* size);

/*! This call returns the size of a raw position data record.
  \param eib the handle for the EIB
  \param size pointer to a variable that receives the actual size
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIB7SizeOfFIFOEntryRaw(EIB7_HANDLE eib, unsigned long* size);

/*! This call returns the number of entries in the soft-realtime FIFO
  \param eib the handle for the EIB
  \param cnt pointer to a variable that receives the actual number of entries
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIB7FIFOEntryCount(EIB7_HANDLE eib, unsigned long* cnt);

/*! Initialize callback mechanism with user-defined function
  \param eib the handle for the EIB
  \param data pointer to user data, this pointer will be passed as parameter to the
  callback function with every call
  \param activate activation of the callback mechanism: 0=inactive, 1=active
  \param threshold number of packets that have to be available before the callback function is invoked
  \param handler pointer to callback function (NULL-pointer allowed if activate = 0)
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIB7SetDataCallback(EIB7_HANDLE eib, void* data, EIB7_MODE activate, unsigned long threshold, EIB7OnDataAvailable handler);


/*! Specifies the type of the encoder */
typedef enum EIB7_EncoderType
{
  EIB7_EC_Linear = 0, /*!< */
  EIB7_EC_Rotary = 1  /*!< */
}EIB7_EncoderType;

#define EIB7_EC_MINVAL 0
#define EIB7_EC_MAXVAL 1

/*! Specifies the type of refmarks */
typedef enum EIB7_Refmarks
{
  EIB7_RM_None          = 0, /*!< */
  EIB7_RM_One           = 1, /*!< */
  EIB7_RM_DistanceCoded = 2  /*!< */
}EIB7_Refmarks;

#define EIB7_RM_MINVAL 0
#define EIB7_RM_MAXVAL 2

/*! Specifies wether a limit signal is available */
typedef enum EIB7_Limit
{
  EIB7_LS_None      = 0,/*!< */
  EIB7_LS_Available = 1 /*!< */
}EIB7_Limit;

#define EIB7_LS_MINVAL 0
#define EIB7_LS_MAXVAL 1

/*! Specifies wether a homing signal is available */
typedef enum EIB7_Homing
{
  EIB7_HS_None      = 0,/*!< */
  EIB7_HS_Available = 1 /*!< */
}EIB7_Homing;

#define EIB7_HS_MINVAL 0
#define EIB7_HS_MAXVAL 1

/*! Specifies wether signal compensation is active */
typedef enum EIB7_Compensation
{
  EIB7_CS_None       = 0,/*!< */
  EIB7_CS_CompActive = 1 /*!< */
}EIB7_Compensation;

#define EIB7_CS_MINVAL 0
#define EIB7_CS_MAXVAL 1

/*! Specifies the encoder's bandwidth */
typedef enum EIB7_Bandwidth
{
  EIB7_BW_High  = 0,/*!< */
  EIB7_BW_Low   = 1 /*!< */
}EIB7_Bandwidth;

#define EIB7_BW_MINVAL 0
#define EIB7_BW_MAXVAL 1

/*! Specifies the EnDat Interface flags. Please note, that these can be ORed together! */
typedef enum EIB7_InterfaceType
{
  EIB7_IT_Disabled              = 0, /*!< To disable the axis */
  EIB7_IT_Incremental           = 1, /*!< For incremental encoders (1Vss) */
  EIB7_IT_EnDat21               = 2, /*!< EnDat 2.1 protocol */
  EIB7_IT_EnDat01               = 3, /*!< EnDat 0.1 protocol */
  EIB7_IT_EnDat22               = 4, /*!< EnDat 2.2 protocol */
  EIB7_IT_EnDatDelayMeasurement = 8, /*!< only available for EnDat 2.2 protocol */
  EIB7_IT_Incremental_11u    = 2049  /*!< For incremental encoders (11uA) */
}EIB7_InterfaceType;

#define EIB7_IT_MINVAL 0
#define EIB7_IT_MAXVAL 2063 /*!< combination of all possible bits */

/*! Specifies the EnDat Interface clocks. */
typedef enum EIB7_InterfaceClock
{
  EIB7_CLK_Default = 0, /*!< */
  EIB7_CLK_100KHz  = 100000, /*!< */
  EIB7_CLK_300KHz  = 300000, /*!< */
  EIB7_CLK_500KHz  = 500000, /*!< */
  EIB7_CLK_1MHz    = 1000000, /*!< */
  EIB7_CLK_2MHz    = 2000000, /*!< */
  EIB7_CLK_4MHz    = 4000000, /*!< */
  EIB7_CLK_5MHz    = 5000000, /*!< */
  EIB7_CLK_6_66MHz = 6666666 /*!< */
}EIB7_InterfaceClock;

/*! Specifies the recovery time I for EnDat encoders */
typedef enum EIB7_EnDatRecoveryTime
{
  EIB7_RT_Long       = 0,/*!< */
  EIB7_RT_Short      = 1 /*!< */
}EIB7_EnDatRecoveryTime;

#define EIB7_RT_MINVAL 0
#define EIB7_RT_MAXVAL 1

/*! Specifies the calculation time for EnDat encoders */
typedef enum EIB7_EnDatCalcTime
{
  EIB7_CT_Long       = 0,/*!< */
  EIB7_CT_Short      = 1 /*!< */
}EIB7_EnDatCalcTime;

#define EIB7_CT_MINVAL 0
#define EIB7_CT_MAXVAL 1

  
/*! Initializes an axis to the specified encoder settings.
  \param axis handle for the axis
  \param iface interface type (incremental, EnDat) (#EIB7_InterfaceType)
  \param type encoder type (#EIB7_EncoderType)
  \param refmarks number of reference marks (#EIB7_Refmarks)
  \param linecounts counts per revolution
  \param increment nominal increment between two fixed reference marks
  \param homing homing signal (#EIB7_Homing)
  \param limit limit signal (#EIB7_Limit)
  \param comp signal compensation (#EIB7_Compensation)
  \param bandwidth bandwidth for incremental encoder inputs (#EIB7_Bandwidth)
  \param EnDatclock EnDat clock in Hz (#EIB7_InterfaceClock)
  \param recovery recovery time I for EnDat encoders (#EIB7_EnDatRecoveryTime)
  \param calculation calculation time for EnDat encoders (#EIB7_EnDatCalcTime)
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_InvChannel      invalid channel 
  - EIB7_InvInterface    invalid interface type (1Vss, EnDat) 
  - EIB7_InvEncoder      invalid encoder type (rotary/linear) 
  - EIB7_InvRefMarkOpt   invalid reference mark option 
  - EIB7_InvInterpolOpt  invalid interpolation option 
  - EIB7_InvDistCodeRef  invalid option for distance coded reference marks 
  - EIB7_ConfOptIncons   configuration options inconsistent 
  - EIB7_AccNotAllowed   access not allowed(e.g. Soft Realtime Mode enabled, EnDat master already in use, ...) 
*/
  
EIB7_ERR EIB7InitAxis(EIB7_AXIS axis,
		      unsigned long iface,
		      EIB7_EncoderType type,
		      EIB7_Refmarks refmarks,
		      unsigned long linecounts,
		      unsigned long increment,
		      EIB7_Homing homing,
		      EIB7_Limit limit,
		      EIB7_Compensation comp,
		      EIB7_Bandwidth bandwidth,
		      unsigned long EnDatclock,
            EIB7_EnDatRecoveryTime recovery,
            EIB7_EnDatCalcTime calculation);


/*! Resets the incremental counter for this axis. Generates an error if the
  axis is not configured for incremental encoders.
  \param axis handle for the axis
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid    parameter not valid 
*/
EIB7_ERR EIB7ClearCounter(EIB7_AXIS axis);

/*! Returns the current position of the encoder. Returns an error if the EIB is not in polling mode.
  \param axis handle for the axis
  \param status status word
  \param pos position value
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7GetPosition(EIB7_AXIS axis, unsigned short *status, ENCODER_POSITION *pos);

/*! Read the current position value and status information from the specified encoder.
  The position will be latched. The EIB must be in polling mode.
  \param axis handle for the axis
  \param status pointer to a variable which receives the status word.
  \param pos pointer to a variable which receives the position value
  \param ref1 pointer to a variable which receives the reference position 1
  \param ref2 pointer to a variable which receives the reference position 2
  \param refc pointer to a variable which receives the distance coded reference position
  \param timestamp pointer to a variable which receives the timestamp for the position value
  \param counter pointer to a variable which receives the trigger counter
  \param adc00 pointer to a variable which receives the analog value for signal A
  \param adc90 pointer to a variable which receives the analog value for signal B
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_TimerTrgEn Timer Trigger is enabled
  - EIB7_ExtTrgEn External Trigger is enabled
  - EIB7_SoftRTEn Soft Realtime Mode is enabled
*/  
  
EIB7_ERR EIB7GetEncoderData(EIB7_AXIS axis,
			    unsigned short* status,
			    ENCODER_POSITION* pos,
			    ENCODER_POSITION* ref1,
			    ENCODER_POSITION* ref2,
			    ENCODER_POSITION* refc,
			    unsigned long* timestamp,
			    unsigned short* counter,
			    unsigned short* adc00,
			    unsigned short* adc90);

/*! The power supply error flag will be cleared. The EIB generates an error if the power supply error flag was not set.
  It does not matter whether the axis is configured for EnDat or incremental encoders
  \param axis handle for the axis
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/

EIB7_ERR EIB7ClearPowerSupplyError(EIB7_AXIS axis);

/*! The lost trigger flag flag will be cleared. 
  It does not matter whether the axis is configured for EnDat or incremental encoders
  \param axis handle for the axis
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/

EIB7_ERR EIB7ClearLostTriggerError(EIB7_AXIS axis);

/*! Clears the error flags for the encoder. For incremental encoders only the amplitude
  error flag and the noise error flag will be cleared, for EnDat encoders only the CRC
  error flag and the EnDat alarm messages will be cleared.
  It does not matter whether the axis is configured for EnDat or incremental encoders
  \param axis handle for the axis
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7ClearEncoderErrors(EIB7_AXIS axis);

/*! The EnDat error messages will be cleared.
  \param axis handle for the axis
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_InvalidInterface if the channel is configured for incremental encoder
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7ClearEnDatErrorMsg(EIB7_AXIS axis);

/*! Clears the flags for 'Reference position latched'.
  \param axis handle for the axis
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
*/
EIB7_ERR EIB7ClearRefLatched(EIB7_AXIS axis);

/*! Clears all flags for the reference, including 'valid',
  'latched', and 'error'-flags.
  \param axis handle for the axis
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/

EIB7_ERR EIB7ClearRefStatus(EIB7_AXIS axis);

/*! The valid reference positions for with to start the
  latching process
*/
typedef enum 
  {
    EIB7_RP_RefPos1 = 1, /*!< */
    EIB7_RP_RefPos2 = 2  /*!< */
  }EIB7_ReferencePosition;

#define EIB7_RP_MINVAL 1
#define EIB7_RP_MAXVAL 2

/*! The reference position latch operation will be started.
  \param axis handle for the axis
  \param ref indicates the reference position to latch
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_InvalidInterface if the channel is not configured for incremental encoder
*/

EIB7_ERR EIB7StartRef(EIB7_AXIS axis, EIB7_ReferencePosition ref);

/*! The reference position latch operation will be aborted.
  \param axis handle for the axis
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvalidInterface if the channel is not configured for incremental encoder
*/

EIB7_ERR EIB7StopRef(EIB7_AXIS axis);

/*! Returns the status of the reference position latch procedure for the given axis.
  \param axis handle for the axis
  \param active pointer to a variable that receives the current status
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvalidInterface if the channel is not configured for incremental encoder
*/


EIB7_ERR EIB7GetRefActive(EIB7_AXIS axis, EIB7_MODE* active);

/*! The reference mark check can be configured.
  \param axis handle for the axis
  \param mode enable or disable the reference mark check
  \param limit limit for the reference mark check
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvalidInterface if the channel is not configured for incremental encoder
*/
  
EIB7_ERR EIB7SetRefCheck(EIB7_AXIS axis, EIB7_MODE mode, unsigned long limit);


/*! Returns the current position of the EnDat 2.1 encoder. 
  \param axis handle for the axis
  \param status status word
  \param pos position value
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvalidInterface if the channel is configured for incremental encoder
*/
EIB7_ERR EIB7EnDat21GetPosition(EIB7_AXIS axis, unsigned short *status, ENCODER_POSITION *pos);


/*! Selects a memory range for the encoder.
  \param axis handle for the axis
  \param mrs MRS code for the encoder
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat21SelectMemRange(EIB7_AXIS axis, unsigned char mrs);

/*! Writes data to a specified encoder address
  \param axis handle for the axis
  \param addr address for encoder memory
  \param data data programmed to encoder memory
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat21WriteMem(EIB7_AXIS axis, unsigned char addr,  unsigned short data);

/*! Reads data from a specified encoder address
  \param axis handle for the axis
  \param addr address for encoder memory
  \param data data read from encoder memory
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat21ReadMem(EIB7_AXIS axis, unsigned char addr,  unsigned short *data);

/*! Resets the encoder
  \param axis handle for the axis
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat21ResetEncoder(EIB7_AXIS axis);

/*! Read test values from the encoder
  \param axis handle for the axis
  \param high most significant word of the test value (only bits D0..D7 valid)
  \param low least significant word of the test value
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat21ReadTestValue(EIB7_AXIS axis, unsigned long *high, unsigned long *low);

/*! Write test values to the encoder
  \param axis handle for the axis
  \param port port address for the encoder
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat21WriteTestCommand(EIB7_AXIS axis, unsigned char port);

/*! Additional information data structure for EnDat 2.2 encoders */
typedef struct
{
  unsigned short status;
  unsigned short info;
}ENDAT_ADDINFO;

/*! Returns the current position of the EnDat 2.2 encoder plus additional infos. 
  \param axis handle for the axis
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/

EIB7_ERR EIB7EnDat22GetPosition(EIB7_AXIS axis, unsigned short* status, ENCODER_POSITION* pos,
				ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2);


/*! Returns the current position of the EnDat 2.2 encoder and selects a memory range. 
  \param axis handle for the axis
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \param mrs MRS code for the encoder
  \param block block address for the encoder
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat22SelectMemRange(EIB7_AXIS axis, unsigned short* status,
				   ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				   unsigned char mrs, unsigned char block);


/*! Returns the current position of the EnDat 2.2 encoder and write to selected memory area.
  \param axis handle for the axis
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \param addr address for the encoder memory
  \param data data programmed to encoder memory
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat22WriteMem(EIB7_AXIS axis, unsigned short* status,
			     ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
			     unsigned char addr, unsigned short data);


/*! Returns the current position of the EnDat 2.2 encoder and raed from selected memory area.
  \param axis handle for the axis
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \param addr address for the encoder memory
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat22ReadMem(EIB7_AXIS axis, unsigned short* status,
			    ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
			    unsigned char addr);


/*! Returns the current position of the EnDat 2.2 encoder and write test data
  \param axis handle for the axis
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \param port port address for encoder
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat22WriteTestCommand(EIB7_AXIS axis, unsigned short* status,
				     ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				     unsigned char port);


/* Specifies the EnDat 2.2 Additional Information */
typedef enum
{
  EIB7_AI1_NOP                   = 0x00,      /*!< */ 
  EIB7_AI1_Diagnostic            = 0x01,      /*!< */ 
  EIB7_AI1_Position2_word1       = 0x02,      /*!< */ 
  EIB7_AI1_Position2_word2       = 0x03,      /*!< */ 
  EIB7_AI1_Position2_word3       = 0x04,      /*!< */ 
  EIB7_AI1_MemoryLSB             = 0x05,      /*!< */ 
  EIB7_AI1_MemoryMSB             = 0x06,      /*!< */ 
  EIB7_AI1_MRS                   = 0x07,      /*!< */ 
  EIB7_AI1_TestCommand           = 0x08,      /*!< */ 
  EIB7_AI1_TestValue_word1       = 0x09,      /*!< */ 
  EIB7_AI1_TestValue_word2       = 0x0A,      /*!< */ 
  EIB7_AI1_TestValue_word3       = 0x0B,      /*!< */ 
  EIB7_AI1_Temperature1          = 0x0C,      /*!< */ 
  EIB7_AI1_Temperature2          = 0x0D,      /*!< */ 
  EIB7_AI1_AddSensor             = 0x0E,      /*!< */ 
  EIB7_AI1_Stop                  = 0x0F,      /*!< */ 
  EIB7_AI2_NOP                   = 0x10,      /*!< */ 
  EIB7_AI2_Commutation           = 0x11,      /*!< */ 
  EIB7_AI2_Acceleration          = 0x12,      /*!< */ 
  EIB7_AI2_CommAndAccel          = 0x13,      /*!< */ 
  EIB7_AI2_LimitSignal           = 0x14,      /*!< */ 
  EIB7_AI2_LimitAndAccel         = 0x15,      /*!< */ 
  EIB7_AI2_AsyncPos_word1        = 0x16,      /*!< */ 
  EIB7_AI2_AsyncPos_word2        = 0x17,      /*!< */ 
  EIB7_AI2_AsyncPos_word3        = 0x18,      /*!< */ 
  EIB7_AI2_OPSErrorSources       = 0x19,      /*!< */ 
  EIB7_AI2_ReservedA             = 0x1A,      /*!< */
  EIB7_AI2_ReservedB             = 0x1B,      /*!< */
  EIB7_AI2_ReservedC             = 0x1C,      /*!< */
  EIB7_AI2_ReservedD             = 0x1D,      /*!< */
  EIB7_AI2_ReservedE             = 0x1E,      /*!< */
  EIB7_AI2_Stop                  = 0x1F,      /*!< */ 
} EIB7_AdditionalInformation;


#define EIB7_AI_MINVAL 0x00
#define EIB7_AI_MAXVAL 0x1F

/*! Configures the additional information for an EnDat 2.2 encoder
  \param axis handle for the axis
  \param addinfo1 number for the additional information 1
  \param addinfo2 number for the additional information 2
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_InvInterface    axis not configured for EnDat 2.2
  - EIB7_ParamInvalid    parameter not valid (configuration data too long)
  - EIB7_NotInitialized  not initialised 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat22SetAddInfo(EIB7_AXIS axis, unsigned long addinfo1, unsigned long addinfo2);


/*! Configures the FIFO for the additional information for an EnDat 2.2 encoder
  \param axis handle for the axis
  \param mode enable or disable the FIFO
  \param data additional information configuration data
  \param len length of configuration data in bytes
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_InvInterface    axis not configured for EnDat 2.2
  - EIB7_ParamInvalid    parameter not valid (configuration data too long)
  - EIB7_NotInitialized  not initialised 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat22SetAddInfoCycle(EIB7_AXIS axis, EIB7_MODE mode, unsigned char* data, unsigned long len);

/* Specifies the EnDat 2.2 Additional Information */
typedef enum
{
  EIB7_POS_AI              = 0x01,      /*!< */ 
  EIB7_POS_AI_MRS          = 0x02,      /*!< */ 
  EIB7_POS_AI_READ         = 0x03,      /*!< */ 
  EIB7_POS_AI_WRITE        = 0x04,      /*!< */ 
  EIB7_POS_AI_TEST         = 0x05,      /*!< */ 
  EIB7_POS_AI_RESET        = 0x06       /*!< */ 
} EIB7_EnDatCmdMode;


typedef struct
{
   EIB7_EnDatCmdMode mode;
   unsigned char addr;
   unsigned short data;
} EIB7_EnDatCmd;


/*! Configures the FIFO for EnDat 2.2 communication for an EnDat 2.2 encoder
  \param axis handle for the axis
  \param mode enable or disable the FIFO
  \param cfg configuration data for EnDat commands
  \param len length of configuration data in number of FIFO entries
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_InvInterface    axis not configured for EnDat 2.2
  - EIB7_ParamInvalid    parameter not valid (configuration data too long)
  - EIB7_NotInitialized  not initialised 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat22SetCmdCycle(EIB7_AXIS axis, EIB7_MODE mode, EIB7_EnDatCmd* cfg, unsigned long len);


/*! Returns the current position of the EnDat 2.2 encoder and reset the encoder errors
  \param axis handle for the axis
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_InvEnDatCmdCode Invalid command code for the EnDat command interface 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7EnDat22ErrorReset(EIB7_AXIS axis, unsigned short* status,
			       ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2);

/*! Returns the current absolute and incremental positions simultaneously.
  \param axis handle for the axis
  \param statusEnDat EnDat position status word
  \param posEnDat EnDat position value
  \param statusIncr incremental position status word
  \param posIncr incremental position value
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EncPwrSuppErr   encoder power supply error 
  - EIB7_CantLatchPos    can not latch position
  - EIB7_EnDatErrII      EnDat error type II 
  - EIB7_EnDatIfBusy     EnDat interface busy 
  - EIB7_EnDatXmitErr    EnDat transmission error (encoder may be not connected) 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIB7ReadEnDatIncrPos(EIB7_AXIS axis,
			      unsigned short* statusEnDat, ENCODER_POSITION* posEnDat,
			      unsigned short* statusIncr,  ENCODER_POSITION* posIncr);

/*! Enable or disable encoder power supply
  \param axis handle for the axis
  \param mode an EIB7_MODE to enable or disable the power supply
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_InvChannel      invalid channel 
  - EIB7_ParamInvalid    parameter not valid 
*/
EIB7_ERR EIB7SetPowerSupply(EIB7_AXIS axis, EIB7_MODE mode);

/*! The possible power supply failure modes reported by EIB7GetPowerStatus()
*/
typedef enum {
  EIB7_PF_None = 0,        /*!< */
  EIB7_PF_Overcurrent = 1  /*!< */
}EIB7_POWER_FAILURE;


/*! Get the current power supply status for the selected encoder
  \param axis handle for the axis
  \param power supply state
  \param err over current failure 
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7GetPowerSupplyStatus(EIB7_AXIS axis, EIB7_MODE* power, EIB7_POWER_FAILURE* err);

/*! Enable or disable timestamps for the position values
  \param axis handle for the axis
  \param mode an EIB7_MODE to enable or disable the timestamp
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7SetTimestamp(EIB7_AXIS axis, EIB7_MODE mode);

/*! The mode for the IO channel */
typedef enum
  {
    EIB7_IOM_Trigger = 0, /*!< */
    EIB7_IOM_Logical = 1  /*!< */
  }EIB7_IOMODE;

#define EIB7_IOM_MINVAL 0
#define EIB7_IOM_MAXVAL 1

/*! configure an I/O input channel 
  \param io handle for the I/O port
  \param mode operation mode
  \param termination termination resistor
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid    parameter not valid 
*/
  
EIB7_ERR EIB7InitInput(EIB7_IO io, EIB7_IOMODE mode, EIB7_MODE termination);

/*! configure an I/O output channel 
  \param io handle for the I/O port
  \param mode operation mode
  \param enable output driver
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid    parameter not valid 
*/
EIB7_ERR EIB7InitOutput(EIB7_IO io, EIB7_IOMODE mode, EIB7_MODE enable);

/*! Read the current status of an I/O channel
  \param io handle for the I/O port
  \param mode pointer to a variable which receives the operation mode
  \param level pointer to a variable which receives the logic level
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid    parameter not valid 
*/
  
EIB7_ERR EIB7ReadIO(EIB7_IO io, EIB7_IOMODE* mode, unsigned long* level);

/*! Set the logical level for an output I/O channel
  \param io handle for the I/O port
  \param level logical output level
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid    parameter not valid 
*/

EIB7_ERR EIB7WriteIO(EIB7_IO io, unsigned long level);

/*! Get the configuration for an I/O input channel
  \param io handle for the I/O port
  \param mode operation mode
  \param termination status of the termination resistor
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid    parameter not valid 
*/

EIB7_ERR EIB7GetInputConfig(EIB7_IO io, EIB7_IOMODE *mode, EIB7_MODE *termination);

/*! Get the configuration for an I/O output channel
  \param io handle for the I/O port
  \param mode operation mode
  \param enable status of the output driver
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_ParamInvalid    parameter not valid 
*/

EIB7_ERR EIB7GetOutputConfig(EIB7_IO io, EIB7_IOMODE *mode, EIB7_MODE *enable);

/*! Reports the driver identification string
  \param ident pointer to a memory area that receives the identification string
  \param len size of the memory area in bytes
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_BufferTooSmall if the string does not fit in the buffer
*/
EIB7_ERR EIB7GetDriverID(char* ident, unsigned long len);

/*! converts an incremental position value to floating point
  \param src position value from incremental encoder (integer)
  \param dest converted value (double), 1.0 = one signal period
  \return possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_ParamInvalid parameter not valid
*/
EIB7_ERR EIB7IncrPosToDouble(ENCODER_POSITION src, double* dest);


/*! Clears the amplitude error flag and the noise error flag
    for the auxiliary axis.
  \param eib the handle for the EIB
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7AuxClearSignalErrors(EIB7_HANDLE eib);


/*! Clears the trigger lost error flag for the auxiliary axis.
  \param eib the handle for the EIB
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7AuxClearLostTriggerError(EIB7_HANDLE eib);


/*! Resets the counter for the auxiliary axis.
  \param eib the handle for the EIB
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7AuxClearCounter(EIB7_HANDLE eib);


/*! Clears the reference position status for the auxiliary axis.
  \param eib the handle for the EIB
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7AuxClearRefStatus(EIB7_HANDLE eib);


/*! Returns the status for the reference position latch procedure
    of the auxiliary axis.
  \param eib the handle for the EIB
  \param active pointer to a variable that receives the active status
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7AuxGetRefActive(EIB7_HANDLE eib, EIB7_MODE* active);


/*! The reference position latch operation will be started
    for the auxiliary axis.
  \param eib the handle for the EIB
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7AuxStartRef(EIB7_HANDLE eib);


/*! The reference position latch operation will be aborted
    for the auxiliary axis.
  \param eib the handle for the EIB
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_OutOfMemory if not enough memory was available
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7AuxStopRef(EIB7_HANDLE eib);


/*! Returns the current position of the encoder of the auxiliary axis. 
    Returns an error if the EIB is not in polling mode.
  \param eib the handle for the EIB
  \param status status word
  \param pos position value
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_AccNotAllowed if the EIB is not in polling mode
  - EIB7_EncPwrSuppErr if the power supply of the encoder is down
  - EIB7_NotInitialized if the axis has not been initialized
*/
EIB7_ERR EIB7AuxGetPosition(EIB7_HANDLE eib, unsigned short *status, ENCODER_POSITION *pos);


/*! Read the current position value and status information from the auxiliary axis.
  The position will be latched. The EIB must be in the polling mode.
  \param eib the handle for the EIB
  \param status pointer to a variable which receives the status word.
  \param pos pointer to a variable which receives the position value
  \param ref pointer to a variable which receives the reference position
  \param timestamp pointer to a variable which receives the timestamp for the position value
  \param counter pointer to a variable which receives the trigger counter
  \return Some possible return codes are: 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
  - EIB7_AccNotAllowed if the EIB is not in polling mode
  - EIB7_EncPwrSuppErr if the power supply of the encoder is down
  - EIB7_NotInitialized if the axis has not been initialized
*/  
EIB7_ERR EIB7AuxGetEncoderData(EIB7_HANDLE eib,
			    unsigned short* status,
			    ENCODER_POSITION* pos,
			    ENCODER_POSITION* ref,
			    unsigned long* timestamp,
			    unsigned short* counter);


/*! Enable or disable timestamps for the auxiliary axis
  \param eib the handle for the EIB
  \param mode an EIB7_MODE to enable or disable the timestamp
  \return Some possible return codes are:  
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_ConnTimeout if the command did not complete in time
*/
EIB7_ERR EIB7AuxSetTimestamp(EIB7_HANDLE eib, EIB7_MODE mode);



#ifdef __cplusplus
}
#endif

#endif /*__EIB7__H__*/

