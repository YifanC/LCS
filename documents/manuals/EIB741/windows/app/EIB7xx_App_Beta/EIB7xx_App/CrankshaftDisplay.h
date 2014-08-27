///
/// \file       CrankshaftDisplay.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: CrankshaftDisplay.h,v 1.3 2011/12/16 13:37:57 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: CrankshaftDisplay.h,v $
/// Revision 1.3  2011/12/16 13:37:57  rohn
/// Version 0.9.0.1 (siehe History.txt)
///
/// Revision 1.2  2011/09/20 12:24:36  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB graphic display class.
///
/// This class is used for the graphic display of the
/// crankshaft measure data based on .NET graphic.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Resources;
using namespace System::Reflection;
using namespace System::Windows::Forms;

namespace EIB7xx_App
{
    #pragma region "Crankshaft_Display class definition"
    // special definitions
    #define CRANKSHAFT_DATA_PLOT_COLOR      Color::Blue
    #define CRANKSHAFT_REF_POINT_COLOR      Color::Red
    #define CRANKSHAFT_REF_PLOT_COLOR       JH_COLOR_GRAY_LIGHT

    /// <summary>
    /// This class is used for the graphic display of the crankshaft measure data
    /// crankshaft measure data based on .NET graphic.
    /// </summary>
    public ref class Crankshaft_Display
    {
    /// list of polar coordinate graphics
    typedef array<PolarGraph ^>                 GraphList;
    /// list of polar coordinate plot handels on the graphics
    typedef Dictionary<UInt32, UInt32>          HandleList;
    /// list of plots added to the graphic
    typedef Dictionary<UInt32, PolarPlot ^>     PlotList;

    private:
        UInt32              fileVersion;    ///< parameter version for reference file

        GUI_Support ^       guiSupport;     ///< the GUI support class
        ResourceManager ^   resManager;     ///< the resource manager from main window

        Crankshaft_Param ^  param;          ///< the crankshaft parameters

        CheckBox ^          checkBoxStop;   ///< checkbox for stop at 360°
        Button ^            buttonRefSave;  ///< button for save reference
        Button ^            buttonRefLoad;  ///< button for load reference
        Button ^            buttonRefClear; ///< button for clear reference

        GraphList ^         graphList;      ///< polar graph list
        HandleList ^        dataPlotList;   ///< data plot list
        HandleList ^        refPlotList;    ///< reference plot list
        PlotList ^          newPlotList;    ///< new plot list

        UInt32          graphAxisMax;   ///< max value for graph axis in mmm
        Double          tolerance;      ///< tolerance value to reference curve in µm

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="guiSupport">The GUI support class.</param>
        /// <param name="param">The crankshaft parameters.</param>
        /// <param name="checkBoxStop">The checkbox for stop at 360°.</param>
        /// <param name="buttonRefSave">The button for save reference.</param>
        /// <param name="buttonRefLoad">The button for load reference.</param>
        /// <param name="buttonRefClear">The button for clear reference.</param>
        /// <param name="graph1">The crankshaft graph 1.</param>
        /// <param name="graph2">The crankshaft graph 2.</param>
        /// <param name="graph3">The crankshaft graph 3.</param>
        /// <param name="graph4">The crankshaft graph 4.</param>
        Crankshaft_Display(GUI_Support ^      guiSupport,
                           Crankshaft_Param ^ param,
                           CheckBox ^         checkBoxStop,
                           Button ^           buttonRefSave,
                           Button ^           buttonRefLoad,
                           Button ^           buttonRefClear,
                           PolarGraph ^       graph1,
                           PolarGraph ^       graph2,
                           PolarGraph ^       graph3,
                           PolarGraph ^       graph4)
        {
            fileVersion = 1;

            this->guiSupport = guiSupport;
            this->resManager = guiSupport->ResManager;

            this->param  = param;
            graphAxisMax = param->GraphAxisMax;
            tolerance    = param->Tolerance;

            this->checkBoxStop   = checkBoxStop;
            this->buttonRefSave  = buttonRefSave;
            this->buttonRefLoad  = buttonRefLoad;
            this->buttonRefClear = buttonRefClear;

            graphList = gcnew GraphList(4);
            graphList[0] = graph1;
            graphList[1] = graph2;
            graphList[2] = graph3;
            graphList[3] = graph4;

            dataPlotList = gcnew HandleList;
            refPlotList  = gcnew HandleList;
            newPlotList  = gcnew PlotList;

            buttonRefSave->Enabled = false;
        }

        /// <summary>
        /// Gets the checkbox for stop at 360°.
        /// </summary>
        property CheckBox ^ CheckBoxStop
        {
            CheckBox ^ get() { return checkBoxStop; }
        }

        /// <summary>
        /// Gets the button for save reference.
        /// </summary>
        property Button ^ ButtonRefSave
        {
            Button ^ get() { return buttonRefSave; }
        }

        /// <summary>
        /// Enable / disable buttons.
        /// </summary>
        /// <param name="enable">If true enable buttons else disable buttons.</param>
        /// <param name="reference">If true enable save reference button.</param>
        void EnableButtons(bool enable, bool reference)
        {
            checkBoxStop->Enabled = enable;
            buttonRefLoad->Enabled = enable;
            buttonRefClear->Enabled = enable;

            if (   (enable == false)
                || (reference == true))
            {
                buttonRefSave->Enabled = enable;
            }
        }

        /// <summary>
        /// Clear the graphic display.
        /// </summary>
        void ClearDisplay()
        {
            // clear measure data plots but not reference plots
            for each (UInt32 handle in dataPlotList->Keys)
            {
                graphList[dataPlotList[handle]]->DeletePlot(handle);
            }
            dataPlotList->Clear();
            // clear list of current procesed plots
            newPlotList->Clear();
        }

        /// <summary>
        /// Refresh the graphic display.
        /// </summary>
        void RefreshDisplay()
        {
            // check if parameters had been changed
            if (graphAxisMax != param->GraphAxisMax)
            {
                // graph axis scaling had been changed
                graphAxisMax = param->GraphAxisMax;
                // refresh graph scaling
                for each (PolarGraph ^ graph in graphList)
                {
                    graph->GraphAxisMax = graphAxisMax;
                }
            }
            if (tolerance != param->Tolerance)
            {
                // tolerance had been changed
                tolerance = param->Tolerance;
                // refresh reference plots
                for each (UInt32 handle in refPlotList->Keys)
                {
                    graphList[refPlotList[handle]]->PlotFromHandle[handle]->Tolerance = tolerance;
                    graphList[refPlotList[handle]]->RefreshPlot(handle);
                }
                // refresh measure data plots
                for each (UInt32 handle in dataPlotList->Keys)
                {
                    graphList[dataPlotList[handle]]->RefreshPlot(handle);
                }
            }
        }

        /// <summary>
        /// Display measure data.
        /// </summary>
        /// <param name="reference">If true it's a reference plot.</param>
        /// <param name="measureBuffer">The measure data buffer.</param>
        /// <param name="measureNum">The number of measure data in buffer.</param>
        /// <param name="firstIndex">The first index of measure data in buffer.</param>
        /// <param name="forward">The angel direction of measure data in buffer.</param>
        /// <param name="closeFlag">If true close plot from end to start.</param>
        /// <param name="plotStyle">The plot style.</param>
        /// <param name="refStyle">The reference detection style.</param>
	    /// <param name="fileName">Reference file name.</param>
        void DisplayMeasureData(bool reference,
                                CrankshaftBuffer ^ measureBuffer,
                                UInt32 measureNum,
                                UInt32 firstIndex, bool forward,
                                bool closeFlag, PlotStyles plotStyle,
                                RefStyles refStyle,
                                String ^ fileName)
        {
            CrankshaftRefFile ^ refFile = nullptr;
            try
            {
                // clear current reference
                if (reference == true)
                {
                    ClearReference(false);
                }
                // check for reference file
                Double angleDistance = param->AngleDistance;
                UInt32 usedFlag   = 0;
                UInt32 bufferSize = 0;
                if (measureBuffer != nullptr)
                {
                    // no reference file or before write to reference file
                    usedFlag   = measureBuffer->UsedFlag;
                    bufferSize = measureBuffer->MeasureNum;
                }
                if (fileName != nullptr)
                {
                    if (measureBuffer == nullptr)
                    {
                        // open reference file for read
                        refFile = gcnew CrankshaftRefFile(guiSupport, fileName, false);
                        // read file header
                        String ^ fileHeader;
                        refFile->ReadWriteString(fileHeader);
                        // check file header
                        if (fileHeader != REFERENCE_FILE_HEADER)
                        {
                            String ^ message = String::Format(
                                                    resManager->GetString(L"errorEibReferenceFileHeader"),
                                                    fileName);
                            throw gcnew EIB_FileException(message, fileName);
                        }
                        // read parameter version
                        UInt32 paramVersion;
                        refFile->ReadWriteUInt32(paramVersion);
                        // check parameter version
                        refFile->CheckParamVersion(paramVersion, fileVersion);
                    }
                    else
                    {
                        // open reference file for write
                        refFile = gcnew CrankshaftRefFile(guiSupport, fileName, true);
                        // write file header
                        String ^ fileHeader = REFERENCE_FILE_HEADER;
                        refFile->ReadWriteString(fileHeader);
                        // write parameter version
                        refFile->ReadWriteUInt32(fileVersion);
                    }
                    // read / write number of measurements
                    refFile->ReadWriteUInt32(measureNum);
                    // read / write used flag
                    refFile->ReadWriteUInt32(usedFlag);
                    // read / write angle distance
                    refFile->ReadWriteDouble(angleDistance);
                }
                if (measureBuffer == nullptr)
                {
                    // after read from reference file
                    firstIndex = 0;
                    forward    = true;
                    closeFlag  = true;
                    bufferSize = measureNum;
                }
                // get / set measure data
                UInt32 measurePointMax = Math::Min(UInt32(4), CRANKSHAFT_MEASURE_POINT_NUM);
                UInt32 k = firstIndex;
                for (UInt32 j = 0; j < measureNum; j ++)
                {
                    Int32 firstPoint = -1;
                    for (UInt32 i = 0; i < measurePointMax; i ++)
                    {
                        // write measure data to graphic data buffer
                        if ((usedFlag & (0x01 << i)) != 0)
                        {
                            if (firstPoint < 0)
                            {
                                firstPoint = i;
                                // add new plot
                                if (j == 0)
                                {
                                    AddPlot(reference, usedFlag, measureNum,
                                            closeFlag, plotStyle,
                                            refStyle, angleDistance);
                                }
                            }
                            // read / write measure data
                            if (measureBuffer != nullptr)
                            {
                                // no reference file or write to reference file
                                newPlotList[i]->Angel[j]  = measureBuffer->Entry[k]->Angel->Position;
                                newPlotList[i]->Radius[j] = measureBuffer->Entry[k]->Radius[i]->Position;
                            }
                            if (refFile != nullptr)
                            {
                                // read from / write to reference file
                                if (Int32(i) == firstPoint)
                                {
                                    newPlotList[i]->Angel[j] =
                                                    refFile->ReadWriteDouble(newPlotList[i]->Angel[j]);
                                }
                                else if (measureBuffer == nullptr)
                                {
                                    newPlotList[i]->Angel[j] = newPlotList[firstPoint]->Angel[j];
                                }
                                newPlotList[i]->Radius[j] =
                                                refFile->ReadWriteDouble(newPlotList[i]->Radius[j]);
                            }
                            // refresh new plot
                            if (j == measureNum - 1)
                            {
                                newPlotList[i]->ValueNumber = measureNum;
                                newPlotList[i]->ConvertPlot();
                                // set reference in data plots
                                if (reference == true)
                                {
                                    UInt32 refHandle = 0;
                                    for each (refHandle in refPlotList->Keys)
                                    {
                                        if (refPlotList[refHandle] == i)
                                        {
                                            break;
                                        }
                                    }
                                    for each (UInt32 plotHandle in dataPlotList->Keys)
                                    {
                                        if (dataPlotList[plotHandle] == i)
                                        {
                                            graphList[i]->PlotFromHandle[plotHandle]->RefPlot =
                                            graphList[i]->PlotFromHandle[refHandle];
                                            graphList[i]->RefreshPlot(plotHandle);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    // set new measure buffer index
                    if (forward == true)
                    {
                        k ++;
                        if (k >= bufferSize)
                        {
                            k -= bufferSize;
                        }
                    }
                    else
                    {
                        k --;
                        if (k < 0)
                        {
                            k += bufferSize;
                        }
                    }
                }
            }
            finally
            {
                if (refFile != nullptr)
                {
                    // close reference file
                    delete refFile;
                }
            }
        }

        /// <summary>
        /// Add new plot to graphic.
        /// </summary>
        /// <param name="reference">If true it's a reference plot.</param>
        /// <param name="usedFlag">The measure point used flag.</param>
        /// <param name="measureNum">The number of measure data in buffer.</param>
        /// <param name="closeFlag">If true close plot from end to start.</param>
        /// <param name="plotStyle">The plot style.</param>
        /// <param name="refStyle">The reference detection style.</param>
        /// <param name="angleDistance">The angle distance.</param>
        void AddPlot(bool reference,
                     UInt32 usedFlag, UInt32 measureNum,
                     bool closeFlag, PlotStyles plotStyle,
                     RefStyles refStyle, Double angleDistance)
        {
            PolarPlot ^ plot;
            UInt32 plotHandle;
            UInt32 measurePointMax = Math::Min(UInt32(4), CRANKSHAFT_MEASURE_POINT_NUM);
            for (UInt32 i = 0; i < measurePointMax; i ++)
            {
                if ((usedFlag & (0x01 << i)) != 0)
                {
                    if (reference == false)
                    {
                        // set reference in data plots
                        PolarPlot ^ refPlot = nullptr;
                        for each (UInt32 handle in refPlotList->Keys)
                        {
                            if (refPlotList[handle] == i)
                            {
                                refPlot = graphList[i]->PlotFromHandle[handle];
                                break;
                            }
                        }
                        // add data plot to graph
                        plot = gcnew PolarPlot(measureNum,
                                               closeFlag, 1, plotStyle,
                                               CRANKSHAFT_DATA_PLOT_COLOR,
                                               refPlot, refStyle,
                                               CRANKSHAFT_REF_POINT_COLOR);
                        newPlotList[i] = plot;
                        plotHandle = graphList[i]->AddPlot(plot);
                        dataPlotList[plotHandle] = i;
                    }
                    else
                    {
                        // add reference plot to graph
                        plot = gcnew ReferencePlot(measureNum,
                                                   param->Tolerance,
                                                   angleDistance,
                                                   closeFlag, 0, plotStyle,
                                                   CRANKSHAFT_REF_PLOT_COLOR);
                        newPlotList[i] = plot;
                        plotHandle = graphList[i]->AddPlot(plot);
                        refPlotList[plotHandle] = i;
                    }
                }
            }
        }

        /// <summary>
        /// Initialize plot.
        /// </summary>
        /// <param name="refresh">If true refresh the graphic.</param>
        void InitPlot(bool refresh)
        {
            UInt32 measurePointMax = Math::Min(UInt32(4), CRANKSHAFT_MEASURE_POINT_NUM);
            for (UInt32 i = 0; i < measurePointMax; i ++)
            {
                if (newPlotList->ContainsKey(i) == true)
                {
                    newPlotList[i]->InitPlot();
                    if (refresh == true)
                    {
                        graphList[i]->RefreshGraph();
                    }
                }
            }
        }

        /// <summary>
        /// Add new point to plot.
        /// </summary>
        /// <param name="entry">The measure data of the point.</param>
        /// <param name="refresh">If true refresh the graphic.</param>
        void AddPlotPoint(CrankshaftEntry ^ entry, bool refresh)
        {
            UInt32 measurePointMax = Math::Min(UInt32(4), CRANKSHAFT_MEASURE_POINT_NUM);
            for (UInt32 i = 0; i < measurePointMax; i ++)
            {
                if (newPlotList->ContainsKey(i) == true)
                {
                    newPlotList[i]->AddPoint(entry->Angel->Position,
                                             entry->Radius[i]->Position,
                                             entry->Count360);
                    if (refresh == true)
                    {
                        graphList[i]->RefreshGraph();
                    }
                }
            }
        }

        /// <summary>
        /// Refresh the graphic.
        /// </summary>
        void RefreshGraph()
        {
            UInt32 measurePointMax = Math::Min(UInt32(4), CRANKSHAFT_MEASURE_POINT_NUM);
            for (UInt32 i = 0; i < measurePointMax; i ++)
            {
                if (newPlotList->ContainsKey(i) == true)
                {
                    graphList[i]->RefreshGraph();
                }
            }
        }

        /// <summary>
        /// Clear crankshaft reference.
        /// </summary>
        /// <param name="refresh">If true refresh data plots.</param>
        void ClearReference(bool refresh)
        {
            // clear reference plots but not measure data plots
            for each (UInt32 handle in refPlotList->Keys)
            {
                graphList[refPlotList[handle]]->DeletePlot(handle);
            }
            refPlotList->Clear();
            // remove reference in data plots
            if (refresh == true)
            {
                for each (UInt32 handle in dataPlotList->Keys)
                {
                    graphList[dataPlotList[handle]]->PlotFromHandle[handle]->RefPlot = nullptr;
                    graphList[dataPlotList[handle]]->RefreshPlot(handle);
                }
            }
        }
    };
    #pragma endregion
}
