
/*!
  \file eib7_cmd.c
  \brief This file implements the command helper routines for the EIB 7 driver DLL.
*/

#include <stdlib.h>
#include <string.h>
#include "eib7.h"
#include "eib7_cmd.h"
#include "definitions.h"
#include "commands.h"
#include "threading.h"
#include "handles.h"

#ifndef TRUE
#define TRUE (1)
#define FALSE (0)
#endif

/* A small helper macro to check for errors in repetitive calls */
#define CHECKERROR if(error == EIB7_NoError) error =

#define MIN(a,b) (a < b ? a : b)

EIB7_ERR EIBCmd_RequestMaster(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 1;
 
  error = CmdIF_InitializeCommand(ed, EIBCMD_RequestMaster, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  if(status == 0)
    ed->master = TRUE;
  else
    {
      return EIB7_AccNotAllowed;
    }

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_ReleaseMaster(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 1;
 
  error = CmdIF_InitializeCommand(ed, EIBCMD_ReleaseMaster, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  if(status == 0)
    ed->master = FALSE;
  else
    {
      return EIB7_AccNotAllowed;
    }

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_EnDisSoftRT(EIB_Data *ed, int mode)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 1;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDisableSoftRT, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  /*if(status == 0)
    ed->master = FALSE;
  else
    {
      return EIB7_AccNotAllowed;
    } */

  if(status != 0)
      return EIB7_AccNotAllowed;

  return EIB7_NoError;
}


EIB7_ERR EIBCmd_SelectMode(EIB_Data *ed, int mode)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SelectMode, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  if(error == EIB7_FuncNotSupp)
  {
     error = EIB7_NoError;
     error = CmdIF_InitializeCommand(ed, EIBCMD_EnDisableSoftRT, &cmddata);
     if(error != EIB7_NoError)
     {
        return error;
     }
     CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode, sizeof(mode));
     CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
     CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  }
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  if(status != 0)
      return status;

  return EIB7_NoError;
}


EIB7_ERR EIBCmd_ConfigDataPacket(EIB_Data* ed, unsigned char* data, unsigned long length)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ConfigDataPacket, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, data, length);
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  return status;
}

EIB7_ERR EIBCmd_GetRecMemSize(EIB_Data* ed, unsigned long* size)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 0;
  unsigned long memsize = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetRecMemSize, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, &memsize, sizeof(memsize), 0);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  *size = memsize;

  return status;
}

EIB7_ERR EIBCmd_CloseLink(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_CloseLink, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);

  CmdIF_FreeCommand(ed, cmddata);

  return error;
}


EIB7_ERR EIBCmd_SetIPForSoftRT(EIB_Data *ed, unsigned long ip, unsigned long port)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetDestIPforSoftRT, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &ip, sizeof(ip));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &port, sizeof(port));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  switch(status)
    {
    case 0:
      return EIB7_NoError;
    case 1:
      /* marked as 'reserved'... */
      return EIB7_InvalidResponse;
    case 2:
      return EIB7_DestIPUnreach;
    case 3:
      return EIB7_BroadcastInv;
    case 4:
      return EIB7_PortNoInv;
    case 5:
      return EIB7_SoftRTEn;
    default:
      return EIB7_InvalidResponse;
    }

  return EIB7_NoError;
}



EIB7_ERR EIBCmd_SetCustomNetwork(EIB_Data *ed, unsigned long ip, unsigned long netmask,
				 unsigned long gateway, int dhcp)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetCustomNetwork, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &ip, sizeof(ip)); 
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &netmask, sizeof(netmask)); 
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 2, &gateway, sizeof(gateway)); 
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 3, &dhcp, sizeof(dhcp)); 
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  if(status == 0)
    return EIB7_NoError;
  else
    return EIB7_ParamInvalid;
}

EIB7_ERR EIBCmd_SetDHCPTimeout(EIB_Data *ed, unsigned long timeout)
{
  EIB7_ERR error;
  void *cmddata;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetDHCPTimeout, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &timeout, sizeof(timeout)); 
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  if(status == 0)
    return EIB7_NoError;
  else
    return EIB7_ParamInvalid;
}

EIB7_ERR EIBCmd_GetCustomNetwork(EIB_Data *ed, unsigned long *ip, unsigned long *netmask,
				 unsigned long *gateway, int *dhcp)
{
  EIB7_ERR error;
  void *cmddata;
  int dhcp_t;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetCustomNetwork, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, ip, sizeof(*ip), 0); 
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, netmask, sizeof(*netmask), 0); 
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 2, gateway, sizeof(*gateway), 0); 
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 3, &dhcp_t, sizeof(dhcp_t),0 ); 
  *dhcp = dhcp_t & 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  if(*dhcp != 0xff)
    return EIB7_NoError;
  else
    return EIB7_NoCustNetwork;
}

