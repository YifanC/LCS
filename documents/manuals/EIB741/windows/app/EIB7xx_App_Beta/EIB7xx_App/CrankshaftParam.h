///
/// \file       CrankshaftParam.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: CrankshaftParam.h,v 1.3 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: CrankshaftParam.h,v $
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
/// \brief      Crankshaft application classes.
///
/// This classes are used for the management of the parameters
/// used for the crankshaft application.
///

#pragma once

#include "FormMain.h"

using namespace System;

namespace EIB7xx_App
{
 	#pragma region "EibAxis class definition"
    // default parameters
    #define CRANKSHAFT_NOT_USED_EIB_INDEX       UInt32(  0)
    #define CRANKSHAFT_NOT_USED_AXIS_INDEX      UInt32(  0)

    /// EIB / axis selection class
    public ref class EibAxis
    {
    private:
        UInt32  fileVersion;    ///< parameter version for parameter file

        UInt32  eibIndex;       ///< the EIB index
        UInt32  axisIndex;      ///< the axis index

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EibAxis()
        {
            fileVersion = 1;

            eibIndex  = CRANKSHAFT_NOT_USED_EIB_INDEX;
            axisIndex = CRANKSHAFT_NOT_USED_AXIS_INDEX;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        EibAxis(UInt32 eibIndex, UInt32 axisIndex)
        {
            fileVersion = 1;

            this->eibIndex  = eibIndex;
            this->axisIndex = axisIndex;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EibAxis ^ source)
        {
            eibIndex  = source->eibIndex;
            axisIndex = source->axisIndex;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EibAxis ^ source)
        {
            if (eibIndex  != source->eibIndex)  return false;
            if (axisIndex != source->axisIndex) return false;
            return true;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsNotUsed()
        {
            if (eibIndex  != CRANKSHAFT_NOT_USED_EIB_INDEX)  return false;
            if (axisIndex != CRANKSHAFT_NOT_USED_AXIS_INDEX) return false;
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
            paramFile->ReadWriteUInt32(eibIndex);
            paramFile->ReadWriteUInt32(axisIndex);
        }

        /// <summary>
        /// Gets / sets the EIB index.
        /// </summary>
        property UInt32 EibIndex
        {
            UInt32 get() { return eibIndex; }
            void set(UInt32 value) { eibIndex = value; }
        }

        /// <summary>
        /// Gets / sets the axis index.
        /// </summary>
        property UInt32 AxisIndex
        {
            UInt32 get() { return axisIndex; }
            void set(UInt32 value) { axisIndex = value; }
        }
    };
 	#pragma endregion

 	#pragma region "Crankshaft_Param class definition"
    // default parameters
    #define CRANKSHAFT_MEASURE_POINT_NUM        UInt32(   4)
    #define CRANKSHAFT_CHANGE_ANGLE_DISTANCE    Double(  10.0)
    #define CRANKSHAFT_DEFAULT_ANGLE_DISTANCE   Double(   1.0)
    #define CRANKSHAFT_MIN_ANGLE_DISTANCE       Double(   0.1)
    #define CRANKSHAFT_MAX_ANGLE_DISTANCE       Double(  10.0)
    #define CRANKSHAFT_STEP_ANGLE_DISTANCE      Double(   0.1)
    #define CRANKSHAFT_FORMAT_ANGLE_DISTANCE    L"F1"
    #define CRANKSHAFT_DEFAULT_RADIUS_OFFSET    Double(   0.0)
    #define CRANKSHAFT_MIN_RADIUS_OFFSET        Double(-900.0)
    #define CRANKSHAFT_MAX_RADIUS_OFFSET        Double( 900.0)
    #define CRANKSHAFT_STEP_RADIUS_OFFSET       Double(   0.1)
    #define CRANKSHAFT_FORMAT_RADIUS_OFFSET     L"F1"
    #define CRANKSHAFT_DEFAULT_AXIS_MAX         UInt32(  70)
    #define CRANKSHAFT_MIN_AXIS_MAX             UInt32(  10)
    #define CRANKSHAFT_MAX_AXIS_MAX             UInt32( 900)
    #define CRANKSHAFT_STEP_AXIS_MAX            UInt32(   1)
    #define CRANKSHAFT_DEFAULT_TOLERANCE        Double(   100.000)
    #define CRANKSHAFT_MIN_TOLERANCE            Double(     0.001)
    #define CRANKSHAFT_MAX_TOLERANCE            Double( 10000.000)
    #define CRANKSHAFT_STEP_TOLERANCE           Double(     0.001)
    #define CRANKSHAFT_FORMAT_TOLERANCE         L"F3"

    /// EIB / axis selection list
    typedef array<EibAxis ^>  EibAxisList;

    /// <summary>
    /// This class contains the crankshaft application parameters.
    /// </summary>
    public ref class Crankshaft_Param
    {
    private:
        UInt32          fileVersion;    ///< parameter version for parameter file

        EibAxisList ^   measurePoints;  ///< EIB / axis measure point selection
        EibAxis         angelMeasure;   ///< EIB / axis angel measure selection

        Double          angleDistance;  ///< min angle distance in °
        Double          radiusOffset;   ///< radius offset in mm
        UInt32          graphAxisMax;   ///< max value for graph axis in mmm
        Double          tolerance;      ///< tolerance value to reference curve in µm

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        Crankshaft_Param()
        {
            fileVersion = 2;

            measurePoints = gcnew EibAxisList(CRANKSHAFT_MEASURE_POINT_NUM);
            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                measurePoints[i] = gcnew EibAxis;
            }

            angleDistance = CRANKSHAFT_DEFAULT_ANGLE_DISTANCE;
            radiusOffset  = CRANKSHAFT_DEFAULT_RADIUS_OFFSET;
            graphAxisMax  = CRANKSHAFT_DEFAULT_AXIS_MAX;
            tolerance     = CRANKSHAFT_DEFAULT_TOLERANCE;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(Crankshaft_Param ^ source)
        {
            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                measurePoints[i]->Copy(source->measurePoints[i]);
            }
            angelMeasure.Copy(%source->angelMeasure);

            angleDistance = source->angleDistance;
            radiusOffset  = source->radiusOffset;
            graphAxisMax  = source->graphAxisMax;
            tolerance     = source->tolerance;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
		/// <param name="source">The source parameters.</param>
		/// <param name="checkNoEibParam">If true check parameters not send to EIB.</param>
        bool IsEqual(Crankshaft_Param ^ source, bool checkNoEibParam)
        {
            if (checkNoEibParam == true)
            {
                for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
                {
                    if (measurePoints[i]->IsEqual(source->measurePoints[i]) == false) return false;
                }
                if (angelMeasure.IsEqual(%source->angelMeasure) == false) return false;

                if (angleDistance != source->angleDistance) return false;
                if (radiusOffset  != source->radiusOffset)  return false;
                if (graphAxisMax  != source->graphAxisMax)  return false;
                if (tolerance     != source->tolerance)     return false;
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
            UInt32 paramVersion = 0;
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, 1, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                measurePoints[i]->ReadWriteFile(paramFile);
            }
            angelMeasure.ReadWriteFile(paramFile);

            paramFile->ReadWriteDouble(angleDistance);
            // check special versions
            if (   (paramFile->WriteFile == true)
                || (paramVersion > 1))
            {
                paramFile->ReadWriteDouble(radiusOffset);
            }
            paramFile->ReadWriteUInt32(graphAxisMax);
            paramFile->ReadWriteDouble(tolerance);
        }

        /// <summary>
        /// Gets the EIB / axis measure point selection.
        /// </summary>
        property EibAxis ^ MeasurePoint[UInt32]
        {
            EibAxis ^ get(UInt32 index) { return measurePoints[index]; }
        }

        /// <summary>
        /// Gets the EIB / axis angel measure selection.
        /// </summary>
        property EibAxis ^ AngelMeasure
        {
            EibAxis ^ get() { return %angelMeasure; }
        }

        /// <summary>
        /// Gets / sets the min angle distance in °.
        /// </summary>
        property Double AngleDistance
        {
            Double get() { return angleDistance; }
            void set(Double value) { angleDistance = value; }
        }

        /// <summary>
        /// Gets / sets the radius offset in mm.
        /// </summary>
        property Double RadiusOffset
        {
            Double get() { return radiusOffset; }
            void set(Double value) { radiusOffset = value; }
        }

        /// <summary>
        /// Gets / sets the max value for graph axis in mmm.
        /// </summary>
        property UInt32 GraphAxisMax
        {
            UInt32 get() { return graphAxisMax; }
            void set(UInt32 value) { graphAxisMax = value; }
        }

        /// <summary>
        /// Gets / sets the tolerance value to reference curve in µm.
        /// </summary>
        property Double Tolerance
        {
            Double get() { return tolerance; }
            void set(Double value) { tolerance = value; }
        }
    };
    #pragma endregion

 	#pragma region "CrankshaftEntry class definition"
    // radius measure value list
    typedef array<PositionData ^>   RadiusList;

    /// <summary>
    /// This class contains the crankshaft measure values.
    /// </summary>
    public ref class CrankshaftEntry
    {
    private:
        UInt32              index;      ///< measure value index
        PositionData        angel;      ///< angel measure value
        UInt32              count360;   ///< measure number of last 360°
        RadiusList ^        radius;     ///< radius measure value list
        UInt32              setFlag;    ///< radius value set flag

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        CrankshaftEntry()
        {
            radius = gcnew RadiusList(CRANKSHAFT_MEASURE_POINT_NUM);
            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                radius[i] = gcnew PositionData;
            }

            Clear();
        }

    protected:
		/// <summary>
		/// Destructor.
		/// </summary>
		~CrankshaftEntry()
		{
            Delete();
        }

		/// <summary>
		/// Finalizer.
		/// </summary>
		!CrankshaftEntry()
		{
            Delete();
		}

    private:
        /// <summary>
        /// Delete function.
        /// </summary>
        void Delete()
        {
            if (radius != nullptr)
            {
                for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
                {
                    delete radius[i];
                }
                delete radius;
                radius = nullptr;
            }
        }

    public:
        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            angel.Clear();

            count360 = 0;

            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                radius[i]->Clear();
            }

            setFlag = 0;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(CrankshaftEntry ^ source)
        {
            index = source->index;

            angel.Copy(%source->angel);

            count360 = source->count360;

            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                radius[i]->Copy(source->radius[i]);
            }

            setFlag = source->setFlag;
        }

