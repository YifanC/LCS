#ifndef __EIB7_CMD_H__
#define __EIB7_CMD_H__

/*!
  \file eib7_cmd.h
  \brief This file defines the command helper routines for the EIB 7 driver DLL.

  This file defines the command helper routines for the EIB 7 driver DLL.
*/

#include "eib7.h"
#include "eib7_types.h"
#include "definitions.h"
#include "cmdtcp.h"
#include "threading.h"
#include "commands.h"

/*! Requests the master interface for this connection.
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the master interface could be acquired successfully
*/

EIB7_ERR EIBCmd_RequestMaster(EIB_Data *ed);

/*! Requests the master interface for this connection.
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the master interface could be acquired successfully
*/

EIB7_ERR EIBCmd_ReleaseMaster(EIB_Data *ed);

/*! Changes the operating mode of the EIB 
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode 0 = Polling, 1 = Soft RT
  \return EIB7_NoError if the mode could be set correctly
*/

EIB7_ERR EIBCmd_EnDisSoftRT(EIB_Data *ed, int mode);

/*! Changes the operating mode of the EIB $*depricated*$  !!!!
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode 0 = Polling, 1 = Soft RT, 2 = Streaming, 3 = Recording Single, 4 = Recording Rolling
  \return EIB7_NoError if the mode could be set correctly
*/
EIB7_ERR EIBCmd_SelectMode(EIB_Data *ed, int mode);

/*! Sends configuration for the data packet to the EIB
  \param ed the pointer to the EIB_Data structure for this connection
  \param data pointer to array with configuration data
  \length length of data in bytes
  \return EIB7_NoError if the mode could be set correctly
*/

EIB7_ERR EIBCmd_ConfigDataPacket(EIB_Data* ed, unsigned char* data, unsigned long length);


/*! Reads the size of the memory for recording data
  \param ed the pointer to the EIB_Data structure for this connection
  \param size pointer to variable for the memory size in bytes
  \return EIB7_NoError if the cmd ends successful
*/

EIB7_ERR EIBCmd_GetRecMemSize(EIB_Data* ed, unsigned long* size);


/*! Requests the EIB to close it's side of the link
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the cmd ends successful
*/

EIB7_ERR EIBCmd_CloseLink(EIB_Data *ed);

/*! Sets the IP address and port for the soft realtime mode
  \param ed the pointer to the EIB_Data structure for this connection
  \param ip network address for the UDP destination
  \param port destination UDP port number
  \return EIB7_NoError if the address could be set correctly
*/

EIB7_ERR EIBCmd_SetIPForSoftRT(EIB_Data *ed, unsigned long ip, unsigned long port);

/*! Set the custom network parameters
  \param ed the pointer to the EIB_Data structure for this connection
  \param ip the destination ip address
  \param netmask the netmask for the EIB network interface
  \param gateway the ip address of the default gateway for the eib
  \param dhcp this flag specifies, wether DHCP should be turned on in the EIB (0 = DHCP off, 1 = DHCP on)
  \return EIB7_NoError if the function executes correctly
*/
EIB7_ERR EIBCmd_SetCustomNetwork(EIB_Data *ed, unsigned long ip, unsigned long netmask,
				 unsigned long gateway, int dhcp);

/*! Set the DHCP timeout
  \param ed the pointer to the EIB_Data structure for this connection
  \param timeout the timeout for the DHCP client in seconds
  \return EIB7_NoError if the function executes correctly
*/
EIB7_ERR EIBCmd_SetDHCPTimeout(EIB_Data *ed, unsigned long timeout);

/*! Get the custom network parameters
  \param ed the pointer to the EIB_Data structure for this connection
  \param ip pointer to the destination ip address
  \param netmask pointer to the netmask for the EIB network interface
  \param gateway pointer to the ip address of the default gateway for the eib
  \param dhcp pointer to the DHCP flag (0 = DHCP off, 1 = DHCP on)
  \return EIB7_NoError if the function executes correctly
*/
EIB7_ERR EIBCmd_GetCustomNetwork(EIB_Data *ed, unsigned long *ip, unsigned long *netmask,
				 unsigned long *gateway, int *dhcp);

