#include "GUI_schema.h"
#include <iostream>
#include<fstream>
#include <qbuttongroup.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qgridlayout.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <qcombobox.h>
#include <QWidget>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QPixmap>
#include <QVBoxLayout>

#include <qstringlist.h>
#include <pthread.h>
#include <qslider.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>

#include <semaphore.h>
#include <qlcdnumber.h>

using namespace std;

QString GeneralBackGroundColor = "lightblue";
QString bgForeGroundColor = "black";
QString bgBackGroundColor = "lightgray";

QString rbForeGroundColor = "blue";
QString rbBackGroundColor = "lightgray";

QString AAForeGroundColor = "black";
QString CCForeGroundColor = "darkgreen";
QString AABackGroundColor = "lightgray";
QString CCBackGroundColor = "lightgreen";

QSpinBox       *EventSpinBox;
QSpinBox       *LCDSpinBox, *LCDlostSpinBox;
QLCDNumber     *rbMT_3, *rbMT_4, *rbMT_5, *rbMT_6, *rbMT_7, *rbMT_8, *rbMT_9;
pthread_t       LCDt;
/*
 * Constructor
 * Creates all child widgets of the ButtonGroups window
 */
MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{

  //ButtonsGroups::ButtonsGroups( QWidget *parent, const char *name )
  // : QWidget( parent, name )
  //{
  // Create Widgets which allow easy layouting
  vbox = new QVBoxLayout;
  box1 = new QHBoxLayout;
  box2 = new QHBoxLayout;
  box3 = new QHBoxLayout;
  box4 = new QHBoxLayout;
  box5 = new QHBoxLayout;
  vbox->addLayout(box1);
  vbox->addLayout(box2);
  vbox->addLayout(box3);
  vbox->addLayout(box5);
  vbox->addLayout(box4);
 

  //this->setPaletteBackgroundColor( QColor(GeneralBackGroundColor) );
  // ----------------------- LOGOs Table -------------------------  
  // Create a button group
  // QGroupBox* bgLogo = new  QGroupBox("Laboratorium fuer Hochenergie physik");
  //box4->addWidget( bgLogo );
  //QHBoxLayout* grid_0 = new QHBoxLayout();

  //UniBe_Pixmap = new QLabel( "UniBePixmap", bgLogo );
  //UniBe_Pixmap->setPixmap( QPixmap::QPixmap( "./UniBe_logo.jpg",0 ) );
  /*  LHEP_Pixmap = new QLabel(  "LHEPPixmap", bgLogo );
  LHEP_Pixmap->setPixmap( QPixmap::QPixmap( "./lhep_logo.jpg", 0) );
  grid_0->addWidget( LHEP_Pixmap);
  *///grid_0->addWidget( UniBe_Pixmap );
  //  bgLogo->setLayout(grid_0);
  

  // ---------------- Run Setting -------------------------
  // Create a button group
  QGroupBox* bgRunSet = new QGroupBox( tr("Run Setting"));
  box1->addWidget( bgRunSet );

  QVBoxLayout* grid0 = new QVBoxLayout;

  RunLab     = new QLabel( "Run",  bgRunSet );  
  RunLab->setToolTip("Set RUN number"); 
  RunSpinBox = new QSpinBox( bgRunSet );
  RunSpinBox->setMinimum ( 1 );
  RunSpinBox->setMaximum ( 100000 );
  RunSpinBox->setValue ( 100 );
   
  EventLab     = new QLabel( "N of Events", bgRunSet );
  EventLab->setToolTip("Set the number of events that you want to acquire"); 
  EventSpinBox = new QSpinBox( bgRunSet );
  EventSpinBox->setMinimum ( 1 );
  EventSpinBox->setMaximum ( 12000 );
  EventSpinBox->setValue ( 10 );
  
  BoardLab     = new QLabel( "Nr of Boards",  bgRunSet );
  BoardLab->setToolTip("Set the number of V1724 ADC boards installed in the crate"); 
  BoardSpinBox = new QSpinBox( bgRunSet );
  BoardSpinBox->setMinimum ( 1 );
  BoardSpinBox->setMaximum ( 16 );
  BoardSpinBox->setValue ( 8 );

  INPUTLab     = new QLabel( "N of Inputs",   bgRunSet );
  INPUTLab->setToolTip("Set the number of INPUTs for every V1724 ADC board"); 
  INPUTSpinBox = new QSpinBox( bgRunSet );
  INPUTSpinBox->setMinimum ( 1 );
  INPUTSpinBox->setMaximum ( 8 );
  INPUTSpinBox->setValue ( 8 );
  
  LOOPLab     = new QLabel( "N of DAQ loops",   bgRunSet  );
  LOOPLab->setToolTip("Set > 1 if you to acquire more Runs continuosly"); 
  LOOPSpinBox = new QSpinBox( bgRunSet );
  LOOPSpinBox->setMinimum ( 1 );
  LOOPSpinBox->setMaximum ( 500 );
  LOOPSpinBox->setValue ( 1 );
  
  LCDLab     = new QLabel( "N of processed ev.",   bgRunSet );
  //LCDLab->setPaletteForegroundColor( QColor(rbForeGroundColor) );
  LCDSpinBox = new QSpinBox( bgRunSet );
  LCDSpinBox->setMinimum ( 0 );
  LCDSpinBox->setMaximum ( 200000 );
  LCDSpinBox->setValue ( 0 );
  LCDSpinBox->setReadOnly(1);
  
  //label_MT1 = new QLabel( "daqpar.nevents", grid0,"V1724" );
  //rbMT_1 = new QLineEdit( grid0, "1" );
  LCDlostLab     = new QLabel( "Lost events",   bgRunSet );
  //LCDlostLab->setPaletteForegroundColor( QColor(rbForeGroundColor) );
  LCDlostSpinBox = new QSpinBox(bgRunSet);
  LCDlostSpinBox->setReadOnly(1);
  //LCDlostSpinBox->setPaletteForegroundColor( QColor(rbForeGroundColor) );
  


  grid0->addWidget( RunLab );
  grid0->addWidget( RunSpinBox );
  grid0->addWidget( EventLab );
  grid0->addWidget( EventSpinBox );
  grid0->addWidget( BoardLab );
  grid0->addWidget( BoardSpinBox );
  grid0->addWidget( INPUTLab );
  grid0->addWidget( INPUTSpinBox );
  grid0->addWidget( LOOPLab );
  grid0->addWidget( LOOPSpinBox );
  grid0->addWidget( LCDLab );
  grid0->addWidget( LCDSpinBox );
  grid0->addWidget( LCDlostLab );
  grid0->addWidget( LCDlostSpinBox );
  bgRunSet->setLayout(grid0);
  
  this->setLayout(vbox);
  
  // ------- Measure Type Botton Group -------------------------
  
  // Create a button group
  QGroupBox* bgV1724Settings = new QGroupBox(tr("V1724 Settings"));
  box1->addWidget( bgV1724Settings );

  QVBoxLayout* grid_V1724 = new QVBoxLayout;

  DAQ_widthLab     = new QLabel( "Acquisition Window", bgV1724Settings );
  DAQ_widthLab->setToolTip("10 =  0.5ksamples\n 9 =   1ksamples\n 8 =   2ksamples\n 7 =   4ksamples\n 6 =   8ksamples\n 5 =  16ksamples\n 4 =  32ksamples\n 3 =  64ksamples\n 2 = 128ksamples\n 1 = 256ksamples\n 0 = 512ksamples");
  DAQ_widthSpinBox = new QSpinBox( bgV1724Settings );
  DAQ_widthSpinBox->setMinimum ( 1 );
  DAQ_widthSpinBox->setMaximum ( 10 );
  DAQ_widthSpinBox->setValue ( 9 );
  
  nPostLab     = new QLabel( "Post Trigger Samples",  bgV1724Settings );
  nPostLab->setToolTip("This number corresponds to Nsamples*2, with Nsamples = number of post-trigger samples");
  nPostSpinBox = new QSpinBox( bgV1724Settings );
  nPostSpinBox->setMinimum ( 5 );
  nPostSpinBox->setMaximum ( 2147483647 ); // 2^31 bit
  nPostSpinBox->setValue ( 300 );
  
  DownSLab     = new QLabel( "Down Sample", bgV1724Settings );
  DownSLab->setToolTip("ONLY EVEN NUMBERS PERMITTED AS INPUT - Change sampling rate. This register allows to set N: sampling frequency will be divided by N+1");
  DownSSpinBox = new QSpinBox( bgV1724Settings);
  //DownSSpinBox->setToolTip("Change sampling rate");
  DownSSpinBox->setMinimum ( 0 );
  DownSSpinBox->setMaximum ( 2147483647 ); // 2^31 bit
  DownSSpinBox->setValue ( 40 );
  DownSSpinBox->setSingleStep(2);
  //  DownSSpinBox->setSuffix("  EVEN nr. ONLY");

  DACLevelLab     = new QLabel( "DAC Offset Level", bgV1724Settings );
  DACLevelLab->setToolTip("Allow to define a DC offset to be added the input signal in the -1.125V - +1.125V range (0-65535)");
  DACLevelSpinBox = new QSpinBox( bgV1724Settings );
  DACLevelSpinBox->setMinimum ( 0 );
  DACLevelSpinBox->setMaximum ( 0xFFFF );
  DACLevelSpinBox->setValue ( 32000 );

  Monitor_ModeLab     = new QLabel( "Monitor Output Mode", bgV1724Settings );
  Monitor_ModeLab->setToolTip(" 0 = Trigger Majority Mode\n 1 = Test Mode \n 2 = Analog Monitor/Inspection Mode\n 3 = Buffer Occupancy Mode\n 4 = Voltage Level Mode");
  Monitor_ModeSpinBox = new QSpinBox( bgV1724Settings);
  Monitor_ModeSpinBox->setMinimum ( 0 );
  Monitor_ModeSpinBox->setMaximum ( 4 );
  Monitor_ModeSpinBox->setValue ( 0 );

  AnMonInvLab     = new QLabel( "Analogic Inspection Inverter", bgV1724Settings );
  AnMonInvLab->setToolTip("Set the analogic inspection of the Analogic Monitor Output - 0=NOT inverted - 1=inverted");
  AnMonInvLab->setEnabled( false );
  AnMonInvSpinBox = new QSpinBox( bgV1724Settings );
  AnMonInvSpinBox->setMinimum ( 0x0 );
  AnMonInvSpinBox->setMaximum ( 0x1 );
  AnMonInvSpinBox->setValue (    0x0 );
  AnMonInvSpinBox->setEnabled( false );

  AnMonMAGLab     = new QLabel( "Analogic Monitor Magnify factor", bgV1724Settings );
  AnMonMAGLab->setToolTip("Magnify Factor:\n 0 = 1x \n 1 = 2x\n 2 = 4x\n 3 = 8x");
  AnMonMAGLab->setEnabled( false );
  AnMonMAGSpinBox = new QSpinBox( bgV1724Settings );
  AnMonMAGSpinBox->setMinimum ( 0x0 );
  AnMonMAGSpinBox->setMaximum ( 0x3 );
  AnMonMAGSpinBox->setValue (    0x0 );
  AnMonMAGSpinBox->setEnabled( false );
  
  AnMonOffSignLab     = new QLabel( "Analogic Monitor Offset Sign",bgV1724Settings );
  AnMonOffSignLab->setToolTip("Set the sign of the offset of the Analogic Monitor Output - 0= pos - 1=neg");
  AnMonOffSignLab->setEnabled( false );
  AnMonOffSignSpinBox = new QSpinBox( bgV1724Settings );
  AnMonOffSignSpinBox->setMinimum ( 0x0 );
  AnMonOffSignSpinBox->setMaximum ( 0x1 );
  AnMonOffSignSpinBox->setValue (    0x1 );
  AnMonOffSignSpinBox->setEnabled( false );
  
  AnMonOffValLab     = new QLabel( "Analogic Monitor Offset Value", bgV1724Settings );
  AnMonOffValLab->setToolTip("Offset voltage of Analogic Monitor - Range = (0-2048)dec or (0-1)V");
  AnMonOffValLab->setEnabled( false );
  AnMonOffValSpinBox = new QSpinBox( bgV1724Settings );
  AnMonOffValSpinBox->setMinimum ( 0x000 );
  AnMonOffValSpinBox->setMaximum ( 0x7FF );
  AnMonOffValSpinBox->setValue (    0x3FF );
  AnMonOffValSpinBox->setEnabled( false );

  MajMoThresholdLab     = new QLabel( "Majority Mode Threshold", bgV1724Settings );
  MajMoThresholdLab->setToolTip("Threshold for each channel for which Monitor output is incremented by 1.");
  MajMoThresholdLab->setEnabled( false );
  MajMoThresholdBox = new QSpinBox( bgV1724Settings );
  MajMoThresholdBox->setMinimum ( 0x000 );
  MajMoThresholdBox->setMaximum ( 0xFFFF );
  MajMoThresholdBox->setValue (    0x21F2 );
  MajMoThresholdBox->setEnabled( false );

  //connect( slider1, SIGNAL(valueChanged(int)), LCD, SLOT(display(int)) );
  grid_V1724->addWidget( DAQ_widthLab );
  grid_V1724->addWidget( DAQ_widthSpinBox );
  grid_V1724->addWidget( nPostLab );
  grid_V1724->addWidget( nPostSpinBox );
  grid_V1724->addWidget( DownSLab );
  grid_V1724->addWidget( DownSSpinBox );
  grid_V1724->addWidget( DACLevelLab );
  grid_V1724->addWidget( DACLevelSpinBox );
  grid_V1724->addWidget( Monitor_ModeLab );
  grid_V1724->addWidget( Monitor_ModeSpinBox );
  grid_V1724->addWidget( AnMonInvLab );
  grid_V1724->addWidget( AnMonInvSpinBox );
  grid_V1724->addWidget( AnMonMAGLab  );
  grid_V1724->addWidget( AnMonMAGSpinBox);
  grid_V1724->addWidget( AnMonOffSignLab );
  grid_V1724->addWidget( AnMonOffSignSpinBox );
  grid_V1724->addWidget( AnMonOffValLab );
  grid_V1724->addWidget( AnMonOffValSpinBox );
  grid_V1724->addWidget( MajMoThresholdLab );
  grid_V1724->addWidget( MajMoThresholdBox );
  grid_V1724->addStretch(99);
 
  bgV1724Settings->setLayout(grid_V1724);
  
  // ------------ Start HV Button Group -----------
  // Create a button group
  QGroupBox* bgHVSettings = new QGroupBox(tr("HV Settings"));
  box1->addWidget( bgHVSettings );

  QVBoxLayout* grid_hv = new QVBoxLayout;
  
  HV_CatLab     = new QLabel( "TPC Cathode HV(V)",  bgHVSettings );
  HV_CatLab->setToolTip("Set the HV of the Cathode of the TPC"); 
  HV_CatSpinBox = new QSpinBox( bgHVSettings );
  HV_CatSpinBox->setMinimum (     0 );
  HV_CatSpinBox->setMaximum ( 500000 );
  HV_CatSpinBox->setValue (    25000 );

  HV_IndLab     = new QLabel( "TPC Set Induction HV(V)",   bgHVSettings );
  HV_IndLab->setToolTip("Set the HV of the Induction Plane of the TPC"); 
  HV_IndSpinBox = new QSpinBox( bgHVSettings );
  HV_IndSpinBox->setMinimum (    0 );
  HV_IndSpinBox->setMaximum ( 3000 );
  HV_IndSpinBox->setValue (       0 );

  //HV_GetIndLab     = new QLabel( "TPC Get Induction HV(V)",   bgHVSettings );
  //HV_GetIndLab->setToolTip("Get the HV of the Induction Plane of the TPC"); 
  HV_GetIndSpinBox = new QSpinBox( bgHVSettings );
  HV_GetIndSpinBox->setMinimum (    0 );
  HV_GetIndSpinBox->setMaximum ( 3000 );
  HV_GetIndSpinBox->setValue (       0 );

  HV_ColLab     = new QLabel( "TPC Collection HV(V)",   bgHVSettings );
  HV_ColLab->setToolTip("Set the HV of the Collection Plane of the TPC"); 
  HV_ColSpinBox = new QSpinBox( bgHVSettings );
  HV_ColSpinBox->setMinimum ( 0 );
  HV_ColSpinBox->setMaximum ( 3000 );
  HV_ColSpinBox->setValue ( 450 );

  //HV_GetColLab     = new QLabel( "TPC Collection HV(V)",   bgHVSettings );
  //HV_GetColLab->setToolTip("Set the HV of the Collection Plane of the TPC"); 
  HV_GetColSpinBox = new QSpinBox( bgHVSettings );
  HV_GetColSpinBox->setMinimum ( 0 );
  HV_GetColSpinBox->setMaximum ( 3000 );
  HV_GetColSpinBox->setValue ( 450 );

  HV_Cat_Set = new QPushButton(tr("&Set Cathode Voltage"));
  HV_Cat_Get = new QPushButton(tr("&Get Cathode Voltage"));
  HV_Ind_Set = new QPushButton(tr("&Set Induction Voltage"));
  HV_Ind_Get = new QPushButton(tr("&Get Induction Voltage"));
  HV_Col_Set = new QPushButton(tr("&Set Collection Voltage"));
  HV_Col_Get = new QPushButton(tr("&Get Collection Voltage"));

  grid_hv->addWidget( HV_CatLab );
  grid_hv->addWidget( HV_CatSpinBox );
  grid_hv->addWidget( HV_Cat_Set );
  grid_hv->addWidget( HV_Cat_Get );
  grid_hv->addWidget( HV_IndLab ); 
  grid_hv->addWidget( HV_IndSpinBox );
  grid_hv->addWidget( HV_Ind_Set );
  grid_hv->addWidget( HV_GetIndSpinBox );
  grid_hv->addWidget( HV_Ind_Get );
  grid_hv->addWidget( HV_ColLab );
  grid_hv->addWidget( HV_ColSpinBox );
  grid_hv->addWidget( HV_Col_Set );
  grid_hv->addWidget( HV_GetColSpinBox );
  grid_hv->addWidget( HV_Col_Get );
 
  bgHVSettings->setLayout(grid_hv);
 

  connect( HV_Ind_Set, SIGNAL(clicked()), this, SLOT(SetVoltageInd()) );
  connect( HV_Ind_Get, SIGNAL(clicked()), this, SLOT(GetVoltageInd()) );
  connect( HV_Col_Set, SIGNAL(clicked()), this, SLOT(SetVoltageCol()) );
  connect( HV_Col_Get, SIGNAL(clicked()), this, SLOT(GetVoltageCol()) );
    
  // ------------ Start OTHER SETTINGS Button Group -----------
  // Create a button group
  QGroupBox* bgOtherSettings = new QGroupBox(tr("Other Settings"));
  box1->addWidget( bgOtherSettings );

  QVBoxLayout* grid_other = new QVBoxLayout;

  SHMLab = new QLabel(tr("SHM delete"),  bgOtherSettings  );
  SHMLab->setToolTip("Check it if you want to delete SHM"); 
  cb_SHM  = new QCheckBox( "yes/no");
  cb_SHM->setChecked( TRUE );
  
  SaveFileLab = new QLabel(tr("Save File"), bgOtherSettings );
  SaveFileLab->setToolTip("Check it if you want to Save current Run in a file"); 
  cb_SaveFile  = new QCheckBox( "yes/no" );
  //cb_SaveFile->setPaletteForegroundColor( QColor(rbForeGroundColor) );
  cb_SaveFile->setChecked( FALSE );

  TCPLab = new QLabel(tr("TCP Connection"), bgOtherSettings );
  TCPLab->setToolTip("Do you want to read the value of the Energy meter?"); 
  cb_TCP  = new QCheckBox( "yes/no" );
  //cb_SaveFile->setPaletteForegroundColor( QColor(rbForeGroundColor) );
  cb_TCP->setChecked( TRUE );

  LaserLab = new QLabel(tr("Get Laser Position"), bgOtherSettings );
  LaserLab->setToolTip("Turn off/on getting + saving the laser position from encoders"); 
  cb_Laser  = new QCheckBox( "on/off" );
  //cb_SaveFile->setPaletteForegroundColor( QColor(rbForeGroundColor) );
  cb_Laser->setChecked( TRUE );

  St_DAQ       = new QPushButton(tr("&Start DAQ"));
  Imaging      = new QPushButton(tr("&Imaging Display"));
  Oscilloscope = new QPushButton(tr("&Oscilloscope"));
  quit         = new QPushButton(tr("&Quit"));

  LHEP_Pixmap = new QLabel(  "LHEPPixmap", bgOtherSettings );
  LHEP_Pixmap->setPixmap( QPixmap::QPixmap( "./lhep_logo.jpg", 0) );
  


  grid_other->addWidget( SHMLab );
  grid_other->addWidget( cb_SHM );
  grid_other->addWidget( SaveFileLab );
  grid_other->addWidget( cb_SaveFile );
  grid_other->addWidget( TCPLab );
  grid_other->addWidget( cb_TCP );
  grid_other->addWidget( LaserLab );
  grid_other->addWidget( cb_Laser );
  grid_other->addWidget( LHEP_Pixmap);
  grid_other->addWidget( St_DAQ );
  grid_other->addWidget( Imaging );
  //grid_other->addWidget( Oscilloscope );
  grid_other->addWidget( quit );

  
  bgOtherSettings->setLayout(grid_other);

  // ------------ Start DAQ Button Group -----------
  
  // create a groupbox which layouts its childs in a columns
  //QGroupBox* bgrp4 = new QGroupBox( "Command Buttons" );
  //box4->addWidget( bgrp4 );
  //QGridLayout *grid5 = new QGridLayout( 2 , bgrp4);
  //grid5->setSpacing( int( 5 ) );
  
  // insert pushbuttons...

  
  /*QVBoxLayout *but_vbox = new QVBoxLayout;
  but_vbox->addWidget( St_DAQ );
  but_vbox->addWidget( Imaging);
  but_vbox->addWidget( Oscilloscope);
  but_vbox->addWidget( quit );
  but_vbox->addStretch(1);
  bgrp4->setLayout(but_vbox);
*/


  connect( DownSSpinBox, SIGNAL(valueChanged(int)), this, SLOT(EvenAllowedForDownsampling(int)) ); 
  connect( HV_Cat_Set, SIGNAL(clicked()), this, SLOT(SetVoltageCathode()) ); 
  connect( HV_Cat_Get, SIGNAL(clicked()), this, SLOT(GetVoltageCathode()) );
  connect( Monitor_ModeSpinBox, SIGNAL(valueChanged( int )), this, SLOT(Monitor_Mode()) );
  connect( St_DAQ, SIGNAL(clicked()), this, SLOT(Daq_start()) );
  connect( Oscilloscope, SIGNAL( clicked() ), this, SLOT( Oscilloscope_RT_start() ) );
  connect( Imaging, SIGNAL( clicked() ), this, SLOT( Imaging_RT_start() ) );
  connect( quit, SIGNAL(clicked()), qApp, SLOT( quit() ) );
  connect( DAQ_widthSpinBox,SIGNAL(valueChanged(int)), this, SLOT( slotChangeGrp3State() ) );
  connect( nPostSpinBox,SIGNAL(valueChanged(int)), this, SLOT( PostTriggerRange(int) ) ); 
  connect( RunSpinBox,SIGNAL(valueChanged(int)), this, SLOT( RunNumberCheck(int) ) ); 
}