        /// <summary>
        /// Gets / sets the crankshaft measure value index.
        /// </summary>
        property UInt32 Index
        {
            UInt32 get() { return index; }
            void set(UInt32 value) { index = value; }
        }

        /// <summary>
        /// Gets the crankshaft angel measure value.
        /// </summary>
        property PositionData ^ Angel
        {
            PositionData ^ get() { return %angel; }
        }

        /// <summary>
        /// Gets / sets the crankshaft measure number of last 360°.
        /// </summary>
        property UInt32 Count360
        {
            UInt32 get() { return count360; }
            void set(UInt32 value) { count360 = value; }
        }

        /// <summary>
        /// Gets the crankshaft radius measure value.
        /// </summary>
        property PositionData ^ Radius[UInt32]
        {
            PositionData ^ get(UInt32 index) { return radius[index]; }
        }

        /// <summary>
        /// Gets / sets the crankshaft radius value set flag.
        /// </summary>
        property UInt32 SetFlag
        {
            UInt32 get() { return setFlag; }
            void set(UInt32 value) { setFlag = value; }
        }
    };
	#pragma endregion

 	#pragma region "CrankshaftBuffer class definition"
    // crankshaft measure value list
    typedef array<CrankshaftEntry ^>    EntryList;
    // current radius buffer index list
    typedef array<Int32>                RadiusIndexList;

    /// <summary>
    /// This class contains the crankshaft measure values.
    /// </summary>
    public ref class CrankshaftBuffer
    {
    private:
        UInt32              measureNum;     ///< set number of measurements
        UInt32              bufferSize;     ///< measure value buffer size
        EntryList ^         entries;        ///< measure value buffer
        UInt32              angelCount;     ///< number of measured angel values
        UInt32              radiusCount;    ///< number of measured radius values
        Int32               angelIndex;     ///< current angel buffer index
        RadiusIndexList ^   radiusIndex;    ///< current radius buffer index
        Int32               displayIndex;   ///< current display buffer index
        Double              angelDistance;  ///< angel distance for 360° calculation
        bool                angelForward;   ///< angel direction flag
        UInt32              usedFlag;       ///< radius value used flag

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="measureNum">The set number of measurements.</param>
        CrankshaftBuffer(UInt32 measureNum)
        {
            bufferSize = measureNum;

            entries = gcnew EntryList(bufferSize);
            for (UInt32 i = 0; i < bufferSize; i ++)
            {
                entries[i] = gcnew CrankshaftEntry;
            }

            radiusIndex = gcnew RadiusIndexList(CRANKSHAFT_MEASURE_POINT_NUM);

            Clear(measureNum, true);
        }

    protected:
		/// <summary>
		/// Destructor.
		/// </summary>
		~CrankshaftBuffer()
		{
            // close EIB entry
            Delete();
        }

		/// <summary>
		/// Finalizer.
		/// </summary>
		!CrankshaftBuffer()
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
            if (entries != nullptr)
            {
                for (UInt32 i = 0; i < bufferSize; i ++)
                {
                    delete entries[i];
                }
                delete entries;
                entries = nullptr;
            }
        }

    public:
        /// <summary>
        /// Clear function.
        /// </summary>
		/// <param name="measureNum">The set number of measurements.</param>
		/// <param name="clearAll">If true clear all entries.</param>
        void Clear(UInt32 measureNum, bool clearAll)
        {
            this->measureNum = measureNum;

            angelCount   = 0;
            radiusCount  = 0;
            angelIndex   = 0;
            displayIndex = -1;

            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                radiusIndex[i] = 0;
            }

            angelDistance = 0.0;
            angelForward  = true;

            if (clearAll == true)
            {
                for (UInt32 i = 0; i < bufferSize; i ++)
                {
                    entries[i]->Clear();
                }

                usedFlag = 0;
            }
        }

        /// <summary>
        /// Gets the crankshaft set number of measurements.
        /// </summary>
        property UInt32 MeasureNum
        {
            UInt32 get() { return measureNum; }
        }

        /// <summary>
        /// Gets the crankshaft measure value buffer size.
        /// </summary>
        property UInt32 BufferSize
        {
            UInt32 get() { return bufferSize; }
        }

        /// <summary>
        /// Gets the crankshaft measure value.
        /// </summary>
        property CrankshaftEntry ^ Entry[UInt32]
        {
            CrankshaftEntry ^ get(UInt32 index) { return entries[index]; }
        }

        /// <summary>
        /// Gets / sets the number of measured angel values.
        /// </summary>
        property UInt32 AngelCount
        {
            UInt32 get() { return angelCount; }
            void set(UInt32 value) { angelCount = value; }
        }

        /// <summary>
        /// Gets / sets the number of measured radius values.
        /// </summary>
        property UInt32 RadiusCount
        {
            UInt32 get() { return radiusCount; }
            void set(UInt32 value) { radiusCount = value; }
        }

        /// <summary>
        /// Gets / sets the current angel buffer index.
        /// </summary>
        property Int32 AngelIndex
        {
            Int32 get() { return angelIndex; }
            void set(Int32 value) { angelIndex = value; }
        }

        /// <summary>
        /// Gets / sets the current radius buffer index.
        /// </summary>
        property Int32 RadiusIndex[UInt32]
        {
            Int32 get(UInt32 index) { return radiusIndex[index]; }
            void set(UInt32 index, Int32 value) { radiusIndex[index] = value; }
        }

        /// <summary>
        /// Gets / sets the current display buffer index.
        /// </summary>
        property Int32 DisplayIndex
        {
            Int32 get() { return displayIndex; }
            void set(Int32 value) { displayIndex = value; }
        }

        /// <summary>
        /// Gets / sets the crankshaft angel distance for 360° calculation.
        /// </summary>
        property Double AngelDistance
        {
            Double get() { return angelDistance; }
            void set(Double value) { angelDistance = value; }
        }

        /// <summary>
        /// Gets / sets the crankshaft angel direction flag.
        /// </summary>
        property bool AngelForward
        {
            bool get() { return angelForward; }
            void set(bool value) { angelForward = value; }
        }

        /// <summary>
        /// Gets / sets the crankshaft radius value used flag.
        /// </summary>
        property UInt32 UsedFlag
        {
            UInt32 get() { return usedFlag; }
            void set(UInt32 value) { usedFlag = value; }
        }
    };
	#pragma endregion
}
