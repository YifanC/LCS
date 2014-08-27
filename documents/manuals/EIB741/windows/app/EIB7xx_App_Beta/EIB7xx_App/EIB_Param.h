///
/// \file       EIB_Param.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_Param.h,v 1.3 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: EIB_Param.h,v $
/// Revision 1.3  2011/12/16 13:37:56  rohn
/// Version 0.9.0.1 (siehe History.txt)
///
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB parameter classes.
///
/// This classes contain all EIB parameters sent to an EIB
/// during initialization and saved into a project file.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Net;

namespace EIB7xx_App
{
 	#pragma region "Common parameter definitions"
    // special definitions
    #define EIB_MAX_AXIS_NUMBER             4
    #define EIB_MAX_AUX_AXIS_NUMBER         1
    #define EIB_MAX_ALL_AXIS_NUMBER         (EIB_MAX_AXIS_NUMBER + EIB_MAX_AUX_AXIS_NUMBER)
    #define EIB_MAX_TRIGGER_INP_NUMBER      4
    #define EIB_MAX_TRIGGER_OUT_NUMBER      4

    /// <summary>
    /// This enum lists enable / disable values.
    /// </summary>
    enum class EIB7_ENABLE_MODE : UInt32
    {
        EIB7_MD_Disable = 0, /*!< */
        EIB7_MD_Enable = 1   /*!< */
    };
	#pragma endregion

 	#pragma region "EIB_Network_Param class definition"
    // default parameters
    #define EIB_DEFAULT_USE_DHCP        false
    #define EIB_DEFAULT_HOST_NAME       (L"EIB7xx")
    #define EIB_DEFAULT_IP_ADDRESS      0xC0A80102  // "192.168.1.2"

