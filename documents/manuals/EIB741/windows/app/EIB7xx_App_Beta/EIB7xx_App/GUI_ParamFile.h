///
/// \file       GUI_ParamFile.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: GUI_ParamFile.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: GUI_ParamFile.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB file classes.
///
/// This class is used for reading and writing the GUI parameters
/// from or to an initialization file.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::IO;

namespace EIB7xx_App
{
 	#pragma region "GUI_ParamFile class definition"
    // TODO: System.Deployment.Application.InvalidDeploymentException
    // special definitions
    #define INI_FILE_NAME           Application::CommonAppDataPath + L"\\.." + L"\\EIB7xx_App.ini"
    #define INI_FILE_HEADER         L"EIB 7xx App Initialization File"

    /// <summary>
    /// This class is used for reading and writing the GUI parameters
    /// from or to an initialization file.
    /// </summary>
    public ref class GUI_ParamFile : public EIB_BinFileBase
    {
    private:

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
	    /// <param name="fileName">Project file name.</param>
	    /// <param name="writeFile">If true then write to file else read from file.</param>
        GUI_ParamFile(String ^ fileName, bool writeFile)
            : EIB_BinFileBase(fileName, writeFile)
        {
        }

		/// <summary>
		/// Check parameter version.
		/// </summary>
	    /// <param name="readVersion">The parameter version read from file.</param>
	    /// <param name="setVersion">The parameter version set value.</param>
		void CheckParamVersion(UInt32 readVersion, UInt32 setVersion)
		{
            if (readVersion != setVersion)
            {
                throw gcnew Exception(L"Bad initialization file parameter version");
            }
		}
    };
    #pragma endregion
}
