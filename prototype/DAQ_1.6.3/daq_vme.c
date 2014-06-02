#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

// TCP includes
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef _LHEP_USE_SHM_

#define  LHEP_USE_SHM_BUFFER 1
#define  LHEP_USE_SHM_DATA 1 

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#endif

#define DAQ_VME 1

#include "daq_main.h"
#include "daq_vme.h"
#include "daq_shm.h"

typedef int bool; 
enum {false = 0, true = 1}; 

extern ev_header eh;
extern ev_laser laser;
extern bool      debug;
extern bool      forced_stop;

int  daq_vme_init(void);                 // Init all VME boards 
void daq_vme_end(void);                  // Close connection VME-PC
int  daq_TCP_init(void);                 // Init TCP Connection (as Server)
int  daq_v2718_init(void);               // Init Controller CAEN V2718
int  daq_v1724_init(unsigned int, int);  // Init ADC Boards CAEN V1724 - 10  ns sampling
int  daq_v1729_init(unsigned int);       // Init ADC 2GS/s Board       - 0.5 ns sampling
inline int  daq_v1724_MBLT(unsigned int *); //Perform a Read BLT loop to all ADC boards
int daq_v1729_MBLT(); // Perform a Read BLT loop to V1729 2GS/s board
int daq_TCP_read(void);
int posLaserRead (void);
extern int readPos(ev_laser *);

FILE* phout;

//inline void daq_v2817_update_scaler(void);

// Control Board Setting Variables for GUI
unsigned int AMC_Firm, ROC_Firm,  FPctrl,     CCctrl,    MASKctrl,   ANALOGctrl;
unsigned int MMctrl,   PTctrl,    BUFORGctrl, TRGINctrl, TRGOUTctrl, DACctrl;
unsigned int BLTctrl,  BrdIDctrl, DSctrl,     VMEctrl,   MCSTctrl,   RUNctrl, SIZEctrl;

// GUI selection field to ON/OFF the photodiode acquisition
bool LASER = 0;

// GUI selection field to ON/OFF the TCP connection (energy meter acq.)
bool TCP_connection = 0;

// Initialization of the Controller CAEN V2718
int daq_v2718_init(void) {

  CVBoardTypes  VMEBoard;
  short         Link;
  short         Device;

  const short   fmode = 1;

  // Standard setting for the controller V2718
  VMEBoard = cvV2718;
  Device   = 0;
  Link     = 0;

  // Inizialize the controller
  if( CAENVME_Init(VMEBoard, Device, Link, &BHandle) != cvSuccess ) {
    printf("\n\n Error opening V2718\n");
    return -1;
  }
  
  // Disables the auto increment of the VME addresses during the block transfer cycles.
   if( CAENVME_SetFIFOMode(BHandle, fmode) != cvSuccess ) { 
     printf("\n\n Error setting FIFO mode\n"); 
     return -1; 
   } 

  return 0;
}

// Initialization of the TCP Connection
int my_socket; 
int working_socket;
int flag=0;
int daq_TCP_init(void){
  
  // int working_socket;
  struct sockaddr_in my_address; 
  
  memset(&my_address, 0, sizeof(struct sockaddr_in));
  // address family is always AF_INET ...
  my_address.sin_family = AF_INET;
  // strcpy(my_address.sin_data, ADDRESS);
  // unlink(my_address.my_addr);
  my_address.sin_addr.s_addr = INADDR_ANY;
  my_address.sin_port = htons(6969);
  // getting a socket ...
  my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (my_socket < 0) {
    printf("could not create a socket ... bye\n");
    
    return 1;
  }
  else {
    printf("got a socket, will bind now ... \n");
  }
  
  int on = 1;
  int status = 0;
  status = setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));
  if (-1 == status)perror("setsockopt(...,SO_REUSEADDR,...)");
  
  int BINDret=0;
  
  if ((BINDret=bind(my_socket, (const struct sockaddr *)&my_address, sizeof(struct sockaddr_in)) )< 0){
    printf("BIND ref %i\n",BINDret);
    printf("could not bind to the port ... \n");
    close(my_socket);
    
    return 1;
  }
 

  if (listen(my_socket, 0) < 0) {
    printf("failed to listen ... \n");
    close(my_socket);
    
    return 1;
  }
 
  working_socket = accept(my_socket, NULL, NULL);
  if (working_socket < 0) {
    printf("failed to accept new connection ... \n");
    close(my_socket);
    return 1;
  }
  
  //  write(working_socket, "0", 32);

  return my_socket;
}


