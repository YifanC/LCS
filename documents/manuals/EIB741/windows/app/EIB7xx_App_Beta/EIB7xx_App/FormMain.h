///
/// \file       FormMain.h
///
/// \copyright  (C) 2011 DR. JOHANNES HEIDENHAIN GmbH
///
/// \author     Dr. Thomas Rohn
///
/// $Id: FormMain.h,v 1.3 2011/12/16 13:37:55 rohn Exp $
/// $Name:  $
/// $Revision: 1.3 $
/// $State: Exp $
/// $Log: FormMain.h,v $
/// Revision 1.3  2011/12/16 13:37:55  rohn
/// Version 0.9.0.1 (siehe History.txt)
///
/// Revision 1.2  2011/09/20 12:24:35  rohn
/// Version 0.3.0.2 (siehe History.txt)
///
/// Revision 1.1.1.1  2011/08/12 09:23:03  rohn
/// initial version by import
///
///
/// \brief      Main window class.
///
/// This file will be changed by the windows forms designer.
/// Be very carefully in changing anything in this file!
///

#pragma once

#include "eib7.h"

#include "Defines.h"
#include "EIB_FileBase.h"
#include "EIB_BinFileBase.h"
#include "GUI_ParamFile.h"
#include "GUI_Config.h"
#include "GUI_Support.h"
#include "GUI_State.h"
#include "EIB_ParamFile.h"
#include "EIB_DataFile.h"
#include "EIB_Param.h"
#include "EIB_Display.h"
#include "Properties.h"
#include "PropertyBase.h"
#include "DisplayPolarGraph.h"
#include "CrankshaftParam.h"
#include "CrankshaftRefFile.h"
#include "CrankshaftDisplay.h"
#include "CrankshaftMeasure.h"
#include "EIB_ReadParam.h"
#include "EIB_Control.h"
#include "PropertyAxisBase.h"
#include "PropertyAxis.h"
#include "PropertyAuxAxis.h"
#include "PropertyNetwork.h"
#include "PropertyTrigger.h"
#include "EIB_CommonParam.h"
#include "EIB_Entry.h"
#include "EIB_List.h"
#include "PropertyDataPacket.h"
#include "PropertyOpMode.h"
#include "PropertyCrankshaft.h"
#include "PropertyDisplay.h"
#include "DisplayBase.h"
#include "DisplayPosition.h"
#include "DisplayDataPacket.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Resources;


namespace EIB7xx_App {