EIB7_ERR EIBCmd_GetHostname(EIB_Data *ed, char *hostname, unsigned long len)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;

  len &= 0xFFFF;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetHostname, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  if(resplen < 0)
     return EIB7_CantRdHostname;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, hostname, MIN(len, (unsigned long)resplen), 0); 
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }

  /* make sure the result is properly terminated */
  if(len < (unsigned long)resplen)
    hostname[resplen - 1] = 0;
  
  CmdIF_FreeCommand(ed, cmddata);
  
  if(hostname[0] != 0)
    return EIB7_NoError;
  else
    return EIB7_CantRdHostname;
}

EIB7_ERR EIBCmd_SetHostname(EIB_Data *ed, const char *hostname)
{
  EIB7_ERR error;
  void *cmddata;
  int status = 0;
  size_t stringlen;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetHostname, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  stringlen = strlen(hostname) + 1;

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, (void *)hostname, MIN((stringlen & 0xFFFF), 32)); 
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, NULL);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0); 
  status &= 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  switch(status)
    {
    case 0:
      return EIB7_NoError;
    case 1:
      return EIB7_HostnameTooLong;
    case 2:
      return EIB7_HostnameInvalid;
    }
  return EIB7_CantSaveHostn;
}

EIB7_ERR EIBCmd_SetDefaultHostname(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_RestoreDefHostname, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, NULL);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0); 
  status &= 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);
  
  if(status == 0)
    return EIB7_NoError;
  else
    return EIB7_CantSaveHostn;
}

EIB7_ERR EIBCmd_GetSerialNumber(EIB_Data *ed, char *buffer, int size)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetSerialNumber, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, buffer, MIN(resplen, size), 0); 
  buffer[size - 2] = 0; /* Terminate properly in any case! */
  buffer[size - 1] = 0; /* Terminate properly in any case! */
  
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_GetMAC(EIB_Data *ed, unsigned char *mac)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetMACAddress, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, mac, MIN(resplen, 6), 0); 
  
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_GetVersion(EIB_Data *ed, int firmware, unsigned long *version)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetVersion, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &firmware, sizeof(firmware));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, version, sizeof(*version), 0); 
  
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_GetInterfaceVersion(EIB_Data *ed, unsigned long *version)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetInterfaceVersion, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, version, sizeof(*version), 0); 
  
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_GetBootMode(EIB_Data *ed, int *bootmode)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  int bootm;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetBootMode, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &bootm, sizeof(bootm), 0); 
  *bootmode = bootm & 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_GetUpdateError(EIB_Data *ed, unsigned long *status)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetUpdateError, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, status, sizeof(*status), 0); 
  *status &= 0xFF;
  
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

#define MAXCONN 8
#define CONNINFOSZ 13
#define MAXCONNINFOSZ (MAXCONN * CONNINFOSZ)

EIB7_ERR EIBCmd_GetConnectionInfo(EIB_Data *ed, EIB7_CONN_INFO *info,
				  unsigned long size, unsigned long *cnt)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long i;
    
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetConnectionInfo, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  *cnt = MIN(MAXCONNINFOSZ, resplen) / CONNINFOSZ;

  for(i = 0; i < MIN(*cnt, size); i++)
    {
      unsigned long val;
      
      CmdIF_GetParameter(ed, cmddata, 0, &val, sizeof(val), i * CONNINFOSZ);
      info[i].id = val & 0xFF;
      CmdIF_GetParameter(ed, cmddata, 1, &val, sizeof(val), i * CONNINFOSZ);
      info[i].local_port = val & 0xFFFF;
      CmdIF_GetParameter(ed, cmddata, 2, &val, sizeof(val), i * CONNINFOSZ);
      info[i].local_ip = val;
      CmdIF_GetParameter(ed, cmddata, 3, &val, sizeof(val), i * CONNINFOSZ);
      info[i].remote_port = val & 0xFFFF;
      CmdIF_GetParameter(ed, cmddata, 4, &val, sizeof(val), i * CONNINFOSZ);
      info[i].remote_ip = val;
    }

  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_EnableDisableKill(EIB_Data *ed, unsigned long en)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;

  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDisableKill, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &en, sizeof(en));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CmdIF_FreeCommand(ed, cmddata);

  return error;
}

EIB7_ERR EIBCmd_TerminateConnection(EIB_Data *ed, unsigned long id)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  

  error = EIBCmd_EnableDisableKill(ed, 1);
  if(error != EIB7_NoError)
    return error;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_KillOther, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &id, sizeof(id));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0); 
  status &= 0xFF;

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  CmdIF_FreeCommand(ed, cmddata);

  error = EIBCmd_EnableDisableKill(ed, 0);
  if(error != EIB7_NoError)
      return error;

  switch(status)
    {
    case 0:
      return EIB7_NoError;
    case 1:
      return EIB7_AccNotAllowed;
    case 2:
      return EIB7_CantTermConn;
    case 3:
      return EIB7_CantTermSelf;
    default:
      return EIB7_ParamInvalid;
    }
  
}

