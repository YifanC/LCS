///
/// \file       EIB_List.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: EIB_List.h,v 1.2 2011/09/20 12:24:35 rohn Exp $
/// $Name:  $
/// $Revision: 1.2 $
/// $State: Exp $
/// $Log: EIB_List.h,v $
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      EIB list classes.
///
/// This classes are used for the management of the EIBs added to the system
/// via the Add EIB button or the Open Projekt menu entry.
///

#pragma once

#include "FormMain.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Resources;
using namespace System::Windows::Forms;

namespace EIB7xx_App
{
 	#pragma region "EIB_TreeList class definition"
    /// special definitions
    typedef List<EibAxis ^>     EibAxisValueList;

    /// <summary>
    /// This class manages the EIBs added to the system
    /// via the Add EIB button or the Open Projekt menu entry.
    /// These EIBs are saved as instances of the EIB_Entry class
    /// as the tag of the appropriate tree node of the Main Tree View.
    /// </summary>
    public ref class EIB_TreeList
    {
    private:
        UInt32              fileVersion;    ///< parameter version for parameter file

        GUI_Support ^       guiSupport;     ///< the GUI support class
        ResourceManager ^   resManager;     ///< resource manager from main window
        TreeView ^          treeViewMain;   ///< the Main Tree Vew

        UInt32              eibKeyCount;    ///< counter for unique EIB key values

        EIB_Entry ^         selectedEIB;    ///< in the Main Tree View selected EIB
        String ^            eibKey;         ///< key of the EIB selected in the Main Tree View
        UInt32              axisIndex;      ///< axis index of the EIB selected in the Main Tree View
        bool                isAuxAxis;      ///< axis state of the EIB selected in the Main Tree View

        EIB_Common_Param    common;         ///< parameters common to all EIBs in the system

        EIB_DataFile        dataFile;       ///< the measure data file

        DateTime ^          startTime;      ///< measurement start time

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
        EIB_TreeList(GUI_Support ^ guiSupport)
        {
            fileVersion = 1;

            this->guiSupport   = guiSupport;
            this->resManager   = guiSupport->ResManager;
            this->treeViewMain = guiSupport->TreeViewMain;

            selectedEIB = nullptr;
            eibKeyCount = 0;
        }

        /// <summary>
        /// Gets the number of EIBs in the Main tree View.
        /// </summary>
        property UInt32 EibCount
        {
            UInt32 get() { return treeViewMain->Nodes[0]->GetNodeCount(false); }
        }

        /// <summary>
        /// Gets the EIB key from EIB index.
        /// </summary>
        property String ^ EibKey[UInt32]
        {
            String ^ get(UInt32 index)
            {
                return treeViewMain->Nodes[0]->Nodes[index]->Name;
            }
        }

        /// <summary>
        /// Gets the EIB entry from EIB index.
        /// </summary>
        property EIB_Entry ^ EibEntry[UInt32]
        {
            EIB_Entry ^ get(UInt32 index)
            {
                return (EIB_Entry ^)(treeViewMain->Nodes[0]->Nodes[index]->Tag);
            }
        }

        /// <summary>
        /// Gets or sets the EIB selected in the Main tree View.
        /// </summary>
        property EIB_Entry ^ SelectedEIB
        {
            EIB_Entry ^ get() { return selectedEIB; }
            void set(EIB_Entry ^ value) { selectedEIB = value; }
        }

        /// <summary>
        /// Gets or sets the key of the EIB selected in the Main Tree View.
        /// </summary>
        property String ^ EibKey
        {
            String ^ get() { return eibKey; }
            void set(String ^ value) { eibKey = String::Copy(value); }
        }

        /// <summary>
        /// Gets or sets the axis index of the EIB selected in the Main Tree View.
        /// </summary>
        property UInt32 AxisIndex
        {
            UInt32 get() { return axisIndex; }
            void set(UInt32 value) { axisIndex = value; }
        }

        /// <summary>
        /// Gets or sets the axis state of the EIB selected in the Main Tree View.
        /// </summary>
        property bool IsAuxAxis
        {
            bool get() { return isAuxAxis; }
            void set(bool value) { isAuxAxis = value; }
        }

        /// <summary>
        /// Gets the common EIB parameters.
        /// </summary>
        property EIB_Common_Param ^ Common
        {
            EIB_Common_Param ^ get() { return %common; }
        }

    private:
        /// <summary>
        /// Adds a known EIB to the system.
        /// </summary>
		/// <param name="eibToAdd">The EIB to add to the system.</param>
        /// <returns>The Main Tree View node where the EIB has been added to.</returns>
        TreeNode ^ AddEIB(EIB_Entry ^ eibToAdd)
        {
            // get new unique EIB key
            String ^ textName = String::Format(L"EIB" + L" {0:D}", eibKeyCount);
            eibKeyCount ++;
            // add EIB to Main Tree View
            TreeNode ^ eibNode = treeViewMain->Nodes[0]->Nodes->Add(textName,
                                                                    eibToAdd->Parameter->EibName,
                                                                    1, 1);
            // set EIB class as tag of EIB node
            eibNode->Tag = eibToAdd;
            // add axis to EIB node
            TreeNode ^ axisNode;
            Int32 axisIndex;
            Int32 axisNumber = eibToAdd->Parameter->AxisNumber;
            for (axisIndex = 0; axisIndex < axisNumber; axisIndex ++)
            {
                textName = eibToAdd->AxisName[axisIndex + 1];
                axisNode = eibNode->Nodes->Add(L"", textName, 2, 2);
                // set axis index as tag of axis node
                axisNode->Tag = axisIndex + 1;
            }
            // add auxiliary axis to EIB node
            axisNumber = eibToAdd->Parameter->AuxAxisNumber;
            for (axisIndex = 0; axisIndex < axisNumber; axisIndex ++)
            {
                textName = eibToAdd->AxisName[- (axisIndex + 1)];
                axisNode = eibNode->Nodes->Add(L"", textName, 2, 2);
                // set auxiliary axis index as tag of axis node
                axisNode->Tag = - (axisIndex + 1);
            }
            // return the Main Tree View node where the EIB has been added to
            return eibNode;
        }

    public:
        /// <summary>
        /// Adds a new EIB to the system.
        /// </summary>
        /// <returns>The name of the EIB added.</returns>
        String ^ AddNewEIB()
        {
            // create a new EIB
            EIB_Entry ^ eibToAdd = gcnew EIB_Entry(guiSupport, Common, %dataFile);
            // get a unique name for the EIB
            String ^ textName;
            UInt32 nameCount = 1;
            bool nameFound = true;
            while (nameFound == true)
            {
                // create new name
                textName = String::Format(resManager->GetString(L"textEIB") + L" {0:D}", nameCount);
                // check if name already exists
                nameFound = false;
                for each (TreeNode ^ node in treeViewMain->Nodes[0]->Nodes)
                {
                    if (node->Text == textName)
                    {
                        nameFound = true;
                        nameCount ++;
                        break;
                    }
                }
            }
            // set the EIB name
            eibToAdd->Parameter->EibName = textName;
            // add the EIB to the system
            TreeNode ^ eibNode = AddEIB(eibToAdd);
            // expand root and added EIB nodes
            treeViewMain->Nodes[0]->Expand();
            eibNode->Expand();
            // select added EIB node
            treeViewMain->SelectedNode = eibNode;
            // return the Main Tree View node where the EIB has been added to
            return textName;
        }

        /// <summary>
        /// Adds the EIBs from a project file to the system.
        /// </summary>
	    /// <param name="fileName">Project file name.</param>
        /// <returns>The number of EIBs in the system.</returns>
        UInt32 AddEIBsFromProject(String ^ fileName)
        {
            EIB_ParamFile ^ paramFile = nullptr;
            try
            {
                // open param file for read
                paramFile = gcnew EIB_ParamFile(guiSupport, fileName, false);
                // read file header
                String ^ fileHeader;
                paramFile->ReadWriteString(fileHeader);
                // check file header
                if (fileHeader != PARAM_FILE_HEADER)
                {
                    String ^ message = String::Format(resManager->GetString(L"errorEibParamFileHeader"),
                                                      fileName);
                    throw gcnew EIB_FileException(message, fileName);
                }
                // read parameter version
                UInt32 paramVersion;
                paramFile->ReadWriteUInt32(paramVersion);
                // check parameter version
                paramFile->CheckParamVersion(paramVersion, fileVersion);
                // read all parameters to temporary memory
                // read common paramers
                EIB_Common_Param tmpCommon;
                tmpCommon.ReadWriteFile(paramFile);
                // read EIB number
                UInt32 eibNumber;
                paramFile->ReadWriteUInt32(eibNumber);
                // read EIBs from project file
                typedef array<EIB_Entry ^>  TmpEibList;
                TmpEibList ^ tmpEibList = gcnew TmpEibList(eibNumber);
                for (UInt32 i = 0; i < eibNumber; i ++)
                {
                    tmpEibList[i] = gcnew EIB_Entry(guiSupport, Common, %dataFile);
                    tmpEibList[i]->Parameter->ReadWriteFile(paramFile);
                }
                // copy all parameters from temporary memory
                // read common paramers
                Common->Copy(%tmpCommon);
                // delete EIBs in list
                EIB_Entry ^ eibEntry;
                for each (TreeNode ^ node in treeViewMain->Nodes[0]->Nodes)
                {
                    // initialize a single EIB
                    eibEntry = (EIB_Entry ^)node->Tag;
                    delete eibEntry;
                }
                // clear the EIB list
                treeViewMain->Nodes[0]->Nodes->Clear();
                // add EIBs from project file
                for (UInt32 i = 0; i < eibNumber; i ++)
                {
                    AddEIB(tmpEibList[i]);
                }
                // expand root node
                treeViewMain->Nodes[0]->Expand();
                // select root node
                treeViewMain->SelectedNode = treeViewMain->Nodes[0];
                // reset selected EIB
                SetSelectedEIB(nullptr);
            }
            finally
            {
                if (paramFile != nullptr)
                {
                    // close param file
                    delete paramFile;
                }
            }
            // return the number of EIBs in the system
            return treeViewMain->Nodes[0]->GetNodeCount(false);
        }

        /// <summary>
        /// Save EIB parameters.
        /// </summary>
	    /// <param name="fileName">Project file name.</param>
        void SaveEIBParameters(String ^ fileName)
        {
            EIB_ParamFile ^ paramFile = nullptr;
            try
            {
                // open param file for write
                paramFile = gcnew EIB_ParamFile(guiSupport, fileName, true);
                // write file header
                String ^ fileHeader = PARAM_FILE_HEADER;
                paramFile->ReadWriteString(fileHeader);
                // write parameter version
                paramFile->ReadWriteUInt32(fileVersion);
                // write common paramers
                Common->ReadWriteFile(paramFile);
                // write EIB number
                UInt32 eibNumber = treeViewMain->Nodes[0]->GetNodeCount(false);
                paramFile->ReadWriteUInt32(eibNumber);
                // write EIBs to project file
                for (UInt32 i = 0; i < eibNumber; i ++)
                {
                    EibEntry[i]->Parameter->ReadWriteFile(paramFile);
                }
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
        /// Removes the selected EIB from the system.
        /// </summary>
        /// <returns>The name of the EIB removed.</returns>
        String ^ RemoveSelectedEIB()
        {
            // get EIB name
            String ^ eibName = SelectedEIB->Parameter->EibName; 
            // delete selected EIB
            delete SelectedEIB;
            // remove selected EIB
            treeViewMain->Nodes[0]->Nodes->RemoveByKey(EibKey);
            // select root node
            treeViewMain->SelectedNode = treeViewMain->Nodes[0];
            // return the number of EIBs in the system
            return eibName;
        }

        /// <summary>
        /// Set the selected EIB.
        /// </summary>
		/// <param name="treeNode">The selected node in the Main Tree View.</param>
        /// <returns>True if an EIB selected, false if an axis selected.</returns>
        bool SetSelectedEIB(TreeNode ^ treeNode)
        {
            bool eibSelected = false;
            // check for no EIB selected
            if (treeNode == nullptr)
            {
                SelectedEIB = nullptr;
                return false;
            }
            // check for EIB or axis
            if (treeNode->Level == 1)
            {
                // EIB selected
                eibSelected = true;
                // set selected EIB
                SelectedEIB = (EIB_Entry ^)(treeNode->Tag);
                // set selected EIB key
                EibKey = treeNode->Name;
            }
            else
            {
                // axis selected
                // set selected EIB
                SelectedEIB = (EIB_Entry ^)(treeNode->Parent->Tag);
                // set selected EIB key
                EibKey = treeNode->Parent->Name;
                // get axis index
                Int32 axisIndex = (Int32)(treeNode->Tag);
                // set axis parameters
                if (axisIndex > 0)
                {
                    AxisIndex = axisIndex - 1;
                    IsAuxAxis = false;
                }
                else
                {
                    AxisIndex = - (axisIndex + 1);
                    IsAuxAxis = true;
                }
            }
            return eibSelected;
        }

        /// <summary>
        /// Check for EIBs with equal IP addresses.
        /// </summary>
	    /// <param name="throwException">If true then throw EIB_CtrlException
        /// in case of EIBs with equal IP addresses.</param>
        /// <returns>True in case of EIBs with equal IP addresses.</returns>
        bool CheckForEqualIpAddresses(bool throwException)
        {
            EIB_Entry ^ eibEntry0;
            EIB_Entry ^ eibEntry1;
            UInt32 i, j, ipAddress0, ipAddress1;
            UInt32 eibCount = treeViewMain->Nodes[0]->GetNodeCount(false);
            // outer loop
            for (i = 0; i < eibCount; i ++)
            {
                // get EIB IP address
                eibEntry0  = (EIB_Entry ^)treeViewMain->Nodes[0]->Nodes[i]->Tag;
                ipAddress0 = eibEntry0->Parameter->Network->CurrentIpAddress;
                // inner loop
                for (j = i + 1; j < eibCount; j ++)
                {
                    // get EIB IP address
                    eibEntry1  = (EIB_Entry ^)treeViewMain->Nodes[0]->Nodes[j]->Tag;
                    ipAddress1 = eibEntry1->Parameter->Network->CurrentIpAddress;
                    if (ipAddress1 == ipAddress0)
                    {
                        if (throwException == true)
                        {
                            // set equal ip addresses error exception
                            String ^ message = String::Format(resManager->GetString(L"errorEibEqualIp"),
                                                              eibEntry0->Parameter->EibName,
                                                              eibEntry1->Parameter->EibName);
                            throw gcnew EIB_CtrlException(message,
                                                          EIB7_ERROR::EIB7_EqualIpAddress,
                                                          L"CheckForEqualIpAddresses",
                                                          eibEntry0->Parameter->EibName, 0);
                        }
                        return true;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Check crankshaft parameters.
        /// </summary>
        /// <returns>The list of not valid parameters.</returns>
        EibAxisValueList ^ CheckCrankshaftParam()
        {
            EibAxisValueList ^ notValidValues = nullptr;
            // check for crankshaft mode
            if (guiSupport->GuiConfig->GuiParam->AppCrankshaft == false)
            {
                return notValidValues;
            }
            UInt32 eibIndex, axisIndex;
            // check for not valid parameters
            EibAxis ^ pointValue;
            for (UInt32 i = 0; i < CRANKSHAFT_MEASURE_POINT_NUM; i ++)
            {
                pointValue = Common->CrankshaftParam->MeasurePoint[i];
                eibIndex   = pointValue->EibIndex  - 1;
                axisIndex  = pointValue->AxisIndex - 1;
                if (   (pointValue->IsNotUsed() == false)
                    && (   (eibIndex  >= EibCount)
                        || (axisIndex >= EibEntry[eibIndex]->Parameter->AxisNumber)
                        || (EibEntry[eibIndex]->Parameter->Axis[axisIndex]->IsEnabled == false)))
                {
                    // save not valid parameter
                    if (notValidValues == nullptr)
                    {
                        notValidValues = gcnew EibAxisValueList;
                    }
                    notValidValues->Add(pointValue);
                }
            }
            EibAxis ^ angleValue = Common->CrankshaftParam->AngelMeasure;
            eibIndex  = angleValue->EibIndex  - 1;
            axisIndex = angleValue->AxisIndex - 1;
            if (   (angleValue->IsNotUsed() == false)
                && (   (eibIndex  >= EibCount)
                    || (axisIndex >= EibEntry[eibIndex]->Parameter->AxisNumber)
                    || (EibEntry[eibIndex]->Parameter->Axis[axisIndex]->IsEnabled == false)))
            {
                // save not valid parameter
                if (notValidValues == nullptr)
                {
                    notValidValues = gcnew EibAxisValueList;
                }
                notValidValues->Add(angleValue);
            }
            return notValidValues;
        }

        /// <summary>
        /// Initialize EIB(s).
        /// </summary>
        void InitializeEIBs()
        {
            // check for multiple IP addresses
            CheckForEqualIpAddresses(true);
            // initialize all EIBs
            EIB_Entry ^ eibEntry;
            for each (TreeNode ^ node in treeViewMain->Nodes[0]->Nodes)
            {
                // initialize a single EIB
                eibEntry = (EIB_Entry ^)node->Tag;
                eibEntry->InitializeEIB();
            }
            // check crankshaft parameters
            EibAxisValueList ^ notValidValues = CheckCrankshaftParam();
            if (notValidValues != nullptr)
            {
                throw gcnew EIB_CtrlException(resManager->GetString(L"errorCrankshaftParam"),
                                              EIB7_ERROR::EIB7_CrankshaftParam,
                                              L"CheckCrankshaftParam",
                                              L"", 0);
            }
        }

        /// <summary>
        /// Check EIB open.
        /// </summary>
        /// <returns>True if EIB is open.</returns>
        bool CheckEibOpen()
        {
            // start all EIBs
            bool isOpen;
            EIB_Entry ^ eibEntry;
            for each (TreeNode ^ node in treeViewMain->Nodes[0]->Nodes)
            {
                // check a single EIB
                eibEntry = (EIB_Entry ^)node->Tag;
                isOpen = eibEntry->CheckEibOpen();
                if (isOpen == false)
                {
                    return false;
                }
            }
            return true;
        }

        /// <summary>
        /// Open measure data file.
        /// </summary>
	    /// <param name="fileName">Data file name.</param>
        void OpenDataFile(String ^ fileName)
        {
            // open measure data file
            dataFile.OpenFile(fileName);
            // write file header
            OpModeConverter ^ opMode = gcnew OpModeConverter;
            String ^ fileText =   DATA_FILE_HEADER
                                + DATA_FILE_NEW_ENTRY
                                + resManager->GetString(L"textDataFileHeader")
                                + DATA_FILE_NEW_LINE + DATA_FILE_NEW_LINE
                                + resManager->GetString(L"textDataFileOperationMode")
                                + DATA_FILE_NEW_ENTRY
                                + opMode->ConvertTo(Common->OpMode->OpMode, String::typeid)
                                + DATA_FILE_NEW_LINE + DATA_FILE_NEW_LINE
                                + resManager->GetString(L"textDataFileConfiguration")
                                + DATA_FILE_NEW_LINE + DATA_FILE_NEW_LINE;
            dataFile.WriteString(fileText);
            // write all EIBs configuration
            EIB_Entry ^ eibEntry;
            TreeNode ^  node;
            for each (node in treeViewMain->Nodes[0]->Nodes)
            {
                // write single EIB configuration
                eibEntry = (EIB_Entry ^)node->Tag;
                eibEntry->WriteDataFileConfig();
                dataFile.WriteString(DATA_FILE_NEW_LINE);
            }
            // write measure start time
            fileText =   resManager->GetString(L"textDataFileStartTime")
                       + DATA_FILE_NEW_ENTRY
                       + startTime->ToString()->Replace(L" ", DATA_FILE_NEW_ENTRY)
                       + DATA_FILE_NEW_LINE + DATA_FILE_NEW_LINE;
            dataFile.WriteString(fileText);
            // write measure data header row
            node     = treeViewMain->Nodes[0]->Nodes[0];
            eibEntry = (EIB_Entry ^)node->Tag;
            eibEntry->WriteDataFileHeader();
            dataFile.WriteString(DATA_FILE_NEW_LINE);
        }

        /// <summary>
        /// Close measure data file.
        /// </summary>
        void CloseDataFile()
        {
            if (dataFile.IsOpen == true)
            {
                //Debug::Print(String::Format(L"Close data file"));
                dataFile.CloseFile();
            }
        }

        /// <summary>
        /// Start EIB measurement.
        /// </summary>
	    /// <param name="fileName">Data file name.</param>
        void StartEIBMeasurement(String ^ fileName)
        {
            try
            {
                // save measure start time
                startTime = DateTime::Now;
                // check for saving measure data
                if (fileName != nullptr)
                {
                    // open measure data file
                    OpenDataFile(fileName);
                }
                // start all EIBs
                EIB_Entry ^ eibEntry;
                TreeNode ^  treeNode;
                for (UInt32 i = 0; i < EibCount; i ++)
                {
                    // start a single EIB
                    treeNode = treeViewMain->Nodes[0]->Nodes[i];
                    eibEntry = (EIB_Entry ^)treeNode->Tag;
                    eibEntry->StartEIBMeasurement(i);
                }
            }
            catch (Exception ^ ex)
            {
                // close measure data file
                CloseDataFile();
                // throw current exception
                throw ex;
            }
        }

        /// <summary>
        /// Stop EIB measurement.
        /// </summary>
        void StopEIBMeasurement()
        {
            try
            {
                // stop all EIBs
                EIB_Entry ^ eibEntry;
                for each (TreeNode ^ node in treeViewMain->Nodes[0]->Nodes)
                {
                    // stop a single EIB
                    eibEntry = (EIB_Entry ^)node->Tag;
                    eibEntry->StopEIBMeasurement();
                }
            }
            finally
            {
                // close measure data file
                CloseDataFile();
            }
        }

        /// <summary>
        /// Check for stop measurement.
        /// </summary>
        /// <returns">True if stop conditions are met.</returns>
        bool CheckForStopMeasurement()
        {
            // check for stop measurement for all EIBs
            bool stopFlag = false;
            EIB_Entry ^ eibEntry;
            for each (TreeNode ^ node in treeViewMain->Nodes[0]->Nodes)
            {
                // check for stop measurement for a single EIB
                eibEntry = (EIB_Entry ^)node->Tag;
                stopFlag = eibEntry->CheckForStopMeasurement();
                if (stopFlag == true)
                {
                    return stopFlag;
                }
            }
            return stopFlag;
        }

        /// <summary>
        /// Read polling measure data.
        /// </summary>
		/// <param name="running">True if system is in running mode.</param>
        /// <returns">True if stop conditions are met.</returns>
        bool ReadPollingData(bool running)
        {
            // read measure data from all EIBs
            bool stopFlag = false;
            EIB_Entry ^ eibEntry;
            TreeNode ^  treeNode;
            for (UInt32 i = 0; i < EibCount; i ++)
            {
                // read measure data from a single EIB
                treeNode = treeViewMain->Nodes[0]->Nodes[i];
                eibEntry = (EIB_Entry ^)treeNode->Tag;
                stopFlag = eibEntry->ReadPollingData(i, running);
                if (stopFlag == true)
                {
                    return stopFlag;
                }
            }
            return stopFlag;
        }

        /// <summary>
        /// Read measure buffer state for recording.
        /// </summary>
		/// <param name="entryNumber">The number of entries in the record buffer of the selected EIB.</param>
		/// <param name="firstReady">True if the first EIB buffer is full.</param>
		/// <param name="allReady">True if all EIB buffers are full.</param>
        /// <returns>The fill level of the record buffer of the selected EIB in %.</returns>
        UInt32 ReadRecordingState(UInt32 % entryNumber, bool % firstReady, bool % allReady)
        {
            // initialize parameters
            firstReady  = false;
            allReady    = false;
            entryNumber = 0;
            UInt32 fillLevel = 0;
            Int32 readyCount = 0;
            // read state from all EIBs
            UInt32 eibFillLevel, eibEntryNumber;
            EIB_Entry ^ eibEntry;
            for each (TreeNode ^ node in treeViewMain->Nodes[0]->Nodes)
            {
                // read state from a single EIB
                eibEntry = (EIB_Entry ^)node->Tag;
                eibFillLevel = eibEntry->ReadRecordingState(eibEntryNumber);
                // set return values for selected EIB
                if (eibEntry == selectedEIB)
                {
                    fillLevel   = eibFillLevel;
                    entryNumber = eibEntryNumber;
                }
                // check ready state
                if (eibFillLevel >= 100)
                {
                    readyCount ++;
                }
            }
            // check ready state
            if (readyCount >= 1)
            {
                firstReady = true;
            }
            if (readyCount == treeViewMain->Nodes[0]->GetNodeCount(false))
            {
                allReady = true;
            }
            return fillLevel;
        }

        /// <summary>
        /// Save recording measure data.
        /// </summary>
	    /// <param name="fileName">Data file name.</param>
        void SaveRecordingData(String ^ fileName)
        {
            try
            {
                // open measure data file
                OpenDataFile(fileName);
                // save recording measure data of all EIBs
                EIB_Entry ^ eibEntry;
                TreeNode ^  treeNode;
                for (UInt32 i = 0; i < EibCount; i ++)
                {
                    // save recording measure data of a single EIB
                    treeNode = treeViewMain->Nodes[0]->Nodes[i];
                    eibEntry = (EIB_Entry ^)treeNode->Tag;
                    eibEntry->SaveRecordingData(i, EibCount);
                }
            }
            finally
            {
                // close measure data file
                CloseDataFile();
            }
        }
    };
	#pragma endregion

 	#pragma region "EIB_CompareList class definition"
    // EIB parameter compare list
    typedef Dictionary<String ^, EIB_Param ^>   CompareList;

    /// <summary>
    /// This class is used to compare the parameters
    /// that have been saved in a project file
    /// or that have been used for EIB initialization
    /// with the current parameters.
    /// </summary>
    public ref class EIB_CompareList
    {
    private:
        GUI_Support ^       guiSupport;     ///< the GUI support class

        EIB_Common_Param    common;         ///< parameters common to all EIBs in the system

    protected:
        CompareList         compareList;    ///< list of EIB parameters

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
        EIB_CompareList(GUI_Support ^ guiSupport)
        {
            this->guiSupport = guiSupport;
        }

        /// <summary>
        /// Clear function.
        /// </summary>
        void Clear()
        {
            // clear list
            compareList.Clear();
        }

        /// <summary>
        /// Copy function.
        /// </summary>
        void Copy(EIB_TreeList ^ source)
        {
            // clear list
            Clear();
            // copy common EIB parameters
            common.Copy(source->Common);
            // copy EIB parameters from tree list
            UInt32 eibCount = source->EibCount;
            for (UInt32 i = 0; i < eibCount; i ++)
            {
                EIB_Param ^ eibParam = gcnew EIB_Param;
                eibParam->Copy(source->EibEntry[i]->Parameter);
                compareList[source->EibKey[i]] = eibParam;
            }
        }

        /// <summary>
        /// Compare function.
        /// </summary>
		/// <param name="source">The source parameters.</param>
		/// <param name="checkNoEibParam">If true check parameters not send to EIB.</param>
        bool IsEqual(EIB_TreeList ^ source, bool checkNoEibParam)
        {
            // check EIB number
            UInt32 eibCount = source->EibCount;
            if (Int32(eibCount) != compareList.Count) return false;
            // check common EIB parameters
            if (common.IsEqual(source->Common, checkNoEibParam) == false) return false;
            // check parameters
            String ^ eibKey;
            for (UInt32 i = 0; i < eibCount; i ++)
            {
                eibKey = source->EibKey[i];
                if (compareList.ContainsKey(eibKey) == false) return false; 
                if (compareList[eibKey]->IsEqual(source->EibEntry[i]->Parameter) == false)
                {
                    return false;
                }
            }
            return true;
        }

        /// <summary>
        /// Gets the number of EIBs in the compare list.
        /// </summary>
        property UInt32 EibCount
        {
            UInt32 get() { return compareList.Count; }
        }
    };
	#pragma endregion

 	#pragma region "EIB_SaveList class definition"
    /// <summary>
    /// This class is used to compare the parameters
    /// that have been saved to project file
    /// with the current parameters.
    /// </summary>
    public ref class EIB_SaveList : public EIB_CompareList
    {
    private:

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
        EIB_SaveList(GUI_Support ^ guiSupport) : EIB_CompareList(guiSupport)
        {
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_TreeList ^ source)
        {
            return EIB_CompareList::IsEqual(source, true);
        }
    };
	#pragma endregion

 	#pragma region "EIB_InitList class definition"
    /// <summary>
    /// This class is used to compare the parameters
    /// that have been used for EIB initialization
    /// with the current parameters.
    /// </summary>
    public ref class EIB_InitList : public EIB_CompareList
    {
    private:
        EIB_TreeList ^      eibTreeList;        ///< the EIB tree list

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
		/// <param name="guiSupport">The GUI support class.</param>
		/// <param name="eibTreeList">The EIB tree list.</param>
        EIB_InitList(GUI_Support ^ guiSupport,
                     EIB_TreeList ^ eibTreeList)
            : EIB_CompareList(guiSupport)
        {
            this->eibTreeList = eibTreeList;
        }

        /// <summary>
        /// Compare function.
        /// </summary>
        bool IsEqual(EIB_TreeList ^ source)
        {
            return EIB_CompareList::IsEqual(source, false);
        }

        /// <summary>
        /// Gets the parametrs from EIB selected in the Main tree View.
        /// </summary>
        property EIB_Param ^ SelectedEibParam
        {
            EIB_Param ^ get()
            {
                // get key from selected EIB
                String ^ eibKey = eibTreeList->EibKey;
                // check if selected EIB is in init list
                if (compareList.ContainsKey(eibKey) == true)
                {
                    return compareList[eibKey];
                }
                return nullptr;
            }
        }
    };
	#pragma endregion
}
