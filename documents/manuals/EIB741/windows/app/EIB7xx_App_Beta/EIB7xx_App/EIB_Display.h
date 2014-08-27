///
/// \file       EIB_Display.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_Display.h,v 1.3 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: EIB_Display.h,v $
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
/// \brief      EIB display classes.
///
/// This classes are used for the management of the parameters and operations
/// used to display the measurement results.
///

#pragma once

#include "FormMain.h"

using namespace System;

namespace EIB7xx_App
{
 	#pragma region "Common display definitions"
    // special definitions
    #define EIB_DISPLAY_TIMER_PERIOD            UInt32(100)
	#pragma endregion

 	#pragma region "EIB_Display_Param class definition"
    // default parameters
    #define EIB_DEFAULT_DISPLAY_PERIOD          UInt32(      1000)
    #define EIB_MIN_DISPLAY_PERIOD              EIB_DISPLAY_TIMER_PERIOD
    #define EIB_MAX_DISPLAY_PERIOD              UInt32(     10000)
    #define EIB_STEP_DISPLAY_PERIOD             EIB_DISPLAY_TIMER_PERIOD
    #define EIB_DEFAULT_DISPLAY_PRECISION       EIB7_PREC_NUM::EIB7_PREC_3

    /// <summary>
    /// This enum lists all EIB display precision numbers.
    /// </summary>
    enum class EIB7_PREC_NUM : UInt32
    {
        EIB7_PREC_0  =  0,
        EIB7_PREC_1,
        EIB7_PREC_2,
        EIB7_PREC_3,
        EIB7_PREC_4,
        EIB7_PREC_5,
        EIB7_PREC_6,
    };