EIB7_ERR EIBCmd_GetTimestampTicks(EIB_Data *ed, unsigned long * ticks)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;

  error = CmdIF_InitializeCommand(ed, EIBCMD_GetTimestampTicks, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, ticks, sizeof(*ticks), 0);
  *ticks &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  return error;
}

EIB7_ERR EIBCmd_SetTimestampPeriod(EIB_Data *ed, unsigned long period)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetTimestampPeriod, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &period, sizeof(period));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}

EIB7_ERR EIBCmd_ResetTimestamp(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;

  error = CmdIF_InitializeCommand(ed, EIBCMD_ResetTimestCount, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);

  CmdIF_FreeCommand(ed, cmddata);

  return error;
}

EIB7_ERR EIBCmd_GetTimerTriggerTicks(EIB_Data *ed, unsigned long * ticks)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;

  error = CmdIF_InitializeCommand(ed, EIBCMD_GetTriggerTicks, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, ticks, sizeof(*ticks), 0);
  *ticks &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  return error;
}

EIB7_ERR EIBCmd_SetTimerTriggerPeriod(EIB_Data *ed, unsigned long period)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetTriggerPeriod, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &period, sizeof(period));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}

EIB7_ERR EIBCmd_SetICTrigger(EIB_Data *ed, unsigned long mode, unsigned long start,
                                                            unsigned long startpos, unsigned long period)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetICTrigger, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode, sizeof(mode));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &start, sizeof(start));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 2, &startpos, sizeof(startpos));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 3, &period, sizeof(period));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}

EIB7_ERR EIBCmd_SetICInterpolation(EIB_Data *ed, unsigned long ipf, unsigned long edge)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetICInterpolation, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &ipf, sizeof(ipf));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &edge, sizeof(edge));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_SetOutputTriggerSrc(EIB_Data *ed, unsigned long channel, unsigned long src)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetOutputTriggerSrc, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  channel++;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &channel, sizeof(channel));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &src, sizeof(src));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}

EIB7_ERR EIBCmd_SetAxisTriggerSrc(EIB_Data *ed, unsigned long channel, unsigned long src)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetAxisTriggerSrc, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  channel++;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &channel, sizeof(channel));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &src, sizeof(src));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_SetAuxAxisTriggerSrc(EIB_Data *ed, unsigned long src)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetAuxAxisTriggerSrc, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &src, sizeof(src));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_SetMasterTriggerSrc(EIB_Data *ed, unsigned long src)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetMasterTriggerSrc, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &src, sizeof(src));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_SetTriggerSource(EIB_Data *ed, unsigned long enable, unsigned long disable, unsigned long* actual)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetTriggerSource, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &enable, sizeof(enable));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &disable, sizeof(disable));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, actual, sizeof(actual), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_ConfigPulsCounter(EIB_Data *ed, unsigned long start, unsigned long trigger,
                                  unsigned long count)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ConfigPulsCounter, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &start, sizeof(start));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &trigger, sizeof(trigger));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 2, &count, sizeof(count));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_GetTriggerDelayTicks(EIB_Data *ed, unsigned long* ticks)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long result = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetTriggerDelayTicks, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &result, sizeof(result), 0);
  *ticks = result & 0xFF;

  CmdIF_FreeCommand(ed, cmddata);
  
  return error;
}


EIB7_ERR EIBCmd_SetTriggerInDelay(EIB_Data *ed, unsigned long channel, unsigned long dly)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetTriggerInputDelay, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  channel++;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &channel, sizeof(channel));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &dly, sizeof(dly));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_SetRITriggerEdge(EIB_Data *ed, unsigned long channel, unsigned long edge)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetRITriggerEdge, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  channel++;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &channel, sizeof(channel));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &edge, sizeof(edge));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}








EIB7_ERR EIBCmd_EnableTimerTrigger(EIB_Data *ed, unsigned long mode)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 1;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDisTimerTrig, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ExtTrgEn;
  
  return error;
}

EIB7_ERR EIBCmd_ResetTriggerCounter(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;

  error = CmdIF_InitializeCommand(ed, EIBCMD_ResetTriggerCount, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);

  CmdIF_FreeCommand(ed, cmddata);

  return error;
}

EIB7_ERR EIBCmd_EnableIncrementalTermination(EIB_Data *ed, unsigned long mode)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDisIncrInpTerm, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_CantChIncInpTrm;
  
  return error;
}

EIB7_ERR EIBCmd_EnableExternalTrigger(EIB_Data *ed, unsigned long mode)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 1;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDisExtTrig, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_TimerTrgEn;
  
  return error;
}