// Initialization of the ADC Boards CAEN V1724
int daq_v1724_init(unsigned int BaseAddr, int board_id)
{
  unsigned int address;
  unsigned int value;
  unsigned int Acquisition_window = daqpar.DAQ_time_window;
  unsigned int nSamples_Post      = daqpar.nPost;
  unsigned int Monitor_M_value    = daqpar.Monitor_Mode_value;
  int          ret  = 0;
  int          l, m = 0;
  
  //if(board_id==8)debug=true;
  //else debug=false;
  //if(debug)printf("daq_v1724_init() %lx \n",BaseAddr);
 
  // AMC FPGA Firmware Revsion x channel
  address = BaseAddr + AMC_FPGA_Firmware_Revsion; 
  for(m = 0; m < N_BOARD; m++){
    ret += CAENVME_ReadCycle(BHandle,address,&AMC_Firm,cvA32_U_DATA,cvD32);
  //  if(debug){
   //   printf("\n AMC_Firmware_X[%i] = %ix", m, AMC_Firm & 0xFF00);
   //   printf("\t AMC_Frimware_Y[%i] = %ix", m, AMC_Firm & 0x00FF);
  //  }
    address += 0x0100;
  }

  // ROC FPGA Firmware Revsion 
  address = BaseAddr + ROC_FPGA_Firmware_Revsion; 
  ret += CAENVME_ReadCycle(BHandle,address,&ROC_Firm,cvA32_U_DATA,cvD32);
  if(debug)printf("\nROC FPGA Firmware Revsion Ret = %d\n",ret);
  if(debug)printf("ROC_FPGA_Firmware_X = %ix\n",ROC_Firm & 0xFF00);
  if(debug)printf("ROC_FPGA_Firmware_Y = %ix\n\n",ROC_Firm & 0x00FF);
  
  // Software Reset Register 
  address = BaseAddr + SW_Reset_register; 
  value = 0x0;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  if(debug) printf("Software reset register  ret = %d \n",ret);
    
  // Front Panel I/O Control (Choose NIM pulse for trigger)
  address = BaseAddr + Front_Panel_IO_Control; 
  value = 0x0;
  ret  = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&FPctrl,cvA32_U_DATA,cvD32);
  if(debug) {
    printf("Front panel I/O Control DATA  ret = %d \n",ret);
    printf("Front panel I/O Control DATA = %ix \n",FPctrl & 0xF);
  }
  
  // Channel Configuration ( 0x8000 pag.53 rev.9 )
  address = BaseAddr + Channel_configuration; 
  value = 0x90; // Analog Monitor ON - TRG Overlap OFF
  ret  = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&CCctrl,cvA32_U_DATA,cvD32);
  if(debug) {
    printf("Channel Configuration ret = %d \n",ret);
    printf("Channel Configuration DATA = %ix \n",CCctrl & 0xFFFFF);
  }
  
  // Channel Enable Mask (0x8120 pag. 59 rev.9 )
  address = BaseAddr + Ch_Enable_Mask; 
  value = 0xFF; //ALL ch. enabled 
  ret  = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32); 
  ret += CAENVME_ReadCycle(BHandle,address,&MASKctrl,cvA32_U_DATA,cvD32);
  //printf("\nAnalog Monitor Read = %lx\n",MMctrl );
  if(debug) {
    printf("Channel Enable Mask Ret   = %d \n" ,ret);
    printf("Channel Enable Mask Value = %ix \n",MASKctrl & 0xFF);
  }
  
  // to Select Monitor Mode (Analogic oputput)
  address = BaseAddr + Monitor_Mode; 
  value = Monitor_M_value;
  ret  = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32); 
  ret += CAENVME_ReadCycle(BHandle,address,&MMctrl,cvA32_U_DATA,cvD32);
  if(debug) {
    printf("Monitor Mode Ret   = %d \n" ,ret);
    printf("Monitor Mode Value = %ix \n",MMctrl & 0xF);
  }
  
  if(Monitor_M_value==2){
    // Analog Monitor Settings (0x8150 pag. 60 rev.9 )
    address = BaseAddr + Analog_Monitor; 

    // All Inputs enabled
    value   = 0x000000FF;
    // Analog Inspection Inverter
    value   = value + ( daqpar.AnMonInv     * 0x1000000);
    // Magnify factor
    value   = value + ( daqpar.AnMonMAG     * 0x10000);
    // DAC Offset Voltage Sign
    if(daqpar.AnMonOffSign==0x1)value = value + ( 0x8 * 0x10000);
    // DAC Offset Voltage Value
    value   = value + ( daqpar.AnMonOffVal  * 0x100);
    
    ret     = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32); 
    ret    += CAENVME_ReadCycle(BHandle,address,&ANALOGctrl,cvA32_U_DATA,cvD32);
    if(debug) {
      printf("\nAnalog Monitor Mode Ret   = %d \n" ,ret);
      printf("Analog Monitor Mode Value = %ix \n",ANALOGctrl);
    }
  }

  if(Monitor_M_value==0){
    // Majority Mode settings for DAC and 
	int i = 0;    
	for(i = 0; i < daqpar.nchannel; i++) {
		address = BaseAddr + Threshold +i*0x0100; 
  		value = daqpar.MajMo_Threshold; 
		ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
		ret += CAENVME_ReadCycle(BHandle,address,&BUFORGctrl,cvA32_U_DATA,cvD32);

		if(debug) printf("Threshold  ret = %d \n",ret);
		
		address = BaseAddr + n_over_Threshold +i*0x0100; 
  		value = 0x0005; 
		ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
		ret += CAENVME_ReadCycle(BHandle,address,&BUFORGctrl,cvA32_U_DATA,cvD32);

		if(debug) printf("n_over_Threshold  ret = %d \n",ret);

	}
  }


  // Post Trigger Settings Register
  address = BaseAddr + Post_Trigger_Settings; 
  //value = 0x1B58;0xF0
  value = nSamples_Post;
  ret  = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32); 
  ret += CAENVME_ReadCycle(BHandle,address,&PTctrl,cvA32_U_DATA,cvD32);
  if(debug) {
    printf("Post trigger settings  ret = %d \n",ret);
    printf("Post trigger settings DATA = %ix \n",PTctrl & 0xFFFF);
  }
  
  // Buffer Organization  
  address = BaseAddr + Buffer_Organization; 
  value = Acquisition_window; // Block_size 1k - Samples/ch=512   
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&BUFORGctrl,cvA32_U_DATA,cvD32);
  if(debug)printf("\n Buffer Organization = %ix\n",BUFORGctrl & 0xF);

  // Trigger Source Enable Mask
  address = BaseAddr + Trigger_Source_Enable; 
  // Local Trigger can not generate global trigger, Software trigger disabled
  value = 0x40000000;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&TRGINctrl,cvA32_U_DATA,cvD32);
  if(debug)printf("\n Trigger IN Enable 0x810C  DATA = %ix\n",TRGINctrl);

  // Channel n ZS_THRESHOLD (0x1n24) pag. 51 manual revision 9
  address = BaseAddr + 0x1024; 
  value = 0x0000000F;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&TRGINctrl,cvA32_U_DATA,cvD32);
  if(debug)printf("\n Trigger IN Enable 0x810C  DATA = %ix\n",TRGINctrl);
  
  // Front Panel Trigger OUT Enable Mask
  address = BaseAddr + Trigger_OUT_Enable; 
  value = 0x40000000;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&TRGOUTctrl,cvA32_U_DATA,cvD32);
  if(debug)printf("\n Trigger OUT Enable 0x810C  DATA = %ix\n",TRGOUTctrl);

  
  // Channel n DAC Register (0x1n98) Channels offset
  address = BaseAddr + DAC_Level; 
  value = daqpar.DACLevel; //0xFFFF/2 = 7FFF - old 0xA000
  for(l = 0; l < N_BOARD; l++){
    ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
    ret += CAENVME_ReadCycle(BHandle,address,&DACctrl,cvA32_U_DATA,cvD32);
    if(debug)printf("\n DAC Level[%i] = %ix",l,DACctrl);
    address += 0x0100;
  }

  // Custom size (only for first 4 boards)
  address = BaseAddr + Custom_size; 
  value = 0;
  if( board_id < 8 )    value = (daqpar.nsamples/2);
  else value = 0;
  ret    += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret    += CAENVME_ReadCycle(BHandle,address,&SIZEctrl,cvA32_U_DATA,cvD32);
  if(debug)printf("\n Board_id= %i \t Custom size = %i", board_id, SIZEctrl);

  // BLT Event Number to be Transferred
  address = BaseAddr + BLT_Event_Number_Transferred; 
  value   = 0x1;
  ret    += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret    += CAENVME_ReadCycle(BHandle,address,&BLTctrl,cvA32_U_DATA,cvD32);
  if(debug)printf("\n BLT Event Number to be Transferred = %ix\n",BLTctrl);
  
  // Board ID (0xEF08 pag 61 rev. 9)
  /*address = BaseAddr + board_id; 
  value = board_id;
  ret  = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32); 
  ret += CAENVME_ReadCycle(BHandle,address,&BrdIDctrl,cvA32_U_DATA,cvD32);
  if(debug) {
    printf("Board ID Ret   = %d \n" ,ret);
    printf("Board ID Value = %lx \n",BrdIDctrl);
  }
  */
  
  // Downsampling factor (pag. 59 rev. 9)
  address = BaseAddr + Downsample         ; 
  value = daqpar.DownSample;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&DSctrl,cvA32_U_DATA,cvD32);
  if(debug)printf("\n Downsampling factor is = %i \n",DSctrl);
  
  // VME Control (0xEF00 pag 61 rev. 9)
  address = BaseAddr + VME_Control; 
  value = 0x10;
  ret  = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32); 
  ret += CAENVME_ReadCycle(BHandle,address,&VMEctrl,cvA32_U_DATA,cvD32);
  if(debug) {
    printf("VME Control Ret   = %d \n" ,ret);
    printf("VME Control Value = %ix \n",VMEctrl);
  }
  
  // Acquisition control 
  address = BaseAddr + Acquisition_control; 
  value = 0x14; //+0x10 for downsampling
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&RUNctrl,cvA32_U_DATA,cvD32);
  if(debug)printf("\n Acquisition Control (4) = %ix\n",RUNctrl);
  
  if( CAENVME_SetOutputRegister(BHandle, 0x0) != cvSuccess ) {
    printf("\n\n Error setting FIFO mode\n");
    return -1;
  }

  // MCST Base Address and Control (0xEF0C pag. 61 rev.9 )
  address = BaseAddr + MCST; 
  if(BaseAddr==fst_brd)value = 0x2BB;
  else if(board_id==(daqpar.board_number-1))value = 0x1BB;
  //  else if(BaseAddr==(fst_brd+(daqpar.board_number*0x00010000)) )value = 0x1BB;
  else value = 0x3BB;
  ret  = CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32); 
  ret += CAENVME_ReadCycle(BHandle,address,&MCSTctrl,cvA32_U_DATA,cvD32);
  if(debug) {
    printf("\nBase Address= %ix \n", BaseAddr);
    printf("MCST Base Address Ret   = %d \n" ,ret);
    printf("MCST Base Address Value = %ix \n",MCSTctrl);
  }
  
  return ret;
}

