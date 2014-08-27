///
/// \file       EIB_ReadParam.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_ReadParam.h,v 1.2 2011/09/20 12:24:35 rohn Exp $
/// $Name:  $
/// $Revision: 1.2 $
/// $State: Exp $
/// $Log: EIB_ReadParam.h,v $
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB list entry classes.
///
/// This class manages the parameters read from an EIB.
/// They will be not be saved to project file.
///

#pragma once

#include "FormMain.h"

using namespace System;

namespace EIB7xx_App
{
 	#pragma region "EIB_Network_Read class definition"
    // default parameter values
    #define EIB_DEFAULT_NETMASK         0xFFFFFF00  // "255.255.255.0"
    #define EIB_DEFAULT_GATEWAY         0xC0A80101  // "192.168.1.1"
    #define EIB_DEFAULT_DHCP_MODE       EIB7_ENABLE_MODE::EIB7_MD_Disable

    /// <summary>
    /// This enum lists all possible boot mode values.
    /// </summary>
    enum class EIB7_MODE_BOOT : UInt32
    {
        EIB7_BM_User        = 0, /*!< User Firmware, User Settings */
        EIB7_BM_FactoryUser = 1, /*!< Factory Firmware, User Settings */
        EIB7_BM_FactoryDefault = 2, /*!< Factory Firmware, Default Settings */
    };

    /// <summary>
    /// This class manages the network parameters read from an EIB.
    /// They will be not be saved to project file.
    /// </summary>
    public ref class EIB_Network_Read
    {
    private:
        GUI_Support ^       guiSupport;         ///< the GUI support class

        bool                readValid;          ///< network parameters valid state

        String ^            hostName;           ///< host name
        UInt32              ipAddress;          ///< IP address
        UInt32              netmask;            ///< netmask
        UInt32              gateway;            ///< gateway
        EIB7_ENABLE_MODE    dhcpMode;           ///< DHCP mode

        UInt64              macAddress;         ///< MAC address
        EIB7_MODE_BOOT      bootMode;           ///< boot mode
        String ^            serialNumber;       ///< serial number
        String ^            identNumber;        ///< device ID
        String ^            firmwareVersion;    ///< firmware version

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
        EIB_Network_Read(GUI_Support ^ guiSupport)
        {
            this->guiSupport = guiSupport;

            readValid = false;

            hostName        = EIB_DEFAULT_HOST_NAME;
            ipAddress       = EIB_DEFAULT_IP_ADDRESS;
            netmask         = EIB_DEFAULT_NETMASK;
            gateway         = EIB_DEFAULT_GATEWAY;
            dhcpMode        = EIB_DEFAULT_DHCP_MODE;

            macAddress      = 0;
            bootMode        = EIB7_MODE_BOOT::EIB7_BM_FactoryDefault;
            serialNumber    = L"";
            identNumber     = L"";
            firmwareVersion = L"";
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Network_Read ^ source)
        {
            guiSupport      = source->guiSupport;
            readValid       = source->readValid;

            hostName        = String::Copy(source->hostName);
            ipAddress       = source->ipAddress;
            netmask         = source->netmask;
            gateway         = source->gateway;
            dhcpMode        = source->dhcpMode;
            
            macAddress      = source->macAddress;
            bootMode        = source->bootMode;
            serialNumber    = String::Copy(source->serialNumber);
            identNumber     = String::Copy(source->identNumber);
            firmwareVersion = String::Copy(source->firmwareVersion);
        }

        /// <summary>
        /// Gets or sets the network parameters valid state.
        /// </summary>
        property bool ReadValid
        {
            bool get() { return readValid; }
            void set(bool value)
            {
                readValid = value;
                // set network parameters write button enable state
                guiSupport->NetworkWriteEnable(readValid);
            }
        }

        /// <summary>
        /// Gets or sets the saved host name parameter.
        /// </summary>
        property String ^ SavedHostName
        {
            String ^ get() { return hostName; }
            void set(String ^ value) { hostName = String::Copy(value); }
        }

        /// <summary>
        /// Gets or sets the saved IP address parameter.
        /// </summary>
        property UInt32 SavedIpAddress
        {
            UInt32 get() { return ipAddress; }
            void set(UInt32 value) { ipAddress = value; }
        }

        /// <summary>
        /// Gets or sets the netmask parameter.
        /// </summary>
        property UInt32 Netmask
        {
            UInt32 get() { return netmask; }
            void set(UInt32 value) { netmask = value; }
        }

        /// <summary>
        /// Gets or sets the gateway parameter.
        /// </summary>
        property UInt32 Gateway
        {
            UInt32 get() { return gateway; }
            void set(UInt32 value) { gateway = value; }
        }

        /// <summary>
        /// Gets or sets the DHCP mode parameter.
        /// </summary>
        property EIB7_ENABLE_MODE SavedDhcpMode
        {
            EIB7_ENABLE_MODE get() { return dhcpMode; }
            void set(EIB7_ENABLE_MODE value) { dhcpMode = value; }
        }

        /// <summary>
        /// Gets or sets the MAC address parameter.
        /// </summary>
        property UInt64 MacAddress
        {
            UInt64 get() { return macAddress; }
            void set(UInt64 value) { macAddress = value; }
        }

        /// <summary>
        /// Gets or sets the boot mode parameter.
        /// </summary>
        property EIB7_MODE_BOOT BootMode
        {
            EIB7_MODE_BOOT get() { return bootMode; }
            void set(EIB7_MODE_BOOT value) { bootMode = value; }
        }

        /// <summary>
        /// Gets or sets the serial number parameter.
        /// </summary>
        property String ^ SerialNumber
        {
            String ^ get() { return serialNumber; }
            void set(String ^ value) { serialNumber = String::Copy(value); }
        }

        /// <summary>
        /// Gets or sets the device ID parameter.
        /// </summary>
        property String ^ IdentNumber
        {
            String ^ get() { return identNumber; }
            void set(String ^ value) { identNumber = String::Copy(value); }
        }

        /// <summary>
        /// Gets or sets the firmware version parameter.
        /// </summary>
        property String ^ FirmwareVersion
        {
            String ^ get() { return firmwareVersion; }
            void set(String ^ value) { firmwareVersion = String::Copy(value); }
        }
    };
	#pragma endregion

