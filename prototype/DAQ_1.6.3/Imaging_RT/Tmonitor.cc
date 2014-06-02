#if defined(__CINT__) && !defined(__MAKECINT__)
#include "./libGUItest.so"
#else
#include "GUItest.h"
#endif

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

// ROOT interface
#include "TApplication.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TThread.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TGFontDialog.h"
#include "TGFrame.h"
#include "TColor.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TText.h"

// User Includes
#include "../daq_main.h"
using namespace std;

MyMainFrame     *Frame;
TThread         *th1;
int MyMainFrame::n_histo   =     1;
int MyMainFrame::event     =     0;
int MyMainFrame::I_board   =     0;   
int MyMainFrame::C_board   =     0;   
int              SAMPLES   =     0;
int              arg2;
int              downS;
const int        bit14     = 16383;
const int        ped_ratio =     5;
//#define PEDESTAL_SUB   0 // Pedestal subtraction (only for display)

// MLuethi: These values just set where to split the wire view!
int temp_channel_num = 32;   // cchsu   note that these two values have to be changed if the num of boards change -> should be implemented later automatically 
int temp_channel_num_plus1 = 33;  // cchsu 
#define LHEP_SHMBUF_SEM_KEY 1940345
#define LHEP_SHMBUF_SHM_KEY 4032913

// #define SHM_DATA_SIZE 1000
// #define LHEP_SHMDAT_SEM_KEY 1940346
// #define LHEP_SHMDAT_SHM_KEY 4032914

