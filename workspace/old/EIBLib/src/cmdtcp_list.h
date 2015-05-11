

#ifndef __CMDTCP_LIST_H__
#define __CMDTCP_LIST_H__



/*!
  \file cmdtcp_list.h
  \brief this file declares the command interface structure and the command
  codes.

  These #defines have to correspond with the order of entries
  in the command lists for the following files:
    - cmdtcp_list.c
*/



/* This file is auto-generated from utils/generate_command_list.py. Please do not edit! */




#define EIBCMD_CloseLink  0   /*!<  */
#define EIBCMD_TestConnection  1   /*!<  */
#define EIBCMD_EnDisableKill  2   /*!<  */
#define EIBCMD_RequestMaster  3   /*!<  */
#define EIBCMD_ReleaseMaster  4   /*!<  */
#define EIBCMD_KillOther  5   /*!<  */
#define EIBCMD_GetConnectionInfo  6   /*!<  */
#define EIBCMD_GetCustomNetwork  7   /*!<  */
#define EIBCMD_SetCustomNetwork  8   /*!<  */
#define EIBCMD_GetHostname  9   /*!<  */
#define EIBCMD_SetHostname  10   /*!<  */
#define EIBCMD_RestoreDefHostname  11   /*!<  */
#define EIBCMD_SetDHCPTimeout  12   /*!<  */
#define EIBCMD_GetBootMode  13   /*!<  */
#define EIBCMD_GetVersion  14   /*!<  */
#define EIBCMD_GetSerialNumber  15   /*!<  */
#define EIBCMD_GetMACAddress  16   /*!<  */
#define EIBCMD_GetUpdateError  17   /*!<  */
#define EIBCMD_EnDisableSoftRT  18   /*!<  */
#define EIBCMD_SetDestIPforSoftRT  19   /*!<  */
#define EIBCMD_EnDisTimerTrig  20   /*!<  */
#define EIBCMD_SoftwareTrigger  21   /*!<  */
#define EIBCMD_GetTriggerTicks  22   /*!<  */
#define EIBCMD_SetTriggerPeriod  23   /*!<  */
#define EIBCMD_EnDisExtTrig  24   /*!<  */
#define EIBCMD_ResetTriggerCount  25   /*!<  */
#define EIBCMD_EnDisTrigOut  26   /*!<  */
#define EIBCMD_EnDisTrigTerm  27   /*!<  */
#define EIBCMD_TriggerInputConf  28   /*!<  */
#define EIBCMD_TriggerOutputConf  29   /*!<  */
#define EIBCMD_ReadExtInputLevel  30   /*!<  */
#define EIBCMD_ReadExtOutputLevel  31   /*!<  */
#define EIBCMD_WriteExtOutputLevel  32   /*!<  */
#define EIBCMD_EnDisTimestamp  33   /*!<  */
#define EIBCMD_GetTimestampTicks  34   /*!<  */
#define EIBCMD_SetTimestampPeriod  35   /*!<  */
#define EIBCMD_ResetTimestCount  36   /*!<  */
#define EIBCMD_ClrIncrEncErrors  37   /*!<  */
#define EIBCMD_ClrEnDatEncErrors  38   /*!<  */
#define EIBCMD_ClrRefLatchStatus  39   /*!<  */
#define EIBCMD_ClrAmplitudeError  40   /*!<  */
#define EIBCMD_ClrNoiseError  41   /*!<  */
#define EIBCMD_ClrTriggerError  42   /*!<  */
#define EIBCMD_ClrEncSupplyError  43   /*!<  */
#define EIBCMD_ClrRefPosLatched  44   /*!<  */
#define EIBCMD_ClrDCRefPosValid  45   /*!<  */
#define EIBCMD_ClrDCRefPosError  46   /*!<  */
#define EIBCMD_InputConfig  47   /*!<  */
#define EIBCMD_EnableRefPosLatch  48   /*!<  */
#define EIBCMD_DisableRefPosLatch  49   /*!<  */
#define EIBCMD_GetRefPosLatchStat  50   /*!<  */
#define EIBCMD_EnDisEncSupply  51   /*!<  */
#define EIBCMD_SetEncSupply  52   /*!<  */
#define EIBCMD_GetEncSupplyStatus  53   /*!<  */
#define EIBCMD_ClrPosCounter  54   /*!<  */
#define EIBCMD_EnDisCompensation  55   /*!<  */
#define EIBCMD_SetEncInputBandw  56   /*!<  */
#define EIBCMD_EnDisIncrInpTerm  57   /*!<  */
#define EIBCMD_GetEncChannelData  58   /*!<  */
#define EIBCMD_GetPosition  59   /*!<  */
#define EIBCMD_EnDatModeCmd  60   /*!<  */
#define EIBCMD_GetIncrAndAbsPos  61   /*!<  */
#define EIBCMD_ResetEIB  62   /*!<  */
#define EIBCMD_IdentifyEIB  63   /*!<  */
#define EIBCMD_GetInterfaceVersion  64   /*!<  */
#define EIBCMD_ConfigDataPacket  65   /*!<  */
#define EIBCMD_TransmitRecData  66   /*!<  */
#define EIBCMD_GetRecStatus  67   /*!<  */
#define EIBCMD_GetStrmStatus  68   /*!<  */
#define EIBCMD_SelectMode  69   /*!<  */
#define EIBCMD_EnDat22SetAddInfo  70   /*!<  */
#define EIBCMD_EnDat22SetAddInfoFIFO  71   /*!<  */
#define EIBCMD_GetRecMemSize  72   /*!<  */
#define EIBCMD_SetICTrigger  73   /*!<  */
#define EIBCMD_SetICInterpolation  74   /*!<  */
#define EIBCMD_SetOutputTriggerSrc  75   /*!<  */
#define EIBCMD_SetAxisTriggerSrc  76   /*!<  */
#define EIBCMD_SetAuxAxisTriggerSrc  77   /*!<  */
#define EIBCMD_SetMasterTriggerSrc  78   /*!<  */
#define EIBCMD_ConfigPulsCounter  79   /*!<  */
#define EIBCMD_SetTriggerInputDelay  80   /*!<  */
#define EIBCMD_SetRITriggerEdge  81   /*!<  */
#define EIBCMD_AuxClearAmpError  82   /*!<  */
#define EIBCMD_AuxClearCounter  83   /*!<  */
#define EIBCMD_AuxClearRefStatus  84   /*!<  */
#define EIBCMD_AuxGetRefPosLatchStatus  85   /*!<  */
#define EIBCMD_AuxStartRef  86   /*!<  */
#define EIBCMD_AuxStopRef  87   /*!<  */
#define EIBCMD_AuxGetPosition  88   /*!<  */
#define EIBCMD_AuxGetEncChannelData  89   /*!<  */
#define EIBCMD_GetTriggerDelayTicks  90   /*!<  */
#define EIBCMD_SetTriggerSource  91   /*!<  */
#define EIBCMD_GetNumOfAxes  92   /*!<  */
#define EIBCMD_EnDat22SetCmdFIFO  93   /*!<  */
#define EIBCMD_SetReferenceCheck  94   /*!<  */




#endif /*__CMDTCP_LIST_H__*/


