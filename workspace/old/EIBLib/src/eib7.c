/*!
  \file eib7.c
  \brief This file is the main interface implementation for the EIB 7 driver DLL.
  This file is the main interface implementation for the EIB 7 driver DLL. Please note, that we use
  a .def file (eib7.def) to export the desired functions to the DLL in Windows.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eib7.h"
#include "eib7_types.h"
#include "eib7_cmd.h"
#include "definitions.h"
#include "cmdtcp.h"
#include "threading.h"
#include "commands.h"
#include "handles.h"
#include "fifo.h"
#include "eib7_errors.h"

#ifdef Win32
#define snprintf _snprintf
#endif


/*! The bitmask for EIBCmd_ClearEncoderErrors() to clear the power supply error */
#define CLR_POWERSUPPLY_BITS (0x01)

/*! The bitmask for EIBCmd_ClearEncoderErrors() to clear the lost trigger error */
#define CLR_LOST_TRIGGER_ERROR (0x02)

/*! The bitmask for EIBCmd_ClearEncoderErrors() to clear the encoder error flags */
#define CLR_ENCODER_ERRORS (0x0c)

/*! The bitmask for EIBCmd_ClearEncoderErrors() to clear the EnDat error message */
#define CLR_ENDAT_ERROR_MSG (0x08)

/*! The bitmask for EIBCmd_ClearRefLatchedStatus() to clear the reference position latched flag */
#define CLR_REF_LATCHED (0x01)

/*! The bitmask for EIBCmd_ClearRefLatchedStatus() to clear the reference position status flags */
#define CLR_REF_STATUS (0x07)

#define RANGECHECK(value, min, max)					\
  if(value < min || value > max) return EIB7_ParamInvalid;	



EIB7_ERR EIB7GetDataFieldPtr(EIB7_HANDLE eib, void *data, EIB7_DataRegion region, EIB7_PositionDataField type, void **field, unsigned long *size)
{
  EIB_Data *ed;
  EIB7_ERR error;
  EIB_DataRegionDescription* pRegion;
  EIB_DataFieldDescription* pField;


  RANGECHECK(region, 0, EIB7_DR_MAXVAL);
  RANGECHECK(type, EIB7_PDF_MINVAL, EIB7_PDF_MAXVAL);
  
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  EIB_AcquireThreadLock(&ed->threadlock);
  
   if(region == EIB7_DR_AUX)
   {
      type = type << 12;
   }

  pRegion = ed->pkt_head.firstregion;

  while(pRegion != NULL)
  {
     if(pRegion->type == region)
     {
        pField = pRegion->firstfield;
        while(pField != NULL)
        {
           if(pField->type == type)
           {
	           *field = (unsigned char *)data + pField->offset;
              *size = pField->size;

	           EIB_ReleaseThreadLock(&ed->threadlock);
         	  
	           return EIB7_NoError;
           }
           
           pField = pField->next;
        }
     }

     pRegion = pRegion->next;
  }

  EIB_ReleaseThreadLock(&ed->threadlock);

  *field = NULL;
  *size = 0;
  
  return EIB7_FieldNotAvail;
}


EIB7_ERR EIB7GetDataFieldPtrRaw(EIB7_HANDLE eib, void *data, EIB7_DataRegion region, EIB7_PositionDataField type, void **field, unsigned long *size)
{
  EIB_Data *ed;
  EIB7_ERR error;
  EIB_DataRegionDescription* pRegion;
  EIB_DataFieldDescription* pField;

  
  RANGECHECK(region, 0, EIB7_DR_MAXVAL);
  RANGECHECK(type, EIB7_PDF_MINVAL, EIB7_PDF_MAXVAL);
  
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  EIB_AcquireThreadLock(&ed->threadlock);
  
   if(region == EIB7_DR_AUX)
   {
      type = type << 12;
   }

  pRegion = ed->pkt_head.firstregion;

  while(pRegion != NULL)
  {
     if(pRegion->type == region)
     {
        pField = pRegion->firstfield;
        while(pField != NULL)
        {
           if(pField->type == type)
           {
	           *field = (unsigned char *)data + pField->offset_raw;
              *size = pField->size_raw;

	           EIB_ReleaseThreadLock(&ed->threadlock);
         	  
	           return EIB7_NoError;
           }

           pField = pField->next;
        }
     }

     pRegion = pRegion->next;
  }

  EIB_ReleaseThreadLock(&ed->threadlock);

  *field = NULL;
  *size = 0;
  
  return EIB7_FieldNotAvail;
}


EIB7_ERR EIB7GetErrorInfo(EIB7_ERR code, char *mnemonic, unsigned long mnemlen, char *message, unsigned long msglen)
{
  int i;

  for(i = 0; EIB7_Errors[i].mnemonic != NULL; i++)
    {
      if(EIB7_Errors[i].code == (unsigned long)code)
	{
	  if(mnemonic != NULL)
	    {
         #pragma warning( suppress : 4996)
	      strncpy(mnemonic, EIB7_Errors[i].mnemonic, mnemlen);
	      mnemonic[mnemlen-1]=0;
	    }
	  if(message != NULL)
	    {
         #pragma warning( suppress : 4996)
	      strncpy(message, EIB7_Errors[i].message, msglen);
	      message[msglen-1]=0;
	    }
	  return EIB7_NoError;
	}
    }

  return EIB7_IllegalParameter;
}


EIB7_ERR EIB7GetHostIP(const char *hostname, unsigned long *ip)
{
  return EIB_GetHostIP(hostname, ip);
}

EIB7_ERR EIB7Open(unsigned long ip, EIB7_HANDLE *eib, long timeout, char *ident, unsigned long len)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned long version;
  long internal_timeout;
  int i;
  EIB7_DataPacketSection packet[5];

  *eib = 0;

  internal_timeout=timeout;
  
  if(internal_timeout == 0)
    internal_timeout = TCP_DEFAULT_TIMEOUT;
  if(internal_timeout < TCP_MIN_TIMEOUT)
    internal_timeout = TCP_MIN_TIMEOUT;
  
  ed = (EIB_Data *)calloc(sizeof(EIB_Data), 1);

  if(ed == NULL)
    return EIB7_OutOfMemory;

  EIB_InitSocket(&ed->conndata.tcpconn.udpsock);
  EIB_InitSocket(&ed->conndata.tcpconn.sock);
  EIB_InitSocket(&ed->conndata.tcpconn.tcpdatasocket);

  error = EIB_OpenSocket(ip, &ed->conndata.tcpconn.sock, internal_timeout);
  if(error != EIB7_NoError)
    {
      free(ed);
      return error;
    }

  EIB_InitThreadLock(&ed->threadlock);
  EIB_InitThreadLock(&ed->fifolock);

  ed->cif = EIB_GetTCPCommandInterface();
  ed->conntype = EIB_TCPConnection;
  
  error = EIB_ReserveHandle(eib, ed);
  if(error != EIB7_NoError)
    {
      EIB_FreeThreadLock(&ed->threadlock);
      EIB_FreeThreadLock(&ed->fifolock);
      EIB_CloseSocket(&ed->conndata.tcpconn.sock);
      free(ed);
      *eib = 0;
      return error;
    }

  ed->handle = *eib;
  ed->conn = &ed->conndata.tcpconn;
  ed->conndata.tcpconn.status = EIB7_CS_Connected;
  ed->master = FALSE;
  ed->fifosize = FIFO_DEFAULT_SIZE;
  
  error = EIB_InitDataFields(ed);
  if(error != EIB7_NoError)
  {
	  EIB_FreeThreadLock(&ed->threadlock);
	  EIB_FreeThreadLock(&ed->fifolock);
	  EIB_CloseSocket(&ed->conndata.tcpconn.sock);
	  free(ed);
	  *eib = 0;
	  return error;
  }

  ed->posdata_allign = POSDATA_ALLIGN_POLL;

  // config default data packet
  packet[0].region = EIB7_DR_Global;
  packet[0].items = EIB7_PDF_TriggerCounter;
  packet[1].region = EIB7_DR_Encoder1;
  packet[1].items = EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef | EIB7_PDF_Analog;
  packet[2].region = EIB7_DR_Encoder2;
  packet[2].items = EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef | EIB7_PDF_Analog;
  packet[3].region = EIB7_DR_Encoder3;
  packet[3].items = EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef | EIB7_PDF_Analog;
  packet[4].region = EIB7_DR_Encoder4;
  packet[4].items = EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef | EIB7_PDF_Analog;
  error = EIB_MakeDataField(packet, 5, ed);
  if(error != EIB7_NoError)
  {
	  EIB_FreeThreadLock(&ed->threadlock);
	  EIB_FreeThreadLock(&ed->fifolock);
	  EIB_CloseSocket(&ed->conndata.tcpconn.sock);
	  free(ed);
	  *eib = 0;
	  return error;
  }
  EIB_CalculateOffsets(ed);

  if(ident != NULL && len > 0)
    {
      error = EIB7GetVersion(*eib, ident, len, EIB7_FW_CurrentlyBooted);
      if(error != EIB7_NoError)
	{
	  EIB_FreeThreadLock(&ed->threadlock);
	  EIB_FreeThreadLock(&ed->fifolock);
	  EIB_CloseSocket(&ed->conndata.tcpconn.sock);
	  free(ed);
	  *eib = 0;
	  return error;
	}
    }
      
  error = EIBCmd_GetInterfaceVersion(ed, &version);
  if(error != EIB7_NoError)
    {
      EIB_FreeThreadLock(&ed->threadlock);
      EIB_FreeThreadLock(&ed->fifolock);
      EIB_CloseSocket(&ed->conndata.tcpconn.sock);
      free(ed);
      *eib = 0;
      /* Explicitly return invalid version error */
      return EIB7_IFVersionInv;
    }
  if(((version >> 24) & 0xff) != EIB7_MAJOR_VERSION ||
     ((version >> 16) & 0xff) <  EIB7_MINOR_VERSION)
    {
      EIB_FreeThreadLock(&ed->threadlock);
      EIB_FreeThreadLock(&ed->fifolock);
      EIB_CloseSocket(&ed->conndata.tcpconn.sock);
      free(ed);
      *eib = 0;
      return EIB7_IFVersionInv;
    }

  for(i = 0; i < NUM_AXIS_PER_EIB; i++)
    {
      ed->axis[i].eib = *eib;
      ed->axis[i].channel = i;
    }

  for(i = 0; i < NUM_PORTS_PER_EIB; i++)
    {
      ed->port[i].eib = *eib;
      ed->port[i].port = i & (NUM_INPUTS_PER_EIB-1);
      ed->port[i].output = i >= NUM_INPUTS_PER_EIB ? 1 : 0;
    }
  
  error = EIB7SetTimeout(*eib,timeout);
  
  return error;
}

