///
/// \file       PropertyAxis.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyAxis.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: PropertyAxis.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB axis parameters input class.
///
/// This class is used for the management of the EIB axis
/// parameters input via the axis property grid. It manages
/// the parameter dependencies for the property grid.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Resources;
using namespace System::Reflection;
using namespace System::Windows::Forms;

namespace EIB7xx_App
{
 	#pragma region "InterfaceTypeConverter class definition"
	/// <summary>
	/// This class definies a type converter for an interface type enum.
    /// enum <--> String.
	/// </summary>
    public ref class InterfaceTypeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        InterfaceTypeConverter() : LocalEnumConverter(EIB7_INTERFACE_TYPE_GUI::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "Bandwidth1VConverter class definition"
	/// <summary>
	/// This class definies a type converter for an bandwidth enum.
    /// enum <--> String.
	/// </summary>
    public ref class Bandwidth1VConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        Bandwidth1VConverter() : LocalEnumConverter(EIB7_BANDWIDTH::typeid, L"_1V")
        {
        }
    };
	#pragma endregion

 	#pragma region "Bandwidth11myConverter class definition"
	/// <summary>
	/// This class definies a type converter for an bandwidth enum.
    /// enum <--> String.
	/// </summary>
    public ref class Bandwidth11myConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        Bandwidth11myConverter() : LocalEnumConverter(EIB7_BANDWIDTH::typeid, L"_11my")
        {
        }
    };
	#pragma endregion

 	#pragma region "CompensationConverter class definition"
	/// <summary>
	/// This class definies a type converter for an compensation enum.
    /// enum <--> String.
	/// </summary>
    public ref class CompensationConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        CompensationConverter() : LocalEnumConverter(EIB7_COMPENSATION::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "AxisTerminationConverter class definition"
	/// <summary>
	/// This class definies a type converter for an axis termination enum.
    /// enum <--> String.
	/// </summary>
    public ref class AxisTerminationConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AxisTerminationConverter() : LocalEnumConverter(EIB7_ENABLE_MODE::typeid, L"_AxisTermination")
        {
        }
    };
	#pragma endregion

 	#pragma region "RefmarksConverter class definition"
	/// <summary>
	/// This class definies a type converter for an refmarks enum.
    /// enum <--> String.
	/// </summary>
    public ref class RefmarksConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        RefmarksConverter() : LocalEnumConverter(EIB7_REFMARKS::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "EncoderTypeConverter class definition"
	/// <summary>
	/// This class definies a type converter for an encoder type enum.
    /// enum <--> String.
	/// </summary>
    public ref class EncoderTypeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EncoderTypeConverter() : LocalEnumConverter(EIB7_ENCODER_TYPE::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "LinecountsConverter class definition"
	/// <summary>
	/// This class definies a type converter for a linecounts value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class LinecountsConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        LinecountsConverter() : UInt32ValueConverter(EIB_MIN_LINECOUNTS,
                                                     EIB_MAX_LINECOUNTS,
                                                     EIB_STEP_LINECOUNTS)
        {
        }
    };
	#pragma endregion

 	#pragma region "IncrementConverter class definition"
	/// <summary>
	/// This class definies a type converter for a increment value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class IncrementConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        IncrementConverter() : UInt32ValueConverter(EIB_MIN_INCREMENT,
                                                    EIB_MAX_INCREMENT,
                                                    EIB_STEP_INCREMENT)
        {
        }
    };
	#pragma endregion

 	#pragma region "SignalPeriodConverter class definition"
	/// <summary>
	/// This class definies a type converter for a signal period value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class SignalPeriodConverter : public DoubleValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        SignalPeriodConverter() : DoubleValueConverter(EIB_MIN_SIGNAL_PERIOD,
                                                       EIB_MAX_SIGNAL_PERIOD,
                                                       EIB_STEP_SIGNAL_PERIOD,
                                                       EIB_FORMAT_SIGNAL_PERIOD)
        {
        }
    };
	#pragma endregion

 	#pragma region "IncSignalsConverter class definition"
	/// <summary>
	/// This class definies a type converter for a use incremental signals selection.
    /// bool <--> String.
	/// </summary>
    public ref class IncSignalsConverter : public BoolSelectConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        IncSignalsConverter() : BoolSelectConverter(L"textIncSignalsOn",
                                                    L"textIncSignalsOff")
        {
        }
    };
	#pragma endregion

 	#pragma region "RuntimeCompConverter class definition"
	/// <summary>
	/// This class definies a type converter for a use runtime compensation selection.
    /// bool <--> String.
	/// </summary>
    public ref class RuntimeCompConverter : public BoolSelectConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        RuntimeCompConverter() : BoolSelectConverter(L"textRuntimeCompOn",
                                                     L"textRuntimeCompOff")
        {
        }
    };
	#pragma endregion

 	#pragma region "EnDatClock21Converter class definition"
	/// <summary>
	/// This class definies a type converter for an EnDat clock enum.
    /// enum <--> String.
	/// </summary>
    public ref class EnDatClock21Converter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EnDatClock21Converter() : LocalEnumConverter(EIB7_INTERFACE_CLOCK_21::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "EnDatClock22Converter class definition"
	/// <summary>
	/// This class definies a type converter for an EnDat clock enum.
    /// enum <--> String.
	/// </summary>
    public ref class EnDatClock22Converter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EnDatClock22Converter() : LocalEnumConverter(EIB7_INTERFACE_CLOCK_22::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "RecoveryConverter class definition"
	/// <summary>
	/// This class definies a type converter for a recovery time enum.
    /// enum <--> String.
	/// </summary>
    public ref class RecoveryConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        RecoveryConverter() : LocalEnumConverter(EIB7_RECOVERY_TIME::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "CalculationConverter class definition"
	/// <summary>
	/// This class definies a type converter for a calculation time enum.
    /// enum <--> String.
	/// </summary>
    public ref class CalculationConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        CalculationConverter() : LocalEnumConverter(EIB7_ENDAT_CALC_TIME::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "AddInfoModeConverter class definition"
	/// <summary>
	/// This class definies a type converter for an additional information mode enum.
    /// enum <--> String.
	/// </summary>
    public ref class AddInfoModeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AddInfoModeConverter() : LocalEnumConverter(EIB7_ADD_INFO_MODE::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "AddInfo1Converter class definition"
	/// <summary>
	/// This class definies a type converter for an additional information 1 enum.
    /// enum <--> String.
	/// </summary>
    public ref class AddInfo1Converter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AddInfo1Converter() : LocalEnumConverter(EIB7_ADD_INFO_1::typeid)
        {
        }

        /// <summary>
        /// This function returns the prefix for the value key strings.
        /// </summary>
        virtual String ^ GetPrefix(Object ^ value) override
        {
            return String::Format(L"0x{0,2:X2}: ", UInt32((EIB7_ADD_INFO_1)value));
        }
    };
	#pragma endregion

 	#pragma region "AddInfo2Converter class definition"
	/// <summary>
	/// This class definies a type converter for an additional information 2 enum.
    /// enum <--> String.
	/// </summary>
    public ref class AddInfo2Converter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AddInfo2Converter() : LocalEnumConverter(EIB7_ADD_INFO_2::typeid)
        {
        }

        /// <summary>
        /// This function returns the prefix for the value key strings.
        /// </summary>
        virtual String ^ GetPrefix(Object ^ value) override
        {
            return String::Format(L"0x{0,2:X2}: ", UInt32((EIB7_ADD_INFO_2)value));
        }
    };
	#pragma endregion

 	#pragma region "AddInfoNumConverter class definition"
	/// <summary>
	/// This class definies a type converter for an UInt32 enum class.
    /// enum <--> String.
	/// </summary>
    public ref class AddInfoNumConverter : public NumericEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AddInfoNumConverter() : NumericEnumConverter(EIB7_ADD_INFO_NUM::typeid)
        {
        }

        /// <summary>
        /// This function performs the convertion enum numeric value --> String.
        /// </summary>
        virtual String ^ EnumNumericValueToString(Object ^ value) override
        {
            return UInt32((EIB7_ADD_INFO_NUM)value).ToString();
        }
    };
	#pragma endregion

 	#pragma region "AddInfoFifoConverter class definition"
	/// <summary>
	/// This class definies a type converter for an additional information all enum.
    /// enum <--> String.
	/// </summary>
    public ref class AddInfoFifoConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AddInfoFifoConverter() : LocalEnumConverter(EIB7_ADD_INFO_ALL::typeid)
        {
        }

        /// <summary>
        /// This function returns the prefix for the value key strings.
        /// </summary>
        virtual String ^ GetPrefix(Object ^ value) override
        {
            return String::Format(L"0x{0,2:X2}: ", UInt32((EIB7_ADD_INFO_ALL)value));
        }
    };
	#pragma endregion

 	#pragma region "PropertyAxis class definition"
    // additional information fifo list
    typedef array<PropertyEntry ^>      AddInfoFifoList;

    /// <summary>
    /// This class is used for the management of the EIB axis
    /// parameters input via the axis property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropertyAxis : public PropertyAxisBase
    {
    private:
        EIB_Param ^                 parameter;          ///< parameters of the saved EIB
        EIB_Axis_Param ^            axisParameter;      ///< axis parameters of the saved EIB

        PropertyEntry ^             peInterfaceType;    ///< the property entry for interface type
        PropertyEntryCollection ^   pcIncrementOpt;     ///< the property list for incremental options
        PropertyEntry ^             peLinecounts;       ///< the property entry for linecounts
        PropertyEntry ^             peIncrement;        ///< the property entry for increment
        PropertyEntry ^             peSignalPeriod;     ///< the property entry for signal period

        PropertyEntryCollection ^   pcEnDat21Opt;       ///< the property list for EnDat 2.1 options
        PropertyEntryCollection ^   pcEnDat22Opt;       ///< the property list for EnDat 2.2 options

        PropertyEntryCollection ^   pcAddInfoFix;       ///< the property list for fix additional information
        PropertyEntry ^             peAddInfoNum;       ///< the property entry for additional info fifo number
        AddInfoFifoList ^           peAddInfoFifo;      ///< the property entry list for additional info fifos

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="parameter">The parameters of the saved EIB.</param>
		/// <param name="axisParameter">The axis parameters of the saved EIB.</param>
        PropertyAxis(GUI_Support ^    guiSupport,
                     EIB_Param ^      parameter,
                     EIB_Axis_Param ^ axisParameter)
            : PropertyAxisBase(guiSupport,
                               axisParameter,
                               axisParameter,
                               3,
                               false)
        {
            this->parameter     = parameter;
            this->axisParameter = axisParameter;

            currentState = CURRENT_STATE_INIT;
            // create category strings
            String ^ categoryInterfaceParameters =
                                String::Format(CATEGORY_HEADER,
                                               1, resManager->GetString(L"categoryInterfaceParameters"));
            String ^ categoryIncrementalOptions =
                                String::Format(CATEGORY_HEADER,
                                               2, resManager->GetString(L"categoryIncrementalOptions"));
            String ^ categoryEnDatOptions =
                                String::Format(CATEGORY_HEADER,
                                               2, resManager->GetString(L"categoryEnDatOptions"));
            // create the property list for axis parameters
            // create the property entry for interface type
            peInterfaceType = gcnew PropertyEntry
                                            (L"InterfaceTypeGui", EIB7_INTERFACE_TYPE_GUI::typeid,
                                             categoryInterfaceParameters,
                                             resManager->GetString(L"textInterface"),
                                             resManager->GetString(L"descriptionInterface"),
                                             EIB_DEFAULT_INTERFACE_TYPE_GUI,
                                             InterfaceTypeConverter::typeid);
            // create the property list for incremental options
            pcIncrementOpt = gcnew PropertyEntryCollection();
            pcIncrementOpt->Add(gcnew PropertyEntry
                                            (L"Bandwidth", EIB7_BANDWIDTH::typeid,
                                             categoryIncrementalOptions,
                                             resManager->GetString(L"textBandwidth"),
                                             resManager->GetString(L"descriptionBandwidth"),
                                             EIB_DEFAULT_BANDWIDTH_1V,
                                             Bandwidth1VConverter::typeid));
            pcIncrementOpt->Add(gcnew PropertyEntry
                                            (L"Compensation", EIB7_COMPENSATION::typeid,
                                             categoryIncrementalOptions,
                                             resManager->GetString(L"textCompensation"),
                                             resManager->GetString(L"descriptionCompensation"),
                                             EIB_DEFAULT_COMPENSATION,
                                             CompensationConverter::typeid));
            pcIncrementOpt->Add(gcnew PropertyEntry
                                            (L"AxisTermination", EIB7_ENABLE_MODE::typeid,
                                             categoryIncrementalOptions,
                                             resManager->GetString(L"textAxisTermination"),
                                             resManager->GetString(L"descriptionAxisTermination"),
                                             EIB_DEFAULT_AXIS_TERMINATION,
                                             AxisTerminationConverter::typeid));
            pcIncrementOpt->Add(gcnew PropertyEntry
                                            (L"Refmarks", EIB7_REFMARKS::typeid,
                                             categoryIncrementalOptions,
                                             resManager->GetString(L"textRefmarks"),
                                             resManager->GetString(L"descriptionRefmarks"),
                                             EIB_DEFAULT_REFMARKS,
                                             RefmarksConverter::typeid));
            pcIncrementOpt->Add(gcnew PropertyEntry
                                            (L"EncoderType", EIB7_ENCODER_TYPE::typeid,
                                             categoryIncrementalOptions,
                                             resManager->GetString(L"textEncoderType"),
                                             resManager->GetString(L"descriptionEncoderType"),
                                             EIB_DEFAULT_ENCODER_TYPE,
                                             EncoderTypeConverter::typeid));
            // create the property entry for linecounts
            peLinecounts = gcnew PropertyEntry
                                            (L"Linecounts", UInt32::typeid,
                                             categoryIncrementalOptions,
                                             resManager->GetString(L"textLinecounts"),
                                             resManager->GetString(L"descriptionLinecounts"),
                                             EIB_DEFAULT_LINECOUNTS,
                                             LinecountsConverter::typeid);
            // create the property entry for increment
            peIncrement = gcnew PropertyEntry
                                            (L"Increment", UInt32::typeid,
                                             categoryIncrementalOptions,
                                             resManager->GetString(L"textIncrement"),
                                             resManager->GetString(L"descriptionIncrement"),
                                             EIB_DEFAULT_INCREMENT,
                                             IncrementConverter::typeid);
            // create the property entry for signal period
            peSignalPeriod = gcnew PropertyEntry
                                            (L"SignalPeriod", Double::typeid,
                                             categoryIncrementalOptions,
                                             resManager->GetString(L"textSignalPeriod"),
                                             resManager->GetString(L"descriptionSignalPeriod"),
                                             EIB_DEFAULT_SIGNAL_PERIOD,
                                             SignalPeriodConverter::typeid);
            // create the property list for EnDat 2.1 and 2.2 options
            pcEnDat21Opt = gcnew PropertyEntryCollection();
            pcEnDat22Opt = gcnew PropertyEntryCollection();
            pcEnDat21Opt->Add(gcnew PropertyEntry
                                            (L"IncSignals", bool::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textIncSignals"),
                                             resManager->GetString(L"descriptionIncSignals"),
                                             EIB_DEFAULT_INC_SIGNALS_21,
                                             IncSignalsConverter::typeid));
            pcEnDat22Opt->Add(gcnew PropertyEntry
                                            (L"IncSignals", bool::typeid, true,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textIncSignals"),
                                             resManager->GetString(L"descriptionIncSignals"),
                                             EIB_DEFAULT_INC_SIGNALS_22,
                                             IncSignalsConverter::typeid));
            pcEnDat21Opt->Add(gcnew PropertyEntry
                                            (L"EnDatClock21", EIB7_INTERFACE_CLOCK_21::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textEnDatClock"),
                                             resManager->GetString(L"descriptionEnDatClock"),
                                             EIB_DEFAULT_INTERFACE_CLOCK_21,
                                             EnDatClock21Converter::typeid));
            pcEnDat22Opt->Add(gcnew PropertyEntry
                                            (L"EnDatClock22", EIB7_INTERFACE_CLOCK_22::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textEnDatClock"),
                                             resManager->GetString(L"descriptionEnDatClock"),
                                             EIB_DEFAULT_INTERFACE_CLOCK_22,
                                             EnDatClock22Converter::typeid));
            pcEnDat21Opt->Add(gcnew PropertyEntry
                                            (L"RuntimeComp", bool::typeid, true,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textRuntimeComp"),
                                             resManager->GetString(L"descriptionRuntimeComp"),
                                             EIB_DEFAULT_RUNTIME_COMP_21,
                                             RuntimeCompConverter::typeid));
            pcEnDat22Opt->Add(gcnew PropertyEntry
                                            (L"RuntimeComp", bool::typeid, true,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textRuntimeComp"),
                                             resManager->GetString(L"descriptionRuntimeComp"),
                                             EIB_DEFAULT_RUNTIME_COMP_22,
                                             RuntimeCompConverter::typeid));
            pcEnDat21Opt->Add(gcnew PropertyEntry
                                            (L"Recovery", EIB7_RECOVERY_TIME::typeid, true,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textRecovery"),
                                             resManager->GetString(L"descriptionRecovery"),
                                             EIB_DEFAULT_RECOVERY_TIME,
                                             RecoveryConverter::typeid));
            pcEnDat22Opt->Add(gcnew PropertyEntry
                                            (L"Recovery", EIB7_RECOVERY_TIME::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textRecovery"),
                                             resManager->GetString(L"descriptionRecovery"),
                                             EIB_DEFAULT_RECOVERY_TIME,
                                             RecoveryConverter::typeid));
            pcEnDat21Opt->Add(gcnew PropertyEntry
                                            (L"Calculation", EIB7_ENDAT_CALC_TIME::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textCalculation"),
                                             resManager->GetString(L"descriptionCalculation"),
                                             EIB_DEFAULT_ENDAT_CALC_TIME_21,
                                             CalculationConverter::typeid));
            pcEnDat22Opt->Add(gcnew PropertyEntry
                                            (L"Calculation", EIB7_ENDAT_CALC_TIME::typeid, true,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textCalculation"),
                                             resManager->GetString(L"descriptionCalculation"),
                                             EIB_DEFAULT_ENDAT_CALC_TIME_22,
                                             CalculationConverter::typeid));
            pcEnDat22Opt->Add(gcnew PropertyEntry
                                            (L"AddInfoMode", EIB7_ADD_INFO_MODE::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textAddInfoMode"),
                                             resManager->GetString(L"descriptionAddInfoMode"),
                                             EIB_DEFAULT_ADD_INFO_MODE,
                                             AddInfoModeConverter::typeid));
            // create the property list for fix additional information
            pcAddInfoFix = gcnew PropertyEntryCollection();
            pcAddInfoFix->Add(gcnew PropertyEntry
                                            (L"AddInfo1", EIB7_ADD_INFO_1::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textAddInfo1"),
                                             resManager->GetString(L"descriptionAddInfo1"),
                                             EIB_DEFAULT_ADD_INFO_1,
                                             AddInfo1Converter::typeid));
            pcAddInfoFix->Add(gcnew PropertyEntry
                                            (L"AddInfo2", EIB7_ADD_INFO_2::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textAddInfo2"),
                                             resManager->GetString(L"descriptionAddInfo2"),
                                             EIB_DEFAULT_ADD_INFO_2,
                                             AddInfo2Converter::typeid));
            // create the property entry for additional info fifo number
            peAddInfoNum = gcnew PropertyEntry
                                            (L"AddInfoNum", EIB7_ADD_INFO_NUM::typeid,
                                             categoryEnDatOptions,
                                             resManager->GetString(L"textAddInfoNum"),
                                             resManager->GetString(L"descriptionAddInfoNum"),
                                             EIB_DEFAULT_ADD_INFO_NUM,
                                             AddInfoNumConverter::typeid);
            // create the property entry list for additional info fifo values
            peAddInfoFifo = gcnew AddInfoFifoList(EIB_MAX_ADD_INFO_NUM);
            for (UInt32 i = 0; i < EIB_MAX_ADD_INFO_NUM; i ++)
            {
                IndexList ^ indexList = gcnew IndexList(1);
                indexList[0] = i;
                String ^ text =        String::Format(resManager->GetString(L"textAddInfoFifo"),        i + 1);
                String ^ description = String::Format(resManager->GetString(L"descriptionAddInfoFifo"), i + 1);
                peAddInfoFifo[i] = gcnew PropertyEntry
                                                (L"AddInfoFifo", indexList, EIB7_ADD_INFO_ALL::typeid,
                                                 categoryEnDatOptions,
                                                 text,
                                                 description,
                                                 EIB_DEFAULT_ADD_INFO_FIFO,
                                                 AddInfoFifoConverter::typeid);
            }
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            // set new input state
            SetInputState(axisParameter->IsEnabled);
            // don't change parameters on first call
            if (currentState == CURRENT_STATE_INIT)
            {
                currentState |= inputState & ~PROPERTY_AXIS_STATE_INIT_READY;
            }
            //GuiSupport->ShowDebugText(String::Format(L"Current: 0x{0,8:X8}, Input: 0x{1,8:X8}\n",
            //                                         currentState, inputState));
            // check current state
            if (currentState != inputState)
            {
                UInt32 changeMask;
                // set axis property list
                Properties->Clear();
                // add interface type
                Properties->Add(peInterfaceType);
                if ((inputState & PROPERTY_AXIS_STATE_ENABLE) != 0)
                {
                    // axis is enabled
                    if ((inputState & PROPERTY_AXIS_STATE_ENDAT) == 0)
                    {
                        // add incremental options
                        Properties->AddRange(pcIncrementOpt);
                        if ((inputState & PROPERTY_AXIS_STATE_INC_1V) != 0)
                        {
                            // set 1V interface options
                            ((PropertyEntry ^)
                                (pcIncrementOpt->ItemOf(L"Bandwidth")))->DefaultValue =
                                                                        EIB_DEFAULT_BANDWIDTH_1V;
                            ((PropertyEntry ^)
                                (pcIncrementOpt->ItemOf(L"Bandwidth")))->ConverterType =
                                                                        Bandwidth1VConverter::typeid;
                            changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                         | PROPERTY_AXIS_STATE_ENDAT
                                         | PROPERTY_AXIS_STATE_INC_1V;
                            if (((currentState ^ inputState) & changeMask) != 0)
                            {
                                axisParameter->Bandwidth = EIB_DEFAULT_BANDWIDTH_1V;
                            }
                        }
                        else
                        {
                            // set 11my interface options
                            ((PropertyEntry ^)
                                (pcIncrementOpt->ItemOf(L"Bandwidth")))->DefaultValue =
                                                                        EIB_DEFAULT_BANDWIDTH_11MY;
                            ((PropertyEntry ^)
                                (pcIncrementOpt->ItemOf(L"Bandwidth")))->ConverterType =
                                                                        Bandwidth11myConverter::typeid;
                            changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                         | PROPERTY_AXIS_STATE_ENDAT
                                         | PROPERTY_AXIS_STATE_INC_1V;
                            if (((currentState ^ inputState) & changeMask) != 0)
                            {
                                axisParameter->Bandwidth = EIB_DEFAULT_BANDWIDTH_11MY;
                            }
                        }
                        if ((inputState & PROPERTY_AXIS_STATE_TYPE_ROT) == 0)
                        {
                            // add signal period
                            Properties->Add(peSignalPeriod);
                            // set signal period value
                            changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                         | PROPERTY_AXIS_STATE_ENDAT
                                         | PROPERTY_AXIS_STATE_TYPE_ROT;
                            if (((currentState ^ inputState) & changeMask) != 0)
                            {
                                axisParameter->SignalPeriod = EIB_DEFAULT_SIGNAL_PERIOD;
                            }
                        }
                        else
                        {
                            // add linecounts
                            Properties->Add(peLinecounts);
                            // set linecounts value
                            changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                         | PROPERTY_AXIS_STATE_ENDAT
                                         | PROPERTY_AXIS_STATE_TYPE_ROT;
                            if (((currentState ^ inputState) & changeMask) != 0)
                            {
                                axisParameter->Linecounts = EIB_DEFAULT_LINECOUNTS;
                            }
                        }
                        if ((inputState & PROPERTY_AXIS_STATE_REF_DIST) != 0)
                        {
                            // add increment
                            Properties->Add(peIncrement);
                            // set increment value
                            changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                         | PROPERTY_AXIS_STATE_ENDAT
                                         | PROPERTY_AXIS_STATE_REF_DIST;
                            if (((currentState ^ inputState) & changeMask) != 0)
                            {
                                axisParameter->Increment = EIB_DEFAULT_INCREMENT;
                            }
                        }
                    }
                    else
                    {
                        // add EnDat options
                        if ((inputState & PROPERTY_AXIS_STATE_ENDAT_22) == 0)
                        {
                            // add EnDat 2.1 options
                            Properties->AddRange(pcEnDat21Opt);
                            // set EnDat 2.1 options values
                            changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                         | PROPERTY_AXIS_STATE_ENDAT
                                         | PROPERTY_AXIS_STATE_ENDAT_22;
                            if (((currentState ^ inputState) & changeMask) != 0)
                            {
                                // set incremental signals value
                                axisParameter->IncSignals = EIB_DEFAULT_INC_SIGNALS_21;
                                // set EnDat clock value
                                axisParameter->EnDatClock21 = EIB_DEFAULT_INTERFACE_CLOCK_21;
                                // set calculation time value
                                axisParameter->Calculation = EIB_DEFAULT_ENDAT_CALC_TIME_21;
                            }
                            // set runtime compensation value
                            axisParameter->RuntimeComp = EIB_DEFAULT_RUNTIME_COMP_21;
                            // set recovery time value
                            axisParameter->Recovery = EIB_DEFAULT_RECOVERY_TIME;
                        }
                        else
                        {
                            // add EnDat 2.2 options
                            Properties->AddRange(pcEnDat22Opt);
                            // set EnDat 2.2 options values
                            changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                         | PROPERTY_AXIS_STATE_ENDAT
                                         | PROPERTY_AXIS_STATE_ENDAT_22;
                            if (((currentState ^ inputState) & changeMask) != 0)
                            {
                                // set EnDat clock value
                                axisParameter->EnDatClock22 = EIB_DEFAULT_INTERFACE_CLOCK_22;
                                // set recovery time value
                                axisParameter->Recovery = EIB_DEFAULT_RECOVERY_TIME;
                                // set additional information mode value
                                axisParameter->AddInfoMode = EIB_DEFAULT_ADD_INFO_MODE;
                            }
                            // set incremental signals value
                            axisParameter->IncSignals = EIB_DEFAULT_INC_SIGNALS_22;
                            // set runtime compensation value
                            axisParameter->RuntimeComp = EIB_DEFAULT_RUNTIME_COMP_22;
                            // set calculation time value
                            axisParameter->Calculation = EIB_DEFAULT_ENDAT_CALC_TIME_22;
                            // set new input state
                            SetInputState(axisParameter->IsEnabled);
                            // add additional information options
                            if ((inputState & PROPERTY_AXIS_STATE_ADD_INFO_ON) != 0)
                            {
                                if ((inputState & PROPERTY_AXIS_STATE_ADD_INFO_CNT) == 0)
                                {
                                    // add fix additional information options
                                    Properties->AddRange(pcAddInfoFix);
                                    // set fix additional information options values
                                    changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                                 | PROPERTY_AXIS_STATE_ENDAT
                                                 | PROPERTY_AXIS_STATE_ENDAT_22
                                                 | PROPERTY_AXIS_STATE_ADD_INFO_ON
                                                 | PROPERTY_AXIS_STATE_ADD_INFO_CNT;
                                    if (((currentState ^ inputState) & changeMask) != 0)
                                    {
                                        // set fix additional information 1 value
                                        axisParameter->AddInfo1 = EIB_DEFAULT_ADD_INFO_1;
                                        // set fix additional information 2 value
                                        axisParameter->AddInfo2 = EIB_DEFAULT_ADD_INFO_2;
                                    }
                                }
                                else
                                {
                                    // add fifo additional information number
                                    Properties->Add(peAddInfoNum);
                                    // set fifo additional information number value
                                    changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                                 | PROPERTY_AXIS_STATE_ENDAT
                                                 | PROPERTY_AXIS_STATE_ENDAT_22
                                                 | PROPERTY_AXIS_STATE_ADD_INFO_ON;
                                    if (   (((currentState ^ inputState) & changeMask) != 0)
                                        || ((currentState & PROPERTY_AXIS_STATE_ADD_INFO_CNT) == 0))
                                    {
                                        // set fifo additional information number value
                                        axisParameter->AddInfoNum = EIB_DEFAULT_ADD_INFO_NUM;
                                    }
                                    // add fifo additional information entries
                                    for (UInt32 i = 0; i < UInt32(axisParameter->AddInfoNum); i ++)
                                    {
                                        Properties->Add(peAddInfoFifo[i]);
                                    }
                                    // set fifo additional information entry values
                                    changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                                 | PROPERTY_AXIS_STATE_ENDAT
                                                 | PROPERTY_AXIS_STATE_ENDAT_22
                                                 | PROPERTY_AXIS_STATE_ADD_INFO_ON
                                                 | PROPERTY_AXIS_STATE_ADD_INFO_CNT;
                                    if (((currentState ^ inputState) & changeMask) != 0)
                                    {
                                        // set fifo additional information entry value
                                        UInt32 first = currentState & PROPERTY_AXIS_STATE_ADD_INFO_CNT;
                                        for (UInt32 i = first; i < UInt32(axisParameter->AddInfoNum); i ++)
                                        {
                                            axisParameter->AddInfoFifo[i] = EIB_DEFAULT_ADD_INFO_FIFO;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // refresh the datapacket parameters
                PropertyAxisBase::RefreshParam();
                // set special parameter dependencies
                if (axisParameter->InterfaceTypeGui != EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22)
                {
                    // reset additional information for non EnDat 2.2 axis
                    axisParameter->AddInfoMode = EIB7_ADD_INFO_MODE::EIB7_AIM_None;
                }
                if (axisParameter->AddInfoMode == EIB7_ADD_INFO_MODE::EIB7_AIM_None)
                {
                    // reset additional information for non EnDat 2.2 axis
                    axisParameter->DataPacketEnDatAI1 = false;
                    axisParameter->DataPacketEnDatAI2 = false;
                }
                // set new state
                currentState = inputState;
            }
            // refresh the property grid
            PropertyBase::RefreshParam();
        }

        /// <summary>
		/// This member overrides PropertyClass.OnGetValue.
		/// </summary>
		virtual void OnGetValue(PropertyEntryEventArgs ^ e) override
		{
            if (e->Property->Name == L"AxisTermination")
            {
                // set parameter class depending from parameter
                paramClass = parameter;
            }
            else
            {
                // set parameter class depending from parameter
                paramClass = axisParameter;
            }
            PropertyBase::OnGetValue(e);
		}

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            if (e->Property->Name == L"AxisTermination")
            {
                // set parameter class depending from parameter
                paramClass = parameter;
                // show warning for setting this parameter for all axis
                // save old parameter value
                PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
                Object ^ oldValue = propertyInfo->GetValue(paramClass, e->Property->Indices);
                // set new parameter value
                PropertyBase::OnSetValue(e);
                // ask for setting this parameter for all axis
                String ^ message = String::Format(resManager->GetString(L"warningSetForAllAxis"),
                                                  parameter->EibName);
                Windows::Forms::DialogResult result =
                            guiSupport->MsgBoxWarning(message,
                                                      MessageBoxButtons::YesNo,
                                                      MessageBoxDefaultButton::Button1);
                if (result == ::DialogResult::No)
                {
                    // restore old parameter value
                    e->Value = oldValue;
                }
            }
            else
            {
                // set parameter class depending from parameter
                paramClass = axisParameter;
            }
            PropertyBase::OnSetValue(e);
		}
    };
	#pragma endregion
}
