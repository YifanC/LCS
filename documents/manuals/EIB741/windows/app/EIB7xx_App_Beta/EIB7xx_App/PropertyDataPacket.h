///
/// \file       PropertyDataPacket.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: PropertyDataPacket.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: PropertyDataPacket.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB axis data packet parameters display class.
///
/// This class is used for the display of the EIB axis data packet
/// parameters via the data packet data grid view.
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
 	#pragma region "PropertyDataPacket class definition"
    // special definitions
    #define PT_FIRST_DATA_COLUMN        1       //< the first data column of the data grid view
    #define PT_FIRST_NEW_COLUMN         2       //< the first new column of the data grid view
    #define PT_FIRST_DATA_ROW           1       //< the first data row of the data grid view
    #define PT_DATA_ROW_NUMBER          8       //< the data row number of the data grid view
    #define PT_VALUE_USED_TEXT          L"X"    //< the data packet value used text
    #define PT_VALUE_NOT_USED_TEXT      L""     //< the data packet value not used text
    #define PT_VALUE_DISABLED_TEXT      L""     //< the data packet value disabled text

    /// <summary>
    /// This class is used for the display of the EIB axis data packet
    /// parameters via the data packet data grid view.
    /// </summary>
    public ref class PropertyDataPacket
    {
    private:
        GUI_Support ^       guiSupport;     ///< the GUI support class
        ResourceManager ^   resManager;     ///< the resource manager from main window
        DataGridView ^      dataGridView;   ///< the Data Grid View
        EIB_TreeList ^      eibTreeList;    ///< the EIB tree list
        Object ^            paramClass;     ///< the class that contains the parameters
        UInt32              axisNumber;     ///< EIB number of axis
        UInt32              auxAxisNumber;  ///< EIB number of auxiliary axis

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="eibTreeList">The EIB tree list.</param>
        PropertyDataPacket(GUI_Support ^  guiSupport,
                           EIB_TreeList ^ eibTreeList)
        {
            this->guiSupport   = guiSupport;
            this->resManager   = guiSupport->ResManager;
            this->dataGridView = guiSupport->DataGridViewDataPacket;
            this->eibTreeList  = eibTreeList;
            axisNumber         = PT_FIRST_NEW_COLUMN - PT_FIRST_DATA_COLUMN;
            auxAxisNumber      = 0;
        }

        /// <summary>
        /// Refresh the data grid view.
        /// </summary>
        void RefreshParam()
        {
            // get the EIBs axis numbers
            UInt32 axisNumber    = eibTreeList->SelectedEIB->Parameter->AxisNumber;
            UInt32 auxAxisNumber = eibTreeList->SelectedEIB->Parameter->AuxAxisNumber;
            // initialize data grid view
            bool setColumnHeader = false;
            UInt32 cellIndex;
            String ^ textName;
            String ^ propertyName;
            DataGridViewCellStyle ^ cellStyle;
            if (   (this->axisNumber    != axisNumber)
                || (this->auxAxisNumber != auxAxisNumber))
            {
                // delete old columns
                UInt32 columnCount = dataGridView->Columns->Count;
                if (columnCount > PT_FIRST_NEW_COLUMN)
                {
                    for (UInt32 i = PT_FIRST_NEW_COLUMN; i < columnCount; i ++)
                    {
                        dataGridView->Columns->RemoveAt(PT_FIRST_NEW_COLUMN);
                    }
                }
                // add new columns with axis 1 column style
                DataGridViewColumn ^ axisColumn;
                for (UInt32 i = PT_FIRST_NEW_COLUMN - PT_FIRST_DATA_COLUMN; i < axisNumber; i ++)
                {
                    axisColumn = (DataGridViewColumn ^)(dataGridView->Columns[PT_FIRST_DATA_COLUMN]->Clone());
                    dataGridView->Columns->Add(axisColumn);
                }
                for (UInt32 i = 0; i < auxAxisNumber; i ++)
                {
                    axisColumn = (DataGridViewColumn ^)(dataGridView->Columns[PT_FIRST_DATA_COLUMN]->Clone());
                    dataGridView->Columns->Add(axisColumn);
                }
                // set column header flag
                setColumnHeader = true;
                // set new data grid view axis numbers
                this->axisNumber    = axisNumber;
                this->auxAxisNumber = auxAxisNumber;
            }
            // add new rows
            UInt32 rowCount = dataGridView->Rows->Count;
            if (rowCount < PT_FIRST_DATA_ROW + PT_DATA_ROW_NUMBER)
            {
                dataGridView->Rows->Add(PT_FIRST_DATA_ROW + PT_DATA_ROW_NUMBER - rowCount);
                // set row header
                for (UInt32 i = 0; i < PT_DATA_ROW_NUMBER; i ++)
                {
                    cellIndex = PT_FIRST_DATA_ROW + i;
                    switch (i)
                    {
                    default: throw gcnew Exception(L"Internal error 1"); break;
                    case 0:
                        textName     = L"textDataPacketStatusWord";
                        propertyName = L"DataPacketStatusWord";
                        break;
                    case 1:
                        textName     = L"textDataPacketPositionData";
                        propertyName = L"DataPacketPositionData";
                        break;
                    case 2:
                        textName     = L"textDataPacketTimestamp";
                        propertyName = L"DataPacketTimestamp";
                        break;
                    case 3:
                        textName     = L"textDataPacketReferencePos";
                        propertyName = L"DataPacketReferencePos";
                        break;
                    case 4:
                        textName     = L"textDataPacketDistCodedRef";
                        propertyName = L"DataPacketDistCodedRef";
                        break;
                    case 5:
                        textName     = L"textDataPacketAnalog";
                        propertyName = L"DataPacketAnalog";
                        break;
                    case 6:
                        textName     = L"textDataPacketEnDatAI1";
                        propertyName = L"DataPacketEnDatAI1";
                        break;
                    case 7:
                        textName     = L"textDataPacketEnDatAI2";
                        propertyName = L"DataPacketEnDatAI2";
                        break;
                    }
                    dataGridView->Rows[cellIndex]->Cells[0]->Value = resManager->GetString(textName);
                    dataGridView->Rows[cellIndex]->Cells[0]->Tag   = propertyName;
                    cellStyle = dataGridView->Rows[cellIndex]->Cells[0]->Style;
                    cellStyle->Alignment = DataGridViewContentAlignment::MiddleLeft;
                    dataGridView->Rows[cellIndex]->Cells[0]->Style = cellStyle;
                }
            }
            // set column header
            if (setColumnHeader == true)
            {
                for (UInt32 i = 0; i < axisNumber + auxAxisNumber; i ++)
                {
                    // set back color of axis column header cell
                    cellIndex = PT_FIRST_DATA_COLUMN + i;
                    cellStyle = dataGridView->Rows[0]->Cells[cellIndex]->Style;
                    cellStyle->BackColor          = JH_COLOR_GRAY_LIGHT;
                    cellStyle->SelectionBackColor = JH_COLOR_GRAY_LIGHT;
                    dataGridView->Rows[0]->Cells[cellIndex]->Style = cellStyle;
                    // set header text
                    dataGridView->Rows[0]->Cells[cellIndex]->Value = eibTreeList->SelectedEIB->AxisName[i + 1];
                }
            }
            // set data packet values
            UInt32 rowCell, columnCell;
            bool isEnabled;
            for (UInt32 row = 0; row < PT_DATA_ROW_NUMBER; row ++)
            {
                rowCell = PT_FIRST_DATA_ROW + row;
                // get property from header cell tag
                propertyName = (String ^)(dataGridView->Rows[rowCell]->Cells[0]->Tag);
                for (UInt32 column = 0; column < axisNumber + auxAxisNumber; column ++)
                {
                    // set parameter class
                    if (column < axisNumber)
                    {
                        paramClass = eibTreeList->SelectedEIB->Parameter->Axis[column];
                        isEnabled  = ((EIB_Axis_Param ^)paramClass)->IsEnabled;
                    }
                    else
                    {
                        paramClass = eibTreeList->SelectedEIB->Parameter->AuxAxis[column - axisNumber];
                        isEnabled  = ((EIB_AuxAxis_Param ^)paramClass)->IsEnabled;
                    }
                    // get property value
                    columnCell = PT_FIRST_DATA_COLUMN + column;
                    PropertyInfo ^ propertyInfo = paramClass->GetType()->GetProperty(propertyName);
                    bool value = (bool)(propertyInfo->GetValue(paramClass, nullptr));
                    if (isEnabled == false)
                    {
                        textName = PT_VALUE_DISABLED_TEXT;
                    }
                    else
                    {
                        if (value == false)
                        {
                            textName = PT_VALUE_NOT_USED_TEXT;
                        }
                        else
                        {
                            textName = PT_VALUE_USED_TEXT;
                        }
                    }
                    dataGridView->Rows[rowCell]->Cells[columnCell]->Value = textName;
                }
            }
        }
    };
	#pragma endregion
}
