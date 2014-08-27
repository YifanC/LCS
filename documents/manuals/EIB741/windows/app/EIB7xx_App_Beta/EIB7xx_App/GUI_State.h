///
/// \file       GUI_State.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: GUI_State.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: GUI_State.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      GUI state management classes.
///
/// This classes are used for the management of the GUI states.
///

#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace EIB7xx_App
{
    /// <summary>
    /// This enum lists all possible trigger events
    /// to change the GUI state.
    /// </summary>
    enum class GUI_StateTrigger : UInt32
    {
        NotUsed,
        InitSystem,
        OpenProject,
        SaveProject,
        AddEIB,
        RemoveEIB,
        InitEIB,
        RunEIB,
        StopEIB,
        SelectDevice,
        SelectEIB,
        SelectAxis,
        MenuConfig,
        MenuPosition,
        TabEIB,
        TabAxis
    };

    /// <summary>
    /// This enum lists all possible states of the Main Tree View.
    /// </summary>
    enum class GUI_StateTree : UInt32
    {
        Init    = 0,
        NoEIB   = 1000,
        EIB     = 2000,
        Axis    = 3000,
        All     = 9000,
    };

    /// <summary>
    /// This enum lists all possible states of the Main Tab Ctrl.
    /// </summary>
    enum class GUI_StateTab : UInt32
    {
        Init    = 0,
        NoTab   = 100,
        EIB     = 200,
        Axis    = 300,
        All     = 900,
    };

    /// <summary>
    /// This enum lists all possible states of operation mode.
    /// </summary>
    enum class GUI_StateOpMode : UInt32
    {
        Init    = 0,
        Empty   = 10,
        Config  = 20,
        Run     = 30,
        All     = 90,
    };

    /// <summary>
    /// This enum lists all possible states of the Main Menu.
    /// </summary>
    enum class GUI_StateMenu : UInt32
    {
        Init     = 0,
        NoMenu   = 1,
        Config   = 2,
        Position = 3,
        All      = 9,
    };

    /// <summary>
    /// This class represents the state of the GUI.
    /// </summary>
    public ref class GUI_State
    {
    private:
        GUI_StateTree   treeState;      ///< Main Tree View state
        GUI_StateTab    tabState;       ///< Main Tab Ctrl state
        GUI_StateOpMode opModeState;    ///< operation mode state
        GUI_StateMenu   menuState;      ///< Main Menu state

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        GUI_State()
        {
            treeState   = GUI_StateTree::Init;
            tabState    = GUI_StateTab::Init;
            opModeState = GUI_StateOpMode::Init;
            menuState   = GUI_StateMenu::Init;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        GUI_State(UInt32 value)
        {
            Value = value;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        GUI_State(GUI_State ^ source)
        {
            Copy(source);
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        GUI_State(GUI_StateTree   treeState,
                  GUI_StateTab    tabState,
                  GUI_StateOpMode opModeState,
                  GUI_StateMenu   menuState)
        {
            Copy(treeState, tabState, opModeState, menuState);
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(GUI_StateTree   treeState,
                  GUI_StateTab    tabState,
                  GUI_StateOpMode opModeState,
                  GUI_StateMenu   menuState)
        {
            this->treeState   = treeState;
            this->tabState    = tabState;
            this->opModeState = opModeState;
            this->menuState   = menuState;
        }
        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(GUI_State ^ source)
        {
            Copy(source->treeState,
                 source->tabState,
                 source->opModeState,
                 source->menuState);
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(GUI_StateTree   treeState,
                     GUI_StateTab    tabState,
                     GUI_StateOpMode opModeState,
                     GUI_StateMenu   menuState)
        {
            if ((this->treeState   != treeState)   && (treeState   != GUI_StateTree::All))   return false;
            if ((this->tabState    != tabState)    && (tabState    != GUI_StateTab::All))    return false;
            if ((this->opModeState != opModeState) && (opModeState != GUI_StateOpMode::All)) return false;
            if ((this->menuState   != menuState)   && (menuState   != GUI_StateMenu::All))   return false;
            return true;
        }
        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(GUI_State ^ source)
        {
            return IsEqual(source->treeState,
                           source->tabState,
                           source->opModeState,
                           source->menuState);
        }

        /// <summary>
        /// Gets or sets the Main Tree View state.
        /// </summary>
        property GUI_StateTree TreeState
        {
            GUI_StateTree get() { return treeState; }
            void set(GUI_StateTree value) { treeState = value; }
        }

        /// <summary>
        /// Gets or sets the Main Tree View state.
        /// </summary>
        property GUI_StateTab TabState
        {
            GUI_StateTab get() { return tabState; }
            void set(GUI_StateTab value) { tabState = value; }
        }

        /// <summary>
        /// Gets or sets the Main Tree View state.
        /// </summary>
        property GUI_StateOpMode OpModeState
        {
            GUI_StateOpMode get() { return opModeState; }
            void set(GUI_StateOpMode value) { opModeState = value; }
        }

        /// <summary>
        /// Gets or sets the Main Tree View state.
        /// </summary>
        property GUI_StateMenu MenuState
        {
            GUI_StateMenu get() { return menuState; }
            void set(GUI_StateMenu value) { menuState = value; }
        }

        /// <summary>
        /// Gets the value of the state.
        /// </summary>
        property UInt32 Value
        {
            UInt32 get()
            {
                return   UInt32(treeState)
                       + UInt32(tabState)
                       + UInt32(opModeState)
                       + UInt32(menuState);
            }
            void set(UInt32 value)
            {
                treeState   =  (GUI_StateTree)  (UInt32(value / 1000) * 1000);
                value       -= UInt32(treeState);
                tabState    =  (GUI_StateTab)   (UInt32(value /  100) *  100);
                value       -= UInt32(tabState);
                opModeState =  (GUI_StateOpMode)(UInt32(value /   10) *   10);
                value       -= UInt32(opModeState);
                menuState   =  (GUI_StateMenu)          value;
            }
        }

    };

    // list of GUI states
    typedef Dictionary<UInt32, UInt32>                  StateList;

    // list of GUI state triggers
    typedef Dictionary<GUI_StateTrigger, StateList ^>   TriggerList;

}
