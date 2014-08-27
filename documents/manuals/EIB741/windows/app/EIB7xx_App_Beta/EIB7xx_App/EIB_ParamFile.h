///
/// \file       EIB_ParamFile.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_ParamFile.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: EIB_ParamFile.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB file classes.
///
/// This classes are used for reading and writing the EIB parameters
/// from or to a project file.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::IO;

namespace EIB7xx_App
{
 	#pragma region "EIB_ParamFile class definition"
    // special definitions
    #define PARAM_FILE_EXT          L"e7p"
    #define PARAM_FILE_HEADER       L"EIB 7xx App Project File"

    /// <summary>
    /// This class manages a project file used for writing and reading
    /// all EIB parameters.
    /// </summary>
    public ref class EIB_ParamFile : public EIB_BinFileBase
    {
    private:
        GUI_Support ^       guiSupport;     ///< the GUI support class
        ResourceManager ^   resManager;     ///< resource manager from main window

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
	    /// <param name="fileName">Project file name.</param>
	    /// <param name="writeFile">If true then write to file else read from file.</param>
        EIB_ParamFile(GUI_Support ^ guiSupport, String ^ fileName, bool writeFile)
            : EIB_BinFileBase(fileName, writeFile)
        {
            this->guiSupport = guiSupport;
            this->resManager = guiSupport->ResManager;
        }

		/// <summary>
		/// Check parameter version.
		/// </summary>
	    /// <param name="readVersion">The parameter version read from file.</param>
	    /// <param name="setVersion">The parameter version set value.</param>
		void CheckParamVersion(UInt32 readVersion, UInt32 setVersion)
		{
            CheckParamVersion(readVersion, setVersion, setVersion);
        }

		/// <summary>
		/// Check parameter version.
		/// </summary>
	    /// <param name="readVersion">The parameter version read from file.</param>
	    /// <param name="minSetVersion">The min parameter version set value.</param>
	    /// <param name="maxSetVersion">The max parameter version set value.</param>
		void CheckParamVersion(UInt32 readVersion, UInt32 minSetVersion, UInt32 maxSetVersion)
		{
            if (   (readVersion < minSetVersion)
                || (readVersion > maxSetVersion))
            {
                String ^ message = String::Format(resManager->GetString(L"errorEibParamFileVersion"),
                                                  fileName);
                throw gcnew EIB_FileException(message, fileName);
            }
		}
    };
    #pragma endregion
};