EIB7_ERR EIBCmd_SoftwareTrigger(EIB_Data *ed, unsigned long source)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SoftwareTrigger, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &source, sizeof(source));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  switch(status)
    {
    case 0:
      return error;
    case 1: 
      return EIB7_TimerTrgEn;
    case 2:
      return EIB7_ExtTrgEn;
    }
  
  return error;
}

EIB7_ERR EIBCmd_Reset(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long reserved;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ResetEIB, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  reserved = 0x55;
  
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &reserved, sizeof(reserved));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);

  CmdIF_FreeCommand(ed, cmddata);

  return error;
}

EIB7_ERR EIBCmd_Identify(EIB_Data *ed, unsigned long mode)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_IdentifyEIB, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_IllegalParameter;
  
  return error;
}


EIB7_ERR EIBCmd_GetNumOfAxes(EIB_Data* ed, unsigned long* dsub, unsigned long* m12, 
                             unsigned long* res2, unsigned long* res3)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long numDsub, numM12, reserved2, reserved3;

  numDsub = numM12 = reserved2 = reserved3 = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetNumOfAxes, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }
  
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status, sizeof(status), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 1, &numDsub, sizeof(numDsub), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 2, &numM12, sizeof(numM12), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, &reserved2, sizeof(reserved2), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, &reserved3, sizeof(reserved3), 0);

  *dsub = numDsub & 0xFF;
  *m12 = numM12 = 0xFF;
  *res2 = reserved2 = 0xFF;
  *res3 = reserved3 = 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return status;
  
  return error;
}


EIB7_ERR EIBCmd_InitAxis(EIB_Data *ed,
			 unsigned long channel,
			 unsigned long flags,
			 unsigned long linecounts,
			 unsigned long increment,
			 unsigned long EnDatclock)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long index;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_InputConfig, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel+1;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,      sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &flags,      sizeof(flags));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &linecounts, sizeof(linecounts));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &increment,  sizeof(increment));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 4, &EnDatclock, sizeof(EnDatclock));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return status;
  
  return error;
}

#define MAKEMASK(channel) ((unsigned long)1 << channel)


EIB7_ERR EIBCmd_ClearCounter(EIB_Data *ed, unsigned long channel)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long mask;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ClrPosCounter, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  mask = MAKEMASK(channel);

  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &mask,    sizeof(mask));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_InvInterface;
  
  return error;
}


EIB7_ERR EIBCmd_GetPosition(EIB_Data *ed, unsigned long channel, unsigned short *status_ret, ENCODER_POSITION *pos)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long status = 0;
  unsigned long index;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetPosition, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel + 1;

  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,    sizeof(index));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      /* an error ocurred */
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);

      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
	return error;

      return status;
    }
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 1, &status,  sizeof(status), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 2, pos,  sizeof(*pos), 0);

  *status_ret = status & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);

  return error;
}

EIB7_ERR EIBCmd_GetEncoderData(EIB_Data *ed, unsigned long channel,
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
  void *cmddata;
  int resplen = 0;
  unsigned long statcode = 0;
  unsigned long data = 0;
  unsigned long index;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetEncChannelData, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel + 1;

  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,    sizeof(index));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 1)
    {
      /* an error ocurred */
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &statcode,  sizeof(statcode), 0);
      statcode &= 0xFF;
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      switch(statcode)
	{
	case 1:
	  return EIB7_InvInterface;
	case 2:
	  return EIB7_TimerTrgEn;
	case 3:
	  return EIB7_ExtTrgEn;
	case 4:
	  return EIB7_SoftRTEn;
	}

      return EIB7_InvalidResponse;
    }
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 1, counter,   sizeof(*counter), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 2, &data,     sizeof(data), 0); *status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, pos,       sizeof(*pos), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, timestamp, sizeof(*timestamp), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 5, ref1,      sizeof(*ref1), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 6, ref2,      sizeof(*ref2), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 7, refc,      sizeof(*refc), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 8, &data,     sizeof(data), 0); *adc00 = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 9, &data,     sizeof(data), 0); *adc90 = data & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);

  return error;
}


EIB7_ERR EIBCmd_ClearEnDatEncoderErrors(EIB_Data *ed, unsigned long channel, unsigned long mask)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long index;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ClrEnDatEncErrors, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel + 1;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &mask,    sizeof(mask));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  return status;
}

EIB7_ERR EIBCmd_ClearIncrementalEncoderErrors(EIB_Data *ed, unsigned long channel, unsigned long mask)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long index;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ClrIncrEncErrors, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel + 1;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &mask,    sizeof(mask));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  return status;
}

EIB7_ERR EIBCmd_ClearRefLatchStatus(EIB_Data *ed, unsigned long channel, unsigned long mask)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long index;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ClrRefLatchStatus, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel + 1;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &mask,    sizeof(mask));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  return status;
}

