///
/// \file       PropertyAuxAxis.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyAuxAxis.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: PropertyAuxAxis.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB axis parameters input class.
///
/// This class is used for the management of the EIB auxiliary axis
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
 	#pragma region "IntervalCounterIpfConverter class definition"
	/// <summary>
	/// This class definies a type converter for an interval counter ipf enum.
    /// enum <--> String.
	/// </summary>
    public ref class IntervalCounterIpfConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        IntervalCounterIpfConverter() : LocalEnumConverter(EIB7_INTERVAL_COUNTER_IPF::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "IntervalCounterEdgeConverter class definition"
	/// <summary>
	/// This class definies a type converter for an interval counter edge enum.
    /// enum <--> String.
	/// </summary>
    public ref class IntervalCounterEdgeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        IntervalCounterEdgeConverter() : LocalEnumConverter(EIB7_INTERVAL_COUNTER_EDGE::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "EnabledConverter class definition"
	/// <summary>
	/// This class definies a type converter for enabled / disabled selection.
    /// bool <--> String.
	/// </summary>
    public ref class EnabledConverter : public BoolSelectConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EnabledConverter() : BoolSelectConverter(L"textEnabled",
                                                 L"textDisabled")
        {
        }
    };
	#pragma endregion

 	#pragma region "PropertyAuxAxis class definition"
    /// <summary>
    /// This class is used for the management of the EIB auxiliary axis
    /// parameters input via the axis property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropertyAuxAxis : public PropertyAxisBase
    {
    private:
        EIB_AuxAxis_Param ^         auxAxisParameter;   ///< auxiliary axis parameters of the saved EIB

        PropertyEntryCollection ^   pcAuxAxisParam;     ///< the property list for auxiliary axis parameters
        PropertyEntryCollection ^   pcAuxAxisDisable;   ///< the property list for disabled auxiliary axis

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="auxAxisParameter">The auxiliary axis parameters of the saved EIB.</param>
        PropertyAuxAxis(GUI_Support ^       guiSupport,
                        EIB_AuxAxis_Param ^ auxAxisParameter)
            : PropertyAxisBase(guiSupport,
                               auxAxisParameter,
                               auxAxisParameter->Axis,
                               2,
                               true)
        {
            this->auxAxisParameter = auxAxisParameter;
            // set auxiliary axis parameter class
            paramClass = auxAxisParameter;

            // create category strings
            String ^ categoryIntervalCounter =
                                String::Format(CATEGORY_HEADER,
                                               1, resManager->GetString(L"categoryIntervalCounter"));
            String ^ categoryAxisParameters =
                                String::Format(CATEGORY_HEADER,
                                               1, resManager->GetString(L"categoryAxisParameters"));
            // create the property list for auxiliary axis parameters
            pcAuxAxisParam = gcnew PropertyEntryCollection();
            pcAuxAxisParam->Add(gcnew PropertyEntry(L"IntervalCounterIpf", EIB7_INTERVAL_COUNTER_IPF::typeid,
                                                    categoryIntervalCounter,
                                                    resManager->GetString(L"textInterpolationFactor"),
                                                    resManager->GetString(L"descriptionInterpolationFactor"),
                                                    EIB_DEFAULT_INTERVAL_COUNTER_IPF,
                                                    IntervalCounterIpfConverter::typeid));
            pcAuxAxisParam->Add(gcnew PropertyEntry(L"IntervalCounterEdge", EIB7_INTERVAL_COUNTER_EDGE::typeid,
                                                    categoryIntervalCounter,
                                                    resManager->GetString(L"textEdgeEvaluation"),
                                                    resManager->GetString(L"descriptionEdgeEvaluation"),
                                                    EIB_DEFAULT_INTERVAL_COUNTER_EDGE,
                                                    IntervalCounterEdgeConverter::typeid));

            // create the property list for disabled auxiliary axis
            pcAuxAxisDisable = gcnew PropertyEntryCollection();
            pcAuxAxisDisable->Add(gcnew PropertyEntry(L"IsEnabled", bool::typeid, true,
                                                      categoryAxisParameters,
                                                      resManager->GetString(L"textActivation"),
                                                      resManager->GetString(L"descriptionActivation"),
                                                      false,
                                                      EnabledConverter::typeid));
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            RefreshParam(true);
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
		/// <param name="refresh">If true refresh the property grid.</param>
        virtual void RefreshParam(bool refresh) override
        {
            // set new input state
            SetInputState(auxAxisParameter->IsEnabled);
            // don't change parameters on first call
            if (currentState == CURRENT_STATE_INIT)
            {
                currentState |= inputState & ~PROPERTY_AXIS_STATE_INIT_READY;
            }
            // check current state
            if (currentState != inputState)
            {
                // set axis property list
                Properties->Clear();
                if ((inputState & PROPERTY_AXIS_STATE_ENABLE) != 0)
                {
                    // axis is enabled
                    // set auxiliary axis parameters property list
                    Properties->AddRange(pcAuxAxisParam);
                }
                else
                {
                    // axis is disabled
                    // set disabled auxiliary axis property list
                    Properties->AddRange(pcAuxAxisDisable);
                }
                // refresh the datapacket parameters
                PropertyAxisBase::RefreshParam();
                // set new state
                currentState = inputState;
            }
            // refresh the property grid
            PropertyBase::RefreshParam(refresh);
        }
    };
	#pragma endregion
}