EIB7_ERR EIB7OpenNoIFCheck(unsigned long ip, EIB7_HANDLE *eib, long timeout, char *ident, unsigned long len)
{
  EIB7_ERR error;
  EIB_Data *ed;
  int i;
  EIB7_DataPacketSection packet[5];

  *eib = 0;

  if(timeout == 0)
    timeout = TCP_DEFAULT_TIMEOUT;
  if(timeout < 100)
    timeout = 100;
  
  ed = (EIB_Data *)calloc(sizeof(EIB_Data), 1);

  if(ed == NULL)
    return EIB7_OutOfMemory;

  EIB_InitSocket(&ed->conndata.tcpconn.udpsock);
  EIB_InitSocket(&ed->conndata.tcpconn.sock);
  EIB_InitSocket(&ed->conndata.tcpconn.tcpdatasocket);

  error = EIB_OpenSocket(ip, &ed->conndata.tcpconn.sock, timeout);
  if(error != EIB7_NoError)
    {
      free(ed);
      return error;
    }

  EIB_InitThreadLock(&ed->threadlock);
  EIB_InitThreadLock(&ed->fifolock);

  ed->cif = EIB_GetTCPCommandInterface();
  ed->conntype = EIB_TCPConnection;
  
  error = EIB_ReserveHandle(eib, ed);
  if(error != EIB7_NoError)
    {
      EIB_FreeThreadLock(&ed->threadlock);
      EIB_FreeThreadLock(&ed->fifolock);
      EIB_CloseSocket(&ed->conndata.tcpconn.sock);
      free(ed);
      *eib = 0;
      return error;
    }

  ed->handle = *eib;
  ed->conn = &ed->conndata.tcpconn;
  ed->conndata.tcpconn.status = EIB7_CS_Connected;
  ed->master = FALSE;
  ed->fifosize = FIFO_DEFAULT_SIZE;
  
  error = EIB_InitDataFields(ed);
  if(error != EIB7_NoError)
  {
	  EIB_FreeThreadLock(&ed->threadlock);
	  EIB_FreeThreadLock(&ed->fifolock);
	  EIB_CloseSocket(&ed->conndata.tcpconn.sock);
	  free(ed);
	  *eib = 0;
	  return error;
  }

  ed->posdata_allign = POSDATA_ALLIGN_POLL;

  // config default data packet
  packet[0].region = EIB7_DR_Global;
  packet[0].items = EIB7_PDF_TriggerCounter;
  packet[1].region = EIB7_DR_Encoder1;
  packet[1].items = EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef | EIB7_PDF_Analog;
  packet[2].region = EIB7_DR_Encoder2;
  packet[2].items = EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef | EIB7_PDF_Analog;
  packet[3].region = EIB7_DR_Encoder3;
  packet[3].items = EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef | EIB7_PDF_Analog;
  packet[4].region = EIB7_DR_Encoder4;
  packet[4].items = EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef | EIB7_PDF_Analog;
  error = EIB_MakeDataField(packet, 5, ed);
  if(error != EIB7_NoError)
  {
	  EIB_FreeThreadLock(&ed->threadlock);
	  EIB_FreeThreadLock(&ed->fifolock);
	  EIB_CloseSocket(&ed->conndata.tcpconn.sock);
	  free(ed);
	  *eib = 0;
	  return error;
  }
  EIB_CalculateOffsets(ed);

  if(ident != NULL && len > 0)
    {
      error = EIB7GetVersion(*eib, ident, len, EIB7_FW_CurrentlyBooted);
      if(error != EIB7_NoError)
	{
	  EIB_FreeThreadLock(&ed->threadlock);
	  EIB_FreeThreadLock(&ed->fifolock);
	  EIB_CloseSocket(&ed->conndata.tcpconn.sock);
	  free(ed);
	  *eib = 0;
	  return error;
	}
    }
      
/* no interface version check */

  for(i = 0; i < NUM_AXIS_PER_EIB; i++)
    {
      ed->axis[i].eib = *eib;
      ed->axis[i].channel = i;
    }

  for(i = 0; i < NUM_PORTS_PER_EIB; i++)
    {
      ed->port[i].eib = *eib;
      ed->port[i].port = i & (NUM_INPUTS_PER_EIB-1);
      ed->port[i].output = i >= NUM_INPUTS_PER_EIB ? 1 : 0;
    }
  
	
  return EIB7_NoError;
}

EIB7_ERR EIB7Close(EIB7_HANDLE eib)
{
   EIB7_ERR error;
   EIB_Data *ed;

   error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
   if(error != EIB7_NoError)
   {
      return error;
   }

   if(ed->conntype != EIB_TCPConnection)
      return EIB7_InvalidHandle;

   EIB_AcquireThreadLock(&ed->threadlock);

   /* Stop the soft-realtime thread. We don't need to use the fifolock here,
   as we continue only when the thread has ended. */
   if(ed->threadrunning)
   {
      ed->threadendflag = 1;

      /*
      Deliberately ignore the errors here
      NOTE: we need to release the thread lock here,
      to avoid deadlock.
      */
      EIB_ReleaseThreadLock(&ed->threadlock);
      EIB_WaitThread(&ed->thread);
      EIB_AcquireThreadLock(&ed->threadlock);

      ed->threadendflag = 0;
      ed->threadrunning = 0;
      ed->threadtype = EIB_THREAD_NONE;

      if(ed->fifo.data)
         EIB_DestroyFIFO(&ed->fifo);

      if(ed->master == FALSE)
         EIBCmd_RequestMaster(ed);

      //EIBCmd_EnDisSoftRT(ed, EIB7_OM_Polling);
      EIBCmd_SelectMode(ed, EIB7_OM_Polling);

      EIBCmd_ReleaseMaster(ed);
   }

   error = EIBCmd_CloseLink(ed);

   if(error == EIB7_ConnReset)
      error = EIB7_NoError;

   /* close all posibly opened sockets in any case */
   EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
   EIB_CloseSocket(&ed->conndata.tcpconn.tcpdatasocket);

   if(ed->conndata.tcpconn.status != EIB7_CS_Closed)
   {
      EIB_CloseSocket(&ed->conndata.tcpconn.sock);
   }

   EIB_FreeHandle(eib);
   EIB_ReleaseThreadLock(&ed->threadlock);
   EIB_FreeThreadLock(&ed->fifolock);
   EIB_FreeThreadLock(&ed->threadlock);
   EIB_DeleteDataField(&(ed->pkt_head));
   free(ed);

   return error;
}

EIB7_ERR EIB7SetTimeout(EIB7_HANDLE eib, long timeout)
{
  int error;
  EIB7_ERR err;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  err = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(err != EIB7_NoError)
    {
      return err;
    }
  
  if(ed->conntype != EIB_TCPConnection)
  {
	  return EIB7_IllegalParameter;
  }

  EIB_AcquireThreadLock(&ed->threadlock);

  if(timeout == 0)
    timeout = TCP_DEFAULT_TIMEOUT;
  if(timeout < TCP_MIN_TIMEOUT)
    timeout = TCP_MIN_TIMEOUT;
    
  error = EIB_SetSocketTimeout(ed->conndata.tcpconn.sock, timeout);

  EIB_ReleaseThreadLock(&ed->threadlock);

  if(error)
	  return EIB7_IllegalParameter;

  return EIB7_NoError;
}

