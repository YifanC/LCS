///
/// \file       EIB_Control.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_Control.h,v 1.3 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: EIB_Control.h,v $
/// Revision 1.3  2011/12/16 13:37:56  rohn
/// Version 0.9.0.1 (siehe History.txt)
///
/// Revision 1.2  2011/09/20 12:24:36  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB control class.
///
/// This class provides the functions for the EIB control
/// via the eib7.dll.
///

#pragma once

#include <memory.h>

#include "FormMain.h"

using namespace System;
using namespace System::Threading;
using namespace System::Runtime::InteropServices;
using namespace System::Runtime::Serialization;
using namespace System::Diagnostics;

namespace EIB7xx_App
{
 	#pragma region "IP_Address class definition"
	/// <summary>
	/// This class definies an IP address.
	/// </summary>
    public ref class IP_Address
    {
    private:
        IPAddress ^         ipAddress;  ///< IP address in net byte order
        ResourceManager ^   resManager; ///< resource manger of the main form

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="ipString">The IP address string.</param>
        IP_Address(String ^ ipString)
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();

            try
            {
                // get IP address in net byte order
                ipAddress = IPAddress::Parse(ipString);
            }
            catch (...)
            {
                // IP address format exception
                throw gcnew FormatException(resManager->GetString(L"errorIpAddress"));
            }
        }

        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="ipValue">The UInt32 value of the IP address in host byte order.</param>
        IP_Address(UInt32 ipValue)
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();