EIB7_ERR EIBCmd_EnableRefPosLatch(EIB_Data *ed, unsigned long channel, unsigned long ref)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long mask1, mask2;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnableRefPosLatch, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  switch(ref)
    {
    case 1:
      mask1 = 1 << channel;
      mask2 = 0;
      break;
    case 2:
      mask1 = 1 << channel;
      mask2 = mask1;
      break;
    };
  
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &mask1,   sizeof(mask1));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &mask2,   sizeof(mask2));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return EIB7_NoError;
}

EIB7_ERR EIBCmd_DisableRefPosLatch(EIB_Data *ed, unsigned long channel)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long mask;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_DisableRefPosLatch, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  mask = MAKEMASK(channel);
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &mask,   sizeof(mask));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return EIB7_NoError;
}

EIB7_ERR EIBCmd_GetRefPosLatchActive(EIB_Data *ed, unsigned long channel, unsigned long *active)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long mask;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetRefPosLatchStat, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  mask = MAKEMASK(channel);
  
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  *active = (mask & status) ? 1 : 0;
  
  return EIB7_NoError;
}


EIB7_ERR EIBCmd_SetReferenceCheck(EIB_Data *ed, unsigned long channel, unsigned long mode, unsigned long limit)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetReferenceCheck, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  channel++;

  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &channel,   sizeof(channel));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &mode,   sizeof(mode));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &limit,   sizeof(limit));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;
  
  return status;
}


EIB7_ERR EIBCmd_EnDat21GetPosition(EIB_Data *ed, unsigned long channel, unsigned short *status_ret, ENCODER_POSITION *pos)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 1;
  index = channel + 1;
  addr = 0;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  /* we use the EnDat22 response fields, as they match perfectly */
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, &status,  sizeof(status), 0); *status_ret = status & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, pos,      sizeof(*pos), 0);
  CmdIF_FreeCommand(ed, cmddata);
  
  return error;
}



EIB7_ERR EIBCmd_EnDat21SelectMemRange(EIB_Data *ed, unsigned long channel, unsigned char mrs)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 2;
  index = channel + 1;
  addr = mrs;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CmdIF_FreeCommand(ed, cmddata);
  
  return EIB7_NoError;
}

EIB7_ERR EIBCmd_EnDat21WriteMem(EIB_Data *ed, unsigned long channel, unsigned long addr,  unsigned long data)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 4;
  index = channel + 1;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CmdIF_FreeCommand(ed, cmddata);
  
  return EIB7_NoError;
}

EIB7_ERR EIBCmd_EnDat21ReadMem(EIB_Data *ed, unsigned long channel, unsigned long addr,  unsigned short *data)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index;
  unsigned long status = 0;
  unsigned long data_low = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 3;
  index = channel + 1;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 1, &data_low,  sizeof(data_low), 0);
  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;
  
  *data = data_low & 0xFFFF;
  
  return EIB7_NoError;
}

EIB7_ERR EIBCmd_EnDat21ResetEncoder(EIB_Data *ed, unsigned long channel)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 5;
  index = channel + 1;
  addr = 0;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_EnDat21ReadTestValue(EIB_Data *ed, unsigned long channel, unsigned long *high, unsigned long *low)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 6;
  index = channel + 1;
  addr = 0;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 1, low,   sizeof(*low), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 2, high,  sizeof(*high), 0);
  CmdIF_FreeCommand(ed, cmddata);

  return error;
}

EIB7_ERR EIBCmd_EnDat21WriteTestCommand(EIB_Data *ed, unsigned long channel, unsigned long port)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 7;
  index = channel + 1;
  addr = port;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CmdIF_FreeCommand(ed, cmddata);

  return EIB7_NoError;
}

EIB7_ERR EIBCmd_EnDat22GetPosition(EIB_Data *ed, unsigned long channel, unsigned short *status_ret,
				   ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 11;
  index = channel + 1;
  addr = 0;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, &status,     sizeof(status), 0); *status_ret = status & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, pos,         sizeof(*pos), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 5, &data,       sizeof(data), 0); ai1->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 6, &data,       sizeof(data), 0); ai1->info = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 7, &data,       sizeof(data), 0); ai2->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 8, &data,       sizeof(data), 0); ai2->info = data & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);
  
  return error;
}
  
EIB7_ERR EIBCmd_EnDat22SelectMemRange(EIB_Data *ed, unsigned long channel, unsigned short* status_ret,
				      ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				      unsigned char mrs, unsigned char block)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 12;
  index = channel + 1;
  addr = mrs;
  data = block;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, &status,     sizeof(status), 0); *status_ret = status & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, pos,         sizeof(*pos), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 5, &data,       sizeof(data), 0); ai1->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 6, &data,       sizeof(data), 0); ai1->info = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 7, &data,       sizeof(data), 0); ai2->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 8, &data,       sizeof(data), 0); ai2->info = data & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);
  
  return error;
}