bool debug = false;
void* myth(void*) {

  const int Board_nr       = arg2;
  
  int Idraw_wire  = MyMainFrame::I_board = 0;
//  int Cdraw_wire  = MyMainFrame::C_board = 8; //cchsu 
  int Cdraw_wire  = MyMainFrame::C_board = 4;
  
  TThread::Printf("Start of myth\n");

  key_t shmbuf_sem_key, shmbuf_shm_key;
  //  key_t shmdat_sem_key, shmdat_shm_key;

  int shmbuf_semid=-1, shmbuf_shmid=-1;
  //  int shmdat_semid=-1, shmdat_shmid=-1;

  void* shmbuf_address;
  //   void* shmdat_address;

  struct sembuf operation[2];

  int shmbuf_raddr=0;
  unsigned short* shmbuf_r;
  //   unsigned short* shmdat_r=NULL;
  int rc;

  int read_ev = 0;
  //   bool forced_stop = false;

  /* keys for shared memory and semaphores */

  shmbuf_sem_key = (key_t) LHEP_SHMBUF_SEM_KEY;
  shmbuf_shm_key = (key_t) LHEP_SHMBUF_SHM_KEY;

  //   shmdat_sem_key = (key_t) LHEP_SHMDAT_SEM_KEY;
  //   shmdat_shm_key = (key_t) LHEP_SHMDAT_SHM_KEY;
  
  printf("\n getting shmbuf semaphore...");
 retry_bsem:
  shmbuf_semid = semget( shmbuf_sem_key, 2, 0666);
  if(debug)printf("\n TMONITOR shmbuf semID= %d",shmbuf_semid);
  if(shmbuf_semid > 0 ) goto ok_bsem;
  usleep(200000);
  goto retry_bsem;
 ok_bsem:
  printf("  OK\n");
  
  printf("getting shmbuf shared memory segment..");
 retry_bshm:
  shmbuf_shmid = shmget(shmbuf_shm_key, 0, 0666);
  if(debug)printf("\n TMONITOR shmbuf shmID= %d",shmbuf_shmid);
  if(shmbuf_shmid > 0) goto ok_bshm;
  usleep(200000);
  goto  retry_bshm;
 ok_bshm:
  printf("  OK\n");

  //   printf("TMONITOR getting shmdat semaphore...");
  //  retry_dsem:
  //   shmdat_semid = semget( shmdat_sem_key, 1, 0444);
  //   if(debug)printf("\n TMONITOR shmdat semID= %d",shmdat_semid);
  //   if(shmdat_semid > 0 ) goto ok_dsem;
  //   usleep(200000);
  //   goto retry_dsem;
  //  ok_dsem:
  //   printf("OK\n");
  
  //   printf("TMONITOR getting shmdat shared memory segment...");
  //  retry_dshm:
  //   shmdat_shmid = shmget(shmdat_shm_key, SHM_DATA_SIZE, 0444);
  //   ùif(debug)printf("\n TMONITOR shmdat shmID= %d",shmdat_shmid);
  //    if(shmdat_shmid > 0) goto ok_dshm;
  //   usleep(200000);
  //   goto  retry_dshm;
  //  ok_dshm:
  //   printf("OK\n");

  /* Attach the shared memory segment to the server process.       */
  shmbuf_address = shmat(shmbuf_shmid, NULL, SHM_RDONLY);
  if(debug)if( shmbuf_address==NULL ) {
    printf("main:TMONITOR  shmat() failed\n");
  }

  //  shmdat_address = shmat(shmdat_shmid, NULL, SHM_RDONLY);
  //  if ( shmdat_address==NULL ) {
  //   printf("main: TMONITOR shmat() failed\n");
  //  }
     
  shmbuf_r = ((unsigned short*) shmbuf_address);
  //   shmdat_r = ((unsigned short*) shmdat_address);
   
  const float conv_fac = 0.152588; // mV per ADC count - 14 bit 
  // Defining binning and gaps of the histos
  Int_t xbin     = SAMPLES;
  Int_t xmin     =       0;
  Int_t xmax     = SAMPLES+1;
  Int_t ybin     = Board_nr * 4;
  //ybin     =64;
  Int_t wire_min =       1;
  Int_t wire_max = Board_nr * 4;
  //wire_max = 64;
  Int_t baseline =       0; 
  
  // Variables for Titles of the histos
  char Xaxis[64];
  char IND[64];
  char COL[64];
  char TITLE_WI[128], WIRE_I[64];
  char TITLE_WC[128], WIRE_C[64];
  char WIRE_I0[64], WIRE_C0[64];
  char WIRE_I_name[64], WIRE_C_name[64];
  //char WIRE_C4[64], WIRE_C5[64], WIRE_C6[64], WIRE_C7[64];
  char PEDESTAL[64];
  // Global view 2D histo setting
  sprintf(IND,      "Global I view");
  sprintf(COL,      "Global C view");
  // Axis definition 
  sprintf(TITLE_WI, "I Wire 0 -7 Event number 0");
  sprintf(WIRE_I,  "Wire I  Axis");
  sprintf(TITLE_WC, "C Wire 0 -7 Event number 0");
  sprintf(WIRE_C,  "Wire C Axis");
  // Wire histos name setting
  /* sprintf(WIRE_C0,  "Wire nr. 0");
  sprintf(WIRE_C1,  "Wire nr. 1");
  sprintf(WIRE_C2,  "Wire nr. 2");
  sprintf(WIRE_C3,  "Wire nr. 3");
  sprintf(WIRE_C4,  "Wire nr. 4");
  sprintf(WIRE_C5,  "Wire nr. 5");
  sprintf(WIRE_C6,  "Wire nr. 6");
  sprintf(WIRE_C7,  "Wire nr. 7");
  */
  sprintf(PEDESTAL, "Pedestal");
  
  // old: const int wiresize = 64;
  const int wiresize = 32;
  // Histos Declarations
  TH2F   *Collection = new TH2F("v2I",IND,xbin,xmin,xmax,ybin,wire_min,wire_max);
  TH2F   *Induction  = new TH2F("v2C",COL,xbin,xmin,xmax,ybin,wire_min,wire_max);
  TH1F   *w_I      = 0;
  TH1F   *w_C      = 0;
  TH1F   *w_c[wiresize] = {0};
  TH1F   *w_i[wiresize] = {0};
  /*  TH1F   *w_c1     = 0;
  TH1F   *w_c2     = 0;
  TH1F   *w_c3     = 0;     
  TH1F   *w_c4     = 0;
  TH1F   *w_c5     = 0;
  TH1F   *w_c6     = 0;
  TH1F   *w_c7     = 0;     
  */
  TH1F   *pedestal = 0;
  TGaxis *iC1       = 0;
  TGaxis *iY1       = 0;
  TGaxis *C1       = 0;
  TGaxis *Y1       = 0;

  w_I  = new TH1F(TITLE_WI, WIRE_I, xbin,xmin,xmax);
  w_C  = new TH1F(TITLE_WC, WIRE_C, xbin,xmin,xmax);
  for(Int_t p=0;p<wiresize;p++){
    sprintf(WIRE_I_name,  "I Wire nr. %i", p);
    sprintf(WIRE_C_name,  "C Wire nr. %i", p);
    sprintf(WIRE_I0,  "I Wire nr. %i", p);
    sprintf(WIRE_C0,  "C Wire nr. %i", p);
    w_i[p] = new TH1F(WIRE_I_name,WIRE_I0,xbin,xmin,xmax);
    w_c[p] = new TH1F(WIRE_C_name,WIRE_C0,xbin,xmin,xmax);
    w_i[p]->SetStats(kFALSE);
    w_c[p]->SetStats(kFALSE);
  }
  /*
    w_c1 = new TH1F("wire 1",WIRE_C1,xbin,xmin,xmax);
    w_c2 = new TH1F("wire 2",WIRE_C2,xbin,xmin,xmax);
    w_c3 = new TH1F("wire 3",WIRE_C3,xbin,xmin,xmax);
    w_c4 = new TH1F("wire 4",WIRE_C4,xbin,xmin,xmax);
    w_c5 = new TH1F("wire 5",WIRE_C5,xbin,xmin,xmax);
    w_c6 = new TH1F("wire 6",WIRE_C6,xbin,xmin,xmax);
    w_c7 = new TH1F("wire 7",WIRE_C7,xbin,xmin,xmax);
  
  w_c->SetStats(kFALSE);
  w_c0->SetStats(kFALSE);
  w_c1->SetStats(kFALSE);
  w_c2->SetStats(kFALSE);
  w_c3->SetStats(kFALSE);
  w_c4->SetStats(kFALSE);
  w_c5->SetStats(kFALSE);
  w_c6->SetStats(kFALSE);
  w_c7->SetStats(kFALSE);
  */
  
  w_I->SetTitle(TITLE_WC);
  w_I->GetXaxis()->SetTitleColor( 4 );
  w_I->GetXaxis()->SetLabelColor( 2 );
  w_I->GetXaxis()->SetLabelSize(0.04);
  sprintf(Xaxis,  "drift time [samples (%i ns)]", downS);
  w_I->GetXaxis()->SetTitle( Xaxis );
 
  w_I->GetYaxis()->SetTitleColor( 4 );
  w_I->GetYaxis()->SetLabelColor( 2 );
  w_I->GetYaxis()->SetLabelSize(0.04);
  w_I->GetYaxis()->SetTitleOffset(0.9);
  w_I->GetYaxis()->SetTitle("Amplitude [ADC counts]");

  w_C->SetTitle(TITLE_WI);
  w_C->GetXaxis()->SetTitleColor( 4 );
  w_C->GetXaxis()->SetLabelColor( 2 );
  w_C->GetXaxis()->SetLabelSize(0.04);
  sprintf(Xaxis,  "drift time [samples (%i ns)]", downS);
  w_C->GetXaxis()->SetTitle( Xaxis );
 
  w_C->GetYaxis()->SetTitleColor( 4 );
  w_C->GetYaxis()->SetLabelColor( 2 );
  w_C->GetYaxis()->SetLabelSize(0.04);
  w_C->GetYaxis()->SetTitleOffset(0.9);
  w_C->GetYaxis()->SetTitle("Amplitude [ADC counts]");
  
  Induction->SetMaximum( 480);
  Induction->SetMinimum(-240);
  Induction->SetStats(kFALSE);
  Induction->GetYaxis()->SetTitle("#Wire");
  Induction->GetYaxis()->SetTitleColor( 4 );
  Induction->GetYaxis()->SetLabelColor( 2 );
  Induction->GetYaxis()->SetLabelSize(0.04);
  
  Collection->SetMaximum( 480);
  Collection->SetMinimum(-240);
  Collection->SetStats(kFALSE);
  Collection->GetYaxis()->SetTitle("#Wire");
  Collection->GetYaxis()->SetTitleColor( 4 );
  Collection->GetYaxis()->SetLabelColor( 2 );
  Collection->GetYaxis()->SetLabelSize(0.04);
  
  Induction->GetXaxis()->SetTitle( Xaxis );
  Induction->GetXaxis()->SetTitleColor( 4 );
  Induction->GetXaxis()->SetLabelColor( 2 );
  Induction->GetXaxis()->SetLabelSize(0.04);

  TF1 *ifx = new TF1("ifx","x",0,(SAMPLES*downS)/1000);
  iC1      = new TGaxis(0, wire_max,SAMPLES, wire_max, "ifx", 510, "-");
  iC1->SetTitle("time [#mus]");
  iC1->SetTitleColor( 4 );
  iC1->SetLabelColor( 2 );
  iC1->SetLabelOffset(-0.01);
  iC1->SetLabelSize(0.04);
  
  TF1 *ify = new TF1("ify","x", 0, (wire_max*3)/10);
  iY1      = new TGaxis(SAMPLES,0,SAMPLES, wire_max, "ify", 510, "+");
  iY1->SetTitle("X Coordinate [cm]");
  iY1->SetTitleColor( 4 );
  iY1->SetLabelColor( 2 );
  iY1->SetLabelOffset(0.03);
  iY1->SetLabelSize(0.04);

  Collection->GetXaxis()->SetTitle( Xaxis );
  Collection->GetXaxis()->SetTitleColor( 4 );
  Collection->GetXaxis()->SetLabelColor( 2 );
  Collection->GetXaxis()->SetLabelSize(0.04);

  //gStyle->SetPalette(255, (Int_t*)0);
  //gStyle->SetOptStat(0000);


    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);


  Int_t     iaddr, wire_start, wire_end;
  UInt_t    weightC;
  Double_t  Ped_Mean[N_BOARD*N_CHANNEL]={0};

  shmbuf_raddr=0;
  
  TF1 *Cfx = new TF1("Cfx","x",0,(SAMPLES*downS)/1000);
  C1      = new TGaxis(0, wire_max,SAMPLES, wire_max, "Cfx", 510, "-");
  C1->SetTitle("time [#mus]");
  C1->SetTitleColor( 4 );
  C1->SetLabelColor( 2 );
  C1->SetLabelOffset(-0.01);
  C1->SetLabelSize(0.04);
  
  TF1 *Cfy = new TF1("Cfy","x", 0, (wire_max*3)/10);
  Y1      = new TGaxis(SAMPLES,0,SAMPLES, wire_max, "Cfy", 510, "+");
  Y1->SetTitle("Y Coordinate [cm]");
  Y1->SetTitleColor( 4 );
  Y1->SetLabelColor( 2 );
  Y1->SetLabelOffset(0.03);
  Y1->SetLabelSize(0.04);
  

  
  // Add Induction and Collection text to the T2HI Histo
  /* TLine *col_l = new TLine(-45, 65, -45, 128);
  col_l->SetLineColor(9);
  col_l->SetLineStyle(2);
  TText *col   = new TText(-50, 80, "Collection");
  col->SetTextSize(0.04);
  col->SetTextFont(72);
  col->SetTextColor(9);
  col->SetTextAngle(90);

  TLine *ind_l = new TLine(-45, 1, -45, 64);
  ind_l->SetLineColor(8);
  ind_l->SetLineStyle(2);
  TText *ind   = new TText(-50, 20, "Induction");
  ind->SetTextSize(0.04);
  ind->SetTextFont(72);
  ind->SetTextColor(8);
  ind->SetTextAngle(90);
  */
  for(;;)
    {      
      operation[0].sem_num = 1; /* Operate on the full sem     */
      operation[0].sem_op = -1; /* Decrement the semval by one   */
      operation[0].sem_flg = 0;
    
      rc = semop( shmbuf_semid, operation, 1 );
      if (rc < 0) 
	{
	  printf("monitor: rc = %d for event %d\n",rc,read_ev); 
	  continue;
	}
      
      Induction->Reset();
      Collection->Reset();
      
      if(debug)printf(" \n TMONITOR monitor: rc OK for event %d\n",read_ev);
      
      iaddr=0;
      
      TThread::Lock();       
      
      // Get from GUI the wires to show 
      Idraw_wire  = MyMainFrame::I_board;
      Cdraw_wire  = MyMainFrame::C_board;
      //  cout << " Idraw_wire= " << Idraw_wire<< endl;
      //cout << "       Cdraw_wire= " << Cdraw_wire<< endl;

      // Print event number
      UInt_t ev_nr = 0;
      ev_nr = shmbuf_r[shmbuf_raddr+4];
      
      cout << "Tmonitor event number = " << ev_nr << endl;
      
      sprintf(TITLE_WI, "I Board nr %i - Event number %i", Idraw_wire, (int)ev_nr);
      w_I->SetTitle(TITLE_WI);
      sprintf(IND,"Global I view (2 wire planes) - Event number %i", (int)ev_nr);
      Induction->SetTitle(IND);
      sprintf(TITLE_WC, "C Board nr %i - Event number %i", Cdraw_wire, (int)ev_nr);
      w_C->SetTitle(TITLE_WC);
      sprintf(COL,"Global C view (2 wire planes) - Event number %i", (int)ev_nr);
      Collection->SetTitle(COL);
      
      //Loop over all boards
      for(int board=0; board < Board_nr; board++) {      
	//	wire_start =  1;                      // wire start 1, 9,17,25
	wire_start =  (board*8)+1;                      // wire start 1, 9,17,25
	wire_end   = ((board+1)*8)+1;                   // wire stop  8,16,24,32
	
	// Loop over all wires
	for(int iwire = wire_start; iwire<wire_end; iwire++) {
	  //cout << "  wire = " << iwire << endl; 
	  // Definition of Pedestal Histo	  
	  if((PEDESTAL_SUB)&&(read_ev%ped_ratio))pedestal = new TH1F("Pedestal",PEDESTAL,bit14,0,bit14+1);
	  
	  //Loop over all the samples
	  for(int isample = 0; isample<SAMPLES; isample++) {  
	    weightC = (shmbuf_r[shmbuf_raddr+iaddr+HEADER+(board*HEADER)] & 0x3FFF);
	    //cout << shmbuf_raddr+iaddr+HEADER+(board*HEADER) << endl;
	    //if(isample<=5)cout<<"weightC= "<<weightC<<endl;
	    iaddr++;
	    if((PEDESTAL_SUB)&&(read_ev%ped_ratio))pedestal->Fill( weightC );
	    
	    // Filling wires
	    //if you want to plot all 64 wires in the analogic TH1F histo monitor  
// cchsu	    if(iwire<65)w_i[iwire-1]->SetBinContent( isample,    weightC - Ped_Mean[iwire-1] );
// cchsu	    if(iwire>64)w_c[iwire-65]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    // This is plotting the lower windows
	    if(iwire< temp_channel_num_plus1)w_i[iwire-1]->SetBinContent( isample,    weightC - Ped_Mean[iwire-1] );
	    if(iwire> temp_channel_num )w_c[iwire-temp_channel_num_plus1]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	
	    
	    /*if(iwire == ( (Idraw_wire*8) + 1) )w_i[0]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
 	      if(iwire == ( (Idraw_wire*8) + 2) )w_i[1]->SetBinContent( isample,  -1*(weightC - Ped_Mean[iwire-1]) );
 	      if(iwire == ( (Idraw_wire*8) + 3) )w_i[2]->SetBinContent( isample,  -1*(weightC - Ped_Mean[iwire-1]) );
 	      if(iwire == ( (Idraw_wire*8) + 4) )w_i[3]->SetBinContent( isample,  -1*(weightC - Ped_Mean[iwire-1]) );
 	      if(iwire == ( (Idraw_wire*8) + 5) )w_i[4]->SetBinContent( isample,  -1*(weightC - Ped_Mean[iwire-1]) );
 	      if(iwire == ( (Idraw_wire*8) + 6) )w_i[5]->SetBinContent( isample,  -1*(weightC - Ped_Mean[iwire-1]) );
 	      if(iwire == ( (Idraw_wire*8) + 7) )w_i[6]->SetBinContent( isample,  -1*(weightC - Ped_Mean[iwire-1]) );
 	      if(iwire == ( (Idraw_wire*8) + 8) )w_i[7]->SetBinContent( isample,  -1*(weightC - Ped_Mean[iwire-1]) );
	    */	    
	    /*if(iwire == ( (Idraw_wire*8) + 1) )w_i[0]->SetBinContent( isample,  weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Idraw_wire*8) + 2) )w_i[1]->SetBinContent( isample,  weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Idraw_wire*8) + 3) )w_i[2]->SetBinContent( isample,  weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Idraw_wire*8) + 4) )w_i[3]->SetBinContent( isample,  weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Idraw_wire*8) + 5) )w_i[4]->SetBinContent( isample,  weightC - Ped_Mean[iwire-1] ); 
	    if(iwire == ( (Idraw_wire*8) + 6) )w_i[5]->SetBinContent( isample,  weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Idraw_wire*8) + 7) )w_i[6]->SetBinContent( isample,  weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Idraw_wire*8) + 8) )w_i[7]->SetBinContent( isample,  weightC - Ped_Mean[iwire-1] );
	    
	    if(iwire == ( (Cdraw_wire*8) + 1) )w_c[0]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Cdraw_wire*8) + 2) )w_c[1]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Cdraw_wire*8) + 3) )w_c[2]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Cdraw_wire*8) + 4) )w_c[3]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Cdraw_wire*8) + 5) )w_c[4]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Cdraw_wire*8) + 6) )w_c[5]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Cdraw_wire*8) + 7) )w_c[6]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    if(iwire == ( (Cdraw_wire*8) + 8) )w_c[7]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    */
	    /*
	      if(iwire ==  64+45 )w_c[0]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
	      if(iwire ==  64+46 )w_c[1]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
	      if(iwire ==  64+47 )w_c[2]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
	      if(iwire ==  64+48 )w_c[3]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
	      if(iwire ==  64+49 )w_c[4]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
	      if(iwire ==  64+50 )w_c[4]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
	      if(iwire ==  64+51 )w_c[5]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
	      if(iwire ==  64+52 )w_c[5]->SetBinContent( isample, -1*(weightC - Ped_Mean[iwire-1]) );
	      //if(iwire == ( (Cdraw_wire*8) + 7) )w_c[6]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	      //if(iwire == ( (Cdraw_wire*8) + 8) )w_c[7]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );    
	      if(iwire == ( (Cdraw_wire*8) + 5) )w_c[4]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	      if(iwire == ( (Cdraw_wire*8) + 6) )w_c[5]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	      if(iwire == ( (Cdraw_wire*8) + 7) )w_c[6]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	      if(iwire == ( (Cdraw_wire*8) + 8) )w_c[7]->SetBinContent( isample, weightC - Ped_Mean[iwire-1] );
	    */
	    // Filling Ind-Collection Image
	 //cchsu   if(iwire<64)Induction->SetBinContent( isample, iwire, weightC - Ped_Mean[iwire-1] );
	// cchsu    else Collection->SetBinContent(isample, iwire-64, (weightC - Ped_Mean[iwire-1]) );
	    // This is for the upper window
	    if(iwire< temp_channel_num )Induction->SetBinContent( isample, iwire, weightC - Ped_Mean[iwire-1] );
	    else 
		{ Collection->SetBinContent(isample, iwire- temp_channel_num, (weightC - Ped_Mean[iwire-1]) );
//		cout << " weightC -  Ped_Mean[iwire-1]" << (weightC -  Ped_Mean[iwire-1]) << endl;
		}
	    //if(isample>1000)cout << isample << "  " << iwire << "  " << weightC - Ped_Mean[iwire] << endl;
	  }
	  // Pedestal Subtraction
	  if((PEDESTAL_SUB)&&(read_ev%ped_ratio)){
	    Ped_Mean[iwire-1] = pedestal->GetMean() - baseline;
	    delete pedestal;
	  }
	}//end loop iwire
      }//end loop board
      
      TThread::UnLock();
      
      if(debug)printf("\nshmbuf_raddr 1= %d\n",shmbuf_raddr);  
      shmbuf_raddr += EVENT_SIZE;
      if(debug)printf("\nshmbuf_raddr 2 = %d\n",shmbuf_raddr);   
      if( shmbuf_raddr == BUFFER_SIZE ) shmbuf_raddr=0;
      
      /*
      if(debug){
	int g = 0;
	for(int f = 0; f<16; f++){
	  for(int l=0; l<1; l++)printf("\nEvent = %i \t board = %i shmbuf_r[0]=%lx ", k, f, shmbuf_r[shmbuf_raddr+l+(f*g)] );
	  g+= (2052*2);
	}
	printf("\n");
	k++;
      }*/
      
      operation[0].sem_num = 0; /* Operate on the empty sem     */
      operation[0].sem_op =  1; /* Increment the semval by one   */
      operation[0].sem_flg = 0;
    
      rc = semop( shmbuf_semid, operation, 1 );

      //   try_data:
      //     operation[0].sem_num = 0; /* Operate on the sem     */
      //     operation[0].sem_op =  0; /* Check for zero         */
      //     operation[0].sem_flg = IPC_NOWAIT;

      //     operation[1].sem_num = 0; /* Operate on the sem     */
      //     operation[1].sem_op =  1; /* Increment              */
      //     operation[1].sem_flg = IPC_NOWAIT;

      //     rc = semop( shmdat_semid, operation, 2 );
      //     //printf("TMONITOR semop shmdat tmonitor = %d", rc);
      //     if (rc < 0) goto update_hist;
      //     //    shmdat_r = ((unsigned short*) shmdat_address);
      //     printf("\n  %d   %d \n",shmdat_r[0],shmdat_r[1]);

      //     operation[0].sem_num = 0; /* Operate on the sem     */
      //     operation[0].sem_op = -1; /* Decrement              */
      // operation[0].sem_flg = IPC_NOWAIT;
      // rc = semop( shmdat_semid, operation, 1 );
      
      // ch0->SetMarkerStyle(21);
      // ch0->SetMarkerSize(0.7);
      // ch1->SetMarkerSize(0.7);
      // ch0->SetFillColor(16);
      // ch0->SetMarkerColor(read_ev+1);
      
      Frame->canv_ind->GetCanvas()->Clear();
      Frame->canv_ind->GetCanvas()->cd();
      //      Frame->canv_ind->GetCanvas()->SetFillColor(kBlack);
      Induction->Draw("COLz");
      iY1->Draw("SAME");
      iC1->Draw("SAME");  
      Frame->canv_col->GetCanvas()->Clear();
      Frame->canv_col->GetCanvas()->cd();
      //Frame->canv_col->GetCanvas()->SetFillColor(kBlack);
      Collection->Draw("COLz");
      
      Y1->Draw("SAME");
      C1->Draw("SAME");
      Frame->canv_w_ind0->GetCanvas()->Clear();
      
      /*      Frame->canv_ind->GetCanvas()->Draw();       
      Frame->canv_col->GetCanvas()->Draw();     
      Frame->canv_ind->GetCanvas()->SetFillColor(kWhite);
      Frame->canv_col->GetCanvas()->SetFillColor(kWhite);
      Frame->canv_ind->GetCanvas()->Draw();       
      Frame->canv_col->GetCanvas()->Draw(); 
      */
      Float_t IMIN    =  0;
      Float_t IMAX    =  0;
      Float_t imin[wiresize] = {0};
      Float_t imax[wiresize] = {0};
      Float_t CMIN    =  0;
      Float_t CMAX    =  0;
      Float_t cmin[wiresize] = {0};
      Float_t cmax[wiresize] = {0};
      for(Int_t p=0;p<wiresize;p++){
	imin[p] = w_i[p]->GetMinimum();
	imax[p] = w_i[p]->GetMaximum();
	if(imin[p]<IMIN)IMIN = imin[p];
	if(imax[p]>IMAX)IMAX = imax[p];
     
	cmin[p] = w_c[p]->GetMinimum();
	cmax[p] = w_c[p]->GetMaximum();
	if(cmin[p]<CMIN)CMIN = cmin[p];
	if(cmax[p]>CMAX)CMAX = cmax[p]; 
      }
      
      w_I->GetYaxis()->SetRangeUser((int)IMIN-10,(int)IMAX+10);
      w_I->Draw("");
      
      TGaxis *w_time = new TGaxis(0, IMAX+10, SAMPLES, IMAX+10, "ifx", 510, "-");
      w_time->SetTitleColor( 4 );
      w_time->SetLabelColor( 2 );
      w_time->SetLabelSize(0.04);
      w_time->SetTitle("time [#mus]        ");
      w_time->SetLabelOffset(-0.01);
      w_time->Draw("SAME");
      
      TF1    *f_mV = new TF1("f_mV","x", (IMIN-10)*conv_fac, (IMAX+10)*conv_fac );
      TGaxis *w_mV = new TGaxis(SAMPLES, IMIN-10, SAMPLES, IMAX+10, "f_mV", 510, "+");   
      w_mV->SetTitleColor( 4 );
      w_mV->SetLabelColor( 2 );
      w_mV->SetLabelSize(0.04);
      w_mV->SetTitle("Amplitude [mV]                                   ");
      w_mV->SetLabelOffset(+0.045);
      w_mV->Draw("SAME");
        
      Frame->canv_w_ind0->GetCanvas()->cd();  
      for(Int_t p=0;p<wiresize;p++){
	gStyle->SetHistLineColor( p+1 );
	w_i[p]->UseCurrentStyle();
	w_i[p]->Draw("SAME");
      }
      
      Frame->canv_w_col0->GetCanvas()->Clear();
      w_C->GetYaxis()->SetRangeUser((int)CMIN-10,(int)CMAX+10);
      w_C->Draw("");

      TGaxis *Cw_time = new TGaxis(0, CMAX+10, SAMPLES, CMAX+10, "Cfx", 510, "-");
      Cw_time->SetTitleColor( 4 );
      Cw_time->SetLabelColor( 2 );
      Cw_time->SetLabelSize(0.04);
      Cw_time->SetTitle("time [#mus]        ");
      Cw_time->SetLabelOffset(-0.01);
      Cw_time->Draw("SAME");
      
      TF1    *Cf_mV = new TF1("Cf_mV","x", (CMIN-10)*conv_fac, (CMAX+10)*conv_fac );
      TGaxis *Cw_mV = new TGaxis(SAMPLES, CMIN-10, SAMPLES, CMAX+10, "Cf_mV", 510, "+");   
      Cw_mV->SetTitleColor( 4 );
      Cw_mV->SetLabelColor( 2 );
      Cw_mV->SetLabelSize(0.04);
      Cw_mV->SetTitle("Amplitude [mV]                                   ");
      Cw_mV->SetLabelOffset(+0.045);
      Cw_mV->Draw("SAME");
 
      Frame->canv_w_col0->GetCanvas()->cd();
      for(Int_t p=0;p<wiresize;p++){
	gStyle->SetHistLineColor( p+1 );
	w_c[p]->UseCurrentStyle();
	w_c[p]->Draw("SAME");
      }
      
      // Legend for wire histos
      /*   TLegend* leg = new TLegend(0.87,0.65,0.99,0.99);
      leg->AddEntry(w_c0,"Wire  0","l");
      leg->AddEntry(w_c1,"Wire  1","l");
      leg->AddEntry(w_c2,"Wire  2","l");
      leg->AddEntry(w_c3,"Wire  3","l");
      leg->AddEntry(w_c4,"Wire  4","l");
      leg->AddEntry(w_c5,"Wire  5","l");
      leg->AddEntry(w_c6,"Wire  6","l");
      leg->AddEntry(w_c7,"Wire  7","l");
      */
      // and add a header (or "title") for the legend
      //  leg->SetHeader("Histos colors");
      // leg->Draw();
      
      Frame->canv_ind->GetCanvas()->Modified();
      Frame->canv_ind->GetCanvas()->Update();
      Frame->canv_w_ind0->GetCanvas()->Modified();
      Frame->canv_w_ind0->GetCanvas()->Update();
      
      Frame->canv_col->GetCanvas()->Modified();
      Frame->canv_col->GetCanvas()->Update();
      Frame->canv_w_col0->GetCanvas()->Modified();
      Frame->canv_w_col0->GetCanvas()->Update();
  
      read_ev++;
  
      for(Int_t p=0;p<wiresize;p++)w_i[p]->Reset();
      for(Int_t p=0;p<wiresize;p++)w_c[p]->Reset();  
    }
  printf("\n Out of the (for ;;) loop of TMONITOR \n");
  
  Frame->canv_w_ind0->GetCanvas()->Modified();
  Frame->canv_w_ind0->GetCanvas()->Update();

  Frame->canv_w_col0->GetCanvas()->Modified();
  Frame->canv_w_col0->GetCanvas()->Update();

  rc = shmdt(shmbuf_address);
  if (rc==-1) {
    printf("TMONITOR main: shmdt() failed\n");
  }
  
  return (void*) 0;
  
}

Pixel_t color  = 0xADD8E6;
Pixel_t color1 = 0xFF00E6;
//int     arg2;
int main( int argc, char *argv[] ) 
{
  SAMPLES = atoi(argv[1]); 
  arg2    = atoi(argv[2]);
  downS   = atoi(argv[3]);

  //cout << " Downsample Tmonitor= " << downS << endl;

  TApplication theApp("App", &argc, argv);
  Frame = new MyMainFrame(gClient->GetRoot(),500,600,1);
  Frame->SetWindowName("Real Time Event Monitor - ARGONTUBE prototype - Ver. 2.0");
  Frame->SetIconPixmap("../LHEP_GUI-1.0/lhep_logo.jpg");
  Frame->SetBackgroundColor(color);
   
//   for(int i = 0; i< SAMPLES; i++) 
//     {
//       x[i] = i;
//     }
  
  // create the TThread instances
  th1 = new TThread("th1",myth);

  th1->Run();

  printf("*** TMONITOR Exit the program by selecting Quit from the File menu ***\n");
  theApp.Run();

  return 1;
}

