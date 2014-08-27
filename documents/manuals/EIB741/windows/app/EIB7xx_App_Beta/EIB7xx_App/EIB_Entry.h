///
/// \file       EIB_Entry.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_Entry.h,v 1.2 2011/09/20 12:24:35 rohn Exp $
/// $Name:  $
/// $Revision: 1.2 $
/// $State: Exp $
/// $Log: EIB_Entry.h,v $
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB list entry classes.
///
/// This classes are used for the management of the EIBs added to the system
/// via the Add EIB button or the Open Projekt menu entry.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;

namespace EIB7xx_App
{
    #pragma region "EIB_Entry class definition"
    /// special definitions
    #define EIB_WAIT_DATA_TIMEOUT_MS         200    ///< wait for data timeout in ms

    /// EIB read buffer structure
    typedef struct
    {
        UInt32  dataCount;                          ///< counter for data aquisition
        char    buffer[EIB_DATA_PACKET_MAX_LENGTH]; ///< read buffer for EIB DLL function calls
    }
    RawBuffer;

    /// EIB axis list
    typedef array<EIB_EnDat_Read ^>     EnDatReadList;
    typedef array<PropertyAxis ^>       PropertyAxisList;
    typedef array<PropertyAuxAxis ^>    PropertyAuxAxisList;

    /// <summary>
    /// This class manages the EIBs added to the system
    /// via the Add EIB button or the Open Projekt menu entry.
    /// These EIBs are saved as instances of this class
    /// as the tag of the appropriate tree node of the Main Tree View.
    /// </summary>
    public ref class EIB_Entry
    {
    private:
        GUI_Support ^       guiSupport;     ///< the GUI support class
        ResourceManager ^   resManager;     ///< the resource manager from main window

        EIB_Param           parameter;      ///< parameters of the saved EIB
        EIB_Network_Read ^  networkRead;    ///< network parameters read from the saved EIB
        EIB_Network_Read ^  defaultRead;    ///< default parameters read from the saved EIB
        EnDatReadList ^     enDatRead;      ///< EnDat parameters read from the saved EIB
        EIB_Ctrl ^          control;        ///< EIB control class

        EIB_Common_Param ^  common;         ///< parameters common to all EIBs in the system

        PropertyNetwork ^       propertyNetwork;    ///< network parameter input
        PropertyTrigger ^       propertyTrigger;    ///< trigger parameter input
        PropertyAxisList ^      propertyAxis;       ///< axis parameter input
        PropertyAuxAxisList ^   propertyAuxAxis;    ///< auxiliary axis parameter input

        UInt32      eibIndex;               ///< the EIB tree index

        UInt32      axisNumber;             ///< EIB number of axis
        UInt32      auxAxisNumber;          ///< EIB number of auxiliary axis

        UInt32      triggerSources;         ///< all currently activated trigger sources

        bool        axisNumberWarning;      ///< warning in case of different axis numbers
        bool        auxAxisNumberWarning;   ///< warning in case of different auxiliary axis numbers

        DataPacketBuffer ^  readBuffer;     ///< data packet read buffer
        DataPacketBuffer ^  displayBuffer;  ///< data packet display buffer
        DataPacketBuffer ^  fileBuffer;     ///< data packet file buffer
        WarningFlags        warningFlags;   ///< measurement warning flags
        EIB_DataFile ^      dataFile;       ///< the measure data file

        RawBuffer *         rawBuffer;          ///< read buffer for EIB DLL function calls
        Mutex ^             bufferMutex;        ///< mutex for read buffer

        UInt16              firstTriggerCnt;    ///< first trigger counter value
        UInt16              lastTriggerCnt;     ///< last trigger counter value
        UInt32              highTriggerCnt;     ///< trigger counter high value
        Int32               diffTriggerCnt;     ///< trigger counter difference value

        Exception ^         onDataError;        ///< exception for data aquisition
        UInt32              onDataCount;        ///< counter for data aquisition
        UInt64              onDataTimeout;      ///< counter for data aquisition timeout

        bool                onDataRunning;      ///< running flag for data aquisition thread
        bool                onDataStop;         ///< stop flag for data aquisition thread
        AutoResetEvent ^    onDataEnd;          ///< data aquisition thread end event

        UInt32              recSaveEntries;     ///< entry number for saving record data        
        Double              recSaveProgrMin;    ///< progress min value for saving record data
        Double              recSaveProgrMax;    ///< progress max value for saving record data
        Exception ^         recSaveError;       ///< exception for saving record data
        AutoResetEvent ^    recSaveEnd;         ///< saving record data thread end event


    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="guiSupport">The GUI support class.</param>
        /// <param name="common">The parameters common to all EIBs in the system.</param>
        /// <param name="dataFile">The measure data file.</param>
        EIB_Entry(GUI_Support ^      guiSupport,
                  EIB_Common_Param ^ common,
                  EIB_DataFile ^     dataFile)
        {
            this->guiSupport = guiSupport;
            this->resManager = guiSupport->ResManager;
            this->common     = common;
            this->dataFile   = dataFile;

            networkRead = gcnew EIB_Network_Read(guiSupport);
            defaultRead = gcnew EIB_Network_Read(guiSupport);

            enDatRead = gcnew EnDatReadList(EIB_MAX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                enDatRead[i] = gcnew EIB_EnDat_Read;
            }

            control = gcnew EIB_Ctrl(guiSupport, %parameter);

            propertyNetwork = gcnew PropertyNetwork(guiSupport, %parameter,
                                                    networkRead, defaultRead,
                                                    control);
            propertyTrigger = gcnew PropertyTrigger(guiSupport, %parameter);

            propertyAxis = gcnew PropertyAxisList(EIB_MAX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AXIS_NUMBER; i ++)
            {
                propertyAxis[i] = gcnew PropertyAxis(guiSupport, %parameter, parameter.Axis[i]);
            }

            propertyAuxAxis = gcnew PropertyAuxAxisList(EIB_MAX_AUX_AXIS_NUMBER);
            for (UInt32 i = 0; i < EIB_MAX_AUX_AXIS_NUMBER; i ++)
            {
                propertyAuxAxis[i] = gcnew PropertyAuxAxis(guiSupport, parameter.AuxAxis[i]);
            }

            axisNumber    = 0;
            auxAxisNumber = 0;

            triggerSources = 0;

            axisNumberWarning    = false;
            auxAxisNumberWarning = false;

            // initialize data aquisition buffers
            readBuffer    = gcnew DataPacketBuffer(EIB_READ_BUFFER_NUMBER);
            displayBuffer = gcnew DataPacketBuffer(1);
            fileBuffer    = gcnew DataPacketBuffer(1);

            rawBuffer = new RawBuffer;

            // initialize mutex for read buffer
            bufferMutex = gcnew Mutex;

            // initialize event for data aquisition thread end
            onDataEnd = gcnew AutoResetEvent(false);

            // initialize event for saving record data thread end
            recSaveEnd = gcnew AutoResetEvent(false);
        }

    protected:
        /// <summary>
        /// Destructor.
        /// </summary>
        ~EIB_Entry()
        {
            // close EIB entry
            Delete();
        }

        /// <summary>
        /// Finalizer.
        /// </summary>
        !EIB_Entry()
        {
            // close EIB entry
            Delete();
        }

    private:
        /// <summary>
        /// Close EIB entry.
        /// </summary>
        void Delete()
        {
            // close EIB
            delete control;
            // close data buffers
            if (readBuffer != nullptr)
            {
                delete readBuffer;
                readBuffer = nullptr;
            }
            if (displayBuffer != nullptr)
            {
                delete displayBuffer;
                displayBuffer = nullptr;
            }
            if (fileBuffer != nullptr)
            {
                delete fileBuffer;
                fileBuffer = nullptr;
            }
            if (rawBuffer != nullptr)
            {
                delete rawBuffer;
                rawBuffer = nullptr;
            }
        }

    public:

    #pragma region "Common functions / Properties"
        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_Entry ^ source)
        {
            parameter.Copy(%source->parameter);
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_Entry ^ source)
        {
            if (parameter.IsEqual(%source->parameter) == false) return false;
            return true;
        }

        /// <summary>
        /// Gets the EIB network parameter input.
        /// </summary>
        property PropertyNetwork ^ ThePropertyNetwork
        {
            PropertyNetwork ^ get() { return propertyNetwork; }
        }

        /// <summary>
        /// Gets the EIB trigger parameter input.
        /// </summary>
        property PropertyTrigger ^ ThePropertyTrigger
        {
            PropertyTrigger ^ get() { return propertyTrigger; }
        }

        /// <summary>
        /// Gets the parameters of the saved EIB.
        /// </summary>
        property EIB_Param ^ Parameter
        {
            EIB_Param ^ get() { return %parameter; }
        }

        /// <summary>
        /// Gets the network parameters read from the saved EIB.
        /// </summary>
        property EIB_Network_Read ^ NetworkRead
        {
            EIB_Network_Read ^ get() { return networkRead; }
        }

        /// <summary>
        /// Gets the default parameters read from the saved EIB.
        /// </summary>
        property EIB_Network_Read ^ DefaultRead
        {
            EIB_Network_Read ^ get() { return defaultRead; }
        }

        /// <summary>
        /// Gets the EnDat parameters read from the saved EIB.
        /// </summary>
        property EIB_EnDat_Read ^ EnDatRead[UInt32]
        {
            EIB_EnDat_Read ^ get(UInt32 index) { return enDatRead[index]; }
        }

        /// <summary>
        /// Gets the display buffer of the saved EIB.
        /// </summary>
        property DataPacketBuffer ^ DisplayBuffer
        {
            DataPacketBuffer ^ get() { return displayBuffer; }
        }

        /// <summary>
        /// Gets the measurement warning flags of the saved EIB.
        /// </summary>
        property WarningFlags ^ WarningFlagValues
        {
            WarningFlags ^ get() { return %warningFlags; }
        }

        /// <summary>
        /// Gets the EIB and axis name.
        /// </summary>
        property String ^ EibAxisName[Int32]
        {
            String ^ get(Int32 axisIndex)
            {
                return control->EibAxisName[axisIndex];
            }
        }