/*! Get the hostname of the eib
  \param ed the pointer to the EIB_Data structure for this connection
  \param hostname pointer to a string receiving the hostname
  \param len size of the string
  \return EIB7_NoError if t hostname could be retrieved
*/
EIB7_ERR EIBCmd_GetHostname(EIB_Data *ed, char *hostname, unsigned long len);

/*! Set the hostname of the eib
  \param ed the pointer to the EIB_Data structure for this connection
  \param hostname pointer to a string
  \return EIB7_NoError if t hostname could be retrieved
*/
EIB7_ERR EIBCmd_SetHostname(EIB_Data *ed, const char *hostname);

/*! Restore the default hostname of the eib
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if t hostname could be retrieved
*/
EIB7_ERR EIBCmd_SetDefaultHostname(EIB_Data *ed);

/*! Get the serial numbers from the eib
  \param ed the pointer to the EIB_Data structure for this connection
  \param buffer the destination memory for the call result. The buffer should be large enough.
  \param size the size of the destination memory area
  \return EIB7_NoError if t hostname could be retrieved
*/
EIB7_ERR EIBCmd_GetSerialNumber(EIB_Data *ed, char *buffer, int size);

/*! Get the MAC address from the eib
  \param ed the pointer to the EIB_Data structure for this connection
  \param mac the destination memory for the call result. The buffer must be large enough.
  \return EIB7_NoError if t hostname could be retrieved
*/
EIB7_ERR EIBCmd_GetMAC(EIB_Data *ed, unsigned char *mac);

/*! Get the version of the specified firmware
  \param ed the pointer to the EIB_Data structure for this connection
  \param firmware 0 = factory firmware, 1 = user firmware
  \param version pointer to an unsigned long which receives the version number
  \return EIB7_NoError if the version could be retrieved
*/
EIB7_ERR EIBCmd_GetVersion(EIB_Data *ed, int firmware, unsigned long *version);

/*! Get the interface version of the currently booted firmware
  \param ed the pointer to the EIB_Data structure for this connection
  \param version pointer to an unsigned long which receives the version number
  \return EIB7_NoError if the version could be retrieved
*/
EIB7_ERR EIBCmd_GetInterfaceVersion(EIB_Data *ed, unsigned long *version);

/*! Get the current boot mode
  \param ed the pointer to the EIB_Data structure for this connection
  \param bootmode 0 = User Firmware/User Settings, 1 = Factory Firmware/User Settings, 2 = Factory Firmware/Default Settings
  \return EIB7_NoError if the version could be retrieved
*/
EIB7_ERR EIBCmd_GetBootMode(EIB_Data *ed, int *bootmode);

/*! Get the firmware update error
  \param ed the pointer to the EIB_Data structure for this connection
  \param error pointer to an unsigned long which receives the error status
  \return EIB7_NoError if the version could be retrieved
*/

EIB7_ERR EIBCmd_GetUpdateError(EIB_Data *ed, unsigned long *error);

/*! Get information on all currently open connections
  \param ed the pointer to the EIB_Data structure for this connection
  \param info pointer to the first element of a set of type EIB7_CONN_INFO[ ].
  \param size maximum number of entries in the set
  \param cnt pointer to an variable that receives the number of open connections
  \return EIB7_NoError if the information could be retrieved
*/
  
EIB7_ERR EIBCmd_GetConnectionInfo(EIB_Data *ed, EIB7_CONN_INFO *info,
				  unsigned long size, unsigned long *cnt);