// SLOT RunNumber check
void MyWidget::RunNumberCheck(int value)
{
  int run = RunSpinBox->value();
  char kill[64]= "ls /data/Run_00000000";
  sprintf(&kill[9], "%.08i", run);
  sprintf(&kill[19], " >runlist_tmp.txt"); 
  //cout << endl;
  //cout << "kill= " << kill << endl;
  
  int ret4 = system(kill);
  //  printf("\n ret run check= %d",ret4);
  
  if(ret4==0)St_DAQ->setEnabled( false );
  if(ret4!=0)St_DAQ->setEnabled( true );

}


// SLOT PostTrigger not larger than acquisition window
void MyWidget::PostTriggerRange(int value)
{
    int   SAMPLES = 0;
  switch ( DAQ_widthSpinBox->value() ){
  case 10:
    SAMPLES =    512; 
    break;
  case  9:
    SAMPLES =   1024; 
    break;
  case  8:
    SAMPLES =   2048; 
    break;
  case  7:
    SAMPLES =   4096; 
    break;
  case  6:
    SAMPLES =   8192; 
    break;
  case  5:
    SAMPLES =  16384; 
    break;
  case  4:
    SAMPLES =  32768; 
    break;
  case  3:
    SAMPLES =  65536; 
    break;
  case  2:
    SAMPLES = 131072; 
    break;
  case  1:
    SAMPLES = 262144; 
    break;
  case  0:
    SAMPLES = 524288; 
    break;
  }

  // Down Sampling  
  value = nPostSpinBox->value();
  
  if (value*2>SAMPLES){
    nPostSpinBox->setValue(SAMPLES/3);
    cout << "Value of Post Trigger not permitted. You can set maximum equal to " << SAMPLES/2 <<"only 2*MAX_SAMPLES - now PostTrigger set to MAX_SAMPLES/3 = " << SAMPLES/2 << endl;
  }
  
}