// Initialization of the ADC Boards v1729 2GS/s
/*
int daq_v1729_init(unsigned int BaseAddr)
{
  BaseAddr = 0xFF000000;
  
  unsigned int address;
  unsigned int value;
  unsigned int resp=0;
  int          ret  = 0;
  
  //Open output binary file for saving the photodiode signals
  char V1729_out[64] = "../diode/diode_data/PhD_00000000";
  sprintf(&V1729_out[24],"%.08u",daqpar.runnum); 
  // sprintf(&V1729_out[21],"_%.04u",read_ev-1); 
  sprintf(&V1729_out[32],"%.04s",".dat");
  phout = fopen(V1729_out, "w" );
  if(phout == NULL) return (void*)0;
  
  // Reset Board register
  address = BaseAddr + 0x0800; 
  value = 0x1;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Reset Board register ret= %i", ret );
  int position=0;
  // FPGA Version
  address = BaseAddr + 0x8200; 
  ret += CAENVME_ReadCycle(BHandle,address,&resp,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s FPGA Version register ret= %i /  value= %x", ret, resp&0xFF );

  // Pre Trigger LSB
  address = BaseAddr + 0x1800; 
  //value = 0x10; //External trigger
  ret += CAENVME_ReadCycle(BHandle,address,&resp,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Pre Trigger LSB register ret= %i /  value= %i", ret, resp&0xFF );

  // Pre Trigger MSB
  address = BaseAddr + 0x1900; 
  //value = 0x10; //External trigger
  ret += CAENVME_ReadCycle(BHandle,address,&resp,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Pre Trigger MSB register ret= %i /  value= %i", ret, resp&0xFF );

  // Post Trigger LSB
  address = BaseAddr + 0x1A00; 
  //value = 0x10; //External trigger
  ret += CAENVME_ReadCycle(BHandle,address,&resp,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Post Trigger LSB register ret= %i /  value= %i", ret, resp&0xFF );

  // Post Trigger MSB
  address = BaseAddr + 0x1B00; 
  //value = 0x10; //External trigger
  ret += CAENVME_ReadCycle(BHandle,address,&resp,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Post Trigger MSB register ret= %i /  value= %i", ret, resp&0xFF );
  
  // Trigger type register
  address = BaseAddr + 0x1D00; 
  value = 0x16; //External trigger
  resp  = 0;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&resp,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Trigger type register ret= %i // resp=%x", ret, resp&0x1F );
  
  // Channel Mask register
  address = BaseAddr + 0x2300; 
  value = 0xF; //0xF 4ch, 0x1 1 ch, 0x3 2ch, 0x7 3ch
  resp  = 0;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  ret += CAENVME_ReadCycle(BHandle,address,&resp,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Channel Mask register ret= %i // resp=%x", ret, resp&0xF);

  // FP Frequency
  address = BaseAddr + 0x8100; 
  ret += CAENVME_ReadCycle(BHandle,address,&resp,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s FP Frequency register ret= %i /  value= %x", ret, resp&0x3 );
  
  // Start Acquisition register
  address = BaseAddr + 0x1700; 
  value = 0x0;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Start Acquisition register ret= %i", ret );

  
  // Interrupt register
 // address = BaseAddr + 0x8000; 
  //value = 0x10; //External trigger
//  ret += CAENVME_ReadCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
//  if(debug)printf("\n V1729 2GS/s Interrupt register ret= %i /  value= %x", ret,value&0x1 );
  
  
  return 0;
}
*/
// Initialization of all the VME boards 
int daq_vme_init(void) {

  unsigned int address;
  // unsigned int lst_brd;
  int ret, i;
  
  // Allocate memory of size "blt_size"
  //blt_buffer = malloc(blt_size);
  
  // Init Controller
  ret = daq_v2718_init();
  if(ret != 0) return ret;
  
  // Init all CAEN 100MS/s ADC V1724 boards
  for(i = 0; i < daqpar.board_number; i++){
    
    address = fst_brd + ( 0x00010000 * i );
    //   if(i>10)debug=true;
    ret = daq_v1724_init(address, i);
    //lst_brd = address;
    //debug=false;
    //printf("\nBase Address= %lx \tRet= %i \n", address, ret);
    if(ret != 0) return ret;
  }

  TCP_connection = (bool)daqpar.TCP_connection;

  // Init v1729 board
//  if(!DIODE)daq_v1729_init(0xFF000000); // cchsu

  // Init TCP Connection
  // if(!TCP_connection)daq_TCP_init();

 
  return ret;
}