/*! Request the EIB to terminate an open TCP connection with a TCP reset. The
   EIB does not close the connection which is used to send this command.
  \param ed the pointer to the EIB_Data structure for this connection
  \param id ID of the open connection
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_TerminateConnection(EIB_Data *ed, unsigned long id);

/* Enable or disable killing other connections in the EIB
  \param ed the pointer to the EIB_Data structure for this connection
  \param en 1: enable termination of other connections, 0: disable
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_EnableDisableKill(EIB_Data *ed, unsigned long en);

/* Get the clock ticks per microsecond of the Timestamp timer.
  \param ed the pointer to the EIB_Data structure for this connection
  \param ticks pointer to a variable that receives the clock ticks per microsecond
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_GetTimestampTicks(EIB_Data *ed, unsigned long * ticks);

/* Set the Timestamp period in clock ticks.
  \param ed the pointer to the EIB_Data structure for this connection
  \param period period of the Timestamp timer in clock ticks,. This value must be > 0.
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_SetTimestampPeriod(EIB_Data *ed, unsigned long period);

/* Reset the Timestamp counter to zero
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_ResetTimestamp(EIB_Data *ed);

/* Get the clock ticks per microsecond of the Timestamp timer.
  \param ed the pointer to the EIB_Data structure for this connection
  \param ticks pointer to a variable that receives the clock ticks per microsecond
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_GetTimerTriggerTicks(EIB_Data *ed, unsigned long * ticks);

/* Set the Timer Trigger period in clock ticks.
  \param ed the pointer to the EIB_Data structure for this connection
  \param period period of the Timer Trigger timer in clock ticks,. This value must be > 0.
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_SetTimerTriggerPeriod(EIB_Data *ed, unsigned long period);

/* Set interval counter trigger
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode operation mode
  \param start at current position or at start position
  \startpos start position
  \param peroid period for periodic trigger
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetICTrigger(EIB_Data *ed, unsigned long mode, unsigned long start,
                                                            unsigned long startpos, unsigned long period);

/* Set interval counter trigger
  \param ed the pointer to the EIB_Data structure for this connection
  \param ipf interpolation factor
  \param edge edge resolution
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetICInterpolation(EIB_Data *ed, unsigned long ipf, unsigned long edge);


/* Set output trigger source
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel trigger output channel
  \param src trigger source
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetOutputTriggerSrc(EIB_Data *ed, unsigned long channel, unsigned long src);


/* Set axis trigger source
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel axis number
  \param src trigger source
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetAxisTriggerSrc(EIB_Data *ed, unsigned long channel, unsigned long src);


/* Set auxiliary axis trigger source
  \param ed the pointer to the EIB_Data structure for this connection
  \param src trigger source
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetAuxAxisTriggerSrc(EIB_Data *ed, unsigned long src);


/* Set master trigger source
  \param ed the pointer to the EIB_Data structure for this connection
  \param src trigger source
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetMasterTriggerSrc(EIB_Data *ed, unsigned long src);

/* Set trigger source
  \param ed the pointer to the EIB_Data structure for this connection
  \param enable trigger sources to be enabled
  \param disable trigger sources to be disabled
  \param actual actually enabled trigger sources
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetTriggerSource(EIB_Data *ed, unsigned long enable, unsigned long disable, unsigned long* actual);

/* Configure the puls counter
  \param ed the pointer to the EIB_Data structure for this connection
  \param start start signal for the puls counter
  \param trigger trigger signal for the puls counter
  \param count number of pulses
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_ConfigPulsCounter(EIB_Data *ed, unsigned long start, unsigned long trigger,
                                  unsigned long count);


/* Get the number of clock ticks per microsecond for the trigger input delay
  \param ed the pointer to the EIB_Data structure for this connection
  \param ticks pointer to variable for the clock ticks per microsecond
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_GetTriggerDelayTicks(EIB_Data *ed, unsigned long* ticks);

/* Set delay for trigger input
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel trigger input channel
  \param dly trigger input delay
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetTriggerInDelay(EIB_Data *ed, unsigned long channel, unsigned long dly);


/* Set active edge of reference signal for triggering
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel axis number
  \param edge trigger edge
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_SetRITriggerEdge(EIB_Data *ed, unsigned long channel, unsigned long edge);








/*! Enable or disable the timer trigger. This call generates an error, if the external
  trigger is already activated
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode 1 = enabled, 0 = disabled
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_EnableTimerTrigger(EIB_Data *ed, unsigned long mode);

/* Reset the Timer counter to zero
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_ResetTriggerCounter(EIB_Data *ed);

/*! Enable or disable incremantal encoder input termination.
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode 1 = enabled, 0 = disabled
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_EnableIncrementalTermination(EIB_Data *ed, unsigned long mode);

/*! Enable or disable the external trigger. This call generates an error, if the external
  trigger is already activated
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode 1 = enabled, 0 = disabled
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_EnableExternalTrigger(EIB_Data *ed, unsigned long mode);

/*! Generates a software trigger
  \param ed the pointer to the EIB_Data structure for this connection
  \param source trigger source (always 0)
*/
EIB7_ERR EIBCmd_SoftwareTrigger(EIB_Data *ed, unsigned long source);