EIB7_ERR EIBCmd_EnDat22WriteMem(EIB_Data *ed, unsigned long channel, unsigned short* status_ret,
				ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				unsigned char addr_in, unsigned short data_in)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 14;
  index = channel + 1;
  addr = addr_in;
  data = data_in;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, &status,     sizeof(status), 0); *status_ret = status & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, pos,         sizeof(*pos), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 5, &data,       sizeof(data), 0); ai1->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 6, &data,       sizeof(data), 0); ai1->info = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 7, &data,       sizeof(data), 0); ai2->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 8, &data,       sizeof(data), 0); ai2->info = data & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);
  
  return error;
}

EIB7_ERR EIBCmd_EnDat22ReadMem(EIB_Data *ed, unsigned long channel, unsigned short* status_ret,
				ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
				unsigned char addr_in)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 13;
  index = channel + 1;
  addr = addr_in;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, &status,     sizeof(status), 0); *status_ret = status & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, pos,         sizeof(*pos), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 5, &data,       sizeof(data), 0); ai1->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 6, &data,       sizeof(data), 0); ai1->info = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 7, &data,       sizeof(data), 0); ai2->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 8, &data,       sizeof(data), 0); ai2->info = data & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);
  
  return error;
}

EIB7_ERR EIBCmd_EnDat22WriteTestCommand(EIB_Data *ed, unsigned long channel, unsigned short* status_ret,
					ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2,
					unsigned char port)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 17;
  index = channel + 1;
  addr = port;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, &status,     sizeof(status), 0); *status_ret = status & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, pos,         sizeof(*pos), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 5, &data,       sizeof(data), 0); ai1->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 6, &data,       sizeof(data), 0); ai1->info = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 7, &data,       sizeof(data), 0); ai2->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 8, &data,       sizeof(data), 0); ai2->info = data & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);
  
  return error;
}

EIB7_ERR EIBCmd_EnDat22SetAddInfo(EIB_Data *ed, unsigned long channel, 
                                                unsigned long addinfo1, unsigned long addinfo2)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDat22SetAddInfo, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  channel += 1;      /* channel is 0 based */
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &channel,   sizeof(channel));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &addinfo1,     sizeof(addinfo1));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addinfo2,    sizeof(addinfo2));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
   CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
   CmdIF_FreeCommand(ed, cmddata);

   if(error != EIB7_NoError)
     return error;

   return status;
}

EIB7_ERR EIBCmd_EnDat22SetAddInfoFIFO(EIB_Data *ed, unsigned long channel, unsigned long mode,
                                                         unsigned char* data, unsigned long len)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDat22SetAddInfoFIFO, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  channel += 1;      /* channel is 0 based */

  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &channel, sizeof(channel));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &mode,    sizeof(mode));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &len,     sizeof(len));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, data,     len);
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
   CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
   CmdIF_FreeCommand(ed, cmddata);

   if(error != EIB7_NoError)
     return error;

   return status;
}


EIB7_ERR EIBCmd_EnDat22SetCmdFIFO(EIB_Data *ed, unsigned long channel, unsigned long mode,
                                  unsigned char* data, unsigned long len, unsigned long entries)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned char unused01 = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDat22SetCmdFIFO, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  channel += 1;      /* channel is 0 based */

  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &channel, sizeof(channel));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &mode,    sizeof(mode));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &entries,     sizeof(entries));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &unused01, sizeof(unused01));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 4, data,     len);
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
   CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
   CmdIF_FreeCommand(ed, cmddata);

   if(error != EIB7_NoError)
     return error;

   return status;
}


EIB7_ERR EIBCmd_EnDat22ErrorReset(EIB_Data *ed, unsigned long channel, unsigned short* status_ret,
				  ENCODER_POSITION* pos, ENDAT_ADDINFO* ai1, ENDAT_ADDINFO* ai2)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long cmd, index, addr, data;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDatModeCmd, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  cmd = 15;
  index = channel + 1;
  addr = 0;
  data = 0;
  
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 0, &index,   sizeof(index));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 1, &cmd,     sizeof(cmd));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 2, &addr,    sizeof(addr));
  CHECKERROR CmdIF_SetParameter  (ed, cmddata, 3, &data,    sizeof(data));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &status,  sizeof(status), 0);
      CmdIF_FreeCommand(ed, cmddata);

      if(error != EIB7_NoError)
        return error;
  
      return status;
    }

  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, &status,     sizeof(status), 0); *status_ret = status & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, pos,         sizeof(*pos), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 5, &data,       sizeof(data), 0); ai1->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 6, &data,       sizeof(data), 0); ai1->info = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 7, &data,       sizeof(data), 0); ai2->status = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 8, &data,       sizeof(data), 0); ai2->info = data & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);
  
  return error;
}