// SLOT onlyEvenNumbersForDownSampling
void MyWidget::EvenAllowedForDownsampling(int odd)
{
  // Down Sampling  
  odd = DownSSpinBox->value();
  if (odd % 2) {
    DownSSpinBox->setValue(odd+1);
    if(0)cout << "DS adjousted to even number2= " << DownSSpinBox->value() << endl;
  }
  
}

// SLOT slotChangeGrp3State()
void MyWidget::slotChangeGrp3State()
{
  //DAQ_widthSpinBox->ChengedValue();
  cb_SHM->setChecked( TRUE );
  
  // Width->setEnabled( cbLED_ON->isChecked() );
}

// SLOT slotChangeGrp3State()
//*void ButtonsGroups::Quit()
//{    
//  pthread_kill(LCDt, SIGKILL);
//  connect( quit, SIGNAL(clicked()), qApp, SLOT(quit()) );
// }


// SLOT slotChangeGrp3State()
void MyWidget::Monitor_Mode()
{
  //DAQ_widthSpinBox->ChengedValue();
  if(Monitor_ModeSpinBox->value()==2){
    AnMonInvLab->setEnabled( true );
    AnMonInvSpinBox->setEnabled( true );
    AnMonMAGLab->setEnabled( true );
    AnMonMAGSpinBox->setEnabled( true );
    AnMonOffSignLab->setEnabled( true );
    AnMonOffSignSpinBox->setEnabled( true );
    AnMonOffValLab->setEnabled( true );
    AnMonOffValSpinBox->setEnabled( true );
    MajMoThresholdLab->setEnabled( false );
    MajMoThresholdBox->setEnabled( false );
  }else if(Monitor_ModeSpinBox->value()==0){
    MajMoThresholdLab->setEnabled( true );
    MajMoThresholdBox->setEnabled( true );
    AnMonInvLab->setEnabled( false );
    AnMonInvSpinBox->setEnabled( false );
    AnMonMAGLab->setEnabled( false );
    AnMonMAGSpinBox->setEnabled( false );
    AnMonOffSignLab->setEnabled( false );
    AnMonOffSignSpinBox->setEnabled( false ); 
    AnMonOffValLab->setEnabled( false );
    AnMonOffValSpinBox->setEnabled( false );
  }else{
    AnMonInvLab->setEnabled( false );
    AnMonInvSpinBox->setEnabled( false );
    AnMonMAGLab->setEnabled( false );
    AnMonMAGSpinBox->setEnabled( false );
    AnMonOffSignLab->setEnabled( false );
    AnMonOffSignSpinBox->setEnabled( false );
    AnMonOffValLab->setEnabled( false );
    AnMonOffValSpinBox->setEnabled( false );
    MajMoThresholdLab->setEnabled( false );
    MajMoThresholdBox->setEnabled( false );
  }
}
        

