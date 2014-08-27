///
/// \file       PropertyTrigger.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyTrigger.h,v 1.2 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.2 $
/// $State: Exp $
/// $Log: PropertyTrigger.h,v $
/// Revision 1.2  2011/12/16 13:37:56  rohn
/// Version 0.9.0.1 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB trigger parameters input class.
///
/// This class is used for the management of the EIB trigger
/// parameters input via the axis property grid. It manages
/// the parameter dependencies for the property grid.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Resources;
using namespace System::Reflection;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;

namespace EIB7xx_App
{
 	#pragma region "TriggerInpModeConverter class definition"
	/// <summary>
	/// This class definies a type converter for a trigger mode enum.
    /// enum <--> String.
	/// </summary>
    public ref class TriggerInpModeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerInpModeConverter() : LocalEnumConverter(EIB7_IO_MODE::typeid, L"_TriggerInput")
        {
        }
    };
	#pragma endregion

 	#pragma region "TriggerInpTermConverter class definition"
	/// <summary>
	/// This class definies a type converter for a trigger termination enum.
    /// enum <--> String.
	/// </summary>
    public ref class TriggerInpTermConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerInpTermConverter() : LocalEnumConverter(EIB7_ENABLE_MODE::typeid, L"_TriggerTermination")
        {
        }
    };
	#pragma endregion

 	#pragma region "TriggerInpDelayConverter class definition"
	/// <summary>
	/// This class definies a type converter for a trigger delay value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class TriggerInpDelayConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerInpDelayConverter() : UInt32ValueConverter(EIB_MIN_TRIGGER_INP_DELAY,
                                                          EIB_MAX_TRIGGER_INP_DELAY,
                                                          EIB_STEP_TRIGGER_INP_DELAY)
        {
        }
    };
	#pragma endregion

 	#pragma region "TriggerOutEnableConverter class definition"
	/// <summary>
	/// This class definies a type converter for a trigger enable enum.
    /// enum <--> String.
	/// </summary>
    public ref class TriggerOutEnableConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerOutEnableConverter() : LocalEnumConverter(EIB7_ENABLE_MODE::typeid, L"_TriggerEnable")
        {
        }
    };
	#pragma endregion

 	#pragma region "TriggerOutModeConverter class definition"
	/// <summary>
	/// This class definies a type converter for a trigger mode enum.
    /// enum <--> String.
	/// </summary>
    public ref class TriggerOutModeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerOutModeConverter() : LocalEnumConverter(EIB7_IO_MODE::typeid, L"_TriggerOutput")
        {
        }
    };
	#pragma endregion

 	#pragma region "TriggerOutSourceConverter class definition"
	/// <summary>
	/// This class definies a type converter for a trigger output source enum.
    /// enum <--> String.
	/// </summary>
    public ref class TriggerOutSourceConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerOutSourceConverter() : LocalEnumConverter(EIB7_TRIGGER_OUT_SOURCE::typeid)
        {
        }

        /// <summary>
        /// This function returns the value key string.
        /// </summary>
        virtual String ^ GetValueText(ITypeDescriptorContext ^ context,
                                      Object ^ value) override
        {
            if (   (context != nullptr)
                && (   ((EIB7_TRIGGER_OUT_SOURCE)value == EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgInSync)
                    || ((EIB7_TRIGGER_OUT_SOURCE)value == EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgInAsync)))
            {
                return   GetPrefix(value)
                       + String::Format(resManager->GetString(value->ToString() + keySuffix),
                                        context->PropertyDescriptor->Category);
            }
            return LocalEnumConverter::GetValueText(context, value);
        }
    };
	#pragma endregion

 	#pragma region "MasterTriggerSourceConverter class definition"
	/// <summary>
	/// This class definies a type converter for a master trigger source enum.
    /// enum <--> String.
	/// </summary>
    public ref class MasterTriggerSourceConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        MasterTriggerSourceConverter() : LocalEnumConverter(EIB7_MASTER_TRIGGER_SOURCE::typeid)
        {
        }

        /// <summary>
        /// This function returns the value key string.
        /// </summary>
        virtual String ^ GetValueText(ITypeDescriptorContext ^ context,
                                      Object ^ value) override
        {
            if ((EIB7_MASTER_TRIGGER_SOURCE)value == EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgRI)
            {
                return   GetPrefix(value)
                       + String::Format(resManager->GetString(value->ToString() + keySuffix),
                                        resManager->GetString(L"textCorrespondingAxis"));
            }
            return LocalEnumConverter::GetValueText(context, value);
        }
    };
	#pragma endregion

 	#pragma region "AxisTriggerSourceConverter class definition"
	/// <summary>
	/// This class definies a type converter for a axis trigger source enum.
    /// enum <--> String.
	/// </summary>
    public ref class AxisTriggerSourceConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AxisTriggerSourceConverter() : LocalEnumConverter(EIB7_AXIS_TRIGGER_SOURCE::typeid)
        {
        }

        /// <summary>
        /// This function returns the value key string.
        /// </summary>
        virtual String ^ GetValueText(ITypeDescriptorContext ^ context,
                                      Object ^ value) override
        {
            if (   (context != nullptr)
                && ((EIB7_AXIS_TRIGGER_SOURCE)value == EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgRI))
            {
                return   GetPrefix(value)
                       + String::Format(resManager->GetString(value->ToString() + keySuffix),
                                        context->PropertyDescriptor->Category);
            }
            return LocalEnumConverter::GetValueText(context, value);
        }
    };
	#pragma endregion

 	#pragma region "AxisTriggerEdgeConverter class definition"
	/// <summary>
	/// This class definies a type converter for a axis trigger edge enum.
    /// enum <--> String.
	/// </summary>
    public ref class AxisTriggerEdgeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        AxisTriggerEdgeConverter() : LocalEnumConverter(EIB7_AXIS_TRIGGER_EDGE::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "TimerTriggerPeriodConverter class definition"
	/// <summary>
	/// This class definies a type converter for a timer trigger period value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class TimerTriggerPeriodConverter : public UInt32ValueConverter 
    {
    public:
        static UInt32 eibMinTimerTriggerPeriod;     ///< static EIB min timer trigger period

        /// <summary>
        /// Constructor.
        /// </summary>
        TimerTriggerPeriodConverter() : UInt32ValueConverter(eibMinTimerTriggerPeriod,
                                                             EIB_MAX_TIMER_TRIGGER_PERIOD,
                                                             EIB_STEP_TIMER_TRIGGER_PERIOD)
        {
        }
    };
	#pragma endregion

 	#pragma region "IntervalCounterModeConverter class definition"
	/// <summary>
	/// This class definies a type converter for an interval counter mode enum.
    /// enum <--> String.
	/// </summary>
    public ref class IntervalCounterModeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        IntervalCounterModeConverter() : LocalEnumConverter(EIB7_INTERVAL_COUNTER_MODE::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "IntervalCounterStartConverter class definition"
	/// <summary>
	/// This class definies a type converter for an interval counter start enum.
    /// enum <--> String.
	/// </summary>
    public ref class IntervalCounterStartConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        IntervalCounterStartConverter() : LocalEnumConverter(EIB7_INTERVAL_COUNTER_START::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "IntervalCounterStartPosConverter class definition"
	/// <summary>
	/// This class definies a type converter for a interval counter start position value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class IntervalCounterStartPosConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        IntervalCounterStartPosConverter() : UInt32ValueConverter(EIB_MIN_INTERVAL_COUNTER_START_POS,
                                                                  EIB_MAX_INTERVAL_COUNTER_START_POS,
                                                                  EIB_STEP_INTERVAL_COUNTER_START_POS)
        {
        }
    };
	#pragma endregion

 	#pragma region "IntervalCounterPeriodConverter class definition"
	/// <summary>
	/// This class definies a type converter for a interval counter start position value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class IntervalCounterPeriodConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        IntervalCounterPeriodConverter() : UInt32ValueConverter(EIB_MIN_INTERVAL_COUNTER_PERIOD,
                                                                EIB_MAX_INTERVAL_COUNTER_PERIOD,
                                                                EIB_STEP_INTERVAL_COUNTER_PERIOD)
        {
        }
    };
	#pragma endregion

 	#pragma region "PulseCounterCountConverter class definition"
	/// <summary>
	/// This class definies a type converter for a pulse counter count value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class PulseCounterCountConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        PulseCounterCountConverter() : UInt32ValueConverter(EIB_MIN_PULSE_COUNTER_COUNT,
                                                            EIB_MAX_PULSE_COUNTER_COUNT,
                                                            EIB_STEP_PULSE_COUNTER_COUNT)
        {
        }
    };
	#pragma endregion

 	#pragma region "PulseCounterStartConverter class definition"
	/// <summary>
	/// This class definies a type converter for a pulse counter start enum.
    /// enum <--> String.
	/// </summary>
    public ref class PulseCounterStartConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        PulseCounterStartConverter() : LocalEnumConverter(EIB7_PULSE_COUNTER_START::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "PulseCounterTriggerConverter class definition"
	/// <summary>
	/// This class definies a type converter for a pulse counter trigger enum.
    /// enum <--> String.
	/// </summary>
    public ref class PulseCounterTriggerConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        PulseCounterTriggerConverter() : LocalEnumConverter(EIB7_PULSE_COUNTER_TRIGGER::typeid)
        {
        }
    };
	#pragma endregion

    // property trigger state machine definitions
    #define PROPERTY_TRIGGER_STATE_INIT_READY       0x0001
    #define PROPERTY_TRIGGER_STATE_ACTIVE           0x0002
    #define PROPERTY_TRIGGER_STATE_MODE_TRIGGER     0x0004
    #define PROPERTY_TRIGGER_STATE_START_POS        0x0008
    #define PROPERTY_TRIGGER_STATE_PERIODIC         0x0010
    #define PROPERTY_TRIGGER_STATE_REFERENCE        0x0020

 	#pragma region "PropTriggerInp class definition"
    /// <summary>
    /// This class is used for the management of the EIB trigger
    /// parameters input via the axis property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropTriggerInp : public PropertyBase
    {
    private:
        EIB_Param ^                 parameter;          ///< parameters of the saved EIB

        UInt32                      triggerIndex;       ///< the trigger input index

        UInt32                      currentState;       ///< the current input state

        PropertyEntryCollection ^   pcTriggerInp;       ///< the property list for trigger input
        PropertyEntry ^             peTriggerDelay;     ///< the property entry for trigger input delay
        
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="parameter">The parameters of the saved EIB.</param>
		/// <param name="triggerIndex">The trigger input index.</param>
        PropTriggerInp(GUI_Support ^ guiSupport,
                       EIB_Param ^   parameter,
                       UInt32        triggerIndex)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridTrigger)
        {
            this->parameter    = parameter;
            this->triggerIndex = triggerIndex;
            // set trigger parameter class
            paramClass = parameter->Trigger;

            // create property index list
            IndexList ^ indexList = gcnew IndexList(1);
            indexList[0] = triggerIndex;
            // create the property list for trigger input
            pcTriggerInp = gcnew PropertyEntryCollection();
            pcTriggerInp->Add(gcnew PropertyEntry(
                                            L"TriggerInpMode", indexList, EIB7_IO_MODE::typeid,
                                            nullptr,
                                            resManager->GetString(L"textTriggerInpMode"),
                                            resManager->GetString(L"descriptionTriggerInpMode"),
                                            EIB_DEFAULT_TRIGGER_INP_MODE,
                                            TriggerInpModeConverter::typeid));
            pcTriggerInp->Add(gcnew PropertyEntry(
                                            L"TriggerInpTerm", indexList, EIB7_ENABLE_MODE::typeid,
                                            nullptr,
                                            resManager->GetString(L"textTriggerInpTerm"),
                                            resManager->GetString(L"descriptionTriggerInpTerm"),
                                            EIB_DEFAULT_TRIGGER_INP_TERM,
                                            TriggerInpTermConverter::typeid));
            // create the property entry for trigger input delay
            peTriggerDelay = gcnew PropertyEntry(
                                            L"TriggerInpDelay", indexList, UInt32::typeid,
                                            nullptr,
                                            resManager->GetString(L"textTriggerInpDelay"),
                                            resManager->GetString(L"descriptionTriggerInpDelay"),
                                            (Object ^)EIB_DEFAULT_TRIGGER_INP_DELAY,
                                            TriggerInpDelayConverter::typeid);
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            // set input state
            UInt32 inputState = PROPERTY_TRIGGER_STATE_INIT_READY;
            if (parameter->Trigger->TriggerInpMode[triggerIndex] == EIB7_IO_MODE::EIB7_IOM_Trigger)
            {
                // trigger input is in trigger mode
                inputState |= PROPERTY_TRIGGER_STATE_MODE_TRIGGER;
            }
            // don't change parameters on first call
            if (currentState == CURRENT_STATE_INIT)
            {
                currentState |= inputState & ~PROPERTY_TRIGGER_STATE_INIT_READY;
            }
            // check current state
            if (currentState != inputState)
            {
                UInt32 changeMask;
                // set property list
                Properties->Clear();
                // add trigger mode and termination
                Properties->AddRange(pcTriggerInp);
                if ((inputState & PROPERTY_TRIGGER_STATE_MODE_TRIGGER) != 0)
                {
                    // add input delay
                    Properties->Add(peTriggerDelay);

                    changeMask = PROPERTY_TRIGGER_STATE_MODE_TRIGGER;
                    if (((currentState ^ inputState) & changeMask) != 0)
                    {
                        parameter->Trigger->TriggerInpDelay[triggerIndex] = EIB_DEFAULT_TRIGGER_INP_DELAY;
                    }
                }
                // set new state
                currentState = inputState;
            }
        }
    };
	#pragma endregion

 	#pragma region "TriggerInpConverter class definition"
	/// <summary>
	/// This class definies a type converter for a trigger input class.
    /// class <--> String.
	/// </summary>
    public ref class TriggerInpConverter : public TopItemConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerInpConverter() : TopItemConverter(PropTriggerInp::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "PropTriggerOut class definition"
    /// <summary>
    /// This class is used for the management of the EIB trigger
    /// parameters input via the axis property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropTriggerOut : public PropertyBase
    {
    private:
        EIB_Param ^                 parameter;          ///< parameters of the saved EIB

        UInt32                      triggerIndex;       ///< the trigger input index

        UInt32                      currentState;       ///< the current input state

        PropertyEntry ^             peTriggerEnable;    ///< the property entry for trigger output enable
        PropertyEntry ^             peTriggerMode;      ///< the property entry for trigger output mode
        PropertyEntry ^             peTriggerSource;    ///< the property entry for trigger output source
        
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="parameter">The parameters of the saved EIB.</param>
		/// <param name="triggerIndex">The trigger input index.</param>
        PropTriggerOut(GUI_Support ^ guiSupport,
                       EIB_Param ^   parameter,
                       UInt32        triggerIndex)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridTrigger)
        {
            this->parameter    = parameter;
            this->triggerIndex = triggerIndex;
            // set trigger parameter class
            paramClass = parameter->Trigger;

            // create property index list
            IndexList ^ indexList = gcnew IndexList(1);
            indexList[0] = triggerIndex;
            // create the property entry for trigger output enable
            peTriggerEnable = gcnew PropertyEntry(
                                            L"TriggerOutEnable", indexList, EIB7_ENABLE_MODE::typeid,
                                            nullptr,
                                            resManager->GetString(L"textTriggerOutEnable"),
                                            resManager->GetString(L"descriptionTriggerOutEnable"),
                                            EIB_DEFAULT_TRIGGER_OUT_ENABLE,
                                            TriggerOutEnableConverter::typeid);
            // create the property entry for trigger output mode
            peTriggerMode = gcnew PropertyEntry(
                                            L"TriggerOutMode", indexList, EIB7_IO_MODE::typeid,
                                            nullptr,
                                            resManager->GetString(L"textTriggerOutMode"),
                                            resManager->GetString(L"descriptionTriggerOutMode"),
                                            EIB_DEFAULT_TRIGGER_OUT_MODE,
                                            TriggerOutModeConverter::typeid);
            // create the property entry for trigger output source
            peTriggerSource = gcnew PropertyEntry(
                                            L"TriggerOutSource", indexList, EIB7_TRIGGER_OUT_SOURCE::typeid,
                                            (triggerIndex + 1).ToString(),
                                            resManager->GetString(L"textTriggerOutSource"),
                                            resManager->GetString(L"descriptionTriggerOutSource"),
                                            EIB_DEFAULT_TRIGGER_OUT_SOURCE,
                                            TriggerOutSourceConverter::typeid);
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            // set input state
            UInt32 inputState = PROPERTY_TRIGGER_STATE_INIT_READY;
            if (parameter->Trigger->TriggerOutEnable[triggerIndex] == EIB7_ENABLE_MODE::EIB7_MD_Enable)
            {
                // trigger ouput is enabled
                inputState |= PROPERTY_TRIGGER_STATE_ACTIVE;
                if (parameter->Trigger->TriggerOutMode[triggerIndex] == EIB7_IO_MODE::EIB7_IOM_Trigger)
                {
                    // trigger ouput is in trigger mode
                    inputState |= PROPERTY_TRIGGER_STATE_MODE_TRIGGER;
                }
            }
            // don't change parameters on first call
            if (currentState == CURRENT_STATE_INIT)
            {
                currentState |= inputState & ~PROPERTY_TRIGGER_STATE_INIT_READY;
            }
            // check current state
            if (currentState != inputState)
            {
                UInt32 changeMask;
                // set property list
                Properties->Clear();
                // add trigger enable
                Properties->Add(peTriggerEnable);
                if ((inputState & PROPERTY_TRIGGER_STATE_ACTIVE) != 0)
                {
                    // add trigger mode
                    Properties->Add(peTriggerMode);

                    changeMask = PROPERTY_TRIGGER_STATE_ACTIVE;
                    if (((currentState ^ inputState) & changeMask) != 0)
                    {
                        parameter->Trigger->TriggerOutMode[triggerIndex] = EIB_DEFAULT_TRIGGER_OUT_MODE;
                    }
                    if ((inputState & PROPERTY_TRIGGER_STATE_MODE_TRIGGER) != 0)
                    {
                        // add trigger source
                        Properties->Add(peTriggerSource);

                        changeMask =   PROPERTY_TRIGGER_STATE_ACTIVE
                                     | PROPERTY_TRIGGER_STATE_MODE_TRIGGER;
                        if (((currentState ^ inputState) & changeMask) != 0)
                        {
                            parameter->Trigger->TriggerOutSource[triggerIndex] = EIB_DEFAULT_TRIGGER_OUT_SOURCE;
                        }
                    }
                }
                // set new state
                currentState = inputState;
            }
        }

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            // show not supported error for software trigger selection
            if (   (e->Property->Name == L"TriggerOutSource")
                && (   ((EIB7_TRIGGER_OUT_SOURCE)(e->Value) == EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgSW1)
                    || ((EIB7_TRIGGER_OUT_SOURCE)(e->Value) == EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgSW2)
                    || ((EIB7_TRIGGER_OUT_SOURCE)(e->Value) == EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgSW3)
                    || ((EIB7_TRIGGER_OUT_SOURCE)(e->Value) == EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgSW4)))
            {
                // save old parameter value
                PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
                Object ^ oldValue = propertyInfo->GetValue(paramClass, e->Property->Indices);
                // set new parameter value
                PropertyBase::OnSetValue(e);
                // show not supported error
                guiSupport->MsgBoxErrorOk(resManager->GetString(L"errorSelectionNotSupported"));
                // restore old parameter value
                e->Value = oldValue;
            }
            // set new parameter values
            PropertyBase::OnSetValue(e);
		}
    };
	#pragma endregion

 	#pragma region "TriggerOutConverter class definition"
	/// <summary>
	/// This class definies a type converter for a trigger output class.
    /// class <--> String.
	/// </summary>
    public ref class TriggerOutConverter : public TopItemConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerOutConverter() : TopItemConverter(PropTriggerOut::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "PropTriggerAxis class definition"
    /// <summary>
    /// This class is used for the management of the EIB trigger
    /// parameters input via the axis property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropTriggerAxis : public PropertyBase
    {
    private:
        EIB_Param ^                 parameter;          ///< parameters of the saved EIB

        UInt32                      axisIndex;          ///< the axis index
        UInt32                      auxAxisNumber;      ///< the auxiliary axis number

        UInt32                      currentState;       ///< the current input state

        PropertyEntry ^             peTriggerSource;    ///< the property entry for axis trigger source
        PropertyEntry ^             peTriggerEdge;      ///< the property entry for axis trigger edge
        
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="parameter">The parameters of the saved EIB.</param>
		/// <param name="axisIndex">The axis index.</param>
		/// <param name="auxAxisNumber">The auxiliary axis number.</param>
        PropTriggerAxis(GUI_Support ^ guiSupport,
                        EIB_Param ^   parameter,
                        UInt32        axisIndex,
                        UInt32        auxAxisNumber)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridTrigger)
        {
            this->parameter     = parameter;
            this->axisIndex     = axisIndex;
            this->auxAxisNumber = auxAxisNumber;
            // set trigger parameter class
            paramClass = parameter->Trigger;

            String ^ propName;
            String ^ category;
            // create property index list
            IndexList ^ indexList = gcnew IndexList(1);
            indexList[0] = axisIndex;
            // create the property entry for axis trigger source
            if (auxAxisNumber == 0)
            {
                propName = L"AxisTriggerSource";
                category = String::Format(resManager->GetString(L"textOfAxis") + L" {0:D}",
                                                                axisIndex + 1);
            }
            else
            {
                propName = L"AuxAxisTriggerSource";
                if (auxAxisNumber == 1)
                {
                    category = resManager->GetString(L"textOfAuxAxis");
                }
                else
                {
                    category = String::Format(resManager->GetString(L"textOfAuxAxisN") + L" {0:D}",
                                                                    axisIndex + 1);
                }
            }
            peTriggerSource = gcnew PropertyEntry(
                                            propName, indexList, EIB7_AXIS_TRIGGER_SOURCE::typeid,
                                            category,
                                            resManager->GetString(L"textAxisTriggerSource"),
                                            resManager->GetString(L"descriptionAxisTriggerSource"),
                                            EIB_DEFAULT_AXIS_TRIGGER_SOURCE,
                                            AxisTriggerSourceConverter::typeid);
            // create the property entry for axis trigger edge
            if (auxAxisNumber == 0)
            {
                propName = L"AxisTriggerEdge";
            }
            else
            {
                propName = L"AuxAxisTriggerEdge";
            }
            peTriggerEdge = gcnew PropertyEntry(
                                            propName, indexList, EIB7_AXIS_TRIGGER_EDGE::typeid,
                                            nullptr,
                                            resManager->GetString(L"textAxisTriggerEdge"),
                                            resManager->GetString(L"descriptionAxisTriggerEdge"),
                                            EIB_DEFAULT_AXIS_TRIGGER_EDGE,
                                            AxisTriggerEdgeConverter::typeid);
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            // set input state
            UInt32 inputState = PROPERTY_TRIGGER_STATE_INIT_READY;
            EIB7_AXIS_TRIGGER_SOURCE triggerSource;
            if (auxAxisNumber == 0)
            {
                triggerSource = parameter->Trigger->AxisTriggerSource[axisIndex];
            }
            else
            {
                triggerSource = parameter->Trigger->AuxAxisTriggerSource[axisIndex];
            }
            if (   (triggerSource == EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgRI)
                || (triggerSource == EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgRImaskedCH1))
            {
                // axis trigger source is a reference impulse
                inputState |= PROPERTY_TRIGGER_STATE_REFERENCE;
            }
            // don't change parameters on first call
            if (currentState == CURRENT_STATE_INIT)
            {
                currentState |= inputState & ~PROPERTY_TRIGGER_STATE_INIT_READY;
            }
            // check current state
            if (currentState != inputState)
            {
                UInt32 changeMask;
                // set property list
                Properties->Clear();
                // add axis trigger source
                Properties->Add(peTriggerSource);
                if ((inputState & PROPERTY_TRIGGER_STATE_REFERENCE) != 0)
                {
                    // add axis trigger edge
                    Properties->Add(peTriggerEdge);

                    changeMask = PROPERTY_TRIGGER_STATE_REFERENCE;
                    if (((currentState ^ inputState) & changeMask) != 0)
                    {
                        if (auxAxisNumber == 0)
                        {
                            parameter->Trigger->AxisTriggerEdge[axisIndex] = EIB_DEFAULT_AXIS_TRIGGER_EDGE;
                        }
                        else
                        {
                            parameter->Trigger->AuxAxisTriggerEdge[axisIndex] = EIB_DEFAULT_AUX_AXIS_TRIGGER_EDGE;
                        }
                    }
                }
                // set new state
                currentState = inputState;
            }
        }

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            // show not supported error for software trigger selection
            if (   (   (e->Property->Name == L"AxisTriggerSource")
                    || (e->Property->Name == L"AuxAxisTriggerSource"))
                && (   ((EIB7_AXIS_TRIGGER_SOURCE)(e->Value) == EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgSW1)
                    || ((EIB7_AXIS_TRIGGER_SOURCE)(e->Value) == EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgSW2)
                    || ((EIB7_AXIS_TRIGGER_SOURCE)(e->Value) == EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgSW3)
                    || ((EIB7_AXIS_TRIGGER_SOURCE)(e->Value) == EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgSW4)))
            {
                // save old parameter value
                PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
                Object ^ oldValue = propertyInfo->GetValue(paramClass, e->Property->Indices);
                // set new parameter value
                PropertyBase::OnSetValue(e);
                // show not supported error
                guiSupport->MsgBoxErrorOk(resManager->GetString(L"errorSelectionNotSupported"));
                // restore old parameter value
                e->Value = oldValue;
            }
            // set new parameter values
            PropertyBase::OnSetValue(e);
		}
    };
	#pragma endregion

 	#pragma region "TriggerAxisConverter class definition"
	/// <summary>
	/// This class definies a type converter for a axis trigger class.
    /// class <--> String.
	/// </summary>
    public ref class TriggerAxisConverter : public TopItemConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TriggerAxisConverter() : TopItemConverter(PropTriggerAxis::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "PropertyTrigger class definition"
    // property trigger input list
    typedef array<PropTriggerInp ^>     TriggerInpList;
    // property trigger output list
    typedef array<PropTriggerOut ^>     TriggerOutList;
    // property axis trigger list
    typedef array<PropTriggerAxis ^>    TriggerAxisList;

    /// <summary>
    /// This class is used for the management of the EIB trigger
    /// parameters input via the axis property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropertyTrigger : public PropertyBase
    {
    private:
        EIB_Param ^                 parameter;          ///< parameters of the saved EIB

        UInt32                      currentState;       ///< the current input state
        bool                        firstShow;          ///< true if the grid is shown first time

        TriggerInpList ^            plTriggerInp;       ///< the property list for trigger input
        TriggerOutList ^            plTriggerOut;       ///< the property list for trigger output
        TriggerAxisList ^           plTriggerAxis;      ///< the property list for axis trigger
        TriggerAxisList ^           plTriggerAuxAxis;   ///< the property list for auxiliary axis trigger

        PropertyEntryCollection ^   pcTriggerInp;       ///< the property list for trigger input
        PropertyEntryCollection ^   pcTriggerOut;       ///< the property list for trigger output
        PropertyEntryCollection ^   pcTriggerAxis;      ///< the property list for axis trigger
        PropertyEntryCollection ^   pcTriggerAuxAxis;   ///< the property list for auxiliary axis trigger

        PropertyEntry ^             peMasterTrigger;    ///< the property entry for master trigger
        PropertyEntry ^             peTimerTrigger;     ///< the property entry for timer trigger

        PropertyEntry ^             peIcMode;           ///< the property entry for interval counter mode
        PropertyEntry ^             peIcStartOpt;       ///< the property entry for interval counter start option
        PropertyEntry ^             peIcStartPos;       ///< the property entry for interval counter start position
        PropertyEntry ^             peIcPeriod;         ///< the property entry for interval counter period

        PropertyEntryCollection ^   pePulseCounter;     ///< the property list for pulse counter

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="parameter">The parameters of the saved EIB.</param>
        PropertyTrigger(GUI_Support ^ guiSupport,
                        EIB_Param ^   parameter)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridTrigger)
        {
            this->parameter = parameter;

            currentState = CURRENT_STATE_INIT;
            firstShow    = true;
            // create category strings
            String ^ categoryTriggerAxis =
                                String::Format(CATEGORY_HEADER,
                                               1, resManager->GetString(L"categoryAxisTrigger"));
            String ^ categoryMasterTrigger =
                                String::Format(CATEGORY_HEADER,
                                               2, resManager->GetString(L"categoryMasterTrigger"));
            String ^ categoryTimerTrigger =
                                String::Format(CATEGORY_HEADER,
                                               3, resManager->GetString(L"categoryTimerTrigger"));
            String ^ categoryIntervalCounter =
                                String::Format(CATEGORY_HEADER,
                                               4, resManager->GetString(L"categoryIntervalCounter"));
            String ^ categoryPulseCounter =
                                String::Format(CATEGORY_HEADER,
                                               5, resManager->GetString(L"categoryPulseCounter"));
            String ^ categoryExtTriggerInp =
                                String::Format(CATEGORY_HEADER,
                                               6, resManager->GetString(L"categoryExtTriggerInp"));
            String ^ categoryExtTriggerOut =
                                String::Format(CATEGORY_HEADER,
                                               7, resManager->GetString(L"categoryExtTriggerOut"));

            // create the property list for trigger input
            plTriggerInp = gcnew TriggerInpList(EIB_MAX_TRIGGER_INP_NUMBER);
            pcTriggerInp = gcnew PropertyEntryCollection();

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_INP_NUMBER; i ++)
            {
                // create the property list for trigger input
                plTriggerInp[i] = gcnew PropTriggerInp(guiSupport, parameter, i);

                IndexList ^ indexList = gcnew IndexList(1);
                indexList[0] = i;
                String ^ text =        String::Format(resManager->GetString(L"textTriggerInp"),        i + 1);
                String ^ description = String::Format(resManager->GetString(L"descriptionTriggerInp"), i + 1);
                pcTriggerInp->Add(gcnew PropertyEntry(
                                                L"TriggerInp", indexList, PropTriggerInp::typeid,
                                                categoryExtTriggerInp,
                                                text,
                                                description,
                                                TriggerInpConverter::typeid));
            }
            // create the property list for trigger input
            plTriggerOut = gcnew TriggerOutList(EIB_MAX_TRIGGER_OUT_NUMBER);
            pcTriggerOut = gcnew PropertyEntryCollection();

            for (UInt32 i = 0; i < EIB_MAX_TRIGGER_OUT_NUMBER; i ++)
            {
                // create the property list for trigger output
                plTriggerOut[i] = gcnew PropTriggerOut(guiSupport, parameter, i);

                IndexList ^ indexList = gcnew IndexList(1);
                indexList[0] = i;
                String ^ text =        String::Format(resManager->GetString(L"textTriggerOut"),        i + 1);
                String ^ description = String::Format(resManager->GetString(L"descriptionTriggerOut"), i + 1);
                pcTriggerOut->Add(gcnew PropertyEntry(
                                                L"TriggerOut", indexList, PropTriggerOut::typeid,
                                                categoryExtTriggerOut,
                                                text,
                                                description,
                                                TriggerOutConverter::typeid));
            }
            // create the property entry for master trigger
            peMasterTrigger = gcnew PropertyEntry(
                                            L"MasterTriggerSource", EIB7_MASTER_TRIGGER_SOURCE::typeid,
                                            categoryMasterTrigger,
                                            resManager->GetString(L"textMasterTriggerSource"),
                                            resManager->GetString(L"descriptionMasterTriggerSource"),
                                            EIB_DEFAULT_MASTER_TRIGGER_SOURCE,
                                            MasterTriggerSourceConverter::typeid);
            // create the property list for axis trigger
            plTriggerAxis = gcnew TriggerAxisList(EIB_MAX_AXIS_NUMBER);
            pcTriggerAxis = gcnew PropertyEntryCollection();

            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                // create the property list for axis trigger
                plTriggerAxis[i] = gcnew PropTriggerAxis(guiSupport, parameter, i, 0);

                IndexList ^ indexList = gcnew IndexList(1);
                indexList[0] = i;
                String ^ text =        String::Format(resManager->GetString(L"textTriggerAxis"),        i + 1);
                String ^ description = String::Format(resManager->GetString(L"descriptionTriggerAxis"), i + 1);
                pcTriggerAxis->Add(gcnew PropertyEntry(
                                                L"TriggerAxis", indexList, PropTriggerAxis::typeid,
                                                categoryTriggerAxis,
                                                text,
                                                description,
                                                TriggerAxisConverter::typeid));
            }
            // create the property list for auxiliary axis trigger
            plTriggerAuxAxis = gcnew TriggerAxisList(EIB_MAX_AUX_AXIS_NUMBER);
            pcTriggerAuxAxis = gcnew PropertyEntryCollection();

            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                // create the property list for axis trigger
                plTriggerAuxAxis[i] = gcnew PropTriggerAxis(guiSupport, parameter, i, EIB_MAX_AUX_AXIS_NUMBER);

                IndexList ^ indexList = gcnew IndexList(1);
                indexList[0] = i;
                String ^ text;
                String ^ description;
                #pragma warning(push)
                #pragma warning(disable: 4127)
                if (EIB_MAX_AUX_AXIS_NUMBER == 1)
                {
                    text =        resManager->GetString(L"textTriggerAuxAxis");
                    description = resManager->GetString(L"descriptionTriggerAuxAxis");
                }
                else
                {
                    text =        String::Format(resManager->GetString(L"textTriggerAuxAxisN"),        i + 1);
                    description = String::Format(resManager->GetString(L"descriptionTriggerAuxAxisN"), i + 1);
                }
                #pragma warning(pop)
                pcTriggerAuxAxis->Add(gcnew PropertyEntry(
                                                L"TriggerAuxAxis", indexList, PropTriggerAxis::typeid,
                                                categoryTriggerAxis,
                                                text,
                                                description,
                                                TriggerAxisConverter::typeid));
            }
            // create the property entry for timer trigger
            TimerTriggerPeriodConverter::eibMinTimerTriggerPeriod = EIB_MIN_TIMER_TRIGGER_PERIOD_OPERATE;
            if (guiSupport->GuiConfig->GuiParam->AdminMode == true)
            {
                TimerTriggerPeriodConverter::eibMinTimerTriggerPeriod = EIB_MIN_TIMER_TRIGGER_PERIOD_ADMIN;
            }
            peTimerTrigger = gcnew PropertyEntry(
                                            L"TimerTriggerPeriod", UInt32::typeid,
                                            categoryTimerTrigger,
                                            resManager->GetString(L"textTriggerPeriode"),
                                            resManager->GetString(L"descriptionTriggerPeriode"),
                                            EIB_DEFAULT_TIMER_TRIGGER_PERIOD,
                                            TimerTriggerPeriodConverter::typeid);
            // create the property entry for interval counter mode
            peIcMode = gcnew PropertyEntry(
                                            L"IntervalCounterMode", EIB7_INTERVAL_COUNTER_MODE::typeid,
                                            categoryIntervalCounter,
                                            resManager->GetString(L"textIntervalCounterMode"),
                                            resManager->GetString(L"descriptionIntervalCounterMode"),
                                            EIB_DEFAULT_INTERVAL_COUNTER_MODE,
                                            IntervalCounterModeConverter::typeid);
            // create the property entry for interval counter start option
            peIcStartOpt = gcnew PropertyEntry(
                                            L"IntervalCounterStart", EIB7_INTERVAL_COUNTER_START::typeid,
                                            categoryIntervalCounter,
                                            resManager->GetString(L"textIntervalCounterStart"),
                                            resManager->GetString(L"descriptionIntervalCounterStart"),
                                            EIB_DEFAULT_INTERVAL_COUNTER_START,
                                            IntervalCounterStartConverter::typeid);
            // create the property entry for interval counter start position
            peIcStartPos = gcnew PropertyEntry(
                                            L"IntervalCounterStartPos", UInt32::typeid,
                                            categoryIntervalCounter,
                                            resManager->GetString(L"textIntervalCounterStartPos"),
                                            resManager->GetString(L"descriptionIntervalCounterStartPos"),
                                            (Object ^)EIB_DEFAULT_INTERVAL_COUNTER_START_POS,
                                            IntervalCounterStartPosConverter::typeid);
            // create the property entry for interval counter period
            peIcPeriod = gcnew PropertyEntry(
                                            L"IntervalCounterPeriod", UInt32::typeid,
                                            categoryIntervalCounter,
                                            resManager->GetString(L"textIntervalCounterPeriod"),
                                            resManager->GetString(L"descriptionIntervalCounterPeriod"),
                                            EIB_DEFAULT_INTERVAL_COUNTER_PERIOD,
                                            IntervalCounterPeriodConverter::typeid);
            // create the property list for pulse counter
            pePulseCounter = gcnew PropertyEntryCollection();
            pePulseCounter->Add(gcnew PropertyEntry(
                                            L"PulseCounterStart", EIB7_PULSE_COUNTER_START::typeid,
                                            categoryPulseCounter,
                                            resManager->GetString(L"textPulseCounterStart"),
                                            resManager->GetString(L"descriptionPulseCounterStart"),
                                            EIB_DEFAULT_PULSE_COUNTER_START,
                                            PulseCounterStartConverter::typeid));
            pePulseCounter->Add(gcnew PropertyEntry(
                                            L"PulseCounterCount", UInt32::typeid,
                                            categoryPulseCounter,
                                            resManager->GetString(L"textPulseCounterCount"),
                                            resManager->GetString(L"descriptionPulseCounterCount"),
                                            EIB_DEFAULT_PULSE_COUNTER_COUNT,
                                            PulseCounterCountConverter::typeid));
            pePulseCounter->Add(gcnew PropertyEntry(
                                            L"PulseCounterTrigger", EIB7_PULSE_COUNTER_TRIGGER::typeid,
                                            categoryPulseCounter,
                                            resManager->GetString(L"textPulseCounterTrigger"),
                                            resManager->GetString(L"descriptionPulseCounterTrigger"),
                                            EIB_DEFAULT_PULSE_COUNTER_TRIGGER,
                                            PulseCounterTriggerConverter::typeid));
        }

        /// <summary>
        /// Gets the EIB extern trigger input class.
        /// </summary>
        property PropTriggerInp ^ TriggerInp[UInt32]
        {
            PropTriggerInp ^ get(UInt32 index) { return plTriggerInp[index]; }
        }

        /// <summary>
        /// Gets the EIB extern trigger output class.
        /// </summary>
        property PropTriggerOut ^ TriggerOut[UInt32]
        {
            PropTriggerOut ^ get(UInt32 index) { return plTriggerOut[index]; }
        }

        /// <summary>
        /// Gets the EIB axis trigger class.
        /// </summary>
        property PropTriggerAxis ^ TriggerAxis[UInt32]
        {
            PropTriggerAxis ^ get(UInt32 index) { return plTriggerAxis[index]; }
        }

        /// <summary>
        /// Gets the EIB auxiliary axis trigger class.
        /// </summary>
        property PropTriggerAxis ^ TriggerAuxAxis[UInt32]
        {
            PropTriggerAxis ^ get(UInt32 index) { return plTriggerAuxAxis[index]; }
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            // refresh trigger input parameters
            for (UInt32 i = 0; i < parameter->Trigger->TriggerInpNumber; i ++)
            {
                plTriggerInp[i]->RefreshParam();
            }
            // refresh trigger output parameters
            for (UInt32 i = 0; i < parameter->Trigger->TriggerOutNumber; i ++)
            {
                plTriggerOut[i]->RefreshParam();
            }
            // refresh axis trigger parameters
            for (UInt32 i = 0; i < parameter->AxisNumber; i ++)
            {
                plTriggerAxis[i]->RefreshParam();
            }
            // refresh auxiliary axis trigger parameters
            for (UInt32 i = 0; i < parameter->AuxAxisNumber; i ++)
            {
                plTriggerAuxAxis[i]->RefreshParam();
            }
            // set input state
            UInt32 inputState = PROPERTY_TRIGGER_STATE_INIT_READY;
            if (parameter->Trigger->IntervalCounterMode != EIB7_INTERVAL_COUNTER_MODE::EIB7_ICM_Disable)
            {
                // interval counter is active
                inputState |= PROPERTY_TRIGGER_STATE_ACTIVE;
                if (parameter->Trigger->IntervalCounterStart == EIB7_INTERVAL_COUNTER_START::EIB7_ICS_StartPos)
                {
                    // interval counter start is start pos
                    inputState |= PROPERTY_TRIGGER_STATE_START_POS;
                }
                if (parameter->Trigger->IntervalCounterMode == EIB7_INTERVAL_COUNTER_MODE::EIB7_ICM_Periodic)
                {
                    // interval counter mode is periodic
                    inputState |= PROPERTY_TRIGGER_STATE_PERIODIC;
                }
            }
            // don't change parameters on first call
            if (currentState == CURRENT_STATE_INIT)
            {
                currentState |= inputState & ~PROPERTY_TRIGGER_STATE_INIT_READY;
            }
            // check current state
            if (currentState != inputState)
            {
                UInt32 changeMask;
                // set property list
                Properties->Clear();
                // add trigger input parameters
                for (UInt32 i = 0; i < parameter->Trigger->TriggerInpNumber; i ++)
                {
                    Properties->Add(pcTriggerInp[i]);
                }
                // add trigger output parameters
                for (UInt32 i = 0; i < parameter->Trigger->TriggerOutNumber; i ++)
                {
                    Properties->Add(pcTriggerOut[i]);
                }
                // add master trigger parameters
                Properties->Add(peMasterTrigger);
                // add axis trigger parameters
                for (UInt32 i = 0; i < parameter->AxisNumber; i ++)
                {
                    Properties->Add(pcTriggerAxis[i]);
                }
                // add auxiliary axis trigger parameters
                for (UInt32 i = 0; i < parameter->AuxAxisNumber; i ++)
                {
                    Properties->Add(pcTriggerAuxAxis[i]);
                }
                // add timer trigger parameters
                Properties->Add(peTimerTrigger);
                // add interval counter mode
                Properties->Add(peIcMode);
                if ((inputState & PROPERTY_TRIGGER_STATE_ACTIVE) != 0)
                {
                    // add interval counter start option
                    Properties->Add(peIcStartOpt);

                    changeMask = PROPERTY_TRIGGER_STATE_ACTIVE;
                    if (((currentState ^ inputState) & changeMask) != 0)
                    {
                        parameter->Trigger->IntervalCounterStart = EIB_DEFAULT_INTERVAL_COUNTER_START;
                    }
                    if ((inputState & PROPERTY_TRIGGER_STATE_START_POS) != 0)
                    {
                        // add interval counter start position
                        Properties->Add(peIcStartPos);

                        changeMask =   PROPERTY_TRIGGER_STATE_ACTIVE
                                     | PROPERTY_TRIGGER_STATE_START_POS;
                        if (((currentState ^ inputState) & changeMask) != 0)
                        {
                            parameter->Trigger->IntervalCounterStartPos = EIB_DEFAULT_INTERVAL_COUNTER_START_POS;
                        }
                    }
                    if ((inputState & PROPERTY_TRIGGER_STATE_PERIODIC) != 0)
                    {
                        // add interval counter period
                        Properties->Add(peIcPeriod);

                        changeMask =   PROPERTY_TRIGGER_STATE_ACTIVE
                                     | PROPERTY_TRIGGER_STATE_PERIODIC;
                        if (((currentState ^ inputState) & changeMask) != 0)
                        {
                            parameter->Trigger->IntervalCounterPeriod = EIB_DEFAULT_INTERVAL_COUNTER_PERIOD;
                        }
                    }
                }
                // add pulse counter parameters
                Properties->AddRange(pePulseCounter);
                // set new state
                currentState = inputState;
            }
            // refresh the property grid
            PropertyBase::RefreshParam();
            // expand grid items
            if (firstShow == true)
            {
//                ThePropertyGrid->ExpandAllGridItems();
                firstShow = false;
            }
        }

        /// <summary>
		/// This member overrides PropertyClass.OnGetValue.
		/// </summary>
		virtual void OnGetValue(PropertyEntryEventArgs ^ e) override
		{
            // set parameter class depending from parameter
            if (   (e->Property->Name == L"TriggerInp")
                || (e->Property->Name == L"TriggerOut")
                || (e->Property->Name == L"TriggerAxis")
                || (e->Property->Name == L"TriggerAuxAxis"))
            {
                paramClass = this;
            }
            else
            {
                paramClass = parameter->Trigger;
            }
            PropertyBase::OnGetValue(e);
		}

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            // set parameter class depending from parameter
            if (   (e->Property->Name == L"TriggerInp")
                || (e->Property->Name == L"TriggerOut")
                || (e->Property->Name == L"TriggerAxis")
                || (e->Property->Name == L"TriggerAuxAxis"))
            {
                paramClass = this;
            }
            else
            {
                paramClass = parameter->Trigger;
                // show not supported error for software trigger selection
                if (   (   (e->Property->Name == L"MasterTriggerSource")
                        && (   ((EIB7_MASTER_TRIGGER_SOURCE)(e->Value) == EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgSW1)
                            || ((EIB7_MASTER_TRIGGER_SOURCE)(e->Value) == EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgSW2)
                            || ((EIB7_MASTER_TRIGGER_SOURCE)(e->Value) == EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgSW3)
                            || ((EIB7_MASTER_TRIGGER_SOURCE)(e->Value) == EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgSW4)))
                    || (   (e->Property->Name == L"PulseCounterStart")
                        && (   ((EIB7_PULSE_COUNTER_START)(e->Value) == EIB7_PULSE_COUNTER_START::EIB7_PS_TrgSW1)
                            || ((EIB7_PULSE_COUNTER_START)(e->Value) == EIB7_PULSE_COUNTER_START::EIB7_PS_TrgSW2)
                            || ((EIB7_PULSE_COUNTER_START)(e->Value) == EIB7_PULSE_COUNTER_START::EIB7_PS_TrgSW3)
                            || ((EIB7_PULSE_COUNTER_START)(e->Value) == EIB7_PULSE_COUNTER_START::EIB7_PS_TrgSW4))))
                {
                    // save old parameter value
                    PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
                    Object ^ oldValue = propertyInfo->GetValue(paramClass, e->Property->Indices);
                    // set new parameter value
                    PropertyBase::OnSetValue(e);
                    // show not supported error
                    guiSupport->MsgBoxErrorOk(resManager->GetString(L"errorSelectionNotSupported"));
                    // restore old parameter value
                    e->Value = oldValue;
                }
            }
            PropertyBase::OnSetValue(e);
		}
    };
	#pragma endregion
}
