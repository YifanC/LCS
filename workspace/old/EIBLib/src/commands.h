#ifndef __COMMANDS_H__
#define __COMMANDS_H__

/*!
  \file commands.h
  \brief this file declares the command interface structure and the command
  codes.

*/

#include "eib7_types.h"
#include "cmdtcp_list.h"


/*! Initialize the command structure for a command to be executed
  \param ed the pointer to the EIB_Data structure for this connection
  \param command the index of the command entry in the Commands array
  \param cmddata a pointer which receives a pointer to the command structure
  \return EIB7_NoError if command was executed successfully
*/
EIB7_ERR CmdIF_InitializeCommand(EIB_Data *ed, int command, void **cmddata);

/*! Free a command structure after use.
  \param ed the pointer to the EIB_Data structure for this connection
  \param cmddata a pointer to the command data structure 
  \return EIB7_NoError if command was executed successfully
*/
EIB7_ERR CmdIF_FreeCommand(EIB_Data *ed, void *cmddata);

/*! Set an argument in the command structure.
  \param ed the pointer to the EIB_Data structure for this connection
  \param cmddata a pointer to the command data structure
  \param index the index of the parameter in the command
  \param value a pointer to the data variable for the parameter.
  For integer data, it is always assumed to point to an 4-Byte data type
  \param size the size of the parameter data. Only valid for parameters with variable size
  \return EIB7_NoError if the argument could be placed in the command
*/
EIB7_ERR CmdIF_SetParameter(EIB_Data *ed, void *cmddata, int index, void *value, int size);

/*! Get a response data field from the command structure
  \param ed the pointer to the EIB_Data structure for this connection
  \param cmddata a pointer to the command data structure
  \param index the index of the result field in the command
  \param value a pointer to the data variable for the parameter.
  For integer data, it is always assumed to point to an 4-Byte data type
  \param size the size of the parameter data. Only valid for parameters with variable size
  \param offset the offset of the parameter data 'page'. Only valid for repeating result fields, e.g. connection info
  \return EIB7_NoError if the result could be retrieved
*/
EIB7_ERR CmdIF_GetParameter(EIB_Data *ed, void *cmddata, int index, void *value, int size, int offset);

/*! Execute a command on the EIB device, and awaits the response. Sets the appropriate
  error information.
  \param ed the pointer to the EIB_Data structure for this connection
  \param cmddata a pointer to the command structure
  \param resplen the size of the response data
  \return EIB7_NoError if command was executed successfully
*/

EIB7_ERR CmdIF_ExecuteCommand(EIB_Data *ed, void *cmddata, int *resplen);

/*! initialize the EIB data structure with the standard data fields
  \param ed the pointer to the EIB_Data structure for this connection
*/
EIB7_ERR EIB_InitDataFields(EIB_Data *ed);

/*! deletes the EIB data strukture
  \param packet pointer to the packet data structure
*/
void EIB_DeleteDataField(EIB_DataPacketDescription* packet);

/*! generates the data packet structure for the current data packet
  \param packet pointer to the new packet data structure
  \param size number of valid entries in the packet configuration data
  \param ed the pointer to the EIB_Data structure for this connection
*/
EIB7_ERR EIB_MakeDataField(EIB7_DataPacketSection* packet, unsigned long size, EIB_Data *ed);

/*! Calculate all offsets for the data fields in the position data structure for this EIB.
  Call this function every time the EIB_Data->field member is updated
  \param ed the pointer to the EIB_Data structure for this connection
*/
void EIB_CalculateOffsets(EIB_Data *ed);

/*! Convert one position data record. Converts all 6-Byte position data fields to
  ENCODER_POSITION values
  \param ed the pointer to the EIB_Data structure for this connection
  \param raw_data a pointer to the raw position data structure
  \param data a pointer to the destination position data structure
*/
void EIB_ConvertPositionData(EIB_Data *ed, void *raw_data, void *data);


/*! Calculates the length of the packet configuration for the EIB
    in bytes.
  \param packet pointer to data packet configuration
  \return number of bytes
*/
int EIB_GetPacketConfigLength(EIB_DataPacketDescription* packet);


/*! Converts the data packet configuration from the configuration structure
    #EIB_DataPacket to an byte array.
  \param packet pointer to data packet configuration
  \param dest pointer to array
  \return 0 if successfull, -1 else
*/
int EIB_ConvertPacketConfiguration(EIB_DataPacketDescription* packet, unsigned char* data, int len);


#endif /* __COMMANDS_H__ */