EIB7_ERR EIBCmd_ReadEnDatIncrPos(EIB_Data *ed, unsigned long channel,
				 unsigned short* statusEnDat, ENCODER_POSITION* posEnDat,
				 unsigned short* statusIncr,  ENCODER_POSITION* posIncr)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long index;
  unsigned long status = 0;
  unsigned long data = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetIncrAndAbsPos, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel + 1;

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);

  if(error != EIB7_NoError)
    {
      CmdIF_FreeCommand(ed, cmddata);
      return error;
    }
  
  if(resplen == 4)
    {
      CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

      CmdIF_FreeCommand(ed, cmddata);
      
      if(error != EIB7_NoError)
        return error;

      return status;
    }

  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, &data, sizeof(data), 0); *statusEnDat = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 2, posEnDat, sizeof(*posEnDat), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 3, &data, sizeof(data), 0); *statusIncr = data & 0xFFFF;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 4, posIncr, sizeof(*posIncr), 0);
  CmdIF_FreeCommand(ed, cmddata);
  
  if(error != EIB7_NoError)
    return error;

  return error;
}


EIB7_ERR EIBCmd_SetPowerSupply(EIB_Data *ed, unsigned long channel, unsigned long mode)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long index;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_SetEncSupply, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel + 1;

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return status;
  
  return error;
}

EIB7_ERR EIBCmd_GetPowerSupplyStatus(EIB_Data *ed, unsigned long channel, EIB7_MODE* power, EIB7_POWER_FAILURE* err)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetEncSupplyStatus, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  *power = status & MAKEMASK(channel) ? EIB7_MD_Enable : EIB7_MD_Disable;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, &status, sizeof(status), 0);
  *err   = status & MAKEMASK(channel) ? EIB7_PF_Overcurrent : EIB7_PF_None;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  return error;
}

EIB7_ERR EIBCmd_SetTimestamp(EIB_Data *ed, unsigned long channel, EIB7_MODE mode)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long index;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDisTimestamp, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = channel + 1;

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return status;
  
  return error;
}

EIB7_ERR EIBCmd_InitInput(EIB_Data *ed, unsigned long port, unsigned long mode, unsigned long termination)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long index;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_TriggerInputConf, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = port + 1;

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &mode, sizeof(mode));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 2, &termination, sizeof(termination));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return status;
  
  return error;
}

EIB7_ERR EIBCmd_InitOutput(EIB_Data *ed, unsigned long port, unsigned long mode, unsigned long enable)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long index;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_TriggerOutputConf, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = port + 1;

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &mode, sizeof(mode));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 2, &enable, sizeof(enable));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return status;
  
  return error;
}

EIB7_ERR EIBCmd_ReadInputLevel(EIB_Data *ed, unsigned long port,
			       unsigned long* mode, unsigned long* level, unsigned long *term)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long index;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ReadExtInputLevel, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  /* Set reserved parameter */
  index = 0;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  *mode  = MAKEMASK(port) & status ? 1 : 0;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, &status, sizeof(status), 0);
  *level = MAKEMASK(port) & status ? 1 : 0;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 2, &status, sizeof(status), 0);
  *term  = MAKEMASK(port) & status ? 1 : 0;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;
  
  return error;
}

EIB7_ERR EIBCmd_ReadOutputLevel(EIB_Data *ed, unsigned long port,
				unsigned long* mode, unsigned long* level, unsigned long *enable)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long index;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_ReadExtOutputLevel, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  /* Set reserved parameter */
  index = 0;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  *enable = MAKEMASK(port) & status ? 1 : 0;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, &status, sizeof(status), 0);
  *mode   = MAKEMASK(port) & status ? 1 : 0;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 2, &status, sizeof(status), 0);
  *level  = MAKEMASK(port) & status ? 1 : 0;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;
  
  return error;
}

EIB7_ERR EIBCmd_GetRecordingData(EIB_Data *ed, EIB7_MODE mode, unsigned long offset, unsigned long length,
                                 unsigned long* ip, unsigned long* port)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen = 0;
  unsigned long ip_addr = 0;
  unsigned long port_nr = 0;
  unsigned long status = 0;
  unsigned char mode_tmp;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_TransmitRecData, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  if(mode == EIB7_MD_Enable)
     mode_tmp = 1;
  else
     mode_tmp = 0;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mode_tmp, sizeof(mode_tmp));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &offset, sizeof(offset));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 2, &length, sizeof(length));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  if(resplen == 10)
  {
     CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, &ip_addr, sizeof(ip_addr), 0);
     CHECKERROR CmdIF_GetParameter(ed, cmddata, 2, &port_nr, sizeof(port_nr), 0);
  }
  else
  {
     ip_addr = 0;
     port_nr = 0;
  }

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
  {
     *ip = 0;
     *port = 0;
     return error;
  }

  if(status)
  {
     *ip = 0;
     *port = 0;
     return status;
  }

  *ip = ip_addr;
  *port = port_nr;
  
  return error;
}


