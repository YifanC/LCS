///
/// \file       PropertyOpMode.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyOpMode.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: PropertyOpMode.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB operation mode parameters input class.
///
/// This class is used for the management of the EIB operation mode
/// parameters input via the operation mode property grid. It manages
/// the parameter dependencies for the property grid.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Resources;
using namespace System::Reflection;
using namespace System::Windows::Forms;

namespace EIB7xx_App
{
 	#pragma region "TimestampPeriodConverter class definition"
	/// <summary>
	/// This class definies a type converter for a timestamp period value.
    /// UInt32 <--> String.
	/// </summary>
    public ref class TimestampPeriodConverter : public UInt32ValueConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TimestampPeriodConverter() : UInt32ValueConverter(EIB_MIN_TIMESTAMP_PERIOD,
                                                          EIB_MAX_TIMESTAMP_PERIOD,
                                                          EIB_STEP_TIMESTAMP_PERIOD)
        {
        }
    };
	#pragma endregion

 	#pragma region "TimestampResetConverter class definition"
	/// <summary>
	/// This class definies a type converter for a reset timestamp selection.
    /// bool <--> String.
	/// </summary>
    public ref class TimestampResetConverter : public BoolSelectConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        TimestampResetConverter() : BoolSelectConverter(L"textResetOnStart",
                                                        L"textNoResetOnStart")
        {
        }
    };
	#pragma endregion

 	#pragma region "PropertyOpMode class definition"
    /// <summary>
    /// This class is used for the management of the EIB operation mode
    /// parameters input via the operation mode property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropertyOpMode : public PropertyBase
    {
    private:
        EIB_TreeList ^      eibTreeList;    ///< the EIB tree list

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="eibTreeList">The EIB tree list.</param>
        PropertyOpMode(GUI_Support ^  guiSupport,
                       EIB_TreeList ^ eibTreeList)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridOpMode)
        {
            this->eibTreeList = eibTreeList;

            // create category strings
            String ^ categoryOpMode =
                                String::Format(CATEGORY_HEADER,
                                               1, resManager->GetString(L"categoryOpMode"));
            String ^ categoryTimestamp =
                                String::Format(CATEGORY_HEADER,
                                               2, resManager->GetString(L"categoryTimestamp"));
            // create the property list for the input parameters
            Properties->Add(gcnew PropertyEntry(L"OpMode", EIB7_MODE_OPERATING::typeid,
                                                categoryOpMode,
                                                resManager->GetString(L"textOpMode"),
                                                resManager->GetString(L"descriptionOpMode"),
                                                EIB_DEFAULT_OP_MODE,
                                                OpModeConverter::typeid));
            Properties->Add(gcnew PropertyEntry(L"TimestampPeriod", UInt32::typeid,
                                                categoryTimestamp,
                                                resManager->GetString(L"textTimestampPeriod"),
                                                resManager->GetString(L"descriptionTimestampPeriod"),
                                                EIB_DEFAULT_TIMESTAMP_PERIOD,
                                                TimestampPeriodConverter::typeid));
            Properties->Add(gcnew PropertyEntry(L"TimestampReset", bool::typeid,
                                                categoryTimestamp,
                                                resManager->GetString(L"textTimestampReset"),
                                                resManager->GetString(L"descriptionTimestampReset"),
                                                EIB_DEFAULT_TIMESTAMP_RESET,
                                                TimestampResetConverter::typeid));

            // set parameter class
            paramClass = eibTreeList->Common->OpMode;

            // refresh the property grid
            RefreshParam();
        }

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            // show warning for setting this parameter for all EIBs
            if (   (   (e->Property->Name == L"OpMode")
                    || (e->Property->Name == L"TimestampPeriod")
                    || (e->Property->Name == L"TimestampReset"))
                && (eibTreeList->EibCount > 1))
            {
                // save old parameter value
                PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
                Object ^ oldValue = propertyInfo->GetValue(paramClass, e->Property->Indices);
                // set new parameter value
                PropertyBase::OnSetValue(e);
                // ask for setting this parameter for all EIBs
                Windows::Forms::DialogResult result =
                            guiSupport->MsgBoxWarning(resManager->GetString(L"warningSetForAllEibs"),
                                                      MessageBoxButtons::YesNo,
                                                      MessageBoxDefaultButton::Button1);
                if (result == ::DialogResult::No)
                {
                    // restore old parameter value
                    e->Value = oldValue;
                }
            }
            // set new parameter values
            PropertyBase::OnSetValue(e);
		}
    };
	#pragma endregion
}
