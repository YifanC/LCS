///
/// \file       EIB_FileBase.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_FileBase.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: EIB_FileBase.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB file classes.
///
/// This class is used as a base class for reading and writing
/// from or to a file.
///

#pragma once

using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::Runtime::Serialization;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Threading;

namespace EIB7xx_App
{
 	#pragma region "EIB_FileBase class definition"
    // special definitions
    #define EIB_FILE_FILTER         L"{1} (*.{0})|*.{0}|{2} (*.*)|*.*"

    /// <summary>
    /// This class is used as a base class for reading and writing
    /// from or to a file.
    /// </summary>
    public ref class EIB_FileBase
    {
    protected:
        String ^            fileName;       ///< the file name
        bool                writeFile;      ///< the file read / write flag

        FileStream ^        fileHandle;     ///< the file handle

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_FileBase()
        {
            fileHandle = nullptr;
        }

    protected:
		/// <summary>
		/// Destructor.
		/// </summary>
		~EIB_FileBase()
		{
            // close file
            CloseFile();
		}

		/// <summary>
		/// Finalizer.
		/// </summary>
		!EIB_FileBase()
		{
            // close file
            CloseFile();
		}

    public:
        /// <summary>
        /// Gets the open state of the file.
        /// </summary>
        property bool IsOpen
        {
            bool get() { return (fileHandle != nullptr); }
        }

        /// <summary>
        /// Gets the read / write state of the file.
        /// </summary>
        property bool WriteFile
        {
            bool get() { return writeFile; }
        }

		/// <summary>
		/// Open file.
		/// </summary>
	    /// <param name="fileName">Project file name.</param>
	    /// <param name="writeFile">If true then write to file else read from file.</param>
		virtual void OpenFile(String ^ fileName, bool writeFile)
		{
            this->fileName  = fileName;
            this->writeFile = writeFile;
            fileHandle      = nullptr;
            // open file
            if (writeFile == false)
            {
                // open file for read
                fileHandle = gcnew FileStream(fileName, FileMode::Open);
            }
            else
            {
                // open file for write
                fileHandle = gcnew FileStream(fileName, FileMode::Create);
            }
            //Debug::Print(String::Format(L"Filename: {0}", fileHandle->Name));
        }

		/// <summary>
		/// Close file.
		/// </summary>
		virtual void CloseFile()
		{
            // close file
            if (fileHandle != nullptr)
            {
                //Debug::Print(String::Format(L"Close file: {0}", fileHandle->Name));
                fileHandle->Close();
                delete fileHandle;
                fileHandle = nullptr;
            }
		}
    };
    #pragma endregion
}
