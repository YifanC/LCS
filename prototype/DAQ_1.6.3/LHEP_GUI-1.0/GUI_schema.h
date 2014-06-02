#ifndef BUTTONS_GROUPS_H
#define BUTTONS_GROUPS_H

#include <qwidget.h>
#include <sys/signal.h>

//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
//#include <Q3PopupMenu>
//#include <TThread.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;

class QLineEdit;
class QLabel;
class QGridLayout;
class QComboBox;
class QCheckBox;
class QSpinBox;
class QTabWidget;
class QWidget;
class QButtonGroup;
class QPushButton;
class QRadioButton;
class QGroupBox;
class TThread;
class QLCDNumber;

extern unsigned short* shmdat_r;
extern void* LCD_th(void*); 
extern int biagio123;

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    //  ButtonsGroups( QWidget *parent = 0, const char *name = 0 );
    MyWidget(QWidget *parent = 0);
    //QSlider      *slider1; 

protected:
   
    // ------------ Start DAQ Button Group -----------
    //QButtonGroup *bgrp4; 
    QPushButton   *St_DAQ, *quit, *tb4, *HV_Cat_Set, *HV_Cat_Get, *HV_Ind_Set, *HV_Ind_Get,*HV_Col_Set, *HV_Col_Get;
    QPushButton   *Oscilloscope, *Imaging;
    // QPopupMenu   *menu;
    // --------------------------------------------------


    // --------- BEGIN Declaration Layout Widget --------
    QVBoxLayout  *vbox; 

    QHBoxLayout  *box1;
    QHBoxLayout  *box2;
    QHBoxLayout  *box3;
    QHBoxLayout  *box4;
    QHBoxLayout  *box5;
    // ---------------------------------------------------

    // - BEGIN Declaration for RUN Setting Button Groups -
    // QButtonGroup *bgRunSet;
    
    //QGridLayout        *grid0;
    
    /* QLCDNumber   *LCD; */

    QLabel       *RunLab;
    QLabel       *EventLab;
    QLabel       *DAQ_widthLab;
    QLabel       *nPostLab;
    QLabel       *Monitor_ModeLab;
    QLabel       *INPUTLab;
    QLabel       *BoardLab;
    QLabel       *DACLevelLab;
    QLabel       *DownSLab;
    QLabel       *AnMonInvLab;
    QLabel       *AnMonMAGLab;
    QLabel       *AnMonOffSignLab;
    QLabel       *AnMonOffValLab;
    QLabel       *MajMoThresholdLab;
    QLabel       *Lost_eventLab;
    QLabel       *LOOPLab;
    QLabel       *HV_CatLab;
    QLabel       *HV_IndLab;
    // QLabel       *HV_GetIndLab;
    QLabel       *HV_ColLab;
   
    QSpinBox     *RunSpinBox;
    /* QSpinBox     *EventSpinBox; */
    QSpinBox     *DAQ_widthSpinBox;
    QSpinBox     *nPostSpinBox;
    QSpinBox     *Monitor_ModeSpinBox;
    QSpinBox     *INPUTSpinBox;
    QSpinBox     *BoardSpinBox;
    QSpinBox     *DACLevelSpinBox;
    QSpinBox     *AnMonInvSpinBox;
    QSpinBox     *AnMonMAGSpinBox; 
    QSpinBox     *AnMonOffSignSpinBox;
    QSpinBox     *AnMonOffValSpinBox;
    QSpinBox     *MajMoThresholdBox;
    QSpinBox     *DownSSpinBox;
    QSpinBox     *LOOPSpinBox;
    QSpinBox     *HV_CatSpinBox;
    QSpinBox     *HV_IndSpinBox;
    QSpinBox     *HV_GetIndSpinBox;
    QSpinBox     *HV_ColSpinBox;
    QSpinBox     *HV_GetColSpinBox;
    // ----------------------------------------------------

    // --- BEGIN V1724 Board Settings --------------------
    //QButtonGroup *bgV1724Settings;
    // QGridLayout        *grid_V1724;
   
    QLabel       *label_MT2;
    QLabel       *label_MT3;
    QLabel       *label_MT4;
    QLabel       *label_MT5;
    QLabel       *label_MT6;
    QLabel       *label_MT7;
    QLabel       *label_MT8;
    QLabel       *label_MT9;
    
    QLabel       *LCDLab;
    QLabel       *LCDlostLab;
    QLabel       *SHMLab;
    QLabel       *SaveFileLab;
    QLabel       *TCPLab;
    QLabel       *LaserLab;

    QCheckBox    *cb_SHM;
    QCheckBox    *cb_SaveFile;
    QCheckBox    *cb_TCP;
    QCheckBox    *cb_Laser;
 
    //  QSpinBox     *LCDSpinBox; 
/*     QLCDNumber   *rbMT_3; */
/*     QLCDNumber   *rbMT_4; */
/*     QLCDNumber   *rbMT_5; */
/*     QLCDNumber   *rbMT_6; */
/*     QLCDNumber   *rbMT_7; */
// ---------------------------------------------------

    // ----- BEGIN Declaration Filling Botton Group ------
    //QButtonGroup *bgFilling;

    QRadioButton *rbFill_1;
    QRadioButton *rbFill_2;
    QRadioButton *rbFill_3;
    QRadioButton *rbFill_4;
    // ---------------------------------------------------

    // - BEGIN Declaration for LED Setting Button Groups -
    // QButtonGroup *bgLEDSet; 
    
    // QGridLayout        *grid; 
    
    QLabel       *LEDLab;
    QLabel       *AmplitudeLab;
    QLabel       *WidthLab;
    QLabel       *RiseTimeLab;
    QLabel       *FallTimeLab;
    QLabel       *RateLab;
    
    QSpinBox     *Amplitude;
    QSpinBox     *Width;
    QSpinBox     *RiseTime;
    QSpinBox     *FallTime;
    QSpinBox     *Rate;
    
    //QCheckBox    *cbLED_ON;
    // ---------------------------------------------------

    
    //------ BEGIN Declaration Shifter names Area --------
    QButtonGroup *bgResNames;
    
    QGridLayout  *Res_grid;
    
    QLabel       *Res_Lab[6];
    
    QComboBox    *c2[6];
    //----------------------------------------------------

    // ------- BEGIN Declaration LOGOs Table -------------
    QButtonGroup *bgLogo;
    
    QGridLayout        *grid_0;
    
    QLabel        *LHEP_Pixmap;
    QLabel        *UniBe_Pixmap;
    // ---------------------------------------------------

    //TThread myth;

protected slots:

  // --------- Funzione che fa diventare grigi i componenti di LED Setting
    

  void EvenAllowedForDownsampling(int);
  void slotChangeGrp3State();
//void Quit();
 void Monitor_Mode();    
 void Daq_start();
 void Oscilloscope_RT_start();
 void Imaging_RT_start();
 void GetVoltageCathode();
 void SetVoltageCathode();
 void GetVoltageInd();
 void SetVoltageInd();
 void GetVoltageCol();
 void SetVoltageCol();
 void PostTriggerRange(int value);
 void RunNumberCheck(int value);
 //void Quit();

//void* DAQ_exec(void*);

  // --------- Funzione di connessione al DBase
// void SQLConnection(); 

};

#endif