EIB7_ERR EIBCmd_GetRecordingStatus(EIB_Data* ed, unsigned long* length, unsigned long* status, unsigned long* progress)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long stat = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetRecStatus, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &stat, sizeof(stat), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, length, sizeof(stat), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 2, status, sizeof(stat), 0);
  *status &= 0xFF;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 3, progress, sizeof(stat), 0);
  *progress &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(stat)
    return stat;
  
  return error;
}

EIB7_ERR EIBCmd_GetStreamingStatus(EIB_Data* ed, unsigned long* length, unsigned long* max, unsigned long* size)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long stat = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_GetStrmStatus, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &stat, sizeof(stat), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, length, sizeof(stat), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 2, max, sizeof(stat), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 3, size, sizeof(stat), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(stat)
    return stat;
  
  return error;
}

EIB7_ERR EIBCmd_WriteIO(EIB_Data *ed, unsigned long port, unsigned long level)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long index;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_WriteExtOutputLevel, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = port + 1;

  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &level, sizeof(level));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  status &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return status;
  
  return error;
}


EIB7_ERR EIBCmd_AuxClearAmpError(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long mask;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxClearAmpError, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  mask = 0x01;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mask, sizeof(mask));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_AuxClearLostTriggerError(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  unsigned long mask;
  
  // the command for clearing the lost trigger error is the same as for clearing the amplitude error
  // -> using: EIBCMD_AuxClearAmpError
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxClearAmpError, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  mask = 0x02;
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &mask, sizeof(mask));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_AuxClearCounter(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  // the command for clearing the lost trigger error is the same as for clearing the amplitude error
  // -> using: EIBCMD_AuxClearAmpError
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxClearCounter, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_AuxClearRefStatus(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  // the command for clearing the lost trigger error is the same as for clearing the amplitude error
  // -> using: EIBCMD_AuxClearAmpError
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxClearRefStatus, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_AuxGetRefPosLatchActive(EIB_Data *ed, unsigned long* active, unsigned long* valid)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  // the command for clearing the lost trigger error is the same as for clearing the amplitude error
  // -> using: EIBCMD_AuxClearAmpError
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxGetRefPosLatchStatus, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 1, active, sizeof(status), 0);
  *active &= 0xFF;
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 2, valid, sizeof(status), 0);
  *valid &= 0xFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_AuxStartRef(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxStartRef, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_AuxStopRef(EIB_Data *ed)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long status = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxStopRef, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return EIB7_ParamInvalid;
  
  return error;
}


EIB7_ERR EIBCmd_AuxGetPosition(EIB_Data *ed, unsigned short *status, ENCODER_POSITION *pos)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long statusword = 0;
  unsigned long cmdstatus = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxGetPosition, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &cmdstatus,  sizeof(cmdstatus), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 1, &statusword,  sizeof(statusword), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 2, pos,  sizeof(*pos), 0);

  *status = statusword & 0xFFFF;
  
  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(cmdstatus)
    return EIB7_ParamInvalid;

  return error;
}


EIB7_ERR EIBCmd_AuxGetEncoderData(EIB_Data *ed,
			       unsigned short* status,
			       ENCODER_POSITION* pos,
			       ENCODER_POSITION* ref,
			       unsigned long* timestamp,
			       unsigned short* counter)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long statcode = 0;
  unsigned long statusword = 0;
  
  error = CmdIF_InitializeCommand(ed, EIBCMD_AuxGetEncChannelData, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata,    &resplen);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 0, &statcode,   sizeof(statcode), 0);  
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 1, &statusword,  sizeof(statusword), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 2, pos,  sizeof(*pos), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 3, ref,  sizeof(*ref), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 4, timestamp, sizeof(*timestamp), 0);
  CHECKERROR CmdIF_GetParameter  (ed, cmddata, 5, counter,   sizeof(*counter), 0);
  //*counter = 0;
  
  *status = statusword & 0xFFFF;

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(statcode)
    return EIB7_ParamInvalid;


  return error;
}


EIB7_ERR EIBCmd_AuxSetTimestamp(EIB_Data *ed, EIB7_MODE mode)
{
  EIB7_ERR error;
  void *cmddata;
  int resplen;
  unsigned long index;
  unsigned long status = 0;
  
  // use the same command as for the timestamp for the axes
  error = CmdIF_InitializeCommand(ed, EIBCMD_EnDisTimestamp, &cmddata);
  if(error != EIB7_NoError)
    {
      return error;
    }

  index = 9;      // for auxiliary axis
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 0, &index, sizeof(index));
  CHECKERROR CmdIF_SetParameter(ed, cmddata, 1, &mode, sizeof(mode));
  CHECKERROR CmdIF_ExecuteCommand(ed, cmddata, &resplen);
  CHECKERROR CmdIF_GetParameter(ed, cmddata, 0, &status, sizeof(status), 0);

  CmdIF_FreeCommand(ed, cmddata);

  if(error != EIB7_NoError)
    return error;

  if(status)
    return status;
  
  return error;
}