	/// <summary>
	/// Zusammenfassung für FormMain
	///
	/// Warnung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie auch
	///          die Ressourcendateiname-Eigenschaft für das Tool zur Kompilierung verwalteter Ressourcen ändern,
	///          das allen RESX-Dateien zugewiesen ist, von denen diese Klasse abhängt.
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class FormMain : public System::Windows::Forms::Form
	{
	public:
		FormMain(String ^ setLanguage, String ^ setApplication, String ^ setPassword)
		{
            isClosing = false;
            initReady = false;
            // pre component initialization
            PreInitializeComponent(setLanguage, setApplication, setPassword);

            // TODO: exception "System.Reflection.AmbiguousMatchException" in
            // resources->ApplyResources(this->tabControlMain, L"tabControlMain");
            // component initialization
            InitializeComponent();

            initReady = true;
            // after component initialization
            PostInitializeComponent();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~FormMain()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelMain;
    private: System::Windows::Forms::ToolStrip^  toolStripTree;
    private: System::Windows::Forms::ToolStripButton^  toolStripButtonEibAdd1;
    private: System::Windows::Forms::ToolStripButton^  toolStripButtonEibDel1;
    private: System::Windows::Forms::StatusStrip^  statusStripStatus;
    private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabelText;
    private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabelValue;
    private: System::Windows::Forms::SplitContainer^  splitContainerH;
    private: System::Windows::Forms::RichTextBox^  richTextBoxReport;
    private: System::Windows::Forms::SplitContainer^  splitContainerV;
    private: System::Windows::Forms::TreeView^  treeViewMain;
    private: System::Windows::Forms::TabControl^  tabControlMain;
    private: System::Windows::Forms::TabPage^  tabPageNetwork;
    private: System::Windows::Forms::TabPage^  tabPageAxis;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelTree;
    private: System::Windows::Forms::ToolStripButton^  toolStripButtonInit1;
    private: System::Windows::Forms::ToolStripButton^  toolStripButtonRun1;
    private: System::Windows::Forms::PictureBox^  pictureBoxGreenWhite;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelTopV;
    private: System::Windows::Forms::PictureBox^  pictureBoxJH;
    private: System::Windows::Forms::Panel^  panelGreenWhite;
    private: System::Windows::Forms::MenuStrip^  menuStripMain;
    private: System::Windows::Forms::ToolStripMenuItem^  dateiToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  projektOeffnenToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  projektSpeichernToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  infoToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  beendenToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  konfigurationToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  anzeigeToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  projektSpeichernUnterToolStripMenuItem;
    private: System::Windows::Forms::TabPage^  tabPageDataPacket;
    private: System::Windows::Forms::TabPage^  tabPageTrigger;
    private: System::Windows::Forms::TabPage^  tabPageOperationMode;
    private: System::Windows::Forms::TabPage^  tabPageSummary;
    private: System::Windows::Forms::TabPage^  tabPageDetail;
    private: System::Windows::Forms::PropertyGrid^  propertyGridNetwork;
    private: System::Windows::Forms::ImageList^  imageListTree1;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelTopL;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelTopR;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelDiadur;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelJH;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelGreenWhite;
    private: System::Windows::Forms::PictureBox^  pictureBoxDiadur1;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelNetwork;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelAxis;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelDataPacket;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelNetworkRead;
    private: System::Windows::Forms::Button^  buttonNetworkRead;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
    private: System::Windows::Forms::ToolStripButton^  toolStripButtonStop;
    private: System::Windows::Forms::ToolStripMenuItem^  clearReportListToolStripMenuItem;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
    private: System::Windows::Forms::TabPage^  tabPageOptions;
    private: System::Windows::Forms::PropertyGrid^  propertyGridAxis;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelTrigger;
    private: System::Windows::Forms::PropertyGrid^  propertyGridTrigger;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelOpMode;
    private: System::Windows::Forms::PropertyGrid^  propertyGridOpMode;
    private: System::Windows::Forms::Button^  buttonNetworkWrite;
    private: System::Windows::Forms::DataGridView^  dataGridViewDataPacket;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Header;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  DataPacketColumnAxis1;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelSummary;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelSummaryPos;
    private: System::Windows::Forms::ProgressBar^  progressBarFillLevel;

    private: System::Windows::Forms::Panel^  panelRecordProgress;
    private: System::Windows::Forms::Label^  labelFillLevel;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelDetail;
    private: System::Windows::Forms::Panel^  panelRecordIndex;
private: System::Windows::Forms::TrackBar^  trackBarIndex;

private: System::Windows::Forms::NumericUpDown^  numericUpDownIndex;

    private: System::Windows::Forms::Timer^  timerDisplay;
    private: System::Windows::Forms::ImageList^  imageListDisplayLed;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelDetailPos;
private: System::Windows::Forms::DataGridView^  dataGridViewDetailDataPacket;



private: System::Windows::Forms::Label^  labelMaxIndex;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelDetailStatus;
    private: System::Windows::Forms::Panel^  panelDisplayAxis0;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelMainAxis0;
    private: System::Windows::Forms::TextBox^  textBoxPosAxis0;
    private: System::Windows::Forms::Label^  labelUnitAxis0;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelButtonAxis0;
    private: System::Windows::Forms::Button^  buttonClearAxis0;
    private: System::Windows::Forms::Button^  buttonRefAxis0;
    private: System::Windows::Forms::Label^  labelAxis0;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelLedAxis0;
    private: System::Windows::Forms::PictureBox^  pictureBoxTriggerAxis0;
    private: System::Windows::Forms::PictureBox^  pictureBoxStatusAxis0;
    private: System::Windows::Forms::PictureBox^  pictureBoxRefAxis0;
    private: System::Windows::Forms::Label^  labelStatusAxis0;
    private: System::Windows::Forms::Label^  labelTriggerAxis0;
    private: System::Windows::Forms::Label^  labelRefAxis0;
private: System::Windows::Forms::Label^  labelIndex;

private: System::Windows::Forms::Panel^  panelDisplayAxis1;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
private: System::Windows::Forms::TextBox^  textBoxPosAxis1;
private: System::Windows::Forms::Label^  labelUnitAxis1;


    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel2;
private: System::Windows::Forms::Button^  buttonClearAxis1;

private: System::Windows::Forms::Button^  buttonRefAxis1;

private: System::Windows::Forms::Label^  labelAxis1;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel3;
private: System::Windows::Forms::PictureBox^  pictureBoxTriggerAxis1;

private: System::Windows::Forms::PictureBox^  pictureBoxStatusAxis1;
private: System::Windows::Forms::PictureBox^  pictureBoxRefAxis1;
private: System::Windows::Forms::Label^  labelStatusAxis1;
private: System::Windows::Forms::Label^  labelTriggerAxis1;




private: System::Windows::Forms::Label^  labelRefAxis1;

private: System::Windows::Forms::Panel^  panelDisplayAxis2;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel4;
private: System::Windows::Forms::TextBox^  textBoxPosAxis2;
private: System::Windows::Forms::Label^  labelUnitAxis2;


    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel5;
private: System::Windows::Forms::Button^  buttonClearAxis2;

private: System::Windows::Forms::Button^  buttonRefAxis2;

private: System::Windows::Forms::Label^  labelAxis2;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel6;
private: System::Windows::Forms::PictureBox^  pictureBoxTriggerAxis2;

private: System::Windows::Forms::PictureBox^  pictureBoxStatusAxis2;
private: System::Windows::Forms::PictureBox^  pictureBoxRefAxis2;
private: System::Windows::Forms::Label^  labelStatusAxis2;
private: System::Windows::Forms::Label^  labelTriggerAxis2;




private: System::Windows::Forms::Label^  labelRefAxis2;

private: System::Windows::Forms::Panel^  panelDisplayAxis3;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel7;
private: System::Windows::Forms::TextBox^  textBoxPosAxis3;
private: System::Windows::Forms::Label^  labelUnitAxis3;


    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel8;
private: System::Windows::Forms::Button^  buttonClearAxis3;

private: System::Windows::Forms::Button^  buttonRefAxis3;

private: System::Windows::Forms::Label^  labelAxis3;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel9;
private: System::Windows::Forms::PictureBox^  pictureBoxTriggerAxis3;

private: System::Windows::Forms::PictureBox^  pictureBoxStatusAxis3;
private: System::Windows::Forms::PictureBox^  pictureBoxRefAxis3;
private: System::Windows::Forms::Label^  labelStatusAxis3;
private: System::Windows::Forms::Label^  labelTriggerAxis3;




private: System::Windows::Forms::Label^  labelRefAxis3;

private: System::Windows::Forms::Panel^  panelDisplayAxis4;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel13;
private: System::Windows::Forms::TextBox^  textBoxPosAxis4;
private: System::Windows::Forms::Label^  labelUnitAxis4;


    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel14;
private: System::Windows::Forms::Button^  buttonClearAxis4;

private: System::Windows::Forms::Button^  buttonRefAxis4;

private: System::Windows::Forms::Label^  labelAxis4;

    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel15;
private: System::Windows::Forms::PictureBox^  pictureBoxTriggerAxis4;

private: System::Windows::Forms::PictureBox^  pictureBoxStatusAxis4;
private: System::Windows::Forms::PictureBox^  pictureBoxRefAxis4;
private: System::Windows::Forms::Label^  labelStatusAxis4;
private: System::Windows::Forms::Label^  labelTriggerAxis4;




private: System::Windows::Forms::Label^  labelRefAxis4;

    private: System::Windows::Forms::Panel^  panel4;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel10;
    private: System::Windows::Forms::TextBox^  textBox4;
    private: System::Windows::Forms::Label^  label16;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel11;
    private: System::Windows::Forms::Button^  button7;
    private: System::Windows::Forms::Button^  button8;
    private: System::Windows::Forms::Label^  label17;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel12;
    private: System::Windows::Forms::PictureBox^  pictureBox10;
    private: System::Windows::Forms::PictureBox^  pictureBox11;
    private: System::Windows::Forms::PictureBox^  pictureBox12;
    private: System::Windows::Forms::Label^  label18;
    private: System::Windows::Forms::Label^  label19;
    private: System::Windows::Forms::Label^  label20;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutDisplay;
    private: System::Windows::Forms::PropertyGrid^  propertyGridDisplay;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelRecordProgress;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutRecordIndex;
private: System::Windows::Forms::Button^  buttonRecordDisplayRun;
private: System::Windows::Forms::ImageList^  imageListRunMode;

private: System::Windows::Forms::Button^  buttonEibReset;


private: System::Windows::Forms::DataGridView^  dataGridViewDetailStatus;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumnName;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumnValue;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumnReset;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn1;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn2;
private: System::Windows::Forms::Button^  buttonSaveRecordingData;
private: System::Windows::Forms::TabPage^  tabPageCrankshaftConfig;
private: System::Windows::Forms::TabPage^  tabPageCrankshaftDisplay;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelCrankshaftConfig;
private: System::Windows::Forms::PropertyGrid^  propertyGridCrankshaft;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelCrankshaftSummary;


private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelCrankshartGraph;







private: System::Windows::Forms::Panel^  panelMeasurePoint1;






private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelCrankshaftDisplay;
private: System::Windows::Forms::Panel^  panelMeasurePoint2;
private: System::Windows::Forms::Panel^  panelMeasurePoint3;
private: System::Windows::Forms::Panel^  panelMeasurePoint4;



private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelCrankshaftReference;
private: System::Windows::Forms::Button^  buttonCrankshaftRefLoad;
private: System::Windows::Forms::Button^  buttonCrankshaftRefClear;



private: System::Windows::Forms::Button^  buttonCrankshaftRefSave;


private: System::Windows::Forms::Panel^  panelGraphAreaCdMp1;
private: System::Windows::Forms::GroupBox^  groupBoxCrankshaftRef;
private: System::Windows::Forms::Label^  labelGD0Grd;


private: System::Windows::Forms::ToolStrip^  toolStripCdMp1;
private: System::Windows::Forms::ToolStripLabel^  toolStripCdLabelMp1;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelGD1;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelGD2;
private: System::Windows::Forms::Label^  labelGD90Grd;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelGD3;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPaneGD5;

private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanelGD4;
private: System::Windows::Forms::Label^  labelGD270Grd;
private: System::Windows::Forms::Label^  labelYmaxCdMp1;
private: System::Windows::Forms::Label^  labelYminCdMp1;
private: System::Windows::Forms::Label^  labelGD180Grd;
private: System::Windows::Forms::Label^  labelXmaxCdMp1;
private: System::Windows::Forms::Label^  labelXminCdMp1;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel16;
private: System::Windows::Forms::CheckBox^  checkBoxStopAt360;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel17;
private: System::Windows::Forms::ToolStrip^  toolStripCdMp2;
private: System::Windows::Forms::ToolStripLabel^  toolStripCdLabelMp2;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel18;
private: System::Windows::Forms::Panel^  panelGraphAreaCdMp2;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel19;
private: System::Windows::Forms::Label^  labelYmaxCdMp2;
private: System::Windows::Forms::Label^  labelYminCdMp2;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel20;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel21;
private: System::Windows::Forms::Label^  labelXminCdMp2;
private: System::Windows::Forms::Label^  labelXmaxCdMp2;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel22;
private: System::Windows::Forms::ToolStrip^  toolStripCdMp3;
private: System::Windows::Forms::ToolStripLabel^  toolStripCdLabelMp3;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel23;
private: System::Windows::Forms::Panel^  panelGraphAreaCdMp3;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel24;
private: System::Windows::Forms::Label^  labelYmaxCdMp3;
private: System::Windows::Forms::Label^  labelYminCdMp3;
private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel25;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel26;
private: System::Windows::Forms::Label^  labelXminCdMp3;
private: System::Windows::Forms::Label^  labelXmaxCdMp3;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel27;
private: System::Windows::Forms::ToolStrip^  toolStripCdMp4;
private: System::Windows::Forms::ToolStripLabel^  toolStripCdLabelMp4;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel28;
private: System::Windows::Forms::Panel^  panelGraphAreaCdMp4;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel29;
private: System::Windows::Forms::Label^  labelYmaxCdMp4;
private: System::Windows::Forms::Label^  labelYminCdMp4;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel30;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel31;
private: System::Windows::Forms::Label^  labelXminCdMp4;
private: System::Windows::Forms::Label^  labelXmaxCdMp4;
private: System::Windows::Forms::Label^  label29;
private: System::Windows::Forms::Button^  buttonEibFirmware;
private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBarStatus;
private: System::Windows::Forms::CheckBox^  checkBoxDebug;










































































































    private: System::ComponentModel::IContainer^  components;

    protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent()
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle6 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle5 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle10 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle7 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle8 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle9 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->splitContainerH = (gcnew System::Windows::Forms::SplitContainer());
            this->splitContainerV = (gcnew System::Windows::Forms::SplitContainer());
            this->tableLayoutPanelTree = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->treeViewMain = (gcnew System::Windows::Forms::TreeView());
            this->imageListTree1 = (gcnew System::Windows::Forms::ImageList(this->components));
            this->toolStripTree = (gcnew System::Windows::Forms::ToolStrip());
            this->toolStripButtonEibAdd1 = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripButtonEibDel1 = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolStripButtonInit1 = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolStripButtonRun1 = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripButtonStop = (gcnew System::Windows::Forms::ToolStripButton());
            this->tabControlMain = (gcnew System::Windows::Forms::TabControl());
            this->tabPageNetwork = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelNetwork = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelNetworkRead = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->buttonNetworkRead = (gcnew System::Windows::Forms::Button());
            this->buttonNetworkWrite = (gcnew System::Windows::Forms::Button());
            this->buttonEibReset = (gcnew System::Windows::Forms::Button());
            this->buttonEibFirmware = (gcnew System::Windows::Forms::Button());
            this->propertyGridNetwork = (gcnew System::Windows::Forms::PropertyGrid());
            this->tabPageAxis = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelAxis = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->propertyGridAxis = (gcnew System::Windows::Forms::PropertyGrid());
            this->tabPageDataPacket = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelDataPacket = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->dataGridViewDataPacket = (gcnew System::Windows::Forms::DataGridView());
            this->Header = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->DataPacketColumnAxis1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->tabPageTrigger = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelTrigger = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->propertyGridTrigger = (gcnew System::Windows::Forms::PropertyGrid());
            this->tabPageOperationMode = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelOpMode = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->propertyGridOpMode = (gcnew System::Windows::Forms::PropertyGrid());
            this->tabPageCrankshaftConfig = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelCrankshaftConfig = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->propertyGridCrankshaft = (gcnew System::Windows::Forms::PropertyGrid());
            this->tabPageSummary = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelSummary = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->panelRecordProgress = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanelRecordProgress = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelFillLevel = (gcnew System::Windows::Forms::Label());
            this->progressBarFillLevel = (gcnew System::Windows::Forms::ProgressBar());
            this->tableLayoutPanelSummaryPos = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->panelDisplayAxis3 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanel7 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->textBoxPosAxis3 = (gcnew System::Windows::Forms::TextBox());
            this->labelUnitAxis3 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel8 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->buttonClearAxis3 = (gcnew System::Windows::Forms::Button());
            this->buttonRefAxis3 = (gcnew System::Windows::Forms::Button());
            this->labelAxis3 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel9 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->pictureBoxTriggerAxis3 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxStatusAxis3 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxRefAxis3 = (gcnew System::Windows::Forms::PictureBox());
            this->labelStatusAxis3 = (gcnew System::Windows::Forms::Label());
            this->labelTriggerAxis3 = (gcnew System::Windows::Forms::Label());
            this->labelRefAxis3 = (gcnew System::Windows::Forms::Label());
            this->panelDisplayAxis1 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->textBoxPosAxis1 = (gcnew System::Windows::Forms::TextBox());
            this->labelUnitAxis1 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->buttonClearAxis1 = (gcnew System::Windows::Forms::Button());
            this->buttonRefAxis1 = (gcnew System::Windows::Forms::Button());
            this->labelAxis1 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel3 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->pictureBoxTriggerAxis1 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxStatusAxis1 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxRefAxis1 = (gcnew System::Windows::Forms::PictureBox());
            this->labelStatusAxis1 = (gcnew System::Windows::Forms::Label());
            this->labelTriggerAxis1 = (gcnew System::Windows::Forms::Label());
            this->labelRefAxis1 = (gcnew System::Windows::Forms::Label());
            this->panelDisplayAxis2 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanel4 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->textBoxPosAxis2 = (gcnew System::Windows::Forms::TextBox());
            this->labelUnitAxis2 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel5 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->buttonClearAxis2 = (gcnew System::Windows::Forms::Button());
            this->buttonRefAxis2 = (gcnew System::Windows::Forms::Button());
            this->labelAxis2 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel6 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->pictureBoxTriggerAxis2 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxStatusAxis2 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxRefAxis2 = (gcnew System::Windows::Forms::PictureBox());
            this->labelStatusAxis2 = (gcnew System::Windows::Forms::Label());
            this->labelTriggerAxis2 = (gcnew System::Windows::Forms::Label());
            this->labelRefAxis2 = (gcnew System::Windows::Forms::Label());
            this->panelDisplayAxis4 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanel13 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->textBoxPosAxis4 = (gcnew System::Windows::Forms::TextBox());
            this->labelUnitAxis4 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel14 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->buttonClearAxis4 = (gcnew System::Windows::Forms::Button());
            this->buttonRefAxis4 = (gcnew System::Windows::Forms::Button());
            this->labelAxis4 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel15 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->pictureBoxTriggerAxis4 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxStatusAxis4 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxRefAxis4 = (gcnew System::Windows::Forms::PictureBox());
            this->labelStatusAxis4 = (gcnew System::Windows::Forms::Label());
            this->labelTriggerAxis4 = (gcnew System::Windows::Forms::Label());
            this->labelRefAxis4 = (gcnew System::Windows::Forms::Label());
            this->tabPageDetail = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelDetail = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelDetailPos = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->dataGridViewDetailDataPacket = (gcnew System::Windows::Forms::DataGridView());
            this->dataGridViewTextBoxColumn1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->dataGridViewTextBoxColumn2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->tableLayoutPanelDetailStatus = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->dataGridViewDetailStatus = (gcnew System::Windows::Forms::DataGridView());
            this->dataGridViewTextBoxColumnName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->dataGridViewTextBoxColumnValue = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->dataGridViewTextBoxColumnReset = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->panelDisplayAxis0 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanelMainAxis0 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->textBoxPosAxis0 = (gcnew System::Windows::Forms::TextBox());
            this->labelUnitAxis0 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanelButtonAxis0 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->buttonClearAxis0 = (gcnew System::Windows::Forms::Button());
            this->buttonRefAxis0 = (gcnew System::Windows::Forms::Button());
            this->labelAxis0 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanelLedAxis0 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->pictureBoxTriggerAxis0 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxStatusAxis0 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxRefAxis0 = (gcnew System::Windows::Forms::PictureBox());
            this->labelStatusAxis0 = (gcnew System::Windows::Forms::Label());
            this->labelTriggerAxis0 = (gcnew System::Windows::Forms::Label());
            this->labelRefAxis0 = (gcnew System::Windows::Forms::Label());
            this->panelRecordIndex = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutRecordIndex = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelIndex = (gcnew System::Windows::Forms::Label());
            this->labelMaxIndex = (gcnew System::Windows::Forms::Label());
            this->numericUpDownIndex = (gcnew System::Windows::Forms::NumericUpDown());
            this->trackBarIndex = (gcnew System::Windows::Forms::TrackBar());
            this->buttonRecordDisplayRun = (gcnew System::Windows::Forms::Button());
            this->imageListRunMode = (gcnew System::Windows::Forms::ImageList(this->components));
            this->buttonSaveRecordingData = (gcnew System::Windows::Forms::Button());
            this->tabPageCrankshaftDisplay = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutPanelCrankshaftSummary = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelCrankshaftDisplay = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelCrankshartGraph = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->panelMeasurePoint1 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanelGD1 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->toolStripCdMp1 = (gcnew System::Windows::Forms::ToolStrip());
            this->toolStripCdLabelMp1 = (gcnew System::Windows::Forms::ToolStripLabel());
            this->tableLayoutPanelGD2 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->panelGraphAreaCdMp1 = (gcnew System::Windows::Forms::Panel());
            this->labelGD0Grd = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPaneGD5 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelYmaxCdMp1 = (gcnew System::Windows::Forms::Label());
            this->labelYminCdMp1 = (gcnew System::Windows::Forms::Label());
            this->labelGD180Grd = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanelGD3 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelGD90Grd = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanelGD4 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelXminCdMp1 = (gcnew System::Windows::Forms::Label());
            this->labelXmaxCdMp1 = (gcnew System::Windows::Forms::Label());
            this->labelGD270Grd = (gcnew System::Windows::Forms::Label());
            this->panelMeasurePoint2 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanel17 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->toolStripCdMp2 = (gcnew System::Windows::Forms::ToolStrip());
            this->toolStripCdLabelMp2 = (gcnew System::Windows::Forms::ToolStripLabel());
            this->tableLayoutPanel18 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->panelGraphAreaCdMp2 = (gcnew System::Windows::Forms::Panel());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel19 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelYmaxCdMp2 = (gcnew System::Windows::Forms::Label());
            this->labelYminCdMp2 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel20 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel21 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelXminCdMp2 = (gcnew System::Windows::Forms::Label());
            this->labelXmaxCdMp2 = (gcnew System::Windows::Forms::Label());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->panelMeasurePoint3 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanel22 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->toolStripCdMp3 = (gcnew System::Windows::Forms::ToolStrip());
            this->toolStripCdLabelMp3 = (gcnew System::Windows::Forms::ToolStripLabel());
            this->tableLayoutPanel23 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->panelGraphAreaCdMp3 = (gcnew System::Windows::Forms::Panel());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel24 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelYmaxCdMp3 = (gcnew System::Windows::Forms::Label());
            this->labelYminCdMp3 = (gcnew System::Windows::Forms::Label());
            this->label12 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel25 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->label13 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel26 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelXminCdMp3 = (gcnew System::Windows::Forms::Label());
            this->labelXmaxCdMp3 = (gcnew System::Windows::Forms::Label());
            this->label21 = (gcnew System::Windows::Forms::Label());
            this->panelMeasurePoint4 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanel27 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->toolStripCdMp4 = (gcnew System::Windows::Forms::ToolStrip());
            this->toolStripCdLabelMp4 = (gcnew System::Windows::Forms::ToolStripLabel());
            this->tableLayoutPanel28 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->panelGraphAreaCdMp4 = (gcnew System::Windows::Forms::Panel());
            this->label22 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel29 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelYmaxCdMp4 = (gcnew System::Windows::Forms::Label());
            this->labelYminCdMp4 = (gcnew System::Windows::Forms::Label());
            this->label25 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel30 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->label26 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel31 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->labelXminCdMp4 = (gcnew System::Windows::Forms::Label());
            this->labelXmaxCdMp4 = (gcnew System::Windows::Forms::Label());
            this->label29 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel16 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->groupBoxCrankshaftRef = (gcnew System::Windows::Forms::GroupBox());
            this->tableLayoutPanelCrankshaftReference = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->buttonCrankshaftRefClear = (gcnew System::Windows::Forms::Button());
            this->buttonCrankshaftRefSave = (gcnew System::Windows::Forms::Button());
            this->buttonCrankshaftRefLoad = (gcnew System::Windows::Forms::Button());
            this->checkBoxStopAt360 = (gcnew System::Windows::Forms::CheckBox());
            this->tabPageOptions = (gcnew System::Windows::Forms::TabPage());
            this->tableLayoutDisplay = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->propertyGridDisplay = (gcnew System::Windows::Forms::PropertyGrid());
            this->richTextBoxReport = (gcnew System::Windows::Forms::RichTextBox());
            this->pictureBoxJH = (gcnew System::Windows::Forms::PictureBox());
            this->panelGreenWhite = (gcnew System::Windows::Forms::Panel());
            this->menuStripMain = (gcnew System::Windows::Forms::MenuStrip());
            this->dateiToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->projektOeffnenToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->projektSpeichernToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->projektSpeichernUnterToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->clearReportListToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->infoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->beendenToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->konfigurationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->anzeigeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->pictureBoxGreenWhite = (gcnew System::Windows::Forms::PictureBox());
            this->tableLayoutPanelMain = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelTopV = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelTopL = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelDiadur = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->pictureBoxDiadur1 = (gcnew System::Windows::Forms::PictureBox());
            this->tableLayoutPanelTopR = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelJH = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->tableLayoutPanelGreenWhite = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->statusStripStatus = (gcnew System::Windows::Forms::StatusStrip());
            this->toolStripStatusLabelText = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->toolStripStatusLabelValue = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->toolStripProgressBarStatus = (gcnew System::Windows::Forms::ToolStripProgressBar());
            this->timerDisplay = (gcnew System::Windows::Forms::Timer(this->components));
            this->imageListDisplayLed = (gcnew System::Windows::Forms::ImageList(this->components));
            this->panel4 = (gcnew System::Windows::Forms::Panel());
            this->tableLayoutPanel10 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->textBox4 = (gcnew System::Windows::Forms::TextBox());
            this->label16 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel11 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->button7 = (gcnew System::Windows::Forms::Button());
            this->button8 = (gcnew System::Windows::Forms::Button());
            this->label17 = (gcnew System::Windows::Forms::Label());
            this->tableLayoutPanel12 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->pictureBox10 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBox11 = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBox12 = (gcnew System::Windows::Forms::PictureBox());
            this->label18 = (gcnew System::Windows::Forms::Label());
            this->label19 = (gcnew System::Windows::Forms::Label());
            this->label20 = (gcnew System::Windows::Forms::Label());
            this->checkBoxDebug = (gcnew System::Windows::Forms::CheckBox());
            this->splitContainerH->Panel1->SuspendLayout();
            this->splitContainerH->Panel2->SuspendLayout();
            this->splitContainerH->SuspendLayout();
            this->splitContainerV->Panel1->SuspendLayout();
            this->splitContainerV->Panel2->SuspendLayout();
            this->splitContainerV->SuspendLayout();
            this->tableLayoutPanelTree->SuspendLayout();
            this->toolStripTree->SuspendLayout();
            this->tabControlMain->SuspendLayout();
            this->tabPageNetwork->SuspendLayout();
            this->tableLayoutPanelNetwork->SuspendLayout();
            this->tableLayoutPanelNetworkRead->SuspendLayout();
            this->tabPageAxis->SuspendLayout();
            this->tableLayoutPanelAxis->SuspendLayout();
            this->tabPageDataPacket->SuspendLayout();
            this->tableLayoutPanelDataPacket->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewDataPacket))->BeginInit();
            this->tabPageTrigger->SuspendLayout();
            this->tableLayoutPanelTrigger->SuspendLayout();
            this->tabPageOperationMode->SuspendLayout();
            this->tableLayoutPanelOpMode->SuspendLayout();
            this->tabPageCrankshaftConfig->SuspendLayout();
            this->tableLayoutPanelCrankshaftConfig->SuspendLayout();
            this->tabPageSummary->SuspendLayout();
            this->tableLayoutPanelSummary->SuspendLayout();
            this->panelRecordProgress->SuspendLayout();
            this->tableLayoutPanelRecordProgress->SuspendLayout();
            this->tableLayoutPanelSummaryPos->SuspendLayout();
            this->panelDisplayAxis3->SuspendLayout();
            this->tableLayoutPanel7->SuspendLayout();
            this->tableLayoutPanel8->SuspendLayout();
            this->tableLayoutPanel9->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis3))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis3))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis3))->BeginInit();
            this->panelDisplayAxis1->SuspendLayout();
            this->tableLayoutPanel1->SuspendLayout();
            this->tableLayoutPanel2->SuspendLayout();
            this->tableLayoutPanel3->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis1))->BeginInit();
            this->panelDisplayAxis2->SuspendLayout();
            this->tableLayoutPanel4->SuspendLayout();
            this->tableLayoutPanel5->SuspendLayout();
            this->tableLayoutPanel6->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis2))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis2))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis2))->BeginInit();
            this->panelDisplayAxis4->SuspendLayout();
            this->tableLayoutPanel13->SuspendLayout();
            this->tableLayoutPanel14->SuspendLayout();
            this->tableLayoutPanel15->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis4))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis4))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis4))->BeginInit();
            this->tabPageDetail->SuspendLayout();
            this->tableLayoutPanelDetail->SuspendLayout();
            this->tableLayoutPanelDetailPos->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewDetailDataPacket))->BeginInit();
            this->tableLayoutPanelDetailStatus->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewDetailStatus))->BeginInit();
            this->panelDisplayAxis0->SuspendLayout();
            this->tableLayoutPanelMainAxis0->SuspendLayout();
            this->tableLayoutPanelButtonAxis0->SuspendLayout();
            this->tableLayoutPanelLedAxis0->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis0))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis0))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis0))->BeginInit();
            this->panelRecordIndex->SuspendLayout();
            this->tableLayoutRecordIndex->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownIndex))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBarIndex))->BeginInit();
            this->tabPageCrankshaftDisplay->SuspendLayout();
            this->tableLayoutPanelCrankshaftSummary->SuspendLayout();
            this->tableLayoutPanelCrankshaftDisplay->SuspendLayout();
            this->tableLayoutPanelCrankshartGraph->SuspendLayout();
            this->panelMeasurePoint1->SuspendLayout();
            this->tableLayoutPanelGD1->SuspendLayout();
            this->toolStripCdMp1->SuspendLayout();
            this->tableLayoutPanelGD2->SuspendLayout();
            this->tableLayoutPaneGD5->SuspendLayout();
            this->tableLayoutPanelGD3->SuspendLayout();
            this->tableLayoutPanelGD4->SuspendLayout();
            this->panelMeasurePoint2->SuspendLayout();
            this->tableLayoutPanel17->SuspendLayout();
            this->toolStripCdMp2->SuspendLayout();
            this->tableLayoutPanel18->SuspendLayout();
            this->tableLayoutPanel19->SuspendLayout();
            this->tableLayoutPanel20->SuspendLayout();
            this->tableLayoutPanel21->SuspendLayout();
            this->panelMeasurePoint3->SuspendLayout();
            this->tableLayoutPanel22->SuspendLayout();
            this->toolStripCdMp3->SuspendLayout();
            this->tableLayoutPanel23->SuspendLayout();
            this->tableLayoutPanel24->SuspendLayout();
            this->tableLayoutPanel25->SuspendLayout();
            this->tableLayoutPanel26->SuspendLayout();
            this->panelMeasurePoint4->SuspendLayout();
            this->tableLayoutPanel27->SuspendLayout();
            this->toolStripCdMp4->SuspendLayout();
            this->tableLayoutPanel28->SuspendLayout();
            this->tableLayoutPanel29->SuspendLayout();
            this->tableLayoutPanel30->SuspendLayout();
            this->tableLayoutPanel31->SuspendLayout();
            this->tableLayoutPanel16->SuspendLayout();
            this->groupBoxCrankshaftRef->SuspendLayout();
            this->tableLayoutPanelCrankshaftReference->SuspendLayout();
            this->tabPageOptions->SuspendLayout();
            this->tableLayoutDisplay->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxJH))->BeginInit();
            this->menuStripMain->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxGreenWhite))->BeginInit();
            this->tableLayoutPanelMain->SuspendLayout();
            this->tableLayoutPanelTopV->SuspendLayout();
            this->tableLayoutPanelTopL->SuspendLayout();
            this->tableLayoutPanelDiadur->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxDiadur1))->BeginInit();
            this->tableLayoutPanelTopR->SuspendLayout();
            this->tableLayoutPanelJH->SuspendLayout();
            this->tableLayoutPanelGreenWhite->SuspendLayout();
            this->statusStripStatus->SuspendLayout();
            this->tableLayoutPanel10->SuspendLayout();
            this->tableLayoutPanel11->SuspendLayout();
            this->tableLayoutPanel12->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox10))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox11))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox12))->BeginInit();
            this->SuspendLayout();
            // 
            // splitContainerH
            // 
            this->splitContainerH->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->splitContainerH, L"splitContainerH");
            this->splitContainerH->Name = L"splitContainerH";
            // 
            // splitContainerH.Panel1
            // 
            this->splitContainerH->Panel1->Controls->Add(this->splitContainerV);
            // 
            // splitContainerH.Panel2
            // 
            this->splitContainerH->Panel2->Controls->Add(this->richTextBoxReport);
            // 
            // splitContainerV
            // 
            this->splitContainerV->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->splitContainerV, L"splitContainerV");
            this->splitContainerV->Name = L"splitContainerV";
            // 
            // splitContainerV.Panel1
            // 
            this->splitContainerV->Panel1->Controls->Add(this->tableLayoutPanelTree);
            // 
            // splitContainerV.Panel2
            // 
            this->splitContainerV->Panel2->Controls->Add(this->tabControlMain);
            // 
            // tableLayoutPanelTree
            // 
            resources->ApplyResources(this->tableLayoutPanelTree, L"tableLayoutPanelTree");
            this->tableLayoutPanelTree->Controls->Add(this->treeViewMain, 0, 1);
            this->tableLayoutPanelTree->Controls->Add(this->toolStripTree, 0, 0);
            this->tableLayoutPanelTree->Name = L"tableLayoutPanelTree";
            // 
            // treeViewMain
            // 
            this->treeViewMain->BackColor = System::Drawing::Color::White;
            this->treeViewMain->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            resources->ApplyResources(this->treeViewMain, L"treeViewMain");
            this->treeViewMain->FullRowSelect = true;
            this->treeViewMain->HideSelection = false;
            this->treeViewMain->ImageList = this->imageListTree1;
            this->treeViewMain->Name = L"treeViewMain";
            this->treeViewMain->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &FormMain::treeViewMain_AfterSelect);
            // 
            // imageListTree1
            // 
            this->imageListTree1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageListTree1.ImageStream")));
            this->imageListTree1->TransparentColor = System::Drawing::Color::Transparent;
            this->imageListTree1->Images->SetKeyName(0, L"Tree_Device.ico");
            this->imageListTree1->Images->SetKeyName(1, L"Tree_EIB.ico");
            this->imageListTree1->Images->SetKeyName(2, L"Tree_Axis.ico");
            // 
            // toolStripTree
            // 
            this->toolStripTree->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->toolStripTree, L"toolStripTree");
            this->toolStripTree->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->toolStripButtonEibAdd1, 
                this->toolStripButtonEibDel1, this->toolStripSeparator1, this->toolStripButtonInit1, this->toolStripSeparator2, this->toolStripButtonRun1, 
                this->toolStripButtonStop});
            this->toolStripTree->Name = L"toolStripTree";
            // 
            // toolStripButtonEibAdd1
            // 
            this->toolStripButtonEibAdd1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            resources->ApplyResources(this->toolStripButtonEibAdd1, L"toolStripButtonEibAdd1");
            this->toolStripButtonEibAdd1->Name = L"toolStripButtonEibAdd1";
            this->toolStripButtonEibAdd1->Click += gcnew System::EventHandler(this, &FormMain::toolStripButtonEibAdd_Click);
            // 
            // toolStripButtonEibDel1
            // 
            this->toolStripButtonEibDel1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            resources->ApplyResources(this->toolStripButtonEibDel1, L"toolStripButtonEibDel1");
            this->toolStripButtonEibDel1->Name = L"toolStripButtonEibDel1";
            this->toolStripButtonEibDel1->Click += gcnew System::EventHandler(this, &FormMain::toolStripButtonEibDel_Click);
            // 
            // toolStripSeparator1
            // 
            this->toolStripSeparator1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            resources->ApplyResources(this->toolStripSeparator1, L"toolStripSeparator1");
            // 
            // toolStripButtonInit1
            // 
            this->toolStripButtonInit1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            resources->ApplyResources(this->toolStripButtonInit1, L"toolStripButtonInit1");
            this->toolStripButtonInit1->Name = L"toolStripButtonInit1";
            this->toolStripButtonInit1->Click += gcnew System::EventHandler(this, &FormMain::toolStripButtonInit_Click);
            // 
            // toolStripSeparator2
            // 
            this->toolStripSeparator2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
            this->toolStripSeparator2->Name = L"toolStripSeparator2";
            resources->ApplyResources(this->toolStripSeparator2, L"toolStripSeparator2");
            // 
            // toolStripButtonRun1
            // 
            this->toolStripButtonRun1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            resources->ApplyResources(this->toolStripButtonRun1, L"toolStripButtonRun1");
            this->toolStripButtonRun1->Name = L"toolStripButtonRun1";
            this->toolStripButtonRun1->Click += gcnew System::EventHandler(this, &FormMain::toolStripButtonRun_Click);
            // 
            // toolStripButtonStop
            // 
            this->toolStripButtonStop->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            resources->ApplyResources(this->toolStripButtonStop, L"toolStripButtonStop");
            this->toolStripButtonStop->Name = L"toolStripButtonStop";
            this->toolStripButtonStop->Click += gcnew System::EventHandler(this, &FormMain::toolStripButtonStop_Click);
            // 
            // tabControlMain
            // 
            this->tabControlMain->Controls->Add(this->tabPageNetwork);
            this->tabControlMain->Controls->Add(this->tabPageAxis);
            this->tabControlMain->Controls->Add(this->tabPageDataPacket);
            this->tabControlMain->Controls->Add(this->tabPageTrigger);
            this->tabControlMain->Controls->Add(this->tabPageOperationMode);
            this->tabControlMain->Controls->Add(this->tabPageCrankshaftConfig);
            this->tabControlMain->Controls->Add(this->tabPageSummary);
            this->tabControlMain->Controls->Add(this->tabPageDetail);
            this->tabControlMain->Controls->Add(this->tabPageCrankshaftDisplay);
            this->tabControlMain->Controls->Add(this->tabPageOptions);
            resources->ApplyResources(this->tabControlMain, L"tabControlMain");
            this->tabControlMain->Name = L"tabControlMain";
            this->tabControlMain->SelectedIndex = 0;
            this->tabControlMain->Selected += gcnew System::Windows::Forms::TabControlEventHandler(this, &FormMain::tabControlMain_Selected);
            this->tabControlMain->DrawItem += gcnew System::Windows::Forms::DrawItemEventHandler(this, &FormMain::tabControlMain_DrawItem);
            // 
            // tabPageNetwork
            // 
            this->tabPageNetwork->BackColor = System::Drawing::Color::White;
            this->tabPageNetwork->Controls->Add(this->tableLayoutPanelNetwork);
            resources->ApplyResources(this->tabPageNetwork, L"tabPageNetwork");
            this->tabPageNetwork->Name = L"tabPageNetwork";
            this->tabPageNetwork->Tag = L"EIB";
            this->tabPageNetwork->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelNetwork
            // 
            resources->ApplyResources(this->tableLayoutPanelNetwork, L"tableLayoutPanelNetwork");
            this->tableLayoutPanelNetwork->Controls->Add(this->tableLayoutPanelNetworkRead, 2, 1);
            this->tableLayoutPanelNetwork->Controls->Add(this->propertyGridNetwork, 0, 1);
            this->tableLayoutPanelNetwork->Name = L"tableLayoutPanelNetwork";
            // 
            // tableLayoutPanelNetworkRead
            // 
            resources->ApplyResources(this->tableLayoutPanelNetworkRead, L"tableLayoutPanelNetworkRead");
            this->tableLayoutPanelNetworkRead->Controls->Add(this->buttonNetworkRead, 0, 0);
            this->tableLayoutPanelNetworkRead->Controls->Add(this->buttonNetworkWrite, 0, 2);
            this->tableLayoutPanelNetworkRead->Controls->Add(this->buttonEibReset, 0, 4);
            this->tableLayoutPanelNetworkRead->Controls->Add(this->buttonEibFirmware, 0, 6);
            this->tableLayoutPanelNetworkRead->Name = L"tableLayoutPanelNetworkRead";
            // 
            // buttonNetworkRead
            // 
            this->buttonNetworkRead->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->buttonNetworkRead, L"buttonNetworkRead");
            this->buttonNetworkRead->Name = L"buttonNetworkRead";
            this->buttonNetworkRead->UseVisualStyleBackColor = false;
            this->buttonNetworkRead->Click += gcnew System::EventHandler(this, &FormMain::buttonNetworkRead_Click);
            // 
            // buttonNetworkWrite
            // 
            this->buttonNetworkWrite->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->buttonNetworkWrite, L"buttonNetworkWrite");
            this->buttonNetworkWrite->Name = L"buttonNetworkWrite";
            this->buttonNetworkWrite->UseVisualStyleBackColor = false;
            this->buttonNetworkWrite->Click += gcnew System::EventHandler(this, &FormMain::buttonNetworkWrite_Click);
            // 
            // buttonEibReset
            // 
            this->buttonEibReset->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->buttonEibReset, L"buttonEibReset");
            this->buttonEibReset->Name = L"buttonEibReset";
            this->buttonEibReset->UseVisualStyleBackColor = false;
            this->buttonEibReset->Click += gcnew System::EventHandler(this, &FormMain::buttonEibReset_Click);
            // 
            // buttonEibFirmware
            // 
            this->buttonEibFirmware->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->buttonEibFirmware, L"buttonEibFirmware");
            this->buttonEibFirmware->Name = L"buttonEibFirmware";
            this->buttonEibFirmware->UseVisualStyleBackColor = false;
            this->buttonEibFirmware->Click += gcnew System::EventHandler(this, &FormMain::buttonEibFirmware_Click);
            // 
            // propertyGridNetwork
            // 
            this->propertyGridNetwork->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->propertyGridNetwork, L"propertyGridNetwork");
            this->propertyGridNetwork->HelpBackColor = System::Drawing::Color::White;
            this->propertyGridNetwork->LineColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->propertyGridNetwork->Name = L"propertyGridNetwork";
            this->propertyGridNetwork->PropertySort = System::Windows::Forms::PropertySort::Categorized;
            this->propertyGridNetwork->SelectedObject = this;
            this->propertyGridNetwork->ToolbarVisible = false;
            this->propertyGridNetwork->ViewBackColor = System::Drawing::Color::White;
            this->propertyGridNetwork->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &FormMain::propertyGridNetwork_PropertyValueChanged);
            // 
            // tabPageAxis
            // 
            this->tabPageAxis->BackColor = System::Drawing::Color::White;
            this->tabPageAxis->Controls->Add(this->tableLayoutPanelAxis);
            resources->ApplyResources(this->tabPageAxis, L"tabPageAxis");
            this->tabPageAxis->Name = L"tabPageAxis";
            this->tabPageAxis->Tag = L"Axis";
            this->tabPageAxis->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelAxis
            // 
            resources->ApplyResources(this->tableLayoutPanelAxis, L"tableLayoutPanelAxis");
            this->tableLayoutPanelAxis->Controls->Add(this->propertyGridAxis, 0, 1);
            this->tableLayoutPanelAxis->Name = L"tableLayoutPanelAxis";
            // 
            // propertyGridAxis
            // 
            this->propertyGridAxis->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->propertyGridAxis, L"propertyGridAxis");
            this->propertyGridAxis->HelpBackColor = System::Drawing::Color::White;
            this->propertyGridAxis->LineColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->propertyGridAxis->Name = L"propertyGridAxis";
            this->propertyGridAxis->PropertySort = System::Windows::Forms::PropertySort::Categorized;
            this->propertyGridAxis->SelectedObject = this->treeViewMain;
            this->propertyGridAxis->ToolbarVisible = false;
            this->propertyGridAxis->ViewBackColor = System::Drawing::Color::White;
            this->propertyGridAxis->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &FormMain::propertyGridAxis_PropertyValueChanged);
            // 
            // tabPageDataPacket
            // 
            this->tabPageDataPacket->BackColor = System::Drawing::Color::White;
            this->tabPageDataPacket->Controls->Add(this->tableLayoutPanelDataPacket);
            resources->ApplyResources(this->tabPageDataPacket, L"tabPageDataPacket");
            this->tabPageDataPacket->Name = L"tabPageDataPacket";
            this->tabPageDataPacket->Tag = L"EIB";
            this->tabPageDataPacket->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelDataPacket
            // 
            resources->ApplyResources(this->tableLayoutPanelDataPacket, L"tableLayoutPanelDataPacket");
            this->tableLayoutPanelDataPacket->Controls->Add(this->dataGridViewDataPacket, 0, 1);
            this->tableLayoutPanelDataPacket->Name = L"tableLayoutPanelDataPacket";
            // 
            // dataGridViewDataPacket
            // 
            this->dataGridViewDataPacket->AllowUserToDeleteRows = false;
            this->dataGridViewDataPacket->AllowUserToResizeRows = false;
            this->dataGridViewDataPacket->BackgroundColor = System::Drawing::Color::White;
            this->dataGridViewDataPacket->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewDataPacket->ColumnHeadersVisible = false;
            this->dataGridViewDataPacket->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Header, 
                this->DataPacketColumnAxis1});
            dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle3->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle3->Font = (gcnew System::Drawing::Font(L"Arial", 11));
            dataGridViewCellStyle3->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle3->SelectionBackColor = System::Drawing::Color::White;
            dataGridViewCellStyle3->SelectionForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle3->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridViewDataPacket->DefaultCellStyle = dataGridViewCellStyle3;
            resources->ApplyResources(this->dataGridViewDataPacket, L"dataGridViewDataPacket");
            this->dataGridViewDataPacket->GridColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->dataGridViewDataPacket->MultiSelect = false;
            this->dataGridViewDataPacket->Name = L"dataGridViewDataPacket";
            this->dataGridViewDataPacket->ReadOnly = true;
            this->dataGridViewDataPacket->RowHeadersVisible = false;
            this->dataGridViewDataPacket->RowTemplate->DefaultCellStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->dataGridViewDataPacket->RowTemplate->ReadOnly = true;
            this->dataGridViewDataPacket->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
            // 
            // Header
            // 
            dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle1->SelectionBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::ControlText;
            this->Header->DefaultCellStyle = dataGridViewCellStyle1;
            resources->ApplyResources(this->Header, L"Header");
            this->Header->Name = L"Header";
            this->Header->ReadOnly = true;
            this->Header->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // DataPacketColumnAxis1
            // 
            dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->DataPacketColumnAxis1->DefaultCellStyle = dataGridViewCellStyle2;
            resources->ApplyResources(this->DataPacketColumnAxis1, L"DataPacketColumnAxis1");
            this->DataPacketColumnAxis1->Name = L"DataPacketColumnAxis1";
            this->DataPacketColumnAxis1->ReadOnly = true;
            this->DataPacketColumnAxis1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // tabPageTrigger
            // 
            this->tabPageTrigger->BackColor = System::Drawing::Color::White;
            this->tabPageTrigger->Controls->Add(this->tableLayoutPanelTrigger);
            resources->ApplyResources(this->tabPageTrigger, L"tabPageTrigger");
            this->tabPageTrigger->Name = L"tabPageTrigger";
            this->tabPageTrigger->Tag = L"EIB";
            this->tabPageTrigger->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelTrigger
            // 
            resources->ApplyResources(this->tableLayoutPanelTrigger, L"tableLayoutPanelTrigger");
            this->tableLayoutPanelTrigger->Controls->Add(this->propertyGridTrigger, 0, 1);
            this->tableLayoutPanelTrigger->Name = L"tableLayoutPanelTrigger";
            // 
            // propertyGridTrigger
            // 
            this->propertyGridTrigger->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->propertyGridTrigger, L"propertyGridTrigger");
            this->propertyGridTrigger->HelpBackColor = System::Drawing::Color::White;
            this->propertyGridTrigger->LineColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->propertyGridTrigger->Name = L"propertyGridTrigger";
            this->propertyGridTrigger->PropertySort = System::Windows::Forms::PropertySort::Categorized;
            this->propertyGridTrigger->SelectedObject = this->treeViewMain;
            this->propertyGridTrigger->ToolbarVisible = false;
            this->propertyGridTrigger->ViewBackColor = System::Drawing::Color::White;
            this->propertyGridTrigger->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &FormMain::propertyGridTrigger_PropertyValueChanged);
            // 
            // tabPageOperationMode
            // 
            this->tabPageOperationMode->BackColor = System::Drawing::Color::White;
            this->tabPageOperationMode->Controls->Add(this->tableLayoutPanelOpMode);
            resources->ApplyResources(this->tabPageOperationMode, L"tabPageOperationMode");
            this->tabPageOperationMode->Name = L"tabPageOperationMode";
            this->tabPageOperationMode->Tag = L"EIB";
            this->tabPageOperationMode->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelOpMode
            // 
            resources->ApplyResources(this->tableLayoutPanelOpMode, L"tableLayoutPanelOpMode");
            this->tableLayoutPanelOpMode->Controls->Add(this->propertyGridOpMode, 0, 1);
            this->tableLayoutPanelOpMode->Name = L"tableLayoutPanelOpMode";
            // 
            // propertyGridOpMode
            // 
            this->propertyGridOpMode->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->propertyGridOpMode, L"propertyGridOpMode");
            this->propertyGridOpMode->HelpBackColor = System::Drawing::Color::White;
            this->propertyGridOpMode->LineColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->propertyGridOpMode->Name = L"propertyGridOpMode";
            this->propertyGridOpMode->PropertySort = System::Windows::Forms::PropertySort::Categorized;
            this->propertyGridOpMode->SelectedObject = this->treeViewMain;
            this->propertyGridOpMode->ToolbarVisible = false;
            this->propertyGridOpMode->ViewBackColor = System::Drawing::Color::White;
            this->propertyGridOpMode->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &FormMain::propertyGridOpMode_PropertyValueChanged);
            // 
            // tabPageCrankshaftConfig
            // 
            this->tabPageCrankshaftConfig->BackColor = System::Drawing::Color::White;
            this->tabPageCrankshaftConfig->Controls->Add(this->tableLayoutPanelCrankshaftConfig);
            resources->ApplyResources(this->tabPageCrankshaftConfig, L"tabPageCrankshaftConfig");
            this->tabPageCrankshaftConfig->Name = L"tabPageCrankshaftConfig";
            this->tabPageCrankshaftConfig->Tag = L"EIB";
            this->tabPageCrankshaftConfig->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelCrankshaftConfig
            // 
            resources->ApplyResources(this->tableLayoutPanelCrankshaftConfig, L"tableLayoutPanelCrankshaftConfig");
            this->tableLayoutPanelCrankshaftConfig->Controls->Add(this->propertyGridCrankshaft, 0, 1);
            this->tableLayoutPanelCrankshaftConfig->Name = L"tableLayoutPanelCrankshaftConfig";
            // 
            // propertyGridCrankshaft
            // 
            this->propertyGridCrankshaft->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->propertyGridCrankshaft, L"propertyGridCrankshaft");
            this->propertyGridCrankshaft->HelpBackColor = System::Drawing::Color::White;
            this->propertyGridCrankshaft->LineColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->propertyGridCrankshaft->Name = L"propertyGridCrankshaft";
            this->propertyGridCrankshaft->PropertySort = System::Windows::Forms::PropertySort::Categorized;
            this->propertyGridCrankshaft->SelectedObject = this->treeViewMain;
            this->propertyGridCrankshaft->ToolbarVisible = false;
            this->propertyGridCrankshaft->ViewBackColor = System::Drawing::Color::White;
            this->propertyGridCrankshaft->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &FormMain::propertyGridCrankshaft_PropertyValueChanged);
            // 
            // tabPageSummary
            // 
            this->tabPageSummary->BackColor = System::Drawing::Color::White;
            this->tabPageSummary->Controls->Add(this->tableLayoutPanelSummary);
            resources->ApplyResources(this->tabPageSummary, L"tabPageSummary");
            this->tabPageSummary->Name = L"tabPageSummary";
            this->tabPageSummary->Tag = L"EIB";
            this->tabPageSummary->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelSummary
            // 
            resources->ApplyResources(this->tableLayoutPanelSummary, L"tableLayoutPanelSummary");
            this->tableLayoutPanelSummary->Controls->Add(this->panelRecordProgress, 1, 3);
            this->tableLayoutPanelSummary->Controls->Add(this->tableLayoutPanelSummaryPos, 1, 1);
            this->tableLayoutPanelSummary->Name = L"tableLayoutPanelSummary";
            // 
            // panelRecordProgress
            // 
            this->panelRecordProgress->Controls->Add(this->tableLayoutPanelRecordProgress);
            resources->ApplyResources(this->panelRecordProgress, L"panelRecordProgress");
            this->panelRecordProgress->Name = L"panelRecordProgress";
            // 
            // tableLayoutPanelRecordProgress
            // 
            resources->ApplyResources(this->tableLayoutPanelRecordProgress, L"tableLayoutPanelRecordProgress");
            this->tableLayoutPanelRecordProgress->Controls->Add(this->labelFillLevel, 0, 0);
            this->tableLayoutPanelRecordProgress->Controls->Add(this->progressBarFillLevel, 2, 0);
            this->tableLayoutPanelRecordProgress->Name = L"tableLayoutPanelRecordProgress";
            // 
            // labelFillLevel
            // 
            resources->ApplyResources(this->labelFillLevel, L"labelFillLevel");
            this->labelFillLevel->Name = L"labelFillLevel";
            // 
            // progressBarFillLevel
            // 
            resources->ApplyResources(this->progressBarFillLevel, L"progressBarFillLevel");
            this->progressBarFillLevel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->progressBarFillLevel->Name = L"progressBarFillLevel";
            this->progressBarFillLevel->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
            // 
            // tableLayoutPanelSummaryPos
            // 
            resources->ApplyResources(this->tableLayoutPanelSummaryPos, L"tableLayoutPanelSummaryPos");
            this->tableLayoutPanelSummaryPos->Controls->Add(this->panelDisplayAxis3, 0, 2);
            this->tableLayoutPanelSummaryPos->Controls->Add(this->panelDisplayAxis1, 0, 0);
            this->tableLayoutPanelSummaryPos->Controls->Add(this->panelDisplayAxis2, 2, 0);
            this->tableLayoutPanelSummaryPos->Controls->Add(this->panelDisplayAxis4, 2, 2);
            this->tableLayoutPanelSummaryPos->Name = L"tableLayoutPanelSummaryPos";
            // 
            // panelDisplayAxis3
            // 
            this->panelDisplayAxis3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelDisplayAxis3->Controls->Add(this->tableLayoutPanel7);
            resources->ApplyResources(this->panelDisplayAxis3, L"panelDisplayAxis3");
            this->panelDisplayAxis3->Name = L"panelDisplayAxis3";
            // 
            // tableLayoutPanel7
            // 
            resources->ApplyResources(this->tableLayoutPanel7, L"tableLayoutPanel7");
            this->tableLayoutPanel7->Controls->Add(this->textBoxPosAxis3, 1, 3);
            this->tableLayoutPanel7->Controls->Add(this->labelUnitAxis3, 3, 3);
            this->tableLayoutPanel7->Controls->Add(this->tableLayoutPanel8, 1, 5);
            this->tableLayoutPanel7->Controls->Add(this->tableLayoutPanel9, 1, 1);
            this->tableLayoutPanel7->Name = L"tableLayoutPanel7";
            // 
            // textBoxPosAxis3
            // 
            this->textBoxPosAxis3->BackColor = System::Drawing::SystemColors::Window;
            resources->ApplyResources(this->textBoxPosAxis3, L"textBoxPosAxis3");
            this->textBoxPosAxis3->Name = L"textBoxPosAxis3";
            this->textBoxPosAxis3->ReadOnly = true;
            // 
            // labelUnitAxis3
            // 
            resources->ApplyResources(this->labelUnitAxis3, L"labelUnitAxis3");
            this->labelUnitAxis3->Name = L"labelUnitAxis3";
            // 
            // tableLayoutPanel8
            // 
            resources->ApplyResources(this->tableLayoutPanel8, L"tableLayoutPanel8");
            this->tableLayoutPanel8->Controls->Add(this->buttonClearAxis3, 4, 0);
            this->tableLayoutPanel8->Controls->Add(this->buttonRefAxis3, 2, 0);
            this->tableLayoutPanel8->Controls->Add(this->labelAxis3, 0, 0);
            this->tableLayoutPanel8->Name = L"tableLayoutPanel8";
            // 
            // buttonClearAxis3
            // 
            this->buttonClearAxis3->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonClearAxis3, L"buttonClearAxis3");
            this->buttonClearAxis3->Name = L"buttonClearAxis3";
            this->buttonClearAxis3->UseVisualStyleBackColor = false;
            this->buttonClearAxis3->Click += gcnew System::EventHandler(this, &FormMain::buttonClearAxis_Click);
            // 
            // buttonRefAxis3
            // 
            this->buttonRefAxis3->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonRefAxis3, L"buttonRefAxis3");
            this->buttonRefAxis3->Name = L"buttonRefAxis3";
            this->buttonRefAxis3->UseVisualStyleBackColor = false;
            this->buttonRefAxis3->Click += gcnew System::EventHandler(this, &FormMain::buttonRefAxis_Click);
            // 
            // labelAxis3
            // 
            resources->ApplyResources(this->labelAxis3, L"labelAxis3");
            this->labelAxis3->Name = L"labelAxis3";
            // 
            // tableLayoutPanel9
            // 
            resources->ApplyResources(this->tableLayoutPanel9, L"tableLayoutPanel9");
            this->tableLayoutPanel9->Controls->Add(this->pictureBoxTriggerAxis3, 5, 0);
            this->tableLayoutPanel9->Controls->Add(this->pictureBoxStatusAxis3, 1, 0);
            this->tableLayoutPanel9->Controls->Add(this->pictureBoxRefAxis3, 3, 0);
            this->tableLayoutPanel9->Controls->Add(this->labelStatusAxis3, 1, 2);
            this->tableLayoutPanel9->Controls->Add(this->labelTriggerAxis3, 5, 2);
            this->tableLayoutPanel9->Controls->Add(this->labelRefAxis3, 3, 2);
            this->tableLayoutPanel9->Name = L"tableLayoutPanel9";
            // 
            // pictureBoxTriggerAxis3
            // 
            resources->ApplyResources(this->pictureBoxTriggerAxis3, L"pictureBoxTriggerAxis3");
            this->pictureBoxTriggerAxis3->Name = L"pictureBoxTriggerAxis3";
            this->pictureBoxTriggerAxis3->TabStop = false;
            // 
            // pictureBoxStatusAxis3
            // 
            resources->ApplyResources(this->pictureBoxStatusAxis3, L"pictureBoxStatusAxis3");
            this->pictureBoxStatusAxis3->Name = L"pictureBoxStatusAxis3";
            this->pictureBoxStatusAxis3->TabStop = false;
            // 
            // pictureBoxRefAxis3
            // 
            resources->ApplyResources(this->pictureBoxRefAxis3, L"pictureBoxRefAxis3");
            this->pictureBoxRefAxis3->Name = L"pictureBoxRefAxis3";
            this->pictureBoxRefAxis3->TabStop = false;
            // 
            // labelStatusAxis3
            // 
            resources->ApplyResources(this->labelStatusAxis3, L"labelStatusAxis3");
            this->labelStatusAxis3->Name = L"labelStatusAxis3";
            // 
            // labelTriggerAxis3
            // 
            resources->ApplyResources(this->labelTriggerAxis3, L"labelTriggerAxis3");
            this->labelTriggerAxis3->Name = L"labelTriggerAxis3";
            // 
            // labelRefAxis3
            // 
            resources->ApplyResources(this->labelRefAxis3, L"labelRefAxis3");
            this->labelRefAxis3->Name = L"labelRefAxis3";
            // 
            // panelDisplayAxis1
            // 
            this->panelDisplayAxis1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelDisplayAxis1->Controls->Add(this->tableLayoutPanel1);
            resources->ApplyResources(this->panelDisplayAxis1, L"panelDisplayAxis1");
            this->panelDisplayAxis1->Name = L"panelDisplayAxis1";
            // 
            // tableLayoutPanel1
            // 
            resources->ApplyResources(this->tableLayoutPanel1, L"tableLayoutPanel1");
            this->tableLayoutPanel1->Controls->Add(this->textBoxPosAxis1, 1, 3);
            this->tableLayoutPanel1->Controls->Add(this->labelUnitAxis1, 3, 3);
            this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel2, 1, 5);
            this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel3, 1, 1);
            this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
            // 
            // textBoxPosAxis1
            // 
            this->textBoxPosAxis1->BackColor = System::Drawing::SystemColors::Window;
            resources->ApplyResources(this->textBoxPosAxis1, L"textBoxPosAxis1");
            this->textBoxPosAxis1->Name = L"textBoxPosAxis1";
            this->textBoxPosAxis1->ReadOnly = true;
            // 
            // labelUnitAxis1
            // 
            resources->ApplyResources(this->labelUnitAxis1, L"labelUnitAxis1");
            this->labelUnitAxis1->Name = L"labelUnitAxis1";
            // 
            // tableLayoutPanel2
            // 
            resources->ApplyResources(this->tableLayoutPanel2, L"tableLayoutPanel2");
            this->tableLayoutPanel2->Controls->Add(this->buttonClearAxis1, 4, 0);
            this->tableLayoutPanel2->Controls->Add(this->buttonRefAxis1, 2, 0);
            this->tableLayoutPanel2->Controls->Add(this->labelAxis1, 0, 0);
            this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
            // 
            // buttonClearAxis1
            // 
            this->buttonClearAxis1->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonClearAxis1, L"buttonClearAxis1");
            this->buttonClearAxis1->Name = L"buttonClearAxis1";
            this->buttonClearAxis1->UseVisualStyleBackColor = false;
            this->buttonClearAxis1->Click += gcnew System::EventHandler(this, &FormMain::buttonClearAxis_Click);
            // 
            // buttonRefAxis1
            // 
            this->buttonRefAxis1->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonRefAxis1, L"buttonRefAxis1");
            this->buttonRefAxis1->Name = L"buttonRefAxis1";
            this->buttonRefAxis1->UseVisualStyleBackColor = false;
            this->buttonRefAxis1->Click += gcnew System::EventHandler(this, &FormMain::buttonRefAxis_Click);
            // 
            // labelAxis1
            // 
            resources->ApplyResources(this->labelAxis1, L"labelAxis1");
            this->labelAxis1->Name = L"labelAxis1";
            // 
            // tableLayoutPanel3
            // 
            resources->ApplyResources(this->tableLayoutPanel3, L"tableLayoutPanel3");
            this->tableLayoutPanel3->Controls->Add(this->pictureBoxTriggerAxis1, 5, 0);
            this->tableLayoutPanel3->Controls->Add(this->pictureBoxStatusAxis1, 1, 0);
            this->tableLayoutPanel3->Controls->Add(this->pictureBoxRefAxis1, 3, 0);
            this->tableLayoutPanel3->Controls->Add(this->labelStatusAxis1, 1, 2);
            this->tableLayoutPanel3->Controls->Add(this->labelTriggerAxis1, 5, 2);
            this->tableLayoutPanel3->Controls->Add(this->labelRefAxis1, 3, 2);
            this->tableLayoutPanel3->Name = L"tableLayoutPanel3";
            // 
            // pictureBoxTriggerAxis1
            // 
            resources->ApplyResources(this->pictureBoxTriggerAxis1, L"pictureBoxTriggerAxis1");
            this->pictureBoxTriggerAxis1->Name = L"pictureBoxTriggerAxis1";
            this->pictureBoxTriggerAxis1->TabStop = false;
            // 
            // pictureBoxStatusAxis1
            // 
            resources->ApplyResources(this->pictureBoxStatusAxis1, L"pictureBoxStatusAxis1");
            this->pictureBoxStatusAxis1->Name = L"pictureBoxStatusAxis1";
            this->pictureBoxStatusAxis1->TabStop = false;
            // 
            // pictureBoxRefAxis1
            // 
            resources->ApplyResources(this->pictureBoxRefAxis1, L"pictureBoxRefAxis1");
            this->pictureBoxRefAxis1->Name = L"pictureBoxRefAxis1";
            this->pictureBoxRefAxis1->TabStop = false;
            // 
            // labelStatusAxis1
            // 
            resources->ApplyResources(this->labelStatusAxis1, L"labelStatusAxis1");
            this->labelStatusAxis1->Name = L"labelStatusAxis1";
            // 
            // labelTriggerAxis1
            // 
            resources->ApplyResources(this->labelTriggerAxis1, L"labelTriggerAxis1");
            this->labelTriggerAxis1->Name = L"labelTriggerAxis1";
            // 
            // labelRefAxis1
            // 
            resources->ApplyResources(this->labelRefAxis1, L"labelRefAxis1");
            this->labelRefAxis1->Name = L"labelRefAxis1";
            // 
            // panelDisplayAxis2
            // 
            this->panelDisplayAxis2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelDisplayAxis2->Controls->Add(this->tableLayoutPanel4);
            resources->ApplyResources(this->panelDisplayAxis2, L"panelDisplayAxis2");
            this->panelDisplayAxis2->Name = L"panelDisplayAxis2";
            // 
            // tableLayoutPanel4
            // 
            resources->ApplyResources(this->tableLayoutPanel4, L"tableLayoutPanel4");
            this->tableLayoutPanel4->Controls->Add(this->textBoxPosAxis2, 1, 3);
            this->tableLayoutPanel4->Controls->Add(this->labelUnitAxis2, 3, 3);
            this->tableLayoutPanel4->Controls->Add(this->tableLayoutPanel5, 1, 5);
            this->tableLayoutPanel4->Controls->Add(this->tableLayoutPanel6, 1, 1);
            this->tableLayoutPanel4->Name = L"tableLayoutPanel4";
            // 
            // textBoxPosAxis2
            // 
            this->textBoxPosAxis2->BackColor = System::Drawing::SystemColors::Window;
            resources->ApplyResources(this->textBoxPosAxis2, L"textBoxPosAxis2");
            this->textBoxPosAxis2->Name = L"textBoxPosAxis2";
            this->textBoxPosAxis2->ReadOnly = true;
            // 
            // labelUnitAxis2
            // 
            resources->ApplyResources(this->labelUnitAxis2, L"labelUnitAxis2");
            this->labelUnitAxis2->Name = L"labelUnitAxis2";
            // 
            // tableLayoutPanel5
            // 
            resources->ApplyResources(this->tableLayoutPanel5, L"tableLayoutPanel5");
            this->tableLayoutPanel5->Controls->Add(this->buttonClearAxis2, 4, 0);
            this->tableLayoutPanel5->Controls->Add(this->buttonRefAxis2, 2, 0);
            this->tableLayoutPanel5->Controls->Add(this->labelAxis2, 0, 0);
            this->tableLayoutPanel5->Name = L"tableLayoutPanel5";
            // 
            // buttonClearAxis2
            // 
            this->buttonClearAxis2->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonClearAxis2, L"buttonClearAxis2");
            this->buttonClearAxis2->Name = L"buttonClearAxis2";
            this->buttonClearAxis2->UseVisualStyleBackColor = false;
            this->buttonClearAxis2->Click += gcnew System::EventHandler(this, &FormMain::buttonClearAxis_Click);
            // 
            // buttonRefAxis2
            // 
            this->buttonRefAxis2->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonRefAxis2, L"buttonRefAxis2");
            this->buttonRefAxis2->Name = L"buttonRefAxis2";
            this->buttonRefAxis2->UseVisualStyleBackColor = false;
            this->buttonRefAxis2->Click += gcnew System::EventHandler(this, &FormMain::buttonRefAxis_Click);
            // 
            // labelAxis2
            // 
            resources->ApplyResources(this->labelAxis2, L"labelAxis2");
            this->labelAxis2->Name = L"labelAxis2";
            // 
            // tableLayoutPanel6
            // 
            resources->ApplyResources(this->tableLayoutPanel6, L"tableLayoutPanel6");
            this->tableLayoutPanel6->Controls->Add(this->pictureBoxTriggerAxis2, 5, 0);
            this->tableLayoutPanel6->Controls->Add(this->pictureBoxStatusAxis2, 1, 0);
            this->tableLayoutPanel6->Controls->Add(this->pictureBoxRefAxis2, 3, 0);
            this->tableLayoutPanel6->Controls->Add(this->labelStatusAxis2, 1, 2);
            this->tableLayoutPanel6->Controls->Add(this->labelTriggerAxis2, 5, 2);
            this->tableLayoutPanel6->Controls->Add(this->labelRefAxis2, 3, 2);
            this->tableLayoutPanel6->Name = L"tableLayoutPanel6";
            // 
            // pictureBoxTriggerAxis2
            // 
            resources->ApplyResources(this->pictureBoxTriggerAxis2, L"pictureBoxTriggerAxis2");
            this->pictureBoxTriggerAxis2->Name = L"pictureBoxTriggerAxis2";
            this->pictureBoxTriggerAxis2->TabStop = false;
            // 
            // pictureBoxStatusAxis2
            // 
            resources->ApplyResources(this->pictureBoxStatusAxis2, L"pictureBoxStatusAxis2");
            this->pictureBoxStatusAxis2->Name = L"pictureBoxStatusAxis2";
            this->pictureBoxStatusAxis2->TabStop = false;
            // 
            // pictureBoxRefAxis2
            // 
            resources->ApplyResources(this->pictureBoxRefAxis2, L"pictureBoxRefAxis2");
            this->pictureBoxRefAxis2->Name = L"pictureBoxRefAxis2";
            this->pictureBoxRefAxis2->TabStop = false;
            // 
            // labelStatusAxis2
            // 
            resources->ApplyResources(this->labelStatusAxis2, L"labelStatusAxis2");
            this->labelStatusAxis2->Name = L"labelStatusAxis2";
            // 
            // labelTriggerAxis2
            // 
            resources->ApplyResources(this->labelTriggerAxis2, L"labelTriggerAxis2");
            this->labelTriggerAxis2->Name = L"labelTriggerAxis2";
            // 
            // labelRefAxis2
            // 
            resources->ApplyResources(this->labelRefAxis2, L"labelRefAxis2");
            this->labelRefAxis2->Name = L"labelRefAxis2";
            // 
            // panelDisplayAxis4
            // 
            this->panelDisplayAxis4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelDisplayAxis4->Controls->Add(this->tableLayoutPanel13);
            resources->ApplyResources(this->panelDisplayAxis4, L"panelDisplayAxis4");
            this->panelDisplayAxis4->Name = L"panelDisplayAxis4";
            // 
            // tableLayoutPanel13
            // 
            resources->ApplyResources(this->tableLayoutPanel13, L"tableLayoutPanel13");
            this->tableLayoutPanel13->Controls->Add(this->textBoxPosAxis4, 1, 3);
            this->tableLayoutPanel13->Controls->Add(this->labelUnitAxis4, 3, 3);
            this->tableLayoutPanel13->Controls->Add(this->tableLayoutPanel14, 1, 5);
            this->tableLayoutPanel13->Controls->Add(this->tableLayoutPanel15, 1, 1);
            this->tableLayoutPanel13->Name = L"tableLayoutPanel13";
            // 
            // textBoxPosAxis4
            // 
            this->textBoxPosAxis4->BackColor = System::Drawing::SystemColors::Window;
            resources->ApplyResources(this->textBoxPosAxis4, L"textBoxPosAxis4");
            this->textBoxPosAxis4->Name = L"textBoxPosAxis4";
            this->textBoxPosAxis4->ReadOnly = true;
            // 
            // labelUnitAxis4
            // 
            resources->ApplyResources(this->labelUnitAxis4, L"labelUnitAxis4");
            this->labelUnitAxis4->Name = L"labelUnitAxis4";
            // 
            // tableLayoutPanel14
            // 
            resources->ApplyResources(this->tableLayoutPanel14, L"tableLayoutPanel14");
            this->tableLayoutPanel14->Controls->Add(this->buttonClearAxis4, 4, 0);
            this->tableLayoutPanel14->Controls->Add(this->buttonRefAxis4, 2, 0);
            this->tableLayoutPanel14->Controls->Add(this->labelAxis4, 0, 0);
            this->tableLayoutPanel14->Name = L"tableLayoutPanel14";
            // 
            // buttonClearAxis4
            // 
            this->buttonClearAxis4->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonClearAxis4, L"buttonClearAxis4");
            this->buttonClearAxis4->Name = L"buttonClearAxis4";
            this->buttonClearAxis4->UseVisualStyleBackColor = false;
            this->buttonClearAxis4->Click += gcnew System::EventHandler(this, &FormMain::buttonClearAxis_Click);
            // 
            // buttonRefAxis4
            // 
            this->buttonRefAxis4->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonRefAxis4, L"buttonRefAxis4");
            this->buttonRefAxis4->Name = L"buttonRefAxis4";
            this->buttonRefAxis4->UseVisualStyleBackColor = false;
            this->buttonRefAxis4->Click += gcnew System::EventHandler(this, &FormMain::buttonRefAxis_Click);
            // 
            // labelAxis4
            // 
            resources->ApplyResources(this->labelAxis4, L"labelAxis4");
            this->labelAxis4->Name = L"labelAxis4";
            // 
            // tableLayoutPanel15
            // 
            resources->ApplyResources(this->tableLayoutPanel15, L"tableLayoutPanel15");
            this->tableLayoutPanel15->Controls->Add(this->pictureBoxTriggerAxis4, 5, 0);
            this->tableLayoutPanel15->Controls->Add(this->pictureBoxStatusAxis4, 1, 0);
            this->tableLayoutPanel15->Controls->Add(this->pictureBoxRefAxis4, 3, 0);
            this->tableLayoutPanel15->Controls->Add(this->labelStatusAxis4, 1, 2);
            this->tableLayoutPanel15->Controls->Add(this->labelTriggerAxis4, 5, 2);
            this->tableLayoutPanel15->Controls->Add(this->labelRefAxis4, 3, 2);
            this->tableLayoutPanel15->Name = L"tableLayoutPanel15";
            // 
            // pictureBoxTriggerAxis4
            // 
            resources->ApplyResources(this->pictureBoxTriggerAxis4, L"pictureBoxTriggerAxis4");
            this->pictureBoxTriggerAxis4->Name = L"pictureBoxTriggerAxis4";
            this->pictureBoxTriggerAxis4->TabStop = false;
            // 
            // pictureBoxStatusAxis4
            // 
            resources->ApplyResources(this->pictureBoxStatusAxis4, L"pictureBoxStatusAxis4");
            this->pictureBoxStatusAxis4->Name = L"pictureBoxStatusAxis4";
            this->pictureBoxStatusAxis4->TabStop = false;
            // 
            // pictureBoxRefAxis4
            // 
            resources->ApplyResources(this->pictureBoxRefAxis4, L"pictureBoxRefAxis4");
            this->pictureBoxRefAxis4->Name = L"pictureBoxRefAxis4";
            this->pictureBoxRefAxis4->TabStop = false;
            // 
            // labelStatusAxis4
            // 
            resources->ApplyResources(this->labelStatusAxis4, L"labelStatusAxis4");
            this->labelStatusAxis4->Name = L"labelStatusAxis4";
            // 
            // labelTriggerAxis4
            // 
            resources->ApplyResources(this->labelTriggerAxis4, L"labelTriggerAxis4");
            this->labelTriggerAxis4->Name = L"labelTriggerAxis4";
            // 
            // labelRefAxis4
            // 
            resources->ApplyResources(this->labelRefAxis4, L"labelRefAxis4");
            this->labelRefAxis4->Name = L"labelRefAxis4";
            // 
            // tabPageDetail
            // 
            this->tabPageDetail->BackColor = System::Drawing::Color::White;
            this->tabPageDetail->Controls->Add(this->tableLayoutPanelDetail);
            resources->ApplyResources(this->tabPageDetail, L"tabPageDetail");
            this->tabPageDetail->Name = L"tabPageDetail";
            this->tabPageDetail->Tag = L"Axis";
            this->tabPageDetail->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelDetail
            // 
            resources->ApplyResources(this->tableLayoutPanelDetail, L"tableLayoutPanelDetail");
            this->tableLayoutPanelDetail->Controls->Add(this->tableLayoutPanelDetailPos, 1, 1);
            this->tableLayoutPanelDetail->Controls->Add(this->panelRecordIndex, 1, 3);
            this->tableLayoutPanelDetail->Name = L"tableLayoutPanelDetail";
            // 
            // tableLayoutPanelDetailPos
            // 
            resources->ApplyResources(this->tableLayoutPanelDetailPos, L"tableLayoutPanelDetailPos");
            this->tableLayoutPanelDetailPos->Controls->Add(this->dataGridViewDetailDataPacket, 2, 0);
            this->tableLayoutPanelDetailPos->Controls->Add(this->tableLayoutPanelDetailStatus, 0, 0);
            this->tableLayoutPanelDetailPos->Name = L"tableLayoutPanelDetailPos";
            // 
            // dataGridViewDetailDataPacket
            // 
            this->dataGridViewDetailDataPacket->AllowUserToDeleteRows = false;
            this->dataGridViewDetailDataPacket->AllowUserToResizeRows = false;
            this->dataGridViewDetailDataPacket->BackgroundColor = System::Drawing::Color::White;
            this->dataGridViewDetailDataPacket->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewDetailDataPacket->ColumnHeadersVisible = false;
            this->dataGridViewDetailDataPacket->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->dataGridViewTextBoxColumn1, 
                this->dataGridViewTextBoxColumn2});
            dataGridViewCellStyle6->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle6->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle6->Font = (gcnew System::Drawing::Font(L"Arial", 11));
            dataGridViewCellStyle6->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle6->SelectionBackColor = System::Drawing::Color::White;
            dataGridViewCellStyle6->SelectionForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle6->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridViewDetailDataPacket->DefaultCellStyle = dataGridViewCellStyle6;
            resources->ApplyResources(this->dataGridViewDetailDataPacket, L"dataGridViewDetailDataPacket");
            this->dataGridViewDetailDataPacket->GridColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->dataGridViewDetailDataPacket->MultiSelect = false;
            this->dataGridViewDetailDataPacket->Name = L"dataGridViewDetailDataPacket";
            this->dataGridViewDetailDataPacket->ReadOnly = true;
            this->dataGridViewDetailDataPacket->RowHeadersVisible = false;
            this->dataGridViewDetailDataPacket->RowTemplate->ReadOnly = true;
            this->dataGridViewDetailDataPacket->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
            // 
            // dataGridViewTextBoxColumn1
            // 
            dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            dataGridViewCellStyle4->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle4->SelectionBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            dataGridViewCellStyle4->SelectionForeColor = System::Drawing::SystemColors::ControlText;
            this->dataGridViewTextBoxColumn1->DefaultCellStyle = dataGridViewCellStyle4;
            resources->ApplyResources(this->dataGridViewTextBoxColumn1, L"dataGridViewTextBoxColumn1");
            this->dataGridViewTextBoxColumn1->Name = L"dataGridViewTextBoxColumn1";
            this->dataGridViewTextBoxColumn1->ReadOnly = true;
            this->dataGridViewTextBoxColumn1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // dataGridViewTextBoxColumn2
            // 
            dataGridViewCellStyle5->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleRight;
            this->dataGridViewTextBoxColumn2->DefaultCellStyle = dataGridViewCellStyle5;
            resources->ApplyResources(this->dataGridViewTextBoxColumn2, L"dataGridViewTextBoxColumn2");
            this->dataGridViewTextBoxColumn2->Name = L"dataGridViewTextBoxColumn2";
            this->dataGridViewTextBoxColumn2->ReadOnly = true;
            this->dataGridViewTextBoxColumn2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // tableLayoutPanelDetailStatus
            // 
            resources->ApplyResources(this->tableLayoutPanelDetailStatus, L"tableLayoutPanelDetailStatus");
            this->tableLayoutPanelDetailStatus->Controls->Add(this->dataGridViewDetailStatus, 0, 2);
            this->tableLayoutPanelDetailStatus->Controls->Add(this->panelDisplayAxis0, 0, 0);
            this->tableLayoutPanelDetailStatus->Name = L"tableLayoutPanelDetailStatus";
            // 
            // dataGridViewDetailStatus
            // 
            this->dataGridViewDetailStatus->AllowUserToDeleteRows = false;
            this->dataGridViewDetailStatus->AllowUserToResizeRows = false;
            this->dataGridViewDetailStatus->BackgroundColor = System::Drawing::Color::White;
            this->dataGridViewDetailStatus->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewDetailStatus->ColumnHeadersVisible = false;
            this->dataGridViewDetailStatus->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->dataGridViewTextBoxColumnName, 
                this->dataGridViewTextBoxColumnValue, this->dataGridViewTextBoxColumnReset});
            dataGridViewCellStyle10->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle10->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle10->Font = (gcnew System::Drawing::Font(L"Arial", 11));
            dataGridViewCellStyle10->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle10->SelectionBackColor = System::Drawing::Color::White;
            dataGridViewCellStyle10->SelectionForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle10->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridViewDetailStatus->DefaultCellStyle = dataGridViewCellStyle10;
            resources->ApplyResources(this->dataGridViewDetailStatus, L"dataGridViewDetailStatus");
            this->dataGridViewDetailStatus->GridColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->dataGridViewDetailStatus->MultiSelect = false;
            this->dataGridViewDetailStatus->Name = L"dataGridViewDetailStatus";
            this->dataGridViewDetailStatus->ReadOnly = true;
            this->dataGridViewDetailStatus->RowHeadersVisible = false;
            this->dataGridViewDetailStatus->RowTemplate->ReadOnly = true;
            this->dataGridViewDetailStatus->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
            this->dataGridViewDetailStatus->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &FormMain::dataGridViewDetailStatus_CellContentClick);
            // 
            // dataGridViewTextBoxColumnName
            // 
            dataGridViewCellStyle7->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            dataGridViewCellStyle7->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle7->SelectionBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            dataGridViewCellStyle7->SelectionForeColor = System::Drawing::SystemColors::ControlText;
            this->dataGridViewTextBoxColumnName->DefaultCellStyle = dataGridViewCellStyle7;
            resources->ApplyResources(this->dataGridViewTextBoxColumnName, L"dataGridViewTextBoxColumnName");
            this->dataGridViewTextBoxColumnName->Name = L"dataGridViewTextBoxColumnName";
            this->dataGridViewTextBoxColumnName->ReadOnly = true;
            this->dataGridViewTextBoxColumnName->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // dataGridViewTextBoxColumnValue
            // 
            dataGridViewCellStyle8->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->dataGridViewTextBoxColumnValue->DefaultCellStyle = dataGridViewCellStyle8;
            resources->ApplyResources(this->dataGridViewTextBoxColumnValue, L"dataGridViewTextBoxColumnValue");
            this->dataGridViewTextBoxColumnValue->Name = L"dataGridViewTextBoxColumnValue";
            this->dataGridViewTextBoxColumnValue->ReadOnly = true;
            this->dataGridViewTextBoxColumnValue->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // dataGridViewTextBoxColumnReset
            // 
            dataGridViewCellStyle9->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->dataGridViewTextBoxColumnReset->DefaultCellStyle = dataGridViewCellStyle9;
            resources->ApplyResources(this->dataGridViewTextBoxColumnReset, L"dataGridViewTextBoxColumnReset");
            this->dataGridViewTextBoxColumnReset->Name = L"dataGridViewTextBoxColumnReset";
            this->dataGridViewTextBoxColumnReset->ReadOnly = true;
            this->dataGridViewTextBoxColumnReset->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridViewTextBoxColumnReset->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // panelDisplayAxis0
            // 
            this->panelDisplayAxis0->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelDisplayAxis0->Controls->Add(this->tableLayoutPanelMainAxis0);
            resources->ApplyResources(this->panelDisplayAxis0, L"panelDisplayAxis0");
            this->panelDisplayAxis0->Name = L"panelDisplayAxis0";
            // 
            // tableLayoutPanelMainAxis0
            // 
            resources->ApplyResources(this->tableLayoutPanelMainAxis0, L"tableLayoutPanelMainAxis0");
            this->tableLayoutPanelMainAxis0->Controls->Add(this->textBoxPosAxis0, 1, 3);
            this->tableLayoutPanelMainAxis0->Controls->Add(this->labelUnitAxis0, 3, 3);
            this->tableLayoutPanelMainAxis0->Controls->Add(this->tableLayoutPanelButtonAxis0, 1, 5);
            this->tableLayoutPanelMainAxis0->Controls->Add(this->tableLayoutPanelLedAxis0, 1, 1);
            this->tableLayoutPanelMainAxis0->Name = L"tableLayoutPanelMainAxis0";
            // 
            // textBoxPosAxis0
            // 
            this->textBoxPosAxis0->BackColor = System::Drawing::SystemColors::Window;
            resources->ApplyResources(this->textBoxPosAxis0, L"textBoxPosAxis0");
            this->textBoxPosAxis0->Name = L"textBoxPosAxis0";
            this->textBoxPosAxis0->ReadOnly = true;
            // 
            // labelUnitAxis0
            // 
            resources->ApplyResources(this->labelUnitAxis0, L"labelUnitAxis0");
            this->labelUnitAxis0->Name = L"labelUnitAxis0";
            // 
            // tableLayoutPanelButtonAxis0
            // 
            resources->ApplyResources(this->tableLayoutPanelButtonAxis0, L"tableLayoutPanelButtonAxis0");
            this->tableLayoutPanelButtonAxis0->Controls->Add(this->buttonClearAxis0, 4, 0);
            this->tableLayoutPanelButtonAxis0->Controls->Add(this->buttonRefAxis0, 2, 0);
            this->tableLayoutPanelButtonAxis0->Controls->Add(this->labelAxis0, 0, 0);
            this->tableLayoutPanelButtonAxis0->Name = L"tableLayoutPanelButtonAxis0";
            // 
            // buttonClearAxis0
            // 
            this->buttonClearAxis0->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonClearAxis0, L"buttonClearAxis0");
            this->buttonClearAxis0->Name = L"buttonClearAxis0";
            this->buttonClearAxis0->UseVisualStyleBackColor = false;
            this->buttonClearAxis0->Click += gcnew System::EventHandler(this, &FormMain::buttonClearAxis_Click);
            // 
            // buttonRefAxis0
            // 
            this->buttonRefAxis0->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->buttonRefAxis0, L"buttonRefAxis0");
            this->buttonRefAxis0->Name = L"buttonRefAxis0";
            this->buttonRefAxis0->UseVisualStyleBackColor = false;
            this->buttonRefAxis0->Click += gcnew System::EventHandler(this, &FormMain::buttonRefAxis_Click);
            // 
            // labelAxis0
            // 
            resources->ApplyResources(this->labelAxis0, L"labelAxis0");
            this->labelAxis0->Name = L"labelAxis0";
            // 
            // tableLayoutPanelLedAxis0
            // 
            resources->ApplyResources(this->tableLayoutPanelLedAxis0, L"tableLayoutPanelLedAxis0");
            this->tableLayoutPanelLedAxis0->Controls->Add(this->pictureBoxTriggerAxis0, 5, 0);
            this->tableLayoutPanelLedAxis0->Controls->Add(this->pictureBoxStatusAxis0, 1, 0);
            this->tableLayoutPanelLedAxis0->Controls->Add(this->pictureBoxRefAxis0, 3, 0);
            this->tableLayoutPanelLedAxis0->Controls->Add(this->labelStatusAxis0, 1, 2);
            this->tableLayoutPanelLedAxis0->Controls->Add(this->labelTriggerAxis0, 5, 2);
            this->tableLayoutPanelLedAxis0->Controls->Add(this->labelRefAxis0, 3, 2);
            this->tableLayoutPanelLedAxis0->Name = L"tableLayoutPanelLedAxis0";
            // 
            // pictureBoxTriggerAxis0
            // 
            resources->ApplyResources(this->pictureBoxTriggerAxis0, L"pictureBoxTriggerAxis0");
            this->pictureBoxTriggerAxis0->Name = L"pictureBoxTriggerAxis0";
            this->pictureBoxTriggerAxis0->TabStop = false;
            // 
            // pictureBoxStatusAxis0
            // 
            resources->ApplyResources(this->pictureBoxStatusAxis0, L"pictureBoxStatusAxis0");
            this->pictureBoxStatusAxis0->Name = L"pictureBoxStatusAxis0";
            this->pictureBoxStatusAxis0->TabStop = false;
            // 
            // pictureBoxRefAxis0
            // 
            resources->ApplyResources(this->pictureBoxRefAxis0, L"pictureBoxRefAxis0");
            this->pictureBoxRefAxis0->Name = L"pictureBoxRefAxis0";
            this->pictureBoxRefAxis0->TabStop = false;
            // 
            // labelStatusAxis0
            // 
            resources->ApplyResources(this->labelStatusAxis0, L"labelStatusAxis0");
            this->labelStatusAxis0->Name = L"labelStatusAxis0";
            // 
            // labelTriggerAxis0
            // 
            resources->ApplyResources(this->labelTriggerAxis0, L"labelTriggerAxis0");
            this->labelTriggerAxis0->Name = L"labelTriggerAxis0";
            // 
            // labelRefAxis0
            // 
            resources->ApplyResources(this->labelRefAxis0, L"labelRefAxis0");
            this->labelRefAxis0->Name = L"labelRefAxis0";
            // 
            // panelRecordIndex
            // 
            this->panelRecordIndex->Controls->Add(this->tableLayoutRecordIndex);
            resources->ApplyResources(this->panelRecordIndex, L"panelRecordIndex");
            this->panelRecordIndex->Name = L"panelRecordIndex";
            // 
            // tableLayoutRecordIndex
            // 
            resources->ApplyResources(this->tableLayoutRecordIndex, L"tableLayoutRecordIndex");
            this->tableLayoutRecordIndex->Controls->Add(this->labelIndex, 0, 0);
            this->tableLayoutRecordIndex->Controls->Add(this->labelMaxIndex, 6, 0);
            this->tableLayoutRecordIndex->Controls->Add(this->numericUpDownIndex, 2, 0);
            this->tableLayoutRecordIndex->Controls->Add(this->trackBarIndex, 4, 0);
            this->tableLayoutRecordIndex->Controls->Add(this->buttonRecordDisplayRun, 8, 0);
            this->tableLayoutRecordIndex->Controls->Add(this->buttonSaveRecordingData, 10, 0);
            this->tableLayoutRecordIndex->Name = L"tableLayoutRecordIndex";
            // 
            // labelIndex
            // 
            resources->ApplyResources(this->labelIndex, L"labelIndex");
            this->labelIndex->Name = L"labelIndex";
            this->labelIndex->Click += gcnew System::EventHandler(this, &FormMain::labelIndex_Click);
            // 
            // labelMaxIndex
            // 
            resources->ApplyResources(this->labelMaxIndex, L"labelMaxIndex");
            this->labelMaxIndex->Name = L"labelMaxIndex";
            // 
            // numericUpDownIndex
            // 
            resources->ApplyResources(this->numericUpDownIndex, L"numericUpDownIndex");
            this->numericUpDownIndex->BackColor = System::Drawing::SystemColors::Window;
            this->numericUpDownIndex->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
            this->numericUpDownIndex->Name = L"numericUpDownIndex";
            this->numericUpDownIndex->ValueChanged += gcnew System::EventHandler(this, &FormMain::numericUpDownIndex_ValueChanged);
            // 
            // trackBarIndex
            // 
            resources->ApplyResources(this->trackBarIndex, L"trackBarIndex");
            this->trackBarIndex->BackColor = System::Drawing::SystemColors::Window;
            this->trackBarIndex->Maximum = 1000;
            this->trackBarIndex->Name = L"trackBarIndex";
            this->trackBarIndex->TickFrequency = 100;
            this->trackBarIndex->ValueChanged += gcnew System::EventHandler(this, &FormMain::trackBarIndex_ValueChanged);
            // 
            // buttonRecordDisplayRun
            // 
            resources->ApplyResources(this->buttonRecordDisplayRun, L"buttonRecordDisplayRun");
            this->buttonRecordDisplayRun->FlatAppearance->BorderColor = System::Drawing::Color::White;
            this->buttonRecordDisplayRun->FlatAppearance->MouseDownBackColor = System::Drawing::Color::White;
            this->buttonRecordDisplayRun->FlatAppearance->MouseOverBackColor = System::Drawing::Color::White;
            this->buttonRecordDisplayRun->ImageList = this->imageListRunMode;
            this->buttonRecordDisplayRun->Name = L"buttonRecordDisplayRun";
            this->buttonRecordDisplayRun->UseVisualStyleBackColor = true;
            this->buttonRecordDisplayRun->Click += gcnew System::EventHandler(this, &FormMain::buttonRecordDisplayRun_Click);
            // 
            // imageListRunMode
            // 
            this->imageListRunMode->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageListRunMode.ImageStream")));
            this->imageListRunMode->TransparentColor = System::Drawing::Color::Transparent;
            this->imageListRunMode->Images->SetKeyName(0, L"EIB_Run.ico");
            this->imageListRunMode->Images->SetKeyName(1, L"EIB_Stop.ico");
            // 
            // buttonSaveRecordingData
            // 
            resources->ApplyResources(this->buttonSaveRecordingData, L"buttonSaveRecordingData");
            this->buttonSaveRecordingData->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->buttonSaveRecordingData->Name = L"buttonSaveRecordingData";
            this->buttonSaveRecordingData->UseVisualStyleBackColor = false;
            this->buttonSaveRecordingData->Click += gcnew System::EventHandler(this, &FormMain::buttonSaveRecordingData_Click);
            // 
            // tabPageCrankshaftDisplay
            // 
            this->tabPageCrankshaftDisplay->BackColor = System::Drawing::Color::White;
            this->tabPageCrankshaftDisplay->Controls->Add(this->tableLayoutPanelCrankshaftSummary);
            resources->ApplyResources(this->tabPageCrankshaftDisplay, L"tabPageCrankshaftDisplay");
            this->tabPageCrankshaftDisplay->Name = L"tabPageCrankshaftDisplay";
            this->tabPageCrankshaftDisplay->Tag = L"EIB";
            this->tabPageCrankshaftDisplay->UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelCrankshaftSummary
            // 
            resources->ApplyResources(this->tableLayoutPanelCrankshaftSummary, L"tableLayoutPanelCrankshaftSummary");
            this->tableLayoutPanelCrankshaftSummary->Controls->Add(this->tableLayoutPanelCrankshaftDisplay, 1, 1);
            this->tableLayoutPanelCrankshaftSummary->Name = L"tableLayoutPanelCrankshaftSummary";
            // 
            // tableLayoutPanelCrankshaftDisplay
            // 
            resources->ApplyResources(this->tableLayoutPanelCrankshaftDisplay, L"tableLayoutPanelCrankshaftDisplay");
            this->tableLayoutPanelCrankshaftDisplay->Controls->Add(this->tableLayoutPanelCrankshartGraph, 0, 0);
            this->tableLayoutPanelCrankshaftDisplay->Controls->Add(this->tableLayoutPanel16, 2, 0);
            this->tableLayoutPanelCrankshaftDisplay->Name = L"tableLayoutPanelCrankshaftDisplay";
            // 
            // tableLayoutPanelCrankshartGraph
            // 
            resources->ApplyResources(this->tableLayoutPanelCrankshartGraph, L"tableLayoutPanelCrankshartGraph");
            this->tableLayoutPanelCrankshartGraph->Controls->Add(this->panelMeasurePoint1, 0, 0);
            this->tableLayoutPanelCrankshartGraph->Controls->Add(this->panelMeasurePoint2, 2, 0);
            this->tableLayoutPanelCrankshartGraph->Controls->Add(this->panelMeasurePoint3, 0, 2);
            this->tableLayoutPanelCrankshartGraph->Controls->Add(this->panelMeasurePoint4, 2, 2);
            this->tableLayoutPanelCrankshartGraph->Name = L"tableLayoutPanelCrankshartGraph";
            // 
            // panelMeasurePoint1
            // 
            this->panelMeasurePoint1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelMeasurePoint1->Controls->Add(this->tableLayoutPanelGD1);
            resources->ApplyResources(this->panelMeasurePoint1, L"panelMeasurePoint1");
            this->panelMeasurePoint1->Name = L"panelMeasurePoint1";
            // 
            // tableLayoutPanelGD1
            // 
            resources->ApplyResources(this->tableLayoutPanelGD1, L"tableLayoutPanelGD1");
            this->tableLayoutPanelGD1->Controls->Add(this->toolStripCdMp1, 0, 1);
            this->tableLayoutPanelGD1->Controls->Add(this->tableLayoutPanelGD2, 1, 1);
            this->tableLayoutPanelGD1->Controls->Add(this->tableLayoutPanelGD3, 1, 0);
            this->tableLayoutPanelGD1->Controls->Add(this->tableLayoutPanelGD4, 1, 2);
            this->tableLayoutPanelGD1->Name = L"tableLayoutPanelGD1";
            // 
            // toolStripCdMp1
            // 
            resources->ApplyResources(this->toolStripCdMp1, L"toolStripCdMp1");
            this->toolStripCdMp1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripCdLabelMp1});
            this->toolStripCdMp1->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Table;
            this->toolStripCdMp1->Name = L"toolStripCdMp1";
            this->toolStripCdMp1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
            this->toolStripCdMp1->TextDirection = System::Windows::Forms::ToolStripTextDirection::Vertical270;
            // 
            // toolStripCdLabelMp1
            // 
            this->toolStripCdLabelMp1->Margin = System::Windows::Forms::Padding(0);
            this->toolStripCdLabelMp1->Name = L"toolStripCdLabelMp1";
            resources->ApplyResources(this->toolStripCdLabelMp1, L"toolStripCdLabelMp1");
            // 
            // tableLayoutPanelGD2
            // 
            resources->ApplyResources(this->tableLayoutPanelGD2, L"tableLayoutPanelGD2");
            this->tableLayoutPanelGD2->Controls->Add(this->panelGraphAreaCdMp1, 1, 0);
            this->tableLayoutPanelGD2->Controls->Add(this->labelGD0Grd, 2, 0);
            this->tableLayoutPanelGD2->Controls->Add(this->tableLayoutPaneGD5, 0, 0);
            this->tableLayoutPanelGD2->Name = L"tableLayoutPanelGD2";
            // 
            // panelGraphAreaCdMp1
            // 
            this->panelGraphAreaCdMp1->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->panelGraphAreaCdMp1, L"panelGraphAreaCdMp1");
            this->panelGraphAreaCdMp1->Name = L"panelGraphAreaCdMp1";
            this->panelGraphAreaCdMp1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormMain::panelGraphAreaCdMp1_Paint);
            // 
            // labelGD0Grd
            // 
            resources->ApplyResources(this->labelGD0Grd, L"labelGD0Grd");
            this->labelGD0Grd->Name = L"labelGD0Grd";
            // 
            // tableLayoutPaneGD5
            // 
            resources->ApplyResources(this->tableLayoutPaneGD5, L"tableLayoutPaneGD5");
            this->tableLayoutPaneGD5->Controls->Add(this->labelYmaxCdMp1, 0, 0);
            this->tableLayoutPaneGD5->Controls->Add(this->labelYminCdMp1, 0, 2);
            this->tableLayoutPaneGD5->Controls->Add(this->labelGD180Grd, 0, 1);
            this->tableLayoutPaneGD5->Name = L"tableLayoutPaneGD5";
            // 
            // labelYmaxCdMp1
            // 
            resources->ApplyResources(this->labelYmaxCdMp1, L"labelYmaxCdMp1");
            this->labelYmaxCdMp1->Name = L"labelYmaxCdMp1";
            // 
            // labelYminCdMp1
            // 
            resources->ApplyResources(this->labelYminCdMp1, L"labelYminCdMp1");
            this->labelYminCdMp1->Name = L"labelYminCdMp1";
            // 
            // labelGD180Grd
            // 
            resources->ApplyResources(this->labelGD180Grd, L"labelGD180Grd");
            this->labelGD180Grd->Name = L"labelGD180Grd";
            // 
            // tableLayoutPanelGD3
            // 
            resources->ApplyResources(this->tableLayoutPanelGD3, L"tableLayoutPanelGD3");
            this->tableLayoutPanelGD3->Controls->Add(this->labelGD90Grd, 1, 0);
            this->tableLayoutPanelGD3->Name = L"tableLayoutPanelGD3";
            // 
            // labelGD90Grd
            // 
            resources->ApplyResources(this->labelGD90Grd, L"labelGD90Grd");
            this->labelGD90Grd->Name = L"labelGD90Grd";
            // 
            // tableLayoutPanelGD4
            // 
            resources->ApplyResources(this->tableLayoutPanelGD4, L"tableLayoutPanelGD4");
            this->tableLayoutPanelGD4->Controls->Add(this->labelXminCdMp1, 0, 0);
            this->tableLayoutPanelGD4->Controls->Add(this->labelXmaxCdMp1, 3, 0);
            this->tableLayoutPanelGD4->Controls->Add(this->labelGD270Grd, 1, 0);
            this->tableLayoutPanelGD4->Name = L"tableLayoutPanelGD4";
            // 
            // labelXminCdMp1
            // 
            resources->ApplyResources(this->labelXminCdMp1, L"labelXminCdMp1");
            this->labelXminCdMp1->Name = L"labelXminCdMp1";
            // 
            // labelXmaxCdMp1
            // 
            resources->ApplyResources(this->labelXmaxCdMp1, L"labelXmaxCdMp1");
            this->labelXmaxCdMp1->Name = L"labelXmaxCdMp1";
            // 
            // labelGD270Grd
            // 
            resources->ApplyResources(this->labelGD270Grd, L"labelGD270Grd");
            this->labelGD270Grd->Name = L"labelGD270Grd";
            // 
            // panelMeasurePoint2
            // 
            this->panelMeasurePoint2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelMeasurePoint2->Controls->Add(this->tableLayoutPanel17);
            resources->ApplyResources(this->panelMeasurePoint2, L"panelMeasurePoint2");
            this->panelMeasurePoint2->Name = L"panelMeasurePoint2";
            // 
            // tableLayoutPanel17
            // 
            resources->ApplyResources(this->tableLayoutPanel17, L"tableLayoutPanel17");
            this->tableLayoutPanel17->Controls->Add(this->toolStripCdMp2, 0, 1);
            this->tableLayoutPanel17->Controls->Add(this->tableLayoutPanel18, 1, 1);
            this->tableLayoutPanel17->Controls->Add(this->tableLayoutPanel20, 1, 0);
            this->tableLayoutPanel17->Controls->Add(this->tableLayoutPanel21, 1, 2);
            this->tableLayoutPanel17->Name = L"tableLayoutPanel17";
            // 
            // toolStripCdMp2
            // 
            resources->ApplyResources(this->toolStripCdMp2, L"toolStripCdMp2");
            this->toolStripCdMp2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripCdLabelMp2});
            this->toolStripCdMp2->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Table;
            this->toolStripCdMp2->Name = L"toolStripCdMp2";
            this->toolStripCdMp2->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
            this->toolStripCdMp2->TextDirection = System::Windows::Forms::ToolStripTextDirection::Vertical270;
            // 
            // toolStripCdLabelMp2
            // 
            this->toolStripCdLabelMp2->Margin = System::Windows::Forms::Padding(0);
            this->toolStripCdLabelMp2->Name = L"toolStripCdLabelMp2";
            resources->ApplyResources(this->toolStripCdLabelMp2, L"toolStripCdLabelMp2");
            // 
            // tableLayoutPanel18
            // 
            resources->ApplyResources(this->tableLayoutPanel18, L"tableLayoutPanel18");
            this->tableLayoutPanel18->Controls->Add(this->panelGraphAreaCdMp2, 1, 0);
            this->tableLayoutPanel18->Controls->Add(this->label1, 2, 0);
            this->tableLayoutPanel18->Controls->Add(this->tableLayoutPanel19, 0, 0);
            this->tableLayoutPanel18->Name = L"tableLayoutPanel18";
            // 
            // panelGraphAreaCdMp2
            // 
            this->panelGraphAreaCdMp2->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->panelGraphAreaCdMp2, L"panelGraphAreaCdMp2");
            this->panelGraphAreaCdMp2->Name = L"panelGraphAreaCdMp2";
            this->panelGraphAreaCdMp2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormMain::panelGraphAreaCdMp2_Paint);
            // 
            // label1
            // 
            resources->ApplyResources(this->label1, L"label1");
            this->label1->Name = L"label1";
            // 
            // tableLayoutPanel19
            // 
            resources->ApplyResources(this->tableLayoutPanel19, L"tableLayoutPanel19");
            this->tableLayoutPanel19->Controls->Add(this->labelYmaxCdMp2, 0, 0);
            this->tableLayoutPanel19->Controls->Add(this->labelYminCdMp2, 0, 2);
            this->tableLayoutPanel19->Controls->Add(this->label4, 0, 1);
            this->tableLayoutPanel19->Name = L"tableLayoutPanel19";
            // 
            // labelYmaxCdMp2
            // 
            resources->ApplyResources(this->labelYmaxCdMp2, L"labelYmaxCdMp2");
            this->labelYmaxCdMp2->Name = L"labelYmaxCdMp2";
            // 
            // labelYminCdMp2
            // 
            resources->ApplyResources(this->labelYminCdMp2, L"labelYminCdMp2");
            this->labelYminCdMp2->Name = L"labelYminCdMp2";
            // 
            // label4
            // 
            resources->ApplyResources(this->label4, L"label4");
            this->label4->Name = L"label4";
            // 
            // tableLayoutPanel20
            // 
            resources->ApplyResources(this->tableLayoutPanel20, L"tableLayoutPanel20");
            this->tableLayoutPanel20->Controls->Add(this->label5, 1, 0);
            this->tableLayoutPanel20->Name = L"tableLayoutPanel20";
            // 
            // label5
            // 
            resources->ApplyResources(this->label5, L"label5");
            this->label5->Name = L"label5";
            // 
            // tableLayoutPanel21
            // 
            resources->ApplyResources(this->tableLayoutPanel21, L"tableLayoutPanel21");
            this->tableLayoutPanel21->Controls->Add(this->labelXminCdMp2, 0, 0);
            this->tableLayoutPanel21->Controls->Add(this->labelXmaxCdMp2, 3, 0);
            this->tableLayoutPanel21->Controls->Add(this->label8, 1, 0);
            this->tableLayoutPanel21->Name = L"tableLayoutPanel21";
            // 
            // labelXminCdMp2
            // 
            resources->ApplyResources(this->labelXminCdMp2, L"labelXminCdMp2");
            this->labelXminCdMp2->Name = L"labelXminCdMp2";
            // 
            // labelXmaxCdMp2
            // 
            resources->ApplyResources(this->labelXmaxCdMp2, L"labelXmaxCdMp2");
            this->labelXmaxCdMp2->Name = L"labelXmaxCdMp2";
            // 
            // label8
            // 
            resources->ApplyResources(this->label8, L"label8");
            this->label8->Name = L"label8";
            // 
            // panelMeasurePoint3
            // 
            this->panelMeasurePoint3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelMeasurePoint3->Controls->Add(this->tableLayoutPanel22);
            resources->ApplyResources(this->panelMeasurePoint3, L"panelMeasurePoint3");
            this->panelMeasurePoint3->Name = L"panelMeasurePoint3";
            // 
            // tableLayoutPanel22
            // 
            resources->ApplyResources(this->tableLayoutPanel22, L"tableLayoutPanel22");
            this->tableLayoutPanel22->Controls->Add(this->toolStripCdMp3, 0, 1);
            this->tableLayoutPanel22->Controls->Add(this->tableLayoutPanel23, 1, 1);
            this->tableLayoutPanel22->Controls->Add(this->tableLayoutPanel25, 1, 0);
            this->tableLayoutPanel22->Controls->Add(this->tableLayoutPanel26, 1, 2);
            this->tableLayoutPanel22->Name = L"tableLayoutPanel22";
            // 
            // toolStripCdMp3
            // 
            resources->ApplyResources(this->toolStripCdMp3, L"toolStripCdMp3");
            this->toolStripCdMp3->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripCdLabelMp3});
            this->toolStripCdMp3->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Table;
            this->toolStripCdMp3->Name = L"toolStripCdMp3";
            this->toolStripCdMp3->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
            this->toolStripCdMp3->TextDirection = System::Windows::Forms::ToolStripTextDirection::Vertical270;
            // 
            // toolStripCdLabelMp3
            // 
            this->toolStripCdLabelMp3->Margin = System::Windows::Forms::Padding(0);
            this->toolStripCdLabelMp3->Name = L"toolStripCdLabelMp3";
            resources->ApplyResources(this->toolStripCdLabelMp3, L"toolStripCdLabelMp3");
            // 
            // tableLayoutPanel23
            // 
            resources->ApplyResources(this->tableLayoutPanel23, L"tableLayoutPanel23");
            this->tableLayoutPanel23->Controls->Add(this->panelGraphAreaCdMp3, 1, 0);
            this->tableLayoutPanel23->Controls->Add(this->label9, 2, 0);
            this->tableLayoutPanel23->Controls->Add(this->tableLayoutPanel24, 0, 0);
            this->tableLayoutPanel23->Name = L"tableLayoutPanel23";
            // 
            // panelGraphAreaCdMp3
            // 
            this->panelGraphAreaCdMp3->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->panelGraphAreaCdMp3, L"panelGraphAreaCdMp3");
            this->panelGraphAreaCdMp3->Name = L"panelGraphAreaCdMp3";
            this->panelGraphAreaCdMp3->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormMain::panelGraphAreaCdMp3_Paint);
            // 
            // label9
            // 
            resources->ApplyResources(this->label9, L"label9");
            this->label9->Name = L"label9";
            // 
            // tableLayoutPanel24
            // 
            resources->ApplyResources(this->tableLayoutPanel24, L"tableLayoutPanel24");
            this->tableLayoutPanel24->Controls->Add(this->labelYmaxCdMp3, 0, 0);
            this->tableLayoutPanel24->Controls->Add(this->labelYminCdMp3, 0, 2);
            this->tableLayoutPanel24->Controls->Add(this->label12, 0, 1);
            this->tableLayoutPanel24->Name = L"tableLayoutPanel24";
            // 
            // labelYmaxCdMp3
            // 
            resources->ApplyResources(this->labelYmaxCdMp3, L"labelYmaxCdMp3");
            this->labelYmaxCdMp3->Name = L"labelYmaxCdMp3";
            // 
            // labelYminCdMp3
            // 
            resources->ApplyResources(this->labelYminCdMp3, L"labelYminCdMp3");
            this->labelYminCdMp3->Name = L"labelYminCdMp3";
            // 
            // label12
            // 
            resources->ApplyResources(this->label12, L"label12");
            this->label12->Name = L"label12";
            // 
            // tableLayoutPanel25
            // 
            resources->ApplyResources(this->tableLayoutPanel25, L"tableLayoutPanel25");
            this->tableLayoutPanel25->Controls->Add(this->label13, 1, 0);
            this->tableLayoutPanel25->Name = L"tableLayoutPanel25";
            // 
            // label13
            // 
            resources->ApplyResources(this->label13, L"label13");
            this->label13->Name = L"label13";
            // 
            // tableLayoutPanel26
            // 
            resources->ApplyResources(this->tableLayoutPanel26, L"tableLayoutPanel26");
            this->tableLayoutPanel26->Controls->Add(this->labelXminCdMp3, 0, 0);
            this->tableLayoutPanel26->Controls->Add(this->labelXmaxCdMp3, 3, 0);
            this->tableLayoutPanel26->Controls->Add(this->label21, 1, 0);
            this->tableLayoutPanel26->Name = L"tableLayoutPanel26";
            // 
            // labelXminCdMp3
            // 
            resources->ApplyResources(this->labelXminCdMp3, L"labelXminCdMp3");
            this->labelXminCdMp3->Name = L"labelXminCdMp3";
            // 
            // labelXmaxCdMp3
            // 
            resources->ApplyResources(this->labelXmaxCdMp3, L"labelXmaxCdMp3");
            this->labelXmaxCdMp3->Name = L"labelXmaxCdMp3";
            // 
            // label21
            // 
            resources->ApplyResources(this->label21, L"label21");
            this->label21->Name = L"label21";
            // 
            // panelMeasurePoint4
            // 
            this->panelMeasurePoint4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->panelMeasurePoint4->Controls->Add(this->tableLayoutPanel27);
            resources->ApplyResources(this->panelMeasurePoint4, L"panelMeasurePoint4");
            this->panelMeasurePoint4->Name = L"panelMeasurePoint4";
            // 
            // tableLayoutPanel27
            // 
            resources->ApplyResources(this->tableLayoutPanel27, L"tableLayoutPanel27");
            this->tableLayoutPanel27->Controls->Add(this->toolStripCdMp4, 0, 1);
            this->tableLayoutPanel27->Controls->Add(this->tableLayoutPanel28, 1, 1);
            this->tableLayoutPanel27->Controls->Add(this->tableLayoutPanel30, 1, 0);
            this->tableLayoutPanel27->Controls->Add(this->tableLayoutPanel31, 1, 2);
            this->tableLayoutPanel27->Name = L"tableLayoutPanel27";
            // 
            // toolStripCdMp4
            // 
            resources->ApplyResources(this->toolStripCdMp4, L"toolStripCdMp4");
            this->toolStripCdMp4->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripCdLabelMp4});
            this->toolStripCdMp4->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Table;
            this->toolStripCdMp4->Name = L"toolStripCdMp4";
            this->toolStripCdMp4->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
            this->toolStripCdMp4->TextDirection = System::Windows::Forms::ToolStripTextDirection::Vertical270;
            // 
            // toolStripCdLabelMp4
            // 
            this->toolStripCdLabelMp4->Margin = System::Windows::Forms::Padding(0);
            this->toolStripCdLabelMp4->Name = L"toolStripCdLabelMp4";
            resources->ApplyResources(this->toolStripCdLabelMp4, L"toolStripCdLabelMp4");
            // 
            // tableLayoutPanel28
            // 
            resources->ApplyResources(this->tableLayoutPanel28, L"tableLayoutPanel28");
            this->tableLayoutPanel28->Controls->Add(this->panelGraphAreaCdMp4, 1, 0);
            this->tableLayoutPanel28->Controls->Add(this->label22, 2, 0);
            this->tableLayoutPanel28->Controls->Add(this->tableLayoutPanel29, 0, 0);
            this->tableLayoutPanel28->Name = L"tableLayoutPanel28";
            // 
            // panelGraphAreaCdMp4
            // 
            this->panelGraphAreaCdMp4->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->panelGraphAreaCdMp4, L"panelGraphAreaCdMp4");
            this->panelGraphAreaCdMp4->Name = L"panelGraphAreaCdMp4";
            this->panelGraphAreaCdMp4->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormMain::panelGraphAreaCdMp4_Paint);
            // 
            // label22
            // 
            resources->ApplyResources(this->label22, L"label22");
            this->label22->Name = L"label22";
            // 
            // tableLayoutPanel29
            // 
            resources->ApplyResources(this->tableLayoutPanel29, L"tableLayoutPanel29");
            this->tableLayoutPanel29->Controls->Add(this->labelYmaxCdMp4, 0, 0);
            this->tableLayoutPanel29->Controls->Add(this->labelYminCdMp4, 0, 2);
            this->tableLayoutPanel29->Controls->Add(this->label25, 0, 1);
            this->tableLayoutPanel29->Name = L"tableLayoutPanel29";
            // 
            // labelYmaxCdMp4
            // 
            resources->ApplyResources(this->labelYmaxCdMp4, L"labelYmaxCdMp4");
            this->labelYmaxCdMp4->Name = L"labelYmaxCdMp4";
            // 
            // labelYminCdMp4
            // 
            resources->ApplyResources(this->labelYminCdMp4, L"labelYminCdMp4");
            this->labelYminCdMp4->Name = L"labelYminCdMp4";
            // 
            // label25
            // 
            resources->ApplyResources(this->label25, L"label25");
            this->label25->Name = L"label25";
            // 
            // tableLayoutPanel30
            // 
            resources->ApplyResources(this->tableLayoutPanel30, L"tableLayoutPanel30");
            this->tableLayoutPanel30->Controls->Add(this->label26, 1, 0);
            this->tableLayoutPanel30->Name = L"tableLayoutPanel30";
            // 
            // label26
            // 
            resources->ApplyResources(this->label26, L"label26");
            this->label26->Name = L"label26";
            // 
            // tableLayoutPanel31
            // 
            resources->ApplyResources(this->tableLayoutPanel31, L"tableLayoutPanel31");
            this->tableLayoutPanel31->Controls->Add(this->labelXminCdMp4, 0, 0);
            this->tableLayoutPanel31->Controls->Add(this->labelXmaxCdMp4, 3, 0);
            this->tableLayoutPanel31->Controls->Add(this->label29, 1, 0);
            this->tableLayoutPanel31->Name = L"tableLayoutPanel31";
            // 
            // labelXminCdMp4
            // 
            resources->ApplyResources(this->labelXminCdMp4, L"labelXminCdMp4");
            this->labelXminCdMp4->Name = L"labelXminCdMp4";
            // 
            // labelXmaxCdMp4
            // 
            resources->ApplyResources(this->labelXmaxCdMp4, L"labelXmaxCdMp4");
            this->labelXmaxCdMp4->Name = L"labelXmaxCdMp4";
            // 
            // label29
            // 
            resources->ApplyResources(this->label29, L"label29");
            this->label29->Name = L"label29";
            // 
            // tableLayoutPanel16
            // 
            resources->ApplyResources(this->tableLayoutPanel16, L"tableLayoutPanel16");
            this->tableLayoutPanel16->Controls->Add(this->groupBoxCrankshaftRef, 0, 3);
            this->tableLayoutPanel16->Controls->Add(this->checkBoxStopAt360, 0, 1);
            this->tableLayoutPanel16->Name = L"tableLayoutPanel16";
            // 
            // groupBoxCrankshaftRef
            // 
            this->groupBoxCrankshaftRef->Controls->Add(this->tableLayoutPanelCrankshaftReference);
            resources->ApplyResources(this->groupBoxCrankshaftRef, L"groupBoxCrankshaftRef");
            this->groupBoxCrankshaftRef->Name = L"groupBoxCrankshaftRef";
            this->groupBoxCrankshaftRef->TabStop = false;
            // 
            // tableLayoutPanelCrankshaftReference
            // 
            resources->ApplyResources(this->tableLayoutPanelCrankshaftReference, L"tableLayoutPanelCrankshaftReference");
            this->tableLayoutPanelCrankshaftReference->Controls->Add(this->buttonCrankshaftRefClear, 1, 5);
            this->tableLayoutPanelCrankshaftReference->Controls->Add(this->buttonCrankshaftRefSave, 1, 1);
            this->tableLayoutPanelCrankshaftReference->Controls->Add(this->buttonCrankshaftRefLoad, 1, 3);
            this->tableLayoutPanelCrankshaftReference->Name = L"tableLayoutPanelCrankshaftReference";
            // 
            // buttonCrankshaftRefClear
            // 
            this->buttonCrankshaftRefClear->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->buttonCrankshaftRefClear, L"buttonCrankshaftRefClear");
            this->buttonCrankshaftRefClear->Name = L"buttonCrankshaftRefClear";
            this->buttonCrankshaftRefClear->UseVisualStyleBackColor = false;
            this->buttonCrankshaftRefClear->Click += gcnew System::EventHandler(this, &FormMain::buttonCrankshaftRefClear_Click);
            // 
            // buttonCrankshaftRefSave
            // 
            this->buttonCrankshaftRefSave->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->buttonCrankshaftRefSave, L"buttonCrankshaftRefSave");
            this->buttonCrankshaftRefSave->Name = L"buttonCrankshaftRefSave";
            this->buttonCrankshaftRefSave->UseVisualStyleBackColor = false;
            this->buttonCrankshaftRefSave->Click += gcnew System::EventHandler(this, &FormMain::buttonCrankshaftRefSave_Click);
            // 
            // buttonCrankshaftRefLoad
            // 
            this->buttonCrankshaftRefLoad->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->buttonCrankshaftRefLoad, L"buttonCrankshaftRefLoad");
            this->buttonCrankshaftRefLoad->Name = L"buttonCrankshaftRefLoad";
            this->buttonCrankshaftRefLoad->UseVisualStyleBackColor = false;
            this->buttonCrankshaftRefLoad->Click += gcnew System::EventHandler(this, &FormMain::buttonCrankshaftRefLoad_Click);
            // 
            // checkBoxStopAt360
            // 
            resources->ApplyResources(this->checkBoxStopAt360, L"checkBoxStopAt360");
            this->checkBoxStopAt360->Name = L"checkBoxStopAt360";
            this->checkBoxStopAt360->UseVisualStyleBackColor = true;
            this->checkBoxStopAt360->Click += gcnew System::EventHandler(this, &FormMain::checkBoxStopAt360_Click);
            // 
            // tabPageOptions
            // 
            this->tabPageOptions->BackColor = System::Drawing::Color::White;
            this->tabPageOptions->Controls->Add(this->tableLayoutDisplay);
            resources->ApplyResources(this->tabPageOptions, L"tabPageOptions");
            this->tabPageOptions->Name = L"tabPageOptions";
            this->tabPageOptions->Tag = L"EIB";
            this->tabPageOptions->UseVisualStyleBackColor = true;
            // 
            // tableLayoutDisplay
            // 
            resources->ApplyResources(this->tableLayoutDisplay, L"tableLayoutDisplay");
            this->tableLayoutDisplay->Controls->Add(this->propertyGridDisplay, 0, 1);
            this->tableLayoutDisplay->Name = L"tableLayoutDisplay";
            // 
            // propertyGridDisplay
            // 
            this->propertyGridDisplay->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->propertyGridDisplay, L"propertyGridDisplay");
            this->propertyGridDisplay->HelpBackColor = System::Drawing::Color::White;
            this->propertyGridDisplay->LineColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->propertyGridDisplay->Name = L"propertyGridDisplay";
            this->propertyGridDisplay->PropertySort = System::Windows::Forms::PropertySort::Categorized;
            this->propertyGridDisplay->SelectedObject = this->treeViewMain;
            this->propertyGridDisplay->ToolbarVisible = false;
            this->propertyGridDisplay->ViewBackColor = System::Drawing::Color::White;
            this->propertyGridDisplay->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &FormMain::propertyGridDisplay_PropertyValueChanged);
            // 
            // richTextBoxReport
            // 
            this->richTextBoxReport->BackColor = System::Drawing::Color::White;
            this->richTextBoxReport->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            resources->ApplyResources(this->richTextBoxReport, L"richTextBoxReport");
            this->richTextBoxReport->Name = L"richTextBoxReport";
            // 
            // pictureBoxJH
            // 
            resources->ApplyResources(this->pictureBoxJH, L"pictureBoxJH");
            this->pictureBoxJH->Name = L"pictureBoxJH";
            this->pictureBoxJH->TabStop = false;
            // 
            // panelGreenWhite
            // 
            this->panelGreenWhite->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->panelGreenWhite, L"panelGreenWhite");
            this->panelGreenWhite->Name = L"panelGreenWhite";
            // 
            // menuStripMain
            // 
            this->menuStripMain->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->menuStripMain, L"menuStripMain");
            this->menuStripMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->dateiToolStripMenuItem, 
                this->konfigurationToolStripMenuItem, this->anzeigeToolStripMenuItem});
            this->menuStripMain->Name = L"menuStripMain";
            // 
            // dateiToolStripMenuItem
            // 
            this->dateiToolStripMenuItem->BackColor = System::Drawing::Color::White;
            this->dateiToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->projektOeffnenToolStripMenuItem, 
                this->projektSpeichernToolStripMenuItem, this->projektSpeichernUnterToolStripMenuItem, this->toolStripSeparator5, this->clearReportListToolStripMenuItem, 
                this->toolStripSeparator4, this->infoToolStripMenuItem, this->toolStripSeparator3, this->beendenToolStripMenuItem});
            resources->ApplyResources(this->dateiToolStripMenuItem, L"dateiToolStripMenuItem");
            this->dateiToolStripMenuItem->Name = L"dateiToolStripMenuItem";
            // 
            // projektOeffnenToolStripMenuItem
            // 
            this->projektOeffnenToolStripMenuItem->Name = L"projektOeffnenToolStripMenuItem";
            resources->ApplyResources(this->projektOeffnenToolStripMenuItem, L"projektOeffnenToolStripMenuItem");
            this->projektOeffnenToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::openProjectToolStripMenuItem_Click);
            // 
            // projektSpeichernToolStripMenuItem
            // 
            this->projektSpeichernToolStripMenuItem->Name = L"projektSpeichernToolStripMenuItem";
            resources->ApplyResources(this->projektSpeichernToolStripMenuItem, L"projektSpeichernToolStripMenuItem");
            this->projektSpeichernToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::saveProjectToolStripMenuItem_Click);
            // 
            // projektSpeichernUnterToolStripMenuItem
            // 
            this->projektSpeichernUnterToolStripMenuItem->Name = L"projektSpeichernUnterToolStripMenuItem";
            resources->ApplyResources(this->projektSpeichernUnterToolStripMenuItem, L"projektSpeichernUnterToolStripMenuItem");
            this->projektSpeichernUnterToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::saveAsProjectToolStripMenuItem_Click);
            // 
            // toolStripSeparator5
            // 
            this->toolStripSeparator5->Name = L"toolStripSeparator5";
            resources->ApplyResources(this->toolStripSeparator5, L"toolStripSeparator5");
            // 
            // clearReportListToolStripMenuItem
            // 
            this->clearReportListToolStripMenuItem->Name = L"clearReportListToolStripMenuItem";
            resources->ApplyResources(this->clearReportListToolStripMenuItem, L"clearReportListToolStripMenuItem");
            this->clearReportListToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::clearReportListToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this->toolStripSeparator4->Name = L"toolStripSeparator4";
            resources->ApplyResources(this->toolStripSeparator4, L"toolStripSeparator4");
            // 
            // infoToolStripMenuItem
            // 
            this->infoToolStripMenuItem->Name = L"infoToolStripMenuItem";
            resources->ApplyResources(this->infoToolStripMenuItem, L"infoToolStripMenuItem");
            this->infoToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::infoToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this->toolStripSeparator3->Name = L"toolStripSeparator3";
            resources->ApplyResources(this->toolStripSeparator3, L"toolStripSeparator3");
            // 
            // beendenToolStripMenuItem
            // 
            this->beendenToolStripMenuItem->Name = L"beendenToolStripMenuItem";
            resources->ApplyResources(this->beendenToolStripMenuItem, L"beendenToolStripMenuItem");
            this->beendenToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::exitToolStripMenuItem_Click);
            // 
            // konfigurationToolStripMenuItem
            // 
            resources->ApplyResources(this->konfigurationToolStripMenuItem, L"konfigurationToolStripMenuItem");
            this->konfigurationToolStripMenuItem->Name = L"konfigurationToolStripMenuItem";
            this->konfigurationToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::configToolStripMenuItem_Click);
            // 
            // anzeigeToolStripMenuItem
            // 
            resources->ApplyResources(this->anzeigeToolStripMenuItem, L"anzeigeToolStripMenuItem");
            this->anzeigeToolStripMenuItem->Name = L"anzeigeToolStripMenuItem";
            this->anzeigeToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::positionToolStripMenuItem_Click);
            // 
            // pictureBoxGreenWhite
            // 
            this->pictureBoxGreenWhite->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->pictureBoxGreenWhite, L"pictureBoxGreenWhite");
            this->pictureBoxGreenWhite->Name = L"pictureBoxGreenWhite";
            this->pictureBoxGreenWhite->TabStop = false;
            // 
            // tableLayoutPanelMain
            // 
            resources->ApplyResources(this->tableLayoutPanelMain, L"tableLayoutPanelMain");
            this->tableLayoutPanelMain->Controls->Add(this->tableLayoutPanelTopV, 0, 0);
            this->tableLayoutPanelMain->Controls->Add(this->statusStripStatus, 0, 3);
            this->tableLayoutPanelMain->Controls->Add(this->splitContainerH, 0, 2);
            this->tableLayoutPanelMain->Name = L"tableLayoutPanelMain";
            // 
            // tableLayoutPanelTopV
            // 
            this->tableLayoutPanelTopV->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(176)), 
                static_cast<System::Int32>(static_cast<System::Byte>(203)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
            resources->ApplyResources(this->tableLayoutPanelTopV, L"tableLayoutPanelTopV");
            this->tableLayoutPanelTopV->Controls->Add(this->tableLayoutPanelTopL, 0, 0);
            this->tableLayoutPanelTopV->Controls->Add(this->tableLayoutPanelTopR, 1, 0);
            this->tableLayoutPanelTopV->Name = L"tableLayoutPanelTopV";
            // 
            // tableLayoutPanelTopL
            // 
            resources->ApplyResources(this->tableLayoutPanelTopL, L"tableLayoutPanelTopL");
            this->tableLayoutPanelTopL->Controls->Add(this->panelGreenWhite, 0, 3);
            this->tableLayoutPanelTopL->Controls->Add(this->tableLayoutPanelDiadur, 0, 1);
            this->tableLayoutPanelTopL->Name = L"tableLayoutPanelTopL";
            // 
            // tableLayoutPanelDiadur
            // 
            resources->ApplyResources(this->tableLayoutPanelDiadur, L"tableLayoutPanelDiadur");
            this->tableLayoutPanelDiadur->Controls->Add(this->pictureBoxDiadur1, 1, 0);
            this->tableLayoutPanelDiadur->Name = L"tableLayoutPanelDiadur";
            // 
            // pictureBoxDiadur1
            // 
            this->pictureBoxDiadur1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(176)), 
                static_cast<System::Int32>(static_cast<System::Byte>(203)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
            resources->ApplyResources(this->pictureBoxDiadur1, L"pictureBoxDiadur1");
            this->pictureBoxDiadur1->Name = L"pictureBoxDiadur1";
            this->pictureBoxDiadur1->TabStop = false;
            // 
            // tableLayoutPanelTopR
            // 
            resources->ApplyResources(this->tableLayoutPanelTopR, L"tableLayoutPanelTopR");
            this->tableLayoutPanelTopR->Controls->Add(this->tableLayoutPanelJH, 1, 1);
            this->tableLayoutPanelTopR->Controls->Add(this->tableLayoutPanelGreenWhite, 0, 3);
            this->tableLayoutPanelTopR->Controls->Add(this->menuStripMain, 1, 3);
            this->tableLayoutPanelTopR->Name = L"tableLayoutPanelTopR";
            // 
            // tableLayoutPanelJH
            // 
            resources->ApplyResources(this->tableLayoutPanelJH, L"tableLayoutPanelJH");
            this->tableLayoutPanelJH->Controls->Add(this->pictureBoxJH, 1, 0);
            this->tableLayoutPanelJH->Controls->Add(this->checkBoxDebug, 0, 0);
            this->tableLayoutPanelJH->Name = L"tableLayoutPanelJH";
            // 
            // tableLayoutPanelGreenWhite
            // 
            this->tableLayoutPanelGreenWhite->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->tableLayoutPanelGreenWhite, L"tableLayoutPanelGreenWhite");
            this->tableLayoutPanelGreenWhite->Controls->Add(this->pictureBoxGreenWhite, 0, 0);
            this->tableLayoutPanelGreenWhite->Name = L"tableLayoutPanelGreenWhite";
            // 
            // statusStripStatus
            // 
            this->statusStripStatus->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->statusStripStatus, L"statusStripStatus");
            this->statusStripStatus->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->toolStripStatusLabelText, 
                this->toolStripStatusLabelValue, this->toolStripProgressBarStatus});
            this->statusStripStatus->Name = L"statusStripStatus";
            // 
            // toolStripStatusLabelText
            // 
            this->toolStripStatusLabelText->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->toolStripStatusLabelText->Name = L"toolStripStatusLabelText";
            resources->ApplyResources(this->toolStripStatusLabelText, L"toolStripStatusLabelText");
            // 
            // toolStripStatusLabelValue
            // 
            this->toolStripStatusLabelValue->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)));
            this->toolStripStatusLabelValue->BorderSides = static_cast<System::Windows::Forms::ToolStripStatusLabelBorderSides>((((System::Windows::Forms::ToolStripStatusLabelBorderSides::Left | System::Windows::Forms::ToolStripStatusLabelBorderSides::Top) 
                | System::Windows::Forms::ToolStripStatusLabelBorderSides::Right) 
                | System::Windows::Forms::ToolStripStatusLabelBorderSides::Bottom));
            this->toolStripStatusLabelValue->BorderStyle = System::Windows::Forms::Border3DStyle::SunkenOuter;
            this->toolStripStatusLabelValue->Name = L"toolStripStatusLabelValue";
            resources->ApplyResources(this->toolStripStatusLabelValue, L"toolStripStatusLabelValue");
            this->toolStripStatusLabelValue->Spring = true;
            // 
            // toolStripProgressBarStatus
            // 
            this->toolStripProgressBarStatus->Name = L"toolStripProgressBarStatus";
            resources->ApplyResources(this->toolStripProgressBarStatus, L"toolStripProgressBarStatus");
            this->toolStripProgressBarStatus->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
            // 
            // timerDisplay
            // 
            this->timerDisplay->Interval = 250;
            this->timerDisplay->Tick += gcnew System::EventHandler(this, &FormMain::timerDisplay_Tick);
            // 
            // imageListDisplayLed
            // 
            this->imageListDisplayLed->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageListDisplayLed.ImageStream")));
            this->imageListDisplayLed->TransparentColor = System::Drawing::Color::Transparent;
            this->imageListDisplayLed->Images->SetKeyName(0, L"LED_trans_trans.ico");
            this->imageListDisplayLed->Images->SetKeyName(1, L"LED_trans_green.ico");
            this->imageListDisplayLed->Images->SetKeyName(2, L"LED_trans_red.ico");
            this->imageListDisplayLed->Images->SetKeyName(3, L"LED_trans_yellow.ico");
            // 
            // panel4
            // 
            this->panel4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(239)), 
                static_cast<System::Int32>(static_cast<System::Byte>(239)));
            resources->ApplyResources(this->panel4, L"panel4");
            this->panel4->Name = L"panel4";
            // 
            // tableLayoutPanel10
            // 
            resources->ApplyResources(this->tableLayoutPanel10, L"tableLayoutPanel10");
            this->tableLayoutPanel10->Controls->Add(this->textBox4, 1, 3);
            this->tableLayoutPanel10->Controls->Add(this->label16, 3, 3);
            this->tableLayoutPanel10->Controls->Add(this->tableLayoutPanel11, 1, 5);
            this->tableLayoutPanel10->Name = L"tableLayoutPanel10";
            // 
            // textBox4
            // 
            this->textBox4->BackColor = System::Drawing::SystemColors::Window;
            resources->ApplyResources(this->textBox4, L"textBox4");
            this->textBox4->Name = L"textBox4";
            this->textBox4->ReadOnly = true;
            // 
            // label16
            // 
            resources->ApplyResources(this->label16, L"label16");
            this->label16->Name = L"label16";
            // 
            // tableLayoutPanel11
            // 
            resources->ApplyResources(this->tableLayoutPanel11, L"tableLayoutPanel11");
            this->tableLayoutPanel11->Controls->Add(this->button7, 4, 0);
            this->tableLayoutPanel11->Controls->Add(this->button8, 2, 0);
            this->tableLayoutPanel11->Controls->Add(this->label17, 0, 0);
            this->tableLayoutPanel11->Name = L"tableLayoutPanel11";
            // 
            // button7
            // 
            this->button7->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->button7, L"button7");
            this->button7->Name = L"button7";
            this->button7->UseVisualStyleBackColor = false;
            // 
            // button8
            // 
            this->button8->BackColor = System::Drawing::Color::White;
            resources->ApplyResources(this->button8, L"button8");
            this->button8->Name = L"button8";
            this->button8->UseVisualStyleBackColor = false;
            // 
            // label17
            // 
            resources->ApplyResources(this->label17, L"label17");
            this->label17->Name = L"label17";
            // 
            // tableLayoutPanel12
            // 
            resources->ApplyResources(this->tableLayoutPanel12, L"tableLayoutPanel12");
            this->tableLayoutPanel12->Controls->Add(this->pictureBox10, 5, 0);
            this->tableLayoutPanel12->Controls->Add(this->pictureBox11, 1, 0);
            this->tableLayoutPanel12->Controls->Add(this->pictureBox12, 3, 0);
            this->tableLayoutPanel12->Controls->Add(this->label18, 1, 2);
            this->tableLayoutPanel12->Controls->Add(this->label19, 5, 2);
            this->tableLayoutPanel12->Controls->Add(this->label20, 3, 2);
            this->tableLayoutPanel12->Name = L"tableLayoutPanel12";
            // 
            // pictureBox10
            // 
            resources->ApplyResources(this->pictureBox10, L"pictureBox10");
            this->pictureBox10->Name = L"pictureBox10";
            this->pictureBox10->TabStop = false;
            // 
            // pictureBox11
            // 
            resources->ApplyResources(this->pictureBox11, L"pictureBox11");
            this->pictureBox11->Name = L"pictureBox11";
            this->pictureBox11->TabStop = false;
            // 
            // pictureBox12
            // 
            resources->ApplyResources(this->pictureBox12, L"pictureBox12");
            this->pictureBox12->Name = L"pictureBox12";
            this->pictureBox12->TabStop = false;
            // 
            // label18
            // 
            resources->ApplyResources(this->label18, L"label18");
            this->label18->Name = L"label18";
            // 
            // label19
            // 
            resources->ApplyResources(this->label19, L"label19");
            this->label19->Name = L"label19";
            // 
            // label20
            // 
            resources->ApplyResources(this->label20, L"label20");
            this->label20->Name = L"label20";
            // 
            // checkBoxDebug
            // 
            resources->ApplyResources(this->checkBoxDebug, L"checkBoxDebug");
            this->checkBoxDebug->Name = L"checkBoxDebug";
            this->checkBoxDebug->UseVisualStyleBackColor = true;
            // 
            // FormMain
            // 
            resources->ApplyResources(this, L"$this");
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::White;
            this->Controls->Add(this->tableLayoutPanelMain);
            this->MainMenuStrip = this->menuStripMain;
            this->Name = L"FormMain";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
            this->splitContainerH->Panel1->ResumeLayout(false);
            this->splitContainerH->Panel2->ResumeLayout(false);
            this->splitContainerH->ResumeLayout(false);
            this->splitContainerV->Panel1->ResumeLayout(false);
            this->splitContainerV->Panel2->ResumeLayout(false);
            this->splitContainerV->ResumeLayout(false);
            this->tableLayoutPanelTree->ResumeLayout(false);
            this->tableLayoutPanelTree->PerformLayout();
            this->toolStripTree->ResumeLayout(false);
            this->toolStripTree->PerformLayout();
            this->tabControlMain->ResumeLayout(false);
            this->tabPageNetwork->ResumeLayout(false);
            this->tableLayoutPanelNetwork->ResumeLayout(false);
            this->tableLayoutPanelNetworkRead->ResumeLayout(false);
            this->tabPageAxis->ResumeLayout(false);
            this->tableLayoutPanelAxis->ResumeLayout(false);
            this->tabPageDataPacket->ResumeLayout(false);
            this->tableLayoutPanelDataPacket->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewDataPacket))->EndInit();
            this->tabPageTrigger->ResumeLayout(false);
            this->tableLayoutPanelTrigger->ResumeLayout(false);
            this->tabPageOperationMode->ResumeLayout(false);
            this->tableLayoutPanelOpMode->ResumeLayout(false);
            this->tabPageCrankshaftConfig->ResumeLayout(false);
            this->tableLayoutPanelCrankshaftConfig->ResumeLayout(false);
            this->tabPageSummary->ResumeLayout(false);
            this->tableLayoutPanelSummary->ResumeLayout(false);
            this->panelRecordProgress->ResumeLayout(false);
            this->tableLayoutPanelRecordProgress->ResumeLayout(false);
            this->tableLayoutPanelRecordProgress->PerformLayout();
            this->tableLayoutPanelSummaryPos->ResumeLayout(false);
            this->panelDisplayAxis3->ResumeLayout(false);
            this->tableLayoutPanel7->ResumeLayout(false);
            this->tableLayoutPanel7->PerformLayout();
            this->tableLayoutPanel8->ResumeLayout(false);
            this->tableLayoutPanel8->PerformLayout();
            this->tableLayoutPanel9->ResumeLayout(false);
            this->tableLayoutPanel9->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis3))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis3))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis3))->EndInit();
            this->panelDisplayAxis1->ResumeLayout(false);
            this->tableLayoutPanel1->ResumeLayout(false);
            this->tableLayoutPanel1->PerformLayout();
            this->tableLayoutPanel2->ResumeLayout(false);
            this->tableLayoutPanel2->PerformLayout();
            this->tableLayoutPanel3->ResumeLayout(false);
            this->tableLayoutPanel3->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis1))->EndInit();
            this->panelDisplayAxis2->ResumeLayout(false);
            this->tableLayoutPanel4->ResumeLayout(false);
            this->tableLayoutPanel4->PerformLayout();
            this->tableLayoutPanel5->ResumeLayout(false);
            this->tableLayoutPanel5->PerformLayout();
            this->tableLayoutPanel6->ResumeLayout(false);
            this->tableLayoutPanel6->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis2))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis2))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis2))->EndInit();
            this->panelDisplayAxis4->ResumeLayout(false);
            this->tableLayoutPanel13->ResumeLayout(false);
            this->tableLayoutPanel13->PerformLayout();
            this->tableLayoutPanel14->ResumeLayout(false);
            this->tableLayoutPanel14->PerformLayout();
            this->tableLayoutPanel15->ResumeLayout(false);
            this->tableLayoutPanel15->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis4))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis4))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis4))->EndInit();
            this->tabPageDetail->ResumeLayout(false);
            this->tableLayoutPanelDetail->ResumeLayout(false);
            this->tableLayoutPanelDetailPos->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewDetailDataPacket))->EndInit();
            this->tableLayoutPanelDetailStatus->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewDetailStatus))->EndInit();
            this->panelDisplayAxis0->ResumeLayout(false);
            this->tableLayoutPanelMainAxis0->ResumeLayout(false);
            this->tableLayoutPanelMainAxis0->PerformLayout();
            this->tableLayoutPanelButtonAxis0->ResumeLayout(false);
            this->tableLayoutPanelButtonAxis0->PerformLayout();
            this->tableLayoutPanelLedAxis0->ResumeLayout(false);
            this->tableLayoutPanelLedAxis0->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTriggerAxis0))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxStatusAxis0))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxRefAxis0))->EndInit();
            this->panelRecordIndex->ResumeLayout(false);
            this->tableLayoutRecordIndex->ResumeLayout(false);
            this->tableLayoutRecordIndex->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownIndex))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBarIndex))->EndInit();
            this->tabPageCrankshaftDisplay->ResumeLayout(false);
            this->tableLayoutPanelCrankshaftSummary->ResumeLayout(false);
            this->tableLayoutPanelCrankshaftDisplay->ResumeLayout(false);
            this->tableLayoutPanelCrankshartGraph->ResumeLayout(false);
            this->panelMeasurePoint1->ResumeLayout(false);
            this->tableLayoutPanelGD1->ResumeLayout(false);
            this->tableLayoutPanelGD1->PerformLayout();
            this->toolStripCdMp1->ResumeLayout(false);
            this->toolStripCdMp1->PerformLayout();
            this->tableLayoutPanelGD2->ResumeLayout(false);
            this->tableLayoutPanelGD2->PerformLayout();
            this->tableLayoutPaneGD5->ResumeLayout(false);
            this->tableLayoutPaneGD5->PerformLayout();
            this->tableLayoutPanelGD3->ResumeLayout(false);
            this->tableLayoutPanelGD3->PerformLayout();
            this->tableLayoutPanelGD4->ResumeLayout(false);
            this->tableLayoutPanelGD4->PerformLayout();
            this->panelMeasurePoint2->ResumeLayout(false);
            this->tableLayoutPanel17->ResumeLayout(false);
            this->tableLayoutPanel17->PerformLayout();
            this->toolStripCdMp2->ResumeLayout(false);
            this->toolStripCdMp2->PerformLayout();
            this->tableLayoutPanel18->ResumeLayout(false);
            this->tableLayoutPanel18->PerformLayout();
            this->tableLayoutPanel19->ResumeLayout(false);
            this->tableLayoutPanel19->PerformLayout();
            this->tableLayoutPanel20->ResumeLayout(false);
            this->tableLayoutPanel20->PerformLayout();
            this->tableLayoutPanel21->ResumeLayout(false);
            this->tableLayoutPanel21->PerformLayout();
            this->panelMeasurePoint3->ResumeLayout(false);
            this->tableLayoutPanel22->ResumeLayout(false);
            this->tableLayoutPanel22->PerformLayout();
            this->toolStripCdMp3->ResumeLayout(false);
            this->toolStripCdMp3->PerformLayout();
            this->tableLayoutPanel23->ResumeLayout(false);
            this->tableLayoutPanel23->PerformLayout();
            this->tableLayoutPanel24->ResumeLayout(false);
            this->tableLayoutPanel24->PerformLayout();
            this->tableLayoutPanel25->ResumeLayout(false);
            this->tableLayoutPanel25->PerformLayout();
            this->tableLayoutPanel26->ResumeLayout(false);
            this->tableLayoutPanel26->PerformLayout();
            this->panelMeasurePoint4->ResumeLayout(false);
            this->tableLayoutPanel27->ResumeLayout(false);
            this->tableLayoutPanel27->PerformLayout();
            this->toolStripCdMp4->ResumeLayout(false);
            this->toolStripCdMp4->PerformLayout();
            this->tableLayoutPanel28->ResumeLayout(false);
            this->tableLayoutPanel28->PerformLayout();
            this->tableLayoutPanel29->ResumeLayout(false);
            this->tableLayoutPanel29->PerformLayout();
            this->tableLayoutPanel30->ResumeLayout(false);
            this->tableLayoutPanel30->PerformLayout();
            this->tableLayoutPanel31->ResumeLayout(false);
            this->tableLayoutPanel31->PerformLayout();
            this->tableLayoutPanel16->ResumeLayout(false);
            this->tableLayoutPanel16->PerformLayout();
            this->groupBoxCrankshaftRef->ResumeLayout(false);
            this->tableLayoutPanelCrankshaftReference->ResumeLayout(false);
            this->tabPageOptions->ResumeLayout(false);
            this->tableLayoutDisplay->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxJH))->EndInit();
            this->menuStripMain->ResumeLayout(false);
            this->menuStripMain->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxGreenWhite))->EndInit();
            this->tableLayoutPanelMain->ResumeLayout(false);
            this->tableLayoutPanelMain->PerformLayout();
            this->tableLayoutPanelTopV->ResumeLayout(false);
            this->tableLayoutPanelTopL->ResumeLayout(false);
            this->tableLayoutPanelDiadur->ResumeLayout(false);
            this->tableLayoutPanelDiadur->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxDiadur1))->EndInit();
            this->tableLayoutPanelTopR->ResumeLayout(false);
            this->tableLayoutPanelTopR->PerformLayout();
            this->tableLayoutPanelJH->ResumeLayout(false);
            this->tableLayoutPanelJH->PerformLayout();
            this->tableLayoutPanelGreenWhite->ResumeLayout(false);
            this->statusStripStatus->ResumeLayout(false);
            this->statusStripStatus->PerformLayout();
            this->tableLayoutPanel10->ResumeLayout(false);
            this->tableLayoutPanel10->PerformLayout();
            this->tableLayoutPanel11->ResumeLayout(false);
            this->tableLayoutPanel11->PerformLayout();
            this->tableLayoutPanel12->ResumeLayout(false);
            this->tableLayoutPanel12->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox10))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox11))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox12))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion
    private:
        bool                initReady;
        bool                isClosing;

        ResourceManager ^   resManager;

        Drawing::Font ^     configMenuFontOriginal;
        Drawing::Font ^     configMenuFontSelected;
        Drawing::Font ^     displayMenuFontOriginal;
        Drawing::Font ^     displayMenuFontSelected;
        
        GUI_Support ^       guiSupport;
        GUI_Config          guiConfig;

        EIB_TreeList ^      eibTreeList;
        EIB_InitList ^      eibInitList;
        EIB_SaveList ^      eibSaveList;

        PropertyDataPacket ^    propertyDataPacket;
        PropertyOpMode ^        propertyOpMode;
        PropertyCrankshaft ^    propertyCrankshaft;
        PropertyDisplay ^       propertyDisplay;

        DisplayPosition ^       displayPosition0;
        DisplayPosition ^       displayPosition1;
        DisplayPosition ^       displayPosition2;
        DisplayPosition ^       displayPosition3;
        DisplayPosition ^       displayPosition4;
        DisplayDataPacket ^     displayDataPacket;

        PolarGraph ^            crankshaftGraph1;
        PolarGraph ^            crankshaftGraph2;
        PolarGraph ^            crankshaftGraph3;
        PolarGraph ^            crankshaftGraph4;
        Crankshaft_Display ^    crankshaftDisplay;
        Crankshaft_Measure ^    crankshaftMeasure;

        TabPage ^           lastConfigTab;
        TabPage ^           lastPositionTab;

        GUI_State           guiState;
        TriggerList         triggerList;
        GUI_StateTrigger    stateGuard;

        RecordingState      recordingState;
        UInt32              lastRecordIndex;
        bool                recordDisplayRun;

        Int32               displayPeriod;
        String ^            dataFileName;

	public:
        bool IsClosing();

    private:
        
        System::Void PreInitializeComponent(String ^ setLanguage,
                                            String ^ setApplication,
                                            String ^ setPassword);
        System::Void PostInitializeComponent();

        System::Void InitializeGuiState();
        System::Void TriggerGuiState(GUI_StateTrigger trigger);
        System::Void TriggerGuiState(GUI_StateTrigger trigger, TabPage ^ newTabPage);
        System::Void SetGuiState(GUI_State ^ newState, TabPage ^ newTabPage);
        bool CheckGuiState(GUI_State ^ checkState);

        System::Void GuiCmdFunction(GuiCommand guiCommand, UInt32 maxTime);
        System::Void GuiCmdThread();
        GuiCommand          guiCommand;
        Exception ^         guiCmdError;
        AutoResetEvent ^    guiCmdEnd;

        System::Void RefreshParam();
        System::Void RefreshDisplay();
        System::Void RefreshDisplay(bool readData);
        System::Void RefreshDisplay(UInt32 displayMode);
        System::Void RefreshDisplay(UInt32 displayMode, bool readData);
        System::Void ClearDisplay();
        System::Void BlinkDisplay();

        System::Void ReadEIBParameters();
        bool SaveEIBParameters(bool saveAs);
        bool CheckEIBParametersChanged();
        System::Void EIBParameterToProject();
        System::Void EIBParameterChanged();

        System::Void SetApplicationTitle(ProjectState projectState);

        System::Void InitializeEIBs();
        System::Void SelectDataFile();
        System::Void StartEIBMeasurement();
        System::Void StopEIBMeasurement(bool stopOnError);
        System::Void RunEIBMeasurement();

        System::Void SetRecordingState(RecordingTrigger recordingTrigger);
        System::Void RefreshRecordingDisplay(UInt32 index);
        System::Void SetRecordingDisplayRun(bool isEnabled);

        System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
        System::Void infoToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void clearReportListToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void openProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void saveProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void saveAsProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void configToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void positionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void toolStripButtonEibAdd_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void toolStripButtonEibDel_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void toolStripButtonInit_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void toolStripButtonRun_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void toolStripButtonStop_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void timerDisplay_Tick(System::Object^  sender, System::EventArgs^  e);
        System::Void treeViewMain_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e);
        System::Void tabControlMain_Selected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e);
        System::Void propertyGridNetwork_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e);
        System::Void buttonNetworkRead_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonNetworkWrite_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonEibReset_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonEibFirmware_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void propertyGridAxis_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e);
        System::Void propertyGridTrigger_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e);
        System::Void propertyGridOpMode_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e);
        System::Void propertyGridCrankshaft_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e);
        System::Void propertyGridDisplay_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e);
        System::Void buttonRefAxis_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonClearAxis_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void dataGridViewDetailStatus_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e);
        System::Void numericUpDownIndex_ValueChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void trackBarIndex_ValueChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void labelIndex_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonRecordDisplayRun_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonSaveRecordingData_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void checkBoxStopAt360_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonCrankshaftRefSave_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonCrankshaftRefLoad_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void buttonCrankshaftRefClear_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void panelGraphAreaCdMp1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
        System::Void panelGraphAreaCdMp2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
        System::Void panelGraphAreaCdMp3_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
        System::Void panelGraphAreaCdMp4_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
        System::Void tabControlMain_DrawItem(System::Object^  sender, System::Windows::Forms::DrawItemEventArgs^  e);
};
}
