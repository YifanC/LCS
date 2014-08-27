///
/// \file       FormMain_Support.cpp
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: FormMain_Support.cpp,v 1.3 2011/12/16 13:37:55 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: FormMain_Support.cpp,v $
/// Revision 1.3  2011/12/16 13:37:55  rohn
/// Version 0.9.0.1 (siehe History.txt)
///
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      Main window class support functions.
///

#include "StdAfx.h"
#include "FormMain.h"

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

namespace EIB7xx_App
{
    /// <summary>
    /// Pre component initialization.
    /// This function will be called in the main window constructor
    /// before InitializeComponent() is called.
    /// It is used for localization configuration.
    /// </summary>
	/// <param name="setLanguage">Set language from command line argument.</param>
	/// <param name="setApplication">Set application from command line argument.</param>
	/// <param name="setPassword">Set password from command line argument.</param>
    System::Void FormMain::PreInitializeComponent(String ^ setLanguage,
                                                  String ^ setApplication,
                                                  String ^ setPassword)
    {
        // create resource manger for localized strings
        resManager = GUI_Support::GetResManager();

        /// read GUI configuration parameters from INI file
        guiConfig.ReadConfigParam();

        // set language
        if (   (setLanguage == L"default")
            || (setLanguage == L"en-US"))
        {
            // use default language = en-US
            Thread::CurrentThread->CurrentUICulture = gcnew CultureInfo("");
        }
        else if (setLanguage == L"de-DE")
        {
            // use german language = de-DE
            Thread::CurrentThread->CurrentUICulture = gcnew CultureInfo("de-DE");
        }
        else
        {
            // use windows language
        }

        // set application type
        guiConfig.GuiParam->AppCrankshaft = false;
        if (setApplication == L"crankshaft")
        {
            guiConfig.GuiParam->AppCrankshaft = true;
        }

        // set operation mode
        guiConfig.GuiParam->AdminMode = false;
        if (setPassword == L"diadur")
        {
            guiConfig.GuiParam->AdminMode = true;
        }
    }

