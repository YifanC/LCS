

/*! \file cmdtcp_list.c
    \brief This file includes a list of the network-based commands.
    
    This file implements the network-based command handling. All commands are
    defined as data, complete with all information needed to access the return values.
    This is done with the idea in mind to be able to switch to a different command
    mechanism (e.g. direct PCI-E bus interface) easily.
*/



/* This file is auto-generated from utils/generate_command_list.py. Please do not edit! */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "cmdtcp.h"
#include "commands.h"




/* Please keep in mind, that the commands in this array
   have to be in sync with the defines from commands.h!
*/
TCP_Command Commands[] = 
  {  
    {
      "CloseLink", /* name */
      0x64,        /* opcode */
      0,           /* numparams */
      { EMPTY_PARAM }, /* param */
      0,           /* numresultss */
      { EMPTY_PARAM } /* result */
    },
    {
      "TestConnection",    /* name */
      0x1f4,               /* opcode */
      1,                   /* numparams */
      {                    /* param */
	{ TCP_PT_Data, 0, -1, "TestData" }
      },
      1,                   /* numresults */
      {                    /* result */
	{ TCP_PT_Data, 0, -1, "TestData" }
      }
    },
    {
      "EnDisableKill",     /* name */
      0x7a,                /* opcode */
      1,                   /* numparams */
      {                    /* param */
	{ TCP_PT_U8, 0, 1, "Flag" }
      },
      0,                   /* numresults */
      { EMPTY_PARAM }          /* result */
    },
    {
      "RequestMaster",     /* name */
      0x81,                /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },         /* param */
      1,                   /* numresults */
      {                    /* result */
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "ReleaseMaster",     /* name */
      0x82,                /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },         /* param */
      1,                   /* numresults */
      {                    /* result */
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "KillOther",         /* name */
      0x79,                /* opcode */
      1,                   /* numparams */
      {                    /* param */
	{ TCP_PT_U8, 0, 1, "ID" }
      },
      1,                   /* numresults */
      {                    /* result */
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "GetConnectionInfo", /* name */
      0x78,                /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },         /* param */
      5,                   /* numresults */
      {                    /* result */
	{ TCP_PT_U8, 0, 1, "ID" },
	{ TCP_PT_U16, 1, 2, "LocalPort" },
	{ TCP_PT_U32, 3, 4, "LocalIP" },
	{ TCP_PT_U16, 7, 2, "RemotePort" },
	{ TCP_PT_U32, 9, 4, "RemoteIP" }
      }
    },
    {
      "GetCustomNetwork",  /* name */
      0x7b,                /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },         /* param */
      4,                   /* numresults */
      {                    /* result */
	{ TCP_PT_U32, 0, 4, "IP" },
	{ TCP_PT_U32, 4, 4, "Netmask" },
	{ TCP_PT_U32, 8, 4, "Gateway" },
	{ TCP_PT_U8, 12, 1, "DHCPFlag" }
      }
    },
    {
      "SetCustomNetwork",  /* name */
      0x7c,                /* opcode */
      4,                   /* numparams */
      {                    /* param */
	{ TCP_PT_U32, 0, 4, "IP" },
	{ TCP_PT_U32, 4, 4, "Netmask" },
	{ TCP_PT_U32, 8, 4, "Gateway" },
	{ TCP_PT_U8, 12, 1, "DHCPFlag" }
      },
      1,                   /* numresults */
      {                    /* result */
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "GetHostname",       /* name */
      0x7d,                /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_Data, 0, -1, "Hostname" }
      }
    },
    {
      "SetHostname",       /* name */
      0x7e,                /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_Data, 0, 32, "Hostname" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "RestoreDefHostname",/* name */
      0x7f,                /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "SetDHCPTimeout",    /* name */
      0x80,                /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Timeout" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "GetBootMode",       /* name */
      0x400,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "BootMode" }
      }
    },
    {
      "GetVersion",        /* name */
      0x401,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Mode" }
      },
      4,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Firmware" },
	{ TCP_PT_U32, 4, 4, "Reserved" },
	{ TCP_PT_U32, 8, 4, "Reserved" },
	{ TCP_PT_U32, 12,4, "Reserved" }
      }
    },
    {
      "GetSerialNumber",   /* name */
      0x402,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_Data, 0, -1, "Serials" }
      }
    },
    {
      "GetMACAddress",     /* name */
      0x403,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_Data, 0, 6, "MACAddress" }
      }
    },
    {
      "GetUpdateError",    /* name */
      0x408,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "State" }
      }
    },
    {
      "EnDisableSoftRT"  , /* name */
      0x3a0,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Active" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "SetDestIPforSoftRT",/* name */
      0x3a1,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U32, 0, 4, "IP" },
	{ TCP_PT_U16, 4, 2, "Port" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "EnDisTimerTrig"  ,  /* name */
      0x3b0,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Mode" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "SoftwareTrigger"  , /* name */
      0x3b5,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Source" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "GetTriggerTicks",   /* name */
      0x3b8,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Ticks" }
      }
    },
    {
      "SetTriggerPeriod",  /* name */
      0x3b1,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U32, 0, 4, "Period" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "EnDisExtTrig"  ,    /* name */
      0x3b2,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "ResetTriggerCount", /* name */
      0x3b3,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Reserved" }
      },
      0,                   /* numresults */
      { EMPTY_PARAM }
    },
    {
      "EnDisTrigOut"  ,    /* name */
      0x3b6,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "EnDisTrigTerm"  ,   /* name */
      0x3b7,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "TriggerInputConf",  /* name */
      0x3b9,               /* opcode */
      3,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Mode" },
	{ TCP_PT_U8, 2, 1, "Term" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "TriggerOutputConf", /* name */
      0x3ba,               /* opcode */
      3,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Mode" },
	{ TCP_PT_U8, 2, 1, "Enable" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "ReadExtInputLevel", /* name */
      0x3bb,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Reserved" }
      },
      4,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Mode" },
	{ TCP_PT_U8, 1, 1, "Level" },
	{ TCP_PT_U8, 2, 1, "Term" },
	{ TCP_PT_U8, 3, 1, "Reserved" }
      }
    },
    {
      "ReadExtOutputLevel",/* name */
      0x3bc,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Reserved" }
      },
      4,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Mode" },
	{ TCP_PT_U8, 1, 1, "Level" },
	{ TCP_PT_U8, 2, 1, "Term" },
	{ TCP_PT_U8, 3, 1, "Reserved" }
      }
    },
    {
      "WriteExtOutputLevel",/* name */
      0x3bd,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Level" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "EnDisTimestamp"  ,  /* name */
      0x3c0,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Mode" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "GetTimestampTicks", /* name */
      0x3c3,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Ticks" }
      }
    },
    {
      "SetTimestampPeriod",/* name */
      0x3c1,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U32, 0, 4, "Time" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "ResetTimestCount",  /* name */
      0x3c2,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      0,                   /* numresults */
      { EMPTY_PARAM }
    },
    {
      "ClrIncrEncErrors",  /* name */
      0x3e1,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Mask" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "ClrEnDatEncErrors", /* name */
      0x3e2,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Mask" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "ClrRefLatchStatus", /* name */
      0x3e3,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Mask" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "ClrAmplitudeError", /* name */
      0x3d1,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Errors" }
      }
    },
    {
      "ClrNoiseError",     /* name */
      0x3d3,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "ClrTriggerError",   /* name */
      0x3b4,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Error" }
      },
      0,                   /* numresults */
      { EMPTY_PARAM }
    },
    {
      "ClrEncSupplyError", /* name */
      0x3d8,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "ClrRefPosLatched",  /* name */
      0x3d4,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "ClrDCRefPosValid",  /* name */
      0x3dd,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "ClrDCRefPosError",  /* name */
      0x3de,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "InputConfig",       /* name */
      0x3f0,               /* opcode */
      5,                   /* numparams */
      {           
	{ TCP_PT_U8,  0, 1, "Channel" },
	{ TCP_PT_U24, 1, 3, "Flags" },
	{ TCP_PT_U32, 4, 4, "Channel" },
	{ TCP_PT_U16, 8, 2, "Channel" },
	{ TCP_PT_U32,10, 4, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "EnableRefPosLatch", /* name */
      0x3d5,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Ref_1" },
	{ TCP_PT_U8, 1, 1, "Ref_2" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "DisableRefPosLatch",/* name */
      0x3df,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "GetRefPosLatchStat",/* name */
      0x3e0,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "EnDisEncSupply",    /* name */
      0x3d2,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Active" }
      }
    },
    {
      "SetEncSupply",      /* name */
      0x3e4,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Supply" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "GetEncSupplyStatus",/* name */
      0x3e5,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      2,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Active" },
	{ TCP_PT_U8, 1, 1, "Error" }
      }
    },
    {
      "ClrPosCounter",     /* name */
      0x3d6,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Active" }
      }
    },
    {
      "EnDisCompensation", /* name */
      0x3d7,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Compensation" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetEncInputBandw",  /* name */
      0x3db,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" },
	{ TCP_PT_U8, 1, 1, "Bandwith" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "EnDisIncrInpTerm",  /* name */
      0x3dc,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Mode" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "GetEncChannelData", /* name */
      0x3d9,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      10,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" },
	/* - OR - */
	{ TCP_PT_U16,  0, 2, "TriggerCounter" },
	{ TCP_PT_U16,  2, 2, "Status" },
	{ TCP_PT_POS,  4, 6, "Position" },
	{ TCP_PT_U32, 10, 4, "Timestamp" },
	{ TCP_PT_POS, 14, 6, "ReferencePosition1" },
	{ TCP_PT_POS, 20, 6, "ReferencePosition2" },
	{ TCP_PT_POS, 26, 6, "DistanceCodedReferencePosition" },
	{ TCP_PT_U16, 32, 2, "AnalogueEncoderSignalA" },
	{ TCP_PT_U16, 34, 2, "AnalogueEncoderSignalB" }
      }
    },
    {
      "GetPosition",       /* name */
      0x501,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      3,                   /* numresults */
      {           
	{ TCP_PT_U32,  0, 4, "Status" },
	/* - OR - */
	{ TCP_PT_U16,  0, 2, "StatusWord" },
	{ TCP_PT_POS,  2, 6, "Position" }
      }
    },
    {
      "EnDatModeCmd",      /* name */
      0x502,               /* opcode */
      4,                   /* numparams */
      {           
	{ TCP_PT_U8 , 0, 1, "Channel" },
	{ TCP_PT_U8 , 1, 1, "Cmd" },
	{ TCP_PT_U8 , 2, 1, "Addr" },
	{ TCP_PT_U16, 3, 2, "Data" }
      },
      9,                   /* numresults */
      {
	{ TCP_PT_U32,  0, 4, "Status" },
	/* - OR - */
	{ TCP_PT_U32,  0, 4, "Data_Low" },
	{ TCP_PT_U32,  4, 4, "Data_High" },
	/* - OR - */
	{ TCP_PT_U16,  0, 2, "Status" },
	{ TCP_PT_POS,  2, 6, "Position" },
	{ TCP_PT_U16,  8, 2, "AdditionalInfo1Status" },
	{ TCP_PT_U16, 10, 2, "AdditionalInfo1" },
	{ TCP_PT_U16, 12, 2, "AdditionalInfo2Status" },
	{ TCP_PT_U16, 14, 2, "AdditionalInfo2" }
      }
    },
    {
      "GetIncrAndAbsPos",  /* name */
      0x503,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Channel" }
      },
      5,                   /* numresults */
      {           
	{ TCP_PT_U32,  0, 4, "Status" },
	/* - OR - */
	{ TCP_PT_U16,  0, 2, "StatusAbs" },
	{ TCP_PT_POS,  2, 6, "PositionAbs" },
	{ TCP_PT_U16,  8, 2, "StatusIncr" },
	{ TCP_PT_POS, 10, 6, "PositionIncr" }
      }
    },
    { 
      "ResetEIB",          /* name */
      0x001,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Key" }
      },
      -1,                  /* numresults (don't wait for answer!) */
      { EMPTY_PARAM } 
    },
    {
      "IdentifyEIB",       /* name */
      0x002,               /* opcode */
      1,                   /* numparams */
      {           
	{ TCP_PT_U8, 0, 1, "Mode" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8, 0, 1, "Status" }
      }
    },
    {
      "GetInterfaceVersion",/* name */
      0x409,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Version" }
      }
    },
    {
      "ConfigDataPacket",/* name */
      0x3A3,               /* opcode */
      1,                   /* numparams */
      {
   { TCP_PT_Data, 0, -1, "pkt config" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "GetRecordingData",/* name */
      0x3A5,               /* opcode */
      3,                   /* numparams */
      {
   { TCP_PT_U8, 0, 1, "mode" },
   { TCP_PT_U32, 1, 4, "offset" },
   { TCP_PT_U32, 5, 4, "length" }
      },
      3,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" },
   /* optional */
   { TCP_PT_U32, 4, 4, "IP" },
   { TCP_PT_U16, 8, 2, "Port" }
      }
    },
    {
      "GetRecordingStatus",/* name */
      0x3A6,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      4,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "status" },
   { TCP_PT_U32, 4, 4, "length" },
   { TCP_PT_U8, 8, 1, "FIFO status" },
   { TCP_PT_U8, 9, 1, "progress" }
      }
    },
    {
      "GetStreamingStatus",/* name */
      0x40A,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      4,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "status" },
   { TCP_PT_U32, 4, 4, "length" },
   { TCP_PT_U32, 8, 4, "max length" },
   { TCP_PT_U32, 12, 4, "size" }
      }
    },
    {
      "SelectMode",/* name */
      0x3A4,               /* opcode */
      1,                   /* numparams */
      {
   { TCP_PT_U8, 0, 1, "mode" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "status" }
      }
    },
    {
      "SetEnDat22AddInfo",/* name */
      0x505,               /* opcode */
      3,                   /* numparams */
      {
   { TCP_PT_U8, 0, 1, "channel" },
   { TCP_PT_U8, 1, 1, "addinfo1" },
   { TCP_PT_U8, 2, 1, "addinfo2" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetEnDat22AddInfoFIFO",/* name */
      0x504,               /* opcode */
      4,                   /* numparams */
      {
   { TCP_PT_U8, 0, 1, "channel" },
   { TCP_PT_U8, 1, 1, "enable" },
   { TCP_PT_U8, 2, 1, "length" },
   { TCP_PT_Data, 3, -1, "data" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "GetRecMemSize",/* name */
      0x3A7,               /* opcode */
      0,                   /* numparams */
      { EMPTY_PARAM },
      2,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" },
   { TCP_PT_U32, 4, 4, "size" }
      }
    },
    {
      "SetIntervalCounterTrigger",/* name */
      0x3BE,               /* opcode */
      4,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Mode" },
   { TCP_PT_U8, 1, 1, "Start" },
   { TCP_PT_U32, 2, 4, "Startpos" },
   { TCP_PT_U32, 6, 4, "Period" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetIntervalCounterInterpolation",/* name */
      0x3BF,               /* opcode */
      2,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "IPF" },
   { TCP_PT_U8, 1, 1, "Edge" },
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetOutputTriggerSource",/* name */
      0x380,               /* opcode */
      2,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Channel" },
   { TCP_PT_U8, 1, 1, "Source" },
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetAxisTriggerSource",/* name */
      0x381,               /* opcode */
      2,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Channel" },
   { TCP_PT_U8, 1, 1, "Source" },
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetAuxAxisTriggerSource",/* name */
      0x382,               /* opcode */
      1,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Source" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetMasterTriggerSource",/* name */
      0x386,               /* opcode */
      1,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Source" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "ConfigPulsCounter",/* name */
      0x383,               /* opcode */
      3,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Start" },
   { TCP_PT_U8, 1, 1, "Trigger" },
   { TCP_PT_U32, 2, 4, "Count" },
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetTriggerInputDelay",/* name */
      0x384,               /* opcode */
      2,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Channel" },
   { TCP_PT_U32, 1, 4, "Delay" },
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetRITriggerEdge",/* name */
      0x385,               /* opcode */
      2,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Channel" },
   { TCP_PT_U8, 1, 1, "Edge" },
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "AuxClearAmpError",/* name */
      0x3E6,               /* opcode */
      1,                   /* numparams */
      {
	{ TCP_PT_U8, 0, 1, "Mask" },
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "AuxClearCounter",/* name */
      0x3E7,               /* opcode */
      0,                   /* numparams */
      {
	EMPTY_PARAM
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "AuxClearRefStatus",/* name */
      0x3E8,               /* opcode */
      0,                   /* numparams */
      {
	EMPTY_PARAM
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "AuxGetRefPosStatus",/* name */
      0x3E9,               /* opcode */
      0,                   /* numparams */
      {
	EMPTY_PARAM
      },
      3,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" },
   { TCP_PT_U8, 4, 1, "Active" },
   { TCP_PT_U8, 5, 1, "Valid" }
      }
    },
    {
      "AuxStartRef",/* name */
      0x3EA,               /* opcode */
      0,                   /* numparams */
      {
	EMPTY_PARAM
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" },
      }
    },
    {
      "AuxStopRef",/* name */
      0x3EB,               /* opcode */
      0,                   /* numparams */
      {
	EMPTY_PARAM
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" },
      }
    },
    {
      "AuxGetPosition",/* name */
      0x506,               /* opcode */
      0,                   /* numparams */
      {
	EMPTY_PARAM
      },
      3,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" },
   { TCP_PT_U16, 4, 2, "Statusword" },
   { TCP_PT_AUXPOS, 6, 4, "Position" },
      }
    },
    {
      "AuxGetEncData",/* name */
      0x507,               /* opcode */
      0,                   /* numparams */
      {
	EMPTY_PARAM
      },
      6,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" },
   { TCP_PT_U16, 4, 2, "Statusword" },
   { TCP_PT_AUXPOS, 6, 4, "Position" },
   { TCP_PT_AUXPOS, 10, 4, "Reference" },
   { TCP_PT_U32, 14, 4, "Timestamp" },
   { TCP_PT_U16, 18, 2, "TriggerCounter" },
      }
    },
    {
      "GetTriggerDelayTicks",   /* name */
      0x387,               /* opcode */
      0,                   /* numparams */
      {           
	EMPTY_PARAM
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U8,  0, 1, "Ticks" },
      }
    },
    {
      "SetTriggerSources",   /* name */
      0x388,               /* opcode */
      2,                   /* numparams */
      {           
	{ TCP_PT_U32,  0, 4, "Enable" },
	{ TCP_PT_U32,  4, 4, "Disable" },
      },
      2,                   /* numresults */
      {           
	{ TCP_PT_U32,  0, 4, "Status" },
	{ TCP_PT_U32,  4, 4, "Actual" },
      }
    },
    {
      "GetNumOfAxes",   /* name */
      0x40D,               /* opcode */
      0,                   /* numparams */
      {           
	EMPTY_PARAM
      },
      5,                   /* numresults */
      {           
	{ TCP_PT_U32,  0, 4, "Status" },
	{ TCP_PT_U8,  4, 1, "DSub" },
	{ TCP_PT_U8,  5, 1, "M12" },
	{ TCP_PT_U8,  6, 1, "reserved" },
	{ TCP_PT_U8,  7, 1, "reserved" },
      }
    },
    {
      "SetEnDat22CmdFIFO",   /* name */
      0x508,               /* opcode */
      5,                   /* numparams */
      {           
   { TCP_PT_U8, 0, 1, "channel" },
   { TCP_PT_U8, 1, 1, "enable" },
   { TCP_PT_U8, 2, 1, "length" },
   { TCP_PT_U8, 3, 1, "unused" },
   { TCP_PT_Data, 4, -1, "data" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },
    {
      "SetReferenceCheck",   /* name */
      0x3EC,               /* opcode */
      3,                   /* numparams */
      {           
   { TCP_PT_U8, 0, 1, "channel" },
   { TCP_PT_U8, 1, 1, "mode" },
   { TCP_PT_U32, 2, 4, "limit" }
      },
      1,                   /* numresults */
      {           
	{ TCP_PT_U32, 0, 4, "Status" }
      }
    },


    { 
      NULL,0,0,{ EMPTY_PARAM },0,{ EMPTY_PARAM } /* To detect end of data */
    }
  };



