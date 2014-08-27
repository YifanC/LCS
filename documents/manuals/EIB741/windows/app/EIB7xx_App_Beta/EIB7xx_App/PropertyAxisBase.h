///
/// \file       PropertyAxisBase.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyAxisBase.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: PropertyAxisBase.h,v $
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
 	#pragma region "DataPacketConverter class definition"
	/// <summary>
	/// This class definies a type converter for a data packet value selection.
    /// bool <--> String.
	/// </summary>
    public ref class DataPacketConverter : public BoolSelectConverter 
    {
    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        DataPacketConverter() : BoolSelectConverter(L"textDataPacketUse",
                                                    L"textDataPacketNotUse")
        {
        }
    };
	#pragma endregion

 	#pragma region "PropertyAxisBase class definition"
    // property state machine definitions
    #define PROPERTY_AXIS_STATE_INIT_READY      0x00000100
    #define PROPERTY_AXIS_STATE_ENABLE          0x00000200
    #define PROPERTY_AXIS_STATE_ENDAT           0x00000400
    #define PROPERTY_AXIS_STATE_INC             0x00000800
    #define PROPERTY_AXIS_STATE_INC_1V          0x00010000
    #define PROPERTY_AXIS_STATE_REF             0x00020000
    #define PROPERTY_AXIS_STATE_REF_DIST        0x00040000
    #define PROPERTY_AXIS_STATE_TYPE_ROT        0x00080000
    #define PROPERTY_AXIS_STATE_ENDAT_22        0x01000000
    #define PROPERTY_AXIS_STATE_ADD_INFO_ON     0x02000000
    #define PROPERTY_AXIS_STATE_ADD_INFO_CNT    0x0000000F

    /// <summary>
    /// This class is used for the management of the EIB axis
    /// parameters input via the axis property grid. It manages
    /// the parameter dependencies for the property grid.
    /// </summary>
    public ref class PropertyAxisBase : public PropertyBase
    {
    private:
        EIB_Axis_Base ^             axisParameter;      ///< axis parameters of the saved EIB
        EIB_Axis_Param ^            refAxisParameter;   ///< reference axis parameters of the saved EIB
        bool                        isAuxAxis;          ///< true it it is an auxiliary axis

        PropertyEntryCollection ^   pcPacketAll;        ///< the property list for all devices
        PropertyEntry ^             pePacketRef;        ///< the property entry for reference marks
        PropertyEntry ^             pePacketDist;       ///< the property entry for distance reference marks
        PropertyEntry ^             pePacketAdc;        ///< the property entry for ADC values
        PropertyEntryCollection ^   pcPacketAddInfo;    ///< the property list for additional information

    protected:
        UInt32                      currentState;       ///< the current input state
        UInt32                      inputState;         ///< the new input state

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="axisParameter">The axis parameters of the saved EIB.</param>
		/// <param name="refAxisParameter">The reference axis parameters of the saved EIB.</param>
		/// <param name="categoryIndex">The index for the category name.</param>
		/// <param name="isAuxAxis">True it it is an auxiliary axis.</param>
        PropertyAxisBase(GUI_Support ^    guiSupport,
                         EIB_Axis_Base ^  axisParameter,
                         EIB_Axis_Param ^ refAxisParameter,
                         UInt32           categoryIndex,
                         bool             isAuxAxis)
            : PropertyBase(guiSupport,
                           guiSupport->PropertyGridAxis)
        {
            this->axisParameter    = axisParameter;
            this->refAxisParameter = refAxisParameter;
            this->isAuxAxis        = isAuxAxis;

            currentState = CURRENT_STATE_INIT;
            // create category strings
            String ^ categoryDataPacket =
                                String::Format(CATEGORY_HEADER,
                                               categoryIndex,
                                               resManager->GetString(L"categoryDataPacket"));
            // create the property list for all devices
            pcPacketAll = gcnew PropertyEntryCollection();
            pcPacketAll->Add(gcnew PropertyEntry
                                            (L"DataPacketStatusWord", bool::typeid,
                                             categoryDataPacket,
                                             resManager->GetString(L"textDataPacketStatusWord"),
                                             resManager->GetString(L"descriptionDataPacketStatusWord"),
                                             PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_StatusWord),
                                             DataPacketConverter::typeid));
            pcPacketAll->Add(gcnew PropertyEntry
                                            (L"DataPacketPositionData", bool::typeid,
                                             categoryDataPacket,
                                             resManager->GetString(L"textDataPacketPositionData"),
                                             resManager->GetString(L"descriptionDataPacketPositionData"),
                                             PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_PositionData),
                                             DataPacketConverter::typeid));
            pcPacketAll->Add(gcnew PropertyEntry
                                            (L"DataPacketTimestamp", bool::typeid,
                                             categoryDataPacket,
                                             resManager->GetString(L"textDataPacketTimestamp"),
                                             resManager->GetString(L"descriptionDataPacketTimestamp"),
                                             PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_Timestamp),
                                             DataPacketConverter::typeid));
            // create the property entry for reference marks
            pePacketRef = gcnew PropertyEntry
                                            (L"DataPacketReferencePos", bool::typeid,
                                             categoryDataPacket,
                                             resManager->GetString(L"textDataPacketReferencePos"),
                                             resManager->GetString(L"descriptionDataPacketReferencePos"),
                                             PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_ReferencePos),
                                             DataPacketConverter::typeid);
            // create the property entry for distance reference marks
            pePacketDist = gcnew PropertyEntry
                                            (L"DataPacketDistCodedRef", bool::typeid,
                                             categoryDataPacket,
                                             resManager->GetString(L"textDataPacketDistCodedRef"),
                                             resManager->GetString(L"descriptionDataPacketDistCodedRef"),
                                             PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_DistCodedRef),
                                             DataPacketConverter::typeid);
            // create the property entry for ADC values
            pePacketAdc = gcnew PropertyEntry
                                            (L"DataPacketAnalog", bool::typeid,
                                             categoryDataPacket,
                                             resManager->GetString(L"textDataPacketAnalog"),
                                             resManager->GetString(L"descriptionDataPacketAnalog"),
                                             PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_Analog),
                                             DataPacketConverter::typeid);
            // create the property list for additional information
            pcPacketAddInfo = gcnew PropertyEntryCollection();
            pcPacketAddInfo->Add(gcnew PropertyEntry
                                            (L"DataPacketEnDatAI1", bool::typeid,
                                             categoryDataPacket,
                                             resManager->GetString(L"textDataPacketEnDatAI1"),
                                             resManager->GetString(L"descriptionDataPacketEnDatAI1"),
                                             PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_EnDat_AI1),
                                             DataPacketConverter::typeid));
            pcPacketAddInfo->Add(gcnew PropertyEntry
                                            (L"DataPacketEnDatAI2", bool::typeid,
                                             categoryDataPacket,
                                             resManager->GetString(L"textDataPacketEnDatAI2"),
                                             resManager->GetString(L"descriptionDataPacketEnDatAI2"),
                                             PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_EnDat_AI2),
                                             DataPacketConverter::typeid));
        }

        /// <summary>
        /// Get data package component default value.
        /// </summary>
	    /// <param name="component">The data packet component to get the default value from.</param>
        /// <returns>The data packet component default value.</returns>
        bool PacketDefault(EIB7_POSITION_DATA_FIELD component)
        {
            if ((EIB_DEFAULT_DATA_PACKET & UInt32(component)) == 0)
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// Set the new input state.
        /// </summary>
	    /// <param name="axisEnabled">The axis enabled state.</param>
        void SetInputState(bool axisEnabled)
        {
            // set input state
            inputState = PROPERTY_AXIS_STATE_INIT_READY;
            if (axisEnabled == true)
            {
                // axis is enabled
                inputState |= PROPERTY_AXIS_STATE_ENABLE;
                if (   (refAxisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat21)
                    || (refAxisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22))
                {
                    // use EnDat encoder
                    inputState |= PROPERTY_AXIS_STATE_ENDAT;
                    if (refAxisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22)
                    {
                        // use EnDat 2.2 encoder
                        inputState |= PROPERTY_AXIS_STATE_ENDAT_22;
                    }
                    else
                    {
                        // use EnDat 2.1 encoder
                    }
                    if (refAxisParameter->AddInfoMode != EIB7_ADD_INFO_MODE::EIB7_AIM_None)
                    {
                        // use EnDat encoder with additional information
                        inputState |= PROPERTY_AXIS_STATE_ADD_INFO_ON;
                        if (refAxisParameter->AddInfoMode == EIB7_ADD_INFO_MODE::EIB7_AIM_Fifo)
                        {
                            // use EnDat encoder with additional information in fifo
                            inputState |= (  UInt32(refAxisParameter->AddInfoNum)
                                           & PROPERTY_AXIS_STATE_ADD_INFO_CNT);
                        }
                        else
                        {
                            // use EnDat encoder with fix additional information
                        }
                    }
                    else
                    {
                        // use EnDat encoder without additional information
                    }
                }
                else
                {
                    // use incremental encoder
                    if (refAxisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                    {
                        // use incremental encoder with 1Vss interface
                        inputState |= PROPERTY_AXIS_STATE_INC_1V;
                    }
                    else
                    {
                        // use incremental encoder with 11Myss interface
                    }
                    if (refAxisParameter->Refmarks != EIB7_REFMARKS::EIB7_RM_None)
                    {
                        // use incremental encoder with reference marks
                        inputState |= PROPERTY_AXIS_STATE_REF;
                        if (refAxisParameter->Refmarks == EIB7_REFMARKS::EIB7_RM_DistanceCoded)
                        {
                            // use incremental encoder with distance coded reference marks
                            inputState |= PROPERTY_AXIS_STATE_REF_DIST;
                        }
                        else
                        {
                            // use incremental encoder with one reference mark
                        }
                    }
                    else
                    {
                        // use incremental encoder without reference marks
                    }
                    if (refAxisParameter->EncoderType == EIB7_ENCODER_TYPE::EIB7_EC_Rotary)
                    {
                        // use linear incremental encoder
                        inputState |= PROPERTY_AXIS_STATE_TYPE_ROT;
                    }
                    else
                    {
                        // use rotary incremental encoder
                    }
                }
                if (   (refAxisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                    || (refAxisParameter->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u))
                {
                    // use encoder with incremental signals
                    inputState |= PROPERTY_AXIS_STATE_INC;
                }
            }
            else
            {
                // axis is disabled
            }
        }

        /// <summary>
        /// Set the currently used parameter class and property list
        /// and refresh the property grid.
        /// </summary>
        virtual void RefreshParam() override
        {
            // check current state
            if (currentState != inputState)
            {
                UInt32 changeMask;
                // set data package property list
                if ((inputState & PROPERTY_AXIS_STATE_ENABLE) != 0)
                {
                    // axis is enabled
                    changeMask =   PROPERTY_AXIS_STATE_ENABLE
                                 | PROPERTY_AXIS_STATE_ENDAT
                                 | PROPERTY_AXIS_STATE_INC_1V
                                 | PROPERTY_AXIS_STATE_ENDAT_22;
                    if (((currentState ^ inputState) & changeMask) != 0)
                    {
                        axisParameter->DataPacket = EIB_DEFAULT_DATA_PACKET;
                    }
                    // add entries for all devices
                    Properties->AddRange(pcPacketAll);
                    if ((inputState & PROPERTY_AXIS_STATE_REF) != 0)
                    {
                        // add reference marks
                        Properties->Add(pePacketRef);
                        // set reference marks value and default value
                        pePacketRef->DefaultValue = true;
                        axisParameter->DataPacketReferencePos = true;

                        if (   (isAuxAxis == false)
                            && ((inputState & PROPERTY_AXIS_STATE_REF_DIST) != 0))
                        {
                            // add distance reference marks
                            Properties->Add(pePacketDist);
                            // set distance reference marks value and default value
                            pePacketDist->DefaultValue = true;
                            axisParameter->DataPacketDistCodedRef = true;
                        }
                    }
                    if (   (isAuxAxis == false)
                        && ((inputState & PROPERTY_AXIS_STATE_INC) != 0))
                    {
                        // add ADC values
                        Properties->Add(pePacketAdc);
                        changeMask = PROPERTY_AXIS_STATE_INC;
                        if (((currentState ^ inputState) & changeMask) != 0)
                        {
                            axisParameter->DataPacketAnalog =
                                        PacketDefault(EIB7_POSITION_DATA_FIELD::EIB7_PDF_Analog);
                        }
                    }
                    if (   (isAuxAxis == false)
                        && ((inputState & PROPERTY_AXIS_STATE_ENDAT_22) != 0)
                        && ((inputState & PROPERTY_AXIS_STATE_ADD_INFO_ON) != 0))
                    {
                        // add additional information
                        Properties->AddRange(pcPacketAddInfo);
                        // set additional information value and default value
                        ((PropertyEntry ^)
                            (pcPacketAddInfo->ItemOf(L"DataPacketEnDatAI1")))->DefaultValue = true;
                        ((PropertyEntry ^)
                            (pcPacketAddInfo->ItemOf(L"DataPacketEnDatAI2")))->DefaultValue = true;
                        axisParameter->DataPacketEnDatAI1 = true;
                        axisParameter->DataPacketEnDatAI2 = true;
                    }
                }
                // set special parameter dependencies
                if (refAxisParameter->Refmarks != EIB7_REFMARKS::EIB7_RM_DistanceCoded)
                {
                    axisParameter->DataPacketDistCodedRef = false;
                }
                if (refAxisParameter->Refmarks == EIB7_REFMARKS::EIB7_RM_None)
                {
                    axisParameter->DataPacketReferencePos = false;
                }
            }
        }
    };
	#pragma endregion
}