    /// <summary>
    /// After component initialization.
    /// This function will be called in the main window constructor
    /// after InitializeComponent() was called.
    /// It is used for special GUI initialization.
    /// </summary>
    System::Void FormMain::PostInitializeComponent()
    {
        // TODO: make some controls invisible
        checkBoxDebug->Visible = false;

        // prepare menu fonts for mark selected menu items
        configMenuFontOriginal  = konfigurationToolStripMenuItem->Font;
        configMenuFontSelected  = gcnew Drawing::Font(configMenuFontOriginal, FontStyle::Bold);
        displayMenuFontOriginal = anzeigeToolStripMenuItem->Font;
        displayMenuFontSelected = gcnew Drawing::Font(displayMenuFontOriginal, FontStyle::Bold);

        // initialize Main Tree View
        String ^ textName = resManager->GetString(L"textDeviceOverview");
        treeViewMain->Nodes->Add(L"", textName, 0, 0);
        treeViewMain->SelectedNode = treeViewMain->Nodes[0];

        // initialize Main Tab Ctrl
        tabControlMain->TabPages->Clear();

        // initialize last tab page values
        lastConfigTab   = nullptr;
        lastPositionTab = nullptr;

        // initialize GUI support
        guiSupport = gcnew GUI_Support(this, resManager, %guiConfig,
                                       richTextBoxReport, treeViewMain,
                                       propertyGridNetwork, propertyGridAxis,
                                       propertyGridTrigger, propertyGridOpMode,
                                       propertyGridCrankshaft,
                                       propertyGridDisplay, dataGridViewDataPacket,
                                       imageListDisplayLed,
                                       buttonNetworkWrite,
                                       progressBarFillLevel,
                                       checkBoxDebug);

        // initialize EIB list
        eibTreeList = gcnew EIB_TreeList(guiSupport);
        eibInitList = gcnew EIB_InitList(guiSupport, eibTreeList);
        eibSaveList = gcnew EIB_SaveList(guiSupport);

        // initialize Network Tab
        propertyGridNetwork->SelectedObject = nullptr;
        // initialize Trigger Tab
        propertyGridTrigger->SelectedObject = nullptr;
        // initialize Axis Tab
        propertyGridAxis->SelectedObject = nullptr;
        // initialize Data Packet Parameter Tab
        propertyDataPacket = gcnew PropertyDataPacket(guiSupport, eibTreeList);
        // initialize OperationMode Tab
        propertyOpMode = gcnew PropertyOpMode(guiSupport, eibTreeList);
        // initialize crankshaft configuration Tab
        propertyCrankshaft = gcnew PropertyCrankshaft(guiSupport, eibTreeList);
        // initialize Display Options Tab
        propertyDisplay = gcnew PropertyDisplay(guiSupport, eibTreeList);
        // initialize Position Data Axis 0 Tab
        displayPosition0 = gcnew DisplayPosition(guiSupport, eibTreeList,
                                                 textBoxPosAxis0,
                                                 labelUnitAxis0,
                                                 pictureBoxStatusAxis0,
                                                 pictureBoxRefAxis0,
                                                 pictureBoxTriggerAxis0,
                                                 buttonRefAxis0,
                                                 buttonClearAxis0,
                                                 0);
        // initialize Position Data Axis 1 Tab
        displayPosition1 = gcnew DisplayPosition(guiSupport, eibTreeList,
                                                 textBoxPosAxis1,
                                                 labelUnitAxis1,
                                                 pictureBoxStatusAxis1,
                                                 pictureBoxRefAxis1,
                                                 pictureBoxTriggerAxis1,
                                                 buttonRefAxis1,
                                                 buttonClearAxis1,
                                                 1);
        // initialize Position Data Axis 2 Tab
        displayPosition2 = gcnew DisplayPosition(guiSupport, eibTreeList,
                                                 textBoxPosAxis2,
                                                 labelUnitAxis2,
                                                 pictureBoxStatusAxis2,
                                                 pictureBoxRefAxis2,
                                                 pictureBoxTriggerAxis2,
                                                 buttonRefAxis2,
                                                 buttonClearAxis2,
                                                 2);
        // initialize Position Data Axis 3 Tab
        displayPosition3 = gcnew DisplayPosition(guiSupport, eibTreeList,
                                                 textBoxPosAxis3,
                                                 labelUnitAxis3,
                                                 pictureBoxStatusAxis3,
                                                 pictureBoxRefAxis3,
                                                 pictureBoxTriggerAxis3,
                                                 buttonRefAxis3,
                                                 buttonClearAxis3,
                                                 3);
        // initialize Position Data Axis 4 Tab
        displayPosition4 = gcnew DisplayPosition(guiSupport, eibTreeList,
                                                 textBoxPosAxis4,
                                                 labelUnitAxis4,
                                                 pictureBoxStatusAxis4,
                                                 pictureBoxRefAxis4,
                                                 pictureBoxTriggerAxis4,
                                                 buttonRefAxis4,
                                                 buttonClearAxis4,
                                                 4);
        // initialize Data Packet Data Tab
        dataGridViewDetailDataPacket->AllowUserToAddRows = false;
        dataGridViewDetailStatus->AllowUserToAddRows     = false;
        displayDataPacket = gcnew DisplayDataPacket(guiSupport, eibTreeList,
                                                    dataGridViewDetailDataPacket,
                                                    dataGridViewDetailStatus);
        // initialize Crankshaft Display Tab
        crankshaftGraph1 = gcnew PolarGraph(guiSupport,
                                            panelGraphAreaCdMp1,
                                            labelXminCdMp1, labelXmaxCdMp1,
                                            labelYminCdMp1, labelYmaxCdMp1,
                                            0,
                                            eibTreeList->Common->CrankshaftParam->GraphAxisMax);
        crankshaftGraph2 = gcnew PolarGraph(guiSupport,
                                            panelGraphAreaCdMp2,
                                            labelXminCdMp2, labelXmaxCdMp2,
                                            labelYminCdMp2, labelYmaxCdMp2,
                                            1,
                                            eibTreeList->Common->CrankshaftParam->GraphAxisMax);
        crankshaftGraph3 = gcnew PolarGraph(guiSupport,
                                            panelGraphAreaCdMp3,
                                            labelXminCdMp3, labelXmaxCdMp3,
                                            labelYminCdMp3, labelYmaxCdMp3,
                                            2,
                                            eibTreeList->Common->CrankshaftParam->GraphAxisMax);
        crankshaftGraph4 = gcnew PolarGraph(guiSupport,
                                            panelGraphAreaCdMp4,
                                            labelXminCdMp4, labelXmaxCdMp4,
                                            labelYminCdMp4, labelYmaxCdMp4,
                                            3,
                                            eibTreeList->Common->CrankshaftParam->GraphAxisMax);
        crankshaftDisplay = gcnew Crankshaft_Display(guiSupport,
                                                     eibTreeList->Common->CrankshaftParam,
                                                     checkBoxStopAt360,
                                                     buttonCrankshaftRefSave,
                                                     buttonCrankshaftRefLoad,
                                                     buttonCrankshaftRefClear,
                                                     crankshaftGraph1,
                                                     crankshaftGraph2,
                                                     crankshaftGraph3,
                                                     crankshaftGraph4);
        crankshaftMeasure = gcnew Crankshaft_Measure(guiSupport,
                                                     eibTreeList->Common->CrankshaftParam,
                                                     crankshaftDisplay);
        eibTreeList->Common->CrankshaftMeasure = crankshaftMeasure;

        // initialize the GUI state mangement
        InitializeGuiState();
        // trigger a new GUI state
        TriggerGuiState(GUI_StateTrigger::InitSystem);

        // set project state
        SetApplicationTitle(ProjectState::NoProject);

        // initialize recording measurement state
        recordingState = RecordingState::Init;
        SetRecordingState(RecordingTrigger::Init);

        // initialize display timer
        timerDisplay->Interval = EIB_DISPLAY_TIMER_PERIOD;
        timerDisplay->Enabled  = true;

        // initialize measure data file
        dataFileName = nullptr;

        // initialize event for GUI command thread end
        guiCmdEnd = gcnew AutoResetEvent(false);
    }

    /// <summary>
    /// Check if system is closing.
    /// </summary>
    /// <returns>True if the system is closing.</returns>
    bool FormMain::IsClosing()
    {
        return isClosing;
    }

    /// <summary>
    /// GUI command function.
    /// </summary>
	/// <param name="guiCommand">The GUI command to process.</param>
	/// <param name="maxTime">The GUI command max time.</param>
    System::Void FormMain::GuiCmdFunction(GuiCommand guiCommand, UInt32 maxTime)
    {
        // initialize GUI command
        this->guiCommand = guiCommand;
        // initialize GUI command error
        guiCmdError = nullptr;
        // initialize GUI command thread
        Thread ^ guiCmdThread = gcnew Thread(
                                gcnew ThreadStart(this,
                                                  &EIB7xx_App::FormMain::GuiCmdThread));
        // start GUI command thread
        guiCmdThread->Start();
        // wait for thread started
        Thread::Sleep(100);
        // wait for thread end
        bool signal;
        UInt32 timeout =  1000;
        UInt32 progress;
        for (UInt32 count = 0; ; count ++)
        {
            // check for thread stopped
            signal = guiCmdEnd->WaitOne(timeout, false);
            if (signal == true)
            {
                // check GUI command error
                if (guiCmdError != nullptr)
                {
                    throw guiCmdError;
                }
                // thread is ready
                toolStripProgressBarStatus->Value = 0;
                break;
            }
            // thread is running
            // show progress
            if (maxTime > 0)
            {
                progress = (count * timeout * 100) / maxTime;
                if (progress >= 100)
                {
                    count = 0;
                    progress = 100;
                }
                toolStripProgressBarStatus->Value = progress;
            }
        }
    }

