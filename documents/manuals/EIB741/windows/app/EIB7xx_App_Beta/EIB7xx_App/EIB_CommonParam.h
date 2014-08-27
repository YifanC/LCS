///
/// \file       EIB_CommonParam.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_CommonParam.h,v 1.1.1.1 2011/08/12 09:23:03 rohn Exp $
/// $Name:  $
/// $Revision: 1.1.1.1 $
/// $State: Exp $
/// $Log: EIB_CommonParam.h,v $
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB parameter classes.
///
/// This classes contain all EIB parameters common to all EIBs and
/// sent to all EIBs during initialization and saved into a project file.
///

#pragma once

#include "FormMain.h"

using namespace System;

namespace EIB7xx_App
{
 	#pragma region "EIB_Common_Param class definition"
    /// <summary>
    /// This class contains the parameters common to all EIBs in the system.
    /// </summary>
    public ref class EIB_Common_Param
    {
    private:
        UInt32                  fileVersion;        ///< parameter version for parameter file

        EIB_OpMode_Param        opMode;             ///< EIB operation mode parameters
        EIB_Display_Param       display;            ///< EIB display parameters
        Crankshaft_Param        crankshaftParam;    ///< crankshaft application parameters

        Crankshaft_Measure ^    crankshaftMeasure;  ///< the crankshaft measurement

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        EIB_Common_Param()
        {
            fileVersion = 2;

            crankshaftMeasure = nullptr;
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Common_Param ^ source)
        {
            opMode.Copy( %source->opMode);
            display.Copy(%source->display);
            crankshaftParam.Copy(%source->crankshaftParam);
        }

        /// <summary>
        /// Compare function.
        /// </summary>
		/// <param name="source">The source parameters.</param>
		/// <param name="checkNoEibParam">If true check parameters not send to EIB.</param>
        bool IsEqual(EIB_Common_Param ^ source, bool checkNoEibParam)
        {
            if (opMode.IsEqual(         %source->opMode)                           == false) return false;
            if (display.IsEqual(        %source->display,         checkNoEibParam) == false) return false;
            if (crankshaftParam.IsEqual(%source->crankshaftParam, checkNoEibParam) == false) return false;
            return true;
        }

        /// <summary>
        /// Read or write parameters from / to file.
        /// </summary>
		/// <param name="paramFile">The EIB parameter file handle.</param>
        void ReadWriteFile(EIB_ParamFile ^ paramFile)
        {
            // read / write parameter version
            UInt32 paramVersion = 0;
            if (paramFile->WriteFile == false)
            {
                // read parameter version
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, 1, fileVersion);
            }
            else
            {
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
            }
            // read / write parameters
            opMode.ReadWriteFile(paramFile);
            display.ReadWriteFile(paramFile);
            // check special versions
            if (   (paramFile->WriteFile == true)
                || (paramVersion > 1))
            {
                crankshaftParam.ReadWriteFile(paramFile);
            }
        }

        /// <summary>
        /// Gets the EIB operation mode parameters.
        /// </summary>
        property EIB_OpMode_Param ^ OpMode
        {
            EIB_OpMode_Param ^ get() { return %opMode; }
        }

        /// <summary>
        /// Gets the EIB display parameters.
        /// </summary>
        property EIB_Display_Param ^ Display
        {
            EIB_Display_Param ^ get() { return %display; }
        }

        /// <summary>
        /// Gets the crankshaft application parameters.
        /// </summary>
        property Crankshaft_Param ^ CrankshaftParam
        {
            Crankshaft_Param ^ get() { return %crankshaftParam; }
        }

        /// <summary>
        /// Gets / sets the crankshaft measurement.
        /// </summary>
        property Crankshaft_Measure ^ CrankshaftMeasure
        {
            Crankshaft_Measure ^ get() { return crankshaftMeasure; }
            void set(Crankshaft_Measure ^ value) { crankshaftMeasure = value; }
        }
    };
	#pragma endregion
}
