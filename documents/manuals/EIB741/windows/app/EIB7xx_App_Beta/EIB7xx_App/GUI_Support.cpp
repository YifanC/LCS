///
/// \file       GUI_Support.cpp
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: GUI_Support.cpp,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: GUI_Support.cpp,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      GUI support function class.
///
/// This class provides functions for access to report list,
/// message boxes, main tree view and other GUI elements.
///

#include "StdAfx.h"
#include "FormMain.h"

using namespace System;
using namespace System::Resources;
using namespace System::Windows::Forms;
using namespace System::Reflection;
using namespace System::Diagnostics;

namespace EIB7xx_App
{
        /// <summary>
        /// Refresh application window.
        /// </summary>
        System::Void GUI_Support::RefreshWindow()
        {
            // check if function is called from other thread
            if (mainForm->InvokeRequired == true)
            {
                mainForm->Invoke(refreshWindowDel);
            }
            else
            {
                RefreshWindowInt();
            }
        }

    /// <summary>
    /// Refresh application window.
    /// </summary>
    System::Void GUI_Support::RefreshWindowInt()
    {
        mainForm->Refresh();
    }

    /// <summary>
    /// Show report text.
    /// The text will be shown in the report list.
    /// The selection will be set at the end of the report list.
    /// </summary>
    /// <param name="report">The text to display in the report list.</param>
    /// <param name="style">The style of text to display in the report list.</param>
    System::Void GUI_Support::ShowReportTextInt(String ^ report, ReportStyle style)
    {
        if (mainForm->IsClosing() == true)
        {
            return;
        }
        // set selection to end of text box
        richTextBoxReport->AppendText(L" ");
        // set selection style
        switch (style)
        {
        case ReportStyle::Warning:
            richTextBoxReport->SelectionColor = REPORT_COLOR_WARNING;
            break;
        case ReportStyle::Error:
            richTextBoxReport->SelectionColor = REPORT_COLOR_ERROR;
            break;
        case ReportStyle::Debug:
            richTextBoxReport->SelectionColor = REPORT_COLOR_DEBUG;
            break;
        default:
            richTextBoxReport->SelectionColor = REPORT_COLOR_TEXT;
            break;
        }
        // set new text entry
        richTextBoxReport->SelectedText = report;
        // show last line of report list
        //int length = richTextBoxReport->Lines->GetLength(0);
        //int index  = richTextBoxReport->GetFirstCharIndexFromLine(length - 1);
        //richTextBoxReport->Select(index, 0);
        richTextBoxReport->ScrollToCaret();
    }
}