        /// <summary>
        /// Gets an EIB axis name.
        /// </summary>
        property String ^ AxisName[Int32]
        {
            String ^ get(Int32 axisIndex)
            {
                return control->AxisName[axisIndex];
            }
        }

        /// <summary>
        /// Refresh the axis parameter property grid.
        /// </summary>
        /// <param name="axisIndex">The axis index of the EIB selected in the Main Tree View.</param>
        /// <param name="isAuxAxis">The axis state of the EIB selected in the Main Tree View.</param>
        void RefreshAxisParam(UInt32 axisIndex, bool isAuxAxis)
        {
            if (isAuxAxis == false)
            {
                propertyAxis[axisIndex]->RefreshParam();
                if (Parameter->Axis[axisIndex]->AuxAxis != nullptr)
                {
                    propertyAuxAxis[axisIndex]->RefreshParam(false);
                }
            }
            else
            {
                propertyAuxAxis[axisIndex]->RefreshParam();
            }
        }
    #pragma endregion

    #pragma region "EIB control functions"
        /// <summary>
        /// Check EIB open.
        /// </summary>
        /// <returns>True if EIB is open.</returns>
        bool CheckEibOpen()
        {
            return control->EIB_CheckOpen();
        }

        /// <summary>
        /// Open EIB.
        /// </summary>
        void OpenEib()
        {
            // open EIB
            control->EIB_Open(Parameter, axisNumber, auxAxisNumber);
            // check axis number
            if (   (axisNumber < Parameter->AxisNumber)
                && (axisNumberWarning == false))
            {
                String ^ message = String::Format(resManager->GetString(L"warningAxisNumber"),
                                                  Parameter->EibName,
                                                  axisNumber,
                                                  Parameter->AxisNumber);
                guiSupport->MsgBoxWarning(message,
                                          MessageBoxButtons::OK,
                                          MessageBoxDefaultButton::Button1);
                axisNumberWarning = true;
            }
            // check auxiliary axis number
            if (   (auxAxisNumber < Parameter->AuxAxisNumber)
                && (auxAxisNumberWarning == false))
            {
                String ^ message = String::Format(resManager->GetString(L"warningAuxAxisNumber"),
                                                  Parameter->EibName,
                                                  auxAxisNumber,
                                                  Parameter->AuxAxisNumber);
                guiSupport->MsgBoxWarning(message,
                                          MessageBoxButtons::OK,
                                          MessageBoxDefaultButton::Button1);
                auxAxisNumberWarning = true;
            }
        }

        /// <summary>
        /// Reset EIB.
        /// </summary>
        void ResetEib()
        {
            // open EIB
            OpenEib();
            // reset EIB
            control->EIB_Reset(Parameter, axisNumber, auxAxisNumber);
        }

        /// <summary>
        /// Update EIB firmware.
        /// </summary>
	    /// <param name="fileName">Firmware file name.</param>
        void UpdateEib(String ^ fileName)
        {
            // open EIB
            OpenEib();
            // update EIB firmware
            control->EIB_Update(fileName, Parameter);
        }
    #pragma endregion

    #pragma region "Network parameter functions"
        /// <summary>
        /// Read network parameters from EIB.
        /// </summary>
        void ReadNetworkParam()
        {
            // open EIB
            OpenEib();
            // read EIB network parameters
            control->EIB_ReadNetworkParam(NetworkRead);
            // read EIB info parameters
            control->EIB_ReadInfoParam(NetworkRead);
            // set default values
            DefaultRead->Copy(NetworkRead);
            // the EIB network read parameters are valid now
            NetworkRead->ReadValid = true;
        }

        /// <summary>
        /// Write network parameters to EIB.
        /// </summary>
        void WriteNetworkParam()
        {
            // open EIB
            OpenEib();
            // write EIB network parameters
            control->EIB_WriteNetworkParam(NetworkRead);
            // set default values
            DefaultRead->Copy(NetworkRead);
        }
    #pragma endregion

    #pragma region "Initialization functions"
        /// <summary>
        /// Initialize EIB(s).
        /// </summary>
        void InitializeEIB()
        {
            // open EIB
            OpenEib();
            // initialize EIB parameters
            InitializeParam();
        }

