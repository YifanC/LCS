///
/// \file       FormMain_Callback.cpp
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: FormMain_Callback.cpp,v 1.2 2011/09/20 12:24:35 rohn Exp $
/// $Name:  $
/// $Revision: 1.2 $
/// $State: Exp $
/// $Log: FormMain_Callback.cpp,v $
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      Main window class callback functions.
///

#include "StdAfx.h"
#include "FormMain.h"
#include "FormInfo.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Resources;
using namespace System::Reflection;
using namespace System::Diagnostics;

namespace EIB7xx_App
{

    #pragma warning(push)
    #pragma warning(disable: 4100)

    /// <summary>
    /// Event handler for the Menu File/Exit click event.
    /// </summary>
    System::Void FormMain::exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // exit application
        Application::Exit();
    }

    /// <summary>
    /// Event handler for the Main Form closing event.
    /// </summary>
    System::Void FormMain::FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
    {
        // check if system is in running state
        if (guiState.OpModeState == GUI_StateOpMode::Run)
        {
            // system is in running state
            Windows::Forms::DialogResult result =
                        guiSupport->MsgBoxWarning(resManager->GetString(L"warningCancelRun"),
                                                  MessageBoxButtons::YesNo,
                                                  MessageBoxDefaultButton::Button2);
            if (result == ::DialogResult::No)
            {
                // cancel closing
                e->Cancel = true;
            }
            else
            {
                // stop running state
                StopEIBMeasurement(false);
            }
        }
        // check if EIB parameters had been changed
        if (   (e->Cancel == false)
            && (CheckEIBParametersChanged() == true)
            && (treeViewMain->Nodes[0]->GetNodeCount(false) > 0))
        {
            for (;;)
            {
                // EIB parameters had been changed
                Windows::Forms::DialogResult result =
                            guiSupport->MsgBoxWarning(resManager->GetString(L"warningParamChanged"),
                                                      MessageBoxButtons::YesNoCancel,
                                                      MessageBoxDefaultButton::Button1);
                if (result == ::DialogResult::Cancel)
                {
                    // cancel closing
                    e->Cancel = true;
                    break;
                }
                else if (result == ::DialogResult::Yes)
                {
                    // save EIB parameters
                    bool saveCancelled = SaveEIBParameters(true);
                    if (saveCancelled == false)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        // check for close
        if (e->Cancel == false)
        {
            // write GUI configuration parameters to INI file
            guiConfig.WriteConfigParam();
            // set closing flag
            isClosing = true;
        }
    }

    /// <summary>
    /// Event handler for the Menu File/Info click event.
    /// </summary>
    System::Void FormMain::infoToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // Show Info Dialog
        FormInfo^ formInfo = gcnew FormInfo();
        formInfo->ShowDialog(this);
    }

    /// <summary>
    /// Event handler for the Menu File/Clear Report List click event.
    /// </summary>
    System::Void FormMain::clearReportListToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // clear report list
        richTextBoxReport->Clear();
    }

    /// <summary>
    /// Event handler for the Menu File/Open Project... click event.
    /// </summary>
    System::Void FormMain::openProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // check if EIB parameters had been changed
        if (   (CheckEIBParametersChanged() == true)
            && (treeViewMain->Nodes[0]->GetNodeCount(false) > 0))
        {
            for (;;)
            {
                // EIB parameters had been changed
                Windows::Forms::DialogResult result =
                            guiSupport->MsgBoxWarning(resManager->GetString(L"warningParamChanged"),
                                                      MessageBoxButtons::YesNoCancel,
                                                      MessageBoxDefaultButton::Button1);
                if (result == ::DialogResult::Cancel)
                {
                    // cancel open project
                    return;
                }
                else if (result == ::DialogResult::Yes)
                {
                    // save EIB parameters
                    bool saveCancelled = SaveEIBParameters(true);
                    if (saveCancelled == false)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        // read EIB parameters
        ReadEIBParameters();
    }

    /// <summary>
    /// Event handler for the Menu File/Save Project click event.
    /// </summary>
    System::Void FormMain::saveProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // save EIB parameters
        SaveEIBParameters(false);
    }

    /// <summary>
    /// Event handler for the Menu File/Save Project As... click event.
    /// </summary>
    System::Void FormMain::saveAsProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // save EIB parameters
        SaveEIBParameters(true);
    }

    /// <summary>
    /// Event handler for the Menu Configuration click event.
    /// </summary>
    System::Void FormMain::configToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // save current tab page
            if (guiState.MenuState == GUI_StateMenu::Position)
            {
                lastPositionTab = tabControlMain->SelectedTab;
            }
            // trigger a new GUI state
            TriggerGuiState(GUI_StateTrigger::MenuConfig, lastConfigTab);
            // refresh parameter inputs
            RefreshParam();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Menu Position click event.
    /// </summary>
    System::Void FormMain::positionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // save current tab page
            if (guiState.MenuState == GUI_StateMenu::Config)
            {
                lastConfigTab = tabControlMain->SelectedTab;
            }
            // trigger a new GUI state
            TriggerGuiState(GUI_StateTrigger::MenuPosition, lastPositionTab);
            // set recording measurement state due to the tab page changed
            SetRecordingState(RecordingTrigger::TabPage);
            // refresh parameter inputs
            RefreshParam();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the ToolStrip Button Add EIB click event.
    /// </summary>
    System::Void FormMain::toolStripButtonEibAdd_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // set state guard flag
            stateGuard = GUI_StateTrigger::AddEIB;
            // add new EIB to system
            String ^ eibName = eibTreeList->AddNewEIB();
            // trigger a new GUI state
            TriggerGuiState(GUI_StateTrigger::AddEIB);
            // select network tab
            tabControlMain->SelectedTab = tabPageNetwork;
            // a project parameter has been changed
            EIBParameterChanged();
            // report success
            guiSupport->ShowInfoText(String::Format(resManager->GetString(L"infoEibAdd"), eibName));
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // reset state guard flag
            stateGuard = GUI_StateTrigger::NotUsed;
        }
    }

    /// <summary>
    /// Event handler for the ToolStrip Button Remove EIB click event.
    /// </summary>
    System::Void FormMain::toolStripButtonEibDel_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // set state guard flag
            stateGuard = GUI_StateTrigger::RemoveEIB;
            // remove selected EIB from system
            String ^ eibName = eibTreeList->RemoveSelectedEIB();
            // trigger a new GUI state
            TriggerGuiState(GUI_StateTrigger::RemoveEIB);
            // a project parameter has been changed
            EIBParameterChanged();
            // report success
            guiSupport->ShowInfoText(String::Format(resManager->GetString(L"infoEibDel"), eibName));
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // reset state guard flag
            stateGuard = GUI_StateTrigger::NotUsed;
        }
    }

    /// <summary>
    /// Event handler for the ToolStrip Button Init EIB click event.
    /// </summary>
    System::Void FormMain::toolStripButtonInit_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // initialize EIB(s)
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // initialize EIB(s)
            InitializeEIBs();
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoEibInitSuccess"));
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoEibInitError"),
                                      ex->Message);
        }
        finally
        {
            this->Cursor = cursor;
        }
    }

    /// <summary>
    /// Event handler for the ToolStrip Button Run Measurement click event.
    /// </summary>
    System::Void FormMain::toolStripButtonRun_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // start running state
        StartEIBMeasurement();
    }

    /// <summary>
    /// Event handler for the ToolStrip Button Stop Measurement click event.
    /// </summary>
    System::Void FormMain::toolStripButtonStop_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // stop running state
        StopEIBMeasurement(false);
    }

    /// <summary>
    /// Event handler for the Display Timer Tick event.
    /// </summary>
    System::Void FormMain::timerDisplay_Tick(System::Object^  sender, System::EventArgs^  e)
    {
        // process running state
        RunEIBMeasurement();
        // manage the blinking controls
        BlinkDisplay();
    }

    /// <summary>
    /// Event handler for the Main Tree View Selection Changed event.
    /// </summary>
    System::Void FormMain::treeViewMain_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
    {
        try
        {
            if (e->Node->Parent == nullptr)
            {
                // no EIB selected
                // set selected EIB
                eibTreeList->SetSelectedEIB(nullptr);
                // trigger a new GUI state
                TriggerGuiState(GUI_StateTrigger::SelectDevice);
            }
            else
            {
                // EIB or axis selected
                EIB_Entry ^ oldSelectedEIB = eibTreeList->SelectedEIB;
                // set selected EIB
                bool eibSelected = eibTreeList->SetSelectedEIB(e->Node);
                // trigger a new GUI state
                if (eibSelected == true)
                {
                    TriggerGuiState(GUI_StateTrigger::SelectEIB);
                }
                else
                {
                    TriggerGuiState(GUI_StateTrigger::SelectAxis);
                }
                //guiSupport->ShowDebugText(String::Format(L"Tree item selected: {0}, AxisIndex: {1}, AxisType: {2}\n",
                //                                         eibTreeList->SelectedEIB->Parameter->EibName,
                //                                         eibTreeList->AxisIndex,
                //                                         eibTreeList->IsAuxAxis));
                // initialize display of recording measurement values
                // due to the selected EIB changed
                if (eibTreeList->SelectedEIB != oldSelectedEIB)
                {
                    RefreshRecordingDisplay(0);
                }
                // refresh parameter inputs
                RefreshParam();
            }
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Main Tab Control Selection Changed event.
    /// </summary>
    System::Void FormMain::tabControlMain_Selected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e)
    {
        try
        {
            if (e->TabPage != nullptr)
            {
                // trigger a new GUI state
                if ((String ^)(e->TabPage->Tag) == L"EIB")
                {
                    TriggerGuiState(GUI_StateTrigger::TabEIB);
                }
                else
                {
                    TriggerGuiState(GUI_StateTrigger::TabAxis);
                }
                //guiSupport->ShowDebugText(String::Format(L"Tab page selected: {0}\n",
                //                                         e->TabPage->Tag));
                // set recording measurement state due to the tab page changed
                SetRecordingState(RecordingTrigger::TabPage);
                // refresh parameter inputs
                RefreshParam();
            }
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Network Tab Property Grid Value Changed event.
    /// </summary>
    System::Void FormMain::propertyGridNetwork_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e)
    {
        try
        {
            //guiSupport->ShowDebugText(String::Format(L"Item changed: {0}, Old: {1}, New: {2}\n",
            //                                         e->ChangedItem->Label,
            //                                         e->OldValue,
            //                                         e->ChangedItem->Value));
            // an EIB network parameter has been changed
            eibTreeList->SelectedEIB->ThePropertyNetwork->PropertyChanged(
                                                        e->ChangedItem->PropertyDescriptor->Name);
            // a parameter has been changed
            EIBParameterChanged();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Network Tab Read Button Click event.
    /// </summary>
    System::Void FormMain::buttonNetworkRead_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // read network parameters from EIB
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // read network parameters from EIB
            eibTreeList->SelectedEIB->ReadNetworkParam();
            // refresh Network Tab
            eibTreeList->SelectedEIB->ThePropertyNetwork->RefreshParam();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoEibReadError"),
                                      ex->Message);
        }
        finally
        {
            this->Cursor = cursor;
        }
    }

    /// <summary>
    /// Event handler for the Network Tab Write Button Click event.
    /// </summary>
    System::Void FormMain::buttonNetworkWrite_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // write network parameters to EIB
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // write network parameters to EIB
            eibTreeList->SelectedEIB->WriteNetworkParam();
            // refresh Network Tab
            eibTreeList->SelectedEIB->ThePropertyNetwork->RefreshParam();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoEibWriteError"),
                                      ex->Message);
        }
        finally
        {
            this->Cursor = cursor;
        }
    }

    /// <summary>
    /// Event handler for the Network Tab Reset Button Click event.
    /// </summary>
    System::Void FormMain::buttonEibReset_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // reset EIB
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // report operation
            guiSupport->ShowWarningText(resManager->GetString(L"infoEibResetWait"));
            // refresh window
            this->Refresh();
            // clear init list
            eibInitList->Clear();
            // reset EIB
            GuiCmdFunction(GuiCommand::ResetEib, 10000);
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoEibResetSuccess"));
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoEibResetError"),
                                      ex->Message);
        }
        finally
        {
            this->Cursor = cursor;
        }
    }

    /// <summary>
    /// Event handler for the Network Tab Firmware Button Click event.
    /// </summary>
    System::Void FormMain::buttonEibFirmware_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // select firmware file
        try
        {
            OpenFileDialog ^ fileDialog = gcnew OpenFileDialog;
            fileDialog->Title = resManager->GetString(L"dialogFirmwareFile");
            fileDialog->InitialDirectory = guiSupport->GuiConfig->GuiParam->FirmwareFileName;
            fileDialog->Filter = String::Format(EIB_FILE_FILTER,
                                                FIRMWARE_FILE_EXT,
                                                resManager->GetString(L"textFirmwareFile"),
                                                resManager->GetString(L"textAllFiles"));
            fileDialog->FilterIndex = 1;
            fileDialog->DefaultExt = FIRMWARE_FILE_EXT;
            fileDialog->AddExtension = true;
            fileDialog->Multiselect = false;
            fileDialog->CheckFileExists = true;
            Windows::Forms::DialogResult result = fileDialog->ShowDialog();
            if (result != ::DialogResult::OK)
            {
                return;
            }
            guiSupport->GuiConfig->GuiParam->FirmwareFileName = fileDialog->FileName;
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoFirmwareLoadError"),
                                      ex->Message);
            return;
        }
        // update EIB firmware
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // report operation
            guiSupport->ShowWarningText(resManager->GetString(L"infoFirmwareWait"));
            // refresh window
            this->Refresh();
            // update EIB firmware
            GuiCmdFunction(GuiCommand::UpdateEib, 60000);
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoFirmwareSuccess"),
                                     guiSupport->GuiConfig->GuiParam->FirmwareFileName);
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoFirmwareError"),
                                      ex->Message);
            return;
        }
        finally
        {
            this->Cursor = cursor;
        }
        // reset EIB
        buttonEibReset_Click(sender, e);
    }

    /// <summary>
    /// Event handler for the Axis Tab Property Grid Value Changed event.
    /// </summary>
    System::Void FormMain::propertyGridAxis_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e)
    {
        try
        {
            // an EIB axis parameter has been changed
            // refresh Axis Tab
            eibTreeList->SelectedEIB->RefreshAxisParam(eibTreeList->AxisIndex, eibTreeList->IsAuxAxis);
            // refresh Data Packet Tab
            propertyDataPacket->RefreshParam();
            // a parameter has been changed
            EIBParameterChanged();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Trigger Tab Property Grid Value Changed event.
    /// </summary>
    System::Void FormMain::propertyGridTrigger_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e)
    {
        try
        {
            // an EIB trigger parameter has been changed
            // refresh Trigger Tab
            eibTreeList->SelectedEIB->ThePropertyTrigger->RefreshParam();
            // a parameter has been changed
            EIBParameterChanged();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Operation Mode Tab Property Grid Value Changed event.
    /// </summary>
    System::Void FormMain::propertyGridOpMode_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e)
    {
        try
        {
            // a parameter has been changed
            EIBParameterChanged();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Crankshaft Tab Property Grid Value Changed event.
    /// </summary>
    System::Void FormMain::propertyGridCrankshaft_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e)
    {
        try
        {
            // a parameter has been changed
            EIBParameterChanged();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Display Tab Property Grid Value Changed event.
    /// </summary>
    System::Void FormMain::propertyGridDisplay_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e)
    {
        try
        {
            // a parameter has been changed
            EIBParameterChanged();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Summary / Details Tab Start REF Button Click event.
    /// </summary>
    System::Void FormMain::buttonRefAxis_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // Summary / Details Tab Start REF Button
            if (((Button ^)sender)->Name == L"buttonRefAxis0")
            {
                eibTreeList->SelectedEIB->AxisStartRef(eibTreeList->AxisIndex,
                                                       eibTreeList->IsAuxAxis,
                                                       true);
            }
            if (((Button ^)sender)->Name == L"buttonRefAxis1")
            {
                eibTreeList->SelectedEIB->AxisStartRef(0, false, true);
            }
            if (((Button ^)sender)->Name == L"buttonRefAxis2")
            {
                eibTreeList->SelectedEIB->AxisStartRef(1, false, true);
            }
            if (((Button ^)sender)->Name == L"buttonRefAxis3")
            {
                eibTreeList->SelectedEIB->AxisStartRef(2, false, true);
            }
            if (((Button ^)sender)->Name == L"buttonRefAxis4")
            {
                eibTreeList->SelectedEIB->AxisStartRef(3, false, true);
            }
            // refresh display tabs
            RefreshDisplay();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Summary / Details Tab Clear Button Click event.
    /// </summary>
    System::Void FormMain::buttonClearAxis_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // Summary / Details Tab Clear Button
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // Summary / Details Tab Clear Button
            if (((Button ^)sender)->Name == L"buttonClearAxis0")
            {
                eibTreeList->SelectedEIB->AxisClear(eibTreeList->AxisIndex, eibTreeList->IsAuxAxis);
            }
            if (((Button ^)sender)->Name == L"buttonClearAxis1")
            {
                eibTreeList->SelectedEIB->AxisClear(0, false);
            }
            if (((Button ^)sender)->Name == L"buttonClearAxis2")
            {
                eibTreeList->SelectedEIB->AxisClear(1, false);
            }
            if (((Button ^)sender)->Name == L"buttonClearAxis3")
            {
                eibTreeList->SelectedEIB->AxisClear(2, false);
            }
            if (((Button ^)sender)->Name == L"buttonClearAxis4")
            {
                eibTreeList->SelectedEIB->AxisClear(3, false);
            }
            // refresh display tabs
            RefreshDisplay();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
        finally
        {
            this->Cursor = cursor;
        }
    }

    /// <summary>
    /// Event handler for the Details Tab Status Table Cell Click event.
    /// </summary>
    System::Void FormMain::dataGridViewDetailStatus_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
    {
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // get reset command from table cell tag
            DataGridView ^ statusTable = (DataGridView ^)sender;
            Object ^ cellTag = statusTable->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Tag;
            DataPacketCmd cmd = DataPacketCmd::None;
            if (cellTag != nullptr)
            {
                cmd = (DataPacketCmd)cellTag;
                if (cmd != DataPacketCmd::None)
                {
                    eibTreeList->SelectedEIB->AxisResetError(eibTreeList->AxisIndex,
                                                             eibTreeList->IsAuxAxis,
                                                             cmd);
                    // refresh display tabs
                    RefreshDisplay();
                }
            }
            //Debug::Print(String::Format(L"Cell cmd: Row: {0}, Column: {1}, Cmd: {2}",
            //                            e->RowIndex, e->ColumnIndex, cmd));
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
        finally
        {
            this->Cursor = cursor;
        }
    }

    /// <summary>
    /// Event handler for the Summary / Details Tab UpDown Value changed event.
    /// </summary>
    System::Void FormMain::numericUpDownIndex_ValueChanged(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // refresh display of recording measurement values 
            RefreshRecordingDisplay(UInt32(numericUpDownIndex->Value));
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Summary / Details Tab Track Bar Value changed event.
    /// </summary>
    System::Void FormMain::trackBarIndex_ValueChanged(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // refresh display of recording measurement values 
            RefreshRecordingDisplay(trackBarIndex->Value);
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Summary / Details Tab Index Label Click event.
    /// </summary>
    System::Void FormMain::labelIndex_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // refresh display of recording measurement values 
            RefreshRecordingDisplay(UInt32(numericUpDownIndex->Value));
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Summary / Details Tab Recording Display Run Click event.
    /// </summary>
    System::Void FormMain::buttonRecordDisplayRun_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // set recording display run mode
            SetRecordingDisplayRun(!recordDisplayRun);
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Summary / Details Tab Recording Display Save Click event.
    /// </summary>
    System::Void FormMain::buttonSaveRecordingData_Click(System::Object^  sender, System::EventArgs^  e)
    {
        // select measure data file
        SelectDataFile();
        // save recording data
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // report start saving
            guiSupport->ShowInfoText(resManager->GetString(L"infoDataSaveStart"));
            // set recording measurement state to save
            SetRecordingState(RecordingTrigger::Save);
            // refresh window
            this->Refresh();
            // save recording data
            eibTreeList->SaveRecordingData(dataFileName);
            //GuiCmdFunction(GuiCommand::SaveRecordingData, 0); does not work because of progress bar invoke
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoDataSaveSuccess"),
                                     dataFileName);
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoDataSaveError"),
                                      ex->Message);
        }
        finally
        {
            this->Cursor = cursor;
            // set recording measurement state to stop
            try
            {
                SetRecordingState(RecordingTrigger::Stop);
            }
            catch (Exception ^ ex)
            {
                guiSupport->MsgBoxErrorOk(ex->Message);
            }
        }
    }

    /// <summary>
    /// Event handler for the Crankshaft Display Tab Stop at 360° Click event.
    /// </summary>
    System::Void FormMain::checkBoxStopAt360_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Crankshaft Display Tab Save Reference Click event.
    /// </summary>
    System::Void FormMain::buttonCrankshaftRefSave_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // select reference file
            SaveFileDialog ^ fileDialog = gcnew SaveFileDialog;
            fileDialog->Title = resManager->GetString(L"dialogReferenceFile");
            fileDialog->InitialDirectory = guiSupport->GuiConfig->GuiParam->ReferenceFileName;
            fileDialog->Filter = String::Format(EIB_FILE_FILTER,
                                                REFERENCE_FILE_EXT,
                                                resManager->GetString(L"textReferenceFile"),
                                                resManager->GetString(L"textAllFiles"));
            fileDialog->FilterIndex = 1;
            fileDialog->DefaultExt = REFERENCE_FILE_EXT;
            fileDialog->AddExtension = true;
            fileDialog->CheckFileExists = false;
            fileDialog->OverwritePrompt = true;
            Windows::Forms::DialogResult result = fileDialog->ShowDialog();
            if (result != ::DialogResult::OK)
            {
                return;
            }
            guiSupport->GuiConfig->GuiParam->ReferenceFileName = fileDialog->FileName;
            // save crankshaft reference
            crankshaftMeasure->SaveReference(guiSupport->GuiConfig->GuiParam->ReferenceFileName);
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoReferenceSaveSuccess"),
                                     guiSupport->GuiConfig->GuiParam->ReferenceFileName);
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoReferenceSaveError"),
                                      ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Crankshaft Display Tab Load Reference Click event.
    /// </summary>
    System::Void FormMain::buttonCrankshaftRefLoad_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // select reference file
            OpenFileDialog ^ fileDialog = gcnew OpenFileDialog;
            fileDialog->Title = resManager->GetString(L"dialogReferenceFile");
            fileDialog->InitialDirectory = guiSupport->GuiConfig->GuiParam->ReferenceFileName;
            fileDialog->Filter = String::Format(EIB_FILE_FILTER,
                                                REFERENCE_FILE_EXT,
                                                resManager->GetString(L"textReferenceFile"),
                                                resManager->GetString(L"textAllFiles"));
            fileDialog->FilterIndex = 1;
            fileDialog->DefaultExt = REFERENCE_FILE_EXT;
            fileDialog->AddExtension = true;
            fileDialog->Multiselect = false;
            fileDialog->CheckFileExists = true;
            Windows::Forms::DialogResult result = fileDialog->ShowDialog();
            if (result != ::DialogResult::OK)
            {
                return;
            }
            guiSupport->GuiConfig->GuiParam->ReferenceFileName = fileDialog->FileName;
            // load crankshaft reference
            crankshaftMeasure->LoadReference(guiSupport->GuiConfig->GuiParam->ReferenceFileName);
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoReferenceLoadSuccess"),
                                     guiSupport->GuiConfig->GuiParam->ReferenceFileName);
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoReferenceLoadError"),
                                      ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Crankshaft Display Tab Clear Reference Click event.
    /// </summary>
    System::Void FormMain::buttonCrankshaftRefClear_Click(System::Object^  sender, System::EventArgs^  e)
    {
        try
        {
            // clear crankshaft reference
            crankshaftMeasure->ClearReference();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Grapik Control 1 Paint event.
    /// </summary>
    System::Void FormMain::panelGraphAreaCdMp1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
    {
        try
        {
            // refresh display to paint graphic
            crankshaftGraph1->PaintGraph();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Grapik Control 2 Paint event.
    /// </summary>
    System::Void FormMain::panelGraphAreaCdMp2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
    {
        try
        {
            // refresh display to paint graphic
            crankshaftGraph2->PaintGraph();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Grapik Control 3 Paint event.
    /// </summary>
    System::Void FormMain::panelGraphAreaCdMp3_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
    {
        try
        {
            // refresh display to paint graphic
            crankshaftGraph3->PaintGraph();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Grapik Control 4 Paint event.
    /// </summary>
    System::Void FormMain::panelGraphAreaCdMp4_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
    {
        try
        {
            // refresh display to paint graphic
            crankshaftGraph4->PaintGraph();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    /// <summary>
    /// Event handler for the Main Tab Control Paint event.
    /// </summary>
    System::Void FormMain::tabControlMain_DrawItem(System::Object^  sender, System::Windows::Forms::DrawItemEventArgs^  e)
    {
        try
        {
            Color FocusedBackColor    = JH_COLOR_WHITE;
            Color FocusedForeColor    = JH_COLOR_BLACK;
            Color NonFocusedBackColor = JH_COLOR_WHITE;
            Color NonFocusedForeColor = JH_COLOR_BLACK;

            Drawing::Font ^ myFont = gcnew Drawing::Font(e->Font, FontStyle::Regular);
            tabControlMain->TabPages[e->Index]->BackColor = FocusedBackColor;
            String ^ tabName = tabControlMain->TabPages[e->Index]->Text;
            StringFormat ^ myFormat = gcnew StringFormat();
            myFormat->Alignment = StringAlignment::Center;

            SolidBrush ^ backColorBrush;
            SolidBrush ^ foreColorBrush;
            Rectangle  ^ rect;
            RectangleF ^ rectF;

            if (e->Index == tabControlMain->SelectedIndex)
            {
                backColorBrush = gcnew SolidBrush(FocusedBackColor);
                foreColorBrush = gcnew SolidBrush(FocusedForeColor);

                rect = gcnew Rectangle(e->Bounds.X     + 4, e->Bounds.Y,
                                       e->Bounds.Width - 6, e->Bounds.Height);
                rectF = gcnew RectangleF((float)(e->Bounds.X     + 1), (float)(e->Bounds.Y      + 4),
                                         (float)(e->Bounds.Width    ), (float)(e->Bounds.Height - 4));
            }
            else
            {
                backColorBrush = gcnew SolidBrush(NonFocusedBackColor);
                foreColorBrush = gcnew SolidBrush(NonFocusedForeColor);

                rect = gcnew Rectangle(e->Bounds.X     + 1, e->Bounds.Y,
                                       e->Bounds.Width - 1, e->Bounds.Height + 1);
                rectF = gcnew RectangleF((float)(e->Bounds.X    ), (float)(e->Bounds.Y      + 4),
                                         (float)(e->Bounds.Width), (float)(e->Bounds.Height - 4));
            }
            e->Graphics->FillRectangle(backColorBrush, *rect);
            e->Graphics->DrawString(tabName, myFont, foreColorBrush, *rectF, myFormat);

            myFormat->~StringFormat();
            myFont->~Font();
            backColorBrush->~SolidBrush();
            foreColorBrush->~SolidBrush();
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
        }
    }

    #pragma warning(pop)

}