    /// <summary>
    /// This class contains all EIB display parameters.
    /// </summary>
    public ref class EIB_Display_Param
    {
    private:
        UInt32              fileVersion;        ///< parameter version for parameter file

        UInt32              displayPeriod;      ///< EIB display period in milliseconds
        EIB7_PREC_NUM       displayPrecision;   ///< EIB display precision

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_Display_Param()
        {
            fileVersion = 1;

            displayPeriod    = EIB_DEFAULT_DISPLAY_PERIOD;
            displayPrecision = EIB_DEFAULT_DISPLAY_PRECISION;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Display_Param ^ source)
        {
            displayPeriod    = source->displayPeriod;
            displayPrecision = source->displayPrecision;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
		/// <param name="source">The source parameters.</param>
		/// <param name="checkNoEibParam">If true check parameters not send to EIB.</param>
        bool IsEqual(EIB_Display_Param ^ source, bool checkNoEibParam)
        {
            if (checkNoEibParam == true)
            {
                if (displayPeriod    != source->displayPeriod)    return false;
                if (displayPrecision != source->displayPrecision) return false;
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
            paramFile->ReadWriteUInt32(displayPeriod);
            displayPrecision = (EIB7_PREC_NUM)paramFile->ReadWriteUInt32(UInt32(displayPrecision));
        }

        /// <summary>
        /// Gets / sets the EIB display period in milliseconds.
        /// </summary>
        property UInt32 DisplayPeriod
        {
            UInt32 get() { return displayPeriod; }
            void set(UInt32 value) { displayPeriod = value; }
        }

        /// <summary>
        /// Gets / sets the EIB display precision.
        /// </summary>
        property EIB7_PREC_NUM DisplayPrecision
        {
            EIB7_PREC_NUM get() { return displayPrecision; }
            void set(EIB7_PREC_NUM value) { displayPrecision = value; }
        }
    };
	#pragma endregion

 	#pragma region "PositionData class definition"
    /// <summary>
    /// This class contains all EIB position display values.
    /// </summary>
    public ref class PositionData
    {
    private:
        bool        valid;          ///< position valid flag
        Double      position;       ///< position value
        String ^    unit;           ///< position unit string
        bool        absolute;       ///< absolute position flag
        bool        multiturn;      ///< multiturn encoder flag
        Int64       revolution;     ///< revolution value

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        PositionData()
        {
            Clear();
        }

        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            valid       = false;
            position    = 0;
            unit        = L"";
            absolute    = false;
            multiturn   = false;
            revolution  = 0;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(PositionData ^ source)
        {
            valid       = source->valid;
            position    = source->position;
            unit        = String::Copy(source->unit);
            absolute    = source->absolute;
            multiturn   = source->multiturn;
            revolution  = source->revolution;
        }

        /// <summary>
        /// Gets / sets the position valid flag.
        /// </summary>
        property bool Valid
        {
            bool get() { return valid; }
            void set(bool value) { valid = value; }
        }

        /// <summary>
        /// Gets / sets the position value.
        /// </summary>
        property Double Position
        {
            Double get() { return position; }
            void set(Double value) { position = value; }
        }

        /// <summary>
        /// Gets / sets the position unit string.
        /// </summary>
        property String ^ Unit
        {
            String ^ get() { return unit; }
            void set(String ^ value) { unit = value; }
        }

        /// <summary>
        /// Gets / sets the absolute position flag.
        /// </summary>
        property bool Absolute
        {
            bool get() { return absolute; }
            void set(bool value) { absolute = value; }
        }

        /// <summary>
        /// Gets / sets the multiturn encoder flag.
        /// </summary>
        property bool Multiturn
        {
            bool get() { return multiturn; }
            void set(bool value) { multiturn = value; }
        }

        /// <summary>
        /// Gets / sets the revolution value.
        /// </summary>
        property Int64 Revolution
        {
            Int64 get() { return revolution; }
            void set(Int64 value) { revolution = value; }
        }
    };
	#pragma endregion

 	#pragma region "DataHistoryData class definition"
    // special definitions
    #define ADD_INFO_LIST_SIZE      10  ///< entry number in add info history list

    // additional information history list
    typedef array<UInt32>       EnDatAddInfoList;

    /// <summary>
    /// This class contains the data packet data history.
    /// </summary>
    public ref class DataHistoryData
    {
    public:
        Int32               enDatAI1Count;  ///< EnDat add info 1 history list count
        Int32               enDatAI1Index;  ///< EnDat add info 1 history list index
        EnDatAddInfoList ^  enDatAI1List;   ///< EnDat add info 1 history list

        Int32               enDatAI2Count;  ///< EnDat add info 2 history list count
        Int32               enDatAI2Index;  ///< EnDat add info 2 history list index
        EnDatAddInfoList ^  enDatAI2List;   ///< EnDat add info 2 history list

        Int64   enDatAI1Pos2Temp;       ///< EnDat add info 1 position 2 temporary value
        bool    enDatAI1Pos2Set0;       ///< EnDat add info 1 position 2 word 0 set flag
        bool    enDatAI1Pos2Set1;       ///< EnDat add info 1 position 2 word 1 set flag
        bool    enDatAI1Pos2Set2;       ///< EnDat add info 1 position 2 word 2 set flag

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        DataHistoryData()
        {
            enDatAI1List = gcnew EnDatAddInfoList(ADD_INFO_LIST_SIZE);
            enDatAI2List = gcnew EnDatAddInfoList(ADD_INFO_LIST_SIZE);

            Clear();
        }

    protected:
		/// <summary>
		/// Destructor.
		/// </summary>
		~DataHistoryData()
		{
            // close EIB entry
            Delete();
        }

		/// <summary>
		/// Finalizer.
		/// </summary>
		!DataHistoryData()
		{
            // close EIB entry
            Delete();
		}

    private:
        /// <summary>
        /// Delete function.
        /// </summary>
        void Delete()
        {
            if (enDatAI1List != nullptr)
            {
                delete enDatAI1List;
                enDatAI1List = nullptr;
            }
            if (enDatAI2List != nullptr)
            {
                delete enDatAI2List;
                enDatAI2List = nullptr;
            }
        }

    public:
        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            enDatAI1Count     = 0;
            enDatAI1Index     = 0;

            enDatAI2Count     = 0;
            enDatAI2Index     = 0;

            enDatAI1Pos2Set0  = false;
            enDatAI1Pos2Set1  = false;
            enDatAI1Pos2Set2  = false;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(DataHistoryData ^ source)
        {
            enDatAI1Count = source->enDatAI1Count;
            enDatAI1Index = source->enDatAI1Index;
            for (UInt32 i = 0; i < ADD_INFO_LIST_SIZE; i ++)
            {
                enDatAI1List[i] = source->enDatAI1List[i];
            }
            enDatAI2Count = source->enDatAI2Count;
            enDatAI2Index = source->enDatAI2Index;
            for (UInt32 i = 0; i < ADD_INFO_LIST_SIZE; i ++)
            {
                enDatAI2List[i] = source->enDatAI2List[i];
            }
        }
    };
	#pragma endregion

 	#pragma region "DataPacketData class definition"
    /// <summary>
    /// This enum lists data packet status values.
    /// </summary>
    enum class DataPacketStatus : UInt16
    {
        PositionValid       = 0x0001,   ///< position value is valid
        RefPos1Valid        = 0x0100,   ///< refernce position 1 value is valid
        RefPos2Valid        = 0x0200,   ///< refernce position 2 value is valid
        RefPosCValid        = 0x0400,   ///< dist. coded ref. pos. value is valid

        EnDatAddInfoValid   = 0x0001,   ///< position value is valid
        ContentsI0          = 0x0020,   ///< contents I0
        ContentsI1          = 0x0040,   ///< contents I1
        ContentsI2          = 0x0080,   ///< contents I2
        ContentsI3          = 0x0100,   ///< contents I3
        ContentsI4          = 0x0200,   ///< contents I4
        EnDatBusy           = 0x0400,   ///< EnDat busy bit
        EnDatRM             = 0x0800,   ///< EnDat RM bit
        EnDatWRN            = 0x1000,   ///< EnDat WRN bit

        ContentsShift       = 0x0005,   ///< contents shift value
        ContentsMask        = 0x001F,   ///< contents mask value

        ErrorAmplitude      = 0x0002,   ///< error signal amplitude
        ErrorEnDatCRC       = 0x0002,   ///< error EnDat CRC
        ErrorFrequency      = 0x0008,   ///< error frequency exceeded
        ErrorPower          = 0x0010,   ///< error encoder power supply
        ErrorFan            = 0x0020,   ///< error fan
        ErrorLostTrigger    = 0x0080,   ///< error lost trigger
        ErrorEnDat1         = 0x0100,   ///< error EnDat 1
        ErrorEnDat2         = 0x0200,   ///< error EnDat 2
        ErrorRefPosC        = 0x0800,   ///< error dist. coded ref. pos. value

        ErrorStatusInc      = 0x08BA,   ///< error incremental status value
        ErrorStatusAux      = 0x00BA,   ///< error auxiliary axis status value
        ErrorStatusEnDat    = 0x03BA,   ///< error EnDat status value
        ErrorStatusAI       = 0x0002,   ///< error add info status value
    };

    /// <summary>
    /// This enum lists data packet status commands.
    /// </summary>
    enum class DataPacketCmd : UInt32
    {
        None,
        ResetErrorSignal,           ///< reset error signal
        ResetErrorPower,            ///< reset error encoder power supply
        ResetErrorLostTrigger,      ///< reset error lost trigger
        ResetErrorRefC,             ///< reset error dist. coded ref. pos. value
    };

    /// <summary>
    /// This class contains all EIB data packet display values.
    /// </summary>
    public ref class DataPacketData
    {
    private:
        UInt16  statusWordValue;        ///< status word value
        bool    statusWordUsed;         ///< status word used flag
        Int64   positionDataValue;      ///< position data value
        bool    positionDataUsed;       ///< position data used flag
        UInt32  timestampValue;         ///< timestamp value
        bool    timestampUsed;          ///< timestamp used flag
        Int64   referencePos1Value;     ///< reference position 1 value
        bool    referencePos1Used;      ///< reference position 1 used flag
        Int64   referencePos2Value;     ///< reference position 2 value
        bool    referencePos2Used;      ///< reference position 2 used flag
        Int64   distCodedRefValue;      ///< distance coded reference position value
        bool    distCodedRefUsed;       ///< distance coded reference position used flag
        Int16   analogAValue;           ///< analog signal A value
        Int16   analogBValue;           ///< analog signal B value
        bool    analogUsed;             ///< analog value used flag
        UInt16  enDatAI1StatusValue;    ///< EnDat additional info 1 status value
        UInt16  enDatAI1InfoValue;      ///< EnDat additional info 1 info value
        bool    enDatAI1Used;           ///< EnDat additional info 1 used flag
        UInt16  enDatAI2StatusValue;    ///< EnDat additional info 2 status value
        UInt16  enDatAI2InfoValue;      ///< EnDat additional info 2 info value
        bool    enDatAI2Used;           ///< EnDat additional info 2 used flag

        Int64   enDatAI1Pos2Value;      ///< EnDat add info 1 position 2 value
        bool    enDatAI1Pos2Valid;      ///< EnDat add info 1 position 2 valid flag

        PositionData        positionData;   ///< position data

        DataHistoryData ^   dataHistory;    ///< data packet data history

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        DataPacketData(DataHistoryData ^ dataHistory)
        {
            this->dataHistory = dataHistory;

            Clear();
        }

        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            statusWordUsed    = false;
            positionDataUsed  = false;
            timestampUsed     = false;
            referencePos1Used = false;
            referencePos2Used = false;
            distCodedRefUsed  = false;
            analogUsed        = false;
            enDatAI1Used      = false;
            enDatAI2Used      = false;

            enDatAI1Pos2Valid = false;

            positionData.Clear();
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(DataPacketData ^ source)
        {
            statusWordValue     = source->statusWordValue;
            statusWordUsed      = source->statusWordUsed;
            positionDataValue   = source->positionDataValue;
            positionDataUsed    = source->positionDataUsed;
            timestampValue      = source->timestampValue;
            timestampUsed       = source->timestampUsed;
            referencePos1Value  = source->referencePos1Value;
            referencePos1Used   = source->referencePos1Used;
            referencePos2Value  = source->referencePos2Value;
            referencePos2Used   = source->referencePos2Used;
            distCodedRefValue   = source->distCodedRefValue;
            distCodedRefUsed    = source->distCodedRefUsed;
            analogAValue        = source->analogAValue;
            analogBValue        = source->analogBValue;
            analogUsed          = source->analogUsed;
            enDatAI1StatusValue = source->enDatAI1StatusValue;
            enDatAI1InfoValue   = source->enDatAI1InfoValue;
            enDatAI1Used        = source->enDatAI1Used;
            enDatAI2StatusValue = source->enDatAI2StatusValue;
            enDatAI2InfoValue   = source->enDatAI2InfoValue;
            enDatAI2Used        = source->enDatAI2Used;
                                          
            enDatAI1Pos2Value   = source->enDatAI1Pos2Value;
            enDatAI1Pos2Valid   = source->enDatAI1Pos2Valid;

            positionData.Copy(%source->positionData);
        }

        /// <summary>
        /// Gets / sets the status word value.
        /// </summary>
        property UInt16 StatusWordValue
        {
            UInt16 get() { return statusWordValue; }
            void set(UInt16 value)
            {
                statusWordValue = value;
                statusWordUsed  = true;
            }
        }

        /// <summary>
        /// Gets / sets the status word used flag.
        /// </summary>
        property bool StatusWordUsed
        {
            bool get() { return statusWordUsed; }
            void set(bool value) { statusWordUsed = value; }
        }

        /// <summary>
        /// Gets / sets the position data value.
        /// </summary>
        property Int64 PositionDataValue
        {
            Int64 get() { return positionDataValue; }
            void set(Int64 value)
            {
                positionDataValue = value;
                positionDataUsed  = true;
            }
        }

        /// <summary>
        /// Gets / sets the position data used flag.
        /// </summary>
        property bool PositionDataUsed
        {
            bool get() { return positionDataUsed; }
            void set(bool value) { positionDataUsed = value; }
        }

        /// <summary>
        /// Gets / sets the timestamp value.
        /// </summary>
        property UInt32 TimestampValue
        {
            UInt32 get() { return timestampValue; }
            void set(UInt32 value)
            {
                timestampValue = value;
                timestampUsed  = true;
            }
        }

        /// <summary>
        /// Gets / sets the timestamp used flag.
        /// </summary>
        property bool TimestampUsed
        {
            bool get() { return timestampUsed; }
            void set(bool value) { timestampUsed = value; }
        }

        /// <summary>
        /// Gets / sets the reference position 1 value.
        /// </summary>
        property Int64 ReferencePos1Value
        {
            Int64 get() { return referencePos1Value; }
            void set(Int64 value)
            {
                referencePos1Value = value;
                referencePos1Used  = true;
            }
        }

        /// <summary>
        /// Gets / sets the reference position 1 used flag.
        /// </summary>
        property bool ReferencePos1Used
        {
            bool get() { return referencePos1Used; }
            void set(bool value) { referencePos1Used = value; }
        }

        /// <summary>
        /// Gets / sets the reference position 2 value.
        /// </summary>
        property Int64 ReferencePos2Value
        {
            Int64 get() { return referencePos2Value; }
            void set(Int64 value)
            {
                referencePos2Value = value;
                referencePos2Used  = true;
            }
        }

        /// <summary>
        /// Gets / sets the reference position 2 used flag.
        /// </summary>
        property bool ReferencePos2Used
        {
            bool get() { return referencePos2Used; }
            void set(bool value) { referencePos2Used = value; }
        }

        /// <summary>
        /// Gets / sets the distance coded reference position value.
        /// </summary>
        property Int64 DistCodedRefValue
        {
            Int64 get() { return distCodedRefValue; }
            void set(Int64 value)
            {
                distCodedRefValue = value;
                distCodedRefUsed  = true;
            }
        }

        /// <summary>
        /// Gets / sets the distance coded reference position used flag.
        /// </summary>
        property bool DistCodedRefUsed
        {
            bool get() { return distCodedRefUsed; }
            void set(bool value) { distCodedRefUsed = value; }
        }

        /// <summary>
        /// Gets / sets the analog signal A value.
        /// </summary>
        property Int16 AnalogAValue
        {
            Int16 get() { return analogAValue; }
            void set(Int16 value)
            {
                analogAValue = value;
                analogUsed   = true;
            }
        }

        /// <summary>
        /// Gets / sets the analog signal B value.
        /// </summary>
        property Int16 AnalogBValue
        {
            Int16 get() { return analogBValue; }
            void set(Int16 value)
            {
                analogBValue = value;
                analogUsed   = true;
            }
        }

        /// <summary>
        /// Gets / sets the analog value used flag.
        /// </summary>
        property bool AnalogUsed
        {
            bool get() { return analogUsed; }
            void set(bool value) { analogUsed = value; }
        }

        /// <summary>
        /// Gets / sets the EnDat additional info 1 data value.
        /// </summary>
        property UInt32 EnDatAI1DataValue
        {
            UInt32 get()
            {
                return (UInt32(enDatAI1InfoValue) << 16) + enDatAI1StatusValue;
            }
            void set(UInt32 value)
            {
                // set add info 1 status and value
                enDatAI1StatusValue = UInt16(value);
                enDatAI1InfoValue   = UInt16(value >> 16);
                enDatAI1Used        = true;
                // add to history buffer
                dataHistory->enDatAI1List[dataHistory->enDatAI1Index] = value;
                dataHistory->enDatAI1Count ++;
                if (dataHistory->enDatAI1Count >= ADD_INFO_LIST_SIZE)
                {
                    dataHistory->enDatAI1Count = ADD_INFO_LIST_SIZE;
                }
                dataHistory->enDatAI1Index ++;
                if (dataHistory->enDatAI1Index >= ADD_INFO_LIST_SIZE)
                {
                    dataHistory->enDatAI1Index = 0;
                }
                // check status
                if (statusWordUsed == true)
                {
                    // check for EnDat errors
                    if (   ((statusWordValue & UInt16(DataPacketStatus::ErrorEnDat1)) != 0)
                        || ((statusWordValue & UInt16(DataPacketStatus::ErrorEnDat2)) != 0))
                    {
                        // in case of errors position 2 is invalid
                        enDatAI1Pos2Valid = false;
                    }
                    else
                    {
                        // check for new position 2 value
                        if (   ((enDatAI1StatusValue & UInt16(DataPacketStatus::EnDatAddInfoValid)) != 0)
                            && ((enDatAI1StatusValue & UInt16(DataPacketStatus::EnDatRM)) != 0))
                        {
                            // check add info contents
                            UInt32 contents =   (   enDatAI1StatusValue
                                                 >> UInt16(DataPacketStatus::ContentsShift))
                                              & UInt16(DataPacketStatus::ContentsMask);
                            if (contents == UInt32(EIB7_ADD_INFO_ALL::EIB7_AI1_Position2_word1))
                            {
                                // save add info 1 position 2 word 0 value
                                dataHistory->enDatAI1Pos2Temp = enDatAI1InfoValue;
                                dataHistory->enDatAI1Pos2Set0 = true;
                                // wait for add info 1 position 2 word 1 and 2 values
                                dataHistory->enDatAI1Pos2Set1 = false;
                                dataHistory->enDatAI1Pos2Set2 = false;
                            }
                            else if (   (dataHistory->enDatAI1Pos2Set0 == true)
                                     && (dataHistory->enDatAI1Pos2Set1 == false)
                                     && (contents == UInt32(EIB7_ADD_INFO_ALL::EIB7_AI1_Position2_word2)))
                            {
                                // save add info 1 position 2 word 1 value
                                dataHistory->enDatAI1Pos2Temp |= Int64(enDatAI1InfoValue) << 16;
                                dataHistory->enDatAI1Pos2Set1  = true;
                            }
                            else if (   (dataHistory->enDatAI1Pos2Set0 == true)
                                     && (dataHistory->enDatAI1Pos2Set2 == false)
                                     && (contents == UInt32(EIB7_ADD_INFO_ALL::EIB7_AI1_Position2_word3)))
                            {
                                // save add info 1 position 2 word 2 value
                                dataHistory->enDatAI1Pos2Temp |= Int64(enDatAI1InfoValue) << 32;
                                dataHistory->enDatAI1Pos2Set2  = true;
                            }
                            if (   (dataHistory->enDatAI1Pos2Set0 == true)
                                && (dataHistory->enDatAI1Pos2Set1 == true)
                                && (dataHistory->enDatAI1Pos2Set2 == true))
                            {
                                // add info 1 position 2 value is valid
                                enDatAI1Pos2Value = dataHistory->enDatAI1Pos2Temp;
                                enDatAI1Pos2Valid = true;
                                // wait for new add info 1 position 2 word 0 value
                                dataHistory->enDatAI1Pos2Set0 = false;
                                //Debug::Print(String::Format(L"EnDat AI 1 Pos 2: 0x{0,16:X16}",
                                //                            enDatAI1Pos2Value));
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Gets the EnDat additional info 1 value count.
        /// </summary>
        property Int32 EnDatAI1Count
        {
            Int32 get()
            {
                if (dataHistory->enDatAI1Count == 0)
                {
                    return 0;
                }
                return dataHistory->enDatAI1Count - 1;
            }
        }

        /// <summary>
        /// Gets the EnDat additional info 1 status value.
        /// </summary>
        property UInt16 EnDatAI1StatusValue[Int32]
        {
            UInt16 get(Int32 index)
            {
                if (index == 0)
                {
                    return enDatAI1StatusValue;
                }
                index = dataHistory->enDatAI1Index - 1 - index;
                if (index < 0)
                {
                    index += ADD_INFO_LIST_SIZE;
                }
                return UInt16(dataHistory->enDatAI1List[index]);
            }
        }

        /// <summary>
        /// Gets the EnDat additional info 1 info value.
        /// </summary>
        property UInt16 EnDatAI1InfoValue[Int32]
        {
            UInt16 get(Int32 index)
            {
                if (index == 0)
                {
                    return enDatAI1InfoValue;
                }
                index = dataHistory->enDatAI1Index - 1 - index;
                if (index < 0)
                {
                    index += ADD_INFO_LIST_SIZE;
                }
                return UInt16(dataHistory->enDatAI1List[index] >> 16);
            }
        }

        /// <summary>
        /// Gets / sets the EnDat additional info 1 used flag.
        /// </summary>
        property bool EnDatAI1Used
        {
            bool get() { return enDatAI1Used; }
            void set(bool value) { enDatAI1Used = value; }
        }

        /// <summary>
        /// Gets / sets the EnDat additional info 2 data value.
        /// </summary>
        property UInt32 EnDatAI2DataValue
        {
            UInt32 get()
            {
                return (UInt32(enDatAI2InfoValue) << 16) + enDatAI2StatusValue;
            }
            void set(UInt32 value)
            {
                // set add info 2 status and value
                enDatAI2StatusValue = UInt16(value);
                enDatAI2InfoValue   = UInt16(value >> 16);
                enDatAI2Used        = true;
                // add to history buffer
                dataHistory->enDatAI2List[dataHistory->enDatAI2Index] = value;
                dataHistory->enDatAI2Count ++;
                if (dataHistory->enDatAI2Count >= ADD_INFO_LIST_SIZE)
                {
                    dataHistory->enDatAI2Count = ADD_INFO_LIST_SIZE;
                }
                dataHistory->enDatAI2Index ++;
                if (dataHistory->enDatAI2Index >= ADD_INFO_LIST_SIZE)
                {
                    dataHistory->enDatAI2Index = 0;
                }
            }
        }

        /// <summary>
        /// Gets the EnDat additional info 2 value count.
        /// </summary>
        property Int32 EnDatAI2Count
        {
            Int32 get()
            {
                if (dataHistory->enDatAI2Count == 0)
                {
                    return 0;
                }
                return dataHistory->enDatAI2Count - 1;
            }
        }

        /// <summary>
        /// Gets the EnDat additional info 2 status value.
        /// </summary>
        property UInt16 EnDatAI2StatusValue[Int32]
        {
            UInt16 get(Int32 index)
            {
                if (index == 0)
                {
                    return enDatAI2StatusValue;
                }
                index = dataHistory->enDatAI2Index - 1 - index;
                if (index < 0)
                {
                    index += ADD_INFO_LIST_SIZE;
                }
                return UInt16(dataHistory->enDatAI2List[index]);
            }
        }

        /// <summary>
        /// Gets the EnDat additional info 2 info value.
        /// </summary>
        property UInt16 EnDatAI2InfoValue[Int32]
        {
            UInt16 get(Int32 index)
            {
                if (index == 0)
                {
                    return enDatAI2InfoValue;
                }
                index = dataHistory->enDatAI2Index - 1 - index;
                if (index < 0)
                {
                    index += ADD_INFO_LIST_SIZE;
                }
                return UInt16(dataHistory->enDatAI2List[index] >> 16);
            }
        }

        /// <summary>
        /// Gets / sets the EnDat additional info 2 used flag.
        /// </summary>
        property bool EnDatAI2Used
        {
            bool get() { return enDatAI2Used; }
            void set(bool value) { enDatAI2Used = value; }
        }

        /// <summary>
        /// Gets the EnDat additional info 1 position 2 value.
        /// </summary>
        property Int64 EnDatAI1Pos2Value
        {
            Int64 get() { return enDatAI1Pos2Value; }
        }

        /// <summary>
        /// Gets / sets the EnDat additional info 1 position 2 valid flag.
        /// </summary>
        property bool EnDatAI1Pos2Valid
        {
            bool get() { return enDatAI1Pos2Valid; }
            void set(bool value) { enDatAI1Pos2Valid = value; }
        }

        /// <summary>
        /// Gets the position data.
        /// </summary>
        property PositionData ^ PositionValues
        {
            PositionData ^ get() { return %positionData; }
        }
    };
	#pragma endregion

 	#pragma region "DataPacketEntry class definition"
    // data packet data history list
    typedef array<DataHistoryData ^>    DataHistoryList;
    // data packet axis data list
    typedef array<DataPacketData ^>     AxisDataList;

    /// <summary>
    /// This class contains all EIB data packet display values.
    /// </summary>
    public ref class DataPacketEntry
    {
    private:
        AxisDataList ^      axis;       ///< data packet axis data
        AxisDataList ^      auxAxis;    ///< data packet auxiliary axis data

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        DataPacketEntry(DataHistoryList ^ axisHistory,
                        DataHistoryList ^ auxAxisHistory)
        {
            axis = gcnew AxisDataList(EIB_MAX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axis[i] = gcnew DataPacketData(axisHistory[i]);
            }
            auxAxis = gcnew AxisDataList(EIB_MAX_AUX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxis[i] = gcnew DataPacketData(auxAxisHistory[i]);
            }

            Clear();
        }

    protected:
		/// <summary>
		/// Destructor.
		/// </summary>
		~DataPacketEntry()
		{
            // close EIB entry
            Delete();
        }

		/// <summary>
		/// Finalizer.
		/// </summary>
		!DataPacketEntry()
		{
            // close EIB entry
            Delete();
		}

    private:
        /// <summary>
        /// Delete function.
        /// </summary>
        void Delete()
        {
            if (axis != nullptr)
            {
                for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
                {
                    delete axis[i];
                }
                delete axis;
                axis = nullptr;
            }
            if (auxAxis != nullptr)
            {
                for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
                {
                    delete auxAxis[i];
                }
                delete auxAxis;
                auxAxis = nullptr;
            }
        }

    public:
        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axis[i]->Clear();
            }
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxis[i]->Clear();
            }
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(DataPacketEntry ^ source)
        {
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
        /// Gets the data packet axis data.
        /// </summary>
        property DataPacketData ^ Axis[UInt32]
        {
            DataPacketData ^ get(UInt32 index) { return axis[index]; }
        }

        /// <summary>
        /// Gets the data packet auxiliary axis data.
        /// </summary>
        property DataPacketData ^ AuxAxis[UInt32]
        {
            DataPacketData ^ get(UInt32 index) { return auxAxis[index]; }
        }
    };
	#pragma endregion

 	#pragma region "DataPacketBuffer class definition"
    /// special definitions
    #define EIB_READ_BUFFER_NUMBER      UInt32( 32)     ///< number of read buffers

    /// data packet data entry list
    typedef array<DataPacketEntry ^>    DataEntryList;

    /// <summary>
    /// This class contains all EIB data packet display values.
    /// </summary>
    public ref class DataPacketBuffer
    {
    private:
        UInt32  readDataCount;          ///< read data counter value
        UInt32  firstTriggerCount;      ///< first trigger counter value
        UInt32  triggerCounterValue;    ///< trigger counter value
        bool    triggerCounterUsed;     ///< trigger counter used flag

        DataHistoryList ^   axisHistory;        ///< axis data history
        DataHistoryList ^   auxAxisHistory;     ///< auxiliary axis data history

        DataEntryList ^     entry;      ///< data packet data entries

        Int32   writeIndex;             ///< current used read buffer for write

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        DataPacketBuffer(UInt32 entryNumber)
        {
            axisHistory = gcnew DataHistoryList(EIB_MAX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axisHistory[i] = gcnew DataHistoryData;
            }
            auxAxisHistory = gcnew DataHistoryList(EIB_MAX_AUX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxisHistory[i] = gcnew DataHistoryData;
            }

            entry = gcnew DataEntryList(entryNumber);
            for (UInt32 i = 0; i < entryNumber; i ++)
            {
                entry[i] = gcnew DataPacketEntry(axisHistory, auxAxisHistory);
            }

            Clear();
        }

    protected:
		/// <summary>
		/// Destructor.
		/// </summary>
		~DataPacketBuffer()
		{
            // close EIB entry
            Delete();
        }

		/// <summary>
		/// Finalizer.
		/// </summary>
		!DataPacketBuffer()
		{
            // close EIB entry
            Delete();
		}

    private:
        /// <summary>
        /// Delete function.
        /// </summary>
        void Delete()
        {
            if (entry != nullptr)
            {
                for (Int32 i = 0; i < entry->Length; i ++)
                {
                    delete entry[i];
                }
                delete entry;
                entry = nullptr;
            }
        }

    public:
        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            readDataCount      = 0;
            triggerCounterUsed = false;

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axisHistory[i]->Clear();
            }
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxisHistory[i]->Clear();
            }

            for (Int32 i = 0; i < entry->Length; i ++)
            {
                entry[i]->Clear();
            }

            writeIndex = 0;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(DataPacketBuffer ^ source, UInt32 sourceIndex)
        {
            readDataCount       = source->readDataCount;
            firstTriggerCount   = source->firstTriggerCount;
            triggerCounterValue = source->triggerCounterValue;
            triggerCounterUsed  = source->triggerCounterUsed;

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axisHistory[i]->Copy(source->axisHistory[i]);
            }
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxisHistory[i]->Copy(source->auxAxisHistory[i]);
            }

            entry[0]->Copy(source->entry[sourceIndex]);
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(UInt32 destIndex, DataPacketBuffer ^ source)
        {
            readDataCount       = source->readDataCount;
            firstTriggerCount   = source->firstTriggerCount;
            triggerCounterValue = source->triggerCounterValue;
            triggerCounterUsed  = source->triggerCounterUsed;

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axisHistory[i]->Copy(source->axisHistory[i]);
            }
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxisHistory[i]->Copy(source->auxAxisHistory[i]);
            }

            entry[destIndex]->Copy(source->entry[0]);
        }

        /// <summary>
        /// Gets / sets the read data counter value.
        /// </summary>
        property UInt32 ReadDataCount
        {
            UInt32 get() { return readDataCount; }
            void set(UInt32 value) { readDataCount = value; }
        }

        /// <summary>
        /// Gets / sets the first trigger counter value.
        /// </summary>
        property UInt32 FirstTriggerCount
        {
            UInt32 get() { return firstTriggerCount; }
            void set(UInt32 value)
            {
                firstTriggerCount  = value;
                triggerCounterUsed = true;
            }
        }

        /// <summary>
        /// Gets / sets the trigger counter value.
        /// </summary>
        property UInt32 TriggerCounterValue
        {
            UInt32 get() { return triggerCounterValue; }
            void set(UInt32 value)
            {
                triggerCounterValue = value;
                triggerCounterUsed  = true;
            }
        }

        /// <summary>
        /// Gets the trigger counter GUI value.
        /// </summary>
        property UInt32 TriggerCounterGui
        {
            UInt32 get() { return triggerCounterValue - firstTriggerCount + 1; }
        }

        /// <summary>
        /// Gets / sets the trigger counter used flag.
        /// </summary>
        property bool TriggerCounterUsed
        {
            bool get() { return triggerCounterUsed; }
            void set(bool value) { triggerCounterUsed = value; }
        }

        /// <summary>
        /// Gets the data packet entry data.
        /// </summary>
        property DataPacketEntry ^ Entry[UInt32]
        {
            DataPacketEntry ^ get(UInt32 index) { return entry[index]; }
        }

        /// <summary>
        /// Gets / sets the current used read buffer for write.
        /// </summary>
        property Int32 WriteIndex
        {
            Int32 get() { return writeIndex; }
            void set(Int32 value) { writeIndex = value; }
        }
    };
	#pragma endregion

 	#pragma region "AxisWarningFlags class definition"
    /// <summary>
    /// This class contains all measurement axis warning flags.
    /// </summary>
    public ref class AxisWarningFlags
    {
    private:
        bool    warningBadPositionParam;        ///< bad position parameter warning flag
        bool    warningRefPos1Disable;          ///< ref. pos. 1 disabled warning flag
        bool    warningRefPosCDisable;          ///< dist. coded ref. pos. disabled warning flag
        bool    warningRefPosEnDat01Invalid;    ///< ref. pos. EnDat 01 invalid warning flag
        bool    warningAI1Pos2Disable;          ///< EnDat add info 1 pos. 2 disabled warning flag

        bool    enDatAI1Pos2Set;                ///< EnDat add info 1 pos. 2 set flag

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AxisWarningFlags()
        {
            Clear();
        }

        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            warningBadPositionParam     = false;
            warningRefPos1Disable       = false;
            warningRefPosCDisable       = false;
            warningRefPosEnDat01Invalid = false;
            warningAI1Pos2Disable       = false;
        }

        /// <summary>
        /// Gets / sets the bad position parameter warning flag.
        /// </summary>
        property bool WarningBadPositionParam
        {
            bool get() { return warningBadPositionParam; }
            void set(bool value) { warningBadPositionParam = value; }
        }

        /// <summary>
        /// Gets / sets the ref. pos. 1 disabled warning flag.
        /// </summary>
        property bool WarningRefPos1Disable
        {
            bool get() { return warningRefPos1Disable; }
            void set(bool value) { warningRefPos1Disable = value; }
        }

        /// <summary>
        /// Gets / sets the dist. coded ref. pos. disabled warning flag.
        /// </summary>
        property bool WarningRefPosCDisable
        {
            bool get() { return warningRefPosCDisable; }
            void set(bool value) { warningRefPosCDisable = value; }
        }

        /// <summary>
        /// Gets / sets the ref. pos. EnDat 01 invalid warning flag.
        /// </summary>
        property bool WarningRefPosEnDat01Invalid
        {
            bool get() { return warningRefPosEnDat01Invalid; }
            void set(bool value) { warningRefPosEnDat01Invalid = value; }
        }

        /// <summary>
        /// Gets / sets the EnDat add info 1 pos. 2 disabled warning flag.
        /// </summary>
        property bool WarningAI1Pos2Disable
        {
            bool get() { return warningAI1Pos2Disable; }
            void set(bool value) { warningAI1Pos2Disable = value; }
        }

        /// <summary>
        /// Gets / sets EnDat add info 1 pos. 2 set flag.
        /// </summary>
        property bool EnDatAI1Pos2Set
        {
            bool get() { return enDatAI1Pos2Set; }
            void set(bool value) { enDatAI1Pos2Set = value; }
        }
    };
	#pragma endregion

 	#pragma region "WarningFlags class definition"
    // axis warning flags list
    typedef array<AxisWarningFlags ^>   AxisWarnings;

    /// <summary>
    /// This class contains all measurement warning flags.
    /// </summary>
    public ref class WarningFlags
    {
    private:
        bool    warningLostTriggerCount;    ///< lost trigger count warning flag
        bool    warningEnDat21Incremental;  ///< EnDat 21 incremental encoder not supported flag

        AxisWarnings ^      axis;           ///< axis warning flags
        AxisWarnings ^      auxAxis;        ///< auxiliary axis warning flags

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        WarningFlags()
        {
            axis = gcnew AxisWarnings(EIB_MAX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axis[i] = gcnew AxisWarningFlags;
            }
            auxAxis = gcnew AxisWarnings(EIB_MAX_AUX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxis[i] = gcnew AxisWarningFlags;
            }

            Clear();
        }

        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            warningLostTriggerCount   = false;
            warningEnDat21Incremental = false;

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                axis[i]->Clear();
            }
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                auxAxis[i]->Clear();
            }
        }

        /// <summary>
        /// Gets / sets the lost trigger count warning flag.
        /// </summary>
        property bool WarningLostTriggerCount
        {
            bool get() { return warningLostTriggerCount; }
            void set(bool value) { warningLostTriggerCount = value; }
        }

        /// <summary>
        /// Gets / sets the EnDat 21 incremental encoder not supported warning flag.
        /// </summary>
        property bool WarningEnDat21Incremental
        {
            bool get() { return warningEnDat21Incremental; }
            void set(bool value) { warningEnDat21Incremental = value; }
        }

        /// <summary>
        /// Gets the axis warning flags.
        /// </summary>
        property AxisWarningFlags ^ Axis[UInt32]
        {
            AxisWarningFlags ^ get(UInt32 index) { return axis[index]; }
        }

        /// <summary>
        /// Gets the auxiliary axis warning flags.
        /// </summary>
        property AxisWarningFlags ^ AuxAxis[UInt32]
        {
            AxisWarningFlags ^ get(UInt32 index) { return auxAxis[index]; }
        }
    };
	#pragma endregion
}