int k = 0;
inline int daq_v1724_MBLT(unsigned int *dddd) { 
  
  unsigned int address;
  int write_pointer      = 0;
  int count;
  int ret;
  
  // Read CBLT  
  address = CBLT_add;
  ret = CAENVME_FIFOBLTReadCycle(BHandle,address,&dddd[write_pointer],event_size*4,cvA32_U_BLT,cvD32,&count);
  //ret = CAENVME_FIFOMBLTReadCycle(BHandle,address,&dddd[write_pointer],event_size*4,cvA32_U_MBLT,&count);
 
  /*  if( (ret == 0) && (dddd[0]==0xbb000c00)){
    //ret++;
    printf("Something wrong in DAQ\n");
    }*/
  
  int f=0;
  int g=0;
  int l=0;
  
  if(ret==0 && debug){
    printf("Add= %ix\n",address);
    printf("\nCBLT ret = %d ", ret); 
    for(f = 0; f<16; f++){
      for(l=0; l<1; l++)printf("\nEvent = %i \t board = %i dddwr[0]=%ix ", k, f, dddd[l] );
      g+= 2052;
    }
    printf("\n");
    k++;
  }
  // debug = false;
  
  return ret;
}

// V1729 2GS/s ADC board
/*
int daq_v1729_MBLT() { 
  
  unsigned int BaseAddr      = 0xFF000000;
  unsigned int address;
  int          ret           = 0;
  unsigned short value       = 0;
  unsigned short trg_time    = 0;
  unsigned int   event_size  = 2564;
  unsigned short DATA[10256] = {0};
  unsigned int   count;
 
  // if(phout == NULL) return (void*)0;
  
  // Interrupt register
  address = BaseAddr + 0x8000; 
  
 interrupt_loop:
  ret = 0;
  ret = CAENVME_ReadCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  //printf("\nret= %i", ret);
  if( (value&0x1) == 0 ) goto interrupt_loop;
  //printf("\n V1729 2GS/s Interrupt register ret= %i /  value= %x", ret,value&0x1 );
  
  // Trigger REC register (Trigger position in time) to re-order the buffer
  address = BaseAddr + 0x2000; 
  //value = 0x10; //External trigger
  ret = CAENVME_ReadCycle(BHandle,address,&trg_time,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Trigger time register ret= %i /  value= %i", ret,trg_time&0xFF );
  //fprintf(phout, "\n%i\t%i\t%i\n", daqpar.runnum, read_ev, trg_time&0xFF); 
  fwrite(&daqpar.runnum, 2, 1, phout);
  fwrite(&read_ev,       2, 1, phout);
  fwrite(&trg_time,      2, 1, phout);

  // RAM Data register  
  // Read V1729 2GS/s RAM Data 
  address = BaseAddr + 0x0D00;
  int j = 0;
  ret   = CAENVME_FIFOBLTReadCycle(BHandle,address,&DATA[j],event_size*2*4,cvA32_U_BLT,cvD32,&count);
  //4ch ret   = CAENVME_FIFOBLTReadCycle(BHandle,address,&DATA[j],event_size*4*2,cvA32_U_BLT,cvD32,&count);
  if(debug)printf("2GS/s V1729 reading access ret=%u \n", ret);
    
  // Write data to a txt file
  unsigned short p = 0;
  int l = 0;
  //fwrite(&rh, rh.header_size, 1, phout);
  for(p=0; p<event_size; p++){
    l = p*4; 
    // fprintf(phout, "%i\t%i\n", p, DATA[l]);//1 channel
    // fprintf(phout, "%i\t%i\t%i\t%i\t%i\n", p, DATA[l], DATA[l+1], DATA[l+2], DATA[l+3]); //4channel
    // fwrite(&p,        2, 1, phout);
    fwrite(&DATA[l],  2, 1, phout);
    fwrite(&DATA[l+1],  2, 1, phout);
    fwrite(&DATA[l+2],  2, 1, phout);
    fwrite(&DATA[l+3],  2, 1, phout);
    
    //if(p<10)printf("DATA[%i]= %i\n", l, DATA[l]);
  }
  
  // Start Acquisition register
  address = BaseAddr + 0x1700; 
  value = 0x0;
  ret += CAENVME_WriteCycle(BHandle,address,&value,cvA32_U_DATA,cvD32);
  if(debug)printf("\n V1729 2GS/s Start Acquisition register ret= %i", ret );

  return ret;
}
*/
char buffer_old[32];
int conto = 0;
int daq_TCP_read(){

  char buffer[32];
  
  //bzero(buffer,sizeof(buffer));
 read_flag:
  // write(working_socket, "1", 32); 
  usleep(10);
  read(working_socket, buffer, 32);
  
  //int idem = strcmp(buffer_old, buffer); 
  //printf("TCP Buffer= %s\t idem= %i\n", buffer, idem);
  //if(idem==0)goto read_flag;
  conto++;
  //  if(!(conto%10))
  //  printf("TCP Buffer buoni %s\t conto=%i \n", buffer, conto);
  
  eh.laser_energy = (float) atof(buffer);
  printf("eh.laser_energy= %f \n", eh.laser_energy);
  //bzero(buffer,sizeof(buffer)); 
  // write(working_socket, "0", 32);

  strcpy(buffer_old,buffer);
  
  if (strlen(buffer) < 3) {
    printf("there was no data ... \n");
  }
    

  return 0;
}