 	#pragma region "EIB_EnDat_Read class definition"
    /// <summary>
    /// This class manages the EnDat parameters read from an EIB.
    /// They will be not be saved to project file.
    /// </summary>
    public ref class EIB_EnDat_Read
    {
    private:
        UInt16                  paramE21W13;        ///< EnDat 21 parameter word 13
        UInt16                  paramE21W14;        ///< EnDat 21 parameter word 14
        UInt32                  paramE21W15_16;     ///< EnDat 21 parameter word 15, 16
        UInt16                  paramE21W17;        ///< EnDat 21 parameter word 17
        UInt32                  paramE21W20_21;     ///< EnDat 21 parameter word 20, 21
        UInt16                  paramE22W27;        ///< EnDat 22 parameter word 27
        UInt32                  paramE22W28_29;     ///< EnDat 22 parameter word 28, 29
        UInt16                  paramE22W34;        ///< EnDat 22 parameter word 34
        bool                    enDat01RefPosValid; ///< EnDat 01 reference position valid flag
        ENCODER_POSITION        enDat01RefPosIncr;  ///< EnDat 01 incremental reference position value
        ENCODER_POSITION        enDat01RefPosEnDat; ///< EnDat 01 EnDat reference position value
        PositionData            enDat01RefPosData;  ///< EnDat 01 reference position data

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_EnDat_Read()
        {
            enDat01RefPosValid = false;
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 21 parameter word 13.
        /// </summary>
        property UInt16 ParamE21W13
        {
            UInt16 get() { return paramE21W13; }
            void set(UInt16 value) { paramE21W13 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 21 parameter word 14.
        /// </summary>
        property UInt16 ParamE21W14
        {
            UInt16 get() { return paramE21W14; }
            void set(UInt16 value) { paramE21W14 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 21 parameter word 15, 16.
        /// </summary>
        property UInt32 ParamE21W15_16
        {
            UInt32 get() { return paramE21W15_16; }
            void set(UInt32 value) { paramE21W15_16 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 21 parameter word 17.
        /// </summary>
        property UInt16 ParamE21W17
        {
            UInt16 get() { return paramE21W17; }
            void set(UInt16 value) { paramE21W17 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 21 parameter word 20, 21.
        /// </summary>
        property UInt32 ParamE21W20_21
        {
            UInt32 get() { return paramE21W20_21; }
            void set(UInt32 value) { paramE21W20_21 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 22 parameter word 27.
        /// </summary>
        property UInt16 ParamE22W27
        {
            UInt16 get() { return paramE22W27; }
            void set(UInt16 value) { paramE22W27 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 22 parameter word 28, 29.
        /// </summary>
        property UInt32 ParamE22W28_29
        {
            UInt32 get() { return paramE22W28_29; }
            void set(UInt32 value) { paramE22W28_29 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 22 parameter word 34.
        /// </summary>
        property UInt16 ParamE22W34
        {
            UInt16 get() { return paramE22W34; }
            void set(UInt16 value) { paramE22W34 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 01 reference position valid flag.
        /// </summary>
        property bool EnDat01RefPosValid
        {
            bool get() { return enDat01RefPosValid; }
            void set(bool value) { enDat01RefPosValid = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 01 incremental reference position value.
        /// </summary>
        property ENCODER_POSITION EnDat01RefPosIncr
        {
            ENCODER_POSITION get() { return enDat01RefPosIncr; }
            void set(ENCODER_POSITION value) { enDat01RefPosIncr = value; }
        }

        /// <summary>
        /// Gets / sets the EIB EnDat 01 EnDat reference position value.
        /// </summary>
        property ENCODER_POSITION EnDat01RefPosEnDat
        {
            ENCODER_POSITION get() { return enDat01RefPosEnDat; }
            void set(ENCODER_POSITION value) { enDat01RefPosEnDat = value; }
        }

        /// <summary>
        /// Gets the EIB EnDat 01 reference position data.
        /// </summary>
        property PositionData ^ EnDat01RefPosData
        {
            PositionData ^ get() { return %enDat01RefPosData; }
        }
    };
	#pragma endregion
}