// Start Oscilloscope Real Time viewer
void MyWidget::Oscilloscope_RT_start()
{
  pid_t pid;
  // char argv2[10];
  //sprintf(argv2,"%d", EventSpinBox->value() );
  char *arg_vector[]={"../Oscilloscope_RT/Oscilloscope", NULL};

  //creazione del nuovo processo che esegue Tmonitor   
  if((pid = fork())== 0)
    {
      //il nuovo processo esegue Tmonitor
      execv("../Oscilloscope_RT/Oscilloscope",arg_vector);
    }
  
  //gestione degli errori per la fork
  if( pid < 0 )
    perror("Fork Error"),exit(1);
}

// Start Imaging Real Time viewer
void MyWidget::Imaging_RT_start()
{
  pid_t pid;
  int   SAMPLES = 0;
  switch ( DAQ_widthSpinBox->value() ){
  case 10:
    SAMPLES =    512; 
    break;
  case  9:
    SAMPLES =   1024; 
    break;
  case  8:
    SAMPLES =   2048; 
    break;
  case  7:
    SAMPLES =   4096; 
    break;
  case  6:
    SAMPLES =   8192; 
    break;
  case  5:
    SAMPLES =  16384; 
    break;
  case  4:
    SAMPLES =  32768; 
    break;
  case  3:
    SAMPLES =  65536; 
    break;
  case  2:
    SAMPLES = 131072; 
    break;
  case  1:
    SAMPLES = 262144; 
    break;
  case  0:
    SAMPLES = 524288; 
    break;
  }

  int      DS           = 10 * ( DownSSpinBox->value() + 1);
  cout << "DS= " << DS << endl;

  char argv2[10], argv3[10], argv4[10];
  sprintf(argv2,"%d", SAMPLES );
  sprintf(argv3,"%d", BoardSpinBox->value() );
  sprintf(argv4,"%i", DS );

  char *arg_vector[]={"../Imaging_RT/Tmonitor", argv2, argv3, argv4, NULL};
  
  //creazione del nuovo processo che esegue Tmonitor   
  if((pid = fork())== 0)
    {
      //il nuovo processo esegue Tmonitor
      execv("../Imaging_RT/Tmonitor",arg_vector);
    }
  
  //gestione degli errori per la fork
  if( pid < 0 )
    perror("Fork Error"),exit(1);
}

