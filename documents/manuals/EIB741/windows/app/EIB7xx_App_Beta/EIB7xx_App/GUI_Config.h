///
/// \file       GUI_Config.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: GUI_Config.h,v 1.3 2011/12/16 13:37:56 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: GUI_Config.h,v $
/// Revision 1.3  2011/12/16 13:37:56  rohn
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
/// This classes manage parameters for GUI configuration.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Resources;
using namespace System::Windows::Forms;
using namespace System::Reflection;

namespace EIB7xx_App
{
 	#pragma region "GUI_Param class definition"
    // special definitions
    #define GUI_DEFAULT_PARAM_FILE_NAME         Application::CommonAppDataPath + L"\\.."
    #define GUI_DEFAULT_DATA_FILE_NAME          Application::CommonAppDataPath + L"\\.."
    #define GUI_DEFAULT_REFERENCE_FILE_NAME     Application::CommonAppDataPath + L"\\.."
    #define GUI_DEFAULT_FIRMWARE_FILE_NAME      Application::CommonAppDataPath + L"\\.."
    #define GUI_DEFAULT_SAVE_MEASURE_DATA       false
    #define GUI_DEFAULT_SHOW_MULTITURN          true
    #define GUI_DEFAULT_APP_CRANKSHAFT          false
    #define GUI_DEFAULT_ADMIN_MODE              false
    #define FIRMWARE_FILE_EXT                   L"flash"