/*! Reset the EIB. After this call, all connections will be closed.
  \param ed the pointer to the EIB_Data structure for this connection
*/
EIB7_ERR EIBCmd_Reset(EIB_Data *ed);

/*! Enable or disable blinking mode of the LED on the front panel
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode 1 = enabled, 0 = disabled
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_Identify(EIB_Data *ed, unsigned long mode);


/*! reads the number of axes in the EIB
  \param ed the pointer to the EIB_Data structure for this connection
  \param dsub number of axes with D-Sub connectors
  \param m12 number of axes with M12 connectors
  \param res2 reserved
  \param res3 reserved
  \return EIB7_NoError if the call returns successfully
*/

EIB7_ERR EIBCmd_GetNumOfAxes(EIB_Data* ed, unsigned long* dsub, unsigned long* m12, 
                             unsigned long* res2, unsigned long* res3);


/*! Initializes an axis to the specified encoder settings.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param flags the setting flags for the encoder
  \param linecounts counts per revolution
  \param increment nominal increment between two fixed reference marks
  \param EnDatclock EnDat clock in Hz (#EIB7_InterfaceClock)
  \return EIB7_NoError if the axis could be configured successfully
*/
EIB7_ERR EIBCmd_InitAxis(EIB_Data *ed,
			 unsigned long channel,
			 unsigned long flags,
			 unsigned long linecounts,
			 unsigned long increment,
			 unsigned long EnDatclock);

/*! Resets the incremental counter for this axis. Generates an error if the
  axis is not configured for incremental encoders.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \return EIB7_NoError if the axis could be reset
*/
EIB7_ERR EIBCmd_ClearCounter(EIB_Data *ed, unsigned long channel);

/*! Returns the current position of the encoder. Returns an error if the EIB is not in polling mode.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \return EIB7_NoError if the axis could be reset
*/
EIB7_ERR EIBCmd_GetPosition(EIB_Data *ed, unsigned long channel, unsigned short *status, ENCODER_POSITION *pos);

/*! Read the current position value and status information from the specified encoder.
  The position will be latched. The EIB must not be in the soft-realtime mode.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status pointer to a variable which receives the status word.
  \param pos pointer to a variable which receives the position value
  \param ref1 pointer to a variable which receives the reference position 1
  \param ref2 pointer to a variable which receives the reference position 2
  \param refc pointer to a variable which receives the distance coded reference position
  \param timestamp pointer to a variable which receives the timestamp for the position value
  \param counter pointer to a variable which receives the trigger counter
  \param adc00 pointer to a variable which receives the analog value for signal A
  \param adc90 pointer to a variable which receives the analog value for signal B
  \return
  - EIB7_NoError if the command executed without error
  - EIB7_ParamInvalid ??
  - EIB7_TimerTrgEn Timer Trigger is enabled
  - EIB7_ExtTrgEn External Trigger is enabled
  - EIB7_SoftRTEn Soft Realtime Mode is enabled
*/  

EIB7_ERR EIBCmd_GetEncoderData(EIB_Data *ed, unsigned long channel,
			       unsigned short* status,
			       ENCODER_POSITION* pos,
			       ENCODER_POSITION* ref1,
			       ENCODER_POSITION* ref2,
			       ENCODER_POSITION* refc,
			       unsigned long* timestamp,
			       unsigned short* counter,
			       unsigned short* adc00,
			       unsigned short* adc90);


/*! Clears the specified encoder errors for the channel.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mask the error mask
  \return
  - EIB7_NoError if the command executed without error
*/

EIB7_ERR EIBCmd_ClearEnDatEncoderErrors(EIB_Data *ed, unsigned long channel, unsigned long mask);

/*! Clears the specified encoder errors for the channel.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mask the error mask
  \return
  - EIB7_NoError if the command executed without error
*/

EIB7_ERR EIBCmd_ClearIncrementalEncoderErrors(EIB_Data *ed, unsigned long channel, unsigned long mask);

/*! Clears the specified reference position status flags for the channel.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mask the error mask
  \return
  - EIB7_NoError if the command executed without error
*/

EIB7_ERR EIBCmd_ClearRefLatchStatus(EIB_Data *ed, unsigned long channel, unsigned long mask);