    /// <summary>
    /// This class contains all EIB network parameters.
    /// </summary>
    public ref class EIB_Network_Param
    {
    private:
        UInt32      fileVersion;    ///< parameter version for parameter file

        bool        useDhcp;        ///< use DHCP flag
        String ^    hostName;       ///< host name
        UInt32      ipAddress;      ///< IP address

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_Network_Param()
        {
            fileVersion = 1;

            useDhcp     = EIB_DEFAULT_USE_DHCP;
            hostName    = EIB_DEFAULT_HOST_NAME;
            ipAddress   = EIB_DEFAULT_IP_ADDRESS;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Network_Param ^ source)
        {
            useDhcp   = source->useDhcp;
            hostName  = String::Copy(source->hostName);
            ipAddress = source->ipAddress;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_Network_Param ^ source)
        {
            if (useDhcp   != source->useDhcp)   return false;
            if (hostName  != source->hostName)  return false;
            if (ipAddress != source->ipAddress) return false;
            return true;
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
		/// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(EIB_ParamFile ^ paramFile)
        {
            // read / write parameter version
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            paramFile->ReadWriteBool(useDhcp);
            paramFile->ReadWriteString(hostName);
            paramFile->ReadWriteUInt32(ipAddress);
        }

        /// <summary>
        /// Gets / sets the use DHCP flag.
        /// </summary>
        property bool CurrentDhcpMode
        {
            bool get() { return useDhcp; }
            void set(bool value) { useDhcp = value; }
        }

        /// <summary>
        /// Gets or sets the current host name parameter.
        /// </summary>
        property String ^ CurrentHostName
        {
            String ^ get() { return hostName; }
            void set(String ^ value) { hostName = String::Copy(value); }
        }

        /// <summary>
        /// Gets / sets the current IP address.
        /// </summary>
        property UInt32 CurrentIpAddress
        {
            UInt32 get() { return ipAddress; }
            void set(UInt32 value) { ipAddress = value; }
        }
    };
	#pragma endregion

 	#pragma region "EIB_OpMode_Param class definition"
    // default parameters
    #define EIB_DEFAULT_OP_MODE                 EIB7_MODE_OPERATING::EIB7_OM_SoftRealtime
    #define EIB_DEFAULT_TIMESTAMP_PERIOD        UInt32(         1)
    #define EIB_MIN_TIMESTAMP_PERIOD            UInt32(         1)
    #define EIB_MAX_TIMESTAMP_PERIOD            UInt32(0xFFFFFFFF)
    #define EIB_STEP_TIMESTAMP_PERIOD           UInt32(         1)
    #define EIB_DEFAULT_TIMESTAMP_RESET         true

    /// <summary>
    /// This enum lists all EIB operation modes.
    /// </summary>
    enum class EIB7_MODE_OPERATING : UInt32
    {
        EIB7_OM_Polling = 0,           /*!< */
        EIB7_OM_SoftRealtime = 1,      /*!< */
        EIB7_OM_Streaming = 2,         /*!< */
        EIB7_OM_RecordingSingle = 3,   /*!< */
        EIB7_OM_RecordingRoll = 4      /*!< */
    };

    /// <summary>
    /// This class contains all EIB operation mode parameters.
    /// </summary>
    public ref class EIB_OpMode_Param
    {
    private:
        UInt32              fileVersion;        ///< parameter version for parameter file

        EIB7_MODE_OPERATING opMode;             ///< EIB operation mode

        UInt32              timestampPeriod;    ///< EIB timestamp period in micro seconds
        bool                timestampReset;     ///< EIB timestamp reset on start

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_OpMode_Param()
        {
            fileVersion = 1;

            opMode = EIB_DEFAULT_OP_MODE;

            timestampPeriod = EIB_DEFAULT_TIMESTAMP_PERIOD;
            timestampReset  = EIB_DEFAULT_TIMESTAMP_RESET;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_OpMode_Param ^ source)
        {
            opMode          = source->opMode;
            timestampPeriod = source->timestampPeriod;
            timestampReset  = source->timestampReset;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_OpMode_Param ^ source)
        {
            if (opMode          != source->opMode)          return false;
            if (timestampPeriod != source->timestampPeriod) return false;
            if (timestampReset  != source->timestampReset)  return false;
            return true;
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
		/// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(EIB_ParamFile ^ paramFile)
        {
            // read / write parameter version
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            opMode = (EIB7_MODE_OPERATING)paramFile->ReadWriteUInt32(UInt32(opMode));
            paramFile->ReadWriteUInt32(timestampPeriod);
            paramFile->ReadWriteBool(timestampReset);
        }

        /// <summary>
        /// Gets / sets the EIB operation mode.
        /// </summary>
        property EIB7_MODE_OPERATING OpMode
        {
            EIB7_MODE_OPERATING get() { return opMode; }
            void set(EIB7_MODE_OPERATING value) { opMode = value; }
        }

        /// <summary>
        /// Gets / sets the EIB timestamp period in micro seconds.
        /// </summary>
        property UInt32 TimestampPeriod
        {
            UInt32 get() { return timestampPeriod; }
            void set(UInt32 value) { timestampPeriod = value; }
        }

        /// <summary>
        /// Gets / sets the EIB timestamp reset behavior.
        /// </summary>
        property bool TimestampReset
        {
            bool get() { return timestampReset; }
            void set(bool value) { timestampReset = value; }
        }
    };
	#pragma endregion

 	#pragma region "EIB_Axis_Base class definition"
    // default parameters
    #define EIB_DEFAULT_DATA_PACKET     (UInt32)(  EIB7_POSITION_DATA_FIELD::EIB7_PDF_StatusWord \
                                                 | EIB7_POSITION_DATA_FIELD::EIB7_PDF_PositionData \
                                                 | EIB7_POSITION_DATA_FIELD::EIB7_PDF_Timestamp)

    /// <summary>
    /// This enum lists all EIB axis data regions.
    /// </summary>
    enum class EIB7_DATA_REGION : UInt32
    {
        EIB7_DR_Global          = 0,    /*!< The field is in the global data area */ 
        EIB7_DR_Encoder1        = 1,    /*!< The field is in the encoder 1 region */
        EIB7_DR_Encoder2        = 2,    /*!< The field is in the encoder 2 region */
        EIB7_DR_Encoder3        = 3,    /*!< The field is in the encoder 3 region */
        EIB7_DR_Encoder4        = 4,    /*!< The field is in the encoder 4 region */
        EIB7_DR_AUX             = 9     /*!< The field is in the auxiliary axis region */
    };

    /// <summary>
    /// This enum lists all EIB axis position data field entrys.
    /// </summary>
    enum class EIB7_POSITION_DATA_FIELD : UInt32
    {
        EIB7_PDF_EndOfList      = 0,      /*!< This field type marks the end of the list of data fields */
        EIB7_PDF_TriggerCounter = 0x0001, /*!< Trigger counter of the EIB (global) */ 
        EIB7_PDF_StatusWord     = 0x0002, /*!< per encoder */
        EIB7_PDF_PositionData   = 0x0004, /*!< per encoder */
        EIB7_PDF_Timestamp      = 0x0008, /*!< per encoder */
        EIB7_PDF_ReferencePos   = 0x0010, /*!< per encoder */
        EIB7_PDF_DistCodedRef   = 0x0020, /*!< per encoder */
        EIB7_PDF_Analog         = 0x0040, /*!< per encoder */
        EIB7_PDF_EnDat_AI1      = 0x0080, /*!< per encoder */
        EIB7_PDF_EnDat_AI2      = 0x0100  /*!< per encoder */
    };

    /// <summary>
    /// This class is the EIB axis parameters base class.
    /// </summary>
    public ref class EIB_Axis_Base
    {
    private:
        UInt32      fileVersion;    ///< parameter version for parameter file

        UInt32      dataPacket;     ///< EIB axis data packet

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_Axis_Base()
        {
            fileVersion = 1;

            dataPacket = EIB_DEFAULT_DATA_PACKET;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Axis_Base ^ source)
        {
            dataPacket = source->dataPacket;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_Axis_Base ^ source)
        {
            if (dataPacket != source->dataPacket) return false;
            return true;
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
		/// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(EIB_ParamFile ^ paramFile)
        {
            // read / write parameter version
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            paramFile->ReadWriteUInt32(dataPacket);
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet.
        /// </summary>
        property UInt32 DataPacket
        {
            UInt32 get() { return dataPacket; }
            void set(UInt32 value) { dataPacket = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet status value.
        /// </summary>
        property bool DataPacketStatusWord
        {
            bool get()
            {
                if ((dataPacket & UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_StatusWord)) == 0)
                {
                    return false;
                }
                return true;
            }
            void set(bool value)
            {
                UInt32 valueMask = UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_StatusWord);
                if (value == false)
                {
                    dataPacket &= ~valueMask;
                }
                else
                {
                    dataPacket |=  valueMask;
                }
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet position value.
        /// </summary>
        property bool DataPacketPositionData
        {
            bool get()
            {
                if ((dataPacket & UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_PositionData)) == 0)
                {
                    return false;
                }
                return true;
            }
            void set(bool value)
            {
                UInt32 valueMask = UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_PositionData);
                if (value == false)
                {
                    dataPacket &= ~valueMask;
                }
                else
                {
                    dataPacket |=  valueMask;
                }
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet timestamp value.
        /// </summary>
        property bool DataPacketTimestamp
        {
            bool get()
            {
                if ((dataPacket & UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_Timestamp)) == 0)
                {
                    return false;
                }
                return true;
            }
            void set(bool value)
            {
                UInt32 valueMask = UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_Timestamp);
                if (value == false)
                {
                    dataPacket &= ~valueMask;
                }
                else
                {
                    dataPacket |=  valueMask;
                }
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet reference position value.
        /// </summary>
        property bool DataPacketReferencePos
        {
            bool get()
            {
                if ((dataPacket & UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_ReferencePos)) == 0)
                {
                    return false;
                }
                return true;
            }
            void set(bool value)
            {
                UInt32 valueMask = UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_ReferencePos);
                if (value == false)
                {
                    dataPacket &= ~valueMask;
                }
                else
                {
                    dataPacket |=  valueMask;
                }
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet distance coded reference value.
        /// </summary>
        property bool DataPacketDistCodedRef
        {
            bool get()
            {
                if ((dataPacket & UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_DistCodedRef)) == 0)
                {
                    return false;
                }
                return true;
            }
            void set(bool value)
            {
                UInt32 valueMask = UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_DistCodedRef);
                if (value == false)
                {
                    dataPacket &= ~valueMask;
                }
                else
                {
                    dataPacket |=  valueMask;
                }
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet analog value.
        /// </summary>
        property bool DataPacketAnalog
        {
            bool get()
            {
                if ((dataPacket & UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_Analog)) == 0)
                {
                    return false;
                }
                return true;
            }
            void set(bool value)
            {
                UInt32 valueMask = UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_Analog);
                if (value == false)
                {
                    dataPacket &= ~valueMask;
                }
                else
                {
                    dataPacket |=  valueMask;
                }
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet additional information 1 value.
        /// </summary>
        property bool DataPacketEnDatAI1
        {
            bool get()
            {
                if ((dataPacket & UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_EnDat_AI1)) == 0)
                {
                    return false;
                }
                return true;
            }
            void set(bool value)
            {
                UInt32 valueMask = UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_EnDat_AI1);
                if (value == false)
                {
                    dataPacket &= ~valueMask;
                }
                else
                {
                    dataPacket |=  valueMask;
                }
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis data packet additional information 2 value.
        /// </summary>
        property bool DataPacketEnDatAI2
        {
            bool get()
            {
                if ((dataPacket & UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_EnDat_AI2)) == 0)
                {
                    return false;
                }
                return true;
            }
            void set(bool value)
            {
                UInt32 valueMask = UInt32(EIB7_POSITION_DATA_FIELD::EIB7_PDF_EnDat_AI2);
                if (value == false)
                {
                    dataPacket &= ~valueMask;
                }
                else
                {
                    dataPacket |=  valueMask;
                }
            }
        }
    };
	#pragma endregion

 	#pragma region "EIB_Axis_Param class definition"
    // default parameters
    #define EIB_DEFAULT_INTERFACE_TYPE_GUI  EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Disabled
    #define EIB_DEFAULT_BANDWIDTH_1V        EIB7_BANDWIDTH::EIB7_BW_High
    #define EIB_DEFAULT_BANDWIDTH_11MY      EIB7_BANDWIDTH::EIB7_BW_Low
    #define EIB_DEFAULT_COMPENSATION        EIB7_COMPENSATION::EIB7_CS_CompActive
    #define EIB_DEFAULT_REFMARKS            EIB7_REFMARKS::EIB7_RM_None
    #define EIB_DEFAULT_ENCODER_TYPE        EIB7_ENCODER_TYPE::EIB7_EC_Rotary
    #define EIB_DEFAULT_LINECOUNTS          UInt32(      5000)
    #define EIB_MIN_LINECOUNTS              UInt32(       100)
    #define EIB_MAX_LINECOUNTS              UInt32(0xFFFFFFFF)
    #define EIB_STEP_LINECOUNTS             UInt32(         1)
    #define EIB_DEFAULT_INCREMENT           UInt32(      1000)
    #define EIB_MIN_INCREMENT               UInt32(         2)
    #define EIB_MAX_INCREMENT               UInt32(    100000)
    #define EIB_STEP_INCREMENT              UInt32(         2)
    #define EIB_DEFAULT_SIGNAL_PERIOD       Double(        20.000)
    #define EIB_MIN_SIGNAL_PERIOD           Double(         0.001)
    #define EIB_MAX_SIGNAL_PERIOD           Double(     10000.000)
    #define EIB_STEP_SIGNAL_PERIOD          Double(         0.001)
    #define EIB_FORMAT_SIGNAL_PERIOD        L"F3"
    #define EIB_DEFAULT_INC_SIGNALS_21      false
    #define EIB_DEFAULT_INC_SIGNALS_22      false
    #define EIB_DEFAULT_INTERFACE_CLOCK_21  EIB7_INTERFACE_CLOCK_21::EIB7_CLK_300KHz
    #define EIB_DEFAULT_INTERFACE_CLOCK_22  EIB7_INTERFACE_CLOCK_22::EIB7_CLK_4MHz
    #define EIB_DEFAULT_RUNTIME_COMP_21     false
    #define EIB_DEFAULT_RUNTIME_COMP_22     true
    #define EIB_DEFAULT_RECOVERY_TIME       EIB7_RECOVERY_TIME::EIB7_RT_Long
    #define EIB_DEFAULT_ENDAT_CALC_TIME_21  EIB7_ENDAT_CALC_TIME::EIB7_CT_Long
    #define EIB_DEFAULT_ENDAT_CALC_TIME_22  EIB7_ENDAT_CALC_TIME::EIB7_CT_Short
    #define EIB_DEFAULT_ADD_INFO_MODE       EIB7_ADD_INFO_MODE::EIB7_AIM_None
    #define EIB_DEFAULT_ADD_INFO_1          EIB7_ADD_INFO_1::EIB7_AI1_NOP
    #define EIB_DEFAULT_ADD_INFO_2          EIB7_ADD_INFO_2::EIB7_AI2_NOP
    #define EIB_DEFAULT_ADD_INFO_NUM        EIB7_ADD_INFO_NUM::EIB7_AIN_3
    #define EIB_MAX_ADD_INFO_NUM            UInt32(10)
    #define EIB_DEFAULT_ADD_INFO_FIFO       EIB7_ADD_INFO_ALL::EIB7_AI1_NOP

    /// <summary>
    /// This enum lists all EIB axis interface types for GUI input.
    /// </summary>
    enum class EIB7_INTERFACE_TYPE_GUI : UInt32
    {
        EIB7_IT_Disabled              = 10, /*!< To disable the axis */
        EIB7_IT_Incremental           = 11, /*!< For incremental encoders (1Vss) */
        EIB7_IT_Incremental_11u       = 12, /*!< For incremental encoders (11uA) */
        EIB7_IT_EnDat21               = 13, /*!< EnDat 2.1 protocol */
        EIB7_IT_EnDat22               = 14, /*!< EnDat 2.2 protocol */
    };

    /// <summary>
    /// This enum lists all EIB axis interface types for EIB setting.
    /// </summary>
    enum class EIB7_INTERFACE_TYPE_EIB : UInt32
    {
        EIB7_IT_Disabled              = 0, /*!< To disable the axis */
        EIB7_IT_Incremental           = 1, /*!< For incremental encoders (1Vss) */
        EIB7_IT_EnDat21               = 2, /*!< EnDat 2.1 protocol */
        EIB7_IT_EnDat01               = 3, /*!< EnDat 0.1 protocol */
        EIB7_IT_EnDat22               = 4, /*!< EnDat 2.2 protocol */
        EIB7_IT_EnDatDelayMeasurement = 8, /*!< only available for EnDat 2.2 protocol */
        EIB7_IT_Incremental_11u    = 2049  /*!< For incremental encoders (11uA) */
    };

    /// <summary>
    /// This enum lists all EIB axis encoder's bandwidth.
    /// </summary>
    enum class EIB7_BANDWIDTH : UInt32
    {
        EIB7_BW_High  = 0,/*!< */
        EIB7_BW_Low   = 1 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis online compensation states.
    /// </summary>
    enum class EIB7_COMPENSATION : UInt32
    {
        EIB7_CS_None       = 0,/*!< */
        EIB7_CS_CompActive = 1 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis reference mark types.
    /// </summary>
    enum class EIB7_REFMARKS : UInt32
    {
          EIB7_RM_None          = 0, /*!< */
          EIB7_RM_One           = 1, /*!< */
          EIB7_RM_DistanceCoded = 2  /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis encoder types.
    /// </summary>
    enum class EIB7_ENCODER_TYPE : UInt32
    {
        EIB7_EC_Linear = 0, /*!< */
        EIB7_EC_Rotary = 1  /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat 2.1 interface clocks.
    /// </summary>
    enum class EIB7_INTERFACE_CLOCK_21 : UInt32
    {
//      EIB7_CLK_Default = 0, /*!< */
        EIB7_CLK_100KHz  = 100000, /*!< */
        EIB7_CLK_300KHz  = 300000, /*!< */
        EIB7_CLK_500KHz  = 500000, /*!< */
        EIB7_CLK_1MHz    = 1000000, /*!< */
        EIB7_CLK_2MHz    = 2000000, /*!< */
//      EIB7_CLK_4MHz    = 4000000, /*!< */
//      EIB7_CLK_5MHz    = 5000000, /*!< */
//      EIB7_CLK_6_66MHz = 6666666 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat 2.2 interface clocks.
    /// </summary>
    enum class EIB7_INTERFACE_CLOCK_22 : UInt32
    {
//      EIB7_CLK_Default = 0, /*!< */
        EIB7_CLK_100KHz  = 100000, /*!< */
        EIB7_CLK_300KHz  = 300000, /*!< */
        EIB7_CLK_500KHz  = 500000, /*!< */
        EIB7_CLK_1MHz    = 1000000, /*!< */
        EIB7_CLK_2MHz    = 2000000, /*!< */
        EIB7_CLK_4MHz    = 4000000, /*!< */
        EIB7_CLK_5MHz    = 5000000, /*!< */
        EIB7_CLK_6_66MHz = 6666666 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat recovery time I states.
    /// </summary>
    enum class EIB7_RECOVERY_TIME : UInt32
    {
        EIB7_RT_Long       = 0,/*!< */
        EIB7_RT_Short      = 1 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat calculation time states.
    /// </summary>
    enum class EIB7_ENDAT_CALC_TIME : UInt32
    {
        EIB7_CT_Long       = 0,/*!< */
        EIB7_CT_Short      = 1 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat additional information modes.
    /// </summary>
    enum class EIB7_ADD_INFO_MODE : UInt32
    {
        EIB7_AIM_None   = 0x00,     /*!< no additional information */ 
        EIB7_AIM_Fix    = 0x01,     /*!< additional information 1 and 2 */ 
        EIB7_AIM_Fifo   = 0x02,     /*!< additional information in fifo */ 
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat additional information numbers.
    /// </summary>
    enum class EIB7_ADD_INFO_NUM : UInt32
    {
        EIB7_AIN_1  =  1,
        EIB7_AIN_2,
        EIB7_AIN_3,
        EIB7_AIN_4,
        EIB7_AIN_5,
        EIB7_AIN_6,
        EIB7_AIN_7,
        EIB7_AIN_8,
        EIB7_AIN_9,
        EIB7_AIN_10,
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat additional information values.
    /// </summary>
    enum class EIB7_ADD_INFO_ALL : UInt32
    {
        EIB7_AI1_NOP                   = 0x00,      /*!< */ 
        EIB7_AI1_Diagnostic            = 0x01,      /*!< */ 
        EIB7_AI1_Position2_word1       = 0x02,      /*!< */ 
        EIB7_AI1_Position2_word2       = 0x03,      /*!< */ 
        EIB7_AI1_Position2_word3       = 0x04,      /*!< */ 
        EIB7_AI1_MemoryLSB             = 0x05,      /*!< */ 
        EIB7_AI1_MemoryMSB             = 0x06,      /*!< */ 
        EIB7_AI1_MRS                   = 0x07,      /*!< */ 
        EIB7_AI1_TestCommand           = 0x08,      /*!< */ 
        EIB7_AI1_TestValue_word1       = 0x09,      /*!< */ 
        EIB7_AI1_TestValue_word2       = 0x0A,      /*!< */ 
        EIB7_AI1_TestValue_word3       = 0x0B,      /*!< */ 
        EIB7_AI1_Temperature1          = 0x0C,      /*!< */ 
        EIB7_AI1_Temperature2          = 0x0D,      /*!< */ 
        EIB7_AI1_AddSensor             = 0x0E,      /*!< */ 
//      EIB7_AI1_Stop                  = 0x0F,      /*!< */ 
        EIB7_AI2_NOP                   = 0x10,      /*!< */ 
        EIB7_AI2_Commutation           = 0x11,      /*!< */ 
        EIB7_AI2_Acceleration          = 0x12,      /*!< */ 
        EIB7_AI2_CommAndAccel          = 0x13,      /*!< */ 
        EIB7_AI2_LimitSignal           = 0x14,      /*!< */ 
        EIB7_AI2_LimitAndAccel         = 0x15,      /*!< */ 
        EIB7_AI2_AsyncPos_word1        = 0x16,      /*!< */ 
        EIB7_AI2_AsyncPos_word2        = 0x17,      /*!< */ 
        EIB7_AI2_AsyncPos_word3        = 0x18,      /*!< */ 
        EIB7_AI2_OPSErrorSources       = 0x19,      /*!< */ 
        EIB7_AI2_ReservedA             = 0x1A,      /*!< */
        EIB7_AI2_ReservedB             = 0x1B,      /*!< */
        EIB7_AI2_ReservedC             = 0x1C,      /*!< */
        EIB7_AI2_ReservedD             = 0x1D,      /*!< */
        EIB7_AI2_ReservedE             = 0x1E,      /*!< */
//      EIB7_AI2_Stop                  = 0x1F,      /*!< */ 
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat additional information 1 values.
    /// </summary>
    enum class EIB7_ADD_INFO_1 : UInt32
    {
        EIB7_AI1_NOP                   = 0x00,      /*!< */ 
        EIB7_AI1_Diagnostic            = 0x01,      /*!< */ 
        EIB7_AI1_Position2_word1       = 0x02,      /*!< */ 
        EIB7_AI1_Position2_word2       = 0x03,      /*!< */ 
        EIB7_AI1_Position2_word3       = 0x04,      /*!< */ 
        EIB7_AI1_MemoryLSB             = 0x05,      /*!< */ 
        EIB7_AI1_MemoryMSB             = 0x06,      /*!< */ 
        EIB7_AI1_MRS                   = 0x07,      /*!< */ 
        EIB7_AI1_TestCommand           = 0x08,      /*!< */ 
        EIB7_AI1_TestValue_word1       = 0x09,      /*!< */ 
        EIB7_AI1_TestValue_word2       = 0x0A,      /*!< */ 
        EIB7_AI1_TestValue_word3       = 0x0B,      /*!< */ 
        EIB7_AI1_Temperature1          = 0x0C,      /*!< */ 
        EIB7_AI1_Temperature2          = 0x0D,      /*!< */ 
        EIB7_AI1_AddSensor             = 0x0E,      /*!< */ 
//      EIB7_AI1_Stop                  = 0x0F,      /*!< */ 
    };

    /// <summary>
    /// This enum lists all EIB axis EnDat additional information 2 values.
    /// </summary>
    enum class EIB7_ADD_INFO_2 : UInt32
    {
        EIB7_AI2_NOP                   = 0x10,      /*!< */ 
        EIB7_AI2_Commutation           = 0x11,      /*!< */ 
        EIB7_AI2_Acceleration          = 0x12,      /*!< */ 
        EIB7_AI2_CommAndAccel          = 0x13,      /*!< */ 
        EIB7_AI2_LimitSignal           = 0x14,      /*!< */ 
        EIB7_AI2_LimitAndAccel         = 0x15,      /*!< */ 
        EIB7_AI2_AsyncPos_word1        = 0x16,      /*!< */ 
        EIB7_AI2_AsyncPos_word2        = 0x17,      /*!< */ 
        EIB7_AI2_AsyncPos_word3        = 0x18,      /*!< */ 
        EIB7_AI2_OPSErrorSources       = 0x19,      /*!< */ 
        EIB7_AI2_ReservedA             = 0x1A,      /*!< */
        EIB7_AI2_ReservedB             = 0x1B,      /*!< */
        EIB7_AI2_ReservedC             = 0x1C,      /*!< */
        EIB7_AI2_ReservedD             = 0x1D,      /*!< */
        EIB7_AI2_ReservedE             = 0x1E,      /*!< */
//      EIB7_AI2_Stop                  = 0x1F,      /*!< */ 
    };

    // EnDat additional information fifo list
    typedef array<EIB7_ADD_INFO_ALL>    AddInfoList; 

    ref class EIB_AuxAxis_Param;

    /// <summary>
    /// This class contains all EIB axis parameters.
    /// </summary>
    public ref class EIB_Axis_Param : public EIB_Axis_Base
    {
    private:
        UInt32                      fileVersion;        ///< parameter version for parameter file

        EIB_AuxAxis_Param ^         auxAxis;            ///< the corresponding auxiliary axis

        EIB7_INTERFACE_TYPE_GUI     interfaceTypeGui;   ///< EIB axis interface type for GUI input
        EIB7_BANDWIDTH              bandwidth;          ///< EIB axis encoder's bandwidth
        EIB7_COMPENSATION           compensation;       ///< EIB axis online compensation state
        EIB7_REFMARKS               refmarks;           ///< EIB axis reference mark type
        EIB7_ENCODER_TYPE           encoderType;        ///< EIB axis encoder type
        UInt32                      linecounts;         ///< EIB axis signal period
        UInt32                      increment;          ///< EIB axis base distance
        Double                      signalPeriod;       ///< EIB axis signal period
        bool                        incSignals;         ///< EIB axis EnDat with incremental signals
        UInt32                      enDatClock;         ///< EIB axis EnDat interface clock
        bool                        runtimeComp;        ///< EIB axis EnDat with runtime compensation
        EIB7_RECOVERY_TIME          recovery;           ///< EIB axis EnDat recovery time I state
        EIB7_ENDAT_CALC_TIME        calculation;        ///< EIB axis EnDat calculation time state
        EIB7_ADD_INFO_MODE          addInfoMode;        ///< EIB axis EnDat additional information mode
        EIB7_ADD_INFO_1             addInfo1;           ///< EIB axis EnDat additional information 1
        EIB7_ADD_INFO_2             addInfo2;           ///< EIB axis EnDat additional information 2
        EIB7_ADD_INFO_NUM           addInfoNum;         ///< EIB axis EnDat additional information number
        AddInfoList ^               addInfoFifo;        ///< EIB axis EnDat additional information fifo

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="auxAxis">The corresponding auxiliary axis.</param>
        EIB_Axis_Param(EIB_AuxAxis_Param ^ auxAxis)
        {
            fileVersion = 1;

            this->auxAxis = auxAxis;

            interfaceTypeGui    = EIB_DEFAULT_INTERFACE_TYPE_GUI;
            bandwidth           = EIB_DEFAULT_BANDWIDTH_1V;
            compensation        = EIB_DEFAULT_COMPENSATION;
            refmarks            = EIB_DEFAULT_REFMARKS;
            encoderType         = EIB_DEFAULT_ENCODER_TYPE;
            linecounts          = EIB_DEFAULT_LINECOUNTS;
            increment           = EIB_DEFAULT_INCREMENT;
            signalPeriod        = EIB_DEFAULT_SIGNAL_PERIOD;
            incSignals          = EIB_DEFAULT_INC_SIGNALS_21;
            enDatClock          = UInt32(EIB_DEFAULT_INTERFACE_CLOCK_21);
            runtimeComp         = EIB_DEFAULT_RUNTIME_COMP_21;
            recovery            = EIB_DEFAULT_RECOVERY_TIME;
            calculation         = EIB_DEFAULT_ENDAT_CALC_TIME_21;
            addInfoMode         = EIB_DEFAULT_ADD_INFO_MODE;
            addInfo1            = EIB_DEFAULT_ADD_INFO_1;
            addInfo2            = EIB_DEFAULT_ADD_INFO_2;
            addInfoNum          = EIB_DEFAULT_ADD_INFO_NUM;

            addInfoFifo = gcnew AddInfoList(EIB_MAX_ADD_INFO_NUM);
            for (UInt32 i = 0; i < EIB_MAX_ADD_INFO_NUM; i ++)
            {
                addInfoFifo[i] = EIB_DEFAULT_ADD_INFO_FIFO;
            }
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Axis_Param ^ source)
        {
            interfaceTypeGui = source->interfaceTypeGui;
            bandwidth        = source->bandwidth;
            compensation     = source->compensation;
            refmarks         = source->refmarks;
            encoderType      = source->encoderType;
            linecounts       = source->linecounts;
            increment        = source->increment;
            signalPeriod     = source->signalPeriod;
            incSignals       = source->incSignals;
            enDatClock       = source->enDatClock;
            runtimeComp      = source->runtimeComp;
            recovery         = source->recovery;
            calculation      = source->calculation;
            addInfoMode      = source->addInfoMode;
            addInfo1         = source->addInfo1;
            addInfo2         = source->addInfo2;
            addInfoNum       = source->addInfoNum;
            for (UInt32 i = 0; i < EIB_MAX_ADD_INFO_NUM; i ++)
            {
                addInfoFifo[i] = source->addInfoFifo[i];
            }
            EIB_Axis_Base::Copy(source);
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_Axis_Param ^ source)
        {
            if (interfaceTypeGui != source->interfaceTypeGui) return false;
            if (bandwidth        != source->bandwidth)        return false;
            if (compensation     != source->compensation)     return false;
            if (refmarks         != source->refmarks)         return false;
            if (encoderType      != source->encoderType)      return false;
            if (linecounts       != source->linecounts)       return false;
            if (increment        != source->increment)        return false;
            if (signalPeriod     != source->signalPeriod)     return false;
            if (incSignals       != source->incSignals)       return false;
            if (enDatClock       != source->enDatClock)       return false;
            if (runtimeComp      != source->runtimeComp)      return false;
            if (recovery         != source->recovery)         return false;
            if (calculation      != source->calculation)      return false;
            if (addInfoMode      != source->addInfoMode)      return false;
            if (addInfo1         != source->addInfo1)         return false;
            if (addInfo2         != source->addInfo2)         return false;
            if (addInfoNum       != source->addInfoNum)       return false;
            for (UInt32 i = 0; i < EIB_MAX_ADD_INFO_NUM; i ++)
            {
                if (addInfoFifo[i] != source->addInfoFifo[i]) return false;
            }
            return EIB_Axis_Base::IsEqual(source);
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
		/// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(EIB_ParamFile ^ paramFile) new
        {
            // read / write parameter version
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            interfaceTypeGui = (EIB7_INTERFACE_TYPE_GUI)paramFile->ReadWriteUInt32(UInt32(interfaceTypeGui));
            bandwidth        = (EIB7_BANDWIDTH)         paramFile->ReadWriteUInt32(UInt32(bandwidth));
            compensation     = (EIB7_COMPENSATION)      paramFile->ReadWriteUInt32(UInt32(compensation));
            refmarks         = (EIB7_REFMARKS)          paramFile->ReadWriteUInt32(UInt32(refmarks));
            encoderType      = (EIB7_ENCODER_TYPE)      paramFile->ReadWriteUInt32(UInt32(encoderType));
            paramFile->ReadWriteUInt32(linecounts);
            paramFile->ReadWriteUInt32(increment);
            paramFile->ReadWriteDouble(signalPeriod);
            paramFile->ReadWriteBool(incSignals);
            paramFile->ReadWriteUInt32(enDatClock);
            paramFile->ReadWriteBool(runtimeComp);
            recovery    = (EIB7_RECOVERY_TIME)  paramFile->ReadWriteUInt32(UInt32(recovery));
            calculation = (EIB7_ENDAT_CALC_TIME)paramFile->ReadWriteUInt32(UInt32(calculation));
            addInfoMode = (EIB7_ADD_INFO_MODE)  paramFile->ReadWriteUInt32(UInt32(addInfoMode));
            addInfo1    = (EIB7_ADD_INFO_1)     paramFile->ReadWriteUInt32(UInt32(addInfo1));
            addInfo2    = (EIB7_ADD_INFO_2)     paramFile->ReadWriteUInt32(UInt32(addInfo2));
            addInfoNum  = (EIB7_ADD_INFO_NUM)   paramFile->ReadWriteUInt32(UInt32(addInfoNum));
            for (UInt32 i = 0; i < EIB_MAX_ADD_INFO_NUM; i ++)
            {
                addInfoFifo[i] = (EIB7_ADD_INFO_ALL)paramFile->ReadWriteUInt32(UInt32(addInfoFifo[i]));
            }
            EIB_Axis_Base::ReadWriteFile(paramFile);
        }

        /// <summary>
        /// Gets the EIB axis enable state.
        /// </summary>
        property bool IsEnabled
        {
            bool get()
            {
                if (interfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Disabled)
                {
                    return false;
                }
                return true;
            }
        }

        /// <summary>
        /// Gets / sets the EIB corresponding auxiliary axis parameters.
        /// </summary>
        property EIB_AuxAxis_Param ^ AuxAxis
        {
            EIB_AuxAxis_Param ^ get() { return auxAxis; }
            void set(EIB_AuxAxis_Param ^ value) { auxAxis = value; }
        }

        /// <summary>
        /// Gets the EIB axis interface type for EIB control.
        /// </summary>
        property UInt32 InterfaceTypeEib
        {
            UInt32 get()
            {
                UInt32 interfaceTypeEib;
                // translate from GUI input parameter to EIB control parameter
                switch (interfaceTypeGui)
                {
                default: throw gcnew Exception(L"Internal error 2"); break;
                case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Disabled:
                    interfaceTypeEib = UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_Disabled);
                    break;
                case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental:
                    interfaceTypeEib = UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_Incremental);
                    break;
                case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u:
                    interfaceTypeEib = UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_Incremental_11u);
                    break;
                case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat21:
                    interfaceTypeEib = UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDat21);
                    break;
                case EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22:
                    interfaceTypeEib = UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDat22);
                    break;
                }
                // check for EnDat 2.1 with incremental signals
                if (   (interfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat21)
                    && (incSignals == true))
                {
                    interfaceTypeEib = UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDat01);
                }
                // check for EnDat 2.2 with runtime compensation
                if (   (interfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22)
                    && (runtimeComp == true))
                {
                    interfaceTypeEib |= UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDatDelayMeasurement);
                }
                return interfaceTypeEib;
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis interface type for GUI input.
        /// </summary>
        property EIB7_INTERFACE_TYPE_GUI InterfaceTypeGui
        {
            EIB7_INTERFACE_TYPE_GUI get() { return interfaceTypeGui; }
            void set(EIB7_INTERFACE_TYPE_GUI value) { interfaceTypeGui = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis encoder's bandwidth.
        /// </summary>
        property EIB7_BANDWIDTH Bandwidth
        {
            EIB7_BANDWIDTH get() { return bandwidth; }
            void set(EIB7_BANDWIDTH value) { bandwidth = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis online compensation state.
        /// </summary>
        property EIB7_COMPENSATION Compensation
        {
            EIB7_COMPENSATION get() { return compensation; }
            void set(EIB7_COMPENSATION value) { compensation = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis reference mark type.
        /// </summary>
        property EIB7_REFMARKS Refmarks
        {
            EIB7_REFMARKS get() { return refmarks; }
            void set(EIB7_REFMARKS value) { refmarks = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis encoder type.
        /// </summary>
        property EIB7_ENCODER_TYPE EncoderType
        {
            EIB7_ENCODER_TYPE get() { return encoderType; }
            void set(EIB7_ENCODER_TYPE value) { encoderType = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis signal period.
        /// </summary>
        property UInt32 Linecounts
        {
            UInt32 get() { return linecounts; }
            void set(UInt32 value) { linecounts = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis base distance.
        /// </summary>
        property UInt32 Increment
        {
            UInt32 get() { return increment; }
            void set(UInt32 value) { increment = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis signal period.
        /// </summary>
        property Double SignalPeriod
        {
            Double get() { return signalPeriod; }
            void set(Double value) { signalPeriod = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat with incremental signals.
        /// </summary>
        property bool IncSignals
        {
            bool get() { return incSignals; }
            void set(bool value) { incSignals = value; }
        }

        /// <summary>
        /// Gets the EIB axis EnDat interface clock.
        /// </summary>
        property UInt32 EnDatClock
        {
            UInt32 get() { return enDatClock; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat 2.1 interface clock.
        /// </summary>
        property EIB7_INTERFACE_CLOCK_21 EnDatClock21
        {
            EIB7_INTERFACE_CLOCK_21 get() { return (EIB7_INTERFACE_CLOCK_21)enDatClock; }
            void set(EIB7_INTERFACE_CLOCK_21 value) { enDatClock = UInt32(value); }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat 2.2 interface clock.
        /// </summary>
        property EIB7_INTERFACE_CLOCK_22 EnDatClock22
        {
            EIB7_INTERFACE_CLOCK_22 get() { return (EIB7_INTERFACE_CLOCK_22)enDatClock; }
            void set(EIB7_INTERFACE_CLOCK_22 value) { enDatClock = UInt32(value); }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat with runtime compensation.
        /// </summary>
        property bool RuntimeComp
        {
            bool get() { return runtimeComp; }
            void set(bool value) { runtimeComp = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat recovery time I state.
        /// </summary>
        property EIB7_RECOVERY_TIME Recovery
        {
            EIB7_RECOVERY_TIME get() { return recovery; }
            void set(EIB7_RECOVERY_TIME value) { recovery = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat calculation time state.
        /// </summary>
        property EIB7_ENDAT_CALC_TIME Calculation
        {
            EIB7_ENDAT_CALC_TIME get() { return calculation; }
            void set(EIB7_ENDAT_CALC_TIME value) { calculation = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat additional information mode.
        /// </summary>
        property EIB7_ADD_INFO_MODE AddInfoMode
        {
            EIB7_ADD_INFO_MODE get() { return addInfoMode; }
            void set(EIB7_ADD_INFO_MODE value) { addInfoMode = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat additional information 1.
        /// </summary>
        property EIB7_ADD_INFO_1 AddInfo1
        {
            EIB7_ADD_INFO_1 get() { return addInfo1; }
            void set(EIB7_ADD_INFO_1 value) { addInfo1 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat additional information 1.
        /// </summary>
        property EIB7_ADD_INFO_2 AddInfo2
        {
            EIB7_ADD_INFO_2 get() { return addInfo2; }
            void set(EIB7_ADD_INFO_2 value) { addInfo2 = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat additional information number.
        /// </summary>
        property EIB7_ADD_INFO_NUM AddInfoNum
        {
            EIB7_ADD_INFO_NUM get() { return addInfoNum; }
            void set(EIB7_ADD_INFO_NUM value) { addInfoNum = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis EnDat additional information fifo value.
        /// </summary>
        property EIB7_ADD_INFO_ALL AddInfoFifo[UInt32]
        {
            EIB7_ADD_INFO_ALL get(UInt32 index) { return addInfoFifo[index]; }
            void set(UInt32 index, EIB7_ADD_INFO_ALL value) { addInfoFifo[index] = value; }
        }
    };
	#pragma endregion

 	#pragma region "EIB_AuxAxis_Param class definition"
    // default parameters
    #define EIB_DEFAULT_INTERVAL_COUNTER_IPF    EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_1x
    #define EIB_DEFAULT_INTERVAL_COUNTER_EDGE   EIB7_INTERVAL_COUNTER_EDGE::EIB7_ICE_4x

    /// <summary>
    /// This enum lists all EIB auxilary axis interval counter ipf values.
    /// </summary>
    enum class EIB7_INTERVAL_COUNTER_IPF : UInt32
    {
        EIB7_ICF_1x       = 0,/*!< */
        EIB7_ICF_2x       = 1,/*!< */
        EIB7_ICF_4x       = 2,/*!< */
        EIB7_ICF_5x       = 3,/*!< */
        EIB7_ICF_10x      = 4,/*!< */
        EIB7_ICF_20x      = 5,/*!< */
        EIB7_ICF_25x      = 6,/*!< */
        EIB7_ICF_50x      = 7,/*!< */
        EIB7_ICF_100x     = 8 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB auxilary axis interval counter edge values.
    /// </summary>
    enum class EIB7_INTERVAL_COUNTER_EDGE : UInt32
    {
        EIB7_ICE_1x       = 0,/*!< */
        EIB7_ICE_2x       = 1,/*!< */
        EIB7_ICE_4x       = 2 /*!< */
    };

    /// <summary>
    /// This class contains all EIB auxiliary axis parameters.
    /// </summary>
    public ref class EIB_AuxAxis_Param : public EIB_Axis_Base
    {
    private:
        UInt32                      fileVersion;            ///< parameter version for parameter file

        EIB_Axis_Param ^            axis;                   ///< the corresponding axis

        EIB7_INTERVAL_COUNTER_IPF   intervalCounterIpf;     ///< EIB auxilary axis interval counter ipf
        EIB7_INTERVAL_COUNTER_EDGE  intervalCounterEdge;    ///< EIB auxilary axis interval counter edge

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="axis">The corresponding axis.</param>
        EIB_AuxAxis_Param(EIB_Axis_Param ^ axis)
        {
            fileVersion = 1;

            this->axis = axis;

            intervalCounterIpf  = EIB_DEFAULT_INTERVAL_COUNTER_IPF;
            intervalCounterEdge = EIB_DEFAULT_INTERVAL_COUNTER_EDGE;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_AuxAxis_Param ^ source)
        {
            intervalCounterIpf  = source->intervalCounterIpf;
            intervalCounterEdge = source->intervalCounterEdge;
            EIB_Axis_Base::Copy(source);
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_AuxAxis_Param ^ source)
        {
            if (intervalCounterIpf  != source->intervalCounterIpf)  return false;
            if (intervalCounterEdge != source->intervalCounterEdge) return false;
            return EIB_Axis_Base::IsEqual(source);
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
        /// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(EIB_ParamFile ^ paramFile) new
        {
            // read / write parameter version
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            intervalCounterIpf  = (EIB7_INTERVAL_COUNTER_IPF)0;
                                            paramFile->ReadWriteUInt32(UInt32(intervalCounterIpf));
            intervalCounterEdge = (EIB7_INTERVAL_COUNTER_EDGE)
                                            paramFile->ReadWriteUInt32(UInt32(intervalCounterEdge));
            EIB_Axis_Base::ReadWriteFile(paramFile);
        }

        /// <summary>
        /// Gets the EIB axis enable state.
        /// </summary>
        property bool IsEnabled
        {
            bool get()
            {
                if (   (axis->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                    || (axis->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u))
                {
                    return true;
                }
                return false;
            }
        }

        /// <summary>
        /// Gets the EIB corresponding axis parameters.
        /// </summary>
        property EIB_Axis_Param ^ Axis
        {
            EIB_Axis_Param ^ get() { return axis; }
        }

        /// <summary>
        /// Gets / sets the EIB axis interval counter ipf.
        /// </summary>
        property EIB7_INTERVAL_COUNTER_IPF IntervalCounterIpf
        {
            EIB7_INTERVAL_COUNTER_IPF get() { return intervalCounterIpf; }
            void set(EIB7_INTERVAL_COUNTER_IPF value) { intervalCounterIpf = value; }
        }

        /// <summary>
        /// Gets the EIB axis interval counter ipf value.
        /// </summary>
        property UInt32 IntervalCounterIpfValue
        {
            UInt32 get()
            {
                switch (intervalCounterIpf)
                {
                default: throw gcnew Exception(L"Internal error 20"); break;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_1x:
                return 1;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_2x:
                return 2;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_4x:
                return 4;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_5x:
                return 5;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_10x:
                return 10;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_20x:
                return 20;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_25x:
                return 25;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_50x:
                return 50;
                case EIB7_INTERVAL_COUNTER_IPF::EIB7_ICF_100x:
                return 100;
                }
            }
        }

        /// <summary>
        /// Gets / sets the EIB axis interval counter edge.
        /// </summary>
        property EIB7_INTERVAL_COUNTER_EDGE IntervalCounterEdge
        {
            EIB7_INTERVAL_COUNTER_EDGE get() { return intervalCounterEdge; }
            void set(EIB7_INTERVAL_COUNTER_EDGE value) { intervalCounterEdge = value; }
        }

        /// <summary>
        /// Gets the EIB axis interval counter edge value.
        /// </summary>
        property UInt32 IntervalCounterEdgeValue
        {
            UInt32 get()
            {
                switch (intervalCounterEdge)
                {
                default: throw gcnew Exception(L"Internal error 21"); break;
                case EIB7_INTERVAL_COUNTER_EDGE::EIB7_ICE_1x:
                return 1;
                case EIB7_INTERVAL_COUNTER_EDGE::EIB7_ICE_2x:
                return 2;
                case EIB7_INTERVAL_COUNTER_EDGE::EIB7_ICE_4x:
                return 4;
                }
            }
        }
    };
	#pragma endregion

 	#pragma region "EIB_Trigger_Param class definition"
    // default parameters
    #define EIB_DEFAULT_TRIGGER_INP_MODE            EIB7_IO_MODE::EIB7_IOM_Trigger
    #define EIB_DEFAULT_TRIGGER_INP_TERM            EIB7_ENABLE_MODE::EIB7_MD_Enable
    #define EIB_DEFAULT_TRIGGER_INP_DELAY           UInt32(         0)
    #define EIB_MIN_TRIGGER_INP_DELAY               UInt32(         0)
    #define EIB_MAX_TRIGGER_INP_DELAY               UInt32(       255)
    #define EIB_STEP_TRIGGER_INP_DELAY              UInt32(         1)
    #define EIB_DEFAULT_TRIGGER_OUT_ENABLE          EIB7_ENABLE_MODE::EIB7_MD_Disable
    #define EIB_DEFAULT_TRIGGER_OUT_MODE            EIB7_IO_MODE::EIB7_IOM_Trigger
    #define EIB_DEFAULT_TRIGGER_OUT_SOURCE          EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgInSync
    #define EIB_DEFAULT_MASTER_TRIGGER_SOURCE       EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgTimer
    #define EIB_DEFAULT_AXIS_TRIGGER_SOURCE         EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgTimer
    #define EIB_DEFAULT_AUX_AXIS_TRIGGER_SOURCE     EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgTimer
    #define EIB_DEFAULT_AXIS_TRIGGER_EDGE           EIB7_AXIS_TRIGGER_EDGE::EIB7_RI_Rising
    #define EIB_DEFAULT_AUX_AXIS_TRIGGER_EDGE       EIB7_AXIS_TRIGGER_EDGE::EIB7_RI_Rising
    #define EIB_DEFAULT_TIMER_TRIGGER_PERIOD        UInt32(      1000)
    #define EIB_MIN_TIMER_TRIGGER_PERIOD_OPERATE    UInt32(       100)
    #define EIB_MIN_TIMER_TRIGGER_PERIOD_ADMIN      UInt32(        20)
    #define EIB_MAX_TIMER_TRIGGER_PERIOD            UInt32(0xFFFFFFFF)
    #define EIB_STEP_TIMER_TRIGGER_PERIOD           UInt32(         1)
    #define EIB_DEFAULT_INTERVAL_COUNTER_MODE       EIB7_INTERVAL_COUNTER_MODE::EIB7_ICM_Disable
    #define EIB_DEFAULT_INTERVAL_COUNTER_START      EIB7_INTERVAL_COUNTER_START::EIB7_ICS_Current
    #define EIB_DEFAULT_INTERVAL_COUNTER_START_POS  UInt32(         0)
    #define EIB_MIN_INTERVAL_COUNTER_START_POS      UInt32(         0)
    #define EIB_MAX_INTERVAL_COUNTER_START_POS      UInt32(0xFFFFFFFF)
    #define EIB_STEP_INTERVAL_COUNTER_START_POS     UInt32(         1)
    #define EIB_DEFAULT_INTERVAL_COUNTER_PERIOD     UInt32(      1000)
    #define EIB_MIN_INTERVAL_COUNTER_PERIOD         UInt32(         6)
    #define EIB_MAX_INTERVAL_COUNTER_PERIOD         UInt32(0xFFFFFFFF)
    #define EIB_STEP_INTERVAL_COUNTER_PERIOD        UInt32(         1)
    #define EIB_DEFAULT_PULSE_COUNTER_COUNT         UInt32(         1)
    #define EIB_MIN_PULSE_COUNTER_COUNT             UInt32(         1)
    #define EIB_MAX_PULSE_COUNTER_COUNT             UInt32(   1048576)
    #define EIB_STEP_PULSE_COUNTER_COUNT            UInt32(         1)
    #define EIB_DEFAULT_PULSE_COUNTER_START         EIB7_PULSE_COUNTER_START::EIB7_PS_TrgInput1
    #define EIB_DEFAULT_PULSE_COUNTER_TRIGGER       EIB7_PULSE_COUNTER_TRIGGER::EIB7_PT_TrgTimer

    /// <summary>
    /// This enum lists all EIB trigger modes.
    /// </summary>
    enum class EIB7_IO_MODE : UInt32
    {
        EIB7_IOM_Trigger = 0, /*!< */
        EIB7_IOM_Logical = 1  /*!< */
    };

    /// <summary>
    /// This enum lists all EIB trigger sources for the trigger output.
    /// </summary>
    enum class EIB7_TRIGGER_OUT_SOURCE : UInt32
    {
        EIB7_OT_TrgInSync       = 0,/*!< */
        EIB7_OT_TrgInAsync      = 1,/*!< */
        EIB7_OT_TrgSW1          = 2,/*!< */
        EIB7_OT_TrgSW2          = 3,/*!< */
        EIB7_OT_TrgSW3          = 4,/*!< */
        EIB7_OT_TrgSW4          = 5,/*!< */
        EIB7_OT_TrgRImaskedCH1  = 6,/*!< */
        EIB7_OT_TrgIC           = 7,/*!< */
        EIB7_OT_TrgPuls         = 8,/*!< */
        EIB7_OT_TrgTimer        = 9 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB master trigger sources.
    /// </summary>
    enum class EIB7_MASTER_TRIGGER_SOURCE : UInt32
    {
        EIB7_AT_TrgInput1       = 0,/*!< */
        EIB7_AT_TrgInput2       = 1,/*!< */
        EIB7_AT_TrgInput3       = 2,/*!< */
        EIB7_AT_TrgInput4       = 3,/*!< */
        EIB7_AT_TrgSW1          = 4,/*!< */
        EIB7_AT_TrgSW2          = 5,/*!< */
        EIB7_AT_TrgSW3          = 6,/*!< */
        EIB7_AT_TrgSW4          = 7,/*!< */
        EIB7_AT_TrgRI           = 8,/*!< */
        EIB7_AT_TrgRImaskedCH1  = 9,/*!< */
        EIB7_AT_TrgIC           = 10,/*!< */
        EIB7_AT_TrgPuls         = 11,/*!< */
        EIB7_AT_TrgTimer        = 12 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB axis trigger sources.
    /// </summary>
    enum class EIB7_AXIS_TRIGGER_SOURCE : UInt32
    {
        EIB7_AT_TrgInput1       = 0,/*!< */
        EIB7_AT_TrgInput2       = 1,/*!< */
        EIB7_AT_TrgInput3       = 2,/*!< */
        EIB7_AT_TrgInput4       = 3,/*!< */
        EIB7_AT_TrgSW1          = 4,/*!< */
        EIB7_AT_TrgSW2          = 5,/*!< */
        EIB7_AT_TrgSW3          = 6,/*!< */
        EIB7_AT_TrgSW4          = 7,/*!< */
        EIB7_AT_TrgRI           = 8,/*!< */
        EIB7_AT_TrgRImaskedCH1  = 9,/*!< */
        EIB7_AT_TrgIC           = 10,/*!< */
        EIB7_AT_TrgPuls         = 11,/*!< */
        EIB7_AT_TrgTimer        = 12 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB trigger sources.
    /// </summary>
    enum class EIB7_TRIGGER_SOURCE : UInt32
    {
        EIB7_TS_TrgInput1       = 0x00000001,/*!< */
        EIB7_TS_TrgInput2       = 0x00000002,/*!< */
        EIB7_TS_TrgInput3       = 0x00000004,/*!< */
        EIB7_TS_TrgInput4       = 0x00000008,/*!< */
        EIB7_TS_TrgRI1          = 0x00000010,/*!< */
        EIB7_TS_TrgRI2          = 0x00000020,/*!< */
        EIB7_TS_TrgRI3          = 0x00000040,/*!< */
        EIB7_TS_TrgRI4          = 0x00000080,/*!< */
        EIB7_TS_TrgRImaskedCH1  = 0x00000100,/*!< */
        EIB7_TS_TrgIC           = 0x00000200,/*!< */
        EIB7_TS_TrgPuls         = 0x00000400,/*!< */
        EIB7_TS_TrgTimer        = 0x00000800,/*!< */
        EIB7_TS_All             = 0xFFFFFFFF
    };

    /// <summary>
    /// This enum lists all EIB axis trigger edges.
    /// </summary>
    enum class EIB7_AXIS_TRIGGER_EDGE : UInt32
    {
        EIB7_RI_Rising        = 0,/*!< */
        EIB7_RI_Falling       = 1,/*!< */
        EIB7_RI_Both          = 2 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB interval counter trigger modes.
    /// </summary>
    enum class EIB7_INTERVAL_COUNTER_MODE : UInt32
    {
        EIB7_ICM_Disable       = 0,/*!< */
        EIB7_ICM_Single        = 1,/*!< */
        EIB7_ICM_Periodic      = 2 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB interval counter trigger start positions.
    /// </summary>
    enum class EIB7_INTERVAL_COUNTER_START : UInt32
    {
        EIB7_ICS_Current       = 0,/*!< */
        EIB7_ICS_StartPos      = 1 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB pulse counter trigger start signals.
    /// </summary>
    enum class EIB7_PULSE_COUNTER_START : UInt32
    {
        EIB7_PS_TrgInput1       = 0,/*!< */
        EIB7_PS_TrgInput2       = 1,/*!< */
        EIB7_PS_TrgInput3       = 2,/*!< */
        EIB7_PS_TrgInput4       = 3,/*!< */
        EIB7_PS_TrgRI1          = 4,/*!< */
        EIB7_PS_TrgRI2          = 5,/*!< */
        EIB7_PS_TrgRI3          = 6,/*!< */
        EIB7_PS_TrgRI4          = 7,/*!< */
        EIB7_PS_TrgSW1          = 8,/*!< */
        EIB7_PS_TrgSW2          = 9,/*!< */
        EIB7_PS_TrgSW3          = 10,/*!< */
        EIB7_PS_TrgSW4          = 11,/*!< */
        EIB7_PS_TrgRImaskedCH1  = 12, /*!< */
        EIB7_PS_TrgIC           = 13 /*!< */
    };

    /// <summary>
    /// This enum lists all EIB pulse counter trigger trigger signals.
    /// </summary>
    enum class EIB7_PULSE_COUNTER_TRIGGER : UInt32
    {
        EIB7_PT_TrgInput1       = 0,/*!< */
        EIB7_PT_TrgInput2       = 1,/*!< */
        EIB7_PT_TrgInput3       = 2,/*!< */
        EIB7_PT_TrgInput4       = 3,/*!< */
        EIB7_PT_TrgRI1          = 4,/*!< */
        EIB7_PT_TrgRI2          = 5,/*!< */
        EIB7_PT_TrgRI3          = 6,/*!< */
        EIB7_PT_TrgRI4          = 7,/*!< */
        EIB7_PT_TrgRImaskedCH1  = 8, /*!< */
        EIB7_PT_TrgIC           = 9, /*!< */
        EIB7_PT_TrgTimer        = 10, /*!< */
    };

    // EIB trigger mode list
    typedef array<EIB7_IO_MODE>                 TriggerModeList;
    // EIB trigger enable list
    typedef array<EIB7_ENABLE_MODE>             TriggerEnableList;
    // EIB trigger input delay list
    typedef array<UInt32>                       TriggerInpDelayList;
    // EIB trigger output source list
    typedef array<EIB7_TRIGGER_OUT_SOURCE>      TriggerOutSrcList;
    // EIB axis trigger source list
    typedef array<EIB7_AXIS_TRIGGER_SOURCE>     AxisTriggerSrcList;
    // EIB axis trigger edge list
    typedef array<EIB7_AXIS_TRIGGER_EDGE>       AxisTriggerEdgeList;

    /// <summary>
    /// This class contains all EIB trigger parameters.
    /// </summary>
    public ref class EIB_Trigger_Param
    {
    private:
        UInt32                      fileVersion;            ///< parameter version for parameter file

        UInt32                      triggerInpNumber;       ///< EIB number of extern trigger inputs
        UInt32                      triggerOutNumber;       ///< EIB number of extern trigger outputs

        TriggerModeList ^           triggerInpMode;         ///< extern trigger input mode
        TriggerEnableList ^         triggerInpTerm;         ///< extern trigger input termination
        TriggerInpDelayList ^       triggerInpDelay;        ///< extern trigger input delay

        TriggerEnableList ^         triggerOutEnable;       ///< extern trigger output enable
        TriggerModeList ^           triggerOutMode;         ///< extern trigger output mode
        TriggerOutSrcList ^         triggerOutSource;       ///< extern trigger output source

        EIB7_MASTER_TRIGGER_SOURCE  masterTriggerSource;    ///< master trigger source

        AxisTriggerSrcList ^        axisTriggerSource;      ///< axis trigger source
        AxisTriggerEdgeList ^       axisTriggerEdge;        ///< axis trigger edge

        AxisTriggerSrcList ^        auxAxisTriggerSource;   ///< auxiliary axis trigger source
        AxisTriggerEdgeList ^       auxAxisTriggerEdge;     ///< auxiliary axis trigger edge

        UInt32                      timerTriggerPeriod;     ///< timer trigger period in micro seconds

        EIB7_INTERVAL_COUNTER_MODE  intervalCounterMode;    ///< interval counter mode
        EIB7_INTERVAL_COUNTER_START intervalCounterStart;   ///< interval counter start
        UInt32                      intervalCounterStartPos;///< interval counter start position
        UInt32                      intervalCounterPeriod;  ///< interval counter period

        UInt32                      pulseCounterCount;      ///< pulse counter count
        EIB7_PULSE_COUNTER_START    pulseCounterStart;      ///< pulse counter start
        EIB7_PULSE_COUNTER_TRIGGER  pulseCounterTrigger;    ///< pulse counter trigger

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_Trigger_Param()
        {
            fileVersion = 1;

            triggerInpNumber = EIB_MAX_TRIGGER_INP_NUMBER;
            triggerOutNumber = EIB_MAX_TRIGGER_OUT_NUMBER;

            triggerInpMode  = gcnew TriggerModeList(EIB_MAX_TRIGGER_INP_NUMBER);
            triggerInpTerm  = gcnew TriggerEnableList(EIB_MAX_TRIGGER_INP_NUMBER);
            triggerInpDelay = gcnew TriggerInpDelayList(EIB_MAX_TRIGGER_INP_NUMBER);

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_INP_NUMBER; i ++)
            {
                triggerInpMode[i]   = EIB_DEFAULT_TRIGGER_INP_MODE;
                triggerInpTerm[i]   = EIB_DEFAULT_TRIGGER_INP_TERM;
                triggerInpDelay[i]  = EIB_DEFAULT_TRIGGER_INP_DELAY;
            }

            triggerOutEnable = gcnew TriggerEnableList(EIB_MAX_TRIGGER_OUT_NUMBER);
            triggerOutMode   = gcnew TriggerModeList(EIB_MAX_TRIGGER_OUT_NUMBER);
            triggerOutSource = gcnew TriggerOutSrcList(EIB_MAX_TRIGGER_OUT_NUMBER);

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_OUT_NUMBER; i ++)
            {
                triggerOutEnable[i] = EIB_DEFAULT_TRIGGER_OUT_ENABLE;
                triggerOutMode[i]   = EIB_DEFAULT_TRIGGER_OUT_MODE;
                triggerOutSource[i] = EIB_DEFAULT_TRIGGER_OUT_SOURCE;
            }

            masterTriggerSource = EIB_DEFAULT_MASTER_TRIGGER_SOURCE;

            axisTriggerSource = gcnew AxisTriggerSrcList(EIB_MAX_AXIS_NUMBER);
            axisTriggerEdge   = gcnew AxisTriggerEdgeList(EIB_MAX_AXIS_NUMBER);

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axisTriggerSource[i] = EIB_DEFAULT_AXIS_TRIGGER_SOURCE;
                axisTriggerEdge[i]   = EIB_DEFAULT_AXIS_TRIGGER_EDGE;
            }

            auxAxisTriggerSource = gcnew AxisTriggerSrcList(EIB_MAX_AUX_AXIS_NUMBER);
            auxAxisTriggerEdge   = gcnew AxisTriggerEdgeList(EIB_MAX_AUX_AXIS_NUMBER);

            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxisTriggerSource[i] = EIB_DEFAULT_AUX_AXIS_TRIGGER_SOURCE;
                auxAxisTriggerEdge[i]   = EIB_DEFAULT_AUX_AXIS_TRIGGER_EDGE;
            }

            timerTriggerPeriod      = EIB_DEFAULT_TIMER_TRIGGER_PERIOD;

            intervalCounterMode     = EIB_DEFAULT_INTERVAL_COUNTER_MODE;
            intervalCounterStart    = EIB_DEFAULT_INTERVAL_COUNTER_START;
            intervalCounterStartPos = EIB_DEFAULT_INTERVAL_COUNTER_START_POS;
            intervalCounterPeriod   = EIB_DEFAULT_INTERVAL_COUNTER_PERIOD;

            pulseCounterCount       = EIB_DEFAULT_PULSE_COUNTER_COUNT;
            pulseCounterStart       = EIB_DEFAULT_PULSE_COUNTER_START;
            pulseCounterTrigger     = EIB_DEFAULT_PULSE_COUNTER_TRIGGER;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Trigger_Param ^ source)
        {
            triggerInpNumber = source->triggerInpNumber;
            triggerOutNumber = source->triggerOutNumber;

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_INP_NUMBER; i ++)
            {
                triggerInpMode[i]   = source->triggerInpMode[i];
                triggerInpTerm[i]   = source->triggerInpTerm[i];
                triggerInpDelay[i]  = source->triggerInpDelay[i];
            }

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_OUT_NUMBER; i ++)
            {
                triggerOutEnable[i] = source->triggerOutEnable[i];
                triggerOutMode[i]   = source->triggerOutMode[i];
                triggerOutSource[i] = source->triggerOutSource[i];
            }

            masterTriggerSource = source->masterTriggerSource;

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axisTriggerSource[i] = source->axisTriggerSource[i];
                axisTriggerEdge[i]   = source->axisTriggerEdge[i];
            }

            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxisTriggerSource[i] = source->auxAxisTriggerSource[i];
                auxAxisTriggerEdge[i]   = source->auxAxisTriggerEdge[i];
            }

            timerTriggerPeriod      = source->timerTriggerPeriod;

            intervalCounterMode     = source->intervalCounterMode;
            intervalCounterStart    = source->intervalCounterStart;
            intervalCounterStartPos = source->intervalCounterStartPos;
            intervalCounterPeriod   = source->intervalCounterPeriod;

            pulseCounterCount       = source->pulseCounterCount;
            pulseCounterStart       = source->pulseCounterStart;
            pulseCounterTrigger     = source->pulseCounterTrigger;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_Trigger_Param ^ source)
        {
            if (triggerInpNumber != source->triggerInpNumber) return false;
            if (triggerOutNumber != source->triggerOutNumber) return false;

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_INP_NUMBER; i ++)
            {
                if (triggerInpMode[i]   != source->triggerInpMode[i])   return false;
                if (triggerInpTerm[i]   != source->triggerInpTerm[i])   return false;
                if (triggerInpDelay[i]  != source->triggerInpDelay[i])  return false;
            }

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_OUT_NUMBER; i ++)
            {
                if (triggerOutEnable[i] != source->triggerOutEnable[i]) return false;
                if (triggerOutMode[i]   != source->triggerOutMode[i])   return false;
                if (triggerOutSource[i] != source->triggerOutSource[i]) return false;
            }

            if (masterTriggerSource != source->masterTriggerSource) return false;

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                if (axisTriggerSource[i] != source->axisTriggerSource[i]) return false;
                if (axisTriggerEdge[i]   != source->axisTriggerEdge[i])   return false;
            }

            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                if (auxAxisTriggerSource[i] != source->auxAxisTriggerSource[i]) return false;
                if (auxAxisTriggerEdge[i]   != source->auxAxisTriggerEdge[i])   return false;
            }

            if (timerTriggerPeriod      != source->timerTriggerPeriod)      return false;

            if (intervalCounterMode     != source->intervalCounterMode)     return false;
            if (intervalCounterStart    != source->intervalCounterStart)    return false;
            if (intervalCounterStartPos != source->intervalCounterStartPos) return false;
            if (intervalCounterPeriod   != source->intervalCounterPeriod)   return false;

            if (pulseCounterCount       != source->pulseCounterCount)       return false;
            if (pulseCounterStart       != source->pulseCounterStart)       return false;
            if (pulseCounterTrigger     != source->pulseCounterTrigger)     return false;
            return true;
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
		/// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(EIB_ParamFile ^ paramFile)
        {
            // read / write parameter version
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            paramFile->ReadWriteUInt32(triggerInpNumber);
            paramFile->ReadWriteUInt32(triggerOutNumber);

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_INP_NUMBER; i ++)
            {
                triggerInpMode[i]  = (EIB7_IO_MODE)    paramFile->ReadWriteUInt32(UInt32(triggerInpMode[i]));
                triggerInpTerm[i]  = (EIB7_ENABLE_MODE)paramFile->ReadWriteUInt32(UInt32(triggerInpTerm[i]));
                paramFile->ReadWriteUInt32(triggerInpDelay[i]);
            }

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_OUT_NUMBER; i ++)
            {
                triggerOutEnable[i] = (EIB7_ENABLE_MODE)       paramFile->ReadWriteUInt32(UInt32(triggerOutEnable[i]));
                triggerOutMode[i]   = (EIB7_IO_MODE)           paramFile->ReadWriteUInt32(UInt32(triggerOutMode[i]));
                triggerOutSource[i] = (EIB7_TRIGGER_OUT_SOURCE)paramFile->ReadWriteUInt32(UInt32(triggerOutSource[i]));
            }

            masterTriggerSource = (EIB7_MASTER_TRIGGER_SOURCE)paramFile->ReadWriteUInt32(UInt32(masterTriggerSource));

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axisTriggerSource[i] = (EIB7_AXIS_TRIGGER_SOURCE)paramFile->ReadWriteUInt32(UInt32(axisTriggerSource[i]));
                axisTriggerEdge[i]   = (EIB7_AXIS_TRIGGER_EDGE)  paramFile->ReadWriteUInt32(UInt32(axisTriggerEdge[i]));
            }

            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxisTriggerSource[i] = (EIB7_AXIS_TRIGGER_SOURCE)paramFile->ReadWriteUInt32(UInt32(auxAxisTriggerSource[i]));
                auxAxisTriggerEdge[i]   = (EIB7_AXIS_TRIGGER_EDGE)  paramFile->ReadWriteUInt32(UInt32(auxAxisTriggerEdge[i]));
            }

            paramFile->ReadWriteUInt32(timerTriggerPeriod);

            intervalCounterMode  = (EIB7_INTERVAL_COUNTER_MODE) paramFile->ReadWriteUInt32(UInt32(intervalCounterMode));
            intervalCounterStart = (EIB7_INTERVAL_COUNTER_START)paramFile->ReadWriteUInt32(UInt32(intervalCounterStart));
            paramFile->ReadWriteUInt32(intervalCounterStartPos);
            paramFile->ReadWriteUInt32(intervalCounterPeriod);

            paramFile->ReadWriteUInt32(pulseCounterCount);
            pulseCounterStart   = (EIB7_PULSE_COUNTER_START)  paramFile->ReadWriteUInt32(UInt32(pulseCounterStart));
            pulseCounterTrigger = (EIB7_PULSE_COUNTER_TRIGGER)paramFile->ReadWriteUInt32(UInt32(pulseCounterTrigger));
        }

        /// <summary>
        /// Gets the EIB number of extern trigger inputs.
        /// </summary>
        property UInt32 TriggerInpNumber
        {
            UInt32 get() { return triggerInpNumber; }
        }

        /// <summary>
        /// Gets the EIB number of extern trigger outputs.
        /// </summary>
        property UInt32 TriggerOutNumber
        {
            UInt32 get() { return triggerOutNumber; }
        }

        /// <summary>
        /// Gets / sets the EIB extern trigger input mode.
        /// </summary>
        property EIB7_IO_MODE TriggerInpMode[UInt32]
        {
            EIB7_IO_MODE get(UInt32 index) { return triggerInpMode[index]; }
            void set(UInt32 index, EIB7_IO_MODE value) { triggerInpMode[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB extern trigger input termination.
        /// </summary>
        property EIB7_ENABLE_MODE TriggerInpTerm[UInt32]
        {
            EIB7_ENABLE_MODE get(UInt32 index) { return triggerInpTerm[index]; }
            void set(UInt32 index, EIB7_ENABLE_MODE value) { triggerInpTerm[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB extern trigger input delay.
        /// </summary>
        property UInt32 TriggerInpDelay[UInt32]
        {
            UInt32 get(UInt32 index) { return triggerInpDelay[index]; }
            void set(UInt32 index, UInt32 value) { triggerInpDelay[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB extern trigger output enable.
        /// </summary>
        property EIB7_ENABLE_MODE TriggerOutEnable[UInt32]
        {
            EIB7_ENABLE_MODE get(UInt32 index) { return triggerOutEnable[index]; }
            void set(UInt32 index, EIB7_ENABLE_MODE value) { triggerOutEnable[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB extern trigger output mode.
        /// </summary>
        property EIB7_IO_MODE TriggerOutMode[UInt32]
        {
            EIB7_IO_MODE get(UInt32 index) { return triggerOutMode[index]; }
            void set(UInt32 index, EIB7_IO_MODE value) { triggerOutMode[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB extern trigger output source.
        /// </summary>
        property EIB7_TRIGGER_OUT_SOURCE TriggerOutSource[UInt32]
        {
            EIB7_TRIGGER_OUT_SOURCE get(UInt32 index) { return triggerOutSource[index]; }
            void set(UInt32 index, EIB7_TRIGGER_OUT_SOURCE value) { triggerOutSource[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB master trigger source.
        /// </summary>
        property EIB7_MASTER_TRIGGER_SOURCE MasterTriggerSource
        {
            EIB7_MASTER_TRIGGER_SOURCE get() { return masterTriggerSource; }
            void set(EIB7_MASTER_TRIGGER_SOURCE value) { masterTriggerSource = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis trigger source.
        /// </summary>
        property EIB7_AXIS_TRIGGER_SOURCE AxisTriggerSource[UInt32]
        {
            EIB7_AXIS_TRIGGER_SOURCE get(UInt32 index) { return axisTriggerSource[index]; }
            void set(UInt32 index, EIB7_AXIS_TRIGGER_SOURCE value) { axisTriggerSource[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB auxiliary axis trigger source.
        /// </summary>
        property EIB7_AXIS_TRIGGER_SOURCE AuxAxisTriggerSource[UInt32]
        {
            EIB7_AXIS_TRIGGER_SOURCE get(UInt32 index) { return auxAxisTriggerSource[index]; }
            void set(UInt32 index, EIB7_AXIS_TRIGGER_SOURCE value) { auxAxisTriggerSource[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB axis trigger source.
        /// </summary>
        property EIB7_AXIS_TRIGGER_EDGE AxisTriggerEdge[UInt32]
        {
            EIB7_AXIS_TRIGGER_EDGE get(UInt32 index) { return axisTriggerEdge[index]; }
            void set(UInt32 index, EIB7_AXIS_TRIGGER_EDGE value) { axisTriggerEdge[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB auxiliary axis trigger source.
        /// </summary>
        property EIB7_AXIS_TRIGGER_EDGE AuxAxisTriggerEdge[UInt32]
        {
            EIB7_AXIS_TRIGGER_EDGE get(UInt32 index) { return auxAxisTriggerEdge[index]; }
            void set(UInt32 index, EIB7_AXIS_TRIGGER_EDGE value) { auxAxisTriggerEdge[index] = value; }
        }

        /// <summary>
        /// Gets / sets the EIB timer trigger period in micro seconds.
        /// </summary>
        property UInt32 TimerTriggerPeriod
        {
            UInt32 get() { return timerTriggerPeriod; }
            void set(UInt32 value) { timerTriggerPeriod = value; }
        }

        /// <summary>
        /// Gets / sets the EIB interval counter mode.
        /// </summary>
        property EIB7_INTERVAL_COUNTER_MODE IntervalCounterMode
        {
            EIB7_INTERVAL_COUNTER_MODE get() { return intervalCounterMode; }
            void set(EIB7_INTERVAL_COUNTER_MODE value) { intervalCounterMode = value; }
        }

        /// <summary>
        /// Gets / sets the EIB interval counter start.
        /// </summary>
        property EIB7_INTERVAL_COUNTER_START IntervalCounterStart
        {
            EIB7_INTERVAL_COUNTER_START get() { return intervalCounterStart; }
            void set(EIB7_INTERVAL_COUNTER_START value) { intervalCounterStart = value; }
        }

        /// <summary>
        /// Gets / sets the EIB interval counter start position.
        /// </summary>
        property UInt32 IntervalCounterStartPos
        {
            UInt32 get() { return intervalCounterStartPos; }
            void set(UInt32 value) { intervalCounterStartPos = value; }
        }

        /// <summary>
        /// Gets / sets the EIB interval counter period.
        /// </summary>
        property UInt32 IntervalCounterPeriod
        {
            UInt32 get() { return intervalCounterPeriod; }
            void set(UInt32 value) { intervalCounterPeriod = value; }
        }

        /// <summary>
        /// Gets / sets the EIB pulse counter count.
        /// </summary>
        property UInt32 PulseCounterCount
        {
            UInt32 get() { return pulseCounterCount; }
            void set(UInt32 value) { pulseCounterCount = value; }
        }

        /// <summary>
        /// Gets / sets the EIB pulse counter start.
        /// </summary>
        property EIB7_PULSE_COUNTER_START PulseCounterStart
        {
            EIB7_PULSE_COUNTER_START get() { return pulseCounterStart; }
            void set(EIB7_PULSE_COUNTER_START value) { pulseCounterStart = value; }
        }

        /// <summary>
        /// Gets / sets the EIB pulse counter trigger.
        /// </summary>
        property EIB7_PULSE_COUNTER_TRIGGER PulseCounterTrigger
        {
            EIB7_PULSE_COUNTER_TRIGGER get() { return pulseCounterTrigger; }
            void set(EIB7_PULSE_COUNTER_TRIGGER value) { pulseCounterTrigger = value; }
        }
    };
	#pragma endregion

 	#pragma region "EIB_Param class definition"
    // default parameters
    #define EIB_DEFAULT_AXIS_TERMINATION        EIB7_ENABLE_MODE::EIB7_MD_Enable

    // EIB axis list
    typedef array<EIB_Axis_Param ^>     AxisList;
    // EIB auxiliary axis list
    typedef array<EIB_AuxAxis_Param ^>  AuxAxisList;

    /// <summary>
    /// This class contains all EIB parameters.
    /// </summary>
    public ref class EIB_Param
    {
    private:
        UInt32              fileVersion;        ///< parameter version for parameter file

        UInt32              axisNumber;         ///< EIB number of axis
        UInt32              auxAxisNumber;      ///< EIB number of auxiliary axis

        EIB7_ENABLE_MODE    axisTermination;    ///< EIB axis termination

        EIB_Network_Param   network;            ///< EIB network parameters
        EIB_Trigger_Param   trigger;            ///< EIB trigger parameters
        EIB_OpMode_Param    opMode;             ///< EIB operation mode parameters

        AxisList ^          axis;               ///< EIB axis parameters
        AuxAxisList ^       auxAxis;            ///< EIB auxiliary axis parameter

        String ^            eibName;            ///< EIB name shown in the Main Tree View
        Int32               treeIndex;          ///< EIB index in the Main Tree View

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_Param()
        {
            fileVersion = 1;

            axisNumber    = EIB_MAX_AXIS_NUMBER;
            auxAxisNumber = EIB_MAX_AUX_AXIS_NUMBER;
            eibName       = L"";
            treeIndex     = -1;

            axisTermination = EIB_DEFAULT_AXIS_TERMINATION;

            axis = gcnew AxisList(EIB_MAX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axis[i] = gcnew EIB_Axis_Param(nullptr);
            }
            auxAxis = gcnew AuxAxisList(EIB_MAX_AUX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxis[i] = gcnew EIB_AuxAxis_Param(axis[i]);
                axis[i]->AuxAxis = auxAxis[i];
            }
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Param ^ source)
        {
            eibName         = String::Copy(source->eibName);
            axisNumber      = source->axisNumber;
            auxAxisNumber   = source->auxAxisNumber;
            axisTermination = source->axisTermination;
            network.Copy(%source->network);
            trigger.Copy(%source->trigger);
            opMode.Copy( %source->opMode);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axis[i]->Copy(source->axis[i]);
            }
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxis[i]->Copy(source->auxAxis[i]);
            }
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_Param ^ source)
        {
            if (eibName         !=  source->eibName)         return false;
            if (axisNumber      !=  source->axisNumber)      return false;
            if (auxAxisNumber   !=  source->auxAxisNumber)   return false;
            if (axisTermination !=  source->axisTermination) return false;
            if (network.IsEqual(%source->network) == false)  return false;
            if (trigger.IsEqual(%source->trigger) == false)  return false;
            if (opMode.IsEqual( %source->opMode)  == false)  return false;
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                if (axis[i]->IsEqual(source->axis[i]) == false) return false;
            }
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                if (auxAxis[i]->IsEqual(source->auxAxis[i]) == false) return false;
            }
            return true;
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
		/// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(EIB_ParamFile ^ paramFile)
        {
            // read / write parameter version
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            paramFile->ReadWriteString(eibName);
            paramFile->ReadWriteUInt32(axisNumber);
            paramFile->ReadWriteUInt32(auxAxisNumber);
            axisTermination = (EIB7_ENABLE_MODE)paramFile->ReadWriteUInt32(UInt32(axisTermination));
            network.ReadWriteFile(paramFile);
            trigger.ReadWriteFile(paramFile);
            opMode.ReadWriteFile(paramFile);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axis[i]->ReadWriteFile(paramFile);
            }
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxis[i]->ReadWriteFile(paramFile);
            }
        }

        /// <summary>
        /// Gets / sets the EIB name.
        /// </summary>
        property String ^ EibName
        {
            String ^ get() { return eibName; }
            void set(String ^ value) { eibName = String::Copy(value); }
        }

        /// <summary>
        /// Gets / sets the EIB index in the Main Tree View.
        /// </summary>
        property UInt32 TreeIndex
        {
            UInt32 get() { return treeIndex; }
            void set(UInt32 value) { treeIndex = value; }
        }

        /// <summary>
        /// Gets the EIB number of axis.
        /// </summary>
        property UInt32 AxisNumber
        {
            UInt32 get() { return axisNumber; }
        }

        /// <summary>
        /// Gets the EIB number of auxiliary axis.
        /// </summary>
        property UInt32 AuxAxisNumber
        {
            UInt32 get() { return auxAxisNumber; }
        }

        /// <summary>
        /// Gets / sets the EIB axis termination.
        /// </summary>
        property EIB7_ENABLE_MODE AxisTermination
        {
            EIB7_ENABLE_MODE get() { return axisTermination; }
            void set(EIB7_ENABLE_MODE value) { axisTermination = value; }
        }

        /// <summary>
        /// Gets the EIB network parameters.
        /// </summary>
        property EIB_Network_Param ^ Network
        {
            EIB_Network_Param ^ get() { return %network; }
        }

        /// <summary>
        /// Gets the EIB trigger parameters.
        /// </summary>
        property EIB_Trigger_Param ^ Trigger
        {
            EIB_Trigger_Param ^ get() { return %trigger; }
        }

        /// <summary>
        /// Gets the EIB operation mode parameters.
        /// </summary>
        property EIB_OpMode_Param ^ OpMode
        {
            EIB_OpMode_Param ^ get() { return %opMode; }
        }

        /// <summary>
        /// Gets the EIB axis parameters.
        /// </summary>
        property EIB_Axis_Param ^ Axis[UInt32]
        {
            EIB_Axis_Param ^ get(UInt32 index) { return axis[index]; }
        }

        /// <summary>
        /// Gets the EIB auxiliary axis parameters.
        /// </summary>
        property EIB_AuxAxis_Param ^ AuxAxis[UInt32]
        {
            EIB_AuxAxis_Param ^ get(UInt32 index) { return auxAxis[index]; }
        }
    };
	#pragma endregion
}
