/*! \file cmdtcp.c
    \brief This file implements the network-based command handling.
    
    This file implements the network-based command handling. All commands are
    defined as data, complete with all information needed to access the return values.
    This is done with the idea in mind to be able to switch to a different command
    mechanism (e.g. direct PCI-E bus interface) easily.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eib7.h"
#include "definitions.h"
#include "commtcp.h"
#include "cmdtcp.h"
#include "commands.h"


#define MAXCOMMANDSIZE 256   /*!< Maximum size of command send buffer */
#define MAXRESPONSESIZE 256  /*!< Maximum size of command response buffer */

#define HEADERSIZE 12 /*!< Command header size */

#define MAXUDPSIZE 256 /*!< Maximum size of a UDP datagram */
#define TCPMSS     1460 /*!< Maximum segment size of one TCP packet */

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

/*! This call sets an U16 data field into the command buffer */

static void SetU16(unsigned char *data, unsigned short value)
{
  data[0] = value & 0x00ff;
  data[1] = (value & 0xff00) >> 8;
}

/*! This call gets an U16 data field from the result buffer */

static unsigned short GetU16(unsigned char *data)
{
  return 
    (unsigned short)data[0] +
    ((unsigned short)data[1] << 8);
}

/*! This call sets an U24 data field into the command buffer */

static void SetU24(unsigned char *data, unsigned long value)
{
  data[0] = value & 0x000000ff;
  data[1] = (value & 0x0000ff00) >> 8;
  data[2] = (value >> 16) & 0xff;
}

/*! This call gets an U24 data field from the result buffer */

static unsigned long GetU24(unsigned char *data)
{
  return 
    (unsigned long)data[0] +
    ((unsigned long)data[1] << 8) +
    ((unsigned long)data[2] << 16);
}

/*! This call sets an U32 data field into the command buffer */

static void SetU32(unsigned char *data, unsigned long value)
{
  data[0] = value & 0x000000ff;
  data[1] = (value & 0x0000ff00) >> 8;
  data[2] = (value >> 16) & 0xff;
  data[3] = (value & 0xff000000) >> 24;
}

/*! This call gets an U32 data field from the result buffer */

static unsigned long GetU32(unsigned char *data)
{
  return 
    (unsigned long)data[0] +
    ((unsigned long)data[1] << 8) +
    ((unsigned long)data[2] << 16) +
    ((unsigned long)data[3] << 24);
}

/*! This call sets an Position data field into the command buffer */

#ifdef Win32
#define LONGLONG_CONST(a) a
#else
#define LONGLONG_CONST(a) a##LL
#endif

static void SetPOS(unsigned char *data, ENCODER_POSITION value)
{
  data[0] =  value & LONGLONG_CONST(0x00000000000000ff);
  data[1] = (value >>  8) & LONGLONG_CONST(0xff);
  data[2] = (value >> 16) & LONGLONG_CONST(0xff);
  data[3] = (value >> 24) & LONGLONG_CONST(0xff);
  data[4] = (value >> 32) & LONGLONG_CONST(0xff);
  data[5] = (value >> 40) & LONGLONG_CONST(0xff);
}

/*! This call gets an Position data field from the result buffer */

static ENCODER_POSITION GetPOS(unsigned char *data)
{
  return 
     (ENCODER_POSITION)data[0] +
    ((ENCODER_POSITION)data[1] << 8 ) +
    ((ENCODER_POSITION)data[2] << 16) +
    ((ENCODER_POSITION)data[3] << 24) +
    ((ENCODER_POSITION)data[4] << 32) +
    ((ENCODER_POSITION)data[5] << 40) +
    ((ENCODER_POSITION)(data[5] & 0x80 ? 0xffff : 0x0000) << 48);
}

static void SetAuxPOS(unsigned char *data, ENCODER_POSITION value)
{
  data[0] =  value & LONGLONG_CONST(0x00000000000000ff);
  data[1] = (value >>  8) & LONGLONG_CONST(0xff);
  data[2] = (value >> 16) & LONGLONG_CONST(0xff);
  data[3] = (value >> 24) & LONGLONG_CONST(0xff);
}

/*! This call gets an Position data field from the result buffer */