int posLaserRead (void){
   ev_laser *ptrStrLaser;
   long long HorizontalCounts;
   long long LinearCounts; 
   float LinearPosDeg;
   float RotaryPosDeg;
   ptrStrLaser = &laser;
   readPos(ptrStrLaser);

   HorizontalCounts = (laser.PosHor-laser.PosRefHor)>>12;
   RotaryPosDeg = (float) HorizontalCounts*360/20000;

   LinearCounts = (float) laser.PosVert;
   LinearPosDeg = (LinearCounts -7728478.0)/69994.7111;
   
   

   
   // Write position in openscad file for live monitoring
   FILE * pFile;
   pFile = fopen ("../laser.scad","w");
   if (pFile!=NULL)
   {
      fprintf(pFile,"include <TPC.scad>\nlaser(%f,%f);\n",RotaryPosDeg,LinearPosDeg);
      fclose (pFile);
         printf("file\n");
   }

   printf("+ Horizontal Pos: ");
   printf("Trigger Counter: %05u, Status: 0x%04X, Timestamp: %u, Position [deg]: %f\n",laser.TriggerStampHor,laser.StatusHor,laser.TimestampHor,RotaryPosDeg);
   printf("+ Vertical Pos:   ");
   printf("Trigger Counter: %05u, Status: 0x%04X, Timestamp: %u, Position [deg]: %f\n",laser.TriggerStampVert,laser.StatusVert,laser.TimestampVert,LinearPosDeg);
   return 1;
}