// Start DAQ button's action
void MyWidget::Daq_start()
{
  bool debug   = false;
  bool shm_del = cb_SHM->checkState();
  int  SAMPLES = 0;
  //  wait(NULL);
  switch ( DAQ_widthSpinBox->value() ){
  case 10:
    SAMPLES =    512; 
    break;
  case  9:
    SAMPLES =   1024; 
    break;
  case  8:
    SAMPLES =   2048; 
    break;
  case  7:
    SAMPLES =   4096; 
    break;
  case  6:
    SAMPLES =   8192; 
    break;
  case  5:
    SAMPLES =  16384; 
    break;
  case  4:
    SAMPLES =  32768; 
    break;
  case  3:
    SAMPLES =  65536; 
    break;
  case  2:
    SAMPLES = 131072; 
    break;
  case  1:
    SAMPLES = 262144; 
    break;
  case  0:
    SAMPLES = 524288; 
    break;
  }
   
  //delete the shmbuf, sembuf, shmdat, semdat
  if(shm_del){
  
    int ret4 = system("ps -efa | grep ../Imaging_RT/Tmonitor >tmp.txt");
    if(debug)printf("\n ret sembuf= %d",ret4);
    
    string line;
    string user, time, time2, b, c, d;
    int pid_ID, pid2, a;
    
    std::ifstream iheader( "tmp.txt" );
    if(!iheader.is_open()){
      cout << "Error in opening tmp.txt File - file to check if the Tmonitor process is still running"<<endl;
      exit(2);
    }
    //  getline(iheader,line);
    iheader >> user >> pid_ID >> pid2 >> a >> time >> b >> time2 >> c >> d;
    cout << "  Tot_events=" <<pid_ID  << "   Process= " <<d <<endl;

    char kill[64];
    sprintf(kill, "kill -9 %d", pid_ID);
    cout << "kill= " << kill << endl;
    int ret5 = system(kill);
    if(debug)printf("\n ret sembuf= %d",ret5);
    
    //sleep(5);
    
    cout << "shm_del ="<< shm_del <<endl;
    int ret0 = system("ipcrm -S 0x001d9b79");
    if(debug)printf("\n ret sembuf= %d",ret0);

    int ret1 = system("ipcrm -M 0x003d8991");
    if(debug)printf("\n ret shmbuf= %d",ret1);
    
    int ret2 = system("ipcrm -S 0x001d9b7a");
    if(debug)printf("\n ret sembuf= %d",ret2);
    
    int ret3 = system("ipcrm -M 0x003d8992");
    if(debug)printf("\n ret shmbuf= %d",ret3);
  }
  


  //pthread_t LCDt;
  pid_t     pid;
  char argv2[10] , argv3[10] ,  argv4[10],  argv5[10];
  char argv6[10] , argv7[10] ,  argv8[10],  argv9[10];
  char argv10[10], argv11[10], argv12[10], argv13[10];
  char argv14[10], argv15[10], argv16[10], argv17[10];
  char argv18[10], argv19[10], argv20[10], argv21[10];
  char argv22[10], argv23[10];
  sprintf(argv2, "%d", EventSpinBox->value() );
  sprintf(argv3, "%d", INPUTSpinBox->value() );
  sprintf(argv4, "%d", RunSpinBox->value() );
  sprintf(argv5, "%d", LOOPSpinBox->value() );
  sprintf(argv6, "%d", DAQ_widthSpinBox->value() );
  sprintf(argv7, "%d", SAMPLES );
  sprintf(argv8, "%d", nPostSpinBox->value() );
  sprintf(argv9, "%d", Monitor_ModeSpinBox->value() );
  sprintf(argv10,"%d", !cb_SaveFile->checkState());
  sprintf(argv11,"%d", BoardSpinBox->value() );
  sprintf(argv12,"%d", DACLevelSpinBox->value() );
  sprintf(argv13,"%d", DownSSpinBox->value() );
  sprintf(argv14,"%d", AnMonInvSpinBox->value() );
  sprintf(argv15,"%d", AnMonOffSignSpinBox->value() );
  sprintf(argv16,"%d", AnMonOffValSpinBox->value() );
  sprintf(argv17,"%d", AnMonMAGSpinBox->value() );
  sprintf(argv18,"%d", HV_CatSpinBox->value() );
  sprintf(argv19,"%d", HV_IndSpinBox->value() );
  sprintf(argv20,"%d", HV_ColSpinBox->value() );
  sprintf(argv21,"%d", !cb_TCP->checkState());
  sprintf(argv22,"%d", !cb_Laser->checkState());
  sprintf(argv23,"%d", MajMoThresholdBox->value() );

  char *arg_vector[]={"../daq_main", argv2, argv3, argv4, argv5, argv6, argv7, argv8, argv9, argv10, argv11, argv12, argv13, argv14, argv15, argv16, argv17, argv18, argv19, argv20, argv21, argv22, argv23, NULL};
  
  //Creation of LCD thread
  //pthread_create(&LCDt, NULL, LCD_th, NULL);
  
  //creazione del nuovo processo che esegue daq_main
  if((pid = fork())== 0)
    {
      //execv("../daq_main",arg_vector);
      execv("../daq_main",arg_vector);
    }
  //gestione degli errori per la fork
  if( pid < 0 )
    perror("Fork Error"),exit(1);  

  cb_SHM->setChecked( FALSE );
}


