///
/// \file       DisplayPolarGraph.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: DisplayPolarGraph.h,v 1.3 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: DisplayPolarGraph.h,v $
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
/// \brief      EIB graphic display class.
///
/// This classes are used for the graphic display of measure data
/// with a polar coordinate diagram based on .NET graphic.
///

#pragma once

#include "FormMain.h"

using namespace System;

namespace EIB7xx_App
{
    #pragma region "PolarPlot class definition"
    /// plot value array
    typedef array<Double>   ValueArray;
    /// array of plot values for the paint fuction
    typedef array<Int32>    PaintArray;
    /// array of pens for the paint fuction
    typedef array<Pen ^>    PenArray;

    /// <summary>
    /// This enum lists plot styles.
    /// </summary>
    enum class PlotStyles : UInt32
    {
        Line  = 0,
        Point = 1
    };

    /// <summary>
    /// This enum lists reference detection styles.
    /// </summary>
    enum class RefStyles : UInt32
    {
        Line  = 0,
        Point = 1
    };

    /// <summary>
    /// This class is used for the graphic display of measure data
    /// with a polar coordinate diagram based on .NET graphic.
    /// </summary>
    public ref class PolarPlot
    {
    protected:
        UInt32          valueNumber;    ///< number of values
        UInt32          bufferSize;     ///< plot buffer size

        bool            closeFlag;      ///< if true close plot from end to start

        UInt32          zOrder;         ///< plot z order

        PlotStyles      plotStyle;      ///< plot style
        RefStyles       refStyle;       ///< reference detection style

        Pen ^           plotPen;        ///< plot pen

        ValueArray ^    angel;          ///< angel value array
        ValueArray ^    radius;         ///< radius value array

        PaintArray ^    paintX0;        ///< x coordinates 0 for paint
        PaintArray ^    paintY0;        ///< Y coordinates 0 for paint
        PenArray ^      paintPen0;      ///< the pen 0 for paint

        Int32           plotAreaWidth;  ///< plot area width
        Int32           plotAreaHeight; ///< plot area height

        Double          xGain;          ///< conversion x gain
        Double          xOffs;          ///< conversion x offset
        Double          yGain;          ///< conversion y gain
        Double          yOffs;          ///< conversion y offset

        PolarPlot ^     refPlot;        ///< reference plot

        Pen ^           refPen;         ///< reference pen

        Double          tolerance;      ///< tolerance value
        Double          angleDistance;  ///< angle distance in °

        Int32           firstIndex;     ///< first plot display index
        Int32           newIndex;       ///< first plot data index
        Int32           refIndex;       ///< reference plot index

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="bufferSize">The plot buffer size.</param>
        /// <param name="closeFlag">If true close plot from end to start.</param>
        /// <param name="zOrder">The plot z order.</param>
        /// <param name="plotStyle">The plot style.</param>
        /// <param name="plotColor">The plot color.</param>
        /// <param name="refPlot">The reference plot color.</param>
        /// <param name="refStyle">The reference detection style.</param>
        /// <param name="refColor">The reference point color.</param>
        PolarPlot(UInt32        bufferSize,
                  bool          closeFlag,
                  UInt32        zOrder,
                  PlotStyles    plotStyle,
                  Color         plotColor,
                  PolarPlot ^   refPlot,
                  RefStyles     refStyle,
                  Color         refColor)
        {
            this->bufferSize  = bufferSize;
            this->closeFlag   = closeFlag;
            this->zOrder      = zOrder;
            this->plotStyle   = plotStyle;
            this->refStyle    = refStyle;

            this->refPlot     = refPlot;
            refIndex          = -1;

            plotPen = gcnew Pen(plotColor);
            refPen  = gcnew Pen(refColor);

            // set number of plot points
            InitPlot();

            // get data arrays
            angel     = gcnew ValueArray(bufferSize);
            radius    = gcnew ValueArray(bufferSize);
            // get arrays for painting
            paintX0   = gcnew PaintArray(bufferSize);
            paintY0   = gcnew PaintArray(bufferSize);
            paintPen0 = gcnew PenArray(bufferSize);
        }

    protected:
		/// <summary>
		/// Destructor.
		/// </summary>
		~PolarPlot()
		{
            Delete();
        }

		/// <summary>
		/// Finalizer.
		/// </summary>
		!PolarPlot()
		{
            Delete();
		}

    private:
        /// <summary>
        /// Delete function.
        /// </summary>
        void Delete()
        {
            if (plotPen != nullptr)
            {
                delete plotPen;
                plotPen = nullptr;
            }
            if (refPen != nullptr)
            {
                delete refPen;
                refPen = nullptr;
            }
            if (angel != nullptr)
            {
                delete angel;
                angel = nullptr;
            }
            if (radius != nullptr)
            {
                delete radius;
                radius = nullptr;
            }
            if (paintX0 != nullptr)
            {
                delete paintX0;
                paintX0 = nullptr;
            }
            if (paintY0 != nullptr)
            {
                delete paintY0;
                paintY0 = nullptr;
            }
            if (paintPen0 != nullptr)
            {
                delete paintPen0;
                paintPen0 = nullptr;
            }
        }

    public:
        /// <summary>
        /// Gets / sets the angel value.
        /// </summary>
        property Double Angel[UInt32]
        {
            Double get(UInt32 index) { return angel[index]; }
            void set(UInt32 index, Double value) { angel[index] = value; }
        }

        /// <summary>
        /// Gets / sets the radius value.
        /// </summary>
        property Double Radius[UInt32]
        {
            Double get(UInt32 index) { return radius[index]; }
            void set(UInt32 index, Double value) { radius[index] = value; }
        }

        /// <summary>
        /// Gets / sets the number of values.
        /// </summary>
        property UInt32 ValueNumber
        {
            UInt32 get() { return valueNumber; }
            void set(UInt32 value) { valueNumber = value; }
        }

        /// <summary>
        /// Sets the reference plot.
        /// </summary>
        property PolarPlot ^ RefPlot
        {
            void set(PolarPlot ^ value)
            {
                refPlot  = value;
                refIndex = -1;
            }
        }

        /// <summary>
        /// Gets / sets the tolerance value to reference curve in µm.
        /// </summary>
        property Double Tolerance
        {
            Double get() { return tolerance; }
            void set(Double value) { tolerance = value; }
        }

        /// <summary>
        /// Gets the min angle distance in °.
        /// </summary>
        property Double AngleDistance
        {
            Double get() { return angleDistance; }
        }

        /// <summary>
        /// Initialize plot.
        /// </summary>
        void InitPlot()
        {
            valueNumber = 0;
            firstIndex  = 0;
            newIndex    = 0;
        }

        /// <summary>
        /// Add point to plot.
        /// </summary>
        /// <param name="angel">The angel value.</param>
        /// <param name="radius">The radius value.</param>
        /// <param name="maxValueNumber">The max value number of the plot.</param>
        void AddPoint(Double angel, Double radius, Int32 maxValueNumber)
        {
            // set new point
            this->angel[newIndex]  = angel;
            this->radius[newIndex] = radius;
            // convert new point
            ConvertPoint(newIndex);
            // set new value number
            valueNumber ++;
            // check max value number
            if (maxValueNumber > 0)
            {
                valueNumber = maxValueNumber;
            }
            if (Int32(valueNumber) > bufferSize)
            {
                valueNumber = bufferSize;
            }
            // set first index for display
            firstIndex = newIndex - (valueNumber - 1);
            if (firstIndex < 0) firstIndex += bufferSize;
            //Debug::Print(String::Format(
            //                L"new: {0}, first: {1}, num: {2}, max: {3}, angel: {4:F3}°",
            //                newIndex, firstIndex, valueNumber, maxValueNumber, angel));
            // get index for next point
            newIndex ++;
            if (newIndex >= Int32(bufferSize)) newIndex -= bufferSize;
        }

        /// <summary>
        /// Prepare plot convertion from polar to cartesian coordinates.
        /// </summary>
        /// <param name="graphAxisMax">The graphic axis max value.</param>
        /// <param name="plotAreaWidth">The plot area width.</param>
        /// <param name="plotAreaHeight">The plot area height.</param>
        void PrepareConvert(UInt32 graphAxisMax, Int32 plotAreaWidth, Int32 plotAreaHeight)
        {
            this->plotAreaWidth  = plotAreaWidth;
            this->plotAreaHeight = plotAreaHeight;
            // set x-y-diagram dimensions
            Double xMin = -Int32(graphAxisMax);
            Double xMax =  Int32(graphAxisMax);
            Double yMin = -Int32(graphAxisMax);
            Double yMax =  Int32(graphAxisMax);
            // set translation values
            xGain = Double(plotAreaWidth) / (xMax - xMin);
            xOffs = - xGain * xMin;
            yGain = - Double(plotAreaHeight) / (yMax - yMin);
            yOffs = Double(plotAreaHeight) - yGain * yMin;
        }

        /// <summary>
        /// Convert plot from polar to cartesian coordinates.
        /// </summary>
        /// <param name="graphAxisMax">The graphic axis max value.</param>
        /// <param name="plotAreaWidth">The plot area width.</param>
        /// <param name="plotAreaHeight">The plot area height.</param>
        void ConvertPlot(UInt32 graphAxisMax,
                         Int32 plotAreaWidth, Int32 plotAreaHeight)
        {
            // prepare convertion
            PrepareConvert(graphAxisMax, plotAreaWidth, plotAreaHeight);
            // convert plot
            ConvertPlot();
        }

        /// <summary>
        /// Convert plot from polar to cartesian coordinates.
        /// </summary>
        void ConvertPlot()
        {
            // convert plot points
            Int32 k = firstIndex;
            for (UInt32 i = 0; i < valueNumber; i ++)
            {
                // convert point
                ConvertPoint(k);
                k ++;
                if (k >= Int32(bufferSize)) k -= bufferSize;
            }
        }

        /// <summary>
        /// Convert plot point from polar to cartesian coordinates.
        /// </summary>
        /// <param name="plotIndex">The plot index.</param>
        virtual void ConvertPoint(Int32 plotIndex)
        {
            // convert point
            ConvertPoint(angel[plotIndex], radius[plotIndex],
                         paintX0[plotIndex], paintY0[plotIndex],
                         paintPen0[plotIndex]);
            // check for reference plot
            if (   (refPlot != nullptr)
                && (paintPen0[plotIndex] != nullptr))
            {
                // check if point is in tolerance
                bool inTolerance = CheckPointRef(plotIndex);
                if (inTolerance == false)
                {
                    paintPen0[plotIndex] = refPen;
                }
            }
        }

        /// <summary>
        /// Convert plot point from polar to cartesian coordinates.
        /// </summary>
        /// <param name="angel">The angel value.</param>
        /// <param name="radius">The radius value.</param>
        /// <param name="paintX">The x coordinates for paint.</param>
        /// <param name="paintY">The y coordinates for paint.</param>
        /// <param name="paintPen">The pen for paint.</param>
        void ConvertPoint(Double angel, Double radius,
                          Int32 % paintX, Int32 % paintY,
                          Pen ^ % paintPen)
        {
            // convert from polar to cartesian coordinates
            Double arc = Math::PI * angel / 180.0;
            Double xValue = radius * Math::Cos(arc);
            Double yValue = radius * Math::Sin(arc);
            // convert from mm to paint area
            paintX = Int32(xGain * xValue + xOffs + 0.5);
            paintY = Int32(yGain * yValue + yOffs + 0.5);
            // check if point is in paint area
            if (   (paintX < 0)
                || (paintX > plotAreaWidth)
                || (paintY < 0)
                || (paintY > plotAreaHeight))
            {
                paintPen = nullptr;
            }
            else
            {
                paintPen = plotPen;
            }
        }

        /// <summary>
        /// Check if point is in tolerance.
        /// </summary>
        /// <param name="plotIndex">The plot index.</param>
        /// <returns">True if point is in tolerance.</returns>
        bool CheckPointRef(Int32 plotIndex)
        {
            // get reference index for the point
            GetRefIndex(plotIndex);
            Int32 refIndex1 = refIndex + 1;
            if (refIndex1 >= Int32(refPlot->ValueNumber)) refIndex1 -= refPlot->ValueNumber;
            //Debug::Print(String::Format(
            //    L"Ref index 1: {0}, ref angel 0: {1:F3}°, ref angel 1: {2:F3}°, plot angel: {3:F3}°",
            //    refIndex, refPlot->Angel[refIndex], refPlot->Angel[refIndex1], angel[plotIndex]));
            // check if point is in tolerance
            Double refRadius;
            Double tolerance = refPlot->Tolerance * 1e-3;
            // check + tolerance value
            refRadius = GetRadiusFromAngel(angel[plotIndex],
                                           refPlot->Angel[refIndex],
                                           refPlot->Radius[refIndex]  + tolerance,
                                           refPlot->Angel[refIndex1],
                                           refPlot->Radius[refIndex1] + tolerance);
            if (radius[plotIndex] > refRadius)
            {
                return false;
            }
            // check - tolerance value
            refRadius = GetRadiusFromAngel(angel[plotIndex],
                                           refPlot->Angel[refIndex],
                                           refPlot->Radius[refIndex]  - tolerance,
                                           refPlot->Angel[refIndex1],
                                           refPlot->Radius[refIndex1] - tolerance);
            if (radius[plotIndex] < refRadius)
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// Get the reference plot index for a plot index.
        /// </summary>
        /// <param name="plotIndex">The plot index.</param>
        void GetRefIndex(Int32 plotIndex)
        {
            bool forward;
            // check for first point
            if (refIndex < 0)
            {
                // calculate reference start index
                Double distance;
                if (angel[plotIndex] >= refPlot->Angel[0])
                {
                    distance = angel[plotIndex] - refPlot->Angel[0];
                }
                else
                {
                    distance = angel[plotIndex] + 360.0 - refPlot->Angel[0];
                }
                refIndex = Int32(Math::Ceiling(distance / 360.0 * refPlot->ValueNumber));
                if (refIndex >= Int32(refPlot->ValueNumber))
                {
                    refIndex = refPlot->ValueNumber - 1;
                }
                //Int32 refIndex1 = refIndex + 1;
                //if (refIndex1 >= Int32(refPlot->ValueNumber)) refIndex1 -= refPlot->ValueNumber;
                //Debug::Print(String::Format(
                //    L"Ref index 0: {0}, ref angel 0: {1:F3}°, ref angel 1: {2:F3}°, plot angel: {3:F3}°",
                //    refIndex, refPlot->Angel[refIndex], refPlot->Angel[refIndex1], angel[plotIndex]));
            }
            // get search direktion
            double checkAngel = angel[plotIndex] - refPlot->Angel[refIndex];
            if (checkAngel < 0.0) checkAngel += 360.0;
            if ((0.0 <= checkAngel) && (checkAngel < 180.0))
            {
                forward = true;
            }
            else
            {
                forward = false;
            }
            Int32 refIndex1 = refIndex;
            //Debug::Print(String::Format(L"Plot angel[{0}]: {1:F3}, Ref idx: {2}, Forward: {3}",
            //                            plotIndex, angel[plotIndex], refIndex, forward));
            for (UInt32 i = 0; i < refPlot->ValueNumber; i ++)
            {
                // get upper index of reference plot interval
                if (forward == true)
                {
                    refIndex  = refIndex1;
                    refIndex1 ++;
                    if (refIndex1 >= Int32(refPlot->ValueNumber)) refIndex1 -= refPlot->ValueNumber;
                }
                else
                {
                    refIndex1 = refIndex;
                    refIndex  --;
                    if (refIndex < 0) refIndex += refPlot->ValueNumber;
                }
                // check if plot point is in reference plot interval
                if (refPlot->Angel[refIndex] <= refPlot->Angel[refIndex1])
                {
                    if (   (refPlot->Angel[refIndex] <= angel[plotIndex])
                        && (angel[plotIndex] < refPlot->Angel[refIndex1]))
                    {
                        //Debug::Print(String::Format(
                        //    L"Count(1): {0}, Plot[{1}]: {2:F3}, Ref[{3}]: {4:F3}, [{5}]: {6:F3}",
                        //    i,
                        //    plotIndex, angel[plotIndex],
                        //    refIndex,  refPlot->Angel[refIndex],
                        //    refIndex1, refPlot->Angel[refIndex1]));
                        return;
                    }
                }
                else
                {
                    if (   (refPlot->Angel[refIndex] <= angel[plotIndex])
                        || (angel[plotIndex] < refPlot->Angel[refIndex1]))
                    {
                        //Debug::Print(String::Format(
                        //    L"Count(2): {0}, Plot[{1}]: {2:F3}, Ref[{3}]: {4:F3}, [{5}]: {6:F3}",
                        //    i,
                        //    plotIndex, angel[plotIndex],
                        //    refIndex,  refPlot->Angel[refIndex],
                        //    refIndex1, refPlot->Angel[refIndex1]));
                        return;
                    }
                }
            }
            //Debug::Print(String::Format(L"Plot angel[{0}]: {1:F3}, Ref idx: {2}, Forward: {3}",
            //                            plotIndex, angel[plotIndex], refIndex, forward));
            // internal error: interval not found
            throw gcnew Exception(L"Internal error 24");
        }

        /// <summary>
        /// Get the radius from an angel on a line defined by two angel / radius pairs.
        /// </summary>
        /// <param name="plotAngel">The plot angel value.</param>
        /// <param name="refAngel0">The reference angel 0 value.</param>
        /// <param name="refRadius0">The reference radius 0 value.</param>
        /// <param name="refAngel1">The reference angel 1 value.</param>
        /// <param name="refRadius1">The reference radius 1 value.</param>
        Double GetRadiusFromAngel(Double plotAngel,
                                  Double refAngel0, Double refRadius0,
                                  Double refAngel1, Double refRadius1)
        {
            Double plotRadius;
            // check reference detection style
            switch(refStyle)
            {
            default: throw gcnew Exception(L"Internal error 23"); break;
            case RefStyles::Point:
                // calculate neerest angle between point 0 and point 1
                Double diff0, diff1;
                if (refAngel0 <= refAngel1)
                {
                    diff0 = plotAngel - refAngel0;
                    diff1 = refAngel1 - plotAngel;
                }
                else if (plotAngel >= refAngel0)
                {
                    diff0 = plotAngel         - refAngel0;
                    diff1 = refAngel1 + 360.0 - plotAngel;
                }
                else
                {
                    diff0 = plotAngel + 360.0 - refAngel0;
                    diff1 = refAngel1         - plotAngel;
                }
                if (diff0 <= diff1)
                {
                    plotRadius = refRadius0;
                }
                else
                {
                    plotRadius = refRadius1;
                }
                break;
            case RefStyles::Line:
                // calculate line between point 0 and point 1
                Double arc, gain;
                arc         = Math::PI * refAngel0 / 180.0;
                Double x0   = refRadius0 * Math::Cos(arc);
                Double y0   = refRadius0 * Math::Sin(arc);
                arc         = Math::PI * refAngel1 / 180.0;
                Double x1   = refRadius1 * Math::Cos(arc);
                Double y1   = refRadius1 * Math::Sin(arc);
                Double dx   = x1 - x0;
                Double dy   = y1 - y0;
                arc         = Math::PI * plotAngel / 180.0;
                if (Math::Abs(dx) < Math::Abs(dy))
                {
                    gain = dx / dy;
                    plotRadius =   (x0             - gain * y0)
                                 / (Math::Cos(arc) - gain * Math::Sin(arc));
                }
                else
                {
                    gain = dy / dx;
                    plotRadius =   (y0             - gain * x0)
                                 / (Math::Sin(arc) - gain * Math::Cos(arc));
                }
                break;
            }
            //Double plotRadius1 = (refRadius1 + refRadius0) / 2.0;
            //Debug::Print(String::Format(L"Ref radius 0: {0:F3}, radius 1: {1:F3}",
            //                            plotRadius, plotRadius1));
            return plotRadius;
        }

        /// <summary>
        /// Paint plot.
        /// </summary>
        /// <param name="graphic">The graphic object.</param>
        /// <param name="zOrder">The plot z order.</param>
        void PaintPlot(Graphics ^ graphic, UInt32 zOrder)
        {
            // check z order
            if (zOrder != this->zOrder)
            {
                return;
            }
            // paint plot
            Int32 j, k;
            switch (plotStyle)
            {
            default: throw gcnew Exception(L"Internal error 22"); break;
            case PlotStyles::Line:
                j = k = firstIndex;
                for (UInt32 i = 1; i < valueNumber; i ++)
                {
                    k = j + 1;
                    if (k >= Int32(bufferSize)) k -= bufferSize;
                    DrawPlotLine(graphic, j, k);
                    j = k;
                }
                if (   (valueNumber > 2)
                    && (closeFlag == true))
                {
                    DrawPlotLine(graphic, k, firstIndex);
                }
                if (valueNumber > 1)
                {
                    break;
                }
            case PlotStyles::Point:
                k = firstIndex;
                for (UInt32 i = 0; i < valueNumber; i ++)
                {
                    DrawPlotPoint(graphic, k);
                    k ++;
                    if (k >= Int32(bufferSize)) k -= bufferSize;
                }
                break;
            }
        }

        /// <summary>
        /// Draw a line of the plot.
        /// </summary>
        /// <param name="graphic">The graphic object.</param>
        /// <param name="lastIndex">The plot line last index.</param>
        /// <param name="firstIndex">The plot line first index.</param>
        virtual void DrawPlotLine(Graphics ^ graphic, Int32 lastIndex, Int32 firstIndex)
        {
            if (   (paintPen0[lastIndex]  != nullptr)
                && (paintPen0[firstIndex] != nullptr))
            {
                graphic->DrawLine(paintPen0[firstIndex],
                                  paintX0[lastIndex],  paintY0[lastIndex],
                                  paintX0[firstIndex], paintY0[firstIndex]);
            }
        }

        /// <summary>
        /// Draw a point of the plot.
        /// </summary>
        /// <param name="graphic">The graphic object.</param>
        /// <param name="index">The plot point index.</param>
        virtual void DrawPlotPoint(Graphics ^ graphic, Int32 index)
        {
            if (paintPen0[index] != nullptr)
            {
                graphic->DrawRectangle(paintPen0[index],
                                       paintX0[index], paintY0[index], 1, 1);
            }
        }
    };
    #pragma endregion

    #pragma region "ReferencePlot class definition"
    /// <summary>
    /// This class is used for the graphic display of measure data
    /// with a polar coordinate diagram based on .NET graphic.
    /// </summary>
    public ref class ReferencePlot : public PolarPlot
    {
    private:
        PaintArray ^    paintX1;        ///< x coordinates 1 for paint
        PaintArray ^    paintY1;        ///< Y coordinates 1 for paint
        PenArray ^      paintPen1;      ///< the pen 1 for paint

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="bufferSize">The plot buffer size.</param>
        /// <param name="tolerance">The reference tolerance value.</param>
        /// <param name="angleDistance">The plot angle distance in °.</param>
        /// <param name="closeFlag">If true close plot from end to start.</param>
        /// <param name="zOrder">The plot z order.</param>
        /// <param name="plotStyle">The plot style.</param>
        /// <param name="plotColor">The plot color.</param>
        ReferencePlot(UInt32        bufferSize,
                      Double        tolerance,
                      Double        angleDistance,
                      bool          closeFlag,
                      UInt32        zOrder,
                      PlotStyles    plotStyle,
                      Color         plotColor)
            : PolarPlot(bufferSize,
                        closeFlag, zOrder, plotStyle, plotColor,
                        nullptr, RefStyles::Point, plotColor)
        {
            this->tolerance     = tolerance;
            this->angleDistance = angleDistance;

            // get arrays for painting
            paintX1   = gcnew PaintArray(bufferSize);
            paintY1   = gcnew PaintArray(bufferSize);
            paintPen1 = gcnew PenArray(bufferSize);
        }

    protected:
		/// <summary>
		/// Destructor.
		/// </summary>
		~ReferencePlot()
		{
            Delete();
        }

		/// <summary>
		/// Finalizer.
		/// </summary>
		!ReferencePlot()
		{
            Delete();
		}

    private:
        /// <summary>
        /// Delete function.
        /// </summary>
        void Delete()
        {
            if (paintX1 != nullptr)
            {
                delete paintX1;
                paintX1 = nullptr;
            }
            if (paintY1 != nullptr)
            {
                delete paintY1;
                paintY1 = nullptr;
            }
            if (paintPen1 != nullptr)
            {
                delete paintPen1;
                paintPen1 = nullptr;
            }
        }

    public:
        /// <summary>
        /// Convert plot point from polar to cartesian coordinates.
        /// </summary>
        /// <param name="plotIndex">The plot index.</param>
        virtual void ConvertPoint(Int32 plotIndex) override
        {
            // convert point
            Double diff = tolerance * 1e-3;
            ConvertPoint(angel[plotIndex], radius[plotIndex] + diff,
                         paintX0[plotIndex], paintY0[plotIndex],
                         paintPen0[plotIndex]);
            ConvertPoint(angel[plotIndex], radius[plotIndex] - diff,
                         paintX1[plotIndex], paintY1[plotIndex],
                         paintPen1[plotIndex]);
        }

        /// <summary>
        /// Draw a line of the plot.
        /// </summary>
        /// <param name="graphic">The graphic object.</param>
        /// <param name="lastIndex">The plot line last index.</param>
        /// <param name="firstIndex">The plot line first index.</param>
        virtual void DrawPlotLine(Graphics ^ graphic, Int32 lastIndex, Int32 firstIndex) override
        {
            if (   (paintPen0[lastIndex]  != nullptr)
                && (paintPen0[firstIndex] != nullptr))
            {
                graphic->DrawLine(paintPen0[firstIndex],
                                  paintX0[lastIndex],  paintY0[lastIndex],
                                  paintX0[firstIndex], paintY0[firstIndex]);
            }
            if (   (paintPen1[lastIndex]  != nullptr)
                && (paintPen1[firstIndex] != nullptr))
            {
                graphic->DrawLine(paintPen1[firstIndex],
                                  paintX1[lastIndex],  paintY1[lastIndex],
                                  paintX1[firstIndex], paintY1[firstIndex]);
            }
        }

        /// <summary>
        /// Draw a point of the plot.
        /// </summary>
        /// <param name="graphic">The graphic object.</param>
        /// <param name="index">The plot point index.</param>
        virtual void DrawPlotPoint(Graphics ^ graphic, Int32 index) override
        {
            if (paintPen0[index] != nullptr)
            {
                graphic->DrawRectangle(paintPen0[index],
                                       paintX0[index], paintY0[index], 1, 1);
            }
            if (paintPen1[index] != nullptr)
            {
                graphic->DrawRectangle(paintPen1[index],
                                       paintX1[index], paintY1[index], 1, 1);
            }
        }
    };
    #pragma endregion

    #pragma region "PolarGraph class definition"
    /// max polar graph z order
    #define POLAR_GRAPH_MAX_Z_ORDER     UInt32( 2)

    /// <summary>
    /// This class is used for the graphic display of measure data
    /// with a polar coordinate diagram based on .NET graphic.
    /// </summary>
    public ref class PolarGraph
    {
    /// list of polar coordinate plots for this graphic
    typedef Dictionary<UInt32, PolarPlot ^>     PlotList;

    private:
        GUI_Support ^       guiSupport;         ///< the GUI support class
        ResourceManager ^   resManager;         ///< the resource manager from main window

        Panel ^         panelGraph;     ///< graphic control
        Graphics ^      graphic;        ///< graphic object
        Label ^         labelXmin;      ///< the Xmin label
        Label ^         labelXmax;      ///< the Xmax label
        Label ^         labelYmin;      ///< the Ymin label
        Label ^         labelYmax;      ///< the Ymin label
        UInt32          measurePoint;   ///< the measure point to display index

        UInt32          graphAxisMax;   ///< the graphic axis max value

        UInt32          plotHandle;     ///< the plot handle
        PlotList ^      plotList;       ///< the plot list

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="guiSupport">The GUI support class.</param>
        /// <param name="panelGraph">The graphic panel.</param>
        /// <param name="labelXmin">The Xmin label.</param>
        /// <param name="labelXmax">The Xmax label.</param>
        /// <param name="labelYmin">The Ymin label.</param>
        /// <param name="labelYmax">The Ymin label.</param>
        /// <param name="measurePoint">The measure point to display index.</param>
        /// <param name="graphAxisMax">The graphic axis max value.</param>
        PolarGraph(GUI_Support ^    guiSupport,
                   Panel ^          panelGraph,
                   Label ^          labelXmin,
                   Label ^          labelXmax,
                   Label ^          labelYmin,
                   Label ^          labelYmax,
                   UInt32           measurePoint,
                   UInt32           graphAxisMax)
        {
            this->guiSupport = guiSupport;
            this->resManager = guiSupport->ResManager;

            this->panelGraph = panelGraph;
            graphic = panelGraph->CreateGraphics();

            this->labelXmin = labelXmin;
            this->labelXmax = labelXmax;
            this->labelYmin = labelYmin;
            this->labelYmax = labelYmax;

            this->measurePoint = measurePoint;

            this->graphAxisMax = graphAxisMax;

            plotHandle = 1;
            plotList   = gcnew PlotList;
        }

        /// <summary>
        /// Sets the max value for graph axis in mmm.
        /// </summary>
        property UInt32 GraphAxisMax
        {
            void set(UInt32 value)
            {
                graphAxisMax = value;
                // display new value
                labelXmin->Text = String::Format(L"-{0}mm", graphAxisMax);
                labelXmax->Text = String::Format(L" {0}mm", graphAxisMax);
                labelYmin->Text = String::Format(L"-{0}mm", graphAxisMax);
                labelYmax->Text = String::Format(L" {0}mm", graphAxisMax);
                // convert all plots
                for each (PolarPlot ^ plot in plotList->Values)
                {
                    plot->ConvertPlot(graphAxisMax, panelGraph->Width, panelGraph->Height);
                }
            }
        }

        /// <summary>
        /// Gets the plot from handle.
        /// </summary>
        property PolarPlot ^ PlotFromHandle[UInt32]
        {
            PolarPlot ^ get(UInt32 handle) { return plotList[handle]; }
        }

        /// <summary>
        /// Add a new plot to the graph.
        /// </summary>
        /// <param name="plot">The plot to add.</param>
        /// <returns>The plot handle.</returns>
        UInt32 AddPlot(PolarPlot ^ plot)
        {
            UInt32 handle = (plotHandle << 2) + measurePoint;
            // convert plot
            plot->ConvertPlot(graphAxisMax, panelGraph->Width, panelGraph->Height);
            // add plot to list
            plotList[handle] = plot;
            // increment plot handle
            plotHandle ++;
            // paint graph
            panelGraph->Invalidate();
            //Debug::Print(String::Format(L"Point {0}: add plot handle: {1}",
            //                            measurePoint, handle));
            return handle;
        }

        /// <summary>
        /// Delete a plot from the graph.
        /// </summary>
        /// <param name="handle">The handle of the plot to remove.</param>
        void DeletePlot(UInt32 handle)
        {
            delete plotList[handle];
            plotList->Remove(handle);
            // paint graph
            panelGraph->Invalidate();
            //Debug::Print(String::Format(L"Point {0}: delete plot handle: {1}",
            //                            measurePoint, handle));
        }

        /// <summary>
        /// Refresh a plot from the graph.
        /// </summary>
        /// <param name="handle">The handle of the plot to remove.</param>
        void RefreshPlot(UInt32 handle)
        {
            plotList[handle]->ConvertPlot(graphAxisMax, panelGraph->Width, panelGraph->Height);
            // paint graph
            panelGraph->Invalidate();
            //Debug::Print(String::Format(L"Point {0}: refresh plot handle: {1}",
            //                            measurePoint, handle));
        }

        /// <summary>
        /// Refresh the graph.
        /// </summary>
        void RefreshGraph()
        {
            // paint graph
            panelGraph->Invalidate();
        }

        /// <summary>
        /// Paint the graphic display.
        /// </summary>
        void PaintGraph()
        {
            for (UInt32 i = 0; i < POLAR_GRAPH_MAX_Z_ORDER; i ++)
            {
                for each (PolarPlot ^ plot in plotList->Values)
                {
                    plot->PaintPlot(graphic, i);
                }
            }
        }
    };
    #pragma endregion
}

