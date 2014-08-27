///
/// \file       DisplayBase.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: DisplayBase.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: DisplayBase.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB axis data display base class.
///
/// This class is used as a base class for the display
/// of the EIB axis data.
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
 	#pragma region "DisplayBase class definition"
    // special definitions
    #define DISPLAY_MODE_INIT_OK        UInt32(0x01)
    #define DISPLAY_MODE_RUNNING        UInt32(0x02)
    #define DISPLAY_MODE_POLLING        UInt32(0x04)
    #define DISPLAY_MODE_RECORDING      UInt32(0x08)

    #define SHOW_LAST_VALUES            true

    /// <summary>
    /// This class is used as a base class for the display
    /// of the EIB axis data.
    /// </summary>
    public ref class DisplayBase
    {
    protected:
        GUI_Support ^       guiSupport;         ///< the GUI support class
        ResourceManager ^   resManager;         ///< the resource manager from main window
        EIB_TreeList ^      eibTreeList;        ///< the EIB tree list
        UInt32              displayAxis;        ///< the axis to display index

        bool                showLastValues;     ///< if true show last values after stop

        UInt32              axisIndex;          ///< the index of the selected axis
        bool                isAuxAxis;          ///< the axis state of the selected axis
        bool                isEnabled;          ///< the enabled state of the selected axis
        bool                isIncremental;      ///< the encoder type of the selected axis
        
        EIB_Param ^         parameter;          ///< the selected EIB parameters
        EIB_Axis_Param ^    axisParam;          ///< the selected axis / reference axis parameters
        DataPacketBuffer ^  displayBuffer;      ///< data packet display buffer
        DataPacketData ^    dataPacketData;     ///< data packet data
        DataPacketData ^    emptyDataPacket;    ///< empty data packet data
        WarningFlags ^      warningFlags;       ///< measurement warning flags
        AxisWarningFlags ^  axisWarnings;       ///< measurement axis warning flags

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="eibTreeList">The EIB tree list.</param>
		/// <param name="displayAxis">The axis to display index.</param>
        DisplayBase(GUI_Support ^  guiSupport,
                    EIB_TreeList ^ eibTreeList,
                    UInt32         displayAxis)
        {
            this->guiSupport  = guiSupport;
            this->resManager  = guiSupport->ResManager;
            this->eibTreeList = eibTreeList;
            this->displayAxis = displayAxis;

            emptyDataPacket = gcnew DataPacketData(nullptr);

            showLastValues    = SHOW_LAST_VALUES;
        }

        /// <summary>
        /// Get the data packet data.
        /// </summary>
		/// <param name="parameter">The EIB parameters.</param>
        void GetDataPacketData(EIB_Param ^ parameter)
        {
            displayBuffer = eibTreeList->SelectedEIB->DisplayBuffer;
            warningFlags  = eibTreeList->SelectedEIB->WarningFlagValues;
            
            if (displayAxis > 0)
            {
                axisIndex = displayAxis - 1;
                isAuxAxis = false;
            }
            else
            {
                axisIndex = eibTreeList->AxisIndex;
                isAuxAxis = eibTreeList->IsAuxAxis;
            }
            if (isAuxAxis == false)
            {
                axisParam      = parameter->Axis[axisIndex];
                isEnabled      = axisParam->IsEnabled;
                isIncremental  = (   (   axisParam->InterfaceTypeGui
                                      == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                                  || (   axisParam->InterfaceTypeGui
                                      == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u));
                dataPacketData = displayBuffer->Entry[0]->Axis[axisIndex];
                axisWarnings   = warningFlags->Axis[axisIndex];
            }
            else
            {
                axisParam      = parameter->AuxAxis[axisIndex]->Axis;
                isEnabled      = parameter->AuxAxis[axisIndex]->IsEnabled;
                isIncremental  = (   (   axisParam->InterfaceTypeGui
                                      == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                                  || (   axisParam->InterfaceTypeGui
                                      == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u));
                dataPacketData = displayBuffer->Entry[0]->AuxAxis[axisIndex];
                axisWarnings   = warningFlags->AuxAxis[axisIndex];
            }
        }
    };
	#pragma endregion
}
