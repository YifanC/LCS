///
/// \file       PropertyDisplay.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyDisplay.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: PropertyDisplay.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB display parameters input class.
///
/// This class is used for the management of the EIB display
/// parameters input via the display options property grid. It manages
/// the parameter dependencies for the property grid.
///

#pragma once

#include "FormMain.h"

using namespace System;

namespace EIB7xx_App
{
 	#pragma region "DisplayPeriodConverter class definition"
	/// <summary>
	/// This class definies a type converter for a display period value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class DisplayPeriodConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        DisplayPeriodConverter() : UInt32ValueConverter(EIB_MIN_DISPLAY_PERIOD,
                                                        EIB_MAX_DISPLAY_PERIOD,
                                                        EIB_STEP_DISPLAY_PERIOD)
        {
        }
    };
	#pragma endregion

 	#pragma region "DisplayPrecisionConverter class definition"
	/// <summary>
	/// This class definies a type converter for an UInt32 enum class.
    /// enum <--> String.
	/// </summary>
    public ref class DisplayPrecisionConverter : public NumericEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        DisplayPrecisionConverter() : NumericEnumConverter(EIB7_PREC_NUM::typeid)
        {
        }

        /// <summary>
        /// This function performs the convertion enum numeric value --> String.
        /// </summary>
        virtual String ^ EnumNumericValueToString(Object ^ value) override
        {
            return UInt32((EIB7_PREC_NUM)value).ToString();
        }
    };
	#pragma endregion

 	#pragma region "SaveMeasureDataConverter class definition"
	/// <summary>
	/// This class definies a type converter for a save measure data selection.
    /// bool <--> String.
	/// </summary>
    public ref class SaveMeasureDataConverter : public BoolSelectConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        SaveMeasureDataConverter() : BoolSelectConverter(L"textSaveMeasureDataYes",
                                                         L"textSaveMeasureDataNo")
        {
        }
    };
	#pragma endregion

 	#pragma region "ShowMultiturnConverter class definition"
	/// <summary>
	/// This class definies a type converter for a save measure data selection.
    /// bool <--> String.
	/// </summary>
    public ref class ShowMultiturnConverter : public BoolSelectConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        ShowMultiturnConverter() : BoolSelectConverter(L"textShowMultiturnYes",
                                                       L"textShowMultiturnNo")
        {
        }
    };
	#pragma endregion

 	#pragma region "AngleDistanceConverter class definition"
	/// <summary>
	/// This class definies a type converter for a angle distance value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class AngleDistanceConverter : public DoubleValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AngleDistanceConverter() : DoubleValueConverter(CRANKSHAFT_MIN_ANGLE_DISTANCE,
                                                        CRANKSHAFT_MAX_ANGLE_DISTANCE,
                                                        CRANKSHAFT_STEP_ANGLE_DISTANCE,
                                                        CRANKSHAFT_FORMAT_ANGLE_DISTANCE)
        {
        }
    };
	#pragma endregion

 	#pragma region "RadiusOffsetConverter class definition"
	/// <summary>
	/// This class definies a type converter for a radius offset value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class RadiusOffsetConverter : public DoubleValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        RadiusOffsetConverter() : DoubleValueConverter(CRANKSHAFT_MIN_RADIUS_OFFSET,
                                                       CRANKSHAFT_MAX_RADIUS_OFFSET,
                                                       CRANKSHAFT_STEP_RADIUS_OFFSET,
                                                       CRANKSHAFT_FORMAT_RADIUS_OFFSET)
        {
        }
    };
	#pragma endregion

 	#pragma region "GraphAxisMaxConverter class definition"
	/// <summary>
	/// This class definies a type converter for a graph axis max value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class GraphAxisMaxConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        GraphAxisMaxConverter() : UInt32ValueConverter(CRANKSHAFT_MIN_AXIS_MAX,
                                                       CRANKSHAFT_MAX_AXIS_MAX,
                                                       CRANKSHAFT_STEP_AXIS_MAX)
        {
        }
    };
	#pragma endregion

 	#pragma region "ToleranceConverter class definition"
	/// <summary>
	/// This class definies a type converter for a tolerance value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class ToleranceConverter : public DoubleValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        ToleranceConverter() : DoubleValueConverter(CRANKSHAFT_MIN_TOLERANCE,
                                                    CRANKSHAFT_MAX_TOLERANCE,
                                                    CRANKSHAFT_STEP_TOLERANCE,
                                                    CRANKSHAFT_FORMAT_TOLERANCE)
        {
        }
    };
	#pragma endregion

 	#pragma region "PropertyDisplay class definition"
    /// <summary>
    /// This class is used for the management of the EIB display
    /// parameters input via the display options property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropertyDisplay : public PropertyBase
    {
    private:
        EIB_TreeList ^      eibTreeList;    ///< the EIB tree list

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="eibTreeList">The EIB tree list.</param>
        PropertyDisplay(GUI_Support ^  guiSupport,
                        EIB_TreeList ^ eibTreeList)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridDisplay)
        {
            this->eibTreeList = eibTreeList;

            // create category strings
            String ^ categoryDisplay    = L"";
            String ^ categoryCrankshaft = L"";
            if (guiSupport->GuiConfig->GuiParam->AppCrankshaft == false)
            {
                categoryDisplay = resManager->GetString(L"categoryDisplay");
            }
            else
            {
                categoryDisplay =
                                String::Format(CATEGORY_HEADER,
                                               1, resManager->GetString(L"categoryDisplay"));
                categoryCrankshaft =
                                String::Format(CATEGORY_HEADER,
                                               2, resManager->GetString(L"categoryCrankshaftDisplay"));
            }
            // create the property list for the input parameters
            Properties->Add(gcnew PropertyEntry(L"DisplayPeriod", UInt32::typeid,
                                                categoryDisplay,
                                                resManager->GetString(L"textDisplayPeriod"),
                                                resManager->GetString(L"descriptionDisplayPeriod"),
                                                EIB_DEFAULT_DISPLAY_PERIOD,
                                                DisplayPeriodConverter::typeid));
            Properties->Add(gcnew PropertyEntry(L"DisplayPrecision", EIB7_PREC_NUM::typeid,
                                                categoryDisplay,
                                                resManager->GetString(L"textDisplayPrecision"),
                                                resManager->GetString(L"descriptionDisplayPrecision"),
                                                EIB_DEFAULT_DISPLAY_PRECISION,
                                                DisplayPrecisionConverter::typeid));
            Properties->Add(gcnew PropertyEntry(L"SaveMeasureData", bool::typeid,
                                                categoryDisplay,
                                                resManager->GetString(L"textSaveMeasureData"),
                                                resManager->GetString(L"descriptionSaveMeasureData"),
                                                GUI_DEFAULT_SAVE_MEASURE_DATA,
                                                SaveMeasureDataConverter::typeid));
            Properties->Add(gcnew PropertyEntry(L"ShowMultiturn", bool::typeid,
                                                categoryDisplay,
                                                resManager->GetString(L"textShowMultiturn"),
                                                resManager->GetString(L"descriptionShowMultiturn"),
                                                GUI_DEFAULT_SHOW_MULTITURN,
                                                ShowMultiturnConverter::typeid));

            if (guiSupport->GuiConfig->GuiParam->AppCrankshaft == true)
            {
                Properties->Add(gcnew PropertyEntry(L"AngleDistance", Double::typeid,
                                                    categoryCrankshaft,
                                                    resManager->GetString(L"textAngleDistance"),
                                                    resManager->GetString(L"descriptionAngleDistance"),
                                                    CRANKSHAFT_DEFAULT_ANGLE_DISTANCE,
                                                    AngleDistanceConverter::typeid));
                Properties->Add(gcnew PropertyEntry(L"RadiusOffset", Double::typeid,
                                                    categoryCrankshaft,
                                                    resManager->GetString(L"textRadiusOffset"),
                                                    resManager->GetString(L"descriptionRadiusOffset"),
                                                    CRANKSHAFT_DEFAULT_RADIUS_OFFSET,
                                                    RadiusOffsetConverter::typeid));
                Properties->Add(gcnew PropertyEntry(L"GraphAxisMax", UInt32::typeid,
                                                    categoryCrankshaft,
                                                    resManager->GetString(L"textGraphAxisMax"),
                                                    resManager->GetString(L"descriptionGraphAxisMax"),
                                                    CRANKSHAFT_DEFAULT_AXIS_MAX,
                                                    GraphAxisMaxConverter::typeid));
                Properties->Add(gcnew PropertyEntry(L"Tolerance", Double::typeid,
                                                    categoryCrankshaft,
                                                    resManager->GetString(L"textTolerance"),
                                                    resManager->GetString(L"descriptionTolerance"),
                                                    CRANKSHAFT_DEFAULT_TOLERANCE,
                                                    ToleranceConverter::typeid));
            }

            // refresh the property grid
            RefreshParam();
        }

        /// <summary>
		/// Set the parameter class depending from parameter.
		/// </summary>
		void SetParamClass(PropertyEntryEventArgs ^ e)
		{
            if (   (e->Property->Name == L"SaveMeasureData")
                || (e->Property->Name == L"ShowMultiturn"))
            {
                paramClass = guiSupport->GuiConfig->GuiParam;
            }
            else if (   (e->Property->Name == L"AngleDistance")
                     || (e->Property->Name == L"RadiusOffset")
                     || (e->Property->Name == L"GraphAxisMax")
                     || (e->Property->Name == L"Tolerance"))
            {
                paramClass = eibTreeList->Common->CrankshaftParam;
            }
            else
            {
                paramClass = eibTreeList->Common->Display;
            }
        }

        /// <summary>
		/// This member overrides PropertyClass.OnGetValue.
		/// </summary>
		virtual void OnGetValue(PropertyEntryEventArgs ^ e) override
		{
            // set parameter class depending from parameter
            SetParamClass(e);

            PropertyBase::OnGetValue(e);
		}

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            // set parameter class depending from parameter
            SetParamClass(e);

            PropertyBase::OnSetValue(e);
		}
    };
	#pragma endregion
}
