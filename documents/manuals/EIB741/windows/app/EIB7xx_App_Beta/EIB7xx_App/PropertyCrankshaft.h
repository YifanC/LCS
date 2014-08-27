///
/// \file       PropertyCrankshaft.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyCrankshaft.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: PropertyCrankshaft.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB crankshaft configuration parameters input class.
///
/// This class is used for the management of the EIB crankshaft configuration
/// parameters input via the crankshaft configuration property grid. It manages
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
 	#pragma region "EibAxisSelectConverter class definition"
    /// special definitions
    typedef List<String ^>     EibAxisNameList;

	/// <summary>
	/// This class definies a type converter for an EIB / axis selection.
	/// </summary>
    public ref class EibAxisSelectConverter : public ListSelectConverter 
    {
    public:
        static EibAxisNameList ^ eibAxisNames;  ///< static EIB / axis name list

        /// <summary>
        /// Constructor.
        /// </summary>
        EibAxisSelectConverter() : ListSelectConverter(eibAxisNames)
        {
        }
    };
	#pragma endregion

    #pragma region "PropertyCrankshaft class definition"
    /// <summary>
    /// This class is used for the management of the EIB crankshaft configuration
    /// parameters input via the crankshaft configuration property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropertyCrankshaft : public PropertyBase
    {
    private:
        EIB_TreeList ^      eibTreeList;    ///< the EIB tree list

        EibAxisValueList ^  eibAxisValues;  ///< the EIB / axis values 

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="eibTreeList">The EIB tree list.</param>
        PropertyCrankshaft(GUI_Support ^  guiSupport,
                           EIB_TreeList ^ eibTreeList)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridCrankshaft)
        {
            this->eibTreeList = eibTreeList;

            // initialize EIB / axis name and value lists
            EibAxisSelectConverter::eibAxisNames  = gcnew EibAxisNameList;
            eibAxisValues                         = gcnew EibAxisValueList;

            // set "not used" entry
            EibAxisSelectConverter::eibAxisNames->Add(resManager->GetString(L"textNotUsed"));
            eibAxisValues->Add(gcnew EibAxis(CRANKSHAFT_NOT_USED_EIB_INDEX,
                                             CRANKSHAFT_NOT_USED_AXIS_INDEX));

            // create category strings
            String ^ categoryCrankshaft = resManager->GetString(L"categoryCrankshaftConfig");
            // create the property list for the input parameters
            Properties->Add(gcnew PropertyEntry(L"AngelMeasure", String::typeid,
                                                categoryCrankshaft,
                                                resManager->GetString(L"textAngelMeasure"),
                                                resManager->GetString(L"descriptionAngelMeasure"),
                                                EibAxisSelectConverter::eibAxisNames[0],
                                                EibAxisSelectConverter::typeid));

            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                IndexList ^ indexList = gcnew IndexList(1);
                indexList[0] = i;
                String ^ text =        String::Format(resManager->GetString(L"textMeasurePoint"),        i + 1);
                String ^ description = String::Format(resManager->GetString(L"descriptionMeasurePoint"), i + 1);
                Properties->Add(gcnew PropertyEntry(
                                                L"MeasurePoint", indexList, String::typeid,
                                                categoryCrankshaft,
                                                text,
                                                description,
                                                EibAxisSelectConverter::eibAxisNames[0],
                                                EibAxisSelectConverter::typeid));
            }

            // set parameter class
            paramClass = eibTreeList->Common->CrankshaftParam;

            // refresh the property grid
            RefreshParam();
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            // set EIB / axis name and value lists depending from current EIB tree
            EibAxisSelectConverter::eibAxisNames->Clear();
            eibAxisValues->Clear();
            // set "not used" entry
            EibAxisSelectConverter::eibAxisNames->Add(resManager->GetString(L"textNotUsed"));
            eibAxisValues->Add(gcnew EibAxis(CRANKSHAFT_NOT_USED_EIB_INDEX,
                                             CRANKSHAFT_NOT_USED_AXIS_INDEX));
            // set current entries from EIB tree
            EIB_Entry ^ eibEntry;
            UInt32 axisNumber;
            for (UInt32 i = 0; i < eibTreeList->EibCount; i ++)
            {
                eibEntry   = eibTreeList->EibEntry[i];
                axisNumber = eibEntry->Parameter->AxisNumber;
                for (UInt32 j = 0; j < axisNumber; j ++)
                {
                    EibAxisSelectConverter::eibAxisNames->Add(eibEntry->EibAxisName[j + 1]);
                    eibAxisValues->Add(gcnew EibAxis(i + 1, j + 1));
                }
            }
            // check for not valid parameters
            EibAxisValueList ^ notValidValues = eibTreeList->CheckCrankshaftParam();
            if (notValidValues != nullptr)
            {
                for each (EibAxis ^ value in notValidValues)
                {
                    // set not valid parameter to not used
                    value->Copy(eibAxisValues[0]);
                }
            }
            // refresh the property grid
            PropertyBase::RefreshParam();
        }

        /// <summary>
		/// This member overrides PropertyClass.OnGetValue.
		/// </summary>
		virtual void OnGetValue(PropertyEntryEventArgs ^ e) override
		{
            // get parameter reference
            PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
            EibAxis ^ value = (EibAxis ^)(propertyInfo->GetValue(paramClass, e->Property->Indices));
            // convert EIB / axis value to name
            e->Value = EibAxisSelectConverter::eibAxisNames[0];
            for (Int32 i = 0; i < eibAxisValues->Count; i ++)
            {
                if (value->IsEqual(eibAxisValues[i]) == true)
                {
                    e->Value = EibAxisSelectConverter::eibAxisNames[i];
                    break;
                }
            }
	        PropertyClass::OnGetValue(e);
		}

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            // get parameter reference
            PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
            EibAxis ^ value = (EibAxis ^)(propertyInfo->GetValue(paramClass, e->Property->Indices));
            // save old parameter value
            EibAxis oldValue;
            oldValue.Copy(value);
            // convert EIB / axis name to value
            String ^ name = (String ^)(e->Value);
            value->Copy(eibAxisValues[0]);
            for (Int32 i = 0; i < EibAxisSelectConverter::eibAxisNames->Count; i ++)
            {
                if (name == EibAxisSelectConverter::eibAxisNames[i])
                {
                    value->Copy(eibAxisValues[i]);
                }
            }
            PropertyClass::OnSetValue(e);
            // check if angle measure and measure point have same configuration
            if (e->Property->Name == L"MeasurePoint")
            {
                EibAxis ^ angleValue = eibTreeList->Common->CrankshaftParam->AngelMeasure;
                if (   (value->IsNotUsed() == false)
                    && (value->IsEqual(angleValue) == true))
                {
                    // value is already used
                    guiSupport->MsgBoxErrorOk(resManager->GetString(L"errorValueAlreadyUsed"));
                    // restore old value
                    value->Copy(%oldValue);
                }
            }
            else if (e->Property->Name == L"AngelMeasure")
            {
                if (value->IsNotUsed() == false)
                {
                    EibAxis ^ pointValue;
                    for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
                    {
                        pointValue = eibTreeList->Common->CrankshaftParam->MeasurePoint[i];
                        if (value->IsEqual(pointValue) == true)
                        {
                            // value is already used
                            guiSupport->MsgBoxErrorOk(resManager->GetString(L"errorValueAlreadyUsed"));
                            // restore old value
                            value->Copy(%oldValue);
                            break;
                        }
                    }
                }
            }
            PropertyClass::OnSetValue(e);
		}
    };
    #pragma endregion
}