    /// <summary>
    /// GUI command thread.
    /// </summary>
    System::Void FormMain::GuiCmdThread()
    {
        try
        {
            switch (guiCommand)
            {
            default: throw gcnew Exception(L"Internal error 26"); break;
            case GuiCommand::ResetEib:
                // reset EIB
                eibTreeList->SelectedEIB->ResetEib();
                break;
            case GuiCommand::UpdateEib:
                // update EIB firmware
                eibTreeList->SelectedEIB->UpdateEib(guiSupport->GuiConfig->GuiParam->FirmwareFileName);
                break;
            case GuiCommand::SaveRecordingData:
                // save recording data
                eibTreeList->SaveRecordingData(dataFileName);
                break;
            }
        }
        catch (Exception ^ ex)
        {
            // end thread due to an error
            guiCmdError = ex;
            return;
        }
        finally
        {
            // set end event
            guiCmdEnd->Set();
        }
    }

    /// <summary>
    /// Refresh EIB parameter inputs.
    /// </summary>
    System::Void FormMain::RefreshParam()
    {
        TabPage ^ selectedTab = tabControlMain->SelectedTab;
        //String ^ debug = String::Format(L"SelectedTab: {0}", selectedTab);
        //Debug::Print(debug);
        // check selected tap page
        bool refreshAll = false;
        if (selectedTab == nullptr)
        {
            refreshAll = true;
        }
        if (   (selectedTab == tabPageNetwork)
            || (refreshAll == true))
        {
            // refresh Network Tab
            eibTreeList->SelectedEIB->ThePropertyNetwork->RefreshParam();
        }
        else if (   (selectedTab == tabPageAxis)
                 || (refreshAll == true))
        {
            // refresh Axis Tab
            eibTreeList->SelectedEIB->RefreshAxisParam(eibTreeList->AxisIndex, eibTreeList->IsAuxAxis);
        }
        else if (   (selectedTab == tabPageDataPacket)
                 || (refreshAll == true))
        {
            // refresh Data Packet Tab
            propertyDataPacket->RefreshParam();
        }
        else if (   (selectedTab == tabPageTrigger)
                 || (refreshAll == true))
        {
            // refresh Trigger Tab
            eibTreeList->SelectedEIB->ThePropertyTrigger->RefreshParam();
        }
        else if (   (selectedTab == tabPageOperationMode)
                 || (refreshAll == true))
        {
            // refresh Operation Mode Tab
            propertyOpMode->RefreshParam();
        }
        else if (   (selectedTab == tabPageCrankshaftConfig)
                 || (refreshAll == true))
        {
            // refresh Crankshaft Configuration Tab
            propertyCrankshaft->RefreshParam();
        }
        else if (   (selectedTab == tabPageSummary)
                 || (refreshAll == true))
        {
            // refresh summary display tabs
            RefreshDisplay();
        }
        else if (   (selectedTab == tabPageDetail)
                 || (refreshAll == true))
        {
            // refresh detail display tabs
            RefreshDisplay();
        }
        else if (   (selectedTab == tabPageCrankshaftDisplay)
                 || (refreshAll == true))
        {
            // refresh crankshaft display tabs
            RefreshDisplay();
        }
        else if (   (selectedTab == tabPageOptions)
                 || (refreshAll == true))
        {
            // refresh Display Options Tab
            propertyDisplay->RefreshParam();
        }
    }

    /// <summary>
    /// Refresh the display tabs.
    /// </summary>
    System::Void FormMain::RefreshDisplay()
    {
        RefreshDisplay(0, true);
    }

    /// <summary>
    /// Refresh the display tabs.
    /// </summary>
	/// <param name="readData">If true read polling data.</param>
    System::Void FormMain::RefreshDisplay(bool readData)
    {
        RefreshDisplay(0, readData);
    }

    /// <summary>
    /// Refresh the display tabs.
    /// </summary>
	/// <param name="displayMode">The display mode flags.</param>
    System::Void FormMain::RefreshDisplay(UInt32 displayMode)
    {
        RefreshDisplay(displayMode, true);
    }