static ENCODER_POSITION GetAuxPOS(unsigned char *data)
{
  return 
     (ENCODER_POSITION)data[0] +
    ((ENCODER_POSITION)data[1] << 8 ) +
    ((ENCODER_POSITION)data[2] << 16) +
    ((ENCODER_POSITION)data[3] << 24) +
    ((ENCODER_POSITION)(data[3] & 0x80 ? 0xffffffff : 0x00000000) << 32);
}


typedef struct 
{
  int Command;
  int SendSize;
  int ReceiveSize;
  unsigned char SendBuffer[MAXCOMMANDSIZE];
  unsigned char ReceiveBuffer[MAXRESPONSESIZE];
}TCP_CommandData;
  
/*! Initialize the command structure for a command to be executed
  \param command the index of the command entry in the Commands array
  \param cmddata a pointer which receives a pointer to the command structure
  \return EIB7_NoError if command was executed successfully
*/
EIB7_ERR TCP_InitializeCommand(int command, void **cmddata)
{
  TCP_CommandData *cd;
  
  cd = (TCP_CommandData *) calloc(sizeof(TCP_CommandData), 1);
  *cmddata = cd;
  
  if(cd == NULL)
    {
      return EIB7_OutOfMemory;
    }

  cd->Command = command;
  cd->SendSize = HEADERSIZE;

  SetU32(&cd->SendBuffer[0], Commands[command].opcode);
  
  return EIB7_NoError;
}

/*! Free a command structure after use.
  \param cmddata a pointer to the TCP_CommandData structure 
  \return EIB7_NoError if command was executed successfully
*/
EIB7_ERR TCP_FreeCommand(void *cmddata)
{
  free(cmddata);
  
  return EIB7_NoError;
}

/*! Set an argument in the command structure.
  \param cmddata a pointer to the command data structure
  \param index the index of the parameter in the command
  \param value a pointer to the data variable for the parameter.
  For integer data, it is always assumed to point to an 4-Byte data type
  \param size the size of the parameter data. Only valid for parameters with variable size
  \return EIB7_NoError if the argument could be placed in the command
*/
EIB7_ERR TCP_SetParameter(void *cmddata, int index, void *value, int size)
{
  TCP_CommandData *cd;
  TCP_Parameter *par;
  int sz;
  int pos;
  
  cd = (TCP_CommandData*)cmddata;

  if(index >= Commands[cd->Command].numparams)
    {
      return EIB7_IllegalParameter;
    }

  par = &Commands[cd->Command].param[index];

  sz = par->size >= 0 ? par->size : size;
  
  pos = par->offset + HEADERSIZE;

  if(pos + sz  > MAXCOMMANDSIZE)
    {
      return EIB7_IllegalParameter;
    }

  switch(par->type)
    {
    case TCP_PT_Data:
      memcpy(&cd->SendBuffer[pos], value, size);
      break;
    case TCP_PT_U8:
      cd->SendBuffer[pos] =        (unsigned char)(*(unsigned long *)value);
      break;
    case TCP_PT_U16:
      SetU16(&cd->SendBuffer[pos], (unsigned short)*(unsigned long *)value);
      break;
    case TCP_PT_U24:
      SetU24(&cd->SendBuffer[pos], *(unsigned long *)value);
      break;
    case TCP_PT_U32:
      SetU32(&cd->SendBuffer[pos], *(unsigned long *)value);
      break;
    case TCP_PT_POS:
      SetPOS(&cd->SendBuffer[pos], *(ENCODER_POSITION *)value);
      break;
    case TCP_PT_AUXPOS:
      SetAuxPOS(&cd->SendBuffer[pos], *(ENCODER_POSITION *)value);
      break;

    default:
      return EIB7_IllegalParameter;
    }

  if(par->offset + sz + HEADERSIZE > cd->SendSize)
    {
      cd->SendSize = par->offset + sz + HEADERSIZE;
    }

  return EIB7_NoError;
}