// Start DAQ button's action
void MyWidget::SetVoltageCathode(){
  
  // pthread_t SetVoltage;
  pid_t     pid;
  char argv2[10] , argv3[10];
  
  float volt = (float)(HV_CatSpinBox->value()/1000.);

  sprintf(argv2,"%i", 0 ); //Read voltage OFF
  sprintf(argv3,"%f", volt );

  char *arg_vector[]={"/opt/HV_Cathode_Control/HVcontrol", argv2, argv3, NULL};
  
  //creazione del nuovo processo che esegue daq_main
  if((pid = fork())== 0)execv("/opt/HV_Cathode_Control/HVcontrol",arg_vector);
  
  //gestione degli errori per la fork
  if( pid < 0 )perror("Fork Error"),exit(1);  
  
}

// Start DAQ button's action
void MyWidget::GetVoltageCathode(){
  
  pid_t     pid;
  char argv2[10] , argv3[10];
  
  // sprintf(argv2,"%i", ">read_voltage.txt" ); //Output file
  sprintf(argv2,"%i", 1 ); //Read voltage ON
  sprintf(argv3,"%f", 0. );

  //printf(argv2,"%d", HV_CatSpinBox->value() );
  
  char *arg_vector[]={"/opt/HV_Cathode_Control/HVcontrol", argv2, argv3, NULL};
  
  //creazione del nuovo processo che esegue daq_main
  if((pid = fork())== 0)execv("/opt/HV_Cathode_Control/HVcontrol", arg_vector);
  
  //gestione degli errori per la fork
  if( pid < 0 )perror("Fork Error"),exit(1);  
 
  usleep(100000);

  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen ( "./read_voltage.bin" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
  
  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  
  cout << " Size = " << lSize<< endl;
  rewind (pFile);
  // char Voltage[128];
  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  fseek(pFile, 26,SEEK_SET);
  result = fread (buffer,1,7,pFile);
  //if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
  
  cout << buffer << endl;

  float readV = atof(buffer)*1000;
  cout << readV << endl;
  HV_CatSpinBox->setValue((int)readV);

  // terminate
  fclose (pFile);
  free (buffer);
  
}

// Set Voltage NIM channel 
void MyWidget::SetVoltageInd(){
  
  pid_t     pid;
  char argv2[10], argv3[10], argv4[10] , argv5[10];
  char argv6[10], argv7[10];
  
  int volt = HV_IndSpinBox->value();

  sprintf(argv2,"%i", 1 );    // Channel number IND==1
  sprintf(argv3,"%i", 0 );    // Get CURRENT
  sprintf(argv4,"%i", 0 );    // GetVoltage
  sprintf(argv5,"%i", 1 );    // Update voltage (switch ON hv)
  sprintf(argv6,"%i", 1 );    // Set Ramp of 20V/s
  sprintf(argv7,"%i", volt ); // Voltage value to be set

  char *arg_vector[]={"/opt/HV_NIM/HVcontrol", argv2, argv3, argv4, argv5, argv6, argv7, NULL};
  
  //creazione del nuovo processo che esegue daq_main
  if((pid = fork())== 0)execv("/opt/HV_NIM/HVcontrol",arg_vector);
  
  //gestione degli errori per la fork
  if( pid < 0 )perror("Fork Error"),exit(1);  
  
}

// Get Voltage NIM channel 
void MyWidget::GetVoltageInd(){

  pid_t     pid;
  char argv2[10], argv3[10], argv4[10] , argv5[10];
  char argv6[10], argv7[10];
  
  int volt = HV_IndSpinBox->value();

  sprintf(argv2,"%i", 1 );    // Channel number IND==1
  sprintf(argv3,"%i", 0 );    // Get CURRENT
  sprintf(argv4,"%i", 1 );    // GetVoltage
  sprintf(argv5,"%i", 0 );    // Update voltage (switch ON hv)
  sprintf(argv6,"%i", 0 );    // Set Ramp of 20V/s
  sprintf(argv7,"%i", volt ); // Voltage value to be set

  char *arg_vector[]={"/opt/HV_NIM/HVcontrol", argv2, argv3, argv4, argv5, argv6, argv7, NULL};
  
  //creazione del nuovo processo che esegue daq_main
  if((pid = fork())== 0)execv("/opt/HV_NIM/HVcontrol",arg_vector);
  
  //gestione degli errori per la fork
  if( pid < 0 )perror("Fork Error"),exit(1);  
  
 usleep(5000000);

  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen ( "./read_wire_voltage.bin" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
  
  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  
  //cout << " Size = " << lSize<< endl;
  rewind (pFile);
  // char Voltage[128];
  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  //fseek(pFile, 26,SEEK_SET);
  result = fread (buffer,1,lSize,pFile);
  //  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
  
  cout << buffer << endl;

  int readV = atoi(buffer)/10;
  //cout << readV << endl;
  HV_GetIndSpinBox->setValue((int)readV);

  // terminate
  fclose (pFile);
  free (buffer);
}


// Set Voltage NIM COLLECTION 
void MyWidget::SetVoltageCol(){
 
  pid_t     pid;
  char argv2[10], argv3[10], argv4[10] , argv5[10];
  char argv6[10], argv7[10];
  
  int volt = HV_ColSpinBox->value();

  sprintf(argv2,"%i", 2 );    // Channel number COL==2
  sprintf(argv3,"%i", 0 );    // Get CURRENT
  sprintf(argv4,"%i", 0 );    // GetVoltage
  sprintf(argv5,"%i", 1 );    // Update voltage (switch ON hv)
  sprintf(argv6,"%i", 1 );    // Set Ramp of 20V/s
  sprintf(argv7,"%i", volt ); // Voltage value to be set

  char *arg_vector[]={"/opt/HV_NIM/HVcontrol", argv2, argv3, argv4, argv5, argv6, argv7, NULL};
  
  //creazione del nuovo processo che esegue daq_main
  if((pid = fork())== 0)execv("/opt/HV_NIM/HVcontrol",arg_vector);
  
  //gestione degli errori per la fork
  if( pid < 0 )perror("Fork Error"),exit(1);  
  
}

// Get Voltage NIM channel COLLECTION
void MyWidget::GetVoltageCol(){
     
  pid_t     pid;
  char argv2[10], argv3[10], argv4[10] , argv5[10];
  char argv6[10], argv7[10];
  
  int volt = HV_ColSpinBox->value();

  sprintf(argv2,"%i", 2 );    // Channel number IND==1
  sprintf(argv3,"%i", 0 );    // Get CURRENT
  sprintf(argv4,"%i", 1 );    // GetVoltage
  sprintf(argv5,"%i", 0 );    // Update voltage (switch ON hv)
  sprintf(argv6,"%i", 0 );    // Set Ramp of 20V/s
  sprintf(argv7,"%i", volt ); // Voltage value to be set

  char *arg_vector[]={"/opt/HV_NIM/HVcontrol", argv2, argv3, argv4, argv5, argv6, argv7, NULL};
  
  //creazione del nuovo processo che esegue daq_main
  if((pid = fork())== 0)execv("/opt/HV_NIM/HVcontrol",arg_vector);
  
  //gestione degli errori per la fork
  if( pid < 0 )perror("Fork Error"),exit(1);  
  
  usleep(5000000);

  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen ( "./read_wire_voltage.bin" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
  
  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  
  //cout << " Size = " << lSize<< endl;
  rewind (pFile);
  // char Voltage[128];
  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  //fseek(pFile, 26,SEEK_SET);
  result = fread (buffer,1,lSize,pFile);
  //  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
  
  cout << buffer << endl;

  int readV = atoi(buffer)/10;
  //cout << readV << endl;
  HV_GetColSpinBox->setValue((int)readV);

  // terminate
  fclose (pFile);
  free (buffer);
}