            // get IP address as byte array in net byte order
            array<Byte> ^ ipBytes = gcnew array<Byte>(4);
            // convert UInt32 to byte array in net byte order
            UInt32 length = sizeof(UInt32);
            for (UInt32 i = 0; i < length; i ++)
            {
               ipBytes[length - i - 1] = (Byte)(ipValue >> (i * 8));
            }
            // get IP address in net byte order
            ipAddress = gcnew IPAddress(ipBytes);
        }

        /// <summary>
        /// Gets the UInt32 value of the IP address in host byte order.
        /// </summary>
        property UInt32 IpValue
        {
            UInt32 get()
            {
                // get IP address as byte array in net byte order
                array<Byte> ^ ipBytes = ipAddress->GetAddressBytes();
                // check number of bytes
                UInt32 length = ipBytes->Length;
                if (length != sizeof(UInt32))
                {
                    // IP address format exception
                    throw gcnew FormatException(resManager->GetString(L"errorIpAddress"));
                }
                // convert byte array to UInt32 in host byte order
                UInt32 iIpValue = 0;
                for (UInt32 i = 0; i < length; i ++)
                {
                   iIpValue += (UInt32)(ipBytes[length - i - 1]) << (i * 8);
                }
                return iIpValue;
            }
        }

        /// <summary>
        /// Gets the string value of the IP address.
        /// </summary>
        property String ^ IpString
        {
            String ^ get() { return ipAddress->ToString(); }
        }
    };
	#pragma endregion

    #pragma region "EIB7_ERROR enum class definition"
    /// <summary>
    /// This enum lists all possible result codes for the driver interface.
    /// </summary>
    enum class EIB7_ERROR : UInt32
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

        // special error codes for EIB7xx_App
        EIB7_EqualIpAddress  = 0xF0000001, /*!< The EIBs have an equal IP address */
        EIB7_DataPacketSize  = 0xF0000002, /*!< The data packet is too large */
        EIB7_DataReadTimeout = 0xF0000003, /*!< Data read timeout */
        EIB7_ConnectClosed   = 0xF0000004, /*!< Connection to EIB has been closed */
        EIB7_AxisDisabled    = 0xF0000005, /*!< All axis of the EIB are disabled */
        EIB7_CrankshaftParam = 0xF0000006, /*!< Bad crankshaft parameter value */
        EIB7_FirmwareTFTP    = 0xF0000007, /*!< Firmware TFTP error */
        EIB7_FirmwareStatus  = 0xF0000008, /*!< Bad firmware update status */
    };
    #pragma endregion

    #pragma region "EIB_CtrlException class definition"
    /// <summary>
    /// This class provides an exception
    /// for the EIB control functions.
    /// </summary>
    [Serializable]
    public ref class EIB_CtrlException : public Exception
    {
    private:
        EIB7_ERROR      eibError;       ///< the EIB error code
        String ^        functionName;   ///< the name of the function witch generated the error
        String ^        eibName;        ///< EIB name shown in the Main Tree View
        Int32           axisIndex;      ///< the axis index

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="message">The error message.</param>
        /// <param name="eibError">The EIB error code.</param>
        /// <param name="functionName">The name of the function witch generated the error.</param>
        /// <param name="eibName">The EIB name.</param>
        /// <param name="axisIndex">The axis index.</param>
        EIB_CtrlException(String ^ message, EIB7_ERROR eibError, String ^ functionName,
                          String ^ eibName, Int32 axisIndex)
            : Exception(message)
        {
            this->eibError      = eibError;
            this->functionName  = String::Copy(functionName);
            this->eibName       = String::Copy(eibName);
            this->axisIndex     = axisIndex;
        }
        
    protected:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_CtrlException(SerializationInfo ^ info, StreamingContext context)
            : Exception(info, context)
        {
        }

        /// <summary>
        /// Gets the EIB error code.
        /// </summary>
        property EIB7_ERROR EibError
        {
            EIB7_ERROR get() { return eibError; }
        }

        /// <summary>
        /// Gets the EIB name.
        /// </summary>
        property String ^ EibName
        {
            String ^ get() { return eibName; }
        }

        /// <summary>
        /// Gets the axis index.
        /// </summary>
        property Int32 AxisIndex
        {
            Int32 get() { return axisIndex; }
        }
    };
    #pragma endregion

    #pragma region "EIB_Ctrl class definition"
    /// special definitions
    #define EIB_HANDLE_INIT                 Int32(   -1)    ///< EIB handle init value
    #define EIB_CTRL_TIMEOUT                Int32( 2000)    ///< timeout ms for EIB control functions
    #define EIB_DHCP_TIMEOUT                UInt32(   5)    ///< timeout s for DHCP client
    #define EIB_STRING_MAX_LENGTH           UInt32(  31)    ///< max length of EIB strings (without \0)
    #define EIB_DATA_PACKET_MAX_LENGTH      UInt32( 256)    ///< max length of EIB data packet
    #define EIB_SIGNAL_INTERPOLATION        UInt32(4096)    ///< interpolation of incremental signals

    /// EIB handle list
    typedef array<Int32>    EibHandleList;

    /// delegate definition for the data callback function
    public delegate void OnDataDelegate(Int32  eibHandle,
                                        UInt32 count,
                                        void * data);

    #pragma region "Parameter / Constructor / Destructor"
    /// <summary>
    /// This class provides the functions for the EIB control
    /// via the eib7.dll.
    /// </summary>
    public ref class EIB_Ctrl
    {
    typedef array<UInt32>               ArrayUInt32;

    private:
        GUI_Support ^       guiSupport;         ///< the GUI support class
        ResourceManager ^   resManager;         ///< resource manager from main window

        EIB_Param ^         parameter;          ///< parameters of the saved EIB

        Int32               eibHandle;          ///< EIB handle of opened EIB
        EibHandleList ^     axisHandle;         ///< axis handle of opened EIB
        EibHandleList ^     triggerInpHandle;   ///< extern trigger input handle of opened EIB
        EibHandleList ^     triggerOutHandle;   ///< extern trigger output handle of opened EIB

        ArrayUInt32 ^       pollingCounter;     ///< counter for read polling data

        OnDataDelegate ^    onDataDelegate;     ///< delegate for data callback function
        IntPtr              onDataPtr;          ///< pointer for data callback function
        GCHandle            onDataHandle;       ///< handle for data callback function

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="guiSupport">The GUI support class.</param>
        /// <param name="parameter">The parameters of the saved EIB.</param>
        EIB_Ctrl(GUI_Support ^ guiSupport,
                 EIB_Param ^   parameter)
        {
            this->guiSupport = guiSupport;
            this->resManager = guiSupport->ResManager;
            this->parameter  = parameter;

            eibHandle = EIB_HANDLE_INIT;
            axisHandle = gcnew EibHandleList(EIB_MAX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axisHandle[i] = EIB_HANDLE_INIT;
            }

            triggerInpHandle = gcnew EibHandleList(EIB_MAX_TRIGGER_INP_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_INP_NUMBER; i ++)
            {
                triggerInpHandle[i] = EIB_HANDLE_INIT;
            }

            triggerOutHandle = gcnew EibHandleList(EIB_MAX_TRIGGER_OUT_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_OUT_NUMBER; i ++)
            {
                triggerOutHandle[i] = EIB_HANDLE_INIT;
            }

            pollingCounter = gcnew ArrayUInt32(EIB_MAX_ALL_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_ALL_AXIS_NUMBER; i ++)
            {
                pollingCounter[i] = 0;
            }

            // initialize data aquisition callback function
            onDataDelegate = gcnew OnDataDelegate(this, &EIB7xx_App::EIB_Ctrl::OnDataFunction);
            pin_ptr<OnDataDelegate ^> tmp =  &onDataDelegate;
            onDataPtr = Marshal::GetFunctionPointerForDelegate(onDataDelegate);
            onDataHandle = GCHandle::Alloc(onDataDelegate);
        }

    protected:
        /// <summary>
        /// Destructor.
        /// </summary>
        ~EIB_Ctrl()
        {
            // close EIB control
            Close_EIB_Ctrl();
        }

        /// <summary>
        /// Finalizer.
        /// </summary>
        !EIB_Ctrl()
        {
            // close EIB control
            Close_EIB_Ctrl();
        }

    private:
        /// <summary>
        /// Close EIB control.
        /// </summary>
        void Close_EIB_Ctrl()
        {
            // close EIB
            EIB_Close(false);
            // close callback handle
            if (onDataHandle.IsAllocated)
            {
                onDataHandle.Free();
            }
        }
    #pragma endregion

    public:

    #pragma region "Properties"
        /// <summary>
        /// Gets the EIB and axis name.
        /// </summary>
        property String ^ EibAxisName[Int32]
        {
            String ^ get(Int32 axisIndex)
            {
                String ^ eibName = String::Copy(parameter->EibName);
                if (axisIndex != 0)
                {
                    eibName += L", " + AxisName[axisIndex];
                }
                return eibName;
            }
        }

        /// <summary>
        /// Gets an EIB axis name.
        /// </summary>
        property String ^ AxisName[Int32]
        {
            String ^ get(Int32 axisIndex)
            {
                String ^ axisName = L"";
                if ((axisIndex > 0) && (axisIndex <= Int32(parameter->AxisNumber)))
                {
                    axisName = String::Format(resManager->GetString(L"textAxis") + L" {0:D}",
                                                                    axisIndex);
                }
                else if ((axisIndex < 0) || (axisIndex > Int32(parameter->AxisNumber)))
                {
                    if (parameter->AuxAxisNumber == 1)
                    {
                        axisName = resManager->GetString(L"textAuxAxis");
                    }
                    else
                    {
                        if (axisIndex < 0)
                        {
                            axisIndex = -axisIndex;
                        }
                        else
                        {
                            axisIndex = axisIndex - Int32(parameter->AxisNumber);
                        }
                        axisName = String::Format(resManager->GetString(L"textAuxAxis") + L" {0:D}",
                                                                        axisIndex);
                    }
                }
                return axisName;
            }
        }
    #pragma endregion

    private:

    #pragma region "Error handling functions"
        /// <summary>
        /// Check the return code from an EIB control function.
        /// In case of an error throw an EIB control exception.
        /// </summary>
        /// <param name="eibError">The EIB error code.</param>
        /// <param name="functionName">The name of the function witch generated the error.</param>
        void CheckEibError(EIB7_ERROR eibError, String ^ functionName)
        {
            CheckEibError(eibError, functionName, 0);
        }

        /// <summary>
        /// Check the return code from an EIB control function.
        /// In case of an error throw an EIB control exception.
        /// </summary>
        /// <param name="eibError">The EIB error code.</param>
        /// <param name="functionName">The name of the function witch generated the error.</param>
        /// <param name="axisIndex">The axis index.</param>
        void CheckEibError(EIB7_ERROR eibError, String ^ functionName, Int32 axisIndex)
        {
            if (eibError != EIB7_ERROR::EIB7_NoError)
            {
                // get error text from ressource manager
                String ^ error = resManager->GetString(eibError.ToString());
                if (error == nullptr)
                {
                    // get error text from EIB library
                    EIB7_ERROR eibError2;
                    #define ERROR_MNEMONIC_LENGTH        80
                    #define ERROR_MESSAGE_LENGTH        256
                    char mnemonic[ERROR_MNEMONIC_LENGTH];
                    char message[ERROR_MESSAGE_LENGTH];
                    eibError2 = (EIB7_ERROR)EIB7GetErrorInfo((EIB7_ERR)eibError,
                                                              mnemonic, ERROR_MNEMONIC_LENGTH,
                                                              message,  ERROR_MESSAGE_LENGTH);
                    if (eibError2 == EIB7_ERROR::EIB7_NoError)
                    {
                        error = gcnew String(message);
                    }
                    else
                    {
                        // get error text from error code enumeration
                        error = eibError.ToString();
                    }
                }
                String ^ message = String::Format(resManager->GetString(L"errorEibCtrlError"),
                                                  EibAxisName[axisIndex], error, functionName);
                throw gcnew EIB_CtrlException(message, eibError, functionName,
                                              parameter->EibName, axisIndex);
            }
        }
    #pragma endregion

    public:

    #pragma region "Open / close functions"
        /// <summary>
        /// Get EIB IP address.
        /// </summary>
        /// <param name="parameter">The EIB parameters.</param>
        /// <returns>The EIB IP address.</returns>
        UInt32 EIB_GetIpAddress(EIB_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            #define BUFFER_LENGTH   80
            char buffer[BUFFER_LENGTH];
            // check EIB address mode
            unsigned long ipAddress;
            if (parameter->Network->CurrentDhcpMode == true)
            {
                // get host name
                UInt32 i, length = parameter->Network->CurrentHostName->Length;
                if (length > EIB_STRING_MAX_LENGTH)
                {
                    length = EIB_STRING_MAX_LENGTH;
                }
                for (i = 0; i < length; i ++)
                {
                    buffer[i] = (SByte)parameter->Network->CurrentHostName[i] & 0x07F;
                }
                buffer[i] = 0;
                // get IP address from host name
                eibError = (EIB7_ERROR)EIB7GetHostIP(buffer, &ipAddress);
                CheckEibError(eibError, L"EIB7GetHostIP");
            }
            else
            {
                ipAddress = parameter->Network->CurrentIpAddress;
            }
            return ipAddress;
        }

        /// <summary>
        /// Open EIB.
        /// </summary>
        /// <param name="parameter">The EIB parameters.</param>
        /// <param name="axisNumber">The EIB axis number to set.</param>
        /// <param name="auxAxisNumber">The EIB auxiliary axis number to set.</param>
        void EIB_Open(EIB_Param ^ parameter, UInt32 % axisNumber, UInt32 % auxAxisNumber)
        {
            EIB7_ERROR eibError;
            #define BUFFER_LENGTH   80
            char buffer[BUFFER_LENGTH];
            // if EIB is already open, then check connection
            if (eibHandle != EIB_HANDLE_INIT)
            {
                // if connection was closed, the handle will be reset
                EIB_CheckOpen();
            }
            // if EIB is already open, then don't open it
            if (eibHandle == EIB_HANDLE_INIT)
            {
                // open EIB
                EIB7_HANDLE tmpEibHandle = eibHandle;
                eibError = (EIB7_ERROR)EIB7Open(EIB_GetIpAddress(parameter),
                                                &tmpEibHandle, EIB_CTRL_TIMEOUT,
                                                buffer, BUFFER_LENGTH);
                CheckEibError(eibError, L"EIB7Open");
                // save new EIB handle
                eibHandle = tmpEibHandle;
                // read axis number
                unsigned long dsub, res1, res2, res3;
                eibError = (EIB7_ERROR)EIB7GetNumOfAxes(eibHandle, &dsub, &res1, &res2, &res3);
                CheckEibError(eibError, L"EIB7GetNumOfAxes");
                axisNumber    = dsub;
                auxAxisNumber = UInt32(1);
                // set axis handles
                EIB7_AXIS set[EIB_MAX_AXIS_NUMBER];
                unsigned long len;
                eibError = (EIB7_ERROR)EIB7GetAxis(eibHandle, set, EIB_MAX_AXIS_NUMBER, &len);
                CheckEibError(eibError, L"EIB7GetAxis");
                for (UInt32 i = 0; i < len; i ++)
                {
                    axisHandle[i] = set[i];
                }
                // set extern trigger handles
                EIB7_IO iset[EIB_MAX_TRIGGER_INP_NUMBER];
                EIB7_IO oset[EIB_MAX_TRIGGER_OUT_NUMBER];
                unsigned long ilen, olen;
                eibError = (EIB7_ERROR)EIB7GetIO(eibHandle, iset, EIB_MAX_TRIGGER_INP_NUMBER, &ilen,
                                                            oset, EIB_MAX_TRIGGER_OUT_NUMBER, &olen);
                CheckEibError(eibError, L"EIB7GetIO");
                for (UInt32 i = 0; i < ilen; i ++)
                {
                    triggerInpHandle[i] = iset[i];
                }
                for (UInt32 i = 0; i < olen; i ++)
                {
                    triggerOutHandle[i] = oset[i];
                }
                //Debug::Print(String::Format(L"Open EIB {0}", eibHandle));
            }
        }

        /// <summary>
        /// Close EIB.
        /// </summary>
        /// <param name="checkError">If true, the close function errors will be checked.</param>
        void EIB_Close(bool checkError)
        {
            EIB7_ERROR eibError;
            // check if EIB is already open
            if (eibHandle != EIB_HANDLE_INIT)
            {
                //Debug::Print(String::Format(L"Close EIB {0}", eibHandle));
                // EIB is open, close EIB
                eibError = (EIB7_ERROR)EIB7Close(eibHandle);
                if (checkError == true)
                {
                    CheckEibError(eibError, L"EIB7Close");
                }
                eibHandle = EIB_HANDLE_INIT;
                for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
                {
                    axisHandle[i] = EIB_HANDLE_INIT;
                }
            }
        }

        /// <summary>
        /// Check EIB open state.
        /// </summary>
        /// <returns>True if EIB is open.</returns>
        bool EIB_CheckOpen()
        {
            EIB7_ERROR eibError;
            // check EIB handle
            if (eibHandle == EIB_HANDLE_INIT)
            {
                return false;
            }
            // check EIB connection
            EIB7_CONN_STATUS status;
            eibError = (EIB7_ERROR)EIB7GetConnectionStatus(eibHandle, &status);
            if (eibError == EIB7_ERROR::EIB7_InvalidHandle)
            {
                // if connection is not right open then close it
                EIB_Close(false);
                return false;
            }
            CheckEibError(eibError, L"EIB7GetConnectionStatus");
            // check connection status
            if (status != EIB7_CS_Connected)
            {
                // if connection is not right open then close it
                EIB_Close(false);
                return false;
            }
            // EIB7GetConnectionStatus doesn't detect a closed and reopened connection
            // use EIB7GetNumberOfOpenConnections for this purpose
            unsigned long cnt;
            eibError = (EIB7_ERROR)EIB7GetNumberOfOpenConnections(eibHandle, &cnt);
            if (   (eibError == EIB7_ERROR::EIB7_ConnReset)
                || (eibError == EIB7_ERROR::EIB7_ConnTimeout))
            {
                // if connection is not right open then close it
                EIB_Close(false);
                return false;
            }
            CheckEibError(eibError, L"EIB7GetNumberOfOpenConnections");
            // connection is open
            return true;
        }
    #pragma endregion

    #pragma region "Network functions"
        /// <summary>
        /// Reset EIB.
        /// </summary>
        /// <param name="parameter">The EIB parameters.</param>
        /// <param name="axisNumber">The EIB axis number to set.</param>
        /// <param name="auxAxisNumber">The EIB auxiliary axis number to set.</param>
        void EIB_Reset(EIB_Param ^ parameter, UInt32 % axisNumber, UInt32 % auxAxisNumber)
        {
            EIB7_ERROR eibError;
            // reset EIB
            eibError = (EIB7_ERROR)EIB7Reset(eibHandle);
            CheckEibError(eibError, L"EIB7Reset");
            //Debug::Print(String::Format(L"EIB reset command"));
            // close EIB
            EIB_Close(false);
            // wait for reset ready
            Thread::Sleep(5000);
            // check for reset ready
            for (UInt32 i = 0; i < 2; i ++)
            {
                try
                {
                    //Debug::Print(String::Format(L"Reopen number: {0}", i));
                    // open EIB
                    EIB_Open(parameter, axisNumber, auxAxisNumber);
                    // return if open was successful
                    return;
                }
                catch (...)
                {
                }
                //Debug::Print(String::Format(L"Reopen error"));
                Thread::Sleep(1000);
            }
            // last try to open EIB
            EIB_Open(parameter, axisNumber, auxAxisNumber);
        }

        /// <summary>
        /// Update EIB firmware.
        /// </summary>
	    /// <param name="fileName">Firmware file name.</param>
        /// <param name="parameter">The EIB parameters.</param>
        void EIB_Update(String ^ fileName, EIB_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            // set timeout for wait for update ready
            eibError = (EIB7_ERROR)EIB7SetTimeout(eibHandle, 100000);
            CheckEibError(eibError, L"EIB7SetTimeout");
            // update EIB firmware
            String ^ args = String::Format(L"-i {0} put {1} tmp\\update.flash",
                                           IP_Address(EIB_GetIpAddress(parameter)).IpString,
                                           fileName);
            //Debug::Print(String::Format(L"tftp arguments: {0}", args));
            Process ^ process = gcnew Process();
            process->StartInfo->FileName  = L"tftp";
            process->StartInfo->Arguments = args;
            process->StartInfo->CreateNoWindow  = true;
            process->StartInfo->UseShellExecute = false;
            process->StartInfo->RedirectStandardOutput = true;
            process->Start();
            //Debug::Print(String::Format(L"tftp output: {0}", process->StandardOutput->ReadToEnd()));
            process->WaitForExit();
            Int32 exitCode = process->ExitCode;
            if (exitCode != 0)
            {
                String ^ message = String::Format(resManager->GetString(L"errorFirmwareTFTP"),
                                                  exitCode, parameter->EibName);
                throw gcnew EIB_CtrlException(message,
                                              EIB7_ERROR::EIB7_FirmwareTFTP,
                                              L"EIB_Update",
                                              parameter->EibName, 0);
            }
            //Debug::Print(String::Format(L"tftp exit code: {0}", exitCode));
            // wait for update ready
            Thread::Sleep(5000);
            // get update status
            EIB7_UPDATE_STATUS status;
            eibError = (EIB7_ERROR)EIB7UpdateState(eibHandle, &status);
            CheckEibError(eibError, L"EIB7UpdateState");
            // reset timeout for next commands
            eibError = (EIB7_ERROR)EIB7SetTimeout(eibHandle, EIB_CTRL_TIMEOUT);
            CheckEibError(eibError, L"EIB7SetTimeout");
            // check update status
            if (status != EIB7_US_UpdateSuccessful)
            {
                String ^ message = String::Format(resManager->GetString(L"errorFirmwareStatus"),
                                                  Int32(status), parameter->EibName);
                throw gcnew EIB_CtrlException(message,
                                              EIB7_ERROR::EIB7_FirmwareStatus,
                                              L"EIB_Update",
                                              parameter->EibName, 0);
            }
        }

        /// <summary>
        /// Read EIB network parameters.
        /// </summary>
        /// <param name="network">The EIB network read parameters.</param>
        void EIB_ReadNetworkParam(EIB_Network_Read ^ network)
        {
            EIB7_ERROR eibError;
            #define BUFFER_LENGTH   80
            char buffer[BUFFER_LENGTH];
            // get host name
            eibError = (EIB7_ERROR)EIB7GetHostname(eibHandle, buffer, BUFFER_LENGTH);
            CheckEibError(eibError, L"EIB7GetHostname");
            // set host name
            network->SavedHostName = gcnew String(buffer);

            // get IP address, netmask, gateway, DHCP flag
            unsigned long ipAddress, netmask, gateway;
            EIB7_MODE dhcpFlag;
            eibError = (EIB7_ERROR)EIB7GetNetwork(eibHandle,
                                                  &ipAddress, &netmask, &gateway,
                                                  &dhcpFlag);
            CheckEibError(eibError, L"EIB7GetNetwork");
            // set IP address
            network->SavedIpAddress = ipAddress;
            // set netmask
            network->Netmask = netmask;
            // set gateway
            network->Gateway = gateway;
            // set DHCP flag
            network->SavedDhcpMode = (EIB7_ENABLE_MODE)dhcpFlag;
        }

        /// <summary>
        /// Write EIB network parameters.
        /// </summary>
        /// <param name="network">The EIB network read parameters.</param>
        void EIB_WriteNetworkParam(EIB_Network_Read ^ network)
        {
            EIB7_ERROR eibError;
            #define BUFFER_LENGTH   80
            char buffer[BUFFER_LENGTH];
            // get host name
            UInt32 i, length = network->SavedHostName->Length;
            if (length > EIB_STRING_MAX_LENGTH)
            {
                length = EIB_STRING_MAX_LENGTH;
            }
            for (i = 0; i < length; i ++)
            {
                buffer[i] = (SByte)network->SavedHostName[i] & 0x07F;
            }
            buffer[i] = 0;
            // set host name
            eibError = (EIB7_ERROR)EIB7SetHostname(eibHandle, buffer);
            CheckEibError(eibError, L"EIB7SetHostname");

            // set IP address, netmask, gateway, DHCP flag
            eibError = (EIB7_ERROR)EIB7SetNetwork(eibHandle,
                                                  network->SavedIpAddress,
                                                  network->Netmask,
                                                  network->Gateway,
                                                  (EIB7_MODE)network->SavedDhcpMode,
                                                  EIB_DHCP_TIMEOUT);
            CheckEibError(eibError, L"EIB7SetNetwork");
        }

        /// <summary>
        /// Read EIB info parameters.
        /// </summary>
        /// <param name="network">The EIB network read parameters.</param>
        void EIB_ReadInfoParam(EIB_Network_Read ^ network)
        {
            EIB7_ERROR eibError;
            #define BUFFER_LENGTH   80
            char buffer[BUFFER_LENGTH];
            unsigned long len = BUFFER_LENGTH;
            // get MAC address
            unsigned char macBuffer[6];
            eibError = (EIB7_ERROR)EIB7GetMAC(eibHandle, macBuffer);
            CheckEibError(eibError, L"EIB7GetMAC");
            // set MAC address
            UInt64 macAddress = 0;
            for (UInt32 i = 0; i < 6; i ++)
            {
                macAddress |= (UInt64)macBuffer[i] << (i * 8);
            }
            network->MacAddress = macAddress;
            
            // get boot mode
            EIB7_BOOT_MODE bootMode;
            eibError = (EIB7_ERROR)EIB7GetBootMode(eibHandle, &bootMode);
            CheckEibError(eibError, L"EIB7GetBootMode");
            // set boot mode
            network->BootMode = (EIB7_MODE_BOOT)bootMode;
            
            // get serial number
            eibError = (EIB7_ERROR)EIB7GetSerialNumber(eibHandle, buffer, len);
            CheckEibError(eibError, L"EIB7GetSerialNumber");
            // set serial number
            network->SerialNumber = gcnew String(buffer);
            
            // get device ID
            eibError = (EIB7_ERROR)EIB7GetIdentNumber(eibHandle, buffer, len);
            CheckEibError(eibError, L"EIB7GetIdentNumber");
            // set device ID
            network->IdentNumber = gcnew String(buffer);
            
            // get firmware version
            eibError = (EIB7_ERROR)EIB7GetVersion(eibHandle, buffer, len, EIB7_FW_CurrentlyBooted);
            CheckEibError(eibError, L"EIB7GetVersion");
            // set firmware version
            network->FirmwareVersion = gcnew String(buffer);
        }
    #pragma endregion

    #pragma region "Parameter initialization functions"
        /// <summary>
        /// Write EIB operation mode parameters.
        /// </summary>
        /// <param name="opMode">The EIB operation mode parameters.</param>
        void EIB_WriteOpModeParam(EIB_OpMode_Param ^ opMode)
        {
            // set operation mode
            EIB_WriteOpMode(opMode->OpMode);
        }

        /// <summary>
        /// Write EIB operation mode.
        /// </summary>
        /// <param name="opMode">The EIB operation mode.</param>
        void EIB_WriteOpMode(EIB7_MODE_OPERATING opMode)
        {
            EIB7_ERROR eibError;
            // set operation mode
            eibError = (EIB7_ERROR)EIB7SelectMode(eibHandle, (EIB7_OPERATING_MODE)opMode);
            CheckEibError(eibError, L"EIB7SelectMode");
        }

        /// <summary>
        /// Write EIB timestamp period.
        /// </summary>
        /// <param name="opMode">The EIB operation mode parameters.</param>
        void EIB_WriteTimestamp(EIB_OpMode_Param ^ opMode)
        {
            EIB7_ERROR eibError;
            // set timestamp period in 탎
            // get timestamp ticks
            unsigned long ticksPerMicroSec;
            eibError = (EIB7_ERROR)EIB7GetTimestampTicks(eibHandle, &ticksPerMicroSec);
            CheckEibError(eibError, L"EIB7GetTimestampTicks");
            // get timestamp period in 탎
            UInt32 ticks = ticksPerMicroSec * opMode->TimestampPeriod;
            // set timestamp ticks
            eibError = (EIB7_ERROR)EIB7SetTimestampPeriod(eibHandle, ticks);
            CheckEibError(eibError, L"EIB7SetTimestampPeriod");
            // reset timestamp counter
            if (opMode->TimestampReset == true)
            {
                eibError = (EIB7_ERROR)EIB7ResetTimestamp(eibHandle);
                CheckEibError(eibError, L"EIB7ResetTimestamp");
            }
        }
    #pragma endregion

    #pragma region "Axis parameter functions"
        /// <summary>
        /// Write EIB data packet parameters and enble / disable axis timestamp.
        /// </summary>
        /// <param name="parameter">The EIB parameters.</param>
        /// <param name="maxAxis">The max axis number.</param>
        /// <param name="maxAuxAxis">The max auxiliary axis number.</param>
        void EIB_WriteDataPacket(EIB_Param ^ parameter, UInt32 maxAxis, UInt32 maxAuxAxis)
        {
            EIB7_ERROR eibError;
            // packet array with entries for the global data area,
            // for 4 axis and for 1 auxiliary axis
            EIB7_DataPacketSection packet[1 + 4 + 1];
            EIB7_DataRegion region;
            maxAxis    = Math::Min(maxAxis,    UInt32(4));
            maxAuxAxis = Math::Min(maxAuxAxis, UInt32(1));
            UInt32 arrayIndex = 0;
            // activate trigger counter for error checking (must be the first entry)
            eibError = (EIB7_ERROR)EIB7AddDataPacketSection(packet, arrayIndex, EIB7_DR_Global,
                                                            EIB7_PDF_TriggerCounter);
            CheckEibError(eibError, L"EIB7AddDataPacketSection");
            arrayIndex ++;
            // create axis datapacket
            for (UInt32 axis = 0; axis < maxAxis; axis ++)
            {
                // check if axis is activated
                if (   (parameter->Axis[axis]->IsEnabled == true)
                    && (parameter->Axis[axis]->DataPacket != 0))
                {
                    switch (axis)
                    {
                    default: throw gcnew Exception(L"Internal error 3"); break;
                    case 0 : region = EIB7_DR_Encoder1; break;
                    case 1 : region = EIB7_DR_Encoder2; break;
                    case 2 : region = EIB7_DR_Encoder3; break;
                    case 3 : region = EIB7_DR_Encoder4; break;
                    }
                    // create datapacket
                    eibError = (EIB7_ERROR)EIB7AddDataPacketSection(packet, arrayIndex, region,
                                                                    parameter->Axis[axis]->DataPacket);
                    CheckEibError(eibError, L"EIB7AddDataPacketSection");
                    //Debug::Print(String::Format(L"Datapacket region: {0}, value: 0x{1,8:X8}",
                    //                          UInt32(region), parameter->Axis[axis]->DataPacket));
                    arrayIndex ++;
                    // activate / deaktivate axis timestamp
                    EIB7_MODE mode;
                    if (parameter->Axis[axis]->DataPacketTimestamp == true)
                    {
                        // enable timestamp
                        mode = EIB7_MD_Enable;
                    }
                    else
                    {
                        // enable timestamp
                        mode = EIB7_MD_Disable;
                    }
                    eibError = (EIB7_ERROR)EIB7SetTimestamp(axisHandle[axis], mode);
                    CheckEibError(eibError, L"EIB7SetTimestamp", axis + 1);
                }
            }
            // create auxiliary axis datapacket
            for (Int32 auxAxis = 0; auxAxis < Int32(maxAuxAxis); auxAxis ++)
            {
                // check if auxiliary axis is activated
                if (   (parameter->AuxAxis[auxAxis]->IsEnabled == true)
                    && (parameter->AuxAxis[auxAxis]->DataPacket != 0))
                {
                    switch (auxAxis)
                    {
                    default: throw gcnew Exception(L"Internal error 4"); break;
                    case 0 : region = EIB7_DR_AUX; break;
                    }
                    // create datapacket
                    eibError = (EIB7_ERROR)EIB7AddDataPacketSection(packet, arrayIndex, region,
                                                                    parameter->AuxAxis[auxAxis]->DataPacket);
                    CheckEibError(eibError, L"EIB7AddDataPacketSection");
                    //Debug::Print(String::Format(L"Datapacket region: {0}, value: 0x{1,8:X8}",
                    //                          UInt32(region), parameter->AuxAxis[auxAxis]->DataPacket));
                    arrayIndex ++;
                    // activate / deaktivate auxiliary axis timestamp
                    EIB7_MODE mode;
                    if (parameter->AuxAxis[auxAxis]->DataPacketTimestamp == true)
                    {
                        // enable timestamp
                        mode = EIB7_MD_Enable;
                    }
                    else
                    {
                        // enable timestamp
                        mode = EIB7_MD_Disable;
                    }
                    eibError = (EIB7_ERROR)EIB7AuxSetTimestamp(eibHandle, mode);
                    CheckEibError(eibError, L"EIB7AuxSetTimestamp", - (auxAxis + 1));
                }
            }
            // configure datapacket
            eibError = (EIB7_ERROR)EIB7ConfigDataPacket(eibHandle, packet, arrayIndex);
            CheckEibError(eibError, L"EIB7ConfigDataPacket");
        }

        /// <summary>
        /// Write EIB axis termination.
        /// </summary>
        /// <param name="parameter">The EIB parameters.</param>
        void EIB_WriteAxisTermination(EIB_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            eibError = (EIB7_ERROR)EIB7EnableIncrementalTermination(eibHandle,
                                                                    (EIB7_MODE)parameter->AxisTermination);
            CheckEibError(eibError, L"EIB7EnableIncrementalTermination");
        }

        /// <summary>
        /// Write EIB axis parameters.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="parameter">The EIB axis parameters.</param>
        /// <returns>True if axis is enabled.</returns>
        bool EIB_WriteAxisParam(UInt32 axis, EIB_Axis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            //Debug::Print(String::Format(L"InterfaceType: 0x{0,8:X8}, EnDat clock: {1}",
            //                            parameter->InterfaceTypeEib,
            //                            parameter->EnDatClock));
            // initialize axis
            eibError = (EIB7_ERROR)EIB7InitAxis(axisHandle[axis],
                                                parameter->InterfaceTypeEib,
                                                (EIB7_EncoderType)parameter->EncoderType,
                                                (EIB7_Refmarks)parameter->Refmarks,
                                                parameter->Linecounts,
                                                parameter->Increment,
                                                EIB7_HS_None,
                                                EIB7_LS_None,
                                                (EIB7_Compensation)parameter->Compensation,
                                                (EIB7_Bandwidth)parameter->Bandwidth,
                                                parameter->EnDatClock,
                                                (EIB7_EnDatRecoveryTime)parameter->Recovery,
                                                (EIB7_EnDatCalcTime)parameter->Calculation);
            CheckEibError(eibError, L"EIB7InitAxis", axis + 1);
            // check if axis is enabled
            if (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Disabled)
            {
                return false;
            }
            return true;
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Write EIB auxiliary axis parameters.
        /// </summary>
        /// <param name="auxAxis">The auxiliary axis index.</param>
        /// <param name="parameter">The EIB auxiliary axis parameters.</param>
        void EIB_WriteAuxAxisParam(Int32 auxAxis, EIB_AuxAxis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            eibError = (EIB7_ERROR)EIB7SetIntervalCounterInterpolation(eibHandle,
                                        (EIB7_IntervalCounterIPF)parameter->IntervalCounterIpf, 
                                        (EIB7_IntervalCounterEdge)parameter->IntervalCounterEdge);
            CheckEibError(eibError, L"EIB7SetIntervalCounterInterpolation", - (auxAxis + 1));
        }
        #pragma warning(pop)

        /// <summary>
        /// Write EIB EnDat additional information parameters.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="parameter">The EIB axis parameters.</param>
        /// <returns>True if EnDat add info 1 pos. 2 was set.</returns>
        bool EIB_WriteAddInfoParam(UInt32 axis, EIB_Axis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            bool enDatAI1Pos2Set = false;
            // check if axis is activated and is EnDat 2.2
            if (   (parameter->IsEnabled == true)
                && (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22))
            {
                // check for additional information enabled
                if (parameter->AddInfoMode != EIB7_ADD_INFO_MODE::EIB7_AIM_None)
                {
                    // start additional information
                    // check EnDat additional information mode
                    if (parameter->AddInfoMode == EIB7_ADD_INFO_MODE::EIB7_AIM_Fix)
                    {
                        // set EnDat 2.2 fixed additional info
                        eibError = (EIB7_ERROR)EIB7EnDat22SetAddInfo(axisHandle[axis],
                                                                     UInt32(parameter->AddInfo1),
                                                                     UInt32(parameter->AddInfo2));
                        CheckEibError(eibError, L"EIB7EnDat22SetAddInfo", axis + 1);
                    }
                    else if (parameter->AddInfoMode == EIB7_ADD_INFO_MODE::EIB7_AIM_Fifo)
                    {
                        // set EnDat 2.2 fifo additional info
                        UInt32 addInfo1Pos2 = 0;
                        unsigned char data[EIB_MAX_ADD_INFO_NUM];
                        UInt32 addInfoNum = Math::Min(UInt32(parameter->AddInfoNum),
                                                      EIB_MAX_ADD_INFO_NUM);
                        for (UInt32 i = 0; i < addInfoNum; i ++)
                        {
                            data[i] = Byte(parameter->AddInfoFifo[i]);
                            //Debug::Print(String::Format(L"AI({0}): {1}", i, data[i]));
                            // check for add info 1 pos 2 value
                            if (parameter->AddInfoFifo[i] == EIB7_ADD_INFO_ALL::EIB7_AI1_Position2_word1)
                            {
                                addInfo1Pos2 |= 0x01;
                            }
                            if (parameter->AddInfoFifo[i] == EIB7_ADD_INFO_ALL::EIB7_AI1_Position2_word2)
                            {
                                addInfo1Pos2 |= 0x02;
                            }
                            if (parameter->AddInfoFifo[i] == EIB7_ADD_INFO_ALL::EIB7_AI1_Position2_word3)
                            {
                                addInfo1Pos2 |= 0x04;
                            }
                        }
                        // check for add info 1 pos 2 value
                        if (addInfo1Pos2 == 0x07)
                        {
                            enDatAI1Pos2Set = true;
                        }
                        //Debug::Print(String::Format(L"Axis: {0}, EnDatAI1Pos2Set: {1}",
                        //                            axis, enDatAI1Pos2Set));
                        // set add info data
                        eibError = (EIB7_ERROR)EIB7EnDat22SetAddInfoCycle(axisHandle[axis],
                                                                          EIB7_MD_Enable,
                                                                          data,
                                                                          addInfoNum);
                        CheckEibError(eibError, L"EIB7EnDat22SetAddInfoCycle", axis + 1);
                    }
                }
                else
                {
                    // stop additional information
                    // disable EnDat 2.2 fixed additional info
                    eibError = (EIB7_ERROR)EIB7EnDat22SetAddInfo(axisHandle[axis],
                                                                 EIB7_AI1_Stop,
                                                                 EIB7_AI2_Stop);
                    CheckEibError(eibError, L"EIB7EnDat22SetAddInfo", axis + 1);
                    // disable EnDat 2.2 fifo additional info
                    eibError = (EIB7_ERROR)EIB7EnDat22SetAddInfoCycle(axisHandle[axis],
                                                                      EIB7_MD_Disable,
                                                                      0,
                                                                      0);
                    CheckEibError(eibError, L"EIB7EnDat22SetAddInfoCycle", axis + 1);
                }
            }
            return enDatAI1Pos2Set;
        }

        /// <summary>
        /// Read EIB EnDat encoder parameters.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="axisParameter">The EIB axis parameters.</param>
        /// <param name="enDatParameter">The EIB EnDat encoder parameters.</param>
        void EIB_ReadEnDatParam(UInt32 axis, EIB_Axis_Param ^ axisParameter,
                                             EIB_EnDat_Read ^ enDatParameter)
        {
            // Read EIB EnDat encoder parameters
            EIB7_ERROR eibError;
            // check if axis is activated and is EnDat 2.2
            if (axisParameter->IsEnabled == true)
            {
                unsigned char mrs;
                unsigned char addr;
                unsigned short data, dataH;
                // read EnDat parameters
                if (   (axisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat21)
                    || (axisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22))
                {
                    // read EnDat 21 parameters
                    // set EnDat 21 parameter word 13
                    mrs = 0xA1;
                    eibError = (EIB7_ERROR)EIB7EnDat21SelectMemRange(axisHandle[axis], mrs);
                    CheckEibError(eibError, L"EIB7EnDat21SelectMemRange", axis + 1);
                    addr = 0x0D;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &data);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    enDatParameter->ParamE21W13 = data;
                    //Debug::Print(String::Format(L"Axis: {0}", axis + 1));
                    //Debug::Print(String::Format(L"E21W13: 0x{0,4:X4}", enDatParameter->ParamE21W13));
                    // set EnDat 21 parameter word 14
                    addr = 0x0E;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &data);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    enDatParameter->ParamE21W14 = data;
                    //Debug::Print(String::Format(L"E21W14: 0x{0,4:X4}", enDatParameter->ParamE21W14));
                    // set EnDat 21 parameter word 15, 16
                    addr = 0x0F;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &data);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    mrs = 0xA3;
                    eibError = (EIB7_ERROR)EIB7EnDat21SelectMemRange(axisHandle[axis], mrs);
                    CheckEibError(eibError, L"EIB7EnDat21SelectMemRange", axis + 1);
                    addr = 0x00;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &dataH);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    enDatParameter->ParamE21W15_16 = (UInt32(dataH) << 16) + data;
                    //Debug::Print(String::Format(L"E21W15_16: 0x{0,8:X8}", enDatParameter->ParamE21W15_16));
                    // set EnDat 21 parameter word 17
                    addr = 0x01;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &data);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    enDatParameter->ParamE21W17 = data;
                    //Debug::Print(String::Format(L"E21W17: 0x{0,4:X4}", enDatParameter->ParamE21W17));
                    // set EnDat 21 parameter word 20, 21
                    addr = 0x04;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &data);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    addr = 0x05;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &dataH);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    enDatParameter->ParamE21W20_21 = (UInt32(dataH) << 16) + data;
                    //Debug::Print(String::Format(L"E21W20_21: 0x{0,8:X8}", enDatParameter->ParamE21W20_21));
                }
                if (axisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22)
                {
                    // read EnDat 22 parameters
                    unsigned short status;
                    ENCODER_POSITION pos;
                    ENDAT_ADDINFO ai1, ai2;
                    // set EnDat 22 parameter word 27
                    mrs = 0xBD;
                    eibError = (EIB7_ERROR)EIB7EnDat22SelectMemRange(axisHandle[axis],
                                                                     &status, &pos, &ai1, &ai2,
                                                                     mrs, 0);
                    CheckEibError(eibError, L"EIB7EnDat22SelectMemRange", axis + 1);
                    addr = 0x1B;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &data);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    enDatParameter->ParamE22W27 = data;
                    //Debug::Print(String::Format(L"E22W27: 0x{0,4:X4}", enDatParameter->ParamE22W27));
                    // set EnDat 22 parameter word 28, 29
                    addr = 0x1C;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &data);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    addr = 0x1D;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &dataH);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    enDatParameter->ParamE22W28_29 = (UInt32(dataH) << 16) + data;
                    //Debug::Print(String::Format(L"E22W28_29: 0x{0,8:X8}", enDatParameter->ParamE22W28_29));
                    // set EnDat 22 parameter word 34
                    addr = 0x22;
                    eibError = (EIB7_ERROR)EIB7EnDat21ReadMem(axisHandle[axis], addr, &data);
                    CheckEibError(eibError, L"EIB7EnDat21ReadMem", axis + 1);
                    enDatParameter->ParamE22W34 = data;
                    //Debug::Print(String::Format(L"E22W34: 0x{0,4:X4}", enDatParameter->ParamE22W34));
                }
                if (axisParameter->InterfaceTypeEib == UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDat01))
                {
                    // read EnDat 01 parameters
                    unsigned short   statusEnDat, statusIncr;
                    ENCODER_POSITION posEnDat,    posIncr;
                    eibError = (EIB7_ERROR)EIB7ReadEnDatIncrPos(axisHandle[axis],
                                                                &statusEnDat, &posEnDat,
                                                                &statusIncr,  &posIncr);
                    CheckEibError(eibError, L"EIB7ReadEnDatIncrPos", axis + 1);
                    // TODO: synchronisation of incremental and EnDat zero points
                    enDatParameter->EnDat01RefPosIncr  = posIncr;//(posIncr & ~0x0FFF) | 0x0A00;
                    enDatParameter->EnDat01RefPosEnDat = posEnDat;
                    enDatParameter->EnDat01RefPosValid = false;
                    if (   ((statusIncr  & (UInt16)DataPacketStatus::PositionValid) != 0)
                        && ((statusEnDat & (UInt16)DataPacketStatus::PositionValid) != 0))
                    {
                        enDatParameter->EnDat01RefPosValid = true;
                    }
                    //Debug::Print(String::Format(L"E01statEnDat: 0x{0,4:X4}, E01posEnDat: 0x{1,16:X16}",
                    //                            statusEnDat, posEnDat));
                    //Debug::Print(String::Format(L"E01statIncr:  0x{0,4:X4}, E01posIncr:  0x{1,16:X16}",
                    //                            statusIncr, posIncr));
                }
            }
        }
    #pragma endregion

    #pragma region "Trigger functions"
        /// <summary>
        /// Write EIB extern trigger input parameters.
        /// </summary>
        /// <param name="trigger">The extern trigger input index.</param>
        /// <param name="parameter">The EIB trigger parameters.</param>
        void EIB_WriteTriggerInpParam(UInt32 trigger, EIB_Trigger_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            // set trigger input mode and termination
            eibError = (EIB7_ERROR)EIB7InitInput(triggerInpHandle[trigger],
                                                 (EIB7_IOMODE)parameter->TriggerInpMode[trigger],
                                                 (EIB7_MODE)parameter->TriggerInpTerm[trigger]);
            CheckEibError(eibError, L"EIB7InitInput");
            // set trigger input delay
            eibError = (EIB7_ERROR)EIB7SetTriggerInputDelay(triggerInpHandle[trigger],
                                                            parameter->TriggerInpDelay[trigger]);
            CheckEibError(eibError, L"EIB7SetTriggerInputDelay");
        }

        /// <summary>
        /// Write EIB extern trigger output parameters.
        /// </summary>
        /// <param name="trigger">The extern trigger output index.</param>
        /// <param name="parameter">The EIB trigger parameters.</param>
        void EIB_WriteTriggerOutParam(UInt32 trigger, EIB_Trigger_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            // set trigger output mode and activation
            eibError = (EIB7_ERROR)EIB7InitOutput(triggerOutHandle[trigger],
                                                  (EIB7_IOMODE)parameter->TriggerOutMode[trigger],
                                                  (EIB7_MODE)parameter->TriggerOutEnable[trigger]);
            CheckEibError(eibError, L"EIB7InitOutput");
            // set trigger output source
            eibError = (EIB7_ERROR)EIB7OutputTriggerSource(
                                        triggerOutHandle[trigger],
                                        (EIB7_OutputTriggerSrc)parameter->TriggerOutSource[trigger]);
            CheckEibError(eibError, L"EIB7OutputTriggerSource");
        }

        /// <summary>
        /// Write EIB trigger axis parameters.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="parameter">The EIB trigger parameters.</param>
        void EIB_WriteTriggerAxisParam(UInt32 axis, EIB_Trigger_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            // set axis trigger source
            eibError = (EIB7_ERROR)EIB7AxisTriggerSource(
                                        axisHandle[axis],
                                        (EIB7_AxisTriggerSrc)parameter->AxisTriggerSource[axis]);
            CheckEibError(eibError, L"EIB7AxisTriggerSource", axis + 1);
            // set axis trigger edge
            eibError = (EIB7_ERROR)EIB7SetRITriggerEdge(
                                        axisHandle[axis],
                                        (EIB7_RITriggerEdge)parameter->AxisTriggerEdge[axis]);
            CheckEibError(eibError, L"EIB7SetRITriggerEdge", axis + 1);
            //Debug::Print(String::Format(L"Trigger: Axis: {0} Source: {1}, Edge: {2}",
            //                            axis,
            //                            (UInt32)parameter->AxisTriggerSource[axis],
            //                            (UInt32)parameter->AxisTriggerEdge[axis]));
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Write EIB trigger auxiliary axis parameters.
        /// </summary>
        /// <param name="auxAxis">The auxiliary axis index.</param>
        /// <param name="parameter">The EIB trigger parameters.</param>
        void EIB_WriteTriggerAuxAxisParam(Int32 auxAxis, EIB_Trigger_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            // set auxiliary axis trigger source
            eibError = (EIB7_ERROR)EIB7AuxAxisTriggerSource(
                                        eibHandle,
                                        (EIB7_AxisTriggerSrc)parameter->AuxAxisTriggerSource[auxAxis]);
            CheckEibError(eibError, L"EIB7AuxAxisTriggerSource", - (auxAxis + 1));
            // set auxiliary axis trigger edge
            eibError = (EIB7_ERROR)EIB7AuxSetRITriggerEdge(
                                        eibHandle,
                                        (EIB7_RITriggerEdge)parameter->AuxAxisTriggerEdge[auxAxis]);
            CheckEibError(eibError, L"EIB7AuxSetRITriggerEdge", - (auxAxis + 1));
        }
        #pragma warning(pop)

        /// <summary>
        /// Write EIB trigger parameters.
        /// </summary>
        /// <param name="parameter">The EIB trigger parameters.</param>
        void EIB_WriteTriggerParam(EIB_Trigger_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            // set master trigger source
            eibError = (EIB7_ERROR)EIB7MasterTriggerSource(
                                        eibHandle,
                                        (EIB7_AxisTriggerSrc)parameter->MasterTriggerSource);
            CheckEibError(eibError, L"EIB7MasterTriggerSource");
            // set timer period in 탎
            // get timer trigger ticks
            unsigned long ticksPerMicroSec;
            eibError = (EIB7_ERROR)EIB7GetTimerTriggerTicks(eibHandle, &ticksPerMicroSec);
            CheckEibError(eibError, L"EIB7GetTimerTriggerTicks");
            // get timer period in 탎
            UInt32 ticks = ticksPerMicroSec * parameter->TimerTriggerPeriod;
            // set timer trigger ticks
            eibError = (EIB7_ERROR)EIB7SetTimerTriggerPeriod(eibHandle, ticks);
            CheckEibError(eibError, L"EIB7SetTimerTriggerPeriod");
            // reset interval counter to prevent an error
            eibError = (EIB7_ERROR)EIB7SetIntervalCounterTrigger(
                                            eibHandle,
                                            EIB7_ICM_Disable, 
                                            EIB7_ICS_Current,
                                            0,
                                            0);
            CheckEibError(eibError, L"EIB7SetIntervalCounterTrigger");
            // set interval counter
            eibError = (EIB7_ERROR)EIB7SetIntervalCounterTrigger(
                                            eibHandle,
                                            (EIB7_IntervalCounterMode)parameter->IntervalCounterMode, 
                                            (EIB7_IntervalCounterStart)parameter->IntervalCounterStart,
                                            parameter->IntervalCounterStartPos,
                                            parameter->IntervalCounterPeriod);
            CheckEibError(eibError, L"EIB7SetIntervalCounterTrigger");
            // set pulse counter
            eibError = (EIB7_ERROR)EIB7ConfigPulsCounter(
                                            eibHandle,
                                            (EIB7_PulsCounterStart)parameter->PulseCounterStart, 
                                            (EIB7_PulsCounterTrigger)parameter->PulseCounterTrigger,
                                            parameter->PulseCounterCount);
            CheckEibError(eibError, L"EIB7ConfigPulsCounter");
        }

        /// <summary>
        /// Enable EIB trigger sources.
        /// </summary>
        /// <param name="enable">If true enable trigger source else disable.</param>
        /// <param name="triggerSources">All trigger sources to activate.</param>
        void EIB_GlobalTriggerEnable(bool enable, UInt32 triggerSources)
        {
            EIB7_ERROR eibError;
            // set enable mode
            EIB7_MODE mode;
            if (enable == true)
            {
                mode = EIB7_MD_Enable;
            }
            else
            {
                mode = EIB7_MD_Disable;
            }
            eibError = (EIB7_ERROR)EIB7GlobalTriggerEnable(eibHandle, mode, triggerSources);
            CheckEibError(eibError, L"EIB7GlobalTriggerEnable");
            //Debug::Print(String::Format(L"TriggerSources: 0x{0,8:X8}", triggerSources));
        }
    #pragma endregion

    #pragma region "Measurement functions"
        /// <summary>
        /// Clear the system for start.
        /// </summary>
        /// <param name="maxAxis">The max axis number.</param>
        /// <param name="maxAuxAxis">The max auxiliary axis number.</param>
        void EIB_ClearForStart(UInt32 maxAxis, UInt32 maxAuxAxis)
        {
            EIB7_ERROR eibError;
            // clear fifo
            eibError = (EIB7_ERROR)EIB7ClearFIFO(eibHandle);
            CheckEibError(eibError, L"EIB7ClearFIFO");
            // clear polling counter
            for (UInt32 axis = 0; axis < maxAxis + maxAuxAxis; axis ++)
            {
                pollingCounter[axis] = 0;
            }
        }

        /// <summary>
        /// Enable data aquisition for soft realtime or streaming.
        /// </summary>
        /// <param name="enable">If true enable callback function else disable.</param>
        /// <param name="buffer">The measure data entry buffer.</param>
        void EIB_DataAquisitionEnable(bool enable, char * buffer)
        {
            EIB7_ERROR eibError;
            // initialize data callback function
            EIB7_MODE activate;
            if (enable == true)
            {
                activate = EIB7_MD_Enable;
            }
            else
            {
                activate = EIB7_MD_Disable;
            }
            eibError = (EIB7_ERROR)EIB7SetDataCallback(
                                            eibHandle, buffer, activate, 1,
                                            static_cast<EIB7OnDataAvailable>(onDataPtr.ToPointer()));
            CheckEibError(eibError, L"EIB7SetDataCallback");
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Data aquisition callback function.
        /// </summary>
        /// <param name="eibHandle">The EIB handle.</param>
        /// <param name="count">The number of entries to read.</param>
        /// <param name="data">The buffer to read to.</param>
        void OnDataFunction(Int32  eibHandle,
                            UInt32 count,
                            void * data)
        {
            //Debug::Print(String::Format(L"Callback count: 0x{0,8:X8}", count));
        }
        #pragma warning(pop)

        /// <summary>
        /// Read measure data for polling.
        /// Measure data are read to data buffer (display buffer).
        /// </summary>
        /// <param name="parameter">The EIB parameters.</param>
        /// <param name="dataBuffer">The data packet buffer.</param>
        /// <param name="maxAxis">The max axis number.</param>
        /// <param name="maxAuxAxis">The max auxiliary axis number.</param>
        void EIB_ReadPollingData(EIB_Param ^ parameter, DataPacketBuffer ^ dataBuffer,
                                 UInt32 maxAxis, UInt32 maxAuxAxis)
        {
            EIB7_ERROR eibError;
            // read polling measure data
            unsigned short status, counter, adc00, adc90;
            ENCODER_POSITION pos, ref1, ref2, refc;
            unsigned long timestamp;
            ENDAT_ADDINFO ai1, ai2;
            // set data counter
            dataBuffer->ReadDataCount ++;
            // get axis and auxiliary axis datapacket values
            DataPacketData ^ dataPacketData;
            bool isEnabled;
            UInt32 auxAxis;
            for (UInt32 axis = 0; axis < maxAxis + maxAuxAxis; axis ++)
            {
                if (axis < maxAxis)
                {
                    dataPacketData = dataBuffer->Entry[0]->Axis[axis];
                    isEnabled      = parameter->Axis[axis]->IsEnabled;
                }
                else
                {
                    auxAxis        = axis - maxAxis;
                    dataPacketData = dataBuffer->Entry[0]->AuxAxis[auxAxis];
                    isEnabled      = parameter->AuxAxis[auxAxis]->IsEnabled;
                }
                // clear data packet data
                dataPacketData->Clear();
                // check if axis is activated
                if (isEnabled == true)
                {
                    // set axis and auxiliary axis datapacket
                    bool showOnStatusFalse = true;
                    if (axis < maxAxis)
                    {
                        // get axis datapacket
                        // check encoder type
                        switch (parameter->Axis[axis]->InterfaceTypeGui)
                        {
                        default: throw gcnew Exception(L"Internal error 16"); break;
                        case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental:
                        case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u:
                            // get incremental encoder status and position
                            eibError = (EIB7_ERROR)EIB7GetEncoderData(axisHandle[axis],
                                                                      &status, &pos,
                                                                      &ref1, &ref2, &refc,
                                                                      &timestamp, &counter,
                                                                      &adc00, &adc90);
                            CheckEibError(eibError, L"EIB7GetEncoderData", axis + 1);
                            // set timestamp
                            dataPacketData->TimestampValue = timestamp;
                            //String ^ debug = String::Format(L"Timestamp: 0x{0,8:X8}",
                            //                                dataPacketData->TimestampValue);
                            //Debug::Print(debug);
                            // set reference position 1 value
                            if (   (showOnStatusFalse == true)
                                || ((status & (UInt16)DataPacketStatus::RefPos1Valid) != 0))
                            {
                                dataPacketData->ReferencePos1Value = ref1;
                                //String ^ debug = String::Format(L"Ref. pos. 1 value: 0x{0,16:X16}",
                                //                                dataPacketData->ReferencePos1Value);
                                //Debug::Print(debug);
                            }
                            // set reference position 2 value
                            if (   (showOnStatusFalse == true)
                                || ((status & (UInt16)DataPacketStatus::RefPos2Valid) != 0))
                            {
                                dataPacketData->ReferencePos2Value = ref2;
                                //String ^ debug = String::Format(L"Ref. pos. 2 value: 0x{0,16:X16}",
                                //                                dataPacketData->ReferencePos2Value);
                                //Debug::Print(debug);
                            }
                            // set distance coded reference position value
                            if (   (showOnStatusFalse == true)
                                || ((status & (UInt16)DataPacketStatus::RefPosCValid) != 0))
                            {
                                dataPacketData->DistCodedRefValue = refc;
                                //String ^ debug = String::Format(L"Dist. coded ref. pos. value: 0x{0,16:X16}",
                                //                                dataPacketData->DistCodedRefValue);
                                //Debug::Print(debug);
                            }
                            // set analog signal values
                            dataPacketData->AnalogAValue = adc00;
                            dataPacketData->AnalogBValue = adc90;
                            //String ^ debug = String::Format(L"Analog signal values: 0x{0,4:X4}, 0x{1,4:X4}",
                            //                                dataPacketData->AnalogAValue,
                            //                                dataPacketData->AnalogBValue);
                            //Debug::Print(debug);
                            break;
                        case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat21:
                            if (   parameter->Axis[axis]->InterfaceTypeEib
                                != UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDat01))
                            {
                                // get EnDat 2.1 encoder status and position
                                eibError = (EIB7_ERROR)EIB7EnDat21GetPosition(axisHandle[axis],
                                                                              &status, &pos);
                                CheckEibError(eibError, L"EIB7EnDat21GetPosition", axis + 1);
                            }
                            else
                            {
                                // get EnDat 01 encoder status and position
                                eibError = (EIB7_ERROR)EIB7GetPosition(axisHandle[axis],
                                                                       &status, &pos);
                                CheckEibError(eibError, L"EIB7GetPosition", axis + 1);
                            }
                            break;
                        case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22:
                            // set additional information
                            unsigned char mrs = 0;
                            EIB_Axis_Param ^ axisParam = parameter->Axis[axis];
                            UInt32 pollingAIMode;
                            // check EnDat additional information mode
                            if (axisParam->AddInfoMode == EIB7_ADD_INFO_MODE::EIB7_AIM_Fix)
                            {
                                // set EnDat 2.2 fixed additional info
                                pollingAIMode = pollingCounter[axis] % 2;
                                switch (pollingAIMode)
                                {
                                default: throw gcnew Exception(L"Internal error 25"); break;
                                case 0:
                                    mrs = 0x40 + Byte(axisParam->AddInfo1);
                                    break;
                                case 1:
                                    mrs = 0x40 + Byte(axisParam->AddInfo2);
                                    break;
                                }
                            }
                            else if (axisParam->AddInfoMode == EIB7_ADD_INFO_MODE::EIB7_AIM_Fifo)
                            {
                                // set EnDat 2.2 fifo additional info
                                pollingAIMode = pollingCounter[axis] % UInt32(axisParam->AddInfoNum);
                                mrs = 0x40 + Byte(axisParam->AddInfoFifo[pollingAIMode]);
                            }
                            else
                            {
                                // reset EnDat 2.2 additional info
                                pollingAIMode = pollingCounter[axis] % 2;
                                switch (pollingAIMode)
                                {
                                default: throw gcnew Exception(L"Internal error 26"); break;
                                case 0:
                                    mrs = 0x40 + 0x0F;
                                    break;
                                case 1:
                                    mrs = 0x40 + 0x1F;
                                    break;
                                }
                            }
                            // get EnDat 2.2 encoder status, position and additional information 
                            if (mrs == 0)
                            {
                                eibError = (EIB7_ERROR)EIB7EnDat22GetPosition(axisHandle[axis],
                                                                              &status, &pos,
                                                                              &ai1, &ai2);
                                CheckEibError(eibError, L"EIB7EnDat22GetPosition", axis + 1);
                            }
                            else
                            {
                                eibError = (EIB7_ERROR)EIB7EnDat22SelectMemRange(axisHandle[axis],
                                                                                 &status, &pos,
                                                                                 &ai1, &ai2,
                                                                                 mrs, 0);
                                CheckEibError(eibError, L"EIB7EnDat22SelectMemRange", axis + 1);
                                //Debug::Print(String::Format(L"MRS AI {0}: 0x{1,2:X2}", axis, mrs));
                            }
                            // set EnDat additional info 1 values
                            if (   (axisParam->AddInfoMode != EIB7_ADD_INFO_MODE::EIB7_AIM_None)
                                && (   (showOnStatusFalse == true)
                                    || ((ai1.status & (UInt16)DataPacketStatus::EnDatAddInfoValid) != 0)))
                            {
                                dataPacketData->EnDatAI1DataValue = (UInt32(ai1.info) << 16) + ai1.status;
                                //Debug::Print(String::Format(L"EnDat info 1 values: 0x{0,4:X4}, 0x{1,4:X4}",
                                //                            dataPacketData->EnDatAI1StatusValue[0],
                                //                            dataPacketData->EnDatAI1InfoValue[0]));
                                //Debug::Print(String::Format(L"EnDat info 1 values: 0x{0,4:X4}, 0x{1,4:X4}",
                                //                            ai1.status, ai1.info));
                            }
                            // set EnDat additional info 2 values
                            if (   (axisParam->AddInfoMode != EIB7_ADD_INFO_MODE::EIB7_AIM_None)
                                && (   (showOnStatusFalse == true)
                                    || ((ai2.status & (UInt16)DataPacketStatus::EnDatAddInfoValid) != 0)))
                            {
                                dataPacketData->EnDatAI2DataValue = (UInt32(ai2.info) << 16) + ai2.status;
                                //Debug::Print(String::Format(L"EnDat info 2 values: 0x{0,4:X4}, 0x{1,4:X4}",
                                //                            dataPacketData->EnDatAI2StatusValue[0],
                                //                            dataPacketData->EnDatAI2InfoValue[0]));
                                //Debug::Print(String::Format(L"EnDat info 2 values: 0x{0,4:X4}, 0x{1,4:X4}",
                                //                            ai2.status, ai2.info));
                            }
                            break;
                        }
                    }
                    else
                    {
                        // get auxiliary axis datapacket
                        // get auxiliary axis status and position
                        eibError = (EIB7_ERROR)EIB7AuxGetEncoderData(eibHandle,
                                                                     &status, &pos, &ref1,
                                                                     &timestamp, &counter);
                        CheckEibError(eibError, L"EIB7AuxGetEncoderData", axis + 1);
                        // set timestamp
                        dataPacketData->TimestampValue = timestamp;
                        //String ^ debug = String::Format(L"Timestamp: 0x{0,8:X8}",
                        //                                dataPacketData->TimestampValue);
                        //Debug::Print(debug);
                        // set reference position 1 value
                        if (   (showOnStatusFalse == true)
                            || ((status & (UInt16)DataPacketStatus::RefPos1Valid) != 0))
                        {
                            dataPacketData->ReferencePos1Value = ref1;
                            //String ^ debug = String::Format(L"Ref. pos. 1 value: 0x{0,16:X16}",
                            //                                dataPacketData->ReferencePos1Value);
                            //Debug::Print(debug);
                        }
                    }
                    // set axis and auxiliary axis datapacket values
                    // set status word
                    dataPacketData->StatusWordValue = status;
                    //String ^ debug = String::Format(L"Status word: 0x{0,4:X4}",
                    //                                dataPacketData->StatusWordValue);
                    //Debug::Print(debug);
                    // set position data
                    if (   (showOnStatusFalse == true)
                        || ((status & (UInt16)DataPacketStatus::PositionValid) != 0))
                    {
                        dataPacketData->PositionDataValue = pos;
                        // DEBUG: set EnDat CRC error
                        //if (   (axis == 1)
                        //    && (dataBuffer->ReadDataCount > 20))
                        //{
                        //    dataPacketData->StatusWordValue |= (UInt16)DataPacketStatus::ErrorEnDatCRC;
                        //}
                        //String ^ debug = String::Format(L"Position data: 0x{0,16:X16}",
                        //                                dataPacketData->PositionDataValue);
                        //Debug::Print(debug);
                    }
                }
                // increment axis polling counter
                pollingCounter[axis] ++;
            }
        }

        /// <summary>
        /// Read measure buffer state for recording.
        /// </summary>
        /// <param name="entryNumber">The number of entries in the record buffer.</param>
        /// <returns>The fill level of the record buffer in %.</returns>
        UInt32 EIB_ReadRecordingState(UInt32 % entryNumber)
        {
            EIB7_ERROR eibError;
            // get record buffer size
            unsigned long size;
            eibError = (EIB7_ERROR)EIB7GetRecordingMemSize(eibHandle, &size);
            CheckEibError(eibError, L"EIB7GetRecordingMemSize");
            // get record buffer fill level
            unsigned long length, status, progress;
            eibError = (EIB7_ERROR)EIB7GetRecordingStatus(eibHandle, &length, &status, &progress);
            CheckEibError(eibError, L"EIB7GetRecordingStatus");
            entryNumber = length;
            // calculate progress in %
            progress = (length * 100) / (size - 1);
            return progress;
        }

        /// <summary>
        /// Open read measure data for recording.
        /// </summary>
        /// <param name="dataIndex">The index of the measurement value in recording buffer.</param>
        void EIB_OpenRecordingData(Int32 dataIndex)
        {
            EIB7_ERROR eibError;
            // check buffer size
            unsigned long size;
            eibError = (EIB7_ERROR)EIB7SizeOfFIFOEntry(eibHandle, &size);
            CheckEibError(eibError, L"EIB7SizeOfFIFOEntry");
            if (size > EIB_DATA_PACKET_MAX_LENGTH)
            {
                // set data packet too large error exception
                String ^ message = String::Format(resManager->GetString(L"errorDataPacketTooLarge"),
                                                  size,
                                                  EIB_DATA_PACKET_MAX_LENGTH);
                throw gcnew EIB_CtrlException(message,
                                              EIB7_ERROR::EIB7_DataPacketSize,
                                              L"EIB_ReadRecordingData",
                                              parameter->EibName, 0);
            }
            // transfer data packet from recording buffer to fifo
            unsigned long offset, length;
            if (dataIndex < 0)
            {
                offset = 0;
                length = 0xFFFFFFFF;
            }
            else
            {
                // get raw data packet size
                eibError = (EIB7_ERROR)EIB7SizeOfFIFOEntryRaw(eibHandle, &size);
                CheckEibError(eibError, L"EIB7SizeOfFIFOEntryRaw");
                // set size to multiple of 4 byte
                // length = (((size - 1) / 4) + 1) * 4;
                length = (size + 3) & ~0x03;
                offset = length * dataIndex;
            }
            eibError = (EIB7_ERROR)EIB7TransferRecordingData(eibHandle, EIB7_MD_Enable, offset, length);
            CheckEibError(eibError, L"EIB7TransferRecordingData (enable)");
            //Debug::Print(String::Format(L"Index: {0}, Offset: 0x{1,8:X8}, Length: 0x{2,8:X8}",
            //                            dataIndex, offset, length));
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Read measure data for recording.
        /// </summary>
        /// <param name="dataIndex">The index of the measurement value in recording buffer.</param>
        /// <param name="buffer">The measure data entry buffer.</param>
        void EIB_ReadRecordingData(UInt32 dataIndex, char * buffer)
        {
            EIB7_ERROR eibError;
            // get transfer data state
            //unsigned long status, progress;
            //eibError = (EIB7_ERROR)EIB7GetRecordingStatus(eibHandle, &length, &status, &progress);
            //CheckEibError(eibError, L"EIB7GetRecordingStatus");
            //Debug::Print(String::Format(L"Length: {0}, Status: {1}, Progress: {2}",
            //                            length, status, progress));
            // read data packet from fifo to copy buffer
            unsigned long entries;
            eibError = (EIB7_ERROR)EIB7ReadFIFOData(eibHandle, buffer, 1, &entries, 5000);
            CheckEibError(eibError, L"EIB7ReadFIFOData");
            // DEBUG: check data read timeout
            //if (dataIndex > 1000)
            //{
            //    entries = 0;
            //}
            if (entries == 0)
            {
                //unsigned long cnt;
                //eibError = (EIB7_ERROR)EIB7FIFOEntryCount(eibHandle, &cnt);
                //CheckEibError(eibError, L"EIB7FIFOEntryCount");
                //Debug::Print(String::Format(L"FIFO read timeout index: {0}, count: {1}",
                //                            dataIndex, cnt));
                // close read measure data for recording
                try
                {
                    EIB_CloseRecordingData();
                }
                catch (Exception ^ ex)
                {
                    guiSupport->MsgBoxErrorOk(ex->Message);
                }
                // set data read timeout error exception
                String ^ message = String::Format(resManager->GetString(L"errorDataReadTimeout"),
                                                  parameter->EibName);
                throw gcnew EIB_CtrlException(message,
                                              EIB7_ERROR::EIB7_DataReadTimeout,
                                              L"EIB_ReadRecordingData",
                                              parameter->EibName, 0);
            }
            //Debug::Print(String::Format(L"Entry buffer: 0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2}",
            //                            buffer[0], buffer[1], buffer[2], buffer[3]));
        }
        #pragma warning(pop)

        /// <summary>
        /// Close read measure data for recording.
        /// </summary>
        void EIB_CloseRecordingData()
        {
            EIB7_ERROR eibError;
            // disable transfer data
            eibError = (EIB7_ERROR)EIB7TransferRecordingData(eibHandle, EIB7_MD_Disable, 0, 0);
            CheckEibError(eibError, L"EIB7TransferRecordingData (disable)");
        }

        /// <summary>
        /// Read data packet size.
        /// </summary>
        /// <returns>The size of a data packet.</returns>
        UInt32 EIB_ReadDataPacketSize()
        {
            EIB7_ERROR eibError;
            unsigned long size;
            eibError = (EIB7_ERROR)EIB7SizeOfFIFOEntry(eibHandle, &size);
            CheckEibError(eibError, L"EIB7SizeOfFIFOEntry");
            return size;
        }

        /// <summary>
        /// Read data packet data.
        /// </summary>
        /// <param name="buffer">The measure data entry buffer.</param>
        /// <param name="timeout">The wait for data timeout in ms.</param>
        /// <returns>The number of data packets read.</returns>
        UInt32 EIB_ReadDataPacketData(char * buffer, Int32 timeout)
        {
            EIB7_ERROR eibError;
            unsigned long entries;
            eibError = (EIB7_ERROR)EIB7ReadFIFOData(eibHandle, buffer, 1, &entries, timeout);
            CheckEibError(eibError, L"EIB7ReadFIFOData");
            return entries;
        }

        /// <summary>
        /// Convert data packet value from read or copy buffer to display or file buffer value.
        /// </summary>
        /// <param name="rawBuffer">The raw data packet read buffer.</param>
        /// <param name="region">The data region in which to look up the field.</param>
        /// <param name="type">The field to look up.</param>
        /// <param name="value">The pointer to the data packet value.</param>
        /// <param name="sizeofValue">The size of the data packet value.</param>
        /// <returns>True if the data packet entry was found.</returns>
        bool EIB_ReadDataPacketValue(char * rawBuffer,
                                     EIB7_DataRegion region, EIB7_PositionDataField type,
                                     void * value, UInt32 sizeofValue)
        {
            UInt32 valueNumber = 1;
            return EIB_ReadDataPacketValue(rawBuffer, region, type,
                                           value, sizeofValue, valueNumber);
        }

        /// <summary>
        /// Convert data packet value from read or copy buffer to display or file buffer value.
        /// </summary>
        /// <param name="rawBuffer">The raw data packet read buffer.</param>
        /// <param name="region">The data region in which to look up the field.</param>
        /// <param name="type">The field to look up.</param>
        /// <param name="value">The pointer to the data packet value.</param>
        /// <param name="sizeofValue">The size of the data packet value.</param>
        /// <param name="valueNumber">The number of the data packet values.</param>
        /// <returns>True if the data packet entry was found.</returns>
        bool EIB_ReadDataPacketValue(char * rawBuffer,
                                     EIB7_DataRegion region, EIB7_PositionDataField type,
                                     void * value, UInt32 sizeofValue, UInt32 % valueNumber)
        {
            EIB7_ERROR eibError;
            // get data packet value
            void * field;
            unsigned long size;
            eibError = (EIB7_ERROR)EIB7GetDataFieldPtr(eibHandle, rawBuffer, region, type,
                                                       &field, &size);
            if (eibError == EIB7_ERROR::EIB7_FieldNotAvail)
            {
                // data packet entry was not found
                return false;
            }
            CheckEibError(eibError, L"EIB7GetDataFieldPtr");
            if (((sizeofValue * valueNumber) % size) != 0)
            {
                throw gcnew Exception(L"Internal error 15");
            }
            // set data packet entry value
            memcpy_s(value, sizeofValue * valueNumber, field, size);
            valueNumber = size / sizeofValue;
            return true;
        }
    #pragma endregion

    #pragma region "Axis command functions"
        /// <summary>
        /// Start axis reference run.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="startRef">If true then start else stop reference run.</param>
        /// <param name="parameter">The EIB axis parameters.</param>
        void EIB_AxisStartRef(UInt32 axis, bool startRef, EIB_Axis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            if (   (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                || (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u))
            {
                if (startRef == true)
                {
                    EIB7_ReferencePosition ref;
                    if (parameter->Refmarks != EIB7_REFMARKS::EIB7_RM_DistanceCoded)
                    {
                        ref = EIB7_RP_RefPos1;
                    }
                    else
                    {
                        ref = EIB7_RP_RefPos2;
                    }
                    eibError = (EIB7_ERROR)EIB7StartRef(axisHandle[axis], ref);
                    CheckEibError(eibError, L"EIB7StartRef", axis + 1);
                    //Debug::Print(String::Format(L"Axis {0} Start REF {1}", axis, Int32(ref)));
                }
                else
                {
                    eibError = (EIB7_ERROR)EIB7StopRef(axisHandle[axis]);
                    CheckEibError(eibError, L"EIB7StopRef", axis + 1);
                    //Debug::Print(String::Format(L"Axis {0} Stop REF", axis));
                }
            }
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Start auxiliary axis reference run.
        /// </summary>
        /// <param name="axis">The auxiliary axis index.</param>
        /// <param name="startRef">If true then start else stop reference run.</param>
        /// <param name="parameter">The EIB auxiliary axis parameters.</param>
        void EIB_AuxAxisStartRef(UInt32 axis, bool startRef, EIB_AuxAxis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            if (parameter->IsEnabled == true)
            {
                if (startRef == true)
                {
                    eibError = (EIB7_ERROR)EIB7AuxStartRef(eibHandle);
                    CheckEibError(eibError, L"EIB7AuxStartRef", -1);
                    //Debug::Print(String::Format(L"Auxiliary axis {0} Start REF", axis));
                }
                else
                {
                    eibError = (EIB7_ERROR)EIB7AuxStopRef(eibHandle);
                    CheckEibError(eibError, L"EIB7AuxStopRef", -1);
                    //Debug::Print(String::Format(L"Auxiliary axis {0} Stop REF", axis));
                }
            }
        }
        #pragma warning(pop)

        /// <summary>
        /// Check axis reference run state.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="parameter">The EIB axis parameters.</param>
        /// <returns>True if the reference run is active.</returns>
        bool EIB_AxisCheckRef(UInt32 axis, EIB_Axis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            if (   (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                || (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u))
            {
                EIB7_MODE active;
                eibError = (EIB7_ERROR)EIB7GetRefActive(axisHandle[axis], &active);
                CheckEibError(eibError, L"EIB7GetRefActive", axis + 1);
                if (active == EIB7_MD_Enable)
                {
                    return true;
                }
            }
            return false;
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Check auxiliary axis reference run state.
        /// </summary>
        /// <param name="axis">The auxiliary axis index.</param>
        /// <param name="parameter">The EIB auxiliary axis parameters.</param>
        /// <returns>True if the reference run is active.</returns>
        bool EIB_AuxAxisCheckRef(UInt32 axis, EIB_AuxAxis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            if (parameter->IsEnabled == true)
            {
                EIB7_MODE active;
                eibError = (EIB7_ERROR)EIB7AuxGetRefActive(eibHandle, &active);
                CheckEibError(eibError, L"EIB7AuxGetRefActive", -1);
                if (active == EIB7_MD_Enable)
                {
                    return true;
                }
            }
            return false;
        }
        #pragma warning(pop)

        /// <summary>
        /// Clear axis.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="parameter">The EIB axis parameters.</param>
        void EIB_AxisClear(UInt32 axis, EIB_Axis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            if (   (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                || (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u))
            {
                eibError = (EIB7_ERROR)EIB7ClearCounter(axisHandle[axis]);
                CheckEibError(eibError, L"EIB7ClearCounter", axis + 1);
                //Debug::Print(String::Format(L"Axis {0} Clear", axis));
            }
            else if (   (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat21)
                     || (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22))
            {
                eibError = (EIB7_ERROR)EIB7EnDat21ResetEncoder(axisHandle[axis]);
                CheckEibError(eibError, L"EIB7EnDat21ResetEncoder", axis + 1);
                //Thread::Sleep(100);
                eibError = (EIB7_ERROR)EIB7ClearEnDatErrorMsg(axisHandle[axis]);
                CheckEibError(eibError, L"EIB7ClearEnDatErrorMsg", axis + 1);
                //Thread::Sleep(100);
                eibError = (EIB7_ERROR)EIB7ClearEncoderErrors(axisHandle[axis]);
                CheckEibError(eibError, L"EIB7ClearEncoderErrors", axis + 1);
                //Debug::Print(String::Format(L"Axis {0} Clear", axis));
            }
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Clear auxiliary axis.
        /// </summary>
        /// <param name="axis">The auxiliary axis index.</param>
        /// <param name="parameter">The EIB auxiliary axis parameters.</param>
        void EIB_AuxAxisClear(UInt32 axis, EIB_AuxAxis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            if (parameter->IsEnabled == true)
            {
                eibError = (EIB7_ERROR)EIB7AuxClearCounter(eibHandle);
                CheckEibError(eibError, L"EIB7AuxClearCounter", -1);
                //Debug::Print(String::Format(L"Auxiliary axis {0} Clear", axis));
            }
        }
        #pragma warning(pop)

        /// <summary>
        /// Reset axis error.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="resetCmd">The error reset command.</param>
        /// <param name="parameter">The EIB axis parameters.</param>
        void EIB_AxisResetError(UInt32 axis, DataPacketCmd resetCmd, EIB_Axis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            if (parameter->IsEnabled == true)
            {
                switch (resetCmd)
                {
                default: throw gcnew Exception(L"Internal error 18"); break;
                case DataPacketCmd::ResetErrorLostTrigger:
                    eibError = (EIB7_ERROR)EIB7ClearLostTriggerError(axisHandle[axis]);
                    CheckEibError(eibError, L"EIB7ClearLostTriggerError", axis + 1);
                    break;
                case DataPacketCmd::ResetErrorSignal:
                    eibError = (EIB7_ERROR)EIB7ClearEncoderErrors(axisHandle[axis]);
                    CheckEibError(eibError, L"EIB7ClearEncoderErrors", axis + 1);
                    break;
                case DataPacketCmd::ResetErrorPower:
                    eibError = (EIB7_ERROR)EIB7ClearPowerSupplyError(axisHandle[axis]);
                    CheckEibError(eibError, L"EIB7ClearPowerSupplyError", axis + 1);
                    break;
                case DataPacketCmd::ResetErrorRefC:
                    if (   (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                        || (parameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u))
                    {
                        eibError = (EIB7_ERROR)EIB7ClearRefStatus(axisHandle[axis]);
                        CheckEibError(eibError, L"EIB7ClearRefStatus", axis + 1);
                    }
                    break;
                }
                //Debug::Print(String::Format(L"Axis {0} reset error", axis));
            }
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Reset auxiliary axis error.
        /// </summary>
        /// <param name="axis">The auxiliary axis index.</param>
        /// <param name="resetCmd">The error reset command.</param>
        /// <param name="parameter">The EIB auxiliary axis parameters.</param>
        void EIB_AuxAxisResetError(UInt32 axis, DataPacketCmd resetCmd, EIB_AuxAxis_Param ^ parameter)
        {
            EIB7_ERROR eibError;
            if (parameter->IsEnabled == true)
            {
                switch (resetCmd)
                {
                default: throw gcnew Exception(L"Internal error 19"); break;
                case DataPacketCmd::ResetErrorLostTrigger:
                    eibError = (EIB7_ERROR)EIB7AuxClearLostTriggerError(eibHandle);
                    CheckEibError(eibError, L"EIB7AuxClearLostTriggerError", -1);
                    break;
                case DataPacketCmd::ResetErrorSignal:
                    eibError = (EIB7_ERROR)EIB7AuxClearSignalErrors(eibHandle);
                    CheckEibError(eibError, L"EIB7AuxClearSignalErrors", -1);
                    break;
                }
                //Debug::Print(String::Format(L"Auxiliary axis {0} reset error", axis));
            }
        }
        #pragma warning(pop)
    #pragma endregion
    };
    #pragma endregion
}