    /// <summary>
    /// Refresh the display tabs.
    /// </summary>
	/// <param name="displayMode">The display mode flags.</param>
	/// <param name="readData">If true read polling data.</param>
    System::Void FormMain::RefreshDisplay(UInt32 displayMode, bool readData)
    {
        // check if EIB is selected
        if (eibTreeList->SelectedEIB == nullptr)
        {
            return;
        }
        try
        {
            EIB_Param ^ parameter = eibTreeList->SelectedEIB->Parameter;
            // check initialisation state
            if (eibInitList->IsEqual(eibTreeList) == true)
            {
                displayMode |= DISPLAY_MODE_INIT_OK;
            }
            // check for running state
            if (   CheckGuiState(gcnew GUI_State(GUI_StateTree::All,
                                                 GUI_StateTab::All,
                                                 GUI_StateOpMode::Run,
                                                 GUI_StateMenu::All))
                == true)
            {
                displayMode |= DISPLAY_MODE_RUNNING;
                // check for polling mode
                if (eibTreeList->Common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_Polling)
                {
                    displayMode |= DISPLAY_MODE_POLLING;
                }
            }
            else if (readData == true)
            {
                // system is not running - read polling data
                if (   (stateGuard != GUI_StateTrigger::RunEIB)
                    && (guiState.OpModeState != GUI_StateOpMode::Run)
                    && ((displayMode & DISPLAY_MODE_INIT_OK)   != 0)
                    && ((displayMode & DISPLAY_MODE_RECORDING) == 0)
                    && (eibInitList->SelectedEibParam != nullptr))
                {
                    parameter = eibInitList->SelectedEibParam;
                    eibTreeList->SelectedEIB->ReadPollingData(0, parameter, false);
                }
            }
            // check selected tap page
            bool refreshAll = false;
            TabPage ^ selectedTab = tabControlMain->SelectedTab;
            if (selectedTab == nullptr)
            {
                refreshAll = true;
            }
            if (   (selectedTab == tabPageSummary)
                || (refreshAll == true))
            {
                // refresh summary display tabs
                displayPosition1->RefreshDisplay(parameter, displayMode);
                displayPosition2->RefreshDisplay(parameter, displayMode);
                displayPosition3->RefreshDisplay(parameter, displayMode);
                displayPosition4->RefreshDisplay(parameter, displayMode);
            }
            else if (   (selectedTab == tabPageDetail)
                     || (refreshAll == true))
            {
                // refresh detail display tabs
                displayPosition0->RefreshDisplay(parameter, displayMode);
                displayDataPacket->RefreshDisplay(parameter, displayMode);
            }
            else if (   (selectedTab == tabPageCrankshaftDisplay)
                     || (refreshAll == true))
            {
                // refresh crankshaft display tabs
                crankshaftDisplay->RefreshDisplay();
            }
        }
        finally
        {
        }
    }

    /// <summary>
    /// Clear the display tabs.
    /// </summary>
    System::Void FormMain::ClearDisplay()
    {
        // clear display tabs
        displayPosition0->ClearDisplay();
        displayPosition1->ClearDisplay();
        displayPosition2->ClearDisplay();
        displayPosition3->ClearDisplay();
        displayPosition4->ClearDisplay();
        displayDataPacket->ClearDisplay();
    }

    /// <summary>
    /// Manage the blinking controls.
    /// </summary>
    System::Void FormMain::BlinkDisplay()
    {
        // blink the display tab blink controls
        displayPosition0->BlinkDisplay();
        displayPosition1->BlinkDisplay();
        displayPosition2->BlinkDisplay();
        displayPosition3->BlinkDisplay();
        displayPosition4->BlinkDisplay();
    }