EIB7_ERR EIB7TestConnection(EIB7_HANDLE eib, unsigned char *src, unsigned char *dest, unsigned long len, unsigned long *rlen)
{
  EIB7_ERR error;
  EIB_Data *ed;
  void *cmddata;
  int rcvlen;
  
  /* initialize return values */
  *rlen = 0;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  /* call test connection command */ 
  error = CmdIF_InitializeCommand(ed, EIBCMD_TestConnection, &cmddata);
  if(error != EIB7_NoError)
    {
      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  error = CmdIF_SetParameter(ed, cmddata, 0, src, len);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  error = CmdIF_ExecuteCommand(ed, cmddata, &rcvlen);
  *rlen = rcvlen;
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  error = CmdIF_GetParameter(ed, cmddata, 0, dest, *rlen, 0);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  CmdIF_FreeCommand(ed, cmddata);
  EIB_ReleaseThreadLock(&ed->threadlock);
  return EIB7_NoError;
}

EIB7_ERR EIB7GetConnectionStatus(EIB7_HANDLE eib, EIB7_CONN_STATUS* status)
{
  EIB7_ERR error;
  EIB_Data *ed;

  /* resolve handle */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(ed->conntype != EIB_TCPConnection)
    return EIB7_InvalidHandle;
  
  *status = ed->conndata.tcpconn.status;

  return EIB7_NoError;
}


EIB7_ERR EIB7GetNumOfAxes(EIB7_HANDLE eib, unsigned long* dsub, unsigned long* res1, 
                          unsigned long* res2, unsigned long* res3)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned long m12;
  unsigned long reserved2, reserved3;


  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetNumOfAxes(ed, dsub, &m12, &reserved2, &reserved3);
  *res1 = 0;   /* currently we do not support this parameter */
  *res2 = 0;   /* currently we do not support this parameter */
  *res3 = 0;   /* currently we do not support this parameter */
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7GetAxis(EIB7_HANDLE eib, EIB7_AXIS* set, unsigned long size, unsigned long* len)
{
  EIB7_ERR error;
  EIB_Data *ed;
  EIB_Axis *axis;
  unsigned long i;
  
  *len = 0;

  if(size > NUM_AXIS_PER_EIB)
    size = NUM_AXIS_PER_EIB;
  
  /* resolve handle */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  for(i = 0; i < size; i++)
    {
      axis = &ed->axis[i];

      error = EIB_ReserveChildHandle(eib, &set[i], axis);

      if(error != EIB7_NoError)
	{
	  unsigned long j;
	  
	  for(j = 0; j < i; j++)
	    {
	      EIB_FreeHandle(set[j]);
	    }

	  return EIB7_OutOfMemory;
	}
    }

  *len = i;

  return EIB7_NoError;
  
}

EIB7_ERR EIB7GetIO(EIB7_HANDLE eib,
		   EIB7_IO* iset, unsigned long isize, unsigned long* ilen,
		   EIB7_IO* oset, unsigned long osize, unsigned long* olen)
{
  EIB7_ERR error;
  EIB_Data *ed;
  EIB_Port *port;
  unsigned long i;
  
  *ilen = 0;
  *olen = 0;

  isize = MIN(NUM_INPUTS_PER_EIB, isize);
  osize = MIN(NUM_PORTS_PER_EIB - NUM_INPUTS_PER_EIB, osize);
    
  /* resolve handle */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  for(i = 0; i < isize; i++)
    {
      port = &ed->port[i];

      error = EIB_ReserveChildHandle(eib, &iset[i], port);

      if(error != EIB7_NoError)
	{
	  unsigned long j;

	  for(j = 0; j < i; j++)
	    {
	      EIB_FreeHandle(iset[j]);
	    }

	  return EIB7_OutOfMemory;
	}
    }

  *ilen = i;

  for(i = 0; i < osize; i++)
    {
      port = &ed->port[i + NUM_INPUTS_PER_EIB];

      error = EIB_ReserveChildHandle(eib, &oset[i], port);

      if(error != EIB7_NoError)
	{
	  unsigned long j;

	  for(j = 0; j < i; j++)
	    {
	      EIB_FreeHandle(oset[j]);
	    }

	  for(j = 0; j < *ilen; j++)
	    {
	      EIB_FreeHandle(iset[j]);
	    }
	  *ilen = 0;
	  
	  return EIB7_OutOfMemory;
	}
    }

  *olen = i;

  return EIB7_NoError;
}

EIB7_ERR EIB7SelectMode(EIB7_HANDLE eib, EIB7_OPERATING_MODE mode)
{
  EIB7_ERR error;
  EIB_Data *ed;
  int was_master;
  unsigned long ip, port;

  RANGECHECK(mode, EIB7_OM_MINVAL, EIB7_OM_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  /* get master interface if it is not requested yet */
  was_master = ed->master;
  if(ed->master == FALSE)
  {
     error = EIBCmd_RequestMaster(ed);
     if(error != EIB7_NoError)
     {
        if(error == 1)
           error = EIB7_AccNotAllowed;

        EIB_ReleaseThreadLock(&ed->threadlock);
        return error;
     }
  }

  /* --------------------------------------------*/
  /* setup the driver                            */
  if(ed->conntype == EIB_TCPConnection)
  {
     switch(mode)
     {
     case EIB7_OM_SoftRealtime:

        if(ed->threadrunning)
        {
           ed->threadendflag = 1;

           /* deliberately ignore the errors here.
           We don't need to use the fifolock here,
           as we continue only when the thread has ended.
           NOTE: we need to release the thread lock here,
           to avoid deadlock.
           */
           EIB_ReleaseThreadLock(&ed->threadlock);
           EIB_WaitThread(&ed->thread);
           EIB_AcquireThreadLock(&ed->threadlock);

           ed->threadendflag = 0;
           ed->threadrunning = 0;
           ed->threadtype = EIB_THREAD_NONE;
        }

        /* free memory if FIFO already exists */
        if(ed->fifo.data)
           EIB_DestroyFIFO(&ed->fifo);
        /* set up the data FIFO */
        error = EIB_CreateFIFO(&ed->fifo, ed->fifosize, ed->posdatasize_raw);
        if(error != EIB7_NoError)
        {
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        /* initialize the UDP socket, and set the destination IP */
        EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
        error = EIB_OpenUDP(ed->conndata.tcpconn.sock,
                           &ed->conndata.tcpconn.udpsock, &ip, &port);
        if(error != EIB7_NoError)
        {
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        error = EIBCmd_SetIPForSoftRT(ed, ip, port);
        if(error != EIB7_NoError)
        {
           EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        /* start our thread */
        ed->threadendflag = 0;
        ed->threadrunning = 0;
        ed->posdata_allign = POSDATA_ALLIGN_SRT;  /* the position data record can only be multiple of 2 bytes long */

        error = EIB_StartThread(EIB_SoftRealtimeHandlerUDP, ed, &ed->thread);
        if(error != EIB7_NoError)
        {
           EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        ed->threadrunning = 1;
        ed->threadtype = EIB_THREAD_SRT;

        error = EIBCmd_SelectMode(ed, mode);
        if(error != EIB7_NoError)
        {
           if(!was_master)
           {
              /*
              Release the master interface if the change mode was not successful
              and we were not master in the beginning
              */
              EIBCmd_ReleaseMaster(ed);
           }

           ed->threadendflag = 1;

           /* deliberately ignore the errors here.
           We don't need to use the fifolock here,
           as we continue only when the thread has ended.
           NOTE: we need to release the thread lock here,
           to avoid deadlock.
           */
           EIB_ReleaseThreadLock(&ed->threadlock);
           EIB_WaitThread(&ed->thread);
           EIB_AcquireThreadLock(&ed->threadlock);

           ed->threadendflag = 0;
           ed->threadrunning = 0;
           ed->threadtype = EIB_THREAD_NONE;
           EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        break;

     case EIB7_OM_Streaming:

        if(ed->threadrunning)
        {
           ed->threadendflag = 1;

           /* deliberately ignore the errors here.
           We don't need to use the fifolock here,
           as we continue only when the thread has ended.
           NOTE: we need to release the thread lock here,
           to avoid deadlock.
           */
           EIB_ReleaseThreadLock(&ed->threadlock);
           EIB_WaitThread(&ed->thread);
           EIB_AcquireThreadLock(&ed->threadlock);

           ed->threadendflag = 0;
           ed->threadrunning = 0;
           ed->threadtype = EIB_THREAD_NONE;
        }

        /* free memory if FIFO already exists */
        if(ed->fifo.data)
           EIB_DestroyFIFO(&ed->fifo);
        /* set up the data FIFO */
        error = EIB_CreateFIFO(&ed->fifo, ed->fifosize, ed->posdatasize_raw);
        if(error != EIB7_NoError)
        {
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        /* initialize the UDP socket, and set the destination IP */
        EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
        port = STREAMING_DEFAULT_PORT;
        error = EIB_OpenUDPDataSocket(ed->conndata.tcpconn.sock, port, &ed->conndata.tcpconn.udpsock);
        if(error != EIB7_NoError)
        {
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        error = EIBCmd_SelectMode(ed, mode);
        if(error != EIB7_NoError)
        {
           if(!was_master)
           {
              /*
              Release the master interface if the change mode was not successful
              and we were not master in the beginning
              */
              EIBCmd_ReleaseMaster(ed);
           }

           EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        /* start our thread */
        ed->threadendflag = 0;
        ed->threadrunning = 0;
        ed->posdata_allign = POSDATA_ALLIGN_STRM;  /* the position data record can only be multiple of 4 bytes long */

        error = EIB_StartThread(EIB_StreamingHandlerUDP, ed, &ed->thread);
        if(error != EIB7_NoError)
        {
           EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        ed->threadrunning = 1;
        ed->threadtype = EIB_THREAD_STRM;

        break;

     case EIB7_OM_RecordingSingle:     /* fallthrough */
     case EIB7_OM_RecordingRoll:       /* fallthrough */
     case EIB7_OM_Polling:

        if(ed->threadrunning)
        {
           ed->threadendflag = 1;

           /* deliberately ignore the errors here.
           We don't need to use the fifolock here,
           as we continue only when the thread has ended.
           NOTE: we need to release the thread lock here,
           to avoid deadlock.
           */
           EIB_ReleaseThreadLock(&ed->threadlock);
           EIB_WaitThread(&ed->thread);
           EIB_AcquireThreadLock(&ed->threadlock);

           ed->threadendflag = 0;
           ed->threadrunning = 0;
           ed->threadtype = EIB_THREAD_NONE;
        }
        if(ed->fifo.data)
           EIB_DestroyFIFO(&ed->fifo);

        ed->posdata_allign = POSDATA_ALLIGN_POLL;     /* no restrictions to the length of the position data record */

        error = EIBCmd_SelectMode(ed, mode);
        if(error != EIB7_NoError)
        {
           if(!was_master)
           {
              /*
              Release the master interface if the change mode was not successful
              and we were not master in the beginning
              */
              EIBCmd_ReleaseMaster(ed);
           }

           EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);
           EIB_ReleaseThreadLock(&ed->threadlock);
           return error;
        }

        EIB_CloseSocket(&ed->conndata.tcpconn.udpsock);

        if(mode == EIB7_OM_Polling)
        {
           EIBCmd_ReleaseMaster(ed);
        }

        break;

     default:
        if(!was_master)
        {
           /*
           Release the master interface if the change mode was not successful
           and we were not master in the beginning
           */
           EIBCmd_ReleaseMaster(ed);
        }

        EIB_ReleaseThreadLock(&ed->threadlock);
        return EIB7_ParamInvalid;
        break;
     } /* end switch() */

  } /* end if() */


  EIB_ReleaseThreadLock(&ed->threadlock);

  return EIB7_NoError;
}


EIB7_ERR EIB7AddDataPacketSection(EIB7_DataPacketSection* packet, unsigned long index, 
                                                   EIB7_DataRegion region, unsigned long items)
{
   if(packet == NULL)
      return EIB7_IllegalParameter;

   packet[index].region = region;
   packet[index].items = items;

   return EIB7_NoError;
}


EIB7_ERR EIB7ConfigDataPacket(EIB7_HANDLE eib, EIB7_DataPacketSection* packet, unsigned long size)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned char* data;
  int length;
  int was_master;
  unsigned long i;
  unsigned long regionmask;

   if(packet == NULL)
      return EIB7_IllegalParameter;

  /* check packet */
  /* the global region must be the first region in the packet */
  /* it is not allowed to add the same region multiple times */
  /* it is not allowed to add empty regions */
   regionmask = 0;
   if(size > 32)
      return EIB7_InvalidPacket;

   for(i=0; i<size; i++)
   {
      if(packet[i].region == EIB7_DR_Global)
      {
         if(i > 0)
            return EIB7_InvalidPacket;
      }
      if(regionmask & ((unsigned long)0x01 << packet[i].region))
         return EIB7_InvalidPacket;
      
      regionmask |= ((unsigned long)0x01 << packet[i].region);

      if(packet[i].items == 0)
         return EIB7_InvalidPacket;
   }


  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  /* we request the master interface to change the packet configuration */
  /* the new configuration applies only after changing the operation mode */
  was_master = ed->master;
  if(!was_master)
  {
     error = EIBCmd_RequestMaster(ed);
     if(error != EIB7_NoError)
     {
        EIB_ReleaseThreadLock(&ed->threadlock);
        return error;
     }
  }

  if(ed->threadrunning)
    {
       if(!was_master)
          EIBCmd_ReleaseMaster(ed);

      EIB_ReleaseThreadLock(&ed->threadlock);
      return EIB7_AccNotAllowed;
    }

  /* store data packet configuration local (handle pointer) for data processing */
  EIB_DeleteDataField(&(ed->pkt_head));
  error = EIB_MakeDataField(packet, size, ed);
  if(error != EIB7_NoError)
    {
      if(!was_master)
         EIBCmd_ReleaseMaster(ed);

      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  EIB_CalculateOffsets(ed);


  /* convert the data packet configuration and send it to the EIB */
  length = EIB_GetPacketConfigLength(&(ed->pkt_head));
  data = (unsigned char*)calloc(length, 1);
  if(data == NULL)
    {
      if(!was_master)
         EIBCmd_ReleaseMaster(ed);

      EIB_ReleaseThreadLock(&ed->threadlock);
      return EIB7_OutOfMemory;
    }
  if((error = EIB_ConvertPacketConfiguration(&(ed->pkt_head), data, length)) < 0)
    {
      if(!was_master)
         EIBCmd_ReleaseMaster(ed);

      EIB_ReleaseThreadLock(&ed->threadlock);
      free(data);
      if(error == -2)
         return EIB7_InvalidPacket;

      return EIB7_OutOfMemory;
    }
  error = EIBCmd_ConfigDataPacket(ed, data, length);
  free(data);
  if(error != EIB7_NoError)
    {
      if(!was_master)
         EIBCmd_ReleaseMaster(ed);

      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  if(!was_master)
     EIBCmd_ReleaseMaster(ed);

  EIB_ReleaseThreadLock(&ed->threadlock);
  return EIB7_NoError;
}


EIB7_ERR EIB7GetRecordingMemSize(EIB7_HANDLE eib, unsigned long* size)
{
   EIB7_ERR error;
   EIB_Data *ed;
   unsigned long mem = 0;
   unsigned long samplesize;

   /* resolve handle and get thread lock */
   error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
   if(error != EIB7_NoError)
   {
      return error;
   }

   EIB_AcquireThreadLock(&ed->threadlock);

   error = EIBCmd_GetRecMemSize(ed, &mem);
   if(error != EIB7_NoError)
   {
      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
   }

   if(ed->posdatasize_raw <= 0)
   {
      EIB_ReleaseThreadLock(&ed->threadlock);
      return EIB7_InvalidPacket;
   }

   samplesize = ed->posdatasize_raw + (ed->posdatasize_raw % POSDATA_ALLIGN_REC);
   *size = mem / samplesize;

   EIB_ReleaseThreadLock(&ed->threadlock);
   return EIB7_NoError;
}


EIB7_ERR EIB7SetNetwork(EIB7_HANDLE eib, unsigned long ip, unsigned long netmask, unsigned long gateway,
			EIB7_MODE dhcp, unsigned long timeout)
{
  EIB7_ERR error;
  EIB_Data *ed;

  RANGECHECK(dhcp, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetCustomNetwork(ed, ip, netmask, gateway, dhcp);
  if(error != EIB7_NoError)
    {
      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  error = EIBCmd_SetDHCPTimeout(ed, MAX(1,timeout));
  if(error != EIB7_NoError)
    {
      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  EIB_ReleaseThreadLock(&ed->threadlock);
  return EIB7_NoError;
}

EIB7_ERR EIB7GetNetwork(EIB7_HANDLE eib, unsigned long *ip, unsigned long *netmask, unsigned long *gateway,
			EIB7_MODE *dhcp)
{
  EIB7_ERR error;
  EIB_Data *ed;
  int dhcpflag;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetCustomNetwork(ed, ip, netmask, gateway, &dhcpflag);
  *dhcp = (dhcpflag != 0) ? EIB7_MD_Enable : EIB7_MD_Disable;
  
  if(error != EIB7_NoError)
    {
      EIB_ReleaseThreadLock(&ed->threadlock);
      return error;
    }

  EIB_ReleaseThreadLock(&ed->threadlock);
  return EIB7_NoError;
}

EIB7_ERR EIB7SetHostname(EIB7_HANDLE eib, const char *hostname)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  if(hostname && hostname[0] != 0)
    {
      error = EIBCmd_SetHostname(ed, hostname);
    }
  else
    {
      error = EIBCmd_SetDefaultHostname(ed);
    }

  EIB_ReleaseThreadLock(&ed->threadlock);
  return error;
}


EIB7_ERR EIB7GetHostname(EIB7_HANDLE eib, char *hostname, unsigned long len)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetHostname(ed, hostname, len);

  EIB_ReleaseThreadLock(&ed->threadlock);
  return error;
}

#define BUFFERSIZE 1024

EIB7_ERR EIB7GetSerialNumber(EIB7_HANDLE eib, char *serial, unsigned long len)
{
  EIB7_ERR error;
  EIB_Data *ed;
  char buffer[BUFFERSIZE];
  
   if(serial == NULL)
     return EIB7_IllegalParameter;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetSerialNumber(ed, buffer, BUFFERSIZE);

  EIB_ReleaseThreadLock(&ed->threadlock);

  if(error == EIB7_NoError)
    {
      char *sernum;

      /* the serial number follows the ident number directly */
      sernum = &buffer[strlen(buffer) + 1];

      if(strlen(sernum) > (len - 1))
	return EIB7_BufferTooSmall;

      #pragma warning( suppress : 4996)
      strncpy(serial, sernum, len);
      serial[len-1] = 0;

    }
      
  return error;
}

EIB7_ERR EIB7GetIdentNumber(EIB7_HANDLE eib, char *ident, unsigned long len)
{
  EIB7_ERR error;
  EIB_Data *ed;
  char buffer[BUFFERSIZE];
  
  if(ident == NULL)
     return EIB7_IllegalParameter;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetSerialNumber(ed, buffer, BUFFERSIZE);

  EIB_ReleaseThreadLock(&ed->threadlock);

  if(error == EIB7_NoError)
    {
      char *sernum;

      /* the ident str is the first part of the response */
      sernum = &buffer[0];

      if(strlen(sernum) > len - 1)
	return EIB7_BufferTooSmall;

      #pragma warning( suppress : 4996)
      strcpy(ident, sernum);
    }
      
  return error;
}

EIB7_ERR EIB7GetMAC(EIB7_HANDLE eib, unsigned char* mac)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetMAC(ed, mac);

  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetVersion(EIB7_HANDLE eib, char * ident, unsigned long len, EIB7_FIRMWARE select)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned long number;
  unsigned int num_i;
  int firmware;

  RANGECHECK(select, EIB7_FW_MINVAL, EIB7_FW_MAXVAL);

  if(ident == NULL)
     return EIB7_IllegalParameter;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  if(select == EIB7_FW_CurrentlyBooted)
    {
      int bootmode;
      
      error = EIBCmd_GetBootMode(ed, &bootmode);
      if(error != EIB7_NoError)
	{
	  EIB_ReleaseThreadLock(&ed->threadlock);
	  return error;
	}
      switch(bootmode)
	{
	case EIB7_BM_User:
	  firmware = 1;
	  break;
	default:
	  firmware = 0;
	}
    }
  else
    if(select == EIB7_FW_User)
      firmware = 1;
    else
      firmware = 0;
  
  error = EIBCmd_GetVersion(ed, firmware, &number);

  if(error == EIB7_NoError)
    {
    	if(number == 1)
    	{
         #pragma warning( suppress : 4996)
    		snprintf(ident, len, "63328101");
         ident[len-1] = 0;
    	}
    	else
    	{
         #pragma warning( suppress : 4996)
         num_i = number & 0xFFFFFFFF;
    		snprintf(ident, len, "%08d", num_i);
         ident[len-1] = 0;
    	}
    }
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetInterfaceVersion(EIB7_HANDLE eib, unsigned long *version)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetInterfaceVersion(ed, version);

  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}
  

EIB7_ERR EIB7GetBootMode(EIB7_HANDLE eib, EIB7_BOOT_MODE *mode)
{
  EIB7_ERR error;
  EIB_Data *ed;
  int bm;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetBootMode(ed, &bm);
  *mode = bm;
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7UpdateState(EIB7_HANDLE eib, EIB7_UPDATE_STATUS *status)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned long err;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetUpdateError(ed, &err);
  *status = err;
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetNumberOfOpenConnections(EIB7_HANDLE eib, unsigned long * cnt)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetConnectionInfo(ed, NULL, 0, cnt);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ConnectionInfo(EIB7_HANDLE eib, EIB7_CONN_INFO* info,
			    unsigned long size, unsigned long * cnt)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetConnectionInfo(ed, info, size, cnt);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7TerminateConnection(EIB7_HANDLE eib, unsigned long id)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_TerminateConnection(ed, id);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetTimestampTicks(EIB7_HANDLE eib, unsigned long * ticks)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetTimestampTicks(ed, ticks);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7SetTimestampPeriod(EIB7_HANDLE eib, unsigned long period)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetTimestampPeriod(ed, period);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ResetTimestamp(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ResetTimestamp(ed);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetTimerTriggerTicks(EIB7_HANDLE eib, unsigned long * ticks)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetTimerTriggerTicks(ed, ticks);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7SetTimerTriggerPeriod(EIB7_HANDLE eib, unsigned long period)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetTimerTriggerPeriod(ed, period);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7SetIntervalCounterTrigger(EIB7_HANDLE eib, EIB7_IntervalCounterMode mode, 
                                       EIB7_IntervalCounterStart start, unsigned long startpos, 
                                       unsigned long period)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_ICM_MINVAL, EIB7_ICM_MAXVAL);
  RANGECHECK(start, EIB7_ICS_MINVAL, EIB7_ICS_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetICTrigger(ed, mode, start, startpos, period);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7SetIntervalCounterInterpolation(EIB7_HANDLE eib, EIB7_IntervalCounterIPF ipf, 
                                             EIB7_IntervalCounterEdge edge)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  RANGECHECK(ipf, EIB7_ICF_MINVAL, EIB7_ICF_MAXVAL);
  RANGECHECK(edge, EIB7_ICE_MINVAL, EIB7_ICE_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetICInterpolation(ed, ipf, edge);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7OutputTriggerSource(EIB7_IO io, EIB7_OutputTriggerSrc src)
{
  EIB7_ERR error;
  EIB_Port *iod;
  EIB_Data *ed;

  RANGECHECK(src, EIB7_OT_MINVAL, EIB7_OT_MAXVAL);
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(io, (void **)&iod);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(iod->output == 0)
     return EIB7_InvalidHandle;

  error = EIB_RetrieveHandlePtr(iod->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetOutputTriggerSrc(ed, iod->port, src);

  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AxisTriggerSource(EIB7_AXIS axis, EIB7_AxisTriggerSrc src)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  RANGECHECK(src, EIB7_AT_MINVAL, EIB7_AT_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetAxisTriggerSrc(ed, ad->channel, src);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxAxisTriggerSource(EIB7_HANDLE eib, EIB7_AxisTriggerSrc src)
{
  EIB7_ERR error;
  EIB_Data *ed;

  RANGECHECK(src, EIB7_AT_MINVAL, EIB7_AT_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetAuxAxisTriggerSrc(ed, src);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7MasterTriggerSource(EIB7_HANDLE eib, EIB7_AxisTriggerSrc src)
{
  EIB7_ERR error;
  EIB_Data *ed;

  RANGECHECK(src, EIB7_AT_MINVAL, EIB7_AT_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetMasterTriggerSrc(ed, src);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7GlobalTriggerEnable(EIB7_HANDLE eib, EIB7_MODE mode, unsigned long src)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned long enable, disable, actual;

  RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  if(mode == EIB7_MD_Enable)
  {
     disable = 0;
     enable = src;
  }
  else
  {
     enable = 0;
     disable = src;
  }

  error = EIBCmd_SetTriggerSource(ed, enable, disable, &actual);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7ConfigPulsCounter(EIB7_HANDLE eib, EIB7_PulsCounterStart start, 
                               EIB7_PulsCounterTrigger trigger, unsigned long count)
{
  EIB7_ERR error;
  EIB_Data *ed;

  RANGECHECK(start, EIB7_PS_MINVAL, EIB7_PS_MAXVAL);
  RANGECHECK(trigger, EIB7_PT_MINVAL, EIB7_PT_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ConfigPulsCounter(ed, start, trigger, count);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7GetTriggerDelayTicks(EIB7_HANDLE eib, unsigned long* ticks)
{
  EIB7_ERR error;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetTriggerDelayTicks(ed, ticks);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7SetTriggerInputDelay(EIB7_IO io, unsigned long dly)
{
  EIB7_ERR error;
  EIB_Port *iod;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(io, (void **)&iod);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(iod->output != 0)
     return EIB7_InvalidHandle;

  error = EIB_RetrieveHandlePtr(iod->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetTriggerInDelay(ed, iod->port, dly);

  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7SetRITriggerEdge(EIB7_AXIS axis, EIB7_RITriggerEdge edge)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  RANGECHECK(edge, EIB7_RI_MINVAL, EIB7_RI_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetRITriggerEdge(ed, ad->channel, edge);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxSetRITriggerEdge(EIB7_HANDLE eib, EIB7_RITriggerEdge edge)
{
  EIB7_ERR error;
  EIB_Data *ed;

  RANGECHECK(edge, EIB7_RI_MINVAL, EIB7_RI_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  /* use channel 9 for auxiliary axis */
  error = EIBCmd_SetRITriggerEdge(ed, 9, edge);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7ResetTriggerCounter(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ResetTriggerCounter(ed);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnableIncrementalTermination(EIB7_HANDLE eib, EIB7_MODE mode)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnableIncrementalTermination(ed, mode);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}
  

EIB7_ERR EIB7SoftwareTrigger(EIB7_HANDLE eib, unsigned long source)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  RANGECHECK(source, EIB7_ST_MINVAL, EIB7_ST_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SoftwareTrigger(ed, source);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7Reset(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_Reset(ed);

  switch(ed->conntype)
    {
    case EIB_TCPConnection:
      EIB_CloseSocket(&ed->conndata.tcpconn.sock);
      ed->conndata.tcpconn.status = EIB7_CS_Closed;
      break;
    }
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  if(error==EIB7_NoError)
  {
    error = EIB7Close(eib);
    if(error==EIB7_SendError)
    {
    	error = EIB7_NoError;
    }
  }
  
  return error;
}
  
EIB7_ERR EIB7Identify(EIB7_HANDLE eib, EIB7_MODE mode)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_Identify(ed, mode);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ReadFIFODataRaw(EIB7_HANDLE eib, void* data, unsigned long cnt,
			     unsigned long* entries, long timeout)
{
  EIB7_ERR error;
  EIB_Data *ed;
  int num;
  EIB_TIME starttime;
  
  *entries = 0;

  EIB_GetStartTime(&starttime);
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  #pragma warning( suppress : 4127)
  while(TRUE)
    {
      EIB_AcquireThreadLock(&ed->fifolock);
      num = EIB_GetFIFONumElements(&ed->fifo);

      if(num == 0)
	{
	  EIB_ReleaseThreadLock(&ed->fifolock);

	  if(timeout == 0 ||
	     (timeout > 0 && EIB_TimeElapsed(&starttime) > timeout))
	    {
	      return EIB7_NoError; 
	    }

	  EIB_Sleep(1);
	}
      else
	{
	  int i;
	  
	  num = MIN((unsigned long)num, cnt);

	  for(i = 0; i < num; i++)
	    {
	      error = EIB_RetrieveFIFO(&ed->fifo, &((unsigned char*)data)[i * ed->posdatasize_raw],
									ed->posdatasize_raw);
	      if(error)
		{
		  EIB_ReleaseThreadLock(&ed->fifolock);
		  *entries = i;

		  return error;
		}
	    }

	  EIB_ReleaseThreadLock(&ed->fifolock);
	  *entries = i;
	  return EIB7_NoError;
	}
    }

  return EIB7_NoError;
}


EIB7_ERR EIB7TransferRecordingData(EIB7_HANDLE eib, EIB7_MODE mode, unsigned long offset, unsigned long length)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned long ip_addr, port;
  
  RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  /* it is allowed to 
     - enable the data transfer if currently no thread is running
     - disable the data transfer if the EIB_THREAD_REC is running
     */
  if((ed->threadrunning > 0) & (mode == EIB7_MD_Enable))
  {
     EIB_ReleaseThreadLock(&ed->threadlock);
     return EIB7_OpModeBlocked;
  }
  if((mode == EIB7_MD_Disable) & (ed->threadtype != EIB_THREAD_REC))
  {
     EIB_ReleaseThreadLock(&ed->threadlock);
     return EIB7_OpModeBlocked;
  }

  /* start/stop data transfer */
  error = EIBCmd_GetRecordingData(ed, mode, offset, length, &ip_addr, &port);
  if(error != EIB7_NoError)
  {
     EIB_ReleaseThreadLock(&ed->threadlock);
     return error;
  }


  if(mode == EIB7_MD_Disable)
  {
     if(ed->threadrunning)
     {
        ed->threadendflag = 1;

        /* deliberately ignore the errors here.
        We don't need to use the fifolock here,
        as we continue only when the thread has ended.
        NOTE: we need to release the thread lock here,
        to avoid deadlock.
        */
        EIB_ReleaseThreadLock(&ed->threadlock);
        EIB_WaitThread(&ed->thread);
        EIB_AcquireThreadLock(&ed->threadlock);

        ed->threadendflag = 0;
        ed->threadrunning = 0;
        ed->threadtype = EIB_THREAD_NONE;
     }
     if(ed->fifo.data)
        EIB_DestroyFIFO(&ed->fifo);

     /* close the TCP socket for data transfer */
     EIB_CloseSocket(&ed->conndata.tcpconn.tcpdatasocket);

     ed->posdata_allign = POSDATA_ALLIGN_POLL;  /* no restrictions to the length of the position data record */
  }
  else
  {
     /* EIB7_MD_Enable */
     /* free memory if FIFO already exists */
     if(ed->fifo.data)
        EIB_DestroyFIFO(&ed->fifo);
     /* set up the data FIFO */
     error = EIB_CreateFIFO(&ed->fifo, ed->fifosize, ed->posdatasize_raw);
     if(error != EIB7_NoError)
     {
        EIB_ReleaseThreadLock(&ed->threadlock);
        return error;
     }

     if(error != EIB7_NoError)
     {
        EIB_ReleaseThreadLock(&ed->threadlock);
        return error;
     }

     /* start our thread */
     ed->threadendflag = 0;
     ed->threadrunning = 0;

     error = EIB_StartThread(EIB_RecordingHandlerTCP, ed, &ed->thread);
     if(error != EIB7_NoError)
     {
        EIB_DestroyFIFO(&ed->fifo);
        EIB_ReleaseThreadLock(&ed->threadlock);
        return error;
     }

     ed->threadrunning = 1;
     ed->threadtype = EIB_THREAD_REC;
     ed->posdata_allign = POSDATA_ALLIGN_REC;  /* the position data can only be multiple of 4 bytes long */

     /* open TCP connection to the EIB */
     error = EIB_OpenTCPDataSocket(ip_addr, port, &ed->conndata.tcpconn.tcpdatasocket);
     if(error != EIB7_NoError)
     {
        ed->threadendflag = 1;

        /* deliberately ignore the errors here.
        We don't need to use the fifolock here,
        as we continue only when the thread has ended.
        NOTE: we need to release the thread lock here,
        to avoid deadlock.
        */
        EIB_ReleaseThreadLock(&ed->threadlock);
        EIB_WaitThread(&ed->thread);
        EIB_AcquireThreadLock(&ed->threadlock);

        ed->threadendflag = 0;
        ed->threadrunning = 0;
        ed->threadtype = EIB_THREAD_NONE;

        EIB_DestroyFIFO(&ed->fifo);
        EIB_ReleaseThreadLock(&ed->threadlock);
        return error;
     }
  }

  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7GetRecordingStatus(EIB7_HANDLE eib, unsigned long* length, unsigned long* status, unsigned long* progress)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned long samplesize;
  unsigned long memlength;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetRecordingStatus(ed, &memlength, status, progress);
  

  if(ed->posdatasize_raw <= 0)
  {
      EIB_ReleaseThreadLock(&ed->threadlock);
      return EIB7_InvalidPacket;
  }

  samplesize = ed->posdatasize_raw + (ed->posdatasize_raw % POSDATA_ALLIGN_REC);
  *length = memlength / samplesize;

  EIB_ReleaseThreadLock(&ed->threadlock);
  return error;
}


EIB7_ERR EIB7GetStreamingStatus(EIB7_HANDLE eib, unsigned long* length, unsigned long* max, unsigned long* size)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetStreamingStatus(ed, length, max, size);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7ReadFIFOData(EIB7_HANDLE eib, void* data, unsigned long cnt,
                          unsigned long* entries, long timeout)
{
   EIB7_ERR error;
   EIB_Data *ed;
   int num;
   EIB_TIME starttime;

   *entries = 0;

   EIB_GetStartTime(&starttime);

   /* resolve handle and get thread lock */
   error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
   if(error != EIB7_NoError)
   {
      return error;
   }

   #pragma warning( suppress : 4127)
   while(TRUE)
   {
      EIB_AcquireThreadLock(&ed->fifolock);
      num = EIB_GetFIFONumElements(&ed->fifo);

      if(num == 0)
      {
         EIB_ReleaseThreadLock(&ed->fifolock);

         if(timeout == 0 ||
            (timeout > 0 && EIB_TimeElapsed(&starttime) > timeout))
         {
            return EIB7_NoError;
         }

         EIB_Sleep(1);
      }
      else
      {
         int i;

         num = MIN((unsigned long)num, cnt);

         for(i = 0; i < num; i++)
         {
            void *element;

            error = EIB_GetElementPtrFIFO(&ed->fifo, &element);

            if(error)
            {
               EIB_ReleaseThreadLock(&ed->fifolock);
               *entries = i;

               return error;
            }

            EIB_ConvertPositionData(ed, element, &((unsigned char*)data)[i * ed->posdatasize]);

            EIB_RemoveElementFIFO(&ed->fifo);
         }

         EIB_ReleaseThreadLock(&ed->fifolock);
         *entries = i;
         return EIB7_NoError;
      }
   }

   return EIB7_NoError;
}

EIB7_ERR EIB7SizeOfFIFOEntry(EIB7_HANDLE eib, unsigned long* size)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  *size = ed->posdatasize;

  return EIB7_NoError;
}

EIB7_ERR EIB7SizeOfFIFOEntryRaw(EIB7_HANDLE eib, unsigned long* size)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  *size = ed->posdatasize_raw;

  return EIB7_NoError;
}

EIB7_ERR EIB7FIFOEntryCount(EIB7_HANDLE eib, unsigned long* cnt)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(ed->threadrunning)
    {
      EIB_AcquireThreadLock(&ed->fifolock);
      *cnt = EIB_GetFIFONumElements(&ed->fifo);
      EIB_ReleaseThreadLock(&ed->fifolock);
    }
  else 
    *cnt = 0;
  
  return EIB7_NoError;
}

EIB7_ERR EIB7ClearFIFO(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(ed->threadrunning)
    {
      EIB_AcquireThreadLock(&ed->fifolock);
      EIB_ClearFIFO(&ed->fifo);
      EIB_ReleaseThreadLock(&ed->fifolock);
    }
  
  return EIB7_NoError;
}

EIB7_ERR EIB7GetFIFOSize(EIB7_HANDLE eib, unsigned long *size)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
	
  *size = 0;
  if(ed->threadrunning)
    {
      EIB_AcquireThreadLock(&ed->fifolock);
      EIB_GetFIFOSize(&ed->fifo, size);
      EIB_ReleaseThreadLock(&ed->fifolock);
    }
  else
    *size = ed->fifosize;
  
  return EIB7_NoError;
}

EIB7_ERR EIB7SetFIFOSize(EIB7_HANDLE eib, unsigned long size)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
	
  if(ed->threadrunning)
    return EIB7_SoftRTEn;

  ed->fifosize = MAX(size, FIFO_MIN_SIZE);
    
  return EIB7_NoError;
}

EIB7_ERR EIB7SetDataCallback(EIB7_HANDLE eib, void* data, EIB7_MODE activate, unsigned long threshold, EIB7OnDataAvailable handler)
{
  EIB7_ERR error;
  EIB_Data *ed;

  RANGECHECK(activate, 0, 1);
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  if(activate)
    {
      ed->callbackdata = data;
      ed->callback = handler;
    }
  else
    ed->callback = NULL;

  ed->threshold = MAX(threshold, 1);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


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
            EIB7_EnDatCalcTime calculation)
{
  EIB7_ERR error, intern_err;
  EIB_Axis *ad;
  EIB_Data *ed;
  unsigned long flags;
  EIB7_MODE power;
  EIB7_POWER_FAILURE power_err;

  RANGECHECK(iface, EIB7_IT_MINVAL, EIB7_IT_MAXVAL);
  RANGECHECK(type, EIB7_EC_MINVAL, EIB7_EC_MAXVAL); 
  RANGECHECK(homing, EIB7_HS_MINVAL, EIB7_HS_MAXVAL);
  RANGECHECK(limit, EIB7_LS_MINVAL, EIB7_LS_MAXVAL);
  RANGECHECK(refmarks, EIB7_RM_MINVAL, EIB7_RM_MAXVAL);
  RANGECHECK(comp, EIB7_CS_MINVAL, EIB7_CS_MAXVAL);
  RANGECHECK(bandwidth, EIB7_BW_MINVAL, EIB7_BW_MAXVAL);
  RANGECHECK(recovery, EIB7_RT_MINVAL, EIB7_RT_MAXVAL);
  RANGECHECK(calculation, EIB7_CT_MINVAL, EIB7_CT_MAXVAL);

 
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  /* check power supply status */
  error = EIBCmd_GetPowerSupplyStatus(ed, ad->channel, &power, &power_err);
  if(error != EIB7_NoError)
  {
    EIB_ReleaseThreadLock(&ed->threadlock);
    return error;
  }
  if((power != EIB7_MD_Enable) | (power_err != EIB7_PF_None))
  {
    EIB_ReleaseThreadLock(&ed->threadlock);
    return EIB7_EncPwrSuppErr;
  }

  /* init axis */
  flags = type == EIB7_EC_Linear ? 2 : 1;
  flags |= ((unsigned long)iface          << 8);
  flags |= ((unsigned long)limit          << 14);
  flags |= ((unsigned long)homing         << 15);
  flags |= ((unsigned long)refmarks       << 16);
  flags |= ((unsigned long)(comp ? 0 : 1) << 21);
  flags |= ((unsigned long)bandwidth      << 22);
  flags |= ((unsigned long)recovery       << 12);  /* for EIB interface version 1.2 this flag is masked out */
  flags |= ((unsigned long)calculation    << 13);  /* for EIB interface version 1.2 this flag is masked out */
  
  error = EIBCmd_InitAxis(ed, ad->channel, flags, linecounts, increment, EnDatclock);
  
  /* clear errors for this axis */
  if(error == EIB7_NoError)
  {
     intern_err = EIBCmd_ClearEnDatEncoderErrors(ed, ad->channel, CLR_ENCODER_ERRORS);
     if(intern_err == EIB7_InvInterface)
       EIBCmd_ClearIncrementalEncoderErrors(ed, ad->channel, CLR_ENCODER_ERRORS);

     EIBCmd_ClearRefLatchStatus(ed, ad->channel, CLR_REF_STATUS);
  }

  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7ClearCounter(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ClearCounter(ed, ad->channel);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}
  
EIB7_ERR EIB7GetPosition(EIB7_AXIS axis, unsigned short *status, ENCODER_POSITION *pos)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetPosition(ed, ad->channel, status, pos);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetUDPMaxBytes(EIB7_HANDLE eib, unsigned long *bytes)
{
  EIB7_ERR error;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  *bytes = ed->maxudp;
  ed->maxudp = 0;
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetEncoderData(EIB7_AXIS axis,
			    unsigned short* status,
			    ENCODER_POSITION* pos,
			    ENCODER_POSITION* ref1,
			    ENCODER_POSITION* ref2,
			    ENCODER_POSITION* refc,
			    unsigned long* timestamp,
			    unsigned short* counter,
			    unsigned short* adc00,
			    unsigned short* adc90)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetEncoderData(ed, ad->channel, status, pos, ref1, ref2, refc, timestamp, counter, adc00, adc90);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}
  
EIB7_ERR EIB7ClearPowerSupplyError(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ClearEnDatEncoderErrors(ed, ad->channel, CLR_POWERSUPPLY_BITS);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ClearLostTriggerError(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ClearEnDatEncoderErrors(ed, ad->channel, CLR_LOST_TRIGGER_ERROR);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ClearEncoderErrors(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ClearEnDatEncoderErrors(ed, ad->channel, CLR_ENCODER_ERRORS);
  if(error == EIB7_InvInterface)
    error = EIBCmd_ClearIncrementalEncoderErrors(ed, ad->channel, CLR_ENCODER_ERRORS);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ClearEnDatErrorMsg(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ClearEnDatEncoderErrors(ed, ad->channel, CLR_ENDAT_ERROR_MSG);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ClearRefLatched(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ClearRefLatchStatus(ed, ad->channel, CLR_REF_LATCHED);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ClearRefStatus(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ClearRefLatchStatus(ed, ad->channel, CLR_REF_STATUS);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7StartRef(EIB7_AXIS axis, EIB7_ReferencePosition ref)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  RANGECHECK(ref, EIB7_RP_MINVAL, EIB7_RP_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnableRefPosLatch(ed, ad->channel, ref);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7StopRef(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_DisableRefPosLatch(ed, ad->channel);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetRefActive(EIB7_AXIS axis, EIB7_MODE* active)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;
  unsigned long status;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetRefPosLatchActive(ed, ad->channel, &status);
  *active = (status != 0 ? EIB7_MD_Enable : EIB7_MD_Disable);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7SetRefCheck(EIB7_AXIS axis, EIB7_MODE mode, unsigned long limit)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetReferenceCheck(ed, ad->channel, mode, limit);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7EnDat21GetPosition(EIB7_AXIS axis, unsigned short *status, ENCODER_POSITION *pos)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat21GetPosition(ed, ad->channel, status, pos);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}
  
EIB7_ERR EIB7EnDat21SelectMemRange(EIB7_AXIS axis, unsigned char mrs)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat21SelectMemRange(ed, ad->channel, mrs);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat21WriteMem(EIB7_AXIS axis, unsigned char addr,  unsigned short data)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat21WriteMem(ed, ad->channel, addr, data);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat21ReadMem(EIB7_AXIS axis, unsigned char addr,  unsigned short *data)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat21ReadMem(ed, ad->channel, addr, data);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat21ResetEncoder(EIB7_AXIS axis)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat21ResetEncoder(ed, ad->channel);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat21ReadTestValue(EIB7_AXIS axis, unsigned long *high, unsigned long *low)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat21ReadTestValue(ed, ad->channel, high, low);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat21WriteTestCommand(EIB7_AXIS axis, unsigned char port)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat21WriteTestCommand(ed, ad->channel, port);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat22GetPosition(EIB7_AXIS axis, unsigned short *status, ENCODER_POSITION* pos,
				ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat22GetPosition(ed, ad->channel, status, pos, ai1, ai2);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat22SelectMemRange(EIB7_AXIS axis, unsigned short* status,
				   ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				   unsigned char mrs, unsigned char block)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat22SelectMemRange(ed, ad->channel, status, pos, ai1, ai2, mrs, block);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat22WriteMem(EIB7_AXIS axis, unsigned short* status,
			     ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
			     unsigned char addr, unsigned short data)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat22WriteMem(ed, ad->channel, status, pos, ai1, ai2, addr, data);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat22ReadMem(EIB7_AXIS axis, unsigned short* status,
			    ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
			    unsigned char addr)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat22ReadMem(ed, ad->channel, status, pos, ai1, ai2, addr);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7EnDat22WriteTestCommand(EIB7_AXIS axis, unsigned short* status,
				     ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				     unsigned char port)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat22WriteTestCommand(ed, ad->channel, status, pos, ai1, ai2, port);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7EnDat22SetAddInfo(EIB7_AXIS axis, unsigned long addinfo1, unsigned long addinfo2)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  RANGECHECK(addinfo1, EIB7_AI_MINVAL, EIB7_AI_MAXVAL);
  RANGECHECK(addinfo2, EIB7_AI_MINVAL, EIB7_AI_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat22SetAddInfo(ed, ad->channel, (addinfo1 | 0x40), (addinfo2 | 0x50));
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7EnDat22SetAddInfoCycle(EIB7_AXIS axis, EIB7_MODE mode, unsigned char* data, unsigned long len)
{
   EIB7_ERR error;
   EIB_Axis *ad;
   EIB_Data *ed;
   unsigned long enable;
   unsigned char* DataConverted;
   unsigned long i;

   RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);
   for(i=0; i<len; i++)
   {
      RANGECHECK(data[i], EIB7_AI_MINVAL, EIB7_AI_MAXVAL);
   }

   /* resolve handle and get thread lock */
   error = EIB_RetrieveHandlePtr(axis, (void *)&ad);
   if(error != EIB7_NoError)
   {
      return error;
   }
   error = EIB_RetrieveHandlePtr(ad->eib, (void *)&ed);
   if(error != EIB7_NoError)
   {
      return error;
   }

   DataConverted = (unsigned char *)calloc(len, 1);
   if(DataConverted == NULL)
      return EIB7_OutOfMemory;

   enable = (mode == EIB7_MD_Enable ? 1 : 0);

   for(i=0; i<len; i++)
   {
      DataConverted[i] = (0x40 | data[i]);
   }

   EIB_AcquireThreadLock(&ed->threadlock);

   error = EIBCmd_EnDat22SetAddInfoFIFO(ed, ad->channel, enable, DataConverted, len);

   EIB_ReleaseThreadLock(&ed->threadlock);

   free(DataConverted);

   return error;
}


EIB7_ERR EIB7EnDat22SetCmdCycle(EIB7_AXIS axis, EIB7_MODE mode, EIB7_EnDatCmd* cfg, unsigned long len)
{
   EIB7_ERR error;
   EIB_Axis *ad;
   EIB_Data *ed;
   unsigned long enable;
   unsigned long entries;
   unsigned char* rawData;
   unsigned long i;
   int idx;

   RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

   /* resolve handle and get thread lock */
   error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
   if(error != EIB7_NoError)
   {
      return error;
   }
   error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
   if(error != EIB7_NoError)
   {
      return error;
   }

   entries = len;
   len *= 4;
   rawData = (unsigned char *)calloc(len, 1);
   if(rawData == NULL)
      return EIB7_OutOfMemory;

   enable = (mode == EIB7_MD_Enable ? 1 : 0);

   idx = 0;

   for(i=0; i<entries; i++)
   {
      rawData[idx++] = cfg[i].mode;
      rawData[idx++] = cfg[i].addr;
      rawData[idx++] = cfg[i].data & 0xFF;
      rawData[idx++] = (cfg[i].data >> 8) & 0xFF;
   }

   EIB_AcquireThreadLock(&ed->threadlock);

   error = EIBCmd_EnDat22SetCmdFIFO(ed, ad->channel, enable, rawData, len, entries);

   EIB_ReleaseThreadLock(&ed->threadlock);

   free(rawData);

   return error;
}


EIB7_ERR EIB7EnDat22ErrorReset(EIB7_AXIS axis, unsigned short* status,
			       ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_EnDat22ErrorReset(ed, ad->channel, status, pos, ai1, ai2);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7ReadEnDatIncrPos(EIB7_AXIS axis, unsigned short* statusEnDat,
			      ENCODER_POSITION* posEnDat, unsigned short* statusIncr, ENCODER_POSITION* posIncr)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ReadEnDatIncrPos(ed, ad->channel, statusEnDat, posEnDat, statusIncr, posIncr);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7SetPowerSupply(EIB7_AXIS axis, EIB7_MODE mode)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetPowerSupply(ed, ad->channel, mode);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetPowerSupplyStatus(EIB7_AXIS axis, EIB7_MODE* power, EIB7_POWER_FAILURE* err)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_GetPowerSupplyStatus(ed, ad->channel, power, err);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7SetTimestamp(EIB7_AXIS axis, EIB7_MODE mode)
{
  EIB7_ERR error;
  EIB_Axis *ad;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(axis, (void **)&ad);
  if(error != EIB7_NoError)
    {
      return error;
    }
  error = EIB_RetrieveHandlePtr(ad->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_SetTimestamp(ed, ad->channel, mode);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7InitInput(EIB7_IO io, EIB7_IOMODE mode, EIB7_MODE termination)
{
  EIB7_ERR error;
  EIB_Port *iod;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_IOM_MINVAL, EIB7_IOM_MAXVAL);
  RANGECHECK(termination, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(io, (void **)&iod);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(iod->output)
    return EIB7_PortDirInv;
 
  error = EIB_RetrieveHandlePtr(iod->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_InitInput(ed, iod->port, mode + 1, termination);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7InitOutput(EIB7_IO io, EIB7_IOMODE mode, EIB7_MODE enable)
{
  EIB7_ERR error;
  EIB_Port *iod;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_IOM_MINVAL, EIB7_IOM_MAXVAL);
  RANGECHECK(enable, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(io, (void **)&iod);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(!iod->output)
    return EIB7_PortDirInv;
 
  error = EIB_RetrieveHandlePtr(iod->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_InitOutput(ed, iod->port, mode + 1, enable);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}
  
EIB7_ERR EIB7ReadIO(EIB7_IO io, EIB7_IOMODE* mode, unsigned long* level)
{
  EIB7_ERR error;
  EIB_Port *iod;
  EIB_Data *ed;
  unsigned long dummy;
  unsigned long md, lvl;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(io, (void **)&iod);
  if(error != EIB7_NoError)
    {
      return error;
    }

  error = EIB_RetrieveHandlePtr(iod->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  if(iod->output)
    error = EIBCmd_ReadOutputLevel(ed, iod->port, &md, &lvl, &dummy);
  else
    error = EIBCmd_ReadInputLevel(ed, iod->port, &md, &lvl, &dummy);

  *mode = md;
  *level = lvl;
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7WriteIO(EIB7_IO io, unsigned long level)
{
  EIB7_ERR error;
  EIB_Port *iod;
  EIB_Data *ed;

  RANGECHECK(level, 0, 1);
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(io, (void **)&iod);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(iod->output == 0)
     return EIB7_InvalidHandle;

  error = EIB_RetrieveHandlePtr(iod->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_WriteIO(ed, iod->port, level);

  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetInputConfig(EIB7_IO io, EIB7_IOMODE *mode, EIB7_MODE *termination)
{
  EIB7_ERR error;
  EIB_Port *iod;
  EIB_Data *ed;
  unsigned long md, term, dummy;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(io, (void **)&iod);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(iod->output)
    return EIB7_PortDirInv;
  
  error = EIB_RetrieveHandlePtr(iod->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ReadInputLevel(ed, iod->port, &md, &dummy, &term);

  *mode = md;
  *termination = term;
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetOutputConfig(EIB7_IO io, EIB7_IOMODE *mode, EIB7_MODE *enable)
{
  EIB7_ERR error;
  EIB_Port *iod;
  EIB_Data *ed;
  unsigned long md, en, dummy;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(io, (void **)&iod);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(!iod->output)
    return EIB7_PortDirInv;
  
  error = EIB_RetrieveHandlePtr(iod->eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_ReadOutputLevel(ed, iod->port, &md, &dummy, &en);

  *mode = md;
  *enable = en;
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}

EIB7_ERR EIB7GetDriverID(char* ident, unsigned long len)
{
   if(ident == NULL)
     return EIB7_IllegalParameter;

  if(strlen(DRIVER_ID_STRING) + 1 > len)
    return EIB7_BufferTooSmall;

  #pragma warning( suppress : 4996)
  strncpy(ident, DRIVER_ID_STRING, len);
  ident[len-1] = 0;

  return EIB7_NoError;
}

EIB7_ERR EIB7GetDriverVersion(unsigned long *version)
{
  *version = DRIVER_VERSION_NUMBER;

  return EIB7_NoError;
}

EIB7_ERR EIB7IncrPosToDouble(ENCODER_POSITION src, double* dest)
{
	double value;
	ENCODER_POSITION check;
	check = (src >> 43) & 0x1FFFFF;
	if((check != 0x0) && (check != 0x1FFFFF)) /* bit 63..43 of src all identical? */
	{
		*dest = 0;
		return EIB7_ParamInvalid;
	}
	value = (double)src;
	*dest = value / 4096.0;
	return EIB7_NoError;
}


EIB7_ERR EIB7AuxClearSignalErrors(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxClearAmpError(ed);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxClearLostTriggerError(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxClearLostTriggerError(ed);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxClearCounter(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxClearCounter(ed);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxClearRefStatus(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  /* resolve handle and get thread lock */
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }

  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxClearRefStatus(ed);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxGetRefActive(EIB7_HANDLE eib, EIB7_MODE* active)
{
  EIB7_ERR error;
  EIB_Data *ed;
  unsigned long act, val;
  
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxGetRefPosLatchActive(ed, &act, &val);
  *active = (act != 0 ? EIB7_MD_Enable : EIB7_MD_Disable);
  //*valid = (val != 0 ? EIB7_MD_Enable : EIB7_MD_Disable);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxStartRef(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxStartRef(ed);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxStopRef(EIB7_HANDLE eib)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxStopRef(ed);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxGetPosition(EIB7_HANDLE eib, unsigned short *status, ENCODER_POSITION *pos)
{
  EIB7_ERR error;
  EIB_Data *ed;

  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxGetPosition(ed, status, pos);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxGetEncoderData(EIB7_HANDLE eib,
			    unsigned short* status,
			    ENCODER_POSITION* pos,
			    ENCODER_POSITION* ref,
			    unsigned long* timestamp,
			    unsigned short* counter)
{
  EIB7_ERR error;
  EIB_Data *ed;

  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxGetEncoderData(ed, status, pos, ref, timestamp, counter);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


EIB7_ERR EIB7AuxSetTimestamp(EIB7_HANDLE eib, EIB7_MODE mode)
{
  EIB7_ERR error;
  EIB_Data *ed;
  
  RANGECHECK(mode, EIB7_MD_MINVAL, EIB7_MD_MAXVAL);

  error = EIB_RetrieveHandlePtr(eib, (void **)&ed);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  EIB_AcquireThreadLock(&ed->threadlock);

  error = EIBCmd_AuxSetTimestamp(ed, mode);
  
  EIB_ReleaseThreadLock(&ed->threadlock);

  return error;
}