        /// <summary>
        /// Initialize EIB parameters.
        /// </summary>
        void InitializeParam()
        {
            // set EIB operation mode to polling for initialization
            control->EIB_WriteOpMode(EIB7_MODE_OPERATING::EIB7_OM_Polling);
            // disable trigger sources
            control->EIB_GlobalTriggerEnable(false, UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_All));
            // write axis parameters
            bool isEnabled;
            bool allDisabled = true;
            UInt32 maxAxis = Math::Min(axisNumber, Parameter->AxisNumber);
            for (UInt32 axis = 0; axis < maxAxis; axis ++)
            {
                // write axis parameters
                isEnabled = control->EIB_WriteAxisParam(axis, Parameter->Axis[axis]);
                if (isEnabled == true)
                {
                    allDisabled = false;
                }
                // write EnDat additional information
                warningFlags.Axis[axis]->EnDatAI1Pos2Set =
                                control->EIB_WriteAddInfoParam(axis, Parameter->Axis[axis]);
                // stop reference run
                AxisStartRef(axis, false, false);
                // clear axis
                AxisClear(axis, false);
                // read EnDat encoder parameters
                control->EIB_ReadEnDatParam(axis, Parameter->Axis[axis], EnDatRead[axis]);
                // calculate EnDat 01 reference position
                if (   Parameter->Axis[axis]->InterfaceTypeEib
                    == UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDat01))
                {
                    EnDatRead[axis]->EnDat01RefPosData->Position = Double(EnDatRead[axis]->EnDat01RefPosEnDat);
                    UInt32 errorFlag = GetEnDatUnitPosValue(EnDatRead[axis],
                                                            EnDatRead[axis]->EnDat01RefPosData,
                                                            EnDatRead[axis]->EnDat01RefPosEnDat,
                                                            warningFlags.Axis[axis], axis + 1);
                    if (errorFlag != 0)
                    {
                        EnDatRead[axis]->EnDat01RefPosValid = false;
                    }
                    //Debug::Print(String::Format(L"E01posEnDat: {0:F3}{1}",
                    //                            EnDatRead[axis]->EnDat01RefPosData->Position,
                    //                            EnDatRead[axis]->EnDat01RefPosData->Unit));
                }
            }
            // check if at least one axis is enabled
            if (allDisabled == true)
            {
                // set all axis disabled error exception
                String ^ message = String::Format(resManager->GetString(L"errorAllAxisDisabled"),
                                                  Parameter->EibName);
                throw gcnew EIB_CtrlException(message,
                                              EIB7_ERROR::EIB7_AxisDisabled,
                                              L"InitializeParam",
                                              Parameter->EibName, 0);
            }
            // write auxiliary axis parameters
            UInt32 maxAuxAxis = Math::Min(auxAxisNumber, Parameter->AuxAxisNumber);
            for (UInt32 auxAxis = 0; auxAxis < maxAuxAxis; auxAxis ++)
            {
                // write auxiliary axis parameters
                control->EIB_WriteAuxAxisParam(auxAxis, Parameter->AuxAxis[auxAxis]);
                // stop reference run
                AxisStartRef(auxAxis, true, false);
                // clear axis
                AxisClear(auxAxis, true);
            }
            // write EIB axis termination
            control->EIB_WriteAxisTermination(Parameter);
            // write EIB data packet parameters and enble / disable axis timestamp
            control->EIB_WriteDataPacket(Parameter, maxAxis, maxAuxAxis);
            // initialize trigger parameters
            InitializeTrigger();

            //String ^ text = String::Format(L"Initialize {0}\n", Parameter->EibName);
            //guiSupport->ShowDebugText(text);
        }

        /// <summary>
        /// Initialize EIB trigger parameters.
        /// </summary>
        void InitializeTrigger()
        {
            // write axis parameters
            UInt32 maxAxis = Math::Min(axisNumber, Parameter->AxisNumber);
            for (UInt32 axis = 0; axis < maxAxis; axis ++)
            {
                // write trigger axis parameters
                control->EIB_WriteTriggerAxisParam(axis, Parameter->Trigger);
            }
            // write auxiliary axis parameters
            UInt32 maxAuxAxis = Math::Min(auxAxisNumber, Parameter->AuxAxisNumber);
            for (UInt32 auxAxis = 0; auxAxis < maxAuxAxis; auxAxis ++)
            {
                // write trigger auxiliary axis parameters
                control->EIB_WriteTriggerAuxAxisParam(auxAxis, Parameter->Trigger);
            }
            // write extern trigger input parameters
            UInt32 maxTriggerInp = Parameter->Trigger->TriggerInpNumber;
            for (UInt32 trigger = 0; trigger < maxTriggerInp; trigger ++)
            {
                // write extern trigger input parameters
                control->EIB_WriteTriggerInpParam(trigger, Parameter->Trigger);
            }
            // write extern trigger output parameters
            UInt32 maxTriggerOut = Parameter->Trigger->TriggerOutNumber;
            for (UInt32 trigger = 0; trigger < maxTriggerOut; trigger ++)
            {
                // write extern trigger output parameters
                control->EIB_WriteTriggerOutParam(trigger, Parameter->Trigger);
            }
            // write EIB trigger parameters
            control->EIB_WriteTriggerParam(Parameter->Trigger);
            // get all enabled trigger sources
            triggerSources = GetEnabledTriggers();
        }
    #pragma endregion

    #pragma region "Trigger functions"
        /// <summary>
        /// Get all enabled axis trigger sources.
        /// </summary>
        /// <param name="axis">The axis index.</param>
        /// <param name="axisTriggerSource">The axis trigger sources.</param>
        /// <param name="triggerSources">All enabled axis trigger sources.</param>
        /// <returns>All enabled axis trigger sources.</returns>
        void GetEnabledAxisTriggers(UInt32 axis,
                                    EIB7_AXIS_TRIGGER_SOURCE axisTriggerSource,
                                    UInt32 % triggerSources)
        {
            switch (axisTriggerSource)
            {
            default: throw gcnew Exception(L"Internal error 6"); break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgInput1:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput1);
                break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgInput2:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput2);
                break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgInput3:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput3);
                break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgInput4:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput4);
                break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgRI:
                switch (axis)
                {
                default: throw gcnew Exception(L"Internal error 5"); break;
                case 0:
                    triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRI1);
                    break;
                case 1:
                    triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRI2);
                    break;
                case 2:
                    triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRI3);
                    break;
                case 3:
                    triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRI4);
                    break;
                }
                break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgRImaskedCH1:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRImaskedCH1);
                break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgIC:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgIC);
                break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgPuls:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgPuls);
                break;
            case EIB7_AXIS_TRIGGER_SOURCE::EIB7_AT_TrgTimer:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgTimer);
                break;
            }
        }

        /// <summary>
        /// Get all enabled trigger sources.
        /// </summary>
        /// <returns>All enabled trigger sources.</returns>
        UInt32 GetEnabledTriggers()
        {
            UInt32 triggerSources = 0;
            // check extern trigger output sources
            UInt32 maxTriggerOut = Parameter->Trigger->TriggerOutNumber;
            for (UInt32 trigger = 0; trigger < maxTriggerOut; trigger ++)
            {
                if (Parameter->Trigger->TriggerOutEnable[trigger] == EIB7_ENABLE_MODE::EIB7_MD_Disable)
                {
                    continue;
                }
                switch (Parameter->Trigger->TriggerOutSource[trigger])
                {
                default: throw gcnew Exception(L"Internal error 8"); break;
                case EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgInSync:
                case EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgInAsync:
                    switch (trigger)
                    {
                    default: throw gcnew Exception(L"Internal error 7"); break;
                    case 0:
                        triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput1);
                        break;
                    case 1:
                        triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput2);
                        break;
                    case 2:
                        triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput3);
                        break;
                    case 3:
                        triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput4);
                        break;
                    }
                    break;
                case EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgRImaskedCH1:
                    triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRImaskedCH1);
                    break;
                case EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgIC:
                    triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgIC);
                    break;
                case EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgPuls:
                    triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgPuls);
                    break;
                case EIB7_TRIGGER_OUT_SOURCE::EIB7_OT_TrgTimer:
                    triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgTimer);
                    break;
                }
            }
            // check axis trigger sources
            UInt32 maxAxis = Math::Min(axisNumber, Parameter->AxisNumber);
            for (UInt32 axis = 0; axis < maxAxis; axis ++)
            {
                if (Parameter->Axis[axis]->IsEnabled == false)
                {
                    continue;
                }
                GetEnabledAxisTriggers(axis,
                                       Parameter->Trigger->AxisTriggerSource[axis],
                                       triggerSources);
            }
            // check auxiliary axis trigger sources
            UInt32 maxAuxAxis = Math::Min(auxAxisNumber, Parameter->AuxAxisNumber);
            for (UInt32 auxAxis = 0; auxAxis < maxAuxAxis; auxAxis ++)
            {
                if (Parameter->AuxAxis[auxAxis]->IsEnabled == false)
                {
                    continue;
                }
                GetEnabledAxisTriggers(auxAxis,
                                       Parameter->Trigger->AuxAxisTriggerSource[auxAxis],
                                       triggerSources);
            }
            // check master trigger sources
            switch (Parameter->Trigger->MasterTriggerSource)
            {
            default: throw gcnew Exception(L"Internal error 9"); break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgInput1:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput1);
                break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgInput2:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput2);
                break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgInput3:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput3);
                break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgInput4:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput4);
                break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgRI:
                // this value will be set from axis trigger source
                //triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRIx);
                break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgRImaskedCH1:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRImaskedCH1);
                break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgIC:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgIC);
                break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgPuls:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgPuls);
                break;
            case EIB7_MASTER_TRIGGER_SOURCE::EIB7_AT_TrgTimer:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgTimer);
                break;
            }
            // check pulse counter start trigger sources
            switch (Parameter->Trigger->PulseCounterStart)
            {
            default: throw gcnew Exception(L"Internal error 10"); break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgInput1:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput1);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgInput2:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput2);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgInput3:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput3);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgInput4:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgInput4);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgRI1:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRI1);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgRI2:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRI2);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgRI3:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRI3);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgRI4:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRI4);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgRImaskedCH1:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgRImaskedCH1);
                break;
            case EIB7_PULSE_COUNTER_START::EIB7_PS_TrgIC:
                triggerSources |= UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_TrgIC);
                break;
            }
            //String ^ debug = String::Format(L"TriggerSources: 0x{0,8:X8}", triggerSources);
            //Debug::Print(debug);
            return triggerSources;
        }
    #pragma endregion

    #pragma region "Measurement control functions"
        /// <summary>
        /// Start EIB measurement.
        /// </summary>
        /// <param name="eibIndex">The EIB index.</param>
        void StartEIBMeasurement(UInt32 eibIndex)
        {
            this->eibIndex = eibIndex;
            UInt32 maxAxis    = Math::Min(axisNumber,    Parameter->AxisNumber);
            UInt32 maxAuxAxis = Math::Min(auxAxisNumber, Parameter->AuxAxisNumber);
            // open EIB
            OpenEib();
            // clear display buffer
            displayBuffer->Clear();
            // clear file buffer
            fileBuffer->Clear();
            // clear warning flags
            warningFlags.Clear();
            // reinitialize trigger parameters because reading polling data
            // could have changed trigger parameters
            InitializeTrigger();
            // JH_110719: initialize EIB parameters
            //InitializeParam();
            // clear the system for start
            control->EIB_ClearForStart(maxAxis, maxAuxAxis);
            // write EIB timestamp period and reset timestamp counter
            control->EIB_WriteTimestamp(common->OpMode);
            // enable data aquisition
            DataAquisitionEnable(true);
            // write EIB operation mode parameters
            control->EIB_WriteOpModeParam(common->OpMode);
            // reset lost trigger error
            for (UInt32 axis = 0; axis < maxAxis; axis ++)
            {
                AxisResetError(axis, false, DataPacketCmd::ResetErrorLostTrigger);
            }
            for (UInt32 auxAxis = 0; auxAxis < maxAuxAxis; auxAxis ++)
            {
                AxisResetError(auxAxis, true, DataPacketCmd::ResetErrorLostTrigger);
            }
            // enable trigger sources
            if (common->OpMode->OpMode != EIB7_MODE_OPERATING::EIB7_OM_Polling)
            {
                control->EIB_GlobalTriggerEnable(true, triggerSources);
            }
            //Debug::Print(String::Format(L"Start {0} Measurement", Parameter->EibName));
        }

        /// <summary>
        /// Stop EIB measurement.
        /// </summary>
        /// <returns>False if connection to EIB was closed.</returns>
        bool StopEIBMeasurement()
        {
            // disable data aquisition
            DataAquisitionEnable(false);
            // check connection to EIB
            bool isOpen = CheckEibOpen();
            if (isOpen == true)
            {
                // disable trigger sources
                control->EIB_GlobalTriggerEnable(false, UInt32(EIB7_TRIGGER_SOURCE::EIB7_TS_All));
                // set EIB operation mode to polling
                control->EIB_WriteOpMode(EIB7_MODE_OPERATING::EIB7_OM_Polling);
                // set axis state
                UInt32 maxAxis = Math::Min(axisNumber, Parameter->AxisNumber);
                for (UInt32 axis = 0; axis < maxAxis; axis ++)
                {
                    // stop reference run
                    AxisStartRef(axis, false, false);
                }
                // set auxiliary axis state
                UInt32 maxAuxAxis = Math::Min(auxAxisNumber, Parameter->AuxAxisNumber);
                for (UInt32 auxAxis = 0; auxAxis < maxAuxAxis; auxAxis ++)
                {
                    // stop reference run
                    AxisStartRef(auxAxis, true, false);
                }
            }
            //Debug::Print(String::Format(L"Stop {0} Measurement", Parameter->EibName));
            return isOpen;
        }

        /// <summary>
        /// Check for stop measurement.
        /// </summary>
        /// <returns">True if stop conditions are met.</returns>
        bool CheckForStopMeasurement()
        {
            return onDataStop;
        }

        /// <summary>
        /// Enable data aquisition for soft realtime or streaming.
        /// </summary>
        /// <param name="enable">If true enable callback function else disable.</param>
        void DataAquisitionEnable(bool enable)
        {
            // initialize data aquisition
            if (enable == true)
            {
                readBuffer->WriteIndex = 0;
                onDataCount            = 0;
                onDataTimeout          = 0;
                onDataError            = nullptr;
                onDataRunning          = true;
                onDataStop             = false;
                highTriggerCnt         = 0;
                diffTriggerCnt         = 0;
            }
            else
            {
                onDataRunning          = false;
            }
            if (   (common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_SoftRealtime)
                || (common->OpMode->OpMode == EIB7_MODE_OPERATING::EIB7_OM_Streaming))
            {
                if (enable == true)
                {
                    // check buffer size
                    UInt32 size = control->EIB_ReadDataPacketSize();
                    if (size > EIB_DATA_PACKET_MAX_LENGTH)
                    {
                        // set data packet too large error exception
                        String ^ message = String::Format(resManager->GetString(L"errorDataPacketTooLarge"),
                                                          size,
                                                          EIB_DATA_PACKET_MAX_LENGTH);
                        throw gcnew EIB_CtrlException(message,
                                                      EIB7_ERROR::EIB7_DataPacketSize,
                                                      L"DataAquisitionEnable",
                                                      Parameter->EibName, 0);
                    }
                }
                bool useCallback = false;
                if (useCallback == false)
                {
                    // initialize data thread
                    if (enable == true)
                    {
                        // initialize data aquisition thread
                        Thread ^ onDataThread = gcnew Thread(
                                                gcnew ThreadStart(this,
                                                                  &EIB7xx_App::EIB_Entry::OnDataThread));
                        // start data thread
                        onDataThread->Start();
                        // wait for thread started
                        Thread::Sleep(100);
                    }
                    else
                    {
                        // wait for thread stopped
                        onDataEnd->WaitOne(5 * EIB_WAIT_DATA_TIMEOUT_MS, false);
                        //Debug::Print(String::Format(L"Trigger count first: 0x{0,4:X4}, last: 0x{1,4:X4}",
                        //                            firstTriggerCnt, lastTriggerCnt));
                        //Debug::Print(String::Format(L"Eib: {0}, read count: {1}, timeout: {2}",
                        //                            Parameter->EibName, onDataCount, onDataTimeout));
                    }
                }
                else
                {
                    // initialize data callback function
                    control->EIB_DataAquisitionEnable(enable, rawBuffer->buffer);
                }
            }
        }

        #pragma warning(push)
        #pragma warning(disable: 4101)
        /// <summary>
        /// Data aquisition thread.
        /// </summary>
        void OnDataThread()
        {
            try
            {
                UInt32 entries;
                UInt16 triggerCnt;
                bool entryFound, stopFlag;
                while (onDataRunning == true)
                {
                    // read data from fifo to read buffer with timeout
                    entries = control->EIB_ReadDataPacketData(rawBuffer->buffer,
                                                              EIB_WAIT_DATA_TIMEOUT_MS);
                    // check for data available
                    if (entries == 0)
                    {
                        onDataTimeout ++;
                    }
                    else
                    {
                        // data had been read from fifo to read buffer
                        // set read counter
                        rawBuffer->dataCount = onDataCount + 1;
                        // TODO: save measure data to file thread
                        // save measure data
                        ReadDataPacket(Parameter, rawBuffer, readBuffer, readBuffer->WriteIndex, dataFile);
                        // read crankshaft measure data
                        stopFlag = common->CrankshaftMeasure->ReadMeasureData(eibIndex, readBuffer);
                        // enter mutex
                        bufferMutex->WaitOne();
                        // increment read buffer write index
                        readBuffer->WriteIndex ++;
                        if (readBuffer->WriteIndex >= EIB_READ_BUFFER_NUMBER)
                        {
                            readBuffer->WriteIndex = 0;
                        }
                        // increment read counter
                        onDataCount ++;
                        // leave mutex
                        bufferMutex->ReleaseMutex();
                        // check for stop
                        if (stopFlag == true)
                        {
                            onDataStop = true;
                        }
                        //Debug::Print(String::Format(L"Data count: {0}", rawBuffer->dataCount));
                    }
                    // wait for fifo data timed out
                    //Debug::Print(String::Format(L"FIFO count: 0x{0,8:X8}", entries));
                }
            }
            catch (AccessViolationException ^ ex)
            {
                // end thread due to an error
                // connection to EIB was closed
                String ^ message = String::Format(resManager->GetString(L"errorConnectionClosed"),
                                                  Parameter->EibName);
                onDataError = gcnew EIB_CtrlException(message,
                                                      EIB7_ERROR::EIB7_ConnectClosed,
                                                      L"OnDataThread",
                                                      Parameter->EibName, 0);
                return;
            }
            catch (Exception ^ ex)
            {
                // end thread due to an error
                onDataError = ex;
                return;
            }
            finally
            {
                // set end event
                onDataEnd->Set();
            }
        }
        #pragma warning(pop)
    #pragma endregion

    #pragma region "Measure data functions"
        /// <summary>
        /// Read and save measure data for polling.
        /// </summary>
        /// <param name="eibIndex">The EIB index.</param>
        /// <param name="running">True if system is in running mode.</param>
        /// <returns">True if stop conditions are met.</returns>
        bool ReadPollingData(UInt32 eibIndex, bool running)
        {
            return ReadPollingData(eibIndex, Parameter, running);
        }

        /// <summary>
        /// Read and save measure data for polling.
        /// </summary>
        /// <param name="eibIndex">The EIB index.</param>
        /// <param name="parameter">The EIB parameters.</param>
        /// <param name="running">True if system is in running mode.</param>
        /// <returns">True if stop conditions are met.</returns>
        bool ReadPollingData(UInt32 eibIndex, EIB_Param ^ parameter, bool running)
        {
            bool stopFlag = false;
            UInt32 maxAxis    = Math::Min(axisNumber,    Parameter->AxisNumber);
            UInt32 maxAuxAxis = Math::Min(auxAxisNumber, Parameter->AuxAxisNumber);
            // read measure data for polling
            control->EIB_ReadPollingData(parameter, displayBuffer, maxAxis, maxAuxAxis);
            // check for running mode
            EIB_DataFile ^ dataFile = nullptr;
            if (running == true)
            {
                dataFile = this->dataFile;
            }
            // save measure data for polling
            ReadDataPacket(parameter, nullptr, displayBuffer, 0, dataFile);
            // check running state
            if (running == true)
            {
                // copy data from display buffer to read buffer
                readBuffer->Copy(readBuffer->WriteIndex, displayBuffer);
                // read crankshaft measure data
                stopFlag = common->CrankshaftMeasure->ReadMeasureData(eibIndex, readBuffer);
                // increment read buffer write index
                readBuffer->WriteIndex ++;
                if (readBuffer->WriteIndex >= EIB_READ_BUFFER_NUMBER)
                {
                    readBuffer->WriteIndex = 0;
                }
            }
            return stopFlag;
        }

        /// <summary>
        /// Read measure data for soft realtime or streaming.
        /// </summary>
        void ReadSoftRtStreamingData()
        {
            // read measure data for soft realtime or streaming
            // check data aquisition error
            if (onDataError != nullptr)
            {
                throw onDataError;
            }
            // check EIB connection
            bool isOpen = control->EIB_CheckOpen();
            if (isOpen == false)
            {
                // connection to EIB was closed
                String ^ message = String::Format(resManager->GetString(L"errorConnectionClosed"),
                                                  Parameter->EibName);
                throw gcnew EIB_CtrlException(message,
                                              EIB7_ERROR::EIB7_ConnectClosed,
                                              L"ReadSoftRtStreamingData",
                                              Parameter->EibName, 0);
            }
            // enter mutex
            bufferMutex->WaitOne();
            // check if data had been read
            if (onDataCount > 0)
            {
                // set read buffer read index
                Int32 readIndex = readBuffer->WriteIndex - 1;
                if (readIndex < 0)
                {
                    readIndex += EIB_READ_BUFFER_NUMBER;
                }
                // copy data from read buffer to display buffer
                displayBuffer->Copy(readBuffer, readIndex);
            }
            // leave mutex
            bufferMutex->ReleaseMutex();
            //Debug::Print(String::Format(L"Data count: 0x{0,8:X8}", onDataCount));
        }

        /// <summary>
        /// Read measure buffer state for recording.
        /// </summary>
        /// <param name="entryNumber">The number of entries in the record buffer.</param>
        /// <returns>The fill level of the record buffer in %.</returns>
        UInt32 ReadRecordingState(UInt32 % entryNumber)
        {
            return control->EIB_ReadRecordingState(entryNumber);
        }

        /// <summary>
        /// Read recording measure data.
        /// </summary>
        /// <param name="dataIndex">The index of the measurement value in recording buffer.</param>
        void ReadRecordingData(UInt32 dataIndex)
        {
            // open read measure data for recording
            control->EIB_OpenRecordingData(dataIndex);
            // initialize parameters
            if (dataIndex == 0)
            {
                highTriggerCnt = 0;
                diffTriggerCnt = 0;
            }
            // read measure data
            ReadRecordingData(dataIndex, displayBuffer, nullptr);
            // close read measure data for recording
            control->EIB_CloseRecordingData();
        }

        /// <summary>
        /// Save recording measure data.
        /// </summary>
        /// <param name="eibIndex">The EIB index.</param>
        /// <param name="eibCount">The EIB number.</param>
        void SaveRecordingData(UInt32 eibIndex, UInt32 eibCount)
        {
            // get number of measured values
            ReadRecordingState(recSaveEntries);
            // initialize parameters
            highTriggerCnt = 0;
            diffTriggerCnt = 0;
            // prepare progress
            recSaveProgrMin = eibIndex       * 100.0 / eibCount;
            recSaveProgrMax = (eibIndex + 1) * 100.0 / eibCount;
            // open read measure data for recording
            control->EIB_OpenRecordingData(-1);
            // check for using thread
            bool useThread = false;
            if (useThread == false)
            {
                // write all measure data
                for (UInt32 i = 0; i < recSaveEntries; i ++)
                {
                    //Debug::Print(String::Format(L"Record index: {0}", i));
                    // read and save measured values
                    ReadRecordingData(i, fileBuffer, dataFile);
                    // show progress
                    UInt32 progress = UInt32(    (recSaveProgrMax - recSaveProgrMin)
                                               * i
                                               / recSaveEntries
                                             + recSaveProgrMin);
                    guiSupport->ShowProgress(progress);
                    // give windows some air...
                    if ((i % 1000) == 0)
                    {
                        Application::DoEvents();
                        guiSupport->RefreshWindow();
                    }
                }
            }
            else
            {
                // initialize thread for saving record data
                recSaveError = nullptr;
                Thread ^ recSaveThread = gcnew Thread(
                                         gcnew ThreadStart(this,
                                                           &EIB7xx_App::EIB_Entry::RecSaveThread));
                // start saving record data thread
                recSaveThread->Start();
                // wait for thread started
                Thread::Sleep(100);
                // wait for saving record data thread stopped
                bool signal;
                for (UInt32 i = 0; i < 60; i ++)
                {
                    signal = recSaveEnd->WaitOne(1000, false);
                    if (signal == true)
                    {
                        break;
                    }
                    // give windows some air...
                    Application::DoEvents();
                    guiSupport->RefreshWindow();
                }
                // check saving record data error
                if (recSaveError != nullptr)
                {
                    throw recSaveError;
                }
            }
            // close read measure data for recording
            control->EIB_CloseRecordingData();
        }

        /// <summary>
        /// Saving record data thread.
        /// </summary>
        void RecSaveThread()
        {
            try
            {
                // write all measure data
                for (UInt32 i = 0; i < recSaveEntries; i ++)
                {
                    // read and save measured values
                    ReadRecordingData(i, fileBuffer, dataFile);
                    // show progress
                    UInt32 progress = UInt32(    (recSaveProgrMax - recSaveProgrMin)
                                               * i
                                               / recSaveEntries
                                             + recSaveProgrMin);
                    guiSupport->ShowProgress(progress);
                    // give windows some air...
                    if ((i % 100) == 0)
                    {
                        //Thread::Sleep(0);
                        Application::DoEvents();
                        guiSupport->RefreshWindow();
                    }
                }
            }
            catch (Exception ^ ex)
            {
                // end thread due to an error
                recSaveError = ex;
                return;
            }
            finally
            {
                // set end event
                recSaveEnd->Set();
            }
        }

        /// <summary>
        /// Read or save recording measure data.
        /// </summary>
        /// <param name="dataIndex">The index of the measurement value in recording buffer.</param>
        /// <param name="dataBuffer">The data packet buffer.</param>
        /// <param name="dataFile">The measure data file.</param>
        void ReadRecordingData(UInt32 dataIndex,
                               DataPacketBuffer ^ dataBuffer,
                               EIB_DataFile ^ dataFile)
        {
            // read measure data
            control->EIB_ReadRecordingData(dataIndex, rawBuffer->buffer);
            //Debug::Print(String::Format(L"Read index: {0}", dataIndex));
            //ENCODER_POSITION llValue;
            //control->EIB_ReadDataPacketValue(rawBuffer->buffer,
            //                                 EIB7_DR_Encoder2, EIB7_PDF_PositionData,
            //                                 &llValue, sizeof(llValue));
            //Debug::Print(String::Format(L"Position data: 0x{0,16:X16}", llValue));
            // set read counter
            rawBuffer->dataCount = dataIndex + 1;
            // set data packet values
            ReadDataPacket(Parameter, rawBuffer, dataBuffer, 0, dataFile);
            //Debug::Print(String::Format(L"Write index: {0}", dataIndex));
        }
    #pragma endregion

    #pragma region "Data packet functions"
        /// <summary>
        /// Convert data packet values from read or copy buffer to display or file buffer.
        /// </summary>
        /// <param name="parameter">The EIB parameters.</param>
        /// <param name="rawBuffer">The data packet read buffer.
        /// If this value is nullptr, the data are already in the data buffer.</param>
        /// <param name="dataBuffer">The data packet data buffer.
        /// If this value is nullptr, the function is used to create the data file header row.</param>
        /// <param name="entryIndex">The index of the data packet entry.</param>
        /// <param name="dataFile">The measure data file.</param>
        void ReadDataPacket(EIB_Param ^ parameter,
                            RawBuffer * rawBuffer,
                            DataPacketBuffer ^ dataBuffer,
                            UInt32 entryIndex,
                            EIB_DataFile ^ dataFile)
        {
            // initialize parameters
            UInt32 maxAxis    = Math::Min(axisNumber,    parameter->AxisNumber);
            UInt32 maxAuxAxis = Math::Min(auxAxisNumber, parameter->AuxAxisNumber);
            // get data packet values
            bool entryFound;
            UInt32 entryNum;
            unsigned short   usValue;
            unsigned short   usArray[2];
            unsigned long    ulValue;
            ENCODER_POSITION llValue;
            ENCODER_POSITION llArray[2];
            // get read and trigger counter
            if (   (rawBuffer != nullptr)
                && (dataBuffer != nullptr))
            {
                // get read counter
                dataBuffer->ReadDataCount = rawBuffer->dataCount;
                //Debug::Print(String::Format(L"Counter read: 0x{0,8:X8}",
                //                            dataBuffer->ReadDataCount));
                // get trigger counter
                entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                              EIB7_DR_Global, EIB7_PDF_TriggerCounter,
                                                              &usValue, sizeof(usValue));
                if (entryFound == false)
                {
                    //Debug::Print(String::Format(L"Trigger counter not found: {0}",
                    //                            dataBuffer->ReadDataCount));
                }
                else
                {
                    // check for first value
                    if (dataBuffer->ReadDataCount == 1)
                    {
                        // set first trigger counter value
                        firstTriggerCnt = usValue;
                    }
                    else if (usValue < lastTriggerCnt)
                    {
                        // set trigger counter high value
                        highTriggerCnt += 0x10000;
                    }
                    // set full trigger counter value
                    dataBuffer->FirstTriggerCount   = firstTriggerCnt;
                    dataBuffer->TriggerCounterValue = highTriggerCnt + usValue;
                    //Debug::Print(String::Format(L"Counter trigger: 0x{0,8:X8}, 0x{1,4:X4}",
                    //                            dataBuffer->TriggerCounterGui, usValue));
                    // check for lost trigger
                    // DEBUG: lost trigger count warning
                    //if (dataBuffer->ReadDataCount >= 100)
                    //{
                    //    dataBuffer->ReadDataCount --;
                    //}
                    if (UInt16(dataBuffer->ReadDataCount) != UInt16(dataBuffer->TriggerCounterGui))
                    {
                        Int32 diffTriggerCnt = dataBuffer->TriggerCounterGui - dataBuffer->ReadDataCount;
                        if (this->diffTriggerCnt != diffTriggerCnt)
                        {
                            //Debug::Print(String::Format(
                            //                L"Lost trigger count: {0}, gui: {1}, new: {2}, last: {3}",
                            //                dataBuffer->ReadDataCount, dataBuffer->TriggerCounterGui,
                            //                usValue, lastTriggerCnt));
                            this->diffTriggerCnt = diffTriggerCnt;
                        }
                        if (warningFlags.WarningLostTriggerCount == false)
                        {
                            // report warning
                            String ^ message = String::Format(resManager->GetString(L"warningLostTriggerCount"),
                                                              Parameter->EibName);
                            guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                        message);
                            // set warning flag
                            warningFlags.WarningLostTriggerCount = true;
                        }
                    }
                    // set last trigger counter value
                    lastTriggerCnt = usValue;
                }
            }
            // write measure data to file
            String ^ lineText = L"";
            String ^ dataText;
            // get axis and auxiliary axis datapacket
            EIB7_DataRegion region;
            DataPacketData ^ dataPacketData;
            UInt32 auxAxis, axisIndex;
            bool isEnabled, isAuxAxis;
            // check for create data file header row
            if (dataBuffer == nullptr)
            {
                maxAxis    = 1;
                maxAuxAxis = 0;
            }
            // loop for all axis
            for (UInt32 axis = 0; axis < maxAxis + maxAuxAxis; axis ++)
            {
                dataText = L"";
                // check for create data file header row
                if (axis < maxAxis)
                {
                    // get axis datapacket
                    axisIndex = axis;
                    isAuxAxis = false;
                    isEnabled = parameter->Axis[axis]->IsEnabled;
                    switch (axis)
                    {
                    default: throw gcnew Exception(L"Internal error 3"); break;
                    case 0 : region = EIB7_DR_Encoder1; break;
                    case 1 : region = EIB7_DR_Encoder2; break;
                    case 2 : region = EIB7_DR_Encoder3; break;
                    case 3 : region = EIB7_DR_Encoder4; break;
                    }
                    if (dataBuffer != nullptr)
                    {
                        dataPacketData = dataBuffer->Entry[entryIndex]->Axis[axis];
                    }
                }
                else
                {
                    // get auxiliary axis datapacket
                    auxAxis   = axis - maxAxis;
                    axisIndex = auxAxis;
                    isAuxAxis = true;
                    isEnabled = parameter->AuxAxis[auxAxis]->IsEnabled;
                    switch (auxAxis)
                    {
                    default: throw gcnew Exception(L"Internal error 4"); break;
                    case 0 : region = EIB7_DR_AUX; break;
                    }
                    if (dataBuffer != nullptr)
                    {
                        dataPacketData = dataBuffer->Entry[entryIndex]->AuxAxis[auxAxis];
                    }
                }
                // check if axis is enabled
                if (   (isEnabled == false)
                    && (   (dataBuffer != nullptr)
                        || (dataFile == nullptr)
                        || (dataFile->IsOpen == false)))
                {
                    continue;
                }
                // axis is enabled or write header text to data file
                // write header data to data file
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        lineText +=   resManager->GetString(L"textDataFileEibName")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataFileAxisName")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataFileIndex")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataFileTriggerCount");
                    }
                    else
                    {
                        lineText +=   parameter->EibName
                                    + DATA_FILE_NEW_ENTRY
                                    + AxisName[axis + 1]
                                    + DATA_FILE_NEW_ENTRY
                                    + dataBuffer->ReadDataCount.ToString()
                                    + DATA_FILE_NEW_ENTRY;

                        if (dataBuffer->TriggerCounterUsed == true)
                        {
                            lineText += dataBuffer->TriggerCounterGui.ToString();
                        }
                    }
                    lineText += DATA_FILE_NEW_ENTRY;
                }
                // clear data packet data
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    dataPacketData->Clear();
                }
                // get axis and auxiliary axis datapacket values
                // get status word
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                                  region, EIB7_PDF_StatusWord,
                                                                  &usValue, sizeof(usValue));
                    if (entryFound == true)
                    {
                        dataPacketData->StatusWordValue = usValue;
                        // DEBUG: set position invalid
                        //if (   (axis == 1)
                        //    && (dataBuffer->ReadDataCount > 100))
                        //{
                        //    dataPacketData->StatusWordValue &= ~(UInt16)DataPacketStatus::PositionValid;
                        //}
                        // DEBUG: set EnDat CRC error
                        //if (   (axis == 1)
                        //    && (dataBuffer->ReadDataCount > 100))
                        //{
                        //    dataPacketData->StatusWordValue |= (UInt16)DataPacketStatus::ErrorEnDatCRC;
                        //}
                        //Debug::Print(String::Format(L"Status word: 0x{0,4:X4}",
                        //                            dataPacketData->StatusWordValue));
                    }
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        dataText += resManager->GetString(L"textDataPacketStatusWord");
                    }
                    else if (dataPacketData->StatusWordUsed == true)
                    {
                        dataText += dataPacketData->StatusWordValue.ToString();
                    }
                    dataText += DATA_FILE_NEW_ENTRY;
                }
                // get position data
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                                  region, EIB7_PDF_PositionData,
                                                                  &llValue, sizeof(llValue));
                    if (entryFound == true)
                    {
                        dataPacketData->PositionDataValue = llValue;
                        //Debug::Print(String::Format(L"Position data: 0x{0,16:X16}",
                        //                            dataPacketData->PositionDataValue));
                    }
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        dataText += resManager->GetString(L"textDataPacketPositionData");
                    }
                    else if (dataPacketData->PositionDataUsed == true)
                    {
                        dataText += dataPacketData->PositionDataValue.ToString();
                    }
                    dataText += DATA_FILE_NEW_ENTRY;
                }
                // get timestamp
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                                  region, EIB7_PDF_Timestamp,
                                                                  &ulValue, sizeof(ulValue));
                    if (entryFound == true)
                    {
                        dataPacketData->TimestampValue = ulValue;
                        //Debug::Print(String::Format(L"Timestamp: 0x{0,8:X8}",
                        //                            dataPacketData->TimestampValue));
                    }
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        dataText += resManager->GetString(L"textDataPacketTimestamp");
                    }
                    else if (dataPacketData->TimestampUsed == true)
                    {
                        dataText += dataPacketData->TimestampValue.ToString();
                    }
                    dataText += DATA_FILE_NEW_ENTRY;
                }
                // get reference position values
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    entryNum = 2;
                    entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                                  region, EIB7_PDF_ReferencePos,
                                                                  llArray, sizeof(ENCODER_POSITION),
                                                                  entryNum);
                    if (entryFound == true)
                    {
                        dataPacketData->ReferencePos1Value = llArray[0];
                        //Debug::Print(String::Format(L"Ref. pos. 1 value: 0x{0,16:X16}",
                        //                            dataPacketData->ReferencePos1Value));
                        if (entryNum == 2)
                        {
                            dataPacketData->ReferencePos2Value = llArray[1];
                            //Debug::Print(String::Format(L"Ref. pos. 2 value: 0x{0,16:X16}",
                            //                            dataPacketData->ReferencePos2Value));
                        }
                    }
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        dataText += resManager->GetString(L"textDataPacketReferencePos1");
                    }
                    else if (dataPacketData->ReferencePos1Used == true)
                    {
                        dataText += dataPacketData->ReferencePos1Value.ToString();
                    }
                    dataText += DATA_FILE_NEW_ENTRY;

                    if (dataBuffer == nullptr)
                    {
                        dataText += resManager->GetString(L"textDataPacketReferencePos2");
                    }
                    else if (dataPacketData->ReferencePos2Used == true)
                    {
                        dataText += dataPacketData->ReferencePos2Value.ToString();
                    }
                    dataText += DATA_FILE_NEW_ENTRY;
                }
                // get distance coded reference position value
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                                  region, EIB7_PDF_DistCodedRef,
                                                                  &llValue, sizeof(llValue));
                    if (entryFound == true)
                    {
                        dataPacketData->DistCodedRefValue = llValue;
                        //Debug::Print(String::Format(L"Dist. coded ref. pos. value: 0x{0,16:X16}",
                        //                            dataPacketData->DistCodedRefValue));
                    }
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        dataText += resManager->GetString(L"textDataPacketDistCodedRef");
                    }
                    else if (dataPacketData->DistCodedRefUsed == true)
                    {
                        dataText += dataPacketData->DistCodedRefValue.ToString();
                    }
                    dataText += DATA_FILE_NEW_ENTRY;
                }
                // get analog signal values
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                                  region, EIB7_PDF_Analog,
                                                                  usArray, sizeof(usArray));
                    if (entryFound == true)
                    {
                        dataPacketData->AnalogAValue = usArray[0];
                        dataPacketData->AnalogBValue = usArray[1];
                        //Debug::Print(String::Format(L"Analog signal values: 0x{0,4:X4}, 0x{1,4:X4}",
                        //                            dataPacketData->AnalogAValue,
                        //                            dataPacketData->AnalogBValue));
                    }
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        dataText +=   resManager->GetString(L"textDataPacketAnalogA")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataPacketAnalogB");
                    }
                    else if (dataPacketData->AnalogUsed == true)
                    {
                        dataText +=   dataPacketData->AnalogAValue.ToString()
                                    + DATA_FILE_NEW_ENTRY
                                    + dataPacketData->AnalogBValue.ToString();
                    }
                    else
                    {
                        dataText +=   DATA_FILE_NEW_ENTRY;
                    }
                    dataText += DATA_FILE_NEW_ENTRY;
                }
                // get EnDat additional info 1 values
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                                  region, EIB7_PDF_EnDat_AI1,
                                                                  &ulValue, sizeof(ulValue));
                    if (entryFound == true)
                    {
                        dataPacketData->EnDatAI1DataValue = ulValue;
                        //Debug::Print(String::Format(L"EnDat info 1 values: 0x{0,4:X4}, 0x{1,4:X4}",
                        //                            dataPacketData->EnDatAI1StatusValue[0],
                        //                            dataPacketData->EnDatAI1InfoValue[0]));
                    }
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        dataText +=   resManager->GetString(L"textDataPacketEnDatAI1Status")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataPacketEnDatAI1Info");
                    }
                    else if (dataPacketData->EnDatAI1Used == true)
                    {
                        dataText +=   dataPacketData->EnDatAI1StatusValue[0].ToString()
                                    + DATA_FILE_NEW_ENTRY
                                    + dataPacketData->EnDatAI1InfoValue[0].ToString();
                    }
                    else
                    {
                        dataText +=   DATA_FILE_NEW_ENTRY;
                    }
                    dataText += DATA_FILE_NEW_ENTRY;
                }
                // get EnDat additional info 2 values
                if (   (rawBuffer != nullptr)
                    && (dataBuffer != nullptr))
                {
                    entryFound = control->EIB_ReadDataPacketValue(rawBuffer->buffer,
                                                                  region, EIB7_PDF_EnDat_AI2,
                                                                  &ulValue, sizeof(ulValue));
                    if (entryFound == true)
                    {
                        dataPacketData->EnDatAI2DataValue = ulValue;
                        //Debug::Print(String::Format(L"EnDat info 2 values: 0x{0,4:X4}, 0x{1,4:X4}",
                        //                            dataPacketData->EnDatAI2StatusValue[0],
                        //                            dataPacketData->EnDatAI2InfoValue[0]));
                    }
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        dataText +=   resManager->GetString(L"textDataPacketEnDatAI2Status")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataPacketEnDatAI2Info");
                    }
                    else if (dataPacketData->EnDatAI2Used == true)
                    {
                        dataText +=   dataPacketData->EnDatAI2StatusValue[0].ToString()
                                    + DATA_FILE_NEW_ENTRY
                                    + dataPacketData->EnDatAI2InfoValue[0].ToString();
                    }
                    else
                    {
                        dataText +=   DATA_FILE_NEW_ENTRY;
                    }
                    dataText += DATA_FILE_NEW_ENTRY;
                }
                // get position value
                if (dataBuffer != nullptr)
                {
                    GetAxisPosition(parameter, axisIndex, isAuxAxis, dataBuffer, entryIndex);
                }
                if (   (dataFile != nullptr)
                    && (dataFile->IsOpen == true))
                {
                    if (dataBuffer == nullptr)
                    {
                        lineText +=   resManager->GetString(L"textDataFilePosition")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataFileUnit")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataFileType")
                                    + DATA_FILE_NEW_ENTRY
                                    + resManager->GetString(L"textDataFileRevolutions");
                    }
                    else
                    {
                        if (dataPacketData->PositionValues->Valid == true)
                        {
                            String ^ valueFormat =
                                String::Format(L"F{0}", UInt32(common->Display->DisplayPrecision));
                            lineText +=   dataPacketData->PositionValues->Position.ToString(valueFormat)
                                        + DATA_FILE_NEW_ENTRY
                                        + dataPacketData->PositionValues->Unit
                                        + DATA_FILE_NEW_ENTRY;

                            if (dataPacketData->PositionValues->Absolute == true)
                            {
                                lineText += resManager->GetString(L"textDataFileAbsolute");
                            }
                            else
                            {
                                lineText += resManager->GetString(L"textDataFileIncremental");
                            }
                            lineText += DATA_FILE_NEW_ENTRY;

                            if (dataPacketData->PositionValues->Multiturn == true)
                            {
                                lineText += dataPacketData->PositionValues->Revolution.ToString();
                            }
                        }
                        else
                        {
                            lineText +=   DATA_FILE_NEW_ENTRY
                                        + DATA_FILE_NEW_ENTRY
                                        + DATA_FILE_NEW_ENTRY;
                        }
                    }
                    lineText += DATA_FILE_NEW_ENTRY;
                    // add data text to header text
                    lineText +=   dataText
                                + DATA_FILE_NEW_LINE;
                }
            }
            // write data packet to data file
            if (   (dataFile != nullptr)
                && (dataFile->IsOpen == true)
                && (lineText != L""))
            {
                dataFile->WriteString(lineText);
                //Debug::Print(String::Format(L"Data text length: {0}", lineText->Length));
            }
        }
    #pragma endregion

    #pragma region "Position calculation functions"
        /// <summary>
        /// Get axis position value.
        /// </summary>
        /// <param name="parameter">The EIB parameters.</param>
        /// <param name="axis">The axis index of the EIB selected in the Main Tree View.</param>
        /// <param name="isAuxAxis">The axis state of the EIB selected in the Main Tree View.</param>
        /// <param name="dataBuffer">The data packet buffer.</param>
        /// <param name="entryIndex">The index of the data packet entry.</param>
        void GetAxisPosition(EIB_Param ^ parameter,
                             Int32 axis, bool isAuxAxis,
                             DataPacketBuffer ^ dataBuffer,
                             UInt32 entryIndex)
        {
            // initialize parameters
            EIB_EnDat_Read ^ enDatParameter = EnDatRead[axis];
            // get axis and auxiliary axis datapacket values
            UInt32 errorFlag = 0;
            EIB_Axis_Param ^ axisParam;
            bool isEnabled, isIncremental, hasRefMark, isDistCoded, isLinear;
            Int32 auxAxis = 0, errorAxis;
            DataPacketData ^ dataPacketData;
            AxisWarningFlags ^ axisWarnings;
            if (isAuxAxis == false)
            {
                axisParam      = parameter->Axis[axis];
                isEnabled      = axisParam->IsEnabled;
                dataPacketData = dataBuffer->Entry[entryIndex]->Axis[axis];
                axisWarnings   = warningFlags.Axis[axis];
                errorAxis      = axis + 1;
            }
            else
            {
                auxAxis        = axis;
                axisParam      = parameter->AuxAxis[auxAxis]->Axis;
                isEnabled      = parameter->AuxAxis[auxAxis]->IsEnabled;
                dataPacketData = dataBuffer->Entry[entryIndex]->AuxAxis[auxAxis];
                axisWarnings   = warningFlags.AuxAxis[auxAxis];
                errorAxis      = - (axis + 1);
            }
            isIncremental  = (   (   axisParam->InterfaceTypeGui
                                  == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental)
                              || (   axisParam->InterfaceTypeGui
                                  == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_Incremental_11u));
            hasRefMark     = (   axisParam->Refmarks
                              == EIB7_REFMARKS::EIB7_RM_One);
            isDistCoded    = (   (isAuxAxis == false)
                              && (   axisParam->Refmarks
                                  == EIB7_REFMARKS::EIB7_RM_DistanceCoded));
            isLinear       = (   axisParam->EncoderType
                              == EIB7_ENCODER_TYPE::EIB7_EC_Linear);
            // initialize position values
            PositionData ^ positionData = dataPacketData->PositionValues;
            positionData->Clear();
            // check if axis is activated
            if (isEnabled == false)
            {
                // axis is deactivated
                return;
            }
            // check if raw position data are valid
            bool showInvalidPosition = true;
            if (   (dataPacketData->StatusWordUsed == false)
                || (   ((  dataPacketData->StatusWordValue
                         & (UInt16)DataPacketStatus::PositionValid) == 0)
                    && (showInvalidPosition == false))
                || (dataPacketData->PositionDataUsed == false))
            {
                // raw position data are not valid
                return;
            }
            // set raw position value
            Int64 positionDataValue = dataPacketData->PositionDataValue;
            positionData->Valid     = true;
            positionData->Position  = Double(positionDataValue);
            // calculate absolute position value
            if (isIncremental == true)
            {
                // incremental encoders / auxiliary axis
                if (hasRefMark == true)
                {
                    // incremental encoder / auxiliary axis with one reference mark
                    if ((  dataPacketData->StatusWordValue
                         & (UInt16)DataPacketStatus::RefPos1Valid) != 0)
                    {
                        if (dataPacketData->ReferencePos1Used == true)
                        {
                            positionDataValue       -= dataPacketData->ReferencePos1Value;
                            positionData->Absolute  =  true;
                            positionData->Position  =  Double(positionDataValue);
                        }
                        else if (axisWarnings->WarningRefPos1Disable == false)
                        {
                            // report warning
                            String ^ message =
                                        String::Format(resManager->GetString(L"warningDataPacketDisable"),
                                                       resManager->GetString(L"textDataPacketReferencePos"),
                                                       EibAxisName[errorAxis]);
                            guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                        message);
                            // set warning flag
                            axisWarnings->WarningRefPos1Disable = true;
                        }
                    }
                }
                else if (isDistCoded == true)
                {
                    // incremental encoder with distance coded reference marks
                    if ((  dataPacketData->StatusWordValue
                         & (UInt16)DataPacketStatus::RefPosCValid) != 0)
                    {
                        if (dataPacketData->DistCodedRefUsed == true)
                        {
                            positionDataValue       -= dataPacketData->DistCodedRefValue;
                            positionData->Absolute  =  true;
                            positionData->Position  =  Double(positionDataValue);
                        }
                        else if (axisWarnings->WarningRefPosCDisable == false)
                        {
                            // report warning
                            String ^ message =
                                        String::Format(resManager->GetString(L"warningDataPacketDisable"),
                                                       resManager->GetString(L"textDataPacketDistCodedRef"),
                                                       EibAxisName[errorAxis]);
                            guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                        message);
                            // set warning flag
                            axisWarnings->WarningRefPosCDisable = true;
                        }
                    }
                }
            }
            else
            {
                // EnDat encoders
                if ((enDatParameter->ParamE21W14 & 0x4000) == 0)
                {
                    // incremental EnDat encoder
                    if (axisParam->InterfaceTypeGui == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22)
                    {
                        // incremental EnDat 22 encoder
                        if (dataPacketData->EnDatAI1Pos2Valid == true)
                        {
                            positionDataValue       -= dataPacketData->EnDatAI1Pos2Value;
                            positionData->Absolute  =  true;
                            positionData->Position  =  Double(positionDataValue);
                        }
                        else if (   (axisWarnings->WarningAI1Pos2Disable == false)
                                 && (axisWarnings->EnDatAI1Pos2Set == false))
                        {
                            // report warning
                            String ^ message =
                                        String::Format(resManager->GetString(L"warningDataPacketDisable"),
                                                       resManager->GetString(L"textDataPacketEnDatAI1Pos2"),
                                                       EibAxisName[errorAxis]);
                            guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                        message);
                            // set warning flag
                            axisWarnings->WarningAI1Pos2Disable = true;
                        }
                    }
                    else if (warningFlags.WarningEnDat21Incremental == false)
                    {
                        // incremental EnDat 21 / 01 encoder not supported
                        guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                    resManager->GetString(L"warningEnDat21Incremental"));
                        // set warning flag
                        warningFlags.WarningEnDat21Incremental = true;
                    }
                }
                else
                {
                    // absolute EnDat encoder
                    if (axisParam->InterfaceTypeEib == UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDat01))
                    {
                        // absolute EnDat 01 encoder
                        if (enDatParameter->EnDat01RefPosValid == true)
                        {
                            positionDataValue       -= enDatParameter->EnDat01RefPosIncr;
                            positionData->Absolute  =  true;
                            positionData->Position  =  Double(positionDataValue);
                        }
                        else if (axisWarnings->WarningRefPosEnDat01Invalid == false)
                        {
                            // report warning
                            String ^ message =
                                        String::Format(resManager->GetString(L"warningRefPosEnDat01Invalid"),
                                                       EibAxisName[errorAxis]);
                            guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                                        message);
                            // set warning flag
                            axisWarnings->WarningRefPosEnDat01Invalid = true;
                        }
                    }
                    else
                    {
                        // other absolute EnDat encoders
                        positionData->Absolute = true;
                    }
                }
            }
            // calculate unit based position value
            Int64 interpolation;
            if (isIncremental == true)
            {
                // incremental encoders / auxiliary axis
                if (isAuxAxis == false)
                {
                    interpolation = EIB_SIGNAL_INTERPOLATION;
                }
                else
                {
                    interpolation =   Parameter->AuxAxis[auxAxis]->IntervalCounterIpfValue
                                    * Parameter->AuxAxis[auxAxis]->IntervalCounterEdgeValue;
                }
                if (isLinear == true)
                {
                    // linear incremental encoder
                    positionData->Position *=   axisParam->SignalPeriod * 1E-3
                                              / Double(interpolation);
                    positionData->Unit      = L"mm";
                }
                else
                {
                    // rotary incremental encoder
                    if (axisParam->Linecounts != 0)
                    {
                        Int64 linecounts = axisParam->Linecounts * interpolation;
                        positionData->Position   =   Double(positionDataValue % linecounts)
                                                   * 360.0 / Double(linecounts);
                        positionData->Unit       =  L"°";
                        positionData->Revolution = positionDataValue / linecounts;
                        positionData->Multiturn  = false;
                        if (positionData->Position < 0)
                        {
                            positionData->Position   += 360.0;
                            positionData->Revolution --;
                        }
                        //Debug::Print(String::Format(L"S: {0}, M: {1}", 
                        //                            positionDataValue % linecounts,
                        //                            positionDataValue / linecounts));
                    }
                    else
                    {
                        // set position parameter error
                        errorFlag = 1;
                    }
                }
            }
            else if (axisParam->InterfaceTypeEib != UInt32(EIB7_INTERFACE_TYPE_EIB::EIB7_IT_EnDat01))
            {
                // EnDat 2.1 and 2.2 encoders
                errorFlag = GetEnDatUnitPosValue(enDatParameter,
                                                 positionData, positionDataValue,
                                                 axisWarnings, errorAxis);
            }
            else
            {
                // EnDat 01 encoders
                if (enDatParameter->ParamE21W15_16 != 0)
                {
                    interpolation = EIB_SIGNAL_INTERPOLATION;
                    if ((enDatParameter->ParamE21W14 & 0x8000) == 0)
                    {
                        // linear EnDat 01 encoder
                        positionData->Position *=   enDatParameter->ParamE21W15_16 * 1E-6
                                                  / Double(interpolation);
                        positionData->Unit      = enDatParameter->EnDat01RefPosData->Unit;
                        positionData->Position += enDatParameter->EnDat01RefPosData->Position;
                    }
                    else
                    {
                        // rotary EnDat 01 encoder
                        Int64 linecounts = enDatParameter->ParamE21W15_16 * interpolation;
                        positionData->Position   =   Double(positionDataValue % linecounts)
                                                   * 360.0 / Double(linecounts);
                        positionData->Unit       = enDatParameter->EnDat01RefPosData->Unit;
                        positionData->Revolution = positionDataValue / linecounts;
                        positionData->Multiturn  = enDatParameter->EnDat01RefPosData->Multiturn;
                        if (positionData->Position < 0)
                        {
                            positionData->Position   += 360.0;
                            positionData->Revolution --;
                        }
                        positionData->Position += enDatParameter->EnDat01RefPosData->Position;
                        if (positionData->Position >= 360.0)
                        {
                            positionData->Position -= 360.0;
                        }
                        positionData->Revolution += enDatParameter->EnDat01RefPosData->Revolution;
                    }
                }
                else
                {
                    // set position parameter error
                    errorFlag = 6;
                }
            }
            // check for error
            if (   (errorFlag != 0)
                && (axisWarnings->WarningBadPositionParam == false))
            {
                // report warning
                String ^ message = String::Format(resManager->GetString(L"warningBadPositionParam"),
                                                  errorFlag, EibAxisName[errorAxis]);
                guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                            message);
                // set warning flag
                axisWarnings->WarningBadPositionParam = true;
            }
        }

        /// <summary>
        /// Get EnDat encoder unit based position value.
        /// </summary>
        /// <param name="enDatParameter">The EnDat read parameters.</param>
        /// <param name="positionData">The position data.</param>
        /// <param name="positionDataValue">The position value.</param>
        /// <param name="axisWarnings">The warning flags.</param>
        /// <param name="errorAxis">The error axis index.</param>
        /// <returns">The error flag.</returns>
        UInt32 GetEnDatUnitPosValue(EIB_EnDat_Read ^ enDatParameter,
                                    PositionData ^   positionData,
                                    Int64 positionDataValue,
                                    AxisWarningFlags ^ axisWarnings,
                                    Int32 errorAxis)
        {
            UInt32 errorFlag = 0;
            // EnDat encoders
            if ((enDatParameter->ParamE21W14 & 0x8000) == 0)
            {
                // linear EnDat encoder
                if ((enDatParameter->ParamE21W14 & 0x4000) == 0)
                {
                    // incremental linear EnDat encoder
                    if (enDatParameter->ParamE21W15_16 == 0)
                    {
                        if (enDatParameter->ParamE21W20_21 != 0)
                        {
                            positionData->Position *= Double(enDatParameter->ParamE21W20_21) * 1E-6;
                            positionData->Unit     =  L"mm";
                        }
                        else
                        {
                            if ((enDatParameter->ParamE22W27 & 0x07) == 0x03)
                            {
                                positionData->Position *= Double(enDatParameter->ParamE22W28_29) * 1E-9;
                                positionData->Unit     =  L"mm";
                            }
                            else
                            {
                                // set position parameter error
                                errorFlag = 2;
                            }
                        }
                    }
                    else
                    {
                        if (enDatParameter->ParamE21W20_21 == 0)
                        {
                            if (   ((enDatParameter->ParamE22W27 & 0x07) == 0x01)
                                && (enDatParameter->ParamE22W28_29 != 0))
                            {
                                positionData->Position *=   (  Double(enDatParameter->ParamE21W15_16)
                                                             / Double(enDatParameter->ParamE22W28_29))
                                                          * 1E-3;
                                positionData->Unit     =  L"mm";
                            }
                            else if (   ((enDatParameter->ParamE22W27 & 0x07) == 0x02)
                                     && (enDatParameter->ParamE22W28_29 != 0))
                            {
                                positionData->Position *=   (  Double(enDatParameter->ParamE21W15_16)
                                                             / Double(1 << enDatParameter->ParamE22W28_29))
                                                          * 1E-3;
                                positionData->Unit     =  L"mm";
                            }
                            else
                            {
                                // set position parameter error
                                errorFlag = 3;
                            }
                        }
                        else
                        {
                            // set position parameter error
                            errorFlag = 4;
                        }
                    }
                }
                else
                {
                    // absolute linear EnDat encoder
                    positionData->Position *= Double(enDatParameter->ParamE21W20_21) * 1E-6;
                    positionData->Unit     =  L"mm";
                }
            }
            else
            {
                // rotary EnDat encoder
                if (enDatParameter->ParamE21W20_21 != 0)
                {
                    if ((enDatParameter->ParamE21W14 & 0x4000) == 0)
                    {
                        // incremental rotary EnDat encoder
                        positionData->Position *= 360.0 / Double(enDatParameter->ParamE21W20_21);
                        positionData->Unit     =  L"°";
                    }
                    else
                    {
                        // absolute rotary EnDat encoder
                        if ((enDatParameter->ParamE21W14 & 0x3000) == 0)
                        {
                            // single turn rotary EnDat encoder
                            positionData->Position *= 360.0 / Double(enDatParameter->ParamE21W20_21);
                            positionData->Unit     =  L"°";
                        }
                        else
                        {
                            // multi turn rotary EnDat encoder
                            Int64 singleTurn         =   positionDataValue
                                                       % enDatParameter->ParamE21W20_21;
                            positionData->Position   =   Double(singleTurn) * 360.0
                                                       / Double(enDatParameter->ParamE21W20_21);
                            positionData->Unit       = L"°";
                            positionData->Revolution = Int64(  positionDataValue
                                                             / enDatParameter->ParamE21W20_21);
                            positionData->Multiturn  = true;
                        }
                    }
                }
                else
                {
                    // set position parameter error
                    errorFlag = 5;
                }
            }
            // check for error
            if (   (errorFlag != 0)
                && (axisWarnings->WarningBadPositionParam == false))
            {
                // report warning
                String ^ message = String::Format(resManager->GetString(L"warningBadPositionParam"),
                                                  errorFlag, EibAxisName[errorAxis]);
                guiSupport->ShowWarningText(resManager->GetString(L"infoEibMeasureWarning"),
                                            message);
                // set warning flag
                axisWarnings->WarningBadPositionParam = true;
            }
            return errorFlag;
        }
    #pragma endregion

    #pragma region "Data file functions"
        /// <summary>
        /// Write EIB configuration to data file.
        /// </summary>
        void WriteDataFileConfig()
        {
            // initialize parameters
            UInt32 maxAxis    = Math::Min(axisNumber,    Parameter->AxisNumber);
            UInt32 maxAuxAxis = Math::Min(auxAxisNumber, Parameter->AuxAxisNumber);
            // loop for all axis
            UInt32 auxAxis;
            for (UInt32 axis = 0; axis < maxAxis + maxAuxAxis; axis ++)
            {
                String ^ fileText =   Parameter->EibName
                                    + DATA_FILE_NEW_ENTRY
                                    + AxisName[axis + 1]
                                    + DATA_FILE_NEW_ENTRY;
                if (axis < maxAxis)
                {
                    InterfaceTypeConverter ^ axisType = gcnew InterfaceTypeConverter;
                    fileText += axisType->ConvertTo(Parameter->Axis[axis]->InterfaceTypeGui, String::typeid);
                }
                else
                {
                    auxAxis = axis - maxAxis;
                    EnabledConverter ^ axisState = gcnew EnabledConverter;
                    fileText += axisState->ConvertTo(Parameter->AuxAxis[auxAxis]->IsEnabled, String::typeid);
                }
                fileText += DATA_FILE_NEW_LINE;
                dataFile->WriteString(fileText);
            }
        }

        /// <summary>
        /// Write measure data header to data file.
        /// </summary>
        void WriteDataFileHeader()
        {
            ReadDataPacket(Parameter, nullptr, nullptr, 0, dataFile);
        }
    #pragma endregion

    #pragma region "Axis command functions"
        /// <summary>
        /// Start axis reference run.
        /// </summary>
        /// <param name="axisIndex">The axis index of the EIB selected in the Main Tree View.</param>
        /// <param name="isAuxAxis">The axis state of the EIB selected in the Main Tree View.</param>
        /// <param name="startRef">If true then start else stop reference run.</param>
        void AxisStartRef(UInt32 axisIndex, bool isAuxAxis, bool startRef)
        {
            if (isAuxAxis == false)
            {
                control->EIB_AxisStartRef(axisIndex, startRef, Parameter->Axis[axisIndex]);
            }
            else
            {
                control->EIB_AuxAxisStartRef(axisIndex, startRef, Parameter->AuxAxis[axisIndex]);
            }
        }

        /// <summary>
        /// Check axis reference run state.
        /// </summary>
        /// <param name="axisIndex">The axis index of the EIB selected in the Main Tree View.</param>
        /// <param name="isAuxAxis">The axis state of the EIB selected in the Main Tree View.</param>
        /// <returns>True if the reference run is active.</returns>
        bool AxisCheckRef(UInt32 axisIndex, bool isAuxAxis)
        {
            if (isAuxAxis == false)
            {
                return control->EIB_AxisCheckRef(axisIndex, Parameter->Axis[axisIndex]);
            }
            else
            {
                return control->EIB_AuxAxisCheckRef(axisIndex, Parameter->AuxAxis[axisIndex]);
            }
        }

        /// <summary>
        /// Clear axis.
        /// </summary>
        /// <param name="axisIndex">The axis index of the EIB selected in the Main Tree View.</param>
        /// <param name="isAuxAxis">The axis state of the EIB selected in the Main Tree View.</param>
        void AxisClear(UInt32 axisIndex, bool isAuxAxis)
        {
            if (isAuxAxis == false)
            {
                control->EIB_AxisClear(axisIndex, Parameter->Axis[axisIndex]);
                // reset reference position 2 valid
                if (   (isAuxAxis == false)
                    && (   Parameter->Axis[axisIndex]->InterfaceTypeGui
                        == EIB7_INTERFACE_TYPE_GUI::EIB7_IT_EnDat22))
                {
                    DisplayBuffer->Entry[0]->Axis[axisIndex]->EnDatAI1Pos2Valid = false;
                }
            }
            else
            {
                control->EIB_AuxAxisClear(axisIndex, Parameter->AuxAxis[axisIndex]);
            }
        }

        /// <summary>
        /// Reset axis error.
        /// </summary>
        /// <param name="axisIndex">The axis index of the EIB selected in the Main Tree View.</param>
        /// <param name="isAuxAxis">The axis state of the EIB selected in the Main Tree View.</param>
        /// <param name="resetCmd">The error reset command.</param>
        void AxisResetError(UInt32 axisIndex, bool isAuxAxis, DataPacketCmd resetCmd)
        {
            if (isAuxAxis == false)
            {
                control->EIB_AxisResetError(axisIndex, resetCmd, Parameter->Axis[axisIndex]);
            }
            else
            {
                control->EIB_AuxAxisResetError(axisIndex, resetCmd, Parameter->AuxAxis[axisIndex]);
            }
        }
    #pragma endregion
    };
    #pragma endregion
}