    /// <summary>
    /// This class contains all GUI configuration parameters.
    /// </summary>
    public ref class GUI_Param
    {
    private:
        UInt32      fileVersion;            ///< parameter version for parameter file

        String ^    paramFileName;          ///< the current used parameter file name
        String ^    dataFileName;           ///< the current used measure data file name
        String ^    referenceFileName;      ///< the current used reference data file name
        String ^    firmwareFileName;       ///< the current used firmware data file name

        bool        saveMeasureData;        ///< save measure data flag
        bool        showMultiturn;          ///< show multiturn encoder revolutions flag
        bool        appCrankshaft;          ///< crankshaft application flag
        bool        adminMode;              ///< admin mode flag

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        GUI_Param()
        {
            fileVersion = 1;

            paramFileName       = GUI_DEFAULT_PARAM_FILE_NAME;
            dataFileName        = GUI_DEFAULT_DATA_FILE_NAME;
            referenceFileName   = GUI_DEFAULT_REFERENCE_FILE_NAME;
            firmwareFileName    = GUI_DEFAULT_FIRMWARE_FILE_NAME;

            saveMeasureData     = GUI_DEFAULT_SAVE_MEASURE_DATA;
            showMultiturn       = GUI_DEFAULT_SHOW_MULTITURN;

            appCrankshaft       = GUI_DEFAULT_APP_CRANKSHAFT;
            adminMode           = GUI_DEFAULT_ADMIN_MODE;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(GUI_Param ^ source)
        {
            paramFileName       = String::Copy(source->paramFileName);
            dataFileName        = String::Copy(source->dataFileName);
            referenceFileName   = String::Copy(source->referenceFileName);
            firmwareFileName    = String::Copy(source->firmwareFileName);
            saveMeasureData     = source->saveMeasureData;
            showMultiturn       = source->showMultiturn;
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
		/// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(GUI_ParamFile ^ paramFile)
        {
            // read / write parameter version
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            paramFile->ReadWriteString(paramFileName);
            paramFile->ReadWriteBool(saveMeasureData);
            paramFile->ReadWriteString(dataFileName);
            paramFile->ReadWriteString(referenceFileName);
            paramFile->ReadWriteString(firmwareFileName);
            paramFile->ReadWriteBool(showMultiturn);
        }

        /// <summary>
        /// Gets / sets the current used parameter file name.
        /// </summary>
        property String ^ ParamFileName
        {
            String ^ get() { return paramFileName; }
            void set(String ^ value) { paramFileName = value; }
        }

        /// <summary>
        /// Gets / sets the current used measure data file name.
        /// </summary>
        property String ^ DataFileName
        {
            String ^ get() { return dataFileName; }
            void set(String ^ value) { dataFileName = value; }
        }

        /// <summary>
        /// Gets / sets the current used reference data file name.
        /// </summary>
        property String ^ ReferenceFileName
        {
            String ^ get() { return referenceFileName; }
            void set(String ^ value) { referenceFileName = value; }
        }

        /// <summary>
        /// Gets / sets the current used firmware data file name.
        /// </summary>
        property String ^ FirmwareFileName
        {
            String ^ get() { return firmwareFileName; }
            void set(String ^ value) { firmwareFileName = value; }
        }

        /// <summary>
        /// Gets / sets the save measure data flag.
        /// </summary>
        property bool SaveMeasureData
        {
            bool get() { return saveMeasureData; }
            void set(bool value) { saveMeasureData = value; }
        }

        /// <summary>
        /// Gets / sets the show multiturn encoder revolutions flag.
        /// </summary>
        property bool ShowMultiturn
        {
            bool get() { return showMultiturn; }
            void set(bool value) { showMultiturn = value; }
        }

        /// <summary>
        /// Gets / sets the crankshaft application flag.
        /// </summary>
        property bool AppCrankshaft
        {
            bool get() { return appCrankshaft; }
            void set(bool value) { appCrankshaft = value; }
        }

        /// <summary>
        /// Gets / sets the admin mode flag.
        /// </summary>
        property bool AdminMode
        {
            bool get() { return adminMode; }
            void set(bool value) { adminMode = value; }
        }
    };
    #pragma endregion

 	#pragma region "GUI_Config class definition"
    /// <summary>
    /// This class manages parameters for GUI configuration.
    /// </summary>
    public ref class GUI_Config
    {
    private:
        UInt32      fileVersion;        ///< parameter version for parameter file

        GUI_Param   guiParam;           ///< The GUI configuration parameters

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        GUI_Config()
        {
            fileVersion = 1;
        }

        /// <summary>
        /// Gets the GUI configuration parameters.
        /// </summary>
        property GUI_Param ^ GuiParam
        {
            GUI_Param ^ get() { return %guiParam; }
        }

        /// <summary>
        /// Read GUI configuration parameters from INI file.
        /// </summary>
        void ReadConfigParam()
        {
            GUI_ParamFile ^ paramFile = nullptr;
            try
            {
                // open param file for read
                paramFile = gcnew GUI_ParamFile(INI_FILE_NAME, false);
                // read file header
                String ^ fileHeader;
                paramFile->ReadWriteString(fileHeader);
                // check file header
                if (fileHeader != INI_FILE_HEADER)
                {
                    throw gcnew Exception(L"Bad initialization file header");
                }
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
                // read all parameters to temporary memory
                // read config paramers
                GUI_Param tmpGuiParam;
                tmpGuiParam.ReadWriteFile(paramFile);
                // copy all parameters from temporary memory
                // read config paramers
                guiParam.Copy(%tmpGuiParam);
            }
            catch (...)
            {
            }
            finally
            {
                if (paramFile != nullptr)
                {
                    // close param file
                    delete paramFile;
                }
            }
        }

        /// <summary>
        /// Write GUI configuration parameters to INI file.
        /// </summary>
        void WriteConfigParam()
        {
            GUI_ParamFile ^ paramFile = nullptr;
            try
            {
                // open param file for write
                paramFile = gcnew GUI_ParamFile(INI_FILE_NAME, true);
                // write file header
                String ^ fileHeader = INI_FILE_HEADER;
                paramFile->ReadWriteString(fileHeader);
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
                // write config paramers
                guiParam.ReadWriteFile(paramFile);
            }
            catch (...)
            {
            }
            finally
            {
                if (paramFile != nullptr)
                {
                    // close param file
                    delete paramFile;
                }
            }
        }
    };
    #pragma endregion
}
