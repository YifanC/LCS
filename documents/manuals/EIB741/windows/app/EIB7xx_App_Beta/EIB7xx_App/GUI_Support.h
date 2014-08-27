///
/// \file       GUI_Support.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: GUI_Support.h,v 1.3 2011/12/16 13:37:57 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: GUI_Support.h,v $
/// Revision 1.3  2011/12/16 13:37:57  rohn
/// Version 0.9.0.1 (siehe History.txt)
///
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      GUI support function class.
///
/// This class provides functions for access to report list,
/// message boxes, main tree view and other GUI elements.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Resources;
using namespace System::Windows::Forms;
using namespace System::Reflection;

namespace EIB7xx_App
{
    ref class FormMain;

    #define REPORT_FIRST_LINE       L"- {0}\n"
    #define REPORT_NEXT_LINE        L"      {0}\n"

    /// <summary>
    /// This class provides functions for access to report list,
    /// message boxes, main tree view and other GUI elements.
    /// </summary>
    public ref class GUI_Support
    {
    private:
        FormMain ^          mainForm;                   ///< the Main Form
        ResourceManager ^   resManager;                 ///< the resource manager from main window
        GUI_Config ^        guiConfig;                  ///< the GUI configuration parameters
        RichTextBox ^       richTextBoxReport;          ///< the Report List
        TreeView ^          treeViewMain;               ///< the Main Tree Vew
        PropertyGrid ^      propertyGridNetwork;        ///< the Network Property Grid
        PropertyGrid ^      propertyGridAxis;           ///< the Axis Property Grid
        PropertyGrid ^      propertyGridTrigger;        ///< the Trigger Property Grid
        PropertyGrid ^      propertyGridOpMode;         ///< the Operation Mode Property Grid
        PropertyGrid ^      propertyGridCrankshaft;     ///< the Crankshaft Configuration Property Grid
        PropertyGrid ^      propertyGridDisplay;        ///< the Display Property Grid
        DataGridView ^      dataGridViewDataPacket;     ///< the Data Packet Data Grid View
        ImageList ^         imageListLed;               ///< the Image List for LED Picture Controls
        Button ^            buttonNetworkWrite;         ///< the Network Parameters Write Button
        ProgressBar ^       progressBarFillLevel;       ///< the Level Progress Bar
        CheckBox ^          checkBoxDebug;              ///< the Debug Check Box

    public:
        /// delegate for calling report functions from other threads
        delegate void ShowReportTextDel(String ^ report, ReportStyle style);
        ShowReportTextDel ^ showReportTextDel;

        delegate void ShowProgressDel(UInt32 progress);
        ShowProgressDel ^   showProgressDel;

        delegate void RefreshWindowDel();
        RefreshWindowDel ^  refreshWindowDel;

        delegate void CheckBoxDebugDel(bool checked);
        CheckBoxDebugDel ^  checkBoxDebugDel;

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="mainForm">The Main Form.</param>
		/// <param name="resManager">The resource manager from main window.</param>
		/// <param name="guiConfig">The GUI configuration parameters.</param>
		/// <param name="richTextBoxReport">The Report List.</param>
		/// <param name="treeViewMain">The Main Tree Vew.</param>
		/// <param name="propertyGridNetwork">The Network Property Grid.</param>
		/// <param name="propertyGridAxis">The Axis Property Grid.</param>
		/// <param name="propertyGridTrigger">The Trigger Property Grid.</param>
		/// <param name="propertyGridOpMode">The Operation Mode Property Grid.</param>
		/// <param name="propertyGridCrankshaft">The Crankshaft Configuration Property Grid.</param>
		/// <param name="propertyGridDisplay">The Display Property Grid.</param>
		/// <param name="dataGridViewDataPacket">The Data Packet Data Grid View.</param>
		/// <param name="imageListLed">The Image List for LED Picture Controls.</param>
		/// <param name="buttonNetworkWrite">The Network Parameters Write Button.</param>
		/// <param name="progressBarFillLevel">The Level Progress Bar.</param>
		/// <param name="checkBoxDebug">The Debug Check Box.</param>
        GUI_Support(FormMain ^        mainForm,
                    ResourceManager ^ resManager,
                    GUI_Config ^      guiConfig,
                    RichTextBox ^     richTextBoxReport,
                    TreeView ^        treeViewMain,
                    PropertyGrid ^    propertyGridNetwork,
                    PropertyGrid ^    propertyGridAxis,
                    PropertyGrid ^    propertyGridTrigger,
                    PropertyGrid ^    propertyGridOpMode,
                    PropertyGrid ^    propertyGridCrankshaft,
                    PropertyGrid ^    propertyGridDisplay,
                    DataGridView ^    dataGridViewDataPacket,
                    ImageList ^       imageListLed,
                    Button ^          buttonNetworkWrite,
                    ProgressBar ^     progressBarFillLevel,
                    CheckBox ^        checkBoxDebug)
        {
            this->mainForm                  = mainForm;
            this->resManager                = resManager;
            this->guiConfig                 = guiConfig;
            this->richTextBoxReport         = richTextBoxReport;
            this->treeViewMain              = treeViewMain;
            this->propertyGridNetwork       = propertyGridNetwork;
            this->propertyGridAxis          = propertyGridAxis;
            this->propertyGridTrigger       = propertyGridTrigger;
            this->propertyGridOpMode        = propertyGridOpMode;
            this->propertyGridCrankshaft    = propertyGridCrankshaft;
            this->propertyGridDisplay       = propertyGridDisplay;
            this->dataGridViewDataPacket    = dataGridViewDataPacket;
            this->imageListLed              = imageListLed;
            this->buttonNetworkWrite        = buttonNetworkWrite;
            this->progressBarFillLevel      = progressBarFillLevel;
            this->checkBoxDebug             = checkBoxDebug;
            
            showReportTextDel = gcnew ShowReportTextDel(this, &GUI_Support::ShowReportTextInt);
            showProgressDel   = gcnew ShowProgressDel(this,   &GUI_Support::ShowProgressInt);
            refreshWindowDel  = gcnew RefreshWindowDel(this,  &GUI_Support::RefreshWindowInt);
            checkBoxDebugDel  = gcnew CheckBoxDebugDel(this,  &GUI_Support::CheckBoxDebugSet);
        }

        /// <summary>
        /// Gets the resource manager from main window.
        /// </summary>
        property ResourceManager ^ ResManager
        {
            ResourceManager ^ get() { return resManager; }
        }

        /// <summary>
        /// Gets the resource manager from main window.
        /// </summary>
        static ResourceManager ^ GetResManager()
        {
            return gcnew ResourceManager(RESOURCE_FILE, Assembly::GetExecutingAssembly());
        }

        /// <summary>
        /// Gets the GUI configuration parameters.
        /// </summary>
        property GUI_Config ^ GuiConfig
        {
            GUI_Config ^ get() { return guiConfig; }
        }

        /// <summary>
        /// Gets the Report List.
        /// </summary>
        property RichTextBox ^ RichTextBoxReport
        {
            RichTextBox ^ get() { return richTextBoxReport; }
        }

        /// <summary>
        /// Gets the Main Tree Vew.
        /// </summary>
        property TreeView ^ TreeViewMain
        {
            TreeView ^ get() { return treeViewMain; }
        }

        /// <summary>
        /// Gets the Network Property Grid.
        /// </summary>
        property PropertyGrid ^ PropertyGridNetwork
        {
            PropertyGrid ^ get() { return propertyGridNetwork; }
        }

        /// <summary>
        /// Gets the Axis Property Grid.
        /// </summary>
        property PropertyGrid ^ PropertyGridAxis
        {
            PropertyGrid ^ get() { return propertyGridAxis; }
        }

        /// <summary>
        /// Gets the Trigger Property Grid.
        /// </summary>
        property PropertyGrid ^ PropertyGridTrigger
        {
            PropertyGrid ^ get() { return propertyGridTrigger; }
        }

        /// <summary>
        /// Gets the Operation Mode Property Grid.
        /// </summary>
        property PropertyGrid ^ PropertyGridOpMode
        {
            PropertyGrid ^ get() { return propertyGridOpMode; }
        }

        /// <summary>
        /// Gets the Crankshaft Configuration Property Grid.
        /// </summary>
        property PropertyGrid ^ PropertyGridCrankshaft
        {
            PropertyGrid ^ get() { return propertyGridCrankshaft; }
        }

        /// <summary>
        /// Gets the Display Property Grid.
        /// </summary>
        property PropertyGrid ^ PropertyGridDisplay
        {
            PropertyGrid ^ get() { return propertyGridDisplay; }
        }

        /// <summary>
        /// Gets the Data Packet Data Grid View.
        /// </summary>
        property DataGridView ^ DataGridViewDataPacket
        {
            DataGridView ^ get() { return dataGridViewDataPacket; }
        }

        /// <summary>
        /// Gets the Image List for LED Picture Controls.
        /// </summary>
        property ImageList ^ ImageListLed
        {
            ImageList ^ get() { return imageListLed; }
        }

        /// <summary>
        /// Gets / sets the debug check box state.
        /// </summary>
        property bool CheckBoxDebug
        {
            bool get() { return checkBoxDebug->Checked; }
            void set(bool value)
            {
                // check if function is called from other thread
                if (checkBoxDebug->InvokeRequired == true)
                {
                    array<Object ^> ^ param = { value };
                    checkBoxDebug->Invoke(checkBoxDebugDel, param);
                }
                else
                {
                    CheckBoxDebugSet(value);
                }
            }
        }

        /// <summary>
        /// Sets the debug check box state.
        /// </summary>
        /// <param name="checked">The debug check box state.</param>
        System::Void CheckBoxDebugSet(bool checked)
        {
            checkBoxDebug->Checked = checked;
        }

        /// <summary>
        /// Refresh application window.
        /// </summary>
        System::Void RefreshWindow();

        /// <summary>
        /// Refresh application window.
        /// </summary>
        System::Void RefreshWindowInt();

        /// <summary>
        /// Set network tab write button enable state.
        /// </summary>
	    /// <param name="enable">The network tab write button enable state.</param>
        System::Void NetworkWriteEnable(bool enable)
        {
            buttonNetworkWrite->Enabled = enable;
        }

        /// <summary>
        /// Show a warning message box.
        /// </summary>
	    /// <param name="message">The message to display at the message box.</param>
	    /// <param name="buttons">The buttons of the message box.</param>
	    /// <param name="defaultButton">The default button of the message box.</param>
        /// <returns>The user input result to the message box.</returns>
        Windows::Forms::DialogResult MsgBoxWarning(String ^ message,
                                                   MessageBoxButtons buttons,
                                                   MessageBoxDefaultButton defaultButton)
        {
            // Displays the MessageBox.
            Windows::Forms::DialogResult result =
                            MessageBox::Show((IWin32Window ^)mainForm,
                                             message,
                                             resManager->GetString(L"textWarning"),
                                             buttons,
                                             MessageBoxIcon::Warning,
                                             defaultButton);
            return result;
        }

        /// <summary>
        /// Show an error message box.
        /// </summary>
	    /// <param name="message">The message to display at the message box.</param>
	    /// <param name="buttons">The buttons of the message box.</param>
	    /// <param name="defaultButton">The default button of the message box.</param>
        /// <returns>The user input result to the message box.</returns>
        Windows::Forms::DialogResult MsgBoxError(String ^ message,
                                                 MessageBoxButtons buttons,
                                                 MessageBoxDefaultButton defaultButton)
        {
            // Displays the MessageBox.
            Windows::Forms::DialogResult result =
                            MessageBox::Show((IWin32Window ^)mainForm,
                                             message,
                                             resManager->GetString(L"textError"),
                                             buttons,
                                             MessageBoxIcon::Error,
                                             defaultButton);
            return result;
        }

        /// <summary>
        /// Show an error message box with OK button.
        /// </summary>
        System::Void MsgBoxErrorOk(String ^ message)
        {
            // Displays the MessageBox.
            MsgBoxError(message, MessageBoxButtons::OK, MessageBoxDefaultButton::Button1);
        }

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list.
        /// The selection will be set at the end of the report list.
        /// </summary>
        /// <param name="report">The text to display in the report list.</param>
        /// <param name="style">The style of text to display in the report list.</param>
        System::Void ShowReportText(String ^ report, ReportStyle style)
        {
            // check if function is called from other thread
            if (richTextBoxReport->InvokeRequired == true)
            {
                array<Object ^> ^ param = { report, style };
                richTextBoxReport->Invoke(showReportTextDel, param);
            }
            else
            {
                ShowReportTextInt(report, style);
            }
        }

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list.
        /// The selection will be set at the end of the report list.
        /// </summary>
        /// <param name="report">The text to display in the report list.</param>
        /// <param name="style">The style of text to display in the report list.</param>
        System::Void ShowReportTextInt(String ^ report, ReportStyle style);

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list in debug report style.
        /// The selection will be set at the end of the report list.
        /// </summary>
	    /// <param name="report">The text to display in the report list.</param>
        System::Void ShowDebugText(String ^ report)
        {
            ShowReportText(report, ReportStyle::Debug);
        }

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list in information report style.
        /// The selection will be set at the end of the report list.
        /// </summary>
	    /// <param name="textLine1">The text line 1 to display in the report list.</param>
        System::Void ShowInfoText(String ^ textLine1)
        {
            String ^ message =   String::Format(REPORT_FIRST_LINE, textLine1);
            ShowReportText(message, ReportStyle::Information);
        }

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list in information report style.
        /// The selection will be set at the end of the report list.
        /// </summary>
	    /// <param name="textLine1">The text line 1 to display in the report list.</param>
	    /// <param name="textLine2">The text line 2 to display in the report list.</param>
        System::Void ShowInfoText(String ^ textLine1, String ^ textLine2)
        {
            String ^ message =   String::Format(REPORT_FIRST_LINE, textLine1)
                               + String::Format(REPORT_NEXT_LINE,  textLine2);
            ShowReportText(message, ReportStyle::Information);
        }

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list in warning report style.
        /// The selection will be set at the end of the report list.
        /// </summary>
	    /// <param name="textLine1">The text line 1 to display in the report list.</param>
        System::Void ShowWarningText(String ^ textLine1)
        {
            String ^ message =   String::Format(REPORT_FIRST_LINE, textLine1);
            ShowReportText(message, ReportStyle::Warning);
        }

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list in warning report style.
        /// The selection will be set at the end of the report list.
        /// </summary>
	    /// <param name="textLine1">The text line 1 to display in the report list.</param>
	    /// <param name="textLine2">The text line 2 to display in the report list.</param>
        System::Void ShowWarningText(String ^ textLine1, String ^ textLine2)
        {
            String ^ message =   String::Format(REPORT_FIRST_LINE, textLine1)
                               + String::Format(REPORT_NEXT_LINE,  textLine2);
            ShowReportText(message, ReportStyle::Warning);
        }

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list in error report style.
        /// The selection will be set at the end of the report list.
        /// </summary>
	    /// <param name="textLine1">The text line 1 to display in the report list.</param>
        System::Void ShowErrorText(String ^ textLine1)
        {
            String ^ message =   String::Format(REPORT_FIRST_LINE, textLine1);
            ShowReportText(message, ReportStyle::Error);
        }

        /// <summary>
        /// Show report text.
        /// The text will be shown in the report list in error report style.
        /// The selection will be set at the end of the report list.
        /// </summary>
	    /// <param name="textLine1">The text line 1 to display in the report list.</param>
	    /// <param name="textLine2">The text line 2 to display in the report list.</param>
        System::Void ShowErrorText(String ^ textLine1, String ^ textLine2)
        {
            String ^ message =   String::Format(REPORT_FIRST_LINE, textLine1)
                               + String::Format(REPORT_NEXT_LINE,  textLine2);
            ShowReportText(message, ReportStyle::Error);
        }

        /// <summary>
        /// Show progress in %.
        /// </summary>
        /// <param name="progress">The progress value in %.</param>
        System::Void ShowProgress(UInt32 progress)
        {
            // check if function is called from other thread
            if (progressBarFillLevel->InvokeRequired == true)
            {
                array<Object ^> ^ param = { progress };
                progressBarFillLevel->Invoke(showProgressDel, param);
            }
            else
            {
                ShowProgressInt(progress);
            }
        }

        /// <summary>
        /// Show progress in %.
        /// </summary>
        /// <param name="progress">The progress value in %.</param>
        System::Void ShowProgressInt(UInt32 progress)
        {
            progressBarFillLevel->Value = progress;
        }
    };
}