/******************** PRODUCER ******************/ 
void * producer(void* bb) {
  
  unsigned int  *data = bb;
  LASER = (bool)daqpar.Laser;
  TCP_connection = (bool)daqpar.TCP_connection;

  
#ifdef LHEP_USE_SHM_DATA
  unsigned short *shm_w;
  int             rc;
#endif
  // unsigned int   Address = fst_brd;
  int             waddr   = 0;
  time_t          ev_time;
  //int             Ret     = 0;
  int             ret     = 0;
  //global_scaler = 0;
  
  do {
    //Check for Bus Error reading memory buffer of the 1st board
    /*Ret = 0;
    Ret = CAENVME_ReadCycle(BHandle,Address,&data[waddr],cvA32_U_DATA,cvD32);
    //printf("\nData Ready ret = %d ", Ret); 
    if( Ret != 0 ) continue;
    */
    if( !sem_wait(&emptyBuffers) ) {
      
      // emptyBuf--;
      //  if(emptyBuf<1)printf("\nPro EmptyBuf=%i", emptyBuf);
      
      if(forced_stop == true) break;
      //if(debug)printf("\nproducer called with waddr = %d \n",waddr);
      
      written_ev++;
      //if(debug)printf("\n written event = %d \n", written_ev );

      time(&ev_time);
      eh.event_number = written_ev;
      eh.event_time   = (unsigned int) ev_time;
      
    read_loop:
      ret = 0;
      ret = daq_v1724_MBLT(&data[waddr]);
      
      if( ret != 0 ) goto read_loop;
      //printf(" Data= %lx\n", data[waddr] );
      if(!(written_ev%1))
      {
         printf("--------------------------------------------------------------------------------------------------------\n");         
         printf("written event = %d \n",written_ev);
         if(!LASER) posLaserRead();
      }
      //Read photodiode
     // if(!DIODE)daq_v1729_MBLT(); // cchsu
 
      //Read TCP Connection buffer
      //if(!TCP_connection)daq_TCP_read();
            
      sem_post(&fullBuffers);
      
      waddr += event_size;
      if(waddr == buffer_size) waddr = 0;
      //if(debug)printf("  write addr= %d       write ev = %d \n",waddr,written_ev);
      
      if(written_ev==daqpar.nevents)break;

    }
    else{
      lost_ev++;
    }

#ifdef LHEP_USE_SHM_DATA
      
    operation[0].sem_num = 0; /* Operate on the sem     */
    operation[0].sem_op =  0; /* Check for zero         */
    operation[0].sem_flg = IPC_NOWAIT;

    operation[1].sem_num = 0; /* Operate on the sem     */
    operation[1].sem_op =  1; /* Increment              */
    operation[1].sem_flg = IPC_NOWAIT;
    
    rc = semop( shmdat_semid, operation, 2 );
    if (rc >= 0) {
      if(debug) printf("rc OK for event %d\n",read_ev);       
      
      shm_w     = ((unsigned short*) shmdat_address);
      shm_w[ 0] = written_ev;  
      shm_w[ 1] = daqpar.nevents;
      shm_w[ 2] = lost_ev;
      
      // Shared memory variables to display VME settings with GUI 
      shm_w[ 3] = AMC_Firm; 
      shm_w[ 4] = ROC_Firm; 
      shm_w[ 5] = FPctrl;
      shm_w[ 6] = CCctrl;
      shm_w[ 7] = MASKctrl;
      shm_w[ 8] = ANALOGctrl;
      shm_w[ 9] = MMctrl; 
      shm_w[10] = PTctrl; 
      shm_w[11] = BUFORGctrl;
      shm_w[12] = TRGINctrl;
      shm_w[13] = TRGOUTctrl;
      shm_w[14] = DACctrl; 
      shm_w[15] = BLTctrl; 
      shm_w[16] = BrdIDctrl;
      shm_w[17] = DSctrl;
      shm_w[18] = VMEctrl;
      shm_w[19] = MCSTctrl; 
      shm_w[20] = RUNctrl; 
 
      if(debug){
	printf("\nshm_w[0] = %d"  , shm_w[0] );
	printf("\tshm_w[1] = %d\t", shm_w[1] );
	printf(  "shm_w[2] = %d\n", shm_w[2] );
	printf(  "shm_w[3] = %d\t", shm_w[3] );
	printf(  "shm_w[4] = %d\t", shm_w[4] ); 
	printf(  "shm_w[5] = %d\n", shm_w[5] ); 
	printf(  "shm_w[6] = %d\t", shm_w[6] ); 
	printf(  "shm_w[7] = %d\n", shm_w[7] ); 
	printf(  "shm_w[8] = %d\t", shm_w[8] ); 
	printf(  "shm_w[9] = %d\n", shm_w[9] ); 
      }

      operation[0].sem_num = 0; /* Operate on the sem     */
      operation[0].sem_op = -1; /* Decrement the semval by one   */
      operation[0].sem_flg = IPC_NOWAIT;
      
      rc = semop( shmdat_semid, operation, 1 );
      //   if (rc < 0)printf("rc<0 semop at the end of the daq_vme loop");
    }
#endif
    
  } while (forced_stop == false);
  
  if(debug)printf("OUT of DAQ loop");
  
  return (void*)0;

}

void daq_vme_end(void) {
  
  CAENVME_End(BHandle);
}
