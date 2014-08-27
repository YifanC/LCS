///
/// \file       DisplayDataPacket.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: DisplayDataPacket.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: DisplayDataPacket.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB axis data packet data display class.
///
/// This class is used for the display of the EIB axis data packet
/// data via the data packet data grid view.
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
 	#pragma region "DisplayDataPacket class definition"
    // special definitions
    #define DT_FIRST_DATA_COLUMN        1       //< the first data column of the data packet data grid
    #define DT_FIRST_NEW_COLUMN         2       //< the first new column of the data packet data grid
    #define DT_FIRST_DATA_ROW           0       //< the first data row of the data packet data grid
    #define DT_DATA_ROW_NUMBER         12       //< the data row number of the data packet data grid

    #define ST_FIRST_DATA_COLUMN        1       //< the first data column of the status data grid
    #define ST_FIRST_NEW_COLUMN         3       //< the first new column of the status data grid
    #define ST_FIRST_DATA_ROW           0       //< the first data row of the status data grid
    #define ST_TABLE_MODE_INCR       0x01       //< the table mode for incremental status
    #define ST_TABLE_MODE_AUX        0x02       //< the table mode for auxiliary axis status
    #define ST_TABLE_MODE_EN_DAT     0x04       //< the table mode for EnDat status
    #define ST_TABLE_MODE_ADD_INFO   0x08       //< the table mode for EnDat additional info status

    #define ST_VALUE_SET_TEXT           L"X"    //< the status value set text
    #define ST_VALUE_NOT_SET_TEXT       L""     //< the status value not set text
    #define ST_VALUE_NOT_USED_TEXT      L""     //< the status value not used text

    /// <summary>
    /// This enum lists status table mode values.
    /// </summary>
    enum class StatusTableMode : UInt32
    {
        Header,
        Status,
        Error,
    };

    /// <summary>
    /// This class is used for the display of the EIB axis data packet
    /// data via the data packet data grid view.
    /// </summary>
    public ref class DisplayDataPacket : public DisplayBase
    {
    private:
        DataGridView ^      dataGridDataPacket;     ///< the Data Packet Data Grid View
        DataGridView ^      dataGridStatus;         ///< the Status Data Grid View

        UInt32              cellIndex;              ///< the table cell index

        UInt32              statusTableMode;        ///< the status table mode

        DataGridViewRow ^   rowHeader;              ///< status table header row
        DataGridViewRow ^   rowError;               ///< status table error row
        DataGridViewRow ^   rowErrorCmd;            ///< status table error row with button

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="eibTreeList">The EIB tree list.</param>
		/// <param name="dataGridDataPacket">The Data Packet Data Grid View.</param>
		/// <param name="dataGridStatus">The Status Data Grid View.</param>
        DisplayDataPacket(GUI_Support ^  guiSupport,
                          EIB_TreeList ^ eibTreeList,
                          DataGridView ^ dataGridDataPacket,
                          DataGridView ^ dataGridStatus)
            : DisplayBase(guiSupport, eibTreeList, 0)
        {
            this->dataGridDataPacket = dataGridDataPacket;
            this->dataGridStatus     = dataGridStatus;

            // add new rows to data packet table
            UInt32 rowCount = dataGridDataPacket->Rows->Count;
            if (rowCount < DT_FIRST_DATA_ROW + DT_DATA_ROW_NUMBER)
            {
                dataGridDataPacket->Rows->Add(DT_FIRST_DATA_ROW + DT_DATA_ROW_NUMBER - rowCount);
            }
            // set initial status table mode
            statusTableMode = 0;
            // create status table row templates
            DataGridViewCellStyle ^ cellStyle1, ^ cellStyle2;
            // create status table header row template
            rowHeader = (DataGridViewRow ^)(dataGridStatus->RowTemplate->Clone());
            rowHeader->CreateCells(dataGridStatus);
            cellStyle1 = rowHeader->Cells[1]->Style;
            cellStyle2 = rowHeader->Cells[2]->Style;
            cellStyle1->BackColor = JH_COLOR_GRAY_LIGHT;
            cellStyle2->BackColor = JH_COLOR_GRAY_LIGHT;
            cellStyle1->SelectionBackColor = JH_COLOR_GRAY_LIGHT;
            cellStyle2->SelectionBackColor = JH_COLOR_GRAY_LIGHT;
            rowHeader->Cells[1]->Style = cellStyle1;
            rowHeader->Cells[2]->Style = cellStyle2;
            // create status table error row template
            rowError = (DataGridViewRow ^)(dataGridStatus->RowTemplate->Clone());
            rowError->CreateCells(dataGridStatus);
            cellStyle1 = rowError->Cells[1]->Style;
            cellStyle1->ForeColor = JH_COLOR_RED;
            cellStyle1->SelectionForeColor = JH_COLOR_RED;
            rowError->Cells[1]->Style = cellStyle1;
            // create status table error row with button template
            rowErrorCmd = (DataGridViewRow ^)(rowError->Clone());
            // replace text box cell with button cell 
            DataGridViewButtonCell ^ buttonCell = gcnew DataGridViewButtonCell;
            rowErrorCmd->Cells->RemoveAt(2);
            rowErrorCmd->Cells->Add(buttonCell);
            rowErrorCmd->Cells[2]->Value = L"";
        }

        /// <summary>
        /// Set the data packet table.
        /// </summary>
	    /// <param name="clear">True if the data packet table has to be clear.</param>
        void SetDataPacketTable(bool clear)
        {
            // set a dummy data packet for clear
            if (clear == true)
            {
                dataPacketData = emptyDataPacket;
            }
            // initialize data packet table cell index
            cellIndex = DT_FIRST_DATA_ROW;
            // set data grid rows
            SetDataPacketRow(L"textDataPacketStatusWord",
                             (clear == true) || (dataPacketData->StatusWordUsed == false),
                             String::Format(L"0x{0,4:X4}", dataPacketData->StatusWordValue));
            SetDataPacketRow(L"textDataPacketPositionData",
                             (clear == true) || (dataPacketData->PositionDataUsed == false),
                             String::Format(L"0x{0,16:X16}", dataPacketData->PositionDataValue));
            SetDataPacketRow(L"textDataPacketTimestamp",
                             (clear == true) || (dataPacketData->TimestampUsed == false),
                             String::Format(L"0x{0,8:X8}", dataPacketData->TimestampValue));
            SetDataPacketRow(L"textDataPacketReferencePos1",
                             (clear == true) || (dataPacketData->ReferencePos1Used == false),
                             String::Format(L"0x{0,16:X16}", dataPacketData->ReferencePos1Value));
            SetDataPacketRow(L"textDataPacketReferencePos2",
                             (clear == true) || (dataPacketData->ReferencePos2Used == false),
                             String::Format(L"0x{0,16:X16}", dataPacketData->ReferencePos2Value));
            SetDataPacketRow(L"textDataPacketDistCodedRef",
                             (clear == true) || (dataPacketData->DistCodedRefUsed == false),
                             String::Format(L"0x{0,16:X16}", dataPacketData->DistCodedRefValue));
            SetDataPacketRow(L"textDataPacketAnalogA",
                             (clear == true) || (dataPacketData->AnalogUsed == false),
                             String::Format(L"0x{0,4:X4}", dataPacketData->AnalogAValue));
            SetDataPacketRow(L"textDataPacketAnalogB",
                             (clear == true) || (dataPacketData->AnalogUsed == false),
                             String::Format(L"0x{0,4:X4}", dataPacketData->AnalogBValue));
            SetDataPacketRow(L"textDataPacketEnDatAI1Status",
                             (clear == true) || (dataPacketData->EnDatAI1Used == false),
                             String::Format(L"0x{0,4:X4}", dataPacketData->EnDatAI1StatusValue[0]));
            SetDataPacketRow(L"textDataPacketEnDatAI1Info",
                             (clear == true) || (dataPacketData->EnDatAI1Used == false),
                             String::Format(L"0x{0,4:X4}", dataPacketData->EnDatAI1InfoValue[0]));
            SetDataPacketRow(L"textDataPacketEnDatAI2Status",
                             (clear == true) || (dataPacketData->EnDatAI2Used == false),
                             String::Format(L"0x{0,4:X4}", dataPacketData->EnDatAI2StatusValue[0]));
            SetDataPacketRow(L"textDataPacketEnDatAI2Info",
                             (clear == true) || (dataPacketData->EnDatAI2Used == false),
                             String::Format(L"0x{0,4:X4}", dataPacketData->EnDatAI2InfoValue[0]));
            // check for add info history
            // set new row number
            UInt32 usedRows = DT_FIRST_DATA_ROW + DT_DATA_ROW_NUMBER;
            if (clear == false)
            {
                if (   (dataPacketData->EnDatAI1Used == true)
                    && (dataPacketData->EnDatAI1Count > 0))
                {
                    usedRows += 1 + 2 * dataPacketData->EnDatAI1Count;
                }
                if (   (dataPacketData->EnDatAI2Used == true)
                    && (dataPacketData->EnDatAI2Count > 0))
                {
                    usedRows += 1 + 2 * dataPacketData->EnDatAI2Count;
                }
            }
            UInt32 rowCount = dataGridDataPacket->Rows->Count;
            if (rowCount < usedRows)
            {
                dataGridDataPacket->Rows->Add(usedRows - rowCount);
            }
            else if (rowCount > usedRows)
            {
                for (UInt32 i = 0; i < rowCount - usedRows; i ++)
                {
                    dataGridDataPacket->Rows->RemoveAt(rowCount - i - 1);
                }
            }
            // set add info history
            if (clear == false)
            {
                if (   (dataPacketData->EnDatAI1Used == true)
                    && (dataPacketData->EnDatAI1Count > 0))
                {
                    // add empty row
                    SetDataPacketRow2(L"", true, L"");
                    // add value rows
                    for (Int32 i = 1; i <= dataPacketData->EnDatAI1Count; i ++)
                    {
                        SetDataPacketRow(L"textHistoryEnDatAI1Status", - i,
                                         String::Format(L"0x{0,4:X4}", dataPacketData->EnDatAI1StatusValue[i]));
                        SetDataPacketRow(L"textHistoryEnDatAI1Info", - i,
                                         String::Format(L"0x{0,4:X4}", dataPacketData->EnDatAI1InfoValue[i]));
                    }
                }
                if (   (dataPacketData->EnDatAI2Used == true)
                    && (dataPacketData->EnDatAI2Count > 0))
                {
                    // add empty row
                    SetDataPacketRow2(L"", true, L"");
                    // add value rows
                    for (Int32 i = 1; i <= dataPacketData->EnDatAI2Count; i ++)
                    {
                        SetDataPacketRow(L"textHistoryEnDatAI2Status", - i,
                                         String::Format(L"0x{0,4:X4}", dataPacketData->EnDatAI2StatusValue[i]));
                        SetDataPacketRow(L"textHistoryEnDatAI2Info", - i,
                                         String::Format(L"0x{0,4:X4}", dataPacketData->EnDatAI2InfoValue[i]));
                    }
                }
            }
        }

        /// <summary>
        /// Set the data packet table row.
        /// </summary>
	    /// <param name="textName">The data packet entry name.</param>
	    /// <param name="clear">True if the data packet table has to be clear.</param>
	    /// <param name="textValue">The data packet entry value.</param>
        void SetDataPacketRow(String ^ textName, bool clear, String ^ textValue)
        {
            SetDataPacketRow2(resManager->GetString(textName), clear, textValue);
        }

        /// <summary>
        /// Set the data packet table row.
        /// </summary>
	    /// <param name="textName">The data packet entry name.</param>
	    /// <param name="index">The row index value.</param>
	    /// <param name="textValue">The data packet entry value.</param>
        void SetDataPacketRow(String ^ textName, Int32 index, String ^ textValue)
        {
            String ^ textName2 = String::Format(resManager->GetString(textName), index);
            SetDataPacketRow2(textName2, false, textValue);
        }

        /// <summary>
        /// Set the data packet table row.
        /// </summary>
	    /// <param name="textName">The data packet entry name.</param>
	    /// <param name="clear">True if the data packet table has to be clear.</param>
	    /// <param name="textValue">The data packet entry value.</param>
        void SetDataPacketRow2(String ^ textName, bool clear, String ^ textValue)
        {
            if (clear == true)
            {
                textValue = L"";
            }
            dataGridDataPacket->Rows[cellIndex]->Cells[0]->Value = textName;
            dataGridDataPacket->Rows[cellIndex]->Cells[1]->Value = textValue;
            cellIndex ++;
            if (Int32(cellIndex) > dataGridDataPacket->Rows->Count)
            {
                throw gcnew Exception(L"Internal error 17");
            }
        }

        /// <summary>
        /// Set the status table.
        /// </summary>
	    /// <param name="clear">True if the status table has to be clear.</param>
	    /// <param name="isRunning">True if the system is in running mode.</param>
	    /// <param name="isPolling">True if the system is in running and polling mode.</param>
        void SetStatusTable(bool clear, bool isRunning, bool isPolling)
        {
            dataGridStatus->Visible = !clear;
            if (clear == true)
            {
                return;
            }
            // initialize status table
            UInt32 newTableMode = 0;
            // check status to show
            if (isIncremental == true)
            {
                // set status table mode
                if (dataPacketData->StatusWordUsed == true)
                {
                    // show incremental and auxiliary axis position status
                    if (isAuxAxis == false)
                    {
                        // show incremental position status
                        newTableMode |= ST_TABLE_MODE_INCR;
                    }
                    else
                    {
                        // show auxiliary axis position status
                        newTableMode |= ST_TABLE_MODE_AUX;
                    }
                }
            }
            else
            {
                // calculate status table row number
                if (dataPacketData->StatusWordUsed == true)
                {
                    // show EnDat position status
                    newTableMode |= ST_TABLE_MODE_EN_DAT;
                }
                if (dataPacketData->EnDatAI1Used == true)
                {
                    // show EnDat add info 1 status
                    newTableMode |= ST_TABLE_MODE_ADD_INFO;
                }
                if (dataPacketData->EnDatAI2Used == true)
                {
                    // show EnDat add info 2 status
                    newTableMode |= ST_TABLE_MODE_ADD_INFO;
                }
            }
            // check status table mode
            if (newTableMode != statusTableMode)
            {
                dataGridStatus->Rows->Clear();
            }
            statusTableMode = newTableMode;
            cellIndex = ST_FIRST_DATA_ROW;
            String ^ textButton;
            // check status to show
            if (isIncremental == true)
            {
                // set status table for incremental encoders
                if (dataPacketData->StatusWordUsed == true)
                {
                    // show incremental and auxiliary axis position status
                    SetStatusRow(L"textDataPacketStatusWord", StatusTableMode::Header);
                    SetStatusRow(L"textStatusPositionValid", StatusTableMode::Status,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::PositionValid) != 0);
                    SetStatusRow(L"textStatusPositionNotValid", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::PositionValid) == 0);
                    SetStatusRow(L"textStatusSignalError", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorAmplitude) != 0,
                                  DataPacketCmd::ResetErrorSignal, L"textButtonReset");
                    SetStatusRow(L"textStatusFrequencyError", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorFrequency) != 0,
                                  DataPacketCmd::ResetErrorSignal, L"textButtonReset");
                    if (isAuxAxis == false)
                    {
                        SetStatusRow(L"textStatusPowerError", StatusTableMode::Error,
                                     (  dataPacketData->StatusWordValue
                                      & (UInt16)DataPacketStatus::ErrorPower) != 0,
                                      DataPacketCmd::ResetErrorPower, L"textButtonReset");
                    }
                    else
                    {
                        SetStatusRow(L"textStatusPowerError", StatusTableMode::Error,
                                     (  dataPacketData->StatusWordValue
                                      & (UInt16)DataPacketStatus::ErrorPower) != 0);
                    }
                    SetStatusRow(L"textStatusFanError", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorFan) != 0);
                    SetStatusRow(L"textStatusLostTrigger", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorLostTrigger) != 0,
                                 DataPacketCmd::ResetErrorLostTrigger, L"textButtonReset");
                    SetStatusRow(L"textStatusRefPos1Saved", StatusTableMode::Status,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::RefPos1Valid) != 0);
                    if (isAuxAxis == false)
                    {
                        // show incremental position status
                        SetStatusRow(L"textStatusRefPos2Saved", StatusTableMode::Status,
                                     (  dataPacketData->StatusWordValue
                                      & (UInt16)DataPacketStatus::RefPos2Valid) != 0);
                        SetStatusRow(L"textStatusRefPosCValid", StatusTableMode::Status,
                                     (  dataPacketData->StatusWordValue
                                      & (UInt16)DataPacketStatus::RefPosCValid) != 0);
                        SetStatusRow(L"textStatusRefPosCError", StatusTableMode::Error,
                                     (  dataPacketData->StatusWordValue
                                      & (UInt16)DataPacketStatus::ErrorRefPosC) != 0,
                                     DataPacketCmd::ResetErrorRefC, L"textButtonReset");
                    }
                }
            }
            else
            {
                // set status table for EnDat encoders
                if (dataPacketData->StatusWordUsed == true)
                {
                    // show EnDat position status
                    SetStatusRow(L"textDataPacketStatusWord", StatusTableMode::Header);
                    SetStatusRow(L"textStatusPositionValid", StatusTableMode::Status,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::PositionValid) != 0);
                    SetStatusRow(L"textStatusPositionNotValid", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::PositionValid) == 0);
                    if (   (isRunning == true)
                        && (isPolling == false))
                    {
                        textButton = L"";
                    }
                    else
                    {
                        textButton = L"textButtonReset";
                    }
                    SetStatusRow(L"textStatusCrcError", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorEnDatCRC) != 0,
                                  DataPacketCmd::ResetErrorSignal, textButton);
                    SetStatusRow(L"textStatusPowerError", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorPower) != 0,
                                  DataPacketCmd::ResetErrorPower, L"textButtonReset");
                    SetStatusRow(L"textStatusFanError", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorFan) != 0);
                    SetStatusRow(L"textStatusLostTrigger", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorLostTrigger) != 0,
                                 DataPacketCmd::ResetErrorLostTrigger, L"textButtonReset");
                    if (   (isRunning == true)
                        && (isPolling == false))
                    {
                        textButton = L"";
                    }
                    else
                    {
                        textButton = L"textButtonReset";
                    }
                    SetStatusRow(L"textStatusEnDatError1", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorEnDat1) != 0,
                                  DataPacketCmd::ResetErrorSignal, textButton);
                    SetStatusRow(L"textStatusEnDatError2", StatusTableMode::Error,
                                 (  dataPacketData->StatusWordValue
                                  & (UInt16)DataPacketStatus::ErrorEnDat2) != 0,
                                  DataPacketCmd::ResetErrorSignal, textButton);
                    // check for special warnings
                    // check for lost trigger status
                    if (   (axisWarnings->WarningBadPositionParam == false)
                        && ((  dataPacketData->StatusWordValue
                             & (UInt16)DataPacketStatus::ErrorLostTrigger) != 0))
                    {
                        // report warning
                        String ^ message = String::Format(resManager->GetString(L"warningLostTriggerStatus"),
                                                          eibTreeList->SelectedEIB->EibAxisName[axisIndex + 1]);
                        guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                    message);
                        // set warning flag
                        axisWarnings->WarningBadPositionParam = true;
                    }
                }
                if (dataPacketData->EnDatAI1Used == true)
                {
                    // show EnDat add info 1 status
                    SetStatusRow(L"textDataPacketEnDatAI1", StatusTableMode::Header);
                    SetStatusRow(L"textStatusAddInfoValid", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::EnDatAddInfoValid) != 0);
                    if (   (isRunning == true)
                        && (isPolling == false))
                    {
                        textButton = L"";
                    }
                    else
                    {
                        textButton = L"textButtonReset";
                    }
                    SetStatusRow(L"textStatusCrcError", StatusTableMode::Error,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::ErrorEnDatCRC) != 0,
                                  DataPacketCmd::ResetErrorSignal, textButton);
                    SetStatusRow(L"textStatusContents0", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI0) != 0);
                    SetStatusRow(L"textStatusContents1", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI1) != 0);
                    SetStatusRow(L"textStatusContents2", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI2) != 0);
                    SetStatusRow(L"textStatusContents3", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI3) != 0);
                    SetStatusRow(L"textStatusContents4", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI4) != 0);
                    SetStatusRow(L"textStatusEnDatBusy", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::EnDatBusy) != 0);
                    SetStatusRow(L"textStatusEnDatRM", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::EnDatRM) != 0);
                    SetStatusRow(L"textStatusEnDatWRN", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI1StatusValue[0]
                                  & (UInt16)DataPacketStatus::EnDatWRN) != 0);
                }
                if (dataPacketData->EnDatAI2Used == true)
                {
                    // show EnDat add info 2 status
                    SetStatusRow(L"textDataPacketEnDatAI2", StatusTableMode::Header);
                    SetStatusRow(L"textStatusAddInfoValid", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::EnDatAddInfoValid) != 0);
                    if (   (isRunning == true)
                        && (isPolling == false))
                    {
                        textButton = L"";
                    }
                    else
                    {
                        textButton = L"textButtonReset";
                    }
                    SetStatusRow(L"textStatusCrcError", StatusTableMode::Error,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::ErrorEnDatCRC) != 0,
                                  DataPacketCmd::ResetErrorSignal, textButton);
                    SetStatusRow(L"textStatusContents0", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI0) != 0);
                    SetStatusRow(L"textStatusContents1", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI1) != 0);
                    SetStatusRow(L"textStatusContents2", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI2) != 0);
                    SetStatusRow(L"textStatusContents3", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI3) != 0);
                    SetStatusRow(L"textStatusContents4", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::ContentsI4) != 0);
                    SetStatusRow(L"textStatusEnDatBusy", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::EnDatBusy) != 0);
                    SetStatusRow(L"textStatusEnDatRM", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::EnDatRM) != 0);
                    SetStatusRow(L"textStatusEnDatWRN", StatusTableMode::Status,
                                 (  dataPacketData->EnDatAI2StatusValue[0]
                                  & (UInt16)DataPacketStatus::EnDatWRN) != 0);
                }
            }
        }

        /// <summary>
        /// Set the status table row.
        /// </summary>
	    /// <param name="textName">The status entry name.</param>
	    /// <param name="mode">The status table row mode.</param>
        void SetStatusRow(String ^ textName, StatusTableMode mode)
        {
            SetStatusRow(textName, mode, false);
        }

        /// <summary>
        /// Set the status table row.
        /// </summary>
	    /// <param name="textName">The status entry name.</param>
	    /// <param name="mode">The status table row mode.</param>
	    /// <param name="active">True if the status entry is set.</param>
        void SetStatusRow(String ^ textName, StatusTableMode mode, bool active)
        {
            SetStatusRow(textName, mode, active, DataPacketCmd::None, L"");
        }

        /// <summary>
        /// Set the status table row.
        /// </summary>
	    /// <param name="textName">The status entry name.</param>
	    /// <param name="mode">The status table row mode.</param>
	    /// <param name="active">True if the status entry is set.</param>
	    /// <param name="cmd">The status table row button command.</param>
	    /// <param name="textButton">The status table row button text.</param>
        void SetStatusRow(String ^ textName, StatusTableMode mode, bool active,
                          DataPacketCmd cmd, String ^ textButton)
        {
            // ATTENTION: dataGridStatus->AllowUserToAddRows = false !!!
            // check row count
            UInt32 rowCount = dataGridStatus->Rows->Count;
            if (cellIndex == rowCount)
            {
                // add new row
                if (mode == StatusTableMode::Header)
                {
                    // add header row
                    dataGridStatus->Rows->Add((DataGridViewRow ^)(rowHeader->Clone()));
                }
                else if (mode == StatusTableMode::Error)
                {
                    if (cmd == DataPacketCmd::None)
                    {
                        // add error row without command button
                        dataGridStatus->Rows->Add((DataGridViewRow ^)(rowError->Clone()));
                    }
                    else
                    {
                        // add error row with command button
                        dataGridStatus->Rows->Add((DataGridViewRow ^)(rowErrorCmd->Clone()));
                    }
                }
                else
                {
                    // add status row
                    dataGridStatus->Rows->Add();
                }
            }
            // set cell content
            String ^ textValue = ST_VALUE_NOT_USED_TEXT;
            if (   (mode == StatusTableMode::Status)
                || (mode == StatusTableMode::Error))
            {
                if (active == true)
                {
                    // set status text
                    textValue = ST_VALUE_SET_TEXT;
                    // enable command button
                    if (cmd != DataPacketCmd::None)
                    {
                        if (textButton != L"")
                        {
                            dataGridStatus->Rows[cellIndex]->Cells[2]->Value = resManager->
                                                                                    GetString(textButton);
                            dataGridStatus->Rows[cellIndex]->Cells[2]->Tag   = cmd;
                        }
                        else
                        {
                            dataGridStatus->Rows[cellIndex]->Cells[2]->Value = L"";
                            dataGridStatus->Rows[cellIndex]->Cells[2]->Tag   = DataPacketCmd::None;
                        }
                    }
                }
                else
                {
                    // set status text
                    textValue = ST_VALUE_NOT_SET_TEXT;
                    // disable command button
                    if (cmd != DataPacketCmd::None)
                    {
                        dataGridStatus->Rows[cellIndex]->Cells[2]->Value = L"";
                        dataGridStatus->Rows[cellIndex]->Cells[2]->Tag   = DataPacketCmd::None;
                    }
                }
            }
            dataGridStatus->Rows[cellIndex]->Cells[0]->Value = resManager->GetString(textName);
            dataGridStatus->Rows[cellIndex]->Cells[1]->Value = textValue;
            // set new cell index
            cellIndex ++;
        }

        /// <summary>
        /// Clear the data packet display.
        /// </summary>
        void ClearDisplay()
        {
            // clear the data packet table
            SetDataPacketTable(true);
            // clear the status report list
            SetStatusTable(true, false, false);
        }

        /// <summary>
        /// Refresh the data packet display.
        /// </summary>
		/// <param name="parameter">The EIB parameters.</param>
		/// <param name="displayMode">The display mode flags.</param>
        void RefreshDisplay(EIB_Param ^ parameter, UInt32 displayMode)
        {
            // initialize flags
            bool isRunning   = ((displayMode & DISPLAY_MODE_RUNNING)   != 0);
            bool isPolling   = ((displayMode & DISPLAY_MODE_POLLING)   != 0);
            bool isRecording = ((displayMode & DISPLAY_MODE_RECORDING) != 0);
            bool displayDataPacket = false;
            bool displayStatus     = false;
            // get data packet data from axis index
            GetDataPacketData(parameter);
            // check if axis is enabled
            if (isEnabled == true)
            {
                if (   (isRunning      == true)
                    || (showLastValues == true)
                    || (isRecording    == true))
                {
                    // set the data packet table
                    SetDataPacketTable(false);
                    displayDataPacket = true;
                    // set the status table
                    if (   (dataPacketData->StatusWordUsed == true)
                        || (dataPacketData->EnDatAI1Used   == true)
                        || (dataPacketData->EnDatAI2Used   == true))
                    {
                        SetStatusTable(false, isRunning, isPolling);
                        displayStatus = true;
                    }
                }
            }
            // disable / clear unused controls
            if (displayDataPacket == false)
            {
                SetDataPacketTable(true);
            }
            if (displayStatus == false)
            {
                SetStatusTable(true, false, false);
            }
        }
    };
	#pragma endregion
}