/*! Get a response data field from the command structure
  \param cmddata a pointer to the command data structure
  \param index the index of the result field in the command
  \param value a pointer to the data variable for the parameter.
  For integer data, it is always assumed to point to an 4-Byte data type
  \param size the size of the parameter data. Only valid for parameters with variable size
  \param offset an additional byte offset for this parameter. Used to simulated arrays.
  \return EIB7_NoError if the result could be retrieved
*/
EIB7_ERR TCP_GetParameter(void *cmddata, int index, void *value, int size, int offset)
{
  TCP_CommandData *cd;
  TCP_Parameter *par;
  int sz;
  int pos;
  
  cd = cmddata;

  if(index >= Commands[cd->Command].numresults)
    {
      return EIB7_IllegalParameter;
    }

  par = &Commands[cd->Command].result[index];

  sz = par->size >= 0 ? par->size : size;

  pos = par->offset + HEADERSIZE + offset;
  
  if(pos + sz > cd->ReceiveSize)
    {
      return EIB7_IllegalParameter;
    }

  switch(par->type)
    {
    case TCP_PT_Data:
      memcpy(value, &cd->ReceiveBuffer[pos], size);
      break;
    case TCP_PT_U8:
      *(unsigned char *)value = cd->ReceiveBuffer[pos];
      break;
    case TCP_PT_U16:
      *(unsigned short *)value = GetU16(&cd->ReceiveBuffer[pos]);
      break;
    case TCP_PT_U24:
      *(unsigned long *)value = GetU24(&cd->ReceiveBuffer[pos]);
      break;
    case TCP_PT_U32:
      *(unsigned long *)value = GetU32(&cd->ReceiveBuffer[pos]);
      break;
    case TCP_PT_POS:
      *(ENCODER_POSITION *)value = GetPOS(&cd->ReceiveBuffer[pos]);
      break;
    case TCP_PT_AUXPOS:
      *(ENCODER_POSITION *)value = GetAuxPOS(&cd->ReceiveBuffer[pos]);
      break;
    default:
      return EIB7_IllegalParameter;
    }

  return EIB7_NoError;
}

static void SetStatus(EIB_TCPconn *conn, int error)
{
  switch(error)
    {
    case EIB7_ConnReset:
      conn->status = EIB7_CS_ConnectionReset;
      break;
    case EIB7_ConnTimeout:
      conn->status = EIB7_CS_Timeout;
      break;
    default:
      conn->status = EIB7_CS_TransmissionError;
    }
}

/*! Execute a command on the EIB device, and awaits the response. Sets the appropriate
  error information.
  \param tcpconn the connection info for this device
  \param seqno the sequence number for the command
  \param cmddata a pointer to the command structure
  \param resplen a pointer to an integer that receives the size of the response data (without header)
  \return EIB7_NoError if command was executed successfully
*/

EIB7_ERR TCP_ExecuteCommand(void *tcpconn, int seqno, void *cmddata, int *resplen)
{
  TCP_CommandData *cd;
  EIB7_ERR error;
  int respno;
  EIB_TCPconn *conn;

  conn = (EIB_TCPconn *)tcpconn;

  if(resplen)
    *resplen = 0;
  
  cd = cmddata;

  SetU32(&cd->SendBuffer[4], cd->SendSize);
  SetU32(&cd->SendBuffer[8], seqno);

  error = EIB_SendSocket(conn->sock, cd->SendBuffer, cd->SendSize);

  if(error != EIB7_NoError)
    {
      SetStatus(conn, error);
      return error;
    }

  /* check if we should wait for *ANY* answer */
  if(Commands[cd->Command].numresults < 0)
    return EIB7_NoError;
  
  do
    {
      unsigned long rescode;
      int size;
      
      cd->ReceiveSize = HEADERSIZE;
      error = EIB_ReceiveSocket(conn->sock, cd->ReceiveBuffer, &cd->ReceiveSize);
      
      if(error != EIB7_NoError)
	{
	  SetStatus(conn, error);
	  return error;
	}
      
      rescode = GetU32(&cd->ReceiveBuffer[0]);
      size = GetU32(&cd->ReceiveBuffer[4]);
      respno = GetU32(&cd->ReceiveBuffer[8]);

      /* Receive the rest of the response, in any case */
      cd->ReceiveSize = size - HEADERSIZE;
      if(cd->ReceiveSize > 0)
	error = EIB_ReceiveSocket(conn->sock, &cd->ReceiveBuffer[HEADERSIZE], &cd->ReceiveSize);
      
      if(error != EIB7_NoError)
	{
	  SetStatus(conn, error);
	  return error;
	}
      
      /* check for matching response */
      if(respno == seqno)
	{
	  if(rescode == 0xc0000000)
	    {
	      return EIB7_FuncNotSupp;
	    }
	  if(rescode == 0xc0000001)
	    {
	      return EIB7_AccNotAllowed;
	    }
	  if(rescode != (0x80000000 | Commands[cd->Command].opcode))
	    {
	      return EIB7_InvalidResponse;
	    }
	  
	  cd->ReceiveSize = size;
	  if(resplen)
	    *resplen = size - HEADERSIZE;

	  conn->status = EIB7_CS_Connected;
	  
	  return EIB7_NoError;
	}
    }
  while (respno < seqno);

  return EIB7_InvalidResponse;
}