    /// <summary>
    /// Read EIB parameters.
    /// </summary>
    System::Void FormMain::ReadEIBParameters()
    {
        try
        {
            // select project file
            OpenFileDialog ^ fileDialog = gcnew OpenFileDialog;
            fileDialog->Title = resManager->GetString(L"dialogProjectFile");
            fileDialog->InitialDirectory = guiSupport->GuiConfig->GuiParam->ParamFileName;
            fileDialog->Filter = String::Format(EIB_FILE_FILTER,
                                                PARAM_FILE_EXT,
                                                resManager->GetString(L"textProjectFile"),
                                                resManager->GetString(L"textAllFiles"));
            fileDialog->FilterIndex = 1;
            fileDialog->DefaultExt = PARAM_FILE_EXT;
            fileDialog->AddExtension = true;
            fileDialog->Multiselect = false;
            fileDialog->CheckFileExists = true;
            Windows::Forms::DialogResult result = fileDialog->ShowDialog();
            if (result != ::DialogResult::OK)
            {
                return;
            }
            guiSupport->GuiConfig->GuiParam->ParamFileName = fileDialog->FileName;
            // set state guard flag
            stateGuard = GUI_StateTrigger::OpenProject;
            // add the EIBs from a project file to the system
            eibTreeList->AddEIBsFromProject(guiSupport->GuiConfig->GuiParam->ParamFileName);
            // set save list
            EIBParameterToProject();
            // clear init list
            eibInitList->Clear();
            // trigger a new GUI state
            TriggerGuiState(GUI_StateTrigger::OpenProject);
            // initialize recording measurement state
            SetRecordingState(RecordingTrigger::Init);
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoEibLoadSuccess"),
                                     guiSupport->GuiConfig->GuiParam->ParamFileName);
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoEibLoadError"),
                                      ex->Message);
            // reset state guard flag
            stateGuard = GUI_StateTrigger::NotUsed;
        }
    }

    /// <summary>
    /// Save EIB parameters.
    /// </summary>
	/// <param name="saveAs">If true then use file save dialog.</param>
    /// <returns>True if saving parameters was canceled.</returns>
    bool FormMain::SaveEIBParameters(bool saveAs)
    {
        try
        {
            // select project file
            if (   (saveAs == true)
                || (eibSaveList->EibCount == 0))
            {
                SaveFileDialog ^ fileDialog = gcnew SaveFileDialog;
                fileDialog->Title = resManager->GetString(L"dialogProjectFile");
                fileDialog->InitialDirectory = guiSupport->GuiConfig->GuiParam->ParamFileName;
                fileDialog->Filter = String::Format(EIB_FILE_FILTER,
                                                    PARAM_FILE_EXT,
                                                    resManager->GetString(L"textProjectFile"),
                                                    resManager->GetString(L"textAllFiles"));
                fileDialog->FilterIndex = 1;
                fileDialog->DefaultExt = PARAM_FILE_EXT;
                fileDialog->AddExtension = true;
                fileDialog->CheckFileExists = false;
                fileDialog->OverwritePrompt = true;
                Windows::Forms::DialogResult result = fileDialog->ShowDialog();
                if (result != ::DialogResult::OK)
                {
                    return true;
                }
                guiSupport->GuiConfig->GuiParam->ParamFileName = fileDialog->FileName;
            }
            // save project
            eibTreeList->SaveEIBParameters(guiSupport->GuiConfig->GuiParam->ParamFileName);
            // set save list
            EIBParameterToProject();
            // trigger a new GUI state
            TriggerGuiState(GUI_StateTrigger::SaveProject);
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoEibSaveSuccess"),
                                     guiSupport->GuiConfig->GuiParam->ParamFileName);
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoEibSaveError"),
                                      ex->Message);
        }
        return false;
    }

    /// <summary>
    /// Check if the EIB parameters had been changed for saving.
    /// </summary>
    /// <returns>True if EIB parameters had been changed for saving.</returns>
    bool FormMain::CheckEIBParametersChanged()
    {
        if (eibSaveList->IsEqual(eibTreeList) == false)
        {
            return true;
        }
        return false;
    }

    /// <summary>
    /// This function will be called if the EIB parameters are equal
    /// to a project file.
    /// </summary>
    System::Void FormMain::EIBParameterToProject()
    {
        // set save list
        eibSaveList->Copy(eibTreeList);
        // set project state
        SetApplicationTitle(ProjectState::ProjectSet);
    }

    /// <summary>
    /// This function will be called if an EIB parameter was chanched
    /// in a property grid.
    /// </summary>
    System::Void FormMain::EIBParameterChanged()
    {
        // set project state
        SetApplicationTitle(ProjectState::ProjectChanged);
    }

    /// <summary>
    /// Set application title depending from project state.
    /// </summary>
	/// <param name="projectState">The project state.</param>
    System::Void FormMain::SetApplicationTitle(ProjectState projectState)
    {
        #define APPLICATION_NAME            L"EIB 7xx Application"
        #define FORMAT_PROJECT_SET          APPLICATION_NAME + L" ({0})"
        #define FORMAT_PROJECT_CHANGED      APPLICATION_NAME + L" ({0} *)"

        String ^ fileName;
        if (eibSaveList->EibCount == 0)
        {
            // project was not loaded or saved jet
            projectState = ProjectState::NoProject;
        }
        else
        {
            // get filename from pathname
            Int32 index = guiSupport->GuiConfig->GuiParam->ParamFileName->LastIndexOf(L"\\");
            if (index >= 0)
            {
                fileName = guiSupport->GuiConfig->GuiParam->ParamFileName->Substring(index + 1);
            }
            else
            {
                fileName = guiSupport->GuiConfig->GuiParam->ParamFileName;
            }
        }
        // set application title with project name
        switch (projectState)
        {
        default: throw gcnew Exception(L"Internal error 11"); break;
        case ProjectState::NoProject:
            this->Text = APPLICATION_NAME;
            break;
        case ProjectState::ProjectSet:
            this->Text = String::Format(FORMAT_PROJECT_SET, fileName);
            break;
        case ProjectState::ProjectChanged:
            if (CheckEIBParametersChanged() == true)
            {
                this->Text = String::Format(FORMAT_PROJECT_CHANGED, fileName);
            }
            else
            {
                this->Text = String::Format(FORMAT_PROJECT_SET, fileName);
            }
            break;
        }
    }

    /// <summary>
    /// Initialize EIB(s).
    /// </summary>
    System::Void FormMain::InitializeEIBs()
    {
        // initialize EIB(s)
        eibTreeList->InitializeEIBs();
        // set save list
        eibInitList->Copy(eibTreeList);
        // trigger a new GUI state
        TriggerGuiState(GUI_StateTrigger::InitEIB);
        // refresh display tabs
        RefreshDisplay();
    }

    /// <summary>
    /// Select measure data file.
    /// </summary>
    System::Void FormMain::SelectDataFile()
    {
        dataFileName = nullptr;
        SaveFileDialog ^ fileDialog = gcnew SaveFileDialog;
        fileDialog->Title = resManager->GetString(L"dialogDataFile");
        fileDialog->InitialDirectory = guiSupport->GuiConfig->GuiParam->DataFileName;
        fileDialog->Filter = String::Format(EIB_FILE_FILTER,
                                            DATA_FILE_EXT,
                                            resManager->GetString(L"textDataFile"),
                                            resManager->GetString(L"textAllFiles"));
        fileDialog->FilterIndex = 1;
        fileDialog->DefaultExt = DATA_FILE_EXT;
        fileDialog->AddExtension = true;
        fileDialog->CheckFileExists = false;
        fileDialog->OverwritePrompt = true;
        Windows::Forms::DialogResult result = fileDialog->ShowDialog();
        if (result == ::DialogResult::OK)
        {
            guiSupport->GuiConfig->GuiParam->DataFileName = fileDialog->FileName;
            dataFileName = fileDialog->FileName;
        }
    }

    /// <summary>
    /// Start EIB measurement.
    /// </summary>
    System::Void FormMain::StartEIBMeasurement()
    {
        // run EIBs
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            bool initEib = false;
            // check if EIB parameters had been changed
            if (eibInitList->IsEqual(eibTreeList) == false)
            {
                // EIB parameters had been changed
                Windows::Forms::DialogResult result =
                            guiSupport->MsgBoxError(resManager->GetString(L"errorInitChanged"),
                                                    MessageBoxButtons::OKCancel,
                                                    MessageBoxDefaultButton::Button1);
                if (result == ::DialogResult::Cancel)
                {
                    // cancel run measurement
                    return;
                }
                // initialize EIB(s)
                initEib = true;
            }
            // check if connection to EIB has been closed
            else if (eibTreeList->CheckEibOpen() == false)
            {
                // EIB parameters had been changed
                Windows::Forms::DialogResult result =
                            guiSupport->MsgBoxError(resManager->GetString(L"errorEibClosed"),
                                                    MessageBoxButtons::OKCancel,
                                                    MessageBoxDefaultButton::Button1);
                if (result == ::DialogResult::Cancel)
                {
                    // cancel run measurement
                    return;
                }
                // initialize EIB(s)
                initEib = true;
            }
            // check for EIB initialization
            if (initEib == true)
            {
                // initialize EIB(s)
                this->Cursor = Cursors::WaitCursor;
                try
                {
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
                    guiSupport->ShowErrorText(resManager->GetString(L"infoEibMeasureAbort"));
                    // cancel run measurement
                    return;
                }
            }
            // check for saving measure data
            dataFileName = nullptr;
            if (   (guiSupport->GuiConfig->GuiParam->SaveMeasureData == true)
                && (   (eibTreeList->Common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_Polling)
                    || (eibTreeList->Common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_SoftRealtime)
                    || (eibTreeList->Common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_Streaming)))
            {
                // select measure data file
                SelectDataFile();
            }
            // set state guard flag
            stateGuard = GUI_StateTrigger::RunEIB;
            // set wait cursor
            this->Cursor = Cursors::WaitCursor;
            // start crankshaft measurement
            crankshaftMeasure->StartMeasurement(eibTreeList->Common->OpMode);
            // run EIBs
            eibTreeList->StartEIBMeasurement(dataFileName);
            // clear display tabs
            ClearDisplay();
            // initialize display period conter
            displayPeriod = 0; // eibTreeList->Common->Display->DisplayPeriod;
            // save current tab page
            TabPage ^ newTabPage = nullptr;
            if (guiState.MenuState == GUI_StateMenu::Config)
            {
                lastConfigTab = tabControlMain->SelectedTab;
                newTabPage = lastPositionTab;
            }
            // trigger a new GUI state
            TriggerGuiState(GUI_StateTrigger::RunEIB, newTabPage);
            // set recording measurement state to start
            SetRecordingState(RecordingTrigger::Start);
            // refresh display tabs
            RefreshDisplay();
            // report success
            guiSupport->ShowInfoText(resManager->GetString(L"infoEibStartSuccess"));
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoEibStartError"),
                                      ex->Message);
            // stop running state
            StopEIBMeasurement(true);
            // reset state guard flag
            stateGuard = GUI_StateTrigger::NotUsed;
        }
        finally
        {
            this->Cursor = cursor;
        }
    }

    /// <summary>
    /// Stop EIB measurement.
    /// </summary>
	/// <param name="stopOnError">True if the measurement was stopped due to an error.</param>
    System::Void FormMain::StopEIBMeasurement(bool stopOnError)
    {
        // stop EIBs
        System::Windows::Forms::Cursor ^ cursor = this->Cursor;
        try
        {
            this->Cursor = Cursors::WaitCursor;
            // set state guard flag
            stateGuard = GUI_StateTrigger::StopEIB;
            // stop EIBs
            eibTreeList->StopEIBMeasurement();
            // trigger a new GUI state
            TriggerGuiState(GUI_StateTrigger::StopEIB);
            // set recording measurement state to stop
            SetRecordingState(RecordingTrigger::Stop);
            // refresh display tabs
            RefreshDisplay();
            // stop crankshaft measurement
            crankshaftMeasure->StopMeasurement();
            // report success
            if (stopOnError == false)
            {
                guiSupport->ShowInfoText(resManager->GetString(L"infoEibStopSuccess"));
                if (dataFileName != nullptr)
                {
                    guiSupport->ShowInfoText(resManager->GetString(L"infoDataSaveSuccess"),
                                             dataFileName);
                }
            }
        }
        catch (Exception ^ ex)
        {
            guiSupport->MsgBoxErrorOk(ex->Message);
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoEibStopError"),
                                      ex->Message);
            // reset state guard flag
            stateGuard = GUI_StateTrigger::NotUsed;
        }
        finally
        {
            this->Cursor = cursor;
        }
    }

    /// <summary>
    /// Run EIB measurement.
    /// </summary>
    System::Void FormMain::RunEIBMeasurement()
    {
        // check for running state
        bool isRunning = CheckGuiState(gcnew GUI_State(GUI_StateTree::All,
                                                       GUI_StateTab::All,
                                                       GUI_StateOpMode::Run,
                                                       GUI_StateMenu::All));
        if (   (isRunning == false)
            && (recordDisplayRun == false))
        {
            // return if isn't running state
            return;
        }
        try
        {
            // show crankshaft measure data
            crankshaftMeasure->ShowMeasureData();
            // check for stop measurement
            bool stopFlag = eibTreeList->CheckForStopMeasurement();
            if (stopFlag == true)
            {
                // stop running state
                StopEIBMeasurement(false);
            }
            // check display period
            displayPeriod -= EIB_DISPLAY_TIMER_PERIOD;
            if (displayPeriod > 0)
            {
                // return if display period has not reached
                return;
            }
            // reset display period counter
            displayPeriod = eibTreeList->Common->Display->DisplayPeriod;
            // read measure data for polling for all EIBs
            if (eibTreeList->Common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_Polling)
            {
                stopFlag = eibTreeList->ReadPollingData(true);
                if (stopFlag == true)
                {
                    // stop running state
                    StopEIBMeasurement(false);
                }
            }
            // perform measurement operations
            if (eibTreeList->SelectedEIB != nullptr)
            {
                if (isRunning == true)
                {
                    // measurement is running
                    switch (eibTreeList->Common->OpMode->OpMode)
                    {
                    default: throw gcnew Exception(L"Internal error 14"); break;
                    case EIB7_MODE_OPERATING::EIB7_OM_SoftRealtime:
                    case EIB7_MODE_OPERATING::EIB7_OM_Streaming:
                        // read measure data for soft realtime or streaming
                        eibTreeList->SelectedEIB->ReadSoftRtStreamingData();
                    case EIB7_MODE_OPERATING::EIB7_OM_Polling:
                        // refresh display tabs for polling, soft realtime or streaming
                        RefreshDisplay();
                        break;
                    case EIB7_MODE_OPERATING::EIB7_OM_RecordingSingle:
                    case EIB7_MODE_OPERATING::EIB7_OM_RecordingRoll:
                        // read recording buffer state
                        UInt32 fillLevel, entryNumber;
                        bool firstEibReady, allEibReady;
                        fillLevel = eibTreeList->ReadRecordingState(entryNumber, firstEibReady, allEibReady);
                        // show recording buffer state in progress bar
                        progressBarFillLevel->Value = fillLevel;
                        //String ^ debug = String::Format(L"Record count: {0}", entryNumber);
                        //Debug::Print(debug);
                        // set read data counter
                        eibTreeList->SelectedEIB->DisplayBuffer->ReadDataCount = entryNumber;
                        // refresh display tabs
                        RefreshDisplay(DISPLAY_MODE_RECORDING);
                        // check for stop in recording single mode
                        bool stopOnFirstEibReady = true;
                        if (   (   eibTreeList->Common->OpMode->OpMode
                                == EIB7_MODE_OPERATING::EIB7_OM_RecordingSingle)
                            && (   (   (stopOnFirstEibReady == true)
                                    && (firstEibReady == true))
                                || (   (stopOnFirstEibReady == false)
                                    && (allEibReady == true))))
                        {
                            // stop running state
                            StopEIBMeasurement(false);
                        }
                        break;
                    }
                }
                if (recordDisplayRun == true)
                {
                    // show recording measurement values automaticly
                    UInt32 dataIndex = lastRecordIndex + 1;
                    if (dataIndex > numericUpDownIndex->Maximum)
                    {
                        dataIndex = 0;
                    }
                    // refresh display of recording measurement values 
                    RefreshRecordingDisplay(dataIndex);
                }
            }
        }
        catch (Exception ^ ex)
        {
            // stop timer
            timerDisplay->Enabled = false;
            // show error
            guiSupport->MsgBoxErrorOk(ex->Message);

            if (isRunning == true)
            {
                // report error
                guiSupport->ShowErrorText(resManager->GetString(L"infoEibMeasureError"),
                                          ex->Message);
                guiSupport->ShowErrorText(resManager->GetString(L"infoEibMeasureAbort"));
                // stop running state
                StopEIBMeasurement(true);
            }
            if (recordDisplayRun == true)
            {
                // disable recording display run
                SetRecordingDisplayRun(false);
            }
            // restart timer
            timerDisplay->Enabled = true;
        }
    }

    /// <summary>
    /// Set recording measurement state.
    /// </summary>
	/// <param name="recordingTrigger">The recording measurement state trigger.</param>
    System::Void FormMain::SetRecordingState(RecordingTrigger recordingTrigger)
    {
        // save current state
        RecordingState oldState = recordingState;
        // set recording state depending from trigger
        switch (recordingTrigger)
        {
        default: throw gcnew Exception(L"Internal error 12"); break;
        case RecordingTrigger::TabPage:
            break;
        case RecordingTrigger::Init:
            // set new recording state
            recordingState = RecordingState::Empty;
            break;
        case RecordingTrigger::Start:
            if (   (eibTreeList->Common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_RecordingSingle)
                || (eibTreeList->Common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_RecordingRoll))
            {
                // set new recording state
                recordingState = RecordingState::Measurement;
                // initialize recording buffer state display
                labelFillLevel->Text = resManager->GetString(L"textFillLevelMeasure");
                progressBarFillLevel->Value = 0;
            }
            else
            {
                // set new recording state
                recordingState = RecordingState::Empty;
            }
            break;
        case RecordingTrigger::Stop:
            if (recordingState == RecordingState::Measurement)
            {
                // set new recording state
                recordingState = RecordingState::Display;
                // initialize display of recording measurement values 
                RefreshRecordingDisplay(0);
            }
            break;
        case RecordingTrigger::Save:
            if (recordingState == RecordingState::Display)
            {
                // set new recording state
                recordingState = RecordingState::Measurement;
                // initialize recording buffer state display
                labelFillLevel->Text = resManager->GetString(L"textFillLevelSave");
                progressBarFillLevel->Value = 0;
            }
            break;
        }
        // set display dialogs depending from recording state
        if (   (recordingState != oldState)
            || (recordingTrigger == RecordingTrigger::TabPage))
        {
            switch (recordingState)
            {
            default: throw gcnew Exception(L"Internal error 13"); break;
            case RecordingState::Empty:
                // remove all recording dialogs from display dialogs
                if (tableLayoutPanelSummary->Controls->Contains(panelRecordProgress) == true)
                {
                    tableLayoutPanelSummary->Controls->Remove(panelRecordProgress);
                }
                if (tableLayoutPanelSummary->Controls->Contains(panelRecordIndex) == true)
                {
                    tableLayoutPanelSummary->Controls->Remove(panelRecordIndex);
                }
                if (tableLayoutPanelDetail->Controls->Contains(panelRecordProgress) == true)
                {
                    tableLayoutPanelDetail->Controls->Remove(panelRecordProgress);
                }
                if (tableLayoutPanelDetail->Controls->Contains(panelRecordIndex) == true)
                {
                    tableLayoutPanelDetail->Controls->Remove(panelRecordIndex);
                }
                // disable recording display run
                SetRecordingDisplayRun(false);
                break;
            case RecordingState::Measurement:
                // add recording progress dialog to current display dialog
                if (tabControlMain->SelectedTab == tabPageSummary)
                {
                    if (tableLayoutPanelSummary->Controls->Contains(panelRecordIndex) == true)
                    {
                        tableLayoutPanelSummary->Controls->Remove(panelRecordIndex);
                    }
                    if (tableLayoutPanelSummary->Controls->Contains(panelRecordProgress) == false)
                    {
                        tableLayoutPanelSummary->Controls->Add(panelRecordProgress, 1, 3);
                    }
                }
                if (tabControlMain->SelectedTab == tabPageDetail)
                {
                    if (tableLayoutPanelDetail->Controls->Contains(panelRecordIndex) == true)
                    {
                        tableLayoutPanelDetail->Controls->Remove(panelRecordIndex);
                    }
                    if (tableLayoutPanelDetail->Controls->Contains(panelRecordProgress) == false)
                    {
                        tableLayoutPanelDetail->Controls->Add(panelRecordProgress, 1, 3);
                    }
                }
                // disable recording display run
                SetRecordingDisplayRun(false);
                break;
            case RecordingState::Display:
                // add recording index dialog to current display dialog
                if (tabControlMain->SelectedTab == tabPageSummary)
                {
                    if (tableLayoutPanelSummary->Controls->Contains(panelRecordProgress) == true)
                    {
                        tableLayoutPanelSummary->Controls->Remove(panelRecordProgress);
                    }
                    if (tableLayoutPanelSummary->Controls->Contains(panelRecordIndex) == false)
                    {
                        tableLayoutPanelSummary->Controls->Add(panelRecordIndex, 1, 3);
                    }
                }
                else if (tabControlMain->SelectedTab == tabPageDetail)
                {
                    if (tableLayoutPanelDetail->Controls->Contains(panelRecordProgress) == true)
                    {
                        tableLayoutPanelDetail->Controls->Remove(panelRecordProgress);
                    }
                    if (tableLayoutPanelDetail->Controls->Contains(panelRecordIndex) == false)
                    {
                        tableLayoutPanelDetail->Controls->Add(panelRecordIndex, 1, 3);
                    }
                }
                else
                {
                    // disable recording display run
                    SetRecordingDisplayRun(false);
                }
                break;
            }
        }
        //String ^ debug = String::Format(L"Recording state: {0}", recordingState);
        //Debug::Print(debug);
    }

    /// <summary>
    /// Refresh display of recording measurement values.
    /// </summary>
	/// <param name="dataIndex">The index of the measurement value in recording buffer.</param>
    System::Void FormMain::RefreshRecordingDisplay(UInt32 dataIndex)
    {
        if (   (eibTreeList->SelectedEIB == nullptr)
            || (recordingState != RecordingState::Display)
            || (   (dataIndex > 0)
                && (dataIndex == lastRecordIndex)))
        {
            return;
        }
        // get number of measured values
        UInt32 entryNumber;
        eibTreeList->SelectedEIB->ReadRecordingState(entryNumber);
        // initialize display controls
        if (dataIndex == 0)
        {
            UInt32 maxValue = entryNumber;
            if (entryNumber > 0)
            {
                maxValue --;
            }
            // set index selection max values
            numericUpDownIndex->Maximum = maxValue;
            trackBarIndex->Maximum      = maxValue;
            labelMaxIndex->Text = String::Format(L"max: {0}", maxValue);
            // set track bar tick frequency
            if (maxValue <= 10)
            {
                trackBarIndex->TickFrequency = 1;
            }
            else
            {
                trackBarIndex->TickFrequency = (maxValue - 1) / 10 + 1;
            }
            // initialize record display run
            if (maxValue < 1)
            {
                buttonRecordDisplayRun->Enabled = false;
            }
            else
            {
                buttonRecordDisplayRun->Enabled = true;
            }
            lastRecordIndex = 1;
        }
        lastRecordIndex = dataIndex;
        // show measured value
        if (entryNumber == 0)
        {
            return;
        }
        // set index selection values
        numericUpDownIndex->Value = dataIndex;
        trackBarIndex->Value      = dataIndex;
        // read measured value
        eibTreeList->SelectedEIB->ReadRecordingData(dataIndex);
        // refresh display tabs
        RefreshDisplay(DISPLAY_MODE_RECORDING);
        //Debug::Print(String::Format(L"Recording index: {0}", dataIndex));
    }

    /// <summary>
    /// Set the recording measurement values display run mode.
    /// </summary>
	/// <param name="isEnabled">True if run mode is enabled.</param>
    System::Void FormMain::SetRecordingDisplayRun(bool isEnabled)
    {
        // set recording display run mode
        recordDisplayRun = isEnabled;
        if (isEnabled == false)
        {
            // set button picture
            buttonRecordDisplayRun->ImageIndex = 0;
        }
        else
        {
            // set button picture
            buttonRecordDisplayRun->ImageIndex = 1;
        }
    }

}
