///
/// \file       PropertyBase.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyBase.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: PropertyBase.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB parameters input base class.
///
/// This class contains some parameters
/// for the EIB parameters input classes.
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
 	#pragma region "LocalEnumConverter class definition"
	/// <summary>
	/// This class definies a type converter for an enum class.
    /// enum <--> String
    /// as a base class for a specific enum class.
	/// </summary>
    public ref class LocalEnumConverter : public EnumConverter 
    {
    protected:
        ResourceManager ^   resManager; ///< resource manger of the main form

        String ^            keySuffix;  ///< suffix for the value key strings

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="type">The enum type.</param>
        LocalEnumConverter(Type ^ type) : EnumConverter(type)
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();
            // set the suffix for the value key strings
            this->keySuffix = L"";
        }

        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="type">The enum type.</param>
		/// <param name="keySuffix">The suffix for the value key strings.</param>
        LocalEnumConverter(Type ^ type, String ^ keySuffix) : EnumConverter(type)
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();
            // set the suffix for the value key strings
            this->keySuffix = keySuffix;
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// This function returns the prefix for the value key strings.
        /// </summary>
        virtual String ^ GetPrefix(Object ^ value)
        {
            return L"";
        }

        /// <summary>
        /// This function returns the value key string.
        /// </summary>
        virtual String ^ GetValueText(ITypeDescriptorContext ^ context,
                                      Object ^ value)
        {
            return   GetPrefix(value)
                   + resManager->GetString(value->ToString() + keySuffix);
        }
        #pragma warning(pop)

        /// <summary>
        /// This function returns true if the convertion
        /// enum --> String is possible.
        /// </summary>
        virtual bool CanConvertTo(ITypeDescriptorContext ^ context,
                                  Type ^ destinationType) override
        {
            return (   (destinationType == EnumType)
                    || EnumConverter::CanConvertTo(context, destinationType));
        }

        /// <summary>
        /// This function performs the convertion enum --> String.
        /// </summary>
        virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context,
                                   CultureInfo ^ culture, 
                                   Object ^ value, 
                                   Type ^ destinationType) override
        {
            if (   (destinationType == String::typeid)
                && (value->GetType() == EnumType))
            {
                return GetValueText(context, value);
            }
            return EnumConverter::ConvertTo(context, culture, value, destinationType);
        }

        /// <summary>
        /// This function returns true if the convertion
        /// String --> enum is possible.
        /// </summary>
        virtual bool CanConvertFrom(ITypeDescriptorContext ^ context,
                                    Type ^ sourceType) override
        {
            return (   (sourceType == String::typeid)
                    || EnumConverter::CanConvertFrom(context, sourceType));
        }

        /// <summary>
        /// This function performs the convertion String --> enum.
        /// </summary>
        virtual public Object ^ ConvertFrom(ITypeDescriptorContext ^ context,
                                            CultureInfo ^ culture,
                                            Object ^ value) override
        {
            if (value->GetType() == String::typeid)
            {
                for each (Object ^ enumValue in Values)
                {
                    if ((String ^)value == GetValueText(context, enumValue))
                    {
                        return enumValue;
                    }
                }
                // enum string format exception
                throw gcnew FormatException(resManager->GetString(L"errorEnumText"));
            }
            return EnumConverter::ConvertFrom(context, culture, value);
        }
    };
	#pragma endregion

 	#pragma region "NumericEnumConverter class definition"
	/// <summary>
	/// This class definies a type converter for an numeric enum class.
    /// enum <--> String
    /// as a base class for numeric enum classes.
	/// </summary>
    public ref class NumericEnumConverter : public EnumConverter 
    {
    private:
        ResourceManager ^   resManager; ///< resource manger of the main form

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="type">The enum type.</param>
        NumericEnumConverter(Type ^ type) : EnumConverter(type)
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// This function performs the convertion enum numeric value --> String.
        /// </summary>
        virtual String ^ EnumNumericValueToString(Object ^ value)
        {
            return L"";
        }
        #pragma warning(pop)

        /// <summary>
        /// This function returns true if the convertion
        /// enum --> String is possible.
        /// </summary>
        virtual bool CanConvertTo(ITypeDescriptorContext ^ context,
                                  Type ^ destinationType) override
        {
            return (   (destinationType == EnumType)
                    || EnumConverter::CanConvertTo(context, destinationType));
        }

        /// <summary>
        /// This function performs the convertion enum --> String.
        /// </summary>
        virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context,
                                   CultureInfo ^ culture, 
                                   Object ^ value, 
                                   Type ^ destinationType) override
        {
            if (   (destinationType == String::typeid)
                && (value->GetType() == EnumType))
            {
                return EnumNumericValueToString(value);
            }
            return EnumConverter::ConvertTo(context, culture, value, destinationType);
        }

        /// <summary>
        /// This function returns true if the convertion
        /// String --> enum is possible.
        /// </summary>
        virtual bool CanConvertFrom(ITypeDescriptorContext ^ context,
                                    Type ^ sourceType) override
        {
            return (   (sourceType == String::typeid)
                    || EnumConverter::CanConvertFrom(context, sourceType));
        }

        /// <summary>
        /// This function performs the convertion String --> enum.
        /// </summary>
        virtual public Object ^ ConvertFrom(ITypeDescriptorContext ^ context,
                                            CultureInfo ^ culture,
                                            Object ^ value) override
        {
            if (value->GetType() == String::typeid)
            {
                for each (Object ^ enumValue in Values)
                {
                    if ((String ^)value == EnumNumericValueToString(enumValue))
                    {
                        return enumValue;
                    }
                }
                // enum string format exception
                throw gcnew FormatException(resManager->GetString(L"errorEnumText"));
            }
            return EnumConverter::ConvertFrom(context, culture, value);
        }
    };
	#pragma endregion

 	#pragma region "UInt32ValueConverter class definition"
	/// <summary>
	/// This class definies a type converter for an UInt32 value class.
    /// UInt32 value <--> String
    /// as a base class for a specific UInt32 value class.
	/// </summary>
    public ref class UInt32ValueConverter : public UInt32Converter 
    {
    private:
        ResourceManager ^   resManager; ///< resource manger of the main form

        UInt32              minValue;   ///< min value for input test
        UInt32              maxValue;   ///< max value for input test
        UInt32              stepValue;  ///< step width value for input test

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="minValue">The min value for input test.</param>
		/// <param name="maxValue">The max value for input test.</param>
		/// <param name="stepValue">The step width value for input test.</param>
        UInt32ValueConverter(UInt32 minValue, UInt32 maxValue, UInt32 stepValue)
            : UInt32Converter()
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();
            // set the input test values
            this->minValue  = minValue;
            this->maxValue  = maxValue;
            this->stepValue = stepValue;
        }

        /// <summary>
        /// This function returns true if the convertion
        /// String --> UInt32 is possible.
        /// </summary>
        virtual bool CanConvertFrom(ITypeDescriptorContext ^ context,
                                    Type ^ sourceType) override
        {
            return (   (sourceType == String::typeid)
                    || UInt32Converter::CanConvertFrom(context, sourceType));
        }

        /// <summary>
        /// This function performs the convertion String --> UInt32.
        /// </summary>
        virtual public Object ^ ConvertFrom(ITypeDescriptorContext ^ context,
                                            CultureInfo ^ culture,
                                            Object ^ value) override
        {
            if (value->GetType() == String::typeid)
            {
                UInt32 newValue = (UInt32)UInt32Converter::ConvertFrom(context, culture, value);
                // check input value
                String ^ error;
                if (newValue < minValue)
                {
                    // new value is less than min value
                    error = String::Format(resManager->GetString(L"errorLessThanMin"), minValue);
                    throw gcnew FormatException(error);
                }
                if (newValue > maxValue)
                {
                    // new value is greater than max value
                    error = String::Format(resManager->GetString(L"errorGreaterThanMax"), maxValue);
                    throw gcnew FormatException(error);
                }
                if ((newValue % stepValue) != 0)
                {
                    // new value has bad step width
                    error = String::Format(resManager->GetString(L"errorBadStepWidth"), stepValue);
                    throw gcnew FormatException(error);
                }
            }
            return UInt32Converter::ConvertFrom(context, culture, value);
        }
    };
	#pragma endregion

 	#pragma region "DoubleValueConverter class definition"
	/// <summary>
	/// This class definies a type converter for an Double value class.
    /// Double value <--> String
    /// as a base class for a specific Double value class.
	/// </summary>
    public ref class DoubleValueConverter : public DoubleConverter 
    {
    private:
        ResourceManager ^   resManager; ///< resource manger of the main form

        Double              minValue;       ///< min value for input test
        Double              maxValue;       ///< max value for input test
        Double              stepValue;      ///< step width value for input test
        String ^            valueFormat;    ///< value format string for output

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="minValue">The min value for input test.</param>
		/// <param name="maxValue">The max value for input test.</param>
		/// <param name="stepValue">The step width value for input test.</param>
		/// <param name="valueFormat">The value format string for output.</param>
        DoubleValueConverter(Double minValue, Double maxValue, Double stepValue,
                             String ^ valueFormat)
            : DoubleConverter()
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();
            // set the input test values
            this->minValue    = minValue;
            this->maxValue    = maxValue;
            this->stepValue   = stepValue;
            this->valueFormat = valueFormat;
        }

        /// <summary>
        /// This function returns true if the convertion
        /// Double --> String is possible.
        /// </summary>
        virtual bool CanConvertTo(ITypeDescriptorContext ^ context,
                                  Type ^ destinationType) override
        {
            return (   (destinationType == Double::typeid)
                    || DoubleConverter::CanConvertTo(context, destinationType));
        }

        /// <summary>
        /// This function performs the convertion Double --> String.
        /// </summary>
        virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context,
                                   CultureInfo ^ culture, 
                                   Object ^ value, 
                                   Type ^ destinationType) override
        {
            if (   (destinationType == String::typeid)
                && (value->GetType() == Double::typeid))
            {
                Double doubleValue = Double(value);
                return doubleValue.ToString(valueFormat, culture);
            }
            return DoubleConverter::ConvertTo(context, culture, value, destinationType);
        }

        /// <summary>
        /// This function returns true if the convertion
        /// String --> Double is possible.
        /// </summary>
        virtual bool CanConvertFrom(ITypeDescriptorContext ^ context,
                                    Type ^ sourceType) override
        {
            return (   (sourceType == String::typeid)
                    || DoubleConverter::CanConvertFrom(context, sourceType));
        }

        /// <summary>
        /// This function performs the convertion String --> Double.
        /// </summary>
        virtual public Object ^ ConvertFrom(ITypeDescriptorContext ^ context,
                                            CultureInfo ^ culture,
                                            Object ^ value) override
        {
            if (value->GetType() == String::typeid)
            {
                Double newValue = (Double)DoubleConverter::ConvertFrom(context, culture, value);
                // check input value
                Double testValue;
                String ^ error;
                testValue = minValue - newValue;
                if (testValue > ZERO_DOUBLE)
                {
                    // new value is less than min value
                    error = String::Format(resManager->GetString(L"errorLessThanMin"), minValue);
                    throw gcnew FormatException(error);
                }
                testValue = newValue - maxValue;
                if (testValue > ZERO_DOUBLE)
                {
                    // new value is greater than max value
                    error = String::Format(resManager->GetString(L"errorGreaterThanMax"), maxValue);
                    throw gcnew FormatException(error);
                }
                testValue = Math::IEEERemainder(newValue, stepValue);
                if ((testValue > ZERO_DOUBLE) || (testValue < -ZERO_DOUBLE))
                {
                    // new value has bad step width
                    error = String::Format(resManager->GetString(L"errorBadStepWidth"), stepValue);
                    throw gcnew FormatException(error);
                }
            }
            return DoubleConverter::ConvertFrom(context, culture, value);
        }
    };
	#pragma endregion

 	#pragma region "BoolSelectConverter class definition"
	/// <summary>
	/// This class definies a type converter for a bool value class.
    /// bool <--> String
    /// as a base class for a specific bool value class.
	/// </summary>
    public ref class BoolSelectConverter : public BooleanConverter 
    {
    private:
        ResourceManager ^   resManager;     ///< resource manger of the main form

        String ^            trueText;       ///< output text for a true value
        String ^            falseText;      ///< output text for a false value

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="trueText">The output text for a true value.</param>
		/// <param name="falseText">The output text for a false value.</param>
        BoolSelectConverter(String ^ trueText, String ^ falseText)
            : BooleanConverter()
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();
            // set the output text values
            this->trueText  = trueText;
            this->falseText = falseText;
        }

        /// <summary>
        /// This function returns true if the convertion
        /// bool --> String is possible.
        /// </summary>
        virtual bool CanConvertTo(ITypeDescriptorContext ^ context,
                                  Type ^ destinationType) override
        {
            return (   (destinationType == bool::typeid)
                    || BooleanConverter::CanConvertTo(context, destinationType));
        }

        /// <summary>
        /// This function performs the convertion bool --> String.
        /// </summary>
        virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context,
                                   CultureInfo ^ culture, 
                                   Object ^ value, 
                                   Type ^ destinationType) override
        {
            if (   (destinationType == String::typeid)
                && (value->GetType() == bool::typeid))
            {
                if ((bool)value == true)
                {
                    return resManager->GetString(trueText);
                }
                return resManager->GetString(falseText);
            }
            return BooleanConverter::ConvertTo(context, culture, value, destinationType);
        }

        /// <summary>
        /// This function returns true if the convertion
        /// String --> UInt32 is possible.
        /// </summary>
        virtual bool CanConvertFrom(ITypeDescriptorContext ^ context,
                                    Type ^ sourceType) override
        {
            return (   (sourceType == String::typeid)
                    || BooleanConverter::CanConvertFrom(context, sourceType));
        }

        /// <summary>
        /// This function performs the convertion String --> UInt32.
        /// </summary>
        virtual public Object ^ ConvertFrom(ITypeDescriptorContext ^ context,
                                            CultureInfo ^ culture,
                                            Object ^ value) override
        {
            if (value->GetType() == String::typeid)
            {
                if ((String ^)value == resManager->GetString(trueText))
                {
                    return true;
                }
                return false;
            }
            return BooleanConverter::ConvertFrom(context, culture, value);
        }
    };
	#pragma endregion

 	#pragma region "TopItemConverter class definition"
	/// <summary>
	/// This class definies a type converter for a class with sub items.
    /// class <--> String
    /// as a base class for a specific classes with sub items.
	/// </summary>
    public ref class TopItemConverter : public ExpandableObjectConverter 
    {
    private:
        Type ^      classType;  ///< class type

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="classType">The class type.</param>
        TopItemConverter(Type ^ classType)
        {
            this->classType = classType;
        }

        /// <summary>
        /// This function returns true if the convertion
        /// class --> String is possible.
        /// </summary>
        virtual bool CanConvertTo(ITypeDescriptorContext ^ context,
                                  Type ^ destinationType) override
        {
            return (   (destinationType == classType)
                    || ExpandableObjectConverter::CanConvertTo(context, destinationType));
        }

        /// <summary>
        /// This function performs the convertion class --> String.
        /// </summary>
        virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context,
                                   CultureInfo ^ culture, 
                                   Object ^ value, 
                                   Type ^ destinationType) override
        {
            if (   (destinationType == String::typeid)
                && (value->GetType() == classType))
            {
                return L"";
            }
            return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
        }
    };
	#pragma endregion

 	#pragma region "ListSelectConverter class definition"
    /// special definitions
    typedef List<String ^>     EibAxisNameList;

	/// <summary>
	/// This class definies a type converter for a value list selection
    /// as a base class for a specific classes with value lists.
	/// </summary>
    public ref class ListSelectConverter : public TypeConverter 
    {
    private:
        Collections::ICollection ^  valueList;  ///< the value list to selct from

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        ListSelectConverter(Collections::ICollection ^ valueList)
        {
            this->valueList = valueList;
        }

        #pragma warning(push)
        #pragma warning(disable: 4100)
        /// <summary>
        /// This function returns true if a standard set of values
        /// is supported.
        /// </summary>
        virtual bool GetStandardValuesSupported(ITypeDescriptorContext ^ context) override
        {
            return true;
        }

        /// <summary>
        /// This function returns true if the collection of standard values
        /// is an exclusive list.
        /// </summary>
        virtual bool GetStandardValuesExclusive(ITypeDescriptorContext ^ context) override
        {
            return true;
        }

        /// <summary>
        /// This function returns the collection of standard values.
        /// </summary>
        virtual StandardValuesCollection ^ GetStandardValues(ITypeDescriptorContext ^ context) override
        {
            return gcnew StandardValuesCollection(valueList);
        }
        #pragma warning(pop)
    };
	#pragma endregion

 	#pragma region "OpModeConverter class definition"
	/// <summary>
	/// This class definies a type converter for an operation mode enum.
    /// enum <--> String.
	/// </summary>
    public ref class OpModeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        OpModeConverter() : LocalEnumConverter(EIB7_MODE_OPERATING::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "PropertyBase class definition"
    // special definitions
    #define CATEGORY_HEADER         L"{0} {1}"
    #define CURRENT_STATE_INIT      UInt32(0)
    
    /// <summary>
    /// This class contains some parameters
    /// for the EIB parameters input classes.
    /// </summary>
    public ref class PropertyBase : public PropertyClass
    {
    private:
        PropertyGrid ^      propertyGrid;   ///< the Property Grid

    protected:
        GUI_Support ^       guiSupport;     ///< the GUI support class
        ResourceManager ^   resManager;     ///< the resource manager from main window
        Object ^            paramClass;     ///< the class that contains the parameters

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="propertyGrid">The Property Grid.</param>
        PropertyBase(GUI_Support ^  guiSupport,
                     PropertyGrid ^ propertyGrid)
        {
            this->guiSupport   = guiSupport;
            this->resManager   = guiSupport->ResManager;
            this->propertyGrid = propertyGrid;
            paramClass         = nullptr;
        }

        /// <summary>
        /// Refresh the property grid.
        /// </summary>
        virtual void RefreshParam()
        {
            RefreshParam(true);
        }

        /// <summary>
        /// Refresh the property grid.
        /// </summary>
		/// <param name="refresh">If true refresh the property grid.</param>
        virtual void RefreshParam(bool refresh)
        {
            if (refresh == true)
            {
                // refresh the property grid
                propertyGrid->SelectedObject = this;
                propertyGrid->Refresh();
            }
        }

		/// <summary>
		/// This member overrides PropertyClass.OnGetValue.
		/// </summary>
		virtual void OnGetValue(PropertyEntryEventArgs ^ e) override
		{
            PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
            e->Value = propertyInfo->GetValue(paramClass, e->Property->Indices);
	        PropertyClass::OnGetValue(e);
		}

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(e->Property->Name);
            propertyInfo->SetValue(paramClass, e->Value, e->Property->Indices);
	        PropertyClass::OnSetValue(e);
		}
    };
	#pragma endregion
}