/*! Enables the reference position latch procedure for the given channel and reference position. 
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param ref the indication, which reference position to latch
  \return
  - EIB7_NoError if the command executed without error
*/
EIB7_ERR EIBCmd_EnableRefPosLatch(EIB_Data *ed, unsigned long channel, unsigned long ref);

/*! Disables the reference position latch procedure for the given channel. 
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \return
  - EIB7_NoError if the command executed without error
*/
EIB7_ERR EIBCmd_DisableRefPosLatch(EIB_Data *ed, unsigned long channel);

/*! Returns the status of the reference position latch procedure for the given channel. 
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param active the status of the procedure (0 = inactive)
  \return
  - EIB7_NoError if the command executed without error
*/
EIB7_ERR EIBCmd_GetRefPosLatchActive(EIB_Data *ed, unsigned long channel, unsigned long *active);

/*! The reference mark check can be configured.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mode enable or disable
  \param limit limit for the reference mark check
  \return
  - EIB7_NoError if the command executed without error
*/
EIB7_ERR EIBCmd_SetReferenceCheck(EIB_Data *ed, unsigned long channel, unsigned long mode, unsigned long limit);


/*! Returns the current position of the encoder. Returns an error if the EIB is not in polling mode.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \return
  - EIB7_NoError if the command executed without error
  - 
*/
EIB7_ERR EIBCmd_EnDat21GetPosition(EIB_Data *ed, unsigned long channel, unsigned short *status, ENCODER_POSITION *pos);

/*! Selects a memory range for the encoder.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mrs MRS code for the encoder
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat21SelectMemRange(EIB_Data *ed, unsigned long channel, unsigned char mrs);

/*! Writes data to a specified encoder address
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param addr address for encoder memory
  \param data data programmed to encoder memory
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/

EIB7_ERR EIBCmd_EnDat21WriteMem(EIB_Data *ed, unsigned long channel, unsigned long addr,  unsigned long data);

/*! Reads data from a specified encoder address
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param addr address for encoder memory
  \param data data programmed to encoder memory
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat21ReadMem(EIB_Data *ed, unsigned long channel, unsigned long addr,  unsigned short *data);

/*! Reads data from a specified encoder address
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat21ResetEncoder(EIB_Data *ed, unsigned long channel);

/*! Read test values from the encoder
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param high most significant word of the test value (only bits D0..D7 valid)
  \param low least significant word of the test value
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat21ReadTestValue(EIB_Data *ed, unsigned long channel, unsigned long *high, unsigned long *low);

/*! Write test values to the encoder
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param port port address for the encoder
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat21WriteTestCommand(EIB_Data *ed, unsigned long channel, unsigned long port);

/*! Returns the current position of the EnDat 2.2 encoder plus additional infos. 
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat22GetPosition(EIB_Data *ed, unsigned long channel, unsigned short *status,
				   ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2);

/*! Returns the current position of the EnDat 2.2 encoder and selects a memory range. 
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \param mrs MRS code for the encoder
  \param block block address for the encoder
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat22SelectMemRange(EIB_Data *ed, unsigned long channel, unsigned short* status,
				      ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				      unsigned char mrs, unsigned char block);

/*! Returns the current position of the EnDat 2.2 encoder and read from selected memory area.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \param addr address for the encoder memory
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat22ReadMem(EIB_Data *ed, unsigned long channel, unsigned short* status,
				ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				unsigned char addr);

/*! Returns the current position of the EnDat 2.2 encoder and write to selected memory area.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \param addr address for the encoder memory
  \param data data programmed to encoder memory
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat22WriteMem(EIB_Data *ed, unsigned long channel, unsigned short* status,
				ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				unsigned char addr, unsigned short data);

/*! Returns the current position of the EnDat 2.2 encoder and raed from selected memory area.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \param port port address for encoder
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat22WriteTestCommand(EIB_Data *ed, unsigned long channel, unsigned short* status,
					ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
					unsigned char port);


/*! Configures the additional information for an EnDat 2.2 encoder
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param addinfo1 number for the additional information 1
  \param addinfo2 number for the additional information 2
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_InvInterface    axis not configured for EnDat 2.2
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIBCmd_EnDat22SetAddInfo(EIB_Data *ed, unsigned long channel, 
                                                unsigned long addinfo1, unsigned long addinfo2);

/*! Configures the FIFO for the additional information for an EnDat 2.2 encoder
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mode enable or disable the FIFO
  \param data pointer to the data array for the additional information configuration
  \param len length of the additional information configuration in bytes
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_InvInterface    axis not configured for EnDat 2.2
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIBCmd_EnDat22SetAddInfoFIFO(EIB_Data *ed, unsigned long channel, unsigned long mode,
                                                         unsigned char* data, unsigned long len);

/*! Configures the FIFO for EnDat 2.2 commands for an EnDat 2.2 encoder
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mode enable or disable the FIFO
  \param data pointer to the data array for the configuration for EnDat commands
  \param len length of the additional information configuration in bytes
  \praam entries length of the additional information configuration in FIFO entries
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - EIB7_InvInterface    axis not configured for EnDat 2.2
  - EIB7_ParamInvalid    parameter not valid 
  - EIB7_NotInitialized  not initialised 
  - EIB7_EnDat22NotSupp  EnDat 2.2 is not supported by this encoder 
  - EIB7_FuncNotSupp     function not supported 
*/
EIB7_ERR EIBCmd_EnDat22SetCmdFIFO(EIB_Data *ed, unsigned long channel, unsigned long mode,
                                  unsigned char* data, unsigned long len, unsigned long entries);