static EIB_CommandInterface TCP_Interface = {
  TCP_InitializeCommand,
  TCP_FreeCommand,
  TCP_SetParameter,
  TCP_GetParameter,
  TCP_ExecuteCommand
};

struct EIB_CommandInterface *EIB_GetTCPCommandInterface()
{
  return &TCP_Interface;
}

void EIB_SoftRealtimeHandlerUDP(void *userdata)
{           
  EIB_Data *ed;
  unsigned char data[MAXUDPSIZE];
  int len;
  int num;
  int bytes;

  ed = (EIB_Data *)userdata;

  while(!ed->threadendflag)
    {
      num = 0;
      
      if(EIB_WaitForUDPData(ed->conndata.tcpconn.udpsock, 1000))
	{
	  EIB_AcquireThreadLock(&ed->fifolock);

	  while(EIB_ReceiveUDP(ed->conndata.tcpconn.udpsock, data, MAXUDPSIZE, &bytes, &len))
	    {
	      ed->maxudp = MAX(bytes, ed->maxudp);
	  
	      /* Check if we just got the thread end signal */
	      if(ed->threadendflag)
		{
		  EIB_ReleaseThreadLock(&ed->fifolock);
		  return;
		}

	      EIB_InsertFIFO(&ed->fifo, data, len, &num);
	    }

	  EIB_ReleaseThreadLock(&ed->fifolock);

	  /* The callback is called outside the critical section, to allow
	     it to call all other EIB7* functions */
	  if(num >= ed->threshold && ed->callback)
	    ed->callback(ed->handle, num, ed->callbackdata);
      
	}
    }
}


#define REC_BUF_SIZE    (2*TCPMSS)



void EIB_RecordingHandlerTCP(void* userdata)
{
   EIB_Data *ed;
   unsigned char data[REC_BUF_SIZE];
   unsigned long memsize;
   unsigned long samples;
   unsigned long samplelength;
   unsigned char* src;
   int len;
   int num;
   int bytes;
   unsigned long i;
   unsigned long checkFifoEmpty;

   ed = (EIB_Data *)userdata;
   memsize = 0;
   samples = 0;
   checkFifoEmpty = 0;

   while(!ed->threadendflag)
   {
      num = 0;

	  if(EIB_WaitForUDPData(ed->conndata.tcpconn.tcpdatasocket, 1000))
      {
         EIB_AcquireThreadLock(&ed->fifolock);

         checkFifoEmpty = 0;
         if(memsize > 0)
            checkFifoEmpty = 1;

         while((EIB_ReceiveUDP(ed->conndata.tcpconn.tcpdatasocket, &data[memsize], REC_BUF_SIZE-memsize, &bytes, &len) > 0) | (checkFifoEmpty > 0))
         {
            checkFifoEmpty = 0;
            ed->maxudp = MAX(bytes, ed->maxudp);

            /* Check if we just got the thread end signal */
            if(ed->threadendflag)
            {
               EIB_ReleaseThreadLock(&ed->fifolock);
               return;
            }

            /* there can not be insert split samples to the fifo */
            memsize += len;
            samplelength = ed->posdatasize_raw + (ed->posdatasize_raw % ed->posdata_allign);
            samples = memsize / samplelength;
            if(samples > (unsigned long)EIB_GetFIFOFreeElements(&ed->fifo))  // always >= 0
               samples = EIB_GetFIFOFreeElements(&ed->fifo);

            for(i=0; i<samples; i++)
            {
               EIB_InsertFIFO(&ed->fifo, &data[i*samplelength], ed->posdatasize_raw, &num);
            }

            /* prevent negativ memory length */
            if((samples * samplelength) <= memsize)
               memsize -= samples * samplelength;
            else
               memsize = 0;

            src = &data[i * samplelength];
            if(src != data)
               memcpy(data, src, memsize);  /* do not copy the memory to itself */
         } /* end while */

         EIB_ReleaseThreadLock(&ed->fifolock);

         if((memsize == REC_BUF_SIZE) || (samples == 0))		/* buffer is full -> wait */
            EIB_Sleep(1);									         /* minmize processor load */

         if((bytes == 0) && (memsize == 0))                 /* no data received, all data copied */
            EIB_Sleep(1);                                   /* wait for thread end */

         /* The callback is called outside the critical section, to allow
         it to call all other EIB7* functions */
         if(num >= ed->threshold && ed->callback)
            ed->callback(ed->handle, num, ed->callbackdata);

      }
	  else
	  {
		 EIB_Sleep(1);									/* minmize processor load */
	  } /* end if */

   } /* end while */
} /* end func */


