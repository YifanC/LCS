///
/// \file       EIB_DataFile.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_DataFile.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: EIB_DataFile.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB file classes.
///
/// This classes are used for writing the EIB measure data
/// to a csv type data file.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::IO;
using namespace System::Threading;

namespace EIB7xx_App
{
 	#pragma region "EIB_DataFile class definition"
    // special definitions
    #define DATA_FILE_EXT           L"csv"
    #define DATA_FILE_HEADER        L"EIB 7xx Application"
    #define DATA_FILE_NEW_ENTRY     L"\t"
    #define DATA_FILE_NEW_LINE      L"\r\n"
    #define DATA_FILE_ENCODING      gcnew UnicodeEncoding

    /// <summary>
    /// This class manages a csv type data file used for writing
    /// the EIB measure data.
    /// </summary>
    public ref class EIB_DataFile : public EIB_FileBase
    {
    private:
        Mutex ^         fileMutex;      ///< mutex for write file

        StreamWriter ^  streamWriter;   ///< the writer for text data

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_DataFile()
        {
            // create file write mutex
            fileMutex = gcnew Mutex;
        }

		/// <summary>
		/// Open data file.
		/// </summary>
	    /// <param name="fileName">Project file name.</param>
		void OpenFile(String ^ fileName)
		{
            streamWriter = nullptr;
            // open data file
            EIB_FileBase::OpenFile(fileName, true);
            // open file for write
            streamWriter = gcnew StreamWriter(fileHandle, DATA_FILE_ENCODING);
        }

		/// <summary>
		/// Close file.
		/// </summary>
		virtual void CloseFile() override
		{
            // close file
            if (streamWriter != nullptr)
            {
                streamWriter->Flush();
                //Debug::Print(String::Format(L"Close streamWriter"));
                streamWriter->Close();
                delete streamWriter;
                streamWriter = nullptr;
            }
            EIB_FileBase::CloseFile();
		}

        /// <summary>
        /// Write string to data file.
        /// </summary>
	    /// <param name="value">The value write to file.</param>
        void WriteString(String ^ value)
        {
            // enter mutex
            fileMutex->WaitOne();
            // write value
            streamWriter->Write(value->ToCharArray());
            // release mutex
            fileMutex->ReleaseMutex();
        }
    };
}