/*! Returns the current position of the EnDat 2.2 encoder and reset the encoder errors
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \param ai1 additional information 1 with status word
  \param ai2 additional information 2 with status word
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_EnDat22ErrorReset(EIB_Data *ed, unsigned long channel, unsigned short* status,
				  ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2);

/*! Returns the current absolute and incremental positions simultaneously.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param statusEnDat EnDat position status word
  \param posEnDat EnDat position value
  \param statusIncr incremental position status word
  \param posIncr incremental position value
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the axis handle is not valid
  - 
*/
EIB7_ERR EIBCmd_ReadEnDatIncrPos(EIB_Data *ed, unsigned long channel,
				 unsigned short* statusEnDat, ENCODER_POSITION* posEnDat,
				 unsigned short* statusIncr,  ENCODER_POSITION* posIncr);

/*! Enable or disable encoder power supply
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mode 1 = enabled, 0 = disabled
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_SetPowerSupply(EIB_Data *ed, unsigned long channel, unsigned long mode);

/*! Get the current power supply status for the selected encoder
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param power supply state
  \param err over current failure
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_GetPowerSupplyStatus(EIB_Data *ed, unsigned long channel, EIB7_MODE* power, EIB7_POWER_FAILURE* err);

/*! Enable or disable timestamps for the position values
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param mode an EIB7_MODE to enable or disable the timestamp
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_SetTimestamp(EIB_Data *ed, unsigned long channel, EIB7_MODE mode);

/*! configure an I/O input channel 
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the I/O channel (0-based)
  \param mode operation mode
  \param termination termination resistor
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_InitInput(EIB_Data *ed, unsigned long channel, unsigned long mode, unsigned long termination);

/*! configure an I/O output channel 
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the I/O channel (0-based)
  \param mode operation mode
  \param enable output driver (0 = disabled, 1 = enabled)
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_InitOutput(EIB_Data *ed, unsigned long channel, unsigned long mode, unsigned long enable);

/*! Read the current status of an input channel
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the I/O channel (0-based)
  \param mode pointer to a variable which receives the operation mode
  \param level pointer to a variable which receives the logic level
  \param term pointer to a variable which receives the termination status
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
  
EIB7_ERR EIBCmd_ReadInputLevel(EIB_Data *ed, unsigned long channel,
			       unsigned long* mode, unsigned long* level, unsigned long *term);

/*! Read the current status of an output channel
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the I/O channel (0-based)
  \param mode pointer to a variable which receives the operation mode
  \param level pointer to a variable which receives the logic level
  \param enable pointer to a variable which receives the enable status
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_ReadOutputLevel(EIB_Data *ed, unsigned long channel,
				unsigned long* mode, unsigned long* level, unsigned long *enable);


/*! Start transfer of recording data
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode start or stop the data transfer
  \param offset the offset for the first byte to transfer (in bytes)
  \param length length of the transmitted data (in bytes) (0xFFFFFFFF = all data)
  \param ip pointer to a variable which receives the ip address of the EIB
  \param port pointer to a variable which receives the TCP port for data transfer
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
  - EIB7_RecDataReadErr if the recording mode is still active
  - EIB7_ParamInvalid if the offset is out of range
*/
EIB7_ERR EIBCmd_GetRecordingData(EIB_Data *ed, EIB7_MODE mode, unsigned long offset, unsigned long length,
                                 unsigned long* ip, unsigned long* port);


