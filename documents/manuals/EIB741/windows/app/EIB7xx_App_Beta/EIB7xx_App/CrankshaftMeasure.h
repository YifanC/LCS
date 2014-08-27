///
/// \file       CrankshaftMeasure.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: CrankshaftMeasure.h,v 1.3 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: CrankshaftMeasure.h,v $
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
/// This classes are used for the management of the operations
/// used for the crankshaft application.
///

#pragma once

#include "FormMain.h"

using namespace System;

namespace EIB7xx_App
{
    #pragma region "Crankshaft_Measure class definition"
    // measure point / axis index list
    typedef array<UInt32>       PointIndexList;

    /// <summary>
    /// This class contains the crankshaft application operations.
    /// </summary>
    public ref class Crankshaft_Measure
    {
    /// measure point index / axis index list
    typedef Dictionary<UInt32, UInt32>              PointAxisList;
    /// eib index / measure point / axis list
    typedef Dictionary<UInt32, PointAxisList ^>     EibPointList;

    /// <summary>
    /// This enum lists plot display modes.
    /// </summary>
    enum class DisplayModes : UInt32
    {
        DirectRefresh = 1,
        DirectPlot    = 2,
        PlotPoint     = 3,
        PlotCurve     = 4
    };

    private:
        GUI_Support ^           guiSupport;     ///< the GUI support class
        ResourceManager ^       resManager;     ///< the resource manager from main window

        Crankshaft_Param ^      param;          ///< the crankshaft parameters

        Crankshaft_Display ^    display;        ///< the crankshaft display

        CrankshaftBuffer ^      readBuffer;     ///< measure value read buffer
        CrankshaftBuffer ^      displayBuffer;  ///< measure value display buffer
        CrankshaftEntry         copyBuffer;     ///< measure value copy buffer

        Mutex ^                 measureMutex;   ///< mutex for measure threads
        Mutex ^                 displayMutex;   ///< mutex for display threads

        UInt32                  measureNum;     ///< number of measurements
        bool                    angelStop;      ///< angel stop flag
        bool                    abortFlag;      ///< the measurement abort flag
        bool                    radiusNotSet;   ///< the radius value not set flag
        UInt32                  angelCount360;  ///< angel count for 360°
        bool                    over360;        ///< flag for 360°
        Int32                   displayIndex;   ///< old display index value
        Double                  angleDistance;  ///< angle distance in °
        
        DisplayModes            displayMode;    ///< display mode during measurement

        EibPointList ^          eibPointList;   ///< eib index / measure point / axis list

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="guiSupport">The GUI support class.</param>
        /// <param name="param">The crankshaft parameters.</param>
        /// <param name="display">The crankshaft display.</param>
        Crankshaft_Measure(GUI_Support ^        guiSupport,
                           Crankshaft_Param ^   param,
                           Crankshaft_Display ^ display)
        {
            this->guiSupport = guiSupport;
            this->resManager = guiSupport->ResManager;

            this->param = param;

            this->display = display;

            readBuffer    = nullptr;
            displayBuffer = nullptr;

            measureMutex = gcnew Mutex;
            displayMutex = gcnew Mutex;

            eibPointList = gcnew EibPointList;

            displayMode = DisplayModes::DirectPlot;
        }

        /// <summary>
        /// Start crankshaft measurement.
        /// </summary>
        /// <param name="opMode">The EIB operation mode parameters.</param>
        void StartMeasurement(EIB_OpMode_Param ^ opMode)
        {
            // check for crankshaft application
            if (guiSupport->GuiConfig->GuiParam->AppCrankshaft == false)
            {
                return;
            }
            // initialize parameters
            abortFlag = false;
            // check configuration parameters
            bool measurePointNotUsed = true;
            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                if (param->MeasurePoint[i]->IsNotUsed() == false)
                {
                    measurePointNotUsed = false;
                    break;
                }
            }
            if (param->AngelMeasure->IsNotUsed() != measurePointNotUsed)
            {
                // no usefull configuration parameters
                guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                            resManager->GetString(L"warningCrankshaftConfig"));
                abortFlag = true;
                return;
            }
            // check operation mode parameters
            if (   (param->AngelMeasure->IsNotUsed() == false)
                && (opMode->OpMode != EIB7_MODE_OPERATING::EIB7_OM_SoftRealtime)
                && (opMode->OpMode != EIB7_MODE_OPERATING::EIB7_OM_Streaming)
                && (opMode->OpMode != EIB7_MODE_OPERATING::EIB7_OM_Polling))
            {
                // bad operation mode is set
                OpModeConverter ^ opModeConv = gcnew OpModeConverter;
                String ^ modes =
                      opModeConv->ConvertTo(EIB7_MODE_OPERATING::EIB7_OM_SoftRealtime, String::typeid)
                    + L", "
                    + opModeConv->ConvertTo(EIB7_MODE_OPERATING::EIB7_OM_Streaming, String::typeid)
                    + L", "
                    + opModeConv->ConvertTo(EIB7_MODE_OPERATING::EIB7_OM_Polling, String::typeid);
                String ^ message =
                    String::Format(resManager->GetString(L"warningCrankshaftOpMode"), modes);
                guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                            message);
                abortFlag = true;
                return;
            }
            // initialize parameters
            angleDistance = param->AngleDistance;
            radiusNotSet  = false;
            displayIndex  = -1;
            // clear the data plots
            display->ClearDisplay();
            // disable buttons
            display->EnableButtons(false, false);
            // check for crankshaft measurement
            if (param->AngelMeasure->IsNotUsed() == true)
            {
                // no crankshaft measurement
                abortFlag = true;
                return;
            }
            // initialize measure value buffer
            measureNum = UInt32(Math::Ceiling(360.0 / angleDistance));
            //Debug::Print(String::Format(L"Crankshaft measure number: {0}", measureNum));
            if (   (readBuffer == nullptr)
                || (readBuffer->BufferSize < measureNum))
            {
                if (readBuffer != nullptr)
                {
                    delete readBuffer;
                }
                readBuffer    = gcnew CrankshaftBuffer(measureNum);
                if (displayBuffer != nullptr)
                {
                    delete displayBuffer;
                }
                if (   (displayMode == DisplayModes::PlotPoint)
                    || (displayMode == DisplayModes::PlotCurve))
                {
                    displayBuffer = gcnew CrankshaftBuffer(measureNum);
                }
            }
            // initialize parameters
            InitMeasureParam(true);
            // initialize parameters
            eibPointList->Clear();
            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                if (param->MeasurePoint[i]->IsNotUsed() == false)
                {
                    // set entry used flag
                    readBuffer->UsedFlag |= 0x01 << i;
                    // set eib, point and axis index lists
                    UInt32 eibIndex  = param->MeasurePoint[i]->EibIndex  - 1;
                    UInt32 axisIndex = param->MeasurePoint[i]->AxisIndex - 1;
                    if (eibPointList->ContainsKey(eibIndex) == false)
                    {
                        PointAxisList ^ pointAxisList = gcnew PointAxisList;
                        pointAxisList[i] = axisIndex;
                        eibPointList[eibIndex] = pointAxisList;
                    }
                    else
                    {
                        eibPointList[eibIndex][i] = axisIndex;
                    }
                }
            }
            if (displayBuffer != nullptr)
            {
                displayBuffer->UsedFlag = readBuffer->UsedFlag;
            }
            // initialize plots for measurement display
            if (   (displayMode == DisplayModes::DirectRefresh)
                || (displayMode == DisplayModes::DirectPlot))
            {
                display->AddPlot(false, readBuffer->UsedFlag, measureNum,
                                 false, PlotStyles::Line,
                                 RefStyles::Point, angleDistance);
            }
            else if (displayMode == DisplayModes::PlotPoint)
            {
                display->AddPlot(false, readBuffer->UsedFlag, measureNum,
                                 false, PlotStyles::Point,
                                 RefStyles::Point, angleDistance);
            }
        }

        /// <summary>
        /// Initialize crankshaft measure parameters.
        /// </summary>
		/// <param name="clearAll">If true clear all entries.</param>
        void InitMeasureParam(bool clearAll)
        {
            angelStop     = false;
            angelCount360 = 0;
            over360       = false;

            readBuffer->Clear(measureNum, clearAll);
            if (displayBuffer != nullptr)
            {
                displayBuffer->Clear(measureNum, clearAll);
            }
        }

        /// <summary>
        /// Read crankshaft measure data.
        /// </summary>
        /// <param name="eibIndex">The EIB index.</param>
        /// <param name="dataPacket">The data packet read buffer.</param>
        /// <returns">True if stop conditions are met.</returns>
        bool ReadMeasureData(UInt32 eibIndex, DataPacketBuffer ^ dataPacket)
        {
            bool stopFlag = false;
            // check for crankshaft application or abort flag
            if (   (guiSupport->GuiConfig->GuiParam->AppCrankshaft == false)
                || (abortFlag == true))
            {
                return stopFlag;
            }
            // read angel measure data
            stopFlag = ReadAngelData(eibIndex, dataPacket);
            if (stopFlag == true)
            {
                return stopFlag;
            }
            // read radius measure data
            stopFlag = ReadRadiusData(eibIndex, dataPacket);
            return stopFlag;
        }

        /// <summary>
        /// Read crankshaft angel measure data.
        /// </summary>
        /// <param name="eibIndex">The EIB index.</param>
        /// <param name="dataPacket">The data packet read buffer.</param>
        /// <returns">True if stop conditions are met.</returns>
        bool ReadAngelData(UInt32 eibIndex, DataPacketBuffer ^ dataPacket)
        {
            bool stopFlag = false;
            // check for angel measurement
            if (   (param->AngelMeasure->EibIndex != eibIndex + 1)
                || (abortFlag == true)
                || (angelStop == true))
            {
                return stopFlag;
            }
            // angel measurement
            UInt32 axisIndex = param->AngelMeasure->AxisIndex - 1;
            // set measure mutex
            measureMutex->WaitOne();
            try
            {
                // check new angel
                PositionData ^ newAngel =
                    dataPacket->Entry[dataPacket->WriteIndex]->Axis[axisIndex]->PositionValues;
                if (   (newAngel->Valid == false)
                    || (newAngel->Unit != L"°"))
                {
                    // no valid angel value
                    guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                resManager->GetString(L"warningCrankshaftAngel"));
                    abortFlag = true;
                    return stopFlag;
                }
                // get last angel
                Int32 lastIndex = readBuffer->AngelIndex - 1;
                if (lastIndex < 0)
                {
                    lastIndex += readBuffer->MeasureNum;
                }
                PositionData ^ lastAngel = readBuffer->Entry[lastIndex]->Angel;
                // check for first angel
                if (readBuffer->AngelCount == 0)
                {
                    // save angel measure data to read buffer
                    SaveAngelData(eibIndex, readBuffer, dataPacket, lastIndex, lastAngel, newAngel);
                    return stopFlag;
                }
                // check angel distance
                bool overDelta = CheckOverDelta(lastAngel->Position, newAngel->Position,
                                                angleDistance);
                if (overDelta == false)
                {
                    // angel distance is to small
                    return stopFlag;
                }
                // check angel direction
                bool forward = CheckForward(lastAngel->Position, newAngel->Position);
                // check for second angel
                if (readBuffer->AngelCount == 1)
                {
                    // save direction flag
                    readBuffer->AngelForward = forward;
                    // save angel measure data to read buffer
                    SaveAngelData(eibIndex, readBuffer, dataPacket, lastIndex, lastAngel, newAngel);
                    return stopFlag;
                }
                // check if direction changed
                if (readBuffer->AngelForward != forward)
                {
                    // direction changed
                    // check angel distance for direction changing
                    overDelta = CheckOverDelta(lastAngel->Position, newAngel->Position,
                                               CRANKSHAFT_CHANGE_ANGLE_DISTANCE);
                    if (overDelta == false)
                    {
                        // angel distance is to small
                        return stopFlag;
                    }
                    //Debug::Print(String::Format(L"Angel direction changed"));
                    // initialize parameters
                    InitMeasureParam(false);
                    // initialize plots
                    display->InitPlot(true);
                    // save direction flag
                    readBuffer->AngelForward = forward;
                    // save angel measure data to read buffer
                    SaveAngelData(eibIndex, readBuffer, dataPacket, lastIndex, lastAngel, newAngel);
                    return stopFlag;
                }
                // get first angel
                PositionData ^ firstAngel = readBuffer->Entry[0]->Angel;
                // get current radius count
                UInt32 radiusCount = readBuffer->RadiusCount;
                // check new angel over 360°
                bool angelOver360 = CheckOver360(firstAngel->Position, lastAngel->Position,
                                                 newAngel->Position, readBuffer->AngelForward);
                if (   (angelOver360 == true)
                    && (angelCount360 == 0))
                {
                    angelCount360 = readBuffer->AngelCount;
                    if (   (over360 == false)
                        && (radiusCount >= angelCount360))
                    {
                        over360 = true;
                        //Debug::Print(String::Format(L"Over 360° angel ok: {0}", angelCount360));
                    }
                }
                // check stop conditions
                angelStop = CheckForStop(readBuffer->AngelCount, angelOver360);
                if (   (angelStop == true)
                    && (radiusCount == readBuffer->AngelCount))
                {
                    stopFlag = true;
                    //Debug::Print(String::Format(L"Angel stop eib: {0}, count: {1}, index: {2}",
                    //                            eibIndex,
                    //                            readBuffer->AngelCount,
                    //                            readBuffer->AngelIndex));
                }
                // check new angel
                if (   (readBuffer->AngelCount > 0)
                    && (angelStop == true))
                {
                    return stopFlag;
                }
                //Debug::Print(String::Format(L"Crankshaft count: {0}, overDelta: {1}, stop: {2}",
                //                            readBuffer->AngelCount, overDelta, angelStop));
                // save angel measure data to read buffer
                SaveAngelData(eibIndex, readBuffer, dataPacket, lastIndex, lastAngel, newAngel);
                // check for synchronisation
                if (readBuffer->AngelCount > radiusCount + readBuffer->MeasureNum / 2)
                {
                    // EIB synchronisation problems
                    guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                resManager->GetString(L"warningCrankshaftSync"));
                    abortFlag = true;
                    return stopFlag;
                }
            }
            finally
            {
                // reset measure mutex
                measureMutex->ReleaseMutex();
            }
            return stopFlag;
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// Save crankshaft angel measure data to read buffer.
        /// </summary>
        /// <param name="eibIndex">The EIB index.</param>
        /// <param name="measureBuffer">The measure data buffer.</param>
        /// <param name="dataPacket">The data packet read buffer.</param>
        /// <param name="lastIndex">The last buffer index.</param>
        /// <param name="lastAngel">The last angle.</param>
        /// <param name="newAngel">The new angle.</param>
        void SaveAngelData(UInt32 eibIndex, 
                           CrankshaftBuffer ^ measureBuffer,
                           DataPacketBuffer ^ dataPacket,
                           Int32 lastIndex,
                           PositionData ^ lastAngel, PositionData ^ newAngel)
        {
            // save index and angel, reset set flag
            Int32 newIndex = measureBuffer->AngelIndex;
            measureBuffer->Entry[newIndex]->Index = dataPacket->ReadDataCount - 1;
            measureBuffer->Entry[newIndex]->Angel->Copy(newAngel);
            measureBuffer->Entry[newIndex]->SetFlag = 0;
            // calculate measure number for last 360°
            CalculateCount360(measureBuffer, lastIndex, newIndex,
                              lastAngel->Position, newAngel->Position);
            // increment counter
            measureBuffer->AngelCount ++;
            // increment index
            measureBuffer->AngelIndex ++;
            if (measureBuffer->AngelIndex >= Int32(measureBuffer->MeasureNum))
            {
                measureBuffer->AngelIndex -= measureBuffer->MeasureNum;
            }
            //Debug::Print(String::Format(
            //                L"Eib: {0}, count: {1}, index: {2}, index: {3}, angel: {4:F3}°",
            //                eibIndex,
            //                readBuffer->AngelCount,
            //                newIndex,
            //                readBuffer->Entry[newIndex]->Index,
            //                readBuffer->Entry[newIndex]->Angel->Position));
        }
        #pragma warning(pop)

        /// <summary>
        /// Read crankshaft radius measure data.
        /// </summary>
        /// <param name="eibIndex">The EIB index.</param>
        /// <param name="dataPacket">The data packet read buffer.</param>
        /// <returns">True if stop conditions are met.</returns>
        bool ReadRadiusData(UInt32 eibIndex, DataPacketBuffer ^ dataPacket)
        {
            bool stopFlag = false;
            // check for radius measurement
            if (   (abortFlag == true)
                || (eibPointList->ContainsKey(eibIndex) == false))
            {
                return stopFlag;
            }
            // radius measurement
            // get index of first measure point of this EIB
            UInt32 point0Index = 0;
            for each (point0Index in eibPointList[eibIndex]->Keys) break;
            // set measure mutex
            measureMutex->WaitOne();
            try
            {
                // loop for multiple new radius values
                for (UInt32 i = 0; i < EIB_READ_BUFFER_NUMBER; i ++)
                {
                    // get current angel index
                    Int32 angelIndex = readBuffer->AngelIndex;
                    // check for new radius value
                    // the radius index will be incremented during loop
                    Int32 radiusIndex = readBuffer->RadiusIndex[point0Index];
                    // get index of last read radius value in data packet buffer
                    UInt32 lastIndexValue = dataPacket->ReadDataCount - 1;
                    // get index of radius value witch is looked for
                    UInt32 waitIndexValue = readBuffer->Entry[radiusIndex]->Index;
                    if (   (radiusIndex == angelIndex)
                        || (lastIndexValue < waitIndexValue))
                    {
                        // no new radius value
                        return stopFlag;
                    }
                    if (i > 0)
                    {
                        //Debug::Print(String::Format(L"Radius loop count: {0}", i));
                    }
                    //Debug::Print(String::Format(L"Eib: {0}, idx angel: {1}, radius: {2}",
                    //                            eibIndex, angelIndex, radiusIndex));
                    // check for radius value in data packet buffer
                    Int32 dataPacketIndex = lastIndexValue - waitIndexValue;
                    Int32 dataPacketNum   = Math::Min(dataPacket->ReadDataCount, EIB_READ_BUFFER_NUMBER);
                    if (dataPacketIndex >= dataPacketNum)
                    {
                        // radius value not found
                        if (radiusNotSet == false)
                        {
                            //Debug::Print(String::Format(L"Radius value not found: last: {0}, wait: {1}",
                            //                            lastIndexValue, waitIndexValue));
                            guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                        resManager->GetString(L"warningCrankshaftRadius"));
                        }
                        radiusNotSet = true;
                    }
                    else
                    {
                        // radius value was found
                        Int32 readIndex = dataPacket->WriteIndex - dataPacketIndex;
                        if (readIndex < 0)
                        {
                            readIndex += EIB_READ_BUFFER_NUMBER;
                        }
                        PositionData ^ newRadius;
                        UInt32 axisIndex;
                        for each (UInt32 pointIndex in eibPointList[eibIndex]->Keys)
                        {
                            axisIndex = eibPointList[eibIndex][pointIndex];
                            // set new radius value
                            newRadius = dataPacket->Entry[readIndex]->Axis[axisIndex]->PositionValues;
                            newRadius->Position -= param->RadiusOffset;
                            readBuffer->Entry[radiusIndex]->Radius[pointIndex]->Copy(newRadius);
                            // check new radius value
                            if (newRadius->Valid == false)
                            {
                                //Debug::Print(String::Format(L"Radius value invalid"));
                                if (radiusNotSet == false)
                                {
                                    guiSupport->ShowWarningText(
                                                    resManager->GetString(L"infoEibMeasureWarning"),
                                                    resManager->GetString(L"warningCrankshaftRadius"));
                                }
                                radiusNotSet = true;
                            }
                        }
                    }
                    // set measure point radius set flag for all measure points / axis of this EIB
                    for each (UInt32 pointIndex in eibPointList[eibIndex]->Keys)
                    {
                        readBuffer->Entry[radiusIndex]->SetFlag |= 0x01 << pointIndex;
                    }
                    // check for last radius entry
                    bool lastRadius = (readBuffer->Entry[radiusIndex]->SetFlag == readBuffer->UsedFlag);
                    if (lastRadius == true)
                    {
                        // last radius entry
                        // increment number of measurements
                        readBuffer->RadiusCount ++;
                        // prepare display data
                        if (displayMode == DisplayModes::DirectRefresh)
                        {
                            // add new point to plot
                            display->AddPlotPoint(readBuffer->Entry[radiusIndex], true);
                            readBuffer->DisplayIndex = radiusIndex;
                        }
                        else if (displayMode == DisplayModes::DirectPlot)
                        {
                            //Debug::Print(String::Format(L"Add plot point: {0}, angel: {1:F3}°",
                            //                            radiusIndex,
                            //                            readBuffer->Entry[radiusIndex]->Angel->Position));
                            // add new point to plot
                            display->AddPlotPoint(readBuffer->Entry[radiusIndex], false);
                            readBuffer->DisplayIndex = radiusIndex;
                        }
                        else
                        {
                            // set new display index and buffer
                            displayMutex->WaitOne();
                            readBuffer->DisplayIndex = radiusIndex;
                            copyBuffer.Copy(readBuffer->Entry[radiusIndex]);
                            displayMutex->ReleaseMutex();
                        }
                        // check for 360° measurement
                        if (   (angelCount360 != 0)
                            && (over360 == false)
                            && (readBuffer->RadiusCount >= angelCount360))
                        {
                            over360 = true;
                            //Debug::Print(String::Format(L"Over 360° radius ok: {0}", angelCount360));
                        }
                    }
                    //for each (UInt32 pointIndex in eibPointList[eibIndex]->Keys)
                    //{
                        //Debug::Print(String::Format(
                        //        L"Eib: {0}, count: {1}, index: {2}, point: {3}, radius: {4:F3}{5}",
                        //        eibIndex,                        
                        //        readBuffer->RadiusCount,
                        //        radiusIndex,
                        //        pointIndex,
                        //        readBuffer->Entry[radiusIndex]->Radius[pointIndex]->Position,
                        //        readBuffer->Entry[radiusIndex]->Radius[pointIndex]->Unit));
                    //}
                    // increment radius index
                    radiusIndex ++;
                    if (radiusIndex >= Int32(readBuffer->MeasureNum))
                    {
                        radiusIndex -= readBuffer->MeasureNum;
                    }
                    // set new radius index for all measure points / axis of this EIB
                    for each (UInt32 pointIndex in eibPointList[eibIndex]->Keys)
                    {
                        readBuffer->RadiusIndex[pointIndex] = radiusIndex;
                    }
                    // get current angel count
                    UInt32 angelCount = readBuffer->AngelCount;
                    // check stop conditions
                    //Debug::Print(String::Format(
                    //             L"Crankshaft last: {0}, stop: {1}, radius: {2}, angel: {3}",
                    //             lastRadius, angelStop, readBuffer->RadiusCount, angelCount));
                    if (   (angelStop == true)
                        && (readBuffer->RadiusCount == angelCount))
                    {
                        stopFlag = true;
                        //Debug::Print(String::Format(L"Radius stop eib: {0}, count: {1}, index: {2}",
                        //                            eibIndex, readBuffer->RadiusCount, radiusIndex));
                        return stopFlag;
                    }
                }
            }
            finally
            {
                // reset measure mutex
                measureMutex->ReleaseMutex();
            }
            return stopFlag;
        }

        /// <summary>
        /// Check for abs(new angel - last angel) > delta.
        /// </summary>
        /// <param name="lastAngel">The last angle.</param>
        /// <param name="newAngel">The new angle.</param>
        /// <param name="deltaAngel">The delta angle.</param>
        /// <returns">True if angel > angel + delta.</returns>
        bool CheckOverDelta(Double lastAngel, Double newAngel, Double deltaAngel)
        {
            Double diffAngel = Math::Abs(newAngel - lastAngel);
            if (diffAngel > 180.0)
            {
                diffAngel = 360.0 - diffAngel;
            }
            return diffAngel >= deltaAngel;
        }

        /// <summary>
        /// Check for new angel > last angel.
        /// </summary>
        /// <param name="lastAngel">The last angle.</param>
        /// <param name="newAngel">The new angle.</param>
        /// <returns">True if new angel > last angel.</returns>
        bool CheckForward(Double lastAngel, Double newAngel)
        {
            bool forward;
            if (lastAngel >= 180.0)
            {
                forward = (   (newAngel > lastAngel)
                           || (newAngel < lastAngel - 180.0));
            }
            else
            {
                forward = (   (newAngel > lastAngel)
                           && (newAngel < lastAngel + 180.0));
            }
            return forward;
        }

        /// <summary>
        /// Check for angel > 360°.
        /// </summary>
        /// <param name="firstAngel">The first angle.</param>
        /// <param name="lastAngel">The last angle.</param>
        /// <param name="newAngel">The new angle.</param>
        /// <param name="forward">The angle direction.</param>
        /// <returns">True if new angel is > 360°.</returns>
        bool CheckOver360(Double firstAngel, Double lastAngel,
                          Double newAngel, bool forward)
        {
            bool angelOver360;
            // check forward
            if (forward == true)
            {
                angelOver360 = (   (   (lastAngel <  firstAngel)
                                    && (newAngel  >= firstAngel))
                                || (   (lastAngel <  firstAngel)
                                    && (newAngel  <  lastAngel))
                                || (   (newAngel  <  lastAngel)
                                    && (newAngel  >= firstAngel)));
            }
            else
            {
                angelOver360 = (   (   (lastAngel >  firstAngel)
                                    && (newAngel  <= firstAngel))
                                || (   (lastAngel >  firstAngel)
                                    && (newAngel  >  lastAngel))
                                || (   (newAngel  >  lastAngel)
                                    && (newAngel  <= firstAngel)));
            }
            // check over 360°
            if (angelOver360 == true)
            {
                //Debug::Print(String::Format(
                //        L"Crankshaft forward: {0}, first: {1:F3}°, last: {2:F3}°, new: {3:F3}°",
                //        forward, firstAngel, lastAngel, newAngel));
            }
            return angelOver360;
        }

        /// <summary>
        /// Check for measurement stop.
        /// </summary>
        /// <param name="measureCount">The number of measurements.</param>
        /// <param name="angelOver360">The angel over 360° check result.</param>
        /// <returns">True if stop conditions are met.</returns>
        bool CheckForStop(UInt32 measureCount, bool angelOver360)
        {
            bool stop = (   (display->CheckBoxStop->Checked == true)
                         && (   (measureCount >= readBuffer->MeasureNum)
                             || (angelOver360 == true)));
            if (stop == true)
            {
                //Debug::Print(String::Format(L"Crankshaft over 360: {0}, count: {1}, num: {2}",
                //                            angelOver360, measureCount, readBuffer->MeasureNum));
            }
            return stop;
        }

        /// <summary>
        /// Calculate measure number for last 360°.
        /// </summary>
        /// <param name="measureBuffer">The measure data buffer.</param>
        /// <param name="lastIndex">The last buffer index.</param>
        /// <param name="newIndex">The new buffer index.</param>
        /// <param name="lastAngel">The last angle.</param>
        /// <param name="newAngel">The new angle.</param>
        void CalculateCount360(CrankshaftBuffer ^ measureBuffer,
                               Int32 lastIndex, Int32 newIndex,
                               Double lastAngel, Double newAngel)
        {
            UInt32 count360 = 1;
            if (measureBuffer->AngelCount > 0)
            {
                // set current measure number
                count360 = measureBuffer->Entry[lastIndex]->Count360 + 1;
                // calculate current angel distance
                if (measureBuffer->AngelForward == true)
                {
                    if (newAngel >= lastAngel)
                    {
                        measureBuffer->AngelDistance += newAngel - lastAngel;
                    }
                    else
                    {
                        measureBuffer->AngelDistance += newAngel + 360.0 - lastAngel;
                    }
                }
                else
                {
                    if (lastAngel >= newAngel)
                    {
                        measureBuffer->AngelDistance += lastAngel - newAngel;
                    }
                    else
                    {
                        measureBuffer->AngelDistance += lastAngel + 360.0 - newAngel;
                    }
                }
                // check current angel distance
                if (measureBuffer->AngelDistance >= 360.0)
                {
                    // distance > 360° --> reduce measure number
                    Int32 index0 = newIndex - (count360 - 1);
                    if (index0 < 0)
                    {
                        index0 += measureBuffer->MeasureNum;
                    }
                    Int32 index1;
                    Double angel0, angel1;
                    while (measureBuffer->AngelDistance >= 360.0)
                    {
                        index1 = index0 + 1;
                        if (index1 >= Int32(measureBuffer->MeasureNum))
                        {
                            index1 -= measureBuffer->MeasureNum;
                        }
                        angel0 = measureBuffer->Entry[index0]->Angel->Position;
                        angel1 = measureBuffer->Entry[index1]->Angel->Position;
                        if (measureBuffer->AngelForward == true)
                        {
                            if (angel1 >= angel0)
                            {
                                measureBuffer->AngelDistance -= angel1 - angel0;
                            }
                            else
                            {
                                measureBuffer->AngelDistance -= angel1 + 360.0 - angel0;
                            }
                        }
                        else
                        {
                            if (angel0 >= angel1)
                            {
                                measureBuffer->AngelDistance -= angel0 - angel1;
                            }
                            else
                            {
                                measureBuffer->AngelDistance -= angel0 + 360.0 - angel1;
                            }
                        }
                        count360 --;
                        index0 = index1;
                    }
                }
            }
            // set new measure number
            if (count360 > measureBuffer->MeasureNum)
            {
                count360 = measureBuffer->MeasureNum;
                //Debug::Print(String::Format(L"Overflow count360"));
            }
            measureBuffer->Entry[newIndex]->Count360 = count360;
            Int32 firstIndex = newIndex - (count360 - 1);
            if (firstIndex < 0) firstIndex += measureBuffer->MeasureNum;
            //Debug::Print(String::Format(
            //        L"First index: {0}, angel: {1:F3}°, last index: {2}, angel: {3:F3}°, dist: {4:F3}°",
            //        firstIndex,
            //        measureBuffer->Entry[firstIndex]->Angel->Position,
            //        newIndex,
            //        measureBuffer->Entry[newIndex]->Angel->Position,
            //        measureBuffer->AngelDistance));
        }

        /// <summary>
        /// Get first index for 360° measurement.
        /// </summary>
        /// <param name="measureBuffer">The measure data buffer.</param>
        /// <param name="measureCount">The number of measurements.</param>
        /// <returns">The first index for 360° measurement.</returns>
        Int32 GetIndex360(CrankshaftBuffer ^ measureBuffer, UInt32 % measureCount)
        {
            UInt32 measureNum = Math::Min(measureBuffer->AngelCount, measureBuffer->MeasureNum);
            Int32  nextIndex  = measureBuffer->DisplayIndex;
            Double lastAngel  = measureBuffer->Entry[nextIndex]->Angel->Position;
            Int32  lastIndex;
            Double nextAngel;
            Double distance   = 0.0;
            // check last angels
            for (measureCount = 1; measureCount < measureNum; measureCount ++)
            {
                lastIndex = nextIndex;
                // set new measure buffer index
                nextIndex --;
                if (nextIndex < 0)
                {
                    nextIndex += measureNum;
                }
                // check new angel
                nextAngel = measureBuffer->Entry[nextIndex]->Angel->Position;
                if (measureBuffer->AngelForward == true)
                {
                    if (nextAngel <= lastAngel)
                    {
                        distance += lastAngel - nextAngel;
                    }
                    else
                    {
                        distance += lastAngel + 360.0 - nextAngel;
                    }
                }
                else
                {
                    if (lastAngel <= nextAngel)
                    {
                        distance += nextAngel - lastAngel;
                    }
                    else
                    {
                        distance += nextAngel + 360.0 - lastAngel;
                    }
                }
                if (distance >= 360.0)
                {
                    nextIndex = lastIndex;
                    break;
                }
                lastAngel = nextAngel;
            }
            //Debug::Print(String::Format(
            //        L"Plot data first index: {0}, angel: {1:F3}°, last index: {2}, angel: {3:F3}°",
            //        nextIndex,
            //        measureBuffer->Entry[nextIndex]->Angel->Position,
            //        measureBuffer->DisplayIndex,
            //        measureBuffer->Entry[measureBuffer->DisplayIndex]->Angel->Position));
            return nextIndex;
        }

        /// <summary>
        /// Show crankshaft measure data.
        /// </summary>
        void ShowMeasureData()
        {
            // check for crankshaft application or abort flag
            if (   (guiSupport->GuiConfig->GuiParam->AppCrankshaft == false)
                || (abortFlag == true)
                || (displayMode == DisplayModes::DirectRefresh))
            {
                return;
            }
            // display measure data
            if (   (readBuffer->RadiusCount > 0)
                && (readBuffer->DisplayIndex != displayIndex))
            {
                displayIndex = readBuffer->DisplayIndex;
                // check display mode
                if (displayMode == DisplayModes::DirectPlot)
                {
                    // refresh graphic
                    display->RefreshGraph();
                    return;
                }
                // copy display buffer
                displayMutex->WaitOne();
                displayBuffer->Entry[displayBuffer->AngelIndex]->Copy(%copyBuffer);
                displayMutex->ReleaseMutex();
                // get last angel
                Int32 lastIndex = displayBuffer->AngelIndex - 1;
                if (lastIndex < 0)
                {
                    lastIndex += displayBuffer->MeasureNum;
                }
                Double lastAngel = displayBuffer->Entry[lastIndex]->Angel->Position;
                // get new angel
                Int32 newIndex  = displayBuffer->AngelIndex;
                Double newAngel = displayBuffer->Entry[newIndex]->Angel->Position;
                // calculate measure number for last 360°
                CalculateCount360(displayBuffer, lastIndex, newIndex, lastAngel, newAngel);
                // set new display index and buffer
                displayBuffer->DisplayIndex = displayBuffer->AngelIndex;
                displayBuffer->AngelIndex ++;
                if (displayBuffer->AngelIndex >= Int32(displayBuffer->MeasureNum))
                {
                    displayBuffer->AngelIndex -= displayBuffer->MeasureNum;
                }
                displayBuffer->AngelCount ++;
                // show measure data
                if (displayMode == DisplayModes::PlotPoint)
                {
                    // add new point to plot
                    display->AddPlotPoint(displayBuffer->Entry[displayBuffer->DisplayIndex], true);
                    return;
                }
                // get plot data
                UInt32 measureCount = displayBuffer->Entry[displayBuffer->DisplayIndex]->Count360;
                Int32 firstIndex    = displayBuffer->DisplayIndex - (measureCount - 1);
                if (firstIndex < 0) firstIndex += displayBuffer->MeasureNum;
                //UInt32 measureCount;
                //Int32  firstIndex = GetIndex360(displayBuffer, measureCount);
                // clear the data plots
                display->ClearDisplay();
                // show new data plots
                display->DisplayMeasureData(false,
                                            displayBuffer, measureCount, firstIndex, true,
                                            false, PlotStyles::Point, RefStyles::Point,
                                            nullptr);
            }
        }

        /// <summary>
        /// Stop crankshaft measurement.
        /// </summary>
        void StopMeasurement()
        {
            // check for crankshaft application
            if (guiSupport->GuiConfig->GuiParam->AppCrankshaft == false)
            {
                return;
            }
            // enable buttons
            display->EnableButtons(true, false);
            // check for abort flag
            if (abortFlag == true)
            {
                return;
            }
            // enable save reference button
            if (   (over360 == true)
                && (radiusNotSet == false))
            {
                display->EnableButtons(true, true);
            }
            // display measure data
            if (readBuffer->RadiusCount > 0)
            {
                // get plot data
                UInt32 measureCount = readBuffer->Entry[readBuffer->DisplayIndex]->Count360;
                Int32 firstIndex    = readBuffer->DisplayIndex - (measureCount - 1);
                if (firstIndex < 0) firstIndex += readBuffer->MeasureNum;
                //UInt32 measureCount1;
                //Int32  firstIndex1 = GetIndex360(readBuffer, measureCount1);
                //Debug::Print(String::Format(
                //                L"First index: {0}, count: {1}, first index: {2}, count: {3}",
                //                firstIndex, measureCount, firstIndex1, measureCount1));
                // clear the data plots
                display->ClearDisplay();
                // show new data plots
                display->DisplayMeasureData(false,
                                            readBuffer, measureCount, firstIndex, true,
                                            over360, PlotStyles::Line, RefStyles::Line,
                                            nullptr);
            }
        }

        /// <summary>
        /// Save crankshaft reference.
        /// </summary>
	    /// <param name="fileName">Reference file name.</param>
        void SaveReference(String ^ fileName)
        {
            // get plot data
            UInt32 measureCount = readBuffer->Entry[readBuffer->DisplayIndex]->Count360;
            Int32 firstIndex;
            if (readBuffer->AngelForward == true)
            {
                firstIndex = readBuffer->DisplayIndex - (measureCount - 1);
            }
            else
            {
                firstIndex = readBuffer->DisplayIndex;
            }
            if (firstIndex < 0) firstIndex += readBuffer->MeasureNum;
            // display reference data
            display->DisplayMeasureData(true,
                                        readBuffer, measureCount,
                                        firstIndex, readBuffer->AngelForward,
                                        true, PlotStyles::Line, RefStyles::Line,
                                        fileName);
        }

        /// <summary>
        /// Load crankshaft reference.
        /// </summary>
	    /// <param name="fileName">Reference file name.</param>
        void LoadReference(String ^ fileName)
        {
            // display reference data
            display->DisplayMeasureData(true,
                                        nullptr, 0, 0, true,
                                        true, PlotStyles::Line, RefStyles::Line,
                                        fileName);
        }

        /// <summary>
        /// Clear crankshaft reference.
        /// </summary>
        void ClearReference()
        {
            display->ClearReference(true);
        }
    };
    #pragma endregion
}
