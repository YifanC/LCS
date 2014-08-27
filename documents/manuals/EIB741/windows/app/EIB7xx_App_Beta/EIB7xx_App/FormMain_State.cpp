///
/// \file       FormMain_State.cpp
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: FormMain_State.cpp,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: FormMain_State.cpp,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      Main window class GUI state machine functions.
///

#include "StdAfx.h"
#include "FormMain.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace EIB7xx_App
{
    /// <summary>
    /// This array lists all possible trigger events
    /// to change the GUI state.
    /// It is derived from enum class GUI_StateTrigger.
    /// </summary>
    GUI_StateTrigger TRIGGER_LIST[] =
    {
        GUI_StateTrigger::OpenProject,
        GUI_StateTrigger::SaveProject,
        GUI_StateTrigger::AddEIB,
        GUI_StateTrigger::RemoveEIB,
        GUI_StateTrigger::InitEIB,
        GUI_StateTrigger::RunEIB,
        GUI_StateTrigger::StopEIB,
        GUI_StateTrigger::SelectDevice,
        GUI_StateTrigger::SelectEIB,
        GUI_StateTrigger::SelectAxis,
        GUI_StateTrigger::MenuConfig,
        GUI_StateTrigger::MenuPosition,
        GUI_StateTrigger::TabEIB,
        GUI_StateTrigger::TabAxis
    };

    /// <summary>
    /// This array lists all possible states of the GUI.
    /// It is derived from the excel sheet GUI_States.xls.
    /// </summary>
    UInt32 STATE_LIST[] =
    {
        1111,
        1121,
        1131,
        2121,
        2131,
        2222,
        2223,
        2233,
        3121,
        3131,
        3222,
        3223,
        3233,
        3322,
        3323,
        3333
    };

    /// <summary>
    /// This array contains the GUI state machine in the form
    /// newStateValue = item[currentStateIndex][triggerIndex]
    /// It is derived from the excel sheet EIB7xx_App_GUI_States.xls.
    /// </summary>
    Int32 NEW_STATE_LIST[] =
    {
        /* S     T   10,   11,   20,   21,   22,   23,   24,   31,   32,   33,   41,   42,   51,   52 */
        /* 1111 */ 1121,   -1, 2222,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
        /* 1121 */ 1121,    0, 2222, 1121,    0, 1131,    0,    0, 2121, 3121,   -1,   -1,   -1,   -1,
        /* 1131 */   -1,   -1,   -1,   -1,   -1,   -1, 1121,    0, 2131, 3131,   -1,   -1,   -1,   -1,
        /* 2121 */ 1121,    0, 2222, 1121,    0, 2131,    0, 1121,    0, 3121, 2222, 2223,   -1,   -1,
        /* 2131 */   -1,   -1,   -1,   -1,   -1,   -1, 2121, 1131,    0, 3131,   -1, 2233,   -1,   -1,
        /* 2222 */ 1121,    0, 2222, 1121,    0, 2233,    0, 1121,    0, 3222,    0, 2223,    0,   -1,
        /* 2223 */ 1121,    0, 2222, 1121,    0, 2233,    0, 1121,    0, 3223, 2222,    0,    0,   -1,
        /* 2233 */   -1,   -1,   -1,   -1,   -1,   -1, 2223, 1131,    0, 3233,   -1,    0,    0,   -1,
        /* 3121 */ 1121,    0, 2222, 1121,    0, 3131,    0, 1121, 2121,    0, 3322, 3323,   -1,   -1,
        /* 3131 */   -1,   -1,   -1,   -1,   -1,   -1, 3121, 1131, 2131,    0,   -1, 3333,   -1,   -1,
        /* 3222 */ 1121,    0, 2222, 1121,    0, 3233,    0, 1121, 2222,    0,    0, 3223,    0, 3322,
        /* 3223 */ 1121,    0, 2222, 1121,    0, 3233,    0, 1121, 2223,    0, 3222,    0,    0, 3323,
        /* 3233 */   -1,   -1,   -1,   -1,   -1,   -1, 3223, 1131, 2233,    0,   -1,    0,    0, 3333,
        /* 3322 */ 1121,    0, 2222, 1121,    0, 3333,    0, 1121, 2222,    0,    0, 3323, 3222,    0,
        /* 3323 */ 1121,    0, 2222, 1121,    0, 3333,    0, 1121, 2223,    0, 3322,    0, 3223,    0,
        /* 3333 */   -1,   -1,   -1,   -1,   -1,   -1, 3323, 1131, 2233,    0,   -1,    0, 3233,    0
    };

    /// <summary>
    /// Initialize the GUI state mangement.
    /// </summary>
    System::Void FormMain::InitializeGuiState()
    {
        // reset state guard
        stateGuard = GUI_StateTrigger::NotUsed;
        // initialize trigger and state lists
        StateList ^ stateList;
        Int32 newStateValue;
        UInt32 i, j, newStateIndex;
        UInt32 triggerCount = sizeof(TRIGGER_LIST) / sizeof(GUI_StateTrigger);
        UInt32 stateCount   = sizeof(STATE_LIST)   / sizeof(UInt32);
        // loop for all triggers
        for (i = 0; i < triggerCount; i ++)
        {
            stateList = gcnew StateList();
            // loop for all states
            for (j = 0; j < stateCount; j ++)
            {
                newStateIndex = j * triggerCount + i;
                newStateValue = NEW_STATE_LIST[newStateIndex];
                // check if trigger is allowed in this state
                if (newStateValue >= 0)
                {
                    stateList[STATE_LIST[j]] = UInt32(newStateValue);
                }
            }
            triggerList[TRIGGER_LIST[i]] = stateList;
        }
    }

    /// <summary>
    /// Trigger a new GUI state.
    /// This function will change the current GUI state to a new
    /// GUI state depending on a trigger event from the GUI and
    /// the current GUI state.
    /// </summary>
	/// <param name="trigger">The GUI state trigger.</param>
    System::Void FormMain::TriggerGuiState(GUI_StateTrigger trigger)
    {
        TriggerGuiState(trigger, nullptr);
    }

    /// <summary>
    /// Trigger a new GUI state.
    /// This function will change the current GUI state to a new
    /// GUI state depending on a trigger event from the GUI and
    /// the current GUI state.
    /// </summary>
	/// <param name="trigger">The GUI state trigger.</param>
	/// <param name="newTabPage">The new tab page to set.</param>
    System::Void FormMain::TriggerGuiState(GUI_StateTrigger trigger, TabPage ^ newTabPage)
    {
        // check state guard
        if (   (stateGuard != GUI_StateTrigger::NotUsed)
            && (stateGuard != trigger))
        {
            return;
        }
        stateGuard = trigger;

        GUI_State ^ newState = nullptr;

        // operate special triggers
        switch (trigger)
        {
        case GUI_StateTrigger::InitSystem:
            if (guiState.IsEqual(GUI_StateTree::Init,
                                 GUI_StateTab::Init,
                                 GUI_StateOpMode::Init,
                                 GUI_StateMenu::Init) == true)
            {
                newState = gcnew GUI_State(GUI_StateTree::NoEIB,
                                           GUI_StateTab::NoTab,
                                           GUI_StateOpMode::Empty,
                                           GUI_StateMenu::NoMenu);
            }
            break;
        }
        // check for unexpected trigger
        if (newState == nullptr)
        {
            StateList ^ stateList;
            UInt32 currentState = guiState.Value;
            UInt32 newStateValue;
            // check trigger list for current trigger
            if (triggerList.ContainsKey(trigger) == true)
            {
                stateList = triggerList[trigger];
                // check state list for current state
                if (stateList->ContainsKey(currentState) == true)
                {
                    newStateValue = stateList[currentState];
                    // check for current state
                    if (newStateValue == 0)
                    {
                        // set new state = current state
                        newState = %guiState;
                    }
                    else
                    {
                        // set new state
                        newState = gcnew GUI_State(newStateValue);
                    }
                }
            }
        }
        // check for unexpected trigger
        if (newState != nullptr)
        {
            GUI_State oldState = %guiState;

            // operate special triggers
            switch (trigger)
            {
            case GUI_StateTrigger::RemoveEIB:
                if (   (newState->IsEqual(GUI_StateTree::NoEIB,
                                          GUI_StateTab::NoTab,
                                          GUI_StateOpMode::Config,
                                          GUI_StateMenu::NoMenu) == true)
                    && (treeViewMain->Nodes[0]->GetNodeCount(false) == 0))
                {
                    newState->OpModeState = GUI_StateOpMode::Empty;
                }
                break;
            }

            // set new GUI state
            SetGuiState(newState, newTabPage);

            //guiSupport->ShowDebugText(String::Format(L"Trigger: {0} from: {1} to: {2} state\n",
            //                                         trigger.ToString(),
            //                                         oldState.Value,
            //                                         newState->Value));
        }
        else
        {
            // report error
            guiSupport->ShowErrorText(resManager->GetString(L"infoInternalError"),
                                      String::Format(L"Unexpected trigger: {0}, in GUI state: {1}!",
                                                     trigger.ToString(), guiState.Value));
        }
        // reset state guard
        stateGuard = GUI_StateTrigger::NotUsed;
    }

    /// <summary>
    /// Set the new GUI state.
    /// This function will change the visible and / or enable state
    /// of menu entries, buttons and tab ctrl pages depending on
    /// the new GUI state.
    /// </summary>
	/// <param name="newState">The new GUI state.</param>
	/// <param name="newTabPage">The new tab page to set.</param>
    System::Void FormMain::SetGuiState(GUI_State ^ newState, TabPage ^ newTabPage)
    {
        // check if state has changed
        if (newState != %guiState)
        {
            // disable all components
            projektOeffnenToolStripMenuItem->Enabled = false;
            projektSpeichernToolStripMenuItem->Enabled = false;
            projektSpeichernUnterToolStripMenuItem->Enabled = false;
            konfigurationToolStripMenuItem->Enabled = false;
            anzeigeToolStripMenuItem->Enabled = false;
            toolStripButtonEibAdd1->Enabled = false;
            toolStripButtonEibDel1->Enabled = false;
            toolStripButtonInit1->Enabled = false;
            toolStripButtonRun1->Enabled = false;
            toolStripButtonStop->Enabled = false;
            tabControlMain->Visible = false;
            konfigurationToolStripMenuItem->Font = configMenuFontOriginal;
            anzeigeToolStripMenuItem->Font = displayMenuFontOriginal;
            // check new state
            if (   (newState->OpModeState == GUI_StateOpMode::Empty)
                || (newState->OpModeState == GUI_StateOpMode::Config))
            {
                // state is GUI_StateOpMode::Empty or GUI_StateOpMode::Config
                projektOeffnenToolStripMenuItem->Enabled = true;
                toolStripButtonEibAdd1->Enabled = true;

                if (newState->OpModeState == GUI_StateOpMode::Empty)
                {
                    // state is GUI_StateOpMode::Empty
                    toolStripButtonEibAdd1->Select();
                }
                if (newState->OpModeState == GUI_StateOpMode::Config)
                {
                    // state is GUI_StateOpMode::Config
                    projektSpeichernToolStripMenuItem->Enabled = true;
                    projektSpeichernUnterToolStripMenuItem->Enabled = true;
                    toolStripButtonInit1->Enabled = true;
                    toolStripButtonRun1->Enabled = true;

                    if (   (newState->TreeState == GUI_StateTree::EIB)
                        || (newState->TreeState == GUI_StateTree::Axis))
                    {
                        // state is GUI_StateTree::EIB or GUI_StateTree::Axis
                        toolStripButtonEibDel1->Enabled = true;
                        konfigurationToolStripMenuItem->Enabled = true;
                        anzeigeToolStripMenuItem->Enabled = true;
                    }
                }
            }
            if (newState->OpModeState == GUI_StateOpMode::Run)
            {
                // state is GUI_StateOpMode::Run
                toolStripButtonStop->Enabled = true;

                if (newState->TreeState != GUI_StateTree::NoEIB)
                {
                    // state is GUI_StateTree::EIB or GUI_StateTree::Axis
                    anzeigeToolStripMenuItem->Enabled = true;
                }
            }
            if (   (newState->MenuState == GUI_StateMenu::Config)
                || (newState->MenuState == GUI_StateMenu::Position))
            {
                // state is GUI_StateMenu::Config or GUI_StateMenu::Position
                tabControlMain->Visible = false;
                TabPage ^ selectedTabPage = nullptr;

                if (newState->MenuState == GUI_StateMenu::Config)
                {
                    // state is GUI_StateMenu::Config
                    konfigurationToolStripMenuItem->Font = configMenuFontSelected;

                    // enable config eib tabs
                    if (tabControlMain->TabPages->Contains(tabPageNetwork) == false)
                    {
                        tabControlMain->TabPages->Clear();
                        tabControlMain->TabPages->Add(tabPageNetwork);
                        tabControlMain->TabPages->Add(tabPageDataPacket);
                        tabControlMain->TabPages->Add(tabPageTrigger);
                        tabControlMain->TabPages->Add(tabPageOperationMode);
                        // add application tabs
                        if (guiConfig.GuiParam->AppCrankshaft == true)
                        {
                            tabControlMain->TabPages->Add(tabPageCrankshaftConfig);
                        }
                    }

                    if (newState->TreeState == GUI_StateTree::Axis)
                    {
                        // enable config axis tabs
                        if (tabControlMain->TabPages->Contains(tabPageAxis) == false)
                        {
                            tabControlMain->TabPages->Insert(1, tabPageAxis);
                        }
                        if (   (guiState.MenuState == GUI_StateMenu::Position)
                            && (newState->TabState == GUI_StateTab::Axis))
                        {
                            selectedTabPage = tabPageAxis;
                        }
                    }
                    else
                    {
                        // disable config axis tabs
                        if (tabControlMain->TabPages->Contains(tabPageAxis) == true)
                        {
                            if (tabControlMain->SelectedTab == tabPageAxis)
                            {
                                selectedTabPage = tabPageNetwork;
                            }
                            tabControlMain->TabPages->Remove(tabPageAxis);
                        }
                    }
                }
                if (newState->MenuState == GUI_StateMenu::Position)
                {
                    // state is GUI_StateMenu::Position
                    anzeigeToolStripMenuItem->Font = displayMenuFontSelected;

                    // enable position eib tabs
                    if (tabControlMain->TabPages->Contains(tabPageSummary) == false)
                    {
                        tabControlMain->TabPages->Clear();
                        tabControlMain->TabPages->Add(tabPageSummary);
                        tabControlMain->TabPages->Add(tabPageOptions);
                        // add application tabs
                        if (guiConfig.GuiParam->AppCrankshaft == true)
                        {
                            tabControlMain->TabPages->Insert(1, tabPageCrankshaftDisplay);
                        }
                    }

                    if (newState->TreeState == GUI_StateTree::Axis)
                    {
                        // enable position axis tabs
                        if (tabControlMain->TabPages->Contains(tabPageDetail) == false)
                        {
                            tabControlMain->TabPages->Insert(1, tabPageDetail);
                        }
                        if (   (guiState.MenuState == GUI_StateMenu::Config)
                            && (newState->TabState == GUI_StateTab::Axis))
                        {
                            selectedTabPage = tabPageDetail;
                        }
                    }
                    else
                    {
                        // disable position axis tabs
                        if (tabControlMain->TabPages->Contains(tabPageDetail) == true)
                        {
                            if (tabControlMain->SelectedTab == tabPageDetail)
                            {
                                selectedTabPage = tabPageSummary;
                            }
                            tabControlMain->TabPages->Remove(tabPageDetail);
                        }
                    }
                }
                // set new tap page
                if (   (newTabPage != nullptr)
                    && (tabControlMain->TabPages->Contains(newTabPage) == true))
                {
                    tabControlMain->SelectedTab = newTabPage;
                }
                else if (selectedTabPage != nullptr)
                {
                    tabControlMain->SelectedTab = selectedTabPage;
                }
                tabControlMain->Visible = true;
            }
            // set new GUI state
            guiState.Copy(newState);
        }
    }

    /// <summary>
    /// This function will check if a given GUI state is set.
    /// </summary>
	/// <param name="checkState">The GUI state to check for.</param>
    bool FormMain::CheckGuiState(GUI_State ^ checkState)
    {
        // check state guard
        if (stateGuard != GUI_StateTrigger::NotUsed)
        {
            return false;
        }
        // check state
        if (guiState.IsEqual(checkState) == false)
        {
            return false;
        }
        return true;
    }

}
