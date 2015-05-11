
/* This file is auto-generated from utils/generate_error_texts.py. Please do not edit! */

struct
  {
    unsigned long code;
    char *mnemonic;
    char *message;
} EIB7_Errors[] =
{
  {
    0x50000001, "EIB7_InvEnDatCmdCode", "Invalid command code for the EnDat command interface"
  },  {
    0x50000002, "EIB7_InvEnDatClock", "Invalid EnDat clock"
  },  {
    0x50000003, "EIB7_EnDat22NotSupp", "EnDat 2.2 is not supported by this encoder"
  },  {
    0x50000004, "EIB7_EnDatErrII", "EnDat error type II"
  },  {
    0x50000005, "EIB7_EnDatIfBusy", "EnDat interface busy"
  },  {
    0x50000006, "EIB7_EnDatXmitErr", "EnDat transmission error (encoder may be not connected)"
  },  {
    0x50000007, "EIB7_MonotimeErr", "invalid monotime for this encoder or EnDat clock"
  },  {
    0x60000001, "EIB7_InvInterface", "invalid interface type (1Vss, EnDat)"
  },  {
    0x60000002, "EIB7_InvChannel", "invalid channel"
  },  {
    0x60000003, "EIB7_NotInitialized", "not initialised"
  },  {
    0x60000004, "EIB7_AccNotAllowed", "access not allowed(e.g. Soft Realtime Mode enabled, EnDat master already in use, ...)"
  },  {
    0x60000005, "EIB7_InvEncoder", "invalid encoder type (rotary/linear)"
  },  {
    0x60000006, "EIB7_InvRefMarkOpt", "invalid reference mark option"
  },  {
    0x60000007, "EIB7_InvInterpolOpt", "invalid interpolation option"
  },  {
    0x60000008, "EIB7_InvDistCodeRef", "invalid option for distance coded reference marks"
  },  {
    0x60000009, "EIB7_ConfOptIncons", "configuration options inconsistent"
  },  {
    0x6000000A, "EIB7_FuncNotSupp", "function not supported"
  },  {
    0x6000000B, "EIB7_CantClearEnc", "can not clear encoder supply error"
  },  {
    0x6000000C, "EIB7_ParamInvalid", "parameter not valid"
  },  {
    0x6000000D, "EIB7_EncPwrSuppErr", "encoder power supply error"
  },  {
    0x6000000E, "EIB7_CantChIncInpTrm", "can not change incremental input termination"
  },  {
    0x60001001, "EIB7_InvalidOpMode", "operation mode not supported"
  },  {
    0x60001002, "EIB7_OpModeActive", "operation mode is already active"
  },  {
    0x60001003, "EIB7_OpModeBlocked", "operation mode blocked"
  },  {
    0x60001004, "EIB7_InvalidIPAddr", "IP address invalid"
  },  {
    0x60002001, "EIB7_PacketTooLong", "data packet too long"
  },  {
    0x60002002, "EIB7_InvalidPacket", "data packet not valid"
  },  {
    0x60003001, "EIB7_RecDataReadErr", "can not read recording data from memory"
  },  {
    0x70000001, "EIB7_CantLatchPos", "can not latch position"
  },  {
    0x70000002, "EIB7_CantLatchDist", "can not latch distance coded reference marks"
  },  {
    0x80000001, "EIB7_TrgNotEn", "trigger output is not enabled"
  },  {
    0x80000002, "EIB7_TrgNotConf", "trigger input/output is not configured as logical I/O"
  },  {
    0x80000003, "EIB7_TimerTrgEn", "Timer Trigger is enabled"
  },  {
    0x80000004, "EIB7_ExtTrgEn", "External Trigger is enabled"
  },  {
    0x80000005, "EIB7_InvTrgPeriod", "invalid Timer Trigger period"
  },  {
    0x80000006, "EIB7_ICTrgEn", "interval counter trigger enabled"
  },  {
    0x80000007, "EIB7_MasterTrgInv", "master trigger invalid"
  },  {
    0x90000001, "EIB7_FirmwNotAvail", "firmware not available"
  },  {
    0x90000002, "EIB7_CantRdSer", "can not read serial number"
  },  {
    0x90000003, "EIB7_CantRdIdent", "can not read ident. number"
  },  {
    0x90000004, "EIB7_NoCustNetwork", "no custom network parameter available"
  },  {
    0x90000005, "EIB7_CantSaveCustNW", "custom network parameter can not be saved"
  },  {
    0x90000006, "EIB7_CantSaveDHCP", "can not save DHCP timeout"
  },  {
    0x90000007, "EIB7_DHCPTimeoutInv", "DHCP timeout invalid"
  },  {
    0x90000008, "EIB7_HostnameTooLong", "hostname is to long/short"
  },  {
    0x90000009, "EIB7_HostnameInvalid", "hostname invalid"
  },  {
    0x9000000A, "EIB7_CantSaveHostn", "can not save hostname"
  },  {
    0x9000000B, "EIB7_CantRestDefHn", "can not restore default hostname"
  },  {
    0x9000000C, "EIB7_CantRdHostname", "can not read hostname"
  },  {
    0x90000031, "EIB7_CantSetStatusLED", "can not set status LED"
  },  {
    0x90000041, "EIB7_NoDestIP", "no destination IP for Soft Realtime Mode"
  },  {
    0x90000042, "EIB7_DestIPUnreach", "destination IP for Soft Realtime Mode unreachable"
  },  {
    0x90000043, "EIB7_InvIPAddress", "invalid IP address or port number"
  },  {
    0x90000044, "EIB7_SoftRTEn", "Soft Realtime Mode already enabled"
  },  {
    0x90000051, "EIB7_CantTermConn", "can not terminate connection"
  },  {
    0x90000052, "EIB7_CantTermSelf", "can not terminate myself"
  },  {
    0x90000061, "EIB7_InvTSPeriod", "invalid timestamp period"
  },  {
    0x90000062, "EIB7_BroadcastInv", "use of broadcast address is not allowed"
  },  {
    0x90000063, "EIB7_PortNoInv", "invalid port number"
  },  {
    0xA0000001, "EIB7_CantInitWinSock", "WSAStartup() call failed on WIN32"
  },  {
    0xA0000002, "EIB7_HostNotFound", "gethostbyname() could not resolve hostname"
  },  {
    0xA0000003, "EIB7_CantOpenSocket", "the socket could not be created"
  },  {
    0xA0000004, "EIB7_CantConnect", "could not connect the socket to the device"
  },  {
    0xA0000005, "EIB7_SendError", "Error ocurred while sending data to the device"
  },  {
    0xA0000006, "EIB7_ReceiveError", "Error ocurred while receiving data from the device"
  },  {
    0xA0000007, "EIB7_OutOfMemory", "Required memory could not be allocated"
  },  {
    0xA0000008, "EIB7_IllegalParameter", "The parameter is invalid for this command"
  },  {
    0xA0000009, "EIB7_InvalidResponse", "The command response of the device is invalid"
  },  {
    0xA000000A, "EIB7_InvalidHandle", "The handle passed to any EIB7* function is invalid"
  },  {
    0xA000000B, "EIB7_ConnReset", "The connection was reset by the peer"
  },  {
    0xA000000C, "EIB7_ConnTimeout", "The network request has timed out"
  },  {
    0xA000000D, "EIB7_BufferTooSmall", "The destination memory size is too small"
  },  {
    0xA000000E, "EIB7_CantStartThread", "Cannot start the soft realtime thread"
  },  {
    0xA000000F, "EIB7_ElementSizeInv", "FIFO element size mismatch"
  },  {
    0xA0000010, "EIB7_FIFOEmpty", "The FIFO is empty, cannot retrieve element"
  },  {
    0xA0000011, "EIB7_FieldNotAvail", "The data field requested is not available for the current connection"
  },  {
    0xA0000012, "EIB7_IFVersionInv", "Invalid interface version"
  },  {
    0xA0000013, "EIB7_FIFOOverflow", "The FIFO had an overflow, some data packets are lost"
  },  {
    0xA0000014, "EIB7_PortDirInv", "The I/O port has the wrong direction for this operation"
  },
  { 0,NULL,NULL }
};