#define STRM_BUF_SIZE    (2*TCPMSS)


void EIB_StreamingHandlerUDP(void* userdata)
{
   EIB_Data *ed;
   unsigned char data[STRM_BUF_SIZE];
   unsigned char buff[10];
   unsigned long memsize;
   unsigned long samples;
   unsigned long samplelength;
   unsigned char* src;
   int len;
   int num;
   int bytes;
   unsigned long i;
   unsigned long recv_total;
   unsigned long checkFifoEmpty;


   ed = (EIB_Data *)userdata;
   memsize = 0;
   samples = 0;
   checkFifoEmpty = 0;
   /* send size of data window */
   SetU32(buff, ed->fifo.size);
   EIB_SendUDP(ed->conndata.tcpconn.udpsock, buff, 4);

   while(!ed->threadendflag)
   {
      num = 0;

      if(EIB_WaitForUDPData(ed->conndata.tcpconn.udpsock, 1000))
      {
         EIB_AcquireThreadLock(&ed->fifolock);
         recv_total = 0;

         checkFifoEmpty = 0;
         if(memsize > 0)
            checkFifoEmpty = 1;

         while((EIB_ReceiveUDP(ed->conndata.tcpconn.udpsock, &data[memsize], STRM_BUF_SIZE-memsize, &bytes, &len) > 0) | (checkFifoEmpty > 0))
         {
            checkFifoEmpty = 0;
            ed->maxudp = MAX(bytes, ed->maxudp);

            /* Check if we just got the thread end signal */
            if(ed->threadendflag)
            {
               EIB_ReleaseThreadLock(&ed->fifolock);
               return;
            }

            /* there can not be insert split samples to the fifo */
            memsize += len;
            recv_total += len;
            samplelength = ed->posdatasize_raw + (ed->posdatasize_raw % ed->posdata_allign);
            samples = memsize / samplelength;

            if(samples > (unsigned long)EIB_GetFIFOFreeElements(&ed->fifo))   // always >= 0
               samples = EIB_GetFIFOFreeElements(&ed->fifo);

            for(i=0; i<samples; i++)
            {
               EIB_InsertFIFO(&ed->fifo, &data[i*samplelength], ed->posdatasize_raw, &num);
            }

            /* prevent negativ memory length */
            if((samples * samplelength) <= memsize)
               memsize -= samples * samplelength;
            else
               memsize = 0;

            src = &data[i * samplelength];
            if(src != data)
               memcpy(data, src, memsize);  /* do not copy the memory to itself */
         } /* end while */

         if(recv_total > 0)
         {
            SetU32(buff, recv_total);
            EIB_SendUDP(ed->conndata.tcpconn.udpsock, buff, 4);
            recv_total = 0;
         }
         EIB_ReleaseThreadLock(&ed->fifolock);
         if((memsize == REC_BUF_SIZE) || (samples == 0))		/* buffer is full -> wait */
            EIB_Sleep(1);									         /* minmize processor load */

         if((bytes == 0) && (memsize == 0))                 /* no data received, all data copied */
            EIB_Sleep(1);                                   /* wait for thread end */

         /* The callback is called outside the critical section, to allow
         it to call all other EIB7* functions */
         if(num >= ed->threshold && ed->callback)
            ed->callback(ed->handle, num, ed->callbackdata);

      }
	  else
	  {
		 EIB_Sleep(1);									/* minmize processor load */
	  } /* end if */
   } /* end while */
} /* end func */



