///
/// \file       EIB_BinFileBase.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_BinFileBase.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: EIB_BinFileBase.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB file classes.
///
/// This class is used as a base class for reading and writing
/// parameters from or to a binary file.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::Runtime::Serialization;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;

namespace EIB7xx_App
{
 	#pragma region "EIB_FileException class definition"
    /// <summary>
    /// This class provides an exception
    /// for the EIB file functions.
    /// </summary>
    [Serializable]
    public ref class EIB_FileException : public Exception
    {
    private:
        String ^        fileName;       ///< the file name

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="message">The error message.</param>
		/// <param name="fileName">The file name.</param>
        EIB_FileException(String ^ message, String ^ fileName)
            : Exception(message)
        {
            this->fileName = String::Copy(fileName);
        }
        
    protected:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_FileException(SerializationInfo ^ info, StreamingContext context)
            : Exception(info, context)
        {
        }

        /// <summary>
        /// Gets the file name.
        /// </summary>
        property String ^ FileName
        {
            String ^ get() { return fileName; }
        }
    };
	#pragma endregion

 	#pragma region "EIB_BinFileBase class definition"
    /// <summary>
    /// This class is used as a base class for reading and writing
    /// parameters from or to a binary file.
    /// </summary>
    public ref class EIB_BinFileBase : public EIB_FileBase
    {
    protected:
        BinaryReader ^      binReader;      ///< the reader for binary data
        BinaryWriter ^      binWriter;      ///< the writer for binary data

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
	    /// <param name="fileName">Project file name.</param>
	    /// <param name="writeFile">If true then write to file else read from file.</param>
        EIB_BinFileBase(String ^ fileName, bool writeFile)
        {
            if (fileName != nullptr)
            {
                OpenFile(fileName, writeFile);
            }
        }

    public:
		/// <summary>
		/// Open binary file.
		/// </summary>
	    /// <param name="fileName">Project file name.</param>
	    /// <param name="writeFile">If true then write to file else read from file.</param>
		virtual void OpenFile(String ^ fileName, bool writeFile) override
		{
            binReader = nullptr;
            binWriter = nullptr;
            // open parameter file
            EIB_FileBase::OpenFile(fileName, writeFile);
            // open parameter file
            if (writeFile == false)
            {
                // open file for read
                binReader = gcnew BinaryReader(fileHandle);
            }
            else
            {
                // open file for write
                binWriter = gcnew BinaryWriter(fileHandle);
            }
        }

		/// <summary>
		/// Close file.
		/// </summary>
		virtual void CloseFile() override
		{
            // close file
            if (binReader != nullptr)
            {
                //Debug::Print(String::Format(L"Close binReader"));
                binReader->Close();
                delete binReader;
                binReader = nullptr;
            }
            if (binWriter != nullptr)
            {
                binWriter->Flush();
                //Debug::Print(String::Format(L"Close binWriter"));
                binWriter->Close();
                delete binWriter;
                binWriter = nullptr;
            }
            EIB_FileBase::CloseFile();
		}

        /// <summary>
        /// Read or write UInt32 value from / to file.
        /// </summary>
	    /// <param name="value">The value read from / write to file.</param>
        /// <returns>The value read from / written to file.</returns>
        UInt32 ReadWriteUInt32(UInt32 % value)
        {
            if (writeFile == false)
            {
                value = binReader->ReadUInt32();
            }
            else
            {
                // write value
                binWriter->Write(value);
            }
            return value;
        }

        /// <summary>
        /// Read or write Double value from / to file.
        /// </summary>
	    /// <param name="value">The value read from / write to file.</param>
        /// <returns>The value read from / written to file.</returns>
        Double ReadWriteDouble(Double % value)
        {
            if (writeFile == false)
            {
                value = binReader->ReadDouble();
            }
            else
            {
                // write value
                binWriter->Write(value);
            }
            return value;
        }

        /// <summary>
        /// Read or write bool value from / to file.
        /// </summary>
	    /// <param name="value">The value read from / write to file.</param>
        /// <returns>The value read from / written to file.</returns>
        bool ReadWriteBool(bool % value)
        {
            if (writeFile == false)
            {
                value = binReader->ReadBoolean();
            }
            else
            {
                // write value
                binWriter->Write(value);
            }
            return value;
        }

        /// <summary>
        /// Read or write string from / to file.
        /// </summary>
	    /// <param name="value">The value read from / write to file.</param>
        /// <returns>The value read from / written to file.</returns>
        String ^ ReadWriteString(String ^ % value)
        {
            if (writeFile == false)
            {
                value = binReader->ReadString();
            }
            else
            {
                // write value
                binWriter->Write(value);
            }
            return value;
        }
    };
    #pragma endregion
}
