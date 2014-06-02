
//#include "CAENVMElib.h" 
// by MLuethi
#include "./CAEN_include/CAENVMElib.h"

#ifdef DAQ_VME
const unsigned int AMC_FPGA_Firmware_Revsion    = 0x108C;
const unsigned int ROC_FPGA_Firmware_Revsion    = 0x8124;
const unsigned int SW_Reset_register            = 0xEF24;
const unsigned int Front_Panel_IO_Control       = 0x811C;
const unsigned int Monitor_Mode                 = 0x8144;
const unsigned int Post_Trigger_Settings        = 0x8114;
const unsigned int Buffer_Organization          = 0x800C;
const unsigned int Trigger_Source_Enable        = 0x810C;
const unsigned int Trigger_OUT_Enable           = 0x8110;
const unsigned int BLT_Event_Number_Transferred = 0xEF1C;
const unsigned int Acquisition_control          = 0x8100;
const unsigned int Channel_configuration        = 0x8000;
const unsigned int Analog_Monitor               = 0x8150;
const unsigned int Ch_Enable_Mask               = 0x8120;
const unsigned int Downsample                   = 0x8128;
const unsigned int MCST                         = 0xEF0C;
const unsigned int Event_Stored                 = 0x812C;
const unsigned int Acquisition_Status           = 0x8104;
const unsigned int VME_Control                  = 0xEF00;
const unsigned int Board_ID                     = 0xEF08;
const unsigned int Custom_size                  = 0x8020;
unsigned int DAC_Level                          = 0x1098;
const unsigned int Threshold                    = 0x1080;
const unsigned int n_over_Threshold             = 0x1084;



const unsigned int  Board_00                    = 0x32100000;
const unsigned int  Board_01                    = 0x32110000; 
const unsigned int  Board_02                    = 0x32120000;  
const unsigned int  Board_03                    = 0x32130000; 
const unsigned int  Board_04                    = 0x32140000; 
const unsigned int  Board_05                    = 0x32150000; 
const unsigned int  Board_06                    = 0x32160000;  
const unsigned int  Board_07                    = 0x32170000; 
const unsigned int  Board_08                    = 0x32180000; 
const unsigned int  Board_09                    = 0x32190000; 
const unsigned int  Board_10                    = 0x321A0000;  
const unsigned int  Board_11                    = 0x321B0000; 
const unsigned int  Board_12                    = 0x321C0000; 
const unsigned int  Board_13                    = 0x321D0000; 
const unsigned int  Board_14                    = 0x321E0000;  
const unsigned int  Board_15                    = 0x321F0000; 

const unsigned int  fst_brd                     = 0x32100000;
//const unsigned int  lst_brd                     = 0x321F0000;
const unsigned int  CBLT_add                    = 0xBB000000;

int           BHandle;
unsigned short global_scaler;

#else

extern long           BHandle;
extern unsigned short global_scaler;

#endif