/*! Get status for recording fifo
  \param ed the pointer to the EIB_Data structure for this connection
  \param length pointer to the variable for the number of bytes in the fifo
  \param status pointer to the variable for the status
  \param progress pointer to the variable for the progress of the data transfer
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_GetRecordingStatus(EIB_Data* ed, unsigned long* length, unsigned long* status, unsigned long* progress);


/*! Get status for streaming fifo
  \param ed the pointer to the EIB_Data structure for this connection
  \param length pointer to the variable for the number of bytes in the fifo
  \param max pointer to the variable for the maximum number of bytes in the fifo since start of streaming mode
  \param size pointer to the variable for the size of the fifo in bytes
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_GetStreamingStatus(EIB_Data* ed, unsigned long* length, unsigned long* status, unsigned long* progress);


/*! Set the logical level for an output I/O channel
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the I/O channel (0-based)
  \param level logical output level
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/

EIB7_ERR EIBCmd_WriteIO(EIB_Data *ed, unsigned long channel, unsigned long level);


/* Clear amplitude error for auxiliary axis
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_AuxClearAmpError(EIB_Data *ed);


/* Clear lost trigger error for auxiliary axis
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_AuxClearLostTriggerError(EIB_Data *ed);


/* Clear the counter of the auxiliary axis
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_AuxClearCounter(EIB_Data *ed);


/* Clears the status for reference position latched for the auxiliary axis
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_AuxClearRefStatus(EIB_Data *ed);


/* Returns the status for the reference position latch procedure
   of the auxiliary axis.
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_AuxGetRefPosLatchActive(EIB_Data *ed, unsigned long* active, unsigned long* valid);


/*! The reference position latch operation will be started
    for the auxiliary axis.
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_AuxStartRef(EIB_Data *ed);


/*! The reference position latch operation will be aborted
    for the auxiliary axis.
  \param ed the pointer to the EIB_Data structure for this connection
  \return EIB7_NoError if the call returns successfully
*/
EIB7_ERR EIBCmd_AuxStopRef(EIB_Data *ed);


/*! Returns the current position of the encoder on the auxiliary axis. 
    Returns an error if the EIB is not in polling mode.
  \param ed the pointer to the EIB_Data structure for this connection
  \param channel the encoder channel (0-based)
  \param status status word
  \param pos position value
  \return EIB7_NoError if the axis could be reset
*/
EIB7_ERR EIBCmd_AuxGetPosition(EIB_Data *ed,
			       unsigned short* status,
			       ENCODER_POSITION* pos);


/*! Read the current position value and status information for the auxiliary axis.
  The position will be latched. The EIB must be in the polling mode.
  \param ed the pointer to the EIB_Data structure for this connection
  \param status pointer to a variable which receives the status word.
  \param pos pointer to a variable which receives the position value
  \param ref pointer to a variable which receives the reference position
  \param timestamp pointer to a variable which receives the timestamp for the position value
  \param counter pointer to a variable which receives the trigger counter
  \return
  - EIB7_NoError if the command executed without error
  - EIB7_ParamInvalid ??
  - EIB7_TimerTrgEn Timer Trigger is enabled
  - EIB7_ExtTrgEn External Trigger is enabled
  - EIB7_SoftRTEn Soft Realtime Mode is enabled
*/  
EIB7_ERR EIBCmd_AuxGetEncoderData(EIB_Data *ed,
			       unsigned short* status,
			       ENCODER_POSITION* pos,
			       ENCODER_POSITION* ref,
			       unsigned long* timestamp,
			       unsigned short* counter);


/*! Enable or disable timestamps for the auxiliary axis
  \param ed the pointer to the EIB_Data structure for this connection
  \param mode an EIB7_MODE to enable or disable the timestamp
  \return 
  - EIB7_NoError if the command executed without error
  - EIB7_InvalidHandle if the EIB handle is not valid
*/
EIB7_ERR EIBCmd_AuxSetTimestamp(EIB_Data *ed, EIB7_MODE mode);




#endif /* __EIB7_CMD_H__ */
