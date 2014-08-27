///
/// \file       PropertyNetwork.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyNetwork.h,v 1.2 2011/09/20 12:24:35 rohn Exp $
/// $Name:  $
/// $Revision: 1.2 $
/// $State: Exp $
/// $Log: PropertyNetwork.h,v $
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB network parameters input class.
///
/// This class is used for the management of the EIB network
/// parameters input via the network property grid. It manages
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
 	#pragma region "UseDhcpConverter class definition"
	/// <summary>
	/// This class definies a type converter for a use DHCP selection.
    /// bool <--> String.
	/// </summary>
    public ref class UseDhcpConverter : public BoolSelectConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        UseDhcpConverter() : BoolSelectConverter(L"textUseDhcp",
                                                 L"textUseIpAddress")
        {
        }
    };
	#pragma endregion

 	#pragma region "EibStringConverter class definition"
	/// <summary>
	/// This class definies a type converter for an EIB string.
    /// The input string will be checked for max length and non ASCII characters.
    /// String <--> String.
	/// </summary>
    public ref class EibStringConverter : public StringConverter 
    {
    private:
        ResourceManager ^   resManager; ///< resource manger of the main form

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EibStringConverter()
        {
            // create resource manger for localized strings
            resManager = GUI_Support::GetResManager();
        }

        /// <summary>
        /// This function returns true if the convertion
        /// String --> String is possible.
        /// </summary>
        virtual bool CanConvertFrom(ITypeDescriptorContext ^ context,
                                    Type ^ sourceType) override
        {
            return (   (sourceType == String::typeid)
                    || TypeConverter::CanConvertFrom(context, sourceType));
        }

        /// <summary>
        /// This function performs the convertion String --> String with input check.
        /// </summary>
        virtual public Object ^ ConvertFrom(ITypeDescriptorContext ^ context,
                                            CultureInfo ^ culture,
                                            Object ^ value) override
        {
            if (value->GetType() == String::typeid)
            {
                String ^ input = (String ^)value;
                // check max string length
                if (input->Length > EIB_STRING_MAX_LENGTH)
                {
                    String ^ error = String::Format(resManager->GetString(L"errorStringLength"),
                                                    EIB_STRING_MAX_LENGTH);
                    throw gcnew FormatException(error);
                }
                // check for non ASCII characters
                for each (Char charValue in input)
                {
                    if ((Int32(charValue) & ~0x07F) != 0)
                    {
                        throw gcnew FormatException(resManager->GetString(L"errorStringValue"));
                    }
                }
                return input;
            }
            return TypeConverter::ConvertFrom(context, culture, value);
        }
    };
	#pragma endregion

 	#pragma region "IpAddressConverter class definition"
	/// <summary>
	/// This class definies a type converter for an IP address.
    /// UInt32 <--> String.
	/// </summary>
    public ref class IpAddressConverter : public TypeConverter 
    {
    public:
        /// <summary>
        /// This function returns true if the convertion
        /// UInt32 --> String is possible.
        /// </summary>
        virtual bool CanConvertTo(ITypeDescriptorContext ^ context,
                                  Type ^ destinationType) override
        {
            return (   (destinationType == UInt32::typeid)
                    || TypeConverter::CanConvertTo(context, destinationType));
        }

        /// <summary>
        /// This function performs the convertion UInt32 --> String.
        /// </summary>
        virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context,
                                   CultureInfo ^ culture, 
                                   Object ^ value, 
                                   Type ^ destinationType) override
        {
            if (   (destinationType == String::typeid)
                && (value->GetType() == UInt32::typeid))
            {
                return IP_Address((UInt32)value).IpString;
            }
            return TypeConverter::ConvertTo(context, culture, value, destinationType);
        }

        /// <summary>
        /// This function returns true if the convertion
        /// String --> UInt32 is possible.
        /// </summary>
        virtual bool CanConvertFrom(ITypeDescriptorContext ^ context,
                                    Type ^ sourceType) override
        {
            return (   (sourceType == String::typeid)
                    || TypeConverter::CanConvertFrom(context, sourceType));
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
                return IP_Address((String ^)value).IpValue;
            }
            return TypeConverter::ConvertFrom(context, culture, value);
        }
    };
	#pragma endregion

 	#pragma region "DhcpModeConverter class definition"
	/// <summary>
	/// This class definies a type converter for a DHCP mode enum.
    /// enum <--> String.
	/// </summary>
    public ref class DhcpModeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        DhcpModeConverter() : LocalEnumConverter(EIB7_ENABLE_MODE::typeid, L"_SavedDhcp")
        {
        }
    };
	#pragma endregion

 	#pragma region "BootModeConverter class definition"
	/// <summary>
	/// This class definies a type converter for an boot mode enum.
    /// enum <--> String.
	/// </summary>
    public ref class BootModeConverter : public LocalEnumConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        BootModeConverter() : LocalEnumConverter(EIB7_MODE_BOOT::typeid)
        {
        }
    };
	#pragma endregion

 	#pragma region "MacAddressConverter class definition"
	/// <summary>
	/// This class definies a type converter for a MAC address.
    /// UInt64 --> String.
	/// </summary>
    public ref class MacAddressConverter : public TypeConverter 
    {
    public:
        /// <summary>
        /// This function returns true if the convertion
        /// UInt64 --> String is possible.
        /// </summary>
        virtual bool CanConvertTo(ITypeDescriptorContext ^ context,
                                  Type ^ destinationType) override
        {
            return (   (destinationType == UInt64::typeid)
                    || TypeConverter::CanConvertTo(context, destinationType));
        }

        /// <summary>
        /// This function performs the convertion UInt64 --> String.
        /// </summary>
        virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context,
                                   CultureInfo ^ culture, 
                                   Object ^ value, 
                                   Type ^ destinationType) override
        {
            if (   (destinationType == String::typeid)
                && (value->GetType() == UInt64::typeid))
            {
                UInt64 macAddress = (UInt64)value;
                String ^ macText = L"";
                for (UInt32 i = 0; i < 6; i ++)
                {
                    macText += String::Format(L"{0,2:X2}",
                                              (macAddress >> ((5 - i) * 8)) & 0x0FF);
                    //macText += String::Format(L"0x{0,2:X2}",
                    //                          (macAddress >> ((5 - i) * 8)) & 0x0FF);
                    if (i < 5)
                    {
                        macText += L":";
                        //macText += L", ";
                    }
                }
                return String::Copy(macText);
            }
            return TypeConverter::ConvertTo(context, culture, value, destinationType);
        }

        /// <summary>
        /// This function returns true if the convertion
        /// String --> UInt64 is possible.
        /// </summary>
        virtual bool CanConvertFrom(ITypeDescriptorContext ^ context,
                                    Type ^ sourceType) override
        {
            if (sourceType == String::typeid)
            {
                return false;
            }
            return TypeConverter::CanConvertFrom(context, sourceType);
        }
    };
	#pragma endregion

 	#pragma region "PropertyNetwork class definition"
    /// <summary>
    /// This class is used for the management of the EIB network
    /// parameters input via the network property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropertyNetwork : public PropertyBase
    {
    private:
        EIB_Param ^                 parameter;      ///< parameters of the saved EIB
        EIB_Network_Read ^          networkRead;    ///< network parameters read from the saved EIB
        EIB_Network_Read ^          defaultRead;    ///< default parameters read from the saved EIB
        EIB_Ctrl ^                  control;        ///< EIB control class

        UInt32                      currentState;   ///< the current input state
        
        PropertyEntryCollection ^   pcDhcpParam;    ///< property list for DHCP mode
        PropertyEntryCollection ^   pcIpAddrParam;  ///< property list for IP address mode
        PropertyEntryCollection ^   pcReadParam;    ///< property list for read parameters

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="parameter">The parameters of the saved EIB.</param>
		/// <param name="networkRead">The network parameters read from the saved EIB.</param>
		/// <param name="defaultRead">The default parameters read from the saved EIB.</param>
		/// <param name="control">The EIB control class.</param>
        PropertyNetwork(GUI_Support ^      guiSupport,
                        EIB_Param ^        parameter,
                        EIB_Network_Read ^ networkRead,
                        EIB_Network_Read ^ defaultRead,
                        EIB_Ctrl ^         control)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridNetwork)
        {
            this->parameter   = parameter;
            this->networkRead = networkRead;
            this->defaultRead = defaultRead;
            this->control     = control;

            currentState = CURRENT_STATE_INIT;
            // create category strings
            String ^ categoryEibAddress =
                                String::Format(CATEGORY_HEADER,
                                               1, resManager->GetString(L"categoryEibAddress"));
            String ^ categoryNetworkParameters =
                                String::Format(CATEGORY_HEADER,
                                               2, resManager->GetString(L"categoryNetworkParameters"));
            String ^ categoryInfoParameters =
                                String::Format(CATEGORY_HEADER,
                                               3, resManager->GetString(L"categoryInfoParameters"));
            // create the property entry for the EIB address mode
            PropertyEntry ^ peCurrentDhcpMode = gcnew PropertyEntry(
                                            L"CurrentDhcpMode", bool::typeid,
                                            categoryEibAddress,
                                            resManager->GetString(L"textCurrentDhcpMode"),
                                            resManager->GetString(L"descriptionCurrentDhcpMode"),
                                            EIB_DEFAULT_USE_DHCP,
                                            UseDhcpConverter::typeid);
            // create the property list for the DHCP parameters
            pcDhcpParam = gcnew PropertyEntryCollection();
            pcDhcpParam->Add(peCurrentDhcpMode);
            pcDhcpParam->Add(gcnew PropertyEntry(
                                            L"CurrentHostName", String::typeid,
                                            categoryEibAddress,
                                            resManager->GetString(L"textHostName"),
                                            resManager->GetString(L"descriptionCurrentHostName"),
                                            EIB_DEFAULT_HOST_NAME,
                                            EibStringConverter::typeid));
            // create the property list for the IP address parameters
            pcIpAddrParam = gcnew PropertyEntryCollection();
            pcIpAddrParam->Add(peCurrentDhcpMode);
            pcIpAddrParam->Add(gcnew PropertyEntry(
                                            L"CurrentIpAddress", UInt32::typeid,
                                            categoryEibAddress,
                                            resManager->GetString(L"textIpAddress"),
                                            resManager->GetString(L"descriptionCurrentIpAddress"),
                                            EIB_DEFAULT_IP_ADDRESS,
                                            IpAddressConverter::typeid));

            // create the property list for the read parameters
            pcReadParam = gcnew PropertyEntryCollection();
            pcReadParam->Add(gcnew PropertyEntry(L"SavedHostName", String::typeid,
                                                 categoryNetworkParameters,
                                                 resManager->GetString(L"textHostName"),
                                                 resManager->GetString(L"descriptionSavedHostName"),
                                                 EIB_DEFAULT_HOST_NAME,
                                                 EibStringConverter::typeid));
            pcReadParam->Add(gcnew PropertyEntry(L"SavedIpAddress", UInt32::typeid,
                                                 categoryNetworkParameters,
                                                 resManager->GetString(L"textIpAddress"),
                                                 resManager->GetString(L"descriptionSavedIpAddress"),
                                                 EIB_DEFAULT_IP_ADDRESS,
                                                 IpAddressConverter::typeid));
            pcReadParam->Add(gcnew PropertyEntry(L"Netmask", UInt32::typeid,
                                                 categoryNetworkParameters,
                                                 resManager->GetString(L"textNetmask"),
                                                 resManager->GetString(L"descriptionNetmask"),
                                                 EIB_DEFAULT_NETMASK,
                                                 IpAddressConverter::typeid));
            pcReadParam->Add(gcnew PropertyEntry(L"Gateway", UInt32::typeid,
                                                 categoryNetworkParameters,
                                                 resManager->GetString(L"textGateway"),
                                                 resManager->GetString(L"descriptionGateway"),
                                                 EIB_DEFAULT_GATEWAY,
                                                 IpAddressConverter::typeid));
            pcReadParam->Add(gcnew PropertyEntry(L"SavedDhcpMode", EIB7_ENABLE_MODE::typeid,
                                                 categoryNetworkParameters,
                                                 resManager->GetString(L"textSavedDhcpMode"),
                                                 resManager->GetString(L"descriptionSavedDhcpMode"),
                                                 EIB_DEFAULT_DHCP_MODE,
                                                 DhcpModeConverter::typeid));

            pcReadParam->Add(gcnew PropertyEntry(L"MacAddress", UInt64::typeid, true,
                                                 categoryInfoParameters,
                                                 resManager->GetString(L"textMacAddress"),
                                                 resManager->GetString(L"descriptionMacAddress"),
                                                 MacAddressConverter::typeid));
            pcReadParam->Add(gcnew PropertyEntry(L"BootMode", EIB7_MODE_BOOT::typeid, true,
                                                 categoryInfoParameters,
                                                 resManager->GetString(L"textBootMode"),
                                                 resManager->GetString(L"descriptionBootMode"),
                                                 BootModeConverter::typeid));
            pcReadParam->Add(gcnew PropertyEntry(L"SerialNumber", String::typeid, true,
                                                 categoryInfoParameters,
                                                 resManager->GetString(L"textSerialNumber"),
                                                 resManager->GetString(L"descriptionSerialNumber")));
            pcReadParam->Add(gcnew PropertyEntry(L"IdentNumber", String::typeid, true,
                                                 categoryInfoParameters,
                                                 resManager->GetString(L"textIdentNumber"),
                                                 resManager->GetString(L"descriptionIdentNumber")));
            pcReadParam->Add(gcnew PropertyEntry(L"FirmwareVersion", String::typeid, true,
                                                 categoryInfoParameters,
                                                 resManager->GetString(L"textFirmwareVersion"),
                                                 resManager->GetString(L"descriptionFirmwareVersion")));
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            // property state machine definitions
            #define PROPERTY_NETWORK_STATE_INIT_READY   0x01
            #define PROPERTY_NETWORK_STATE_USE_DHCP     0x02
            #define PROPERTY_NETWORK_STATE_READ_VALID   0x04
            // set input state
            UInt32 inputState = PROPERTY_NETWORK_STATE_INIT_READY;
            if (parameter->Network->CurrentDhcpMode == true)
            {
                inputState |= PROPERTY_NETWORK_STATE_USE_DHCP;
            }
            if (networkRead->ReadValid == true)
            {
                inputState |= PROPERTY_NETWORK_STATE_READ_VALID;
            }
            // check current state
            if (currentState != inputState)
            {
                // set property list
                Properties->Clear();
                if ((inputState & PROPERTY_NETWORK_STATE_USE_DHCP) == 0)
                {
                    // IP address mode
                    Properties->AddRange(pcIpAddrParam);
                }
                else
                {
                    // DHCP mode
                    Properties->AddRange(pcDhcpParam);
                }
                if ((inputState & PROPERTY_NETWORK_STATE_READ_VALID) != 0)
                {
                    // use network read parameters
                    Properties->AddRange(pcReadParam);
                }
                // set new state
                currentState = inputState;
            }
            // set the default values of the network read parameters
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"SavedHostName")))->DefaultValue =
                                                                            defaultRead->SavedHostName;
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"SavedIpAddress")))->DefaultValue =
                                                                            defaultRead->SavedIpAddress;
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"Netmask")))->DefaultValue =
                                                                            defaultRead->Netmask;
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"Gateway")))->DefaultValue =
                                                                            defaultRead->Gateway;
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"SavedDhcpMode")))->DefaultValue =
                                                                            defaultRead->SavedDhcpMode;

            ((PropertyEntry ^)(pcReadParam->ItemOf(L"MacAddress")))->DefaultValue =
                                                                            defaultRead->MacAddress;
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"BootMode")))->DefaultValue =
                                                                            defaultRead->BootMode;
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"SerialNumber")))->DefaultValue =
                                                                            defaultRead->SerialNumber;
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"IdentNumber")))->DefaultValue =
                                                                            defaultRead->IdentNumber;
            ((PropertyEntry ^)(pcReadParam->ItemOf(L"FirmwareVersion")))->DefaultValue =
                                                                            defaultRead->FirmwareVersion;
            // set network parameters write button enable state
            guiSupport->NetworkWriteEnable(networkRead->ReadValid);
            // refresh the property grid
            PropertyBase::RefreshParam();
        }

        /// <summary>
        /// A property was changed by the user.
        /// </summary>
        void PropertyChanged(String ^ propertyName)
        {
            if (   (propertyName == L"CurrentDhcpMode")
                || (propertyName == L"CurrentHostName")
                || (propertyName == L"CurrentIpAddress"))
            {
                // the EIB network read parameters are no more valid
                networkRead->ReadValid = false;
            }
            if (propertyName == L"CurrentIpAddress")
            {
                // IP address changed --> close EIB
                control->EIB_Close(false);
            }
            // refresh Network Tab
            RefreshParam();
        }

        /// <summary>
		/// This member overrides PropertyClass.OnGetValue.
		/// </summary>
		virtual void OnGetValue(PropertyEntryEventArgs ^ e) override
		{
            // set parameter class depending from parameter
            if (   (e->Property->Name == L"CurrentDhcpMode")
                || (e->Property->Name == L"CurrentHostName")
                || (e->Property->Name == L"CurrentIpAddress"))
            {
                paramClass = parameter->Network;
            }
            else
            {
                paramClass = networkRead;
            }
            PropertyBase::OnGetValue(e);
		}

		/// <summary>
		/// This member overrides PropertyClass.OnSetValue.
		/// </summary>
		virtual void OnSetValue(PropertyEntryEventArgs ^ e) override
		{
            // set parameter class depending from parameter
            if (   (e->Property->Name == L"CurrentDhcpMode")
                || (e->Property->Name == L"CurrentHostName")
                || (e->Property->Name == L"CurrentIpAddress"))
            {
                paramClass = parameter->Network;
            }
            else
            {
                paramClass = networkRead;
            }
            PropertyBase::OnSetValue(e);
		}
    };
	#pragma endregion
}
