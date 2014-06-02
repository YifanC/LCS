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

using namespace std;

MyMainFrame    *Frame;
TThread        *th1;
int MyMainFrame::n_histo = 0;

#define PEDESTAL_SUB   1                // Pedestal subtraction (only for display)
#define N_BOARD       4                          // Number of channels per board
#define N_CHANNEL     8                          // Number of channels per board
#define HEADER        8                          // Header x board in 16bitW
#define SAMPLES     512                          // samples number per channel
#define BOARD_SIZE 4096                            // in 16bitW
#define EVENT_SIZE (BOARD_SIZE + HEADER) * N_BOARD // ev size in 16bit words

#define N_BUFFERS   10
#define BUFFER_SIZE N_BUFFERS * EVENT_SIZE  //in 16bitW

#define LHEP_SHMBUF_SEM_KEY 1940345
#define LHEP_SHMBUF_SHM_KEY 4032913

// #define SHM_DATA_SIZE 1000
// #define LHEP_SHMDAT_SEM_KEY 1940346
// #define LHEP_SHMDAT_SHM_KEY 4032914

Int_t x[SAMPLES] = {0};

bool debug = true;
void* myth(void*) {

  // const Int_t kUPDATE = 1;
  //  const Int_t kUPDATE2 = 1;
  
  int IN = MyMainFrame::n_histo;

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
  
  /* get semaphores */
  //bool debug=true;

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
  shmbuf_raddr=0;
  
  Int_t  y[N_CHANNEL][SAMPLES];
  Int_t ev[EVENT_SIZE] = {0};
  
  TGraph *ch0 = 0;
  TGraph *ch1 = 0;
  TGraph *ch2 = 0;
  TGraph *ch3 = 0;
  TGraph *ch4 = 0;
  TGraph *ch5 = 0;
  TGraph *ch6 = 0;
  TGraph *ch7 = 0;

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
      
     
      //LCD->display((int)read_ev);

      printf(" \n TMONITOR monitor: rc OK for event %d\n",read_ev);
     
      IN = MyMainFrame::n_histo; 
      for(int g = 0 ; g < BOARD_SIZE ; g++)	
	{ 
	  ev[g]= shmbuf_r[(g+shmbuf_raddr+HEADER)+(IN*(BOARD_SIZE+HEADER))] & 0x03FF;
	}
      TThread::Lock();   
      for(int k=0; k < N_CHANNEL; k++) 
	{
	  for(int i = 0; i< SAMPLES; i++) 
	    {
	      y[k][i] = ev[i + ( SAMPLES * k )];
	    }
	}
      
      ch0  = new TGraph (SAMPLES, x, y[0]);
      ch1  = new TGraph (SAMPLES, x, y[1]);
      ch2  = new TGraph (SAMPLES, x, y[2]);
      ch3  = new TGraph (SAMPLES, x, y[3]);
      ch4  = new TGraph (SAMPLES, x, y[4]);
      ch5  = new TGraph (SAMPLES, x, y[5]);
      ch6  = new TGraph (SAMPLES, x, y[6]);
      ch7  = new TGraph (SAMPLES, x, y[7]);
      TThread::UnLock();
      
      if(debug)printf("\nshmbuf_raddr 1= %d\n",shmbuf_raddr);  
      shmbuf_raddr += EVENT_SIZE;
      if(debug)printf("\nshmbuf_raddr 2 = %d\n",shmbuf_raddr);   
      if( shmbuf_raddr == BUFFER_SIZE ) shmbuf_raddr=0;

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
       
      //ch0->SetMarkerStyle(21);
      
      ch0->SetMarkerSize(0.8);
      ch1->SetMarkerSize(0.8);
      ch2->SetMarkerSize(0.8);
      ch3->SetMarkerSize(0.8);
      ch4->SetMarkerSize(0.8);
      ch5->SetMarkerSize(0.8);
      ch6->SetMarkerSize(0.8);
      ch7->SetMarkerSize(0.8);
      ch0->SetTitle("Input 0");
      ch1->SetTitle("Input 1");
      ch2->SetTitle("Input 2");
      ch3->SetTitle("Input 3");
      ch4->SetTitle("Input 4");
      ch5->SetTitle("Input 5");
      ch6->SetTitle("Input 6");
      ch7->SetTitle("Input 7");
      ch0->GetYaxis()->SetTitle("Amplitude [ADC counts]");
      ch0->GetXaxis()->SetTitle("drift time [samples x 10ns]");
      ch1->GetYaxis()->SetTitle("Amplitude [ADC counts]");
      ch1->GetXaxis()->SetTitle("drift time [samples x 10ns]");
      ch2->GetYaxis()->SetTitle("Amplitude [ADC counts]");
      ch2->GetXaxis()->SetTitle("drift time [samples x 10ns]");
      ch3->GetYaxis()->SetTitle("Amplitude [ADC counts]");
      ch3->GetXaxis()->SetTitle("drift time [samples x 10ns]");
      ch4->GetYaxis()->SetTitle("Amplitude [ADC counts]");
      ch4->GetXaxis()->SetTitle("drift time [samples x 10ns]");
      ch5->GetYaxis()->SetTitle("Amplitude [ADC counts]");
      ch5->GetXaxis()->SetTitle("drift time [samples x 10ns]");
      ch6->GetYaxis()->SetTitle("Amplitude [ADC counts]");
      ch6->GetXaxis()->SetTitle("drift time [samples x 10ns]");
      ch7->GetYaxis()->SetTitle("Amplitude [ADC counts]");
      ch7->GetXaxis()->SetTitle("drift time [samples x 10ns]");

      //ch0->SetFillColor(16);
      //ch0->SetMarkerColor(read_ev+1);
      //  if( ( read_ev % kUPDATE2 ) == 0  )
      // 	{
      Frame->fEcanvas0->GetCanvas()->Clear();
      Frame->fEcanvas0->GetCanvas()->cd();
      ch0->Draw("AP");
      Frame->fEcanvas1->GetCanvas()->Clear();
      Frame->fEcanvas1->GetCanvas()->cd();
      ch1->Draw("AP");
      Frame->fEcanvas2->GetCanvas()->Clear();
      Frame->fEcanvas2->GetCanvas()->cd();
      ch2->Draw("AP");
      Frame->fEcanvas3->GetCanvas()->Clear();
      Frame->fEcanvas3->GetCanvas()->cd();
      ch3->Draw("AP");
      Frame->fEcanvas4->GetCanvas()->Clear();
      Frame->fEcanvas4->GetCanvas()->cd();
      ch4->Draw("AP");
      Frame->fEcanvas5->GetCanvas()->Clear();
      Frame->fEcanvas5->GetCanvas()->cd();
      ch5->Draw("AP");
      Frame->fEcanvas6->GetCanvas()->Clear();
      Frame->fEcanvas6->GetCanvas()->cd();
      ch6->Draw("AP");
      Frame->fEcanvas7->GetCanvas()->Clear();
      Frame->fEcanvas7->GetCanvas()->cd();
      ch7->Draw("AP");
	  //usleep(5000);	
      //}
      Frame->fEcanvas0->GetCanvas()->Modified();
      Frame->fEcanvas0->GetCanvas()->Update(); 
      Frame->fEcanvas1->GetCanvas()->Modified();
      Frame->fEcanvas1->GetCanvas()->Update(); 
      Frame->fEcanvas2->GetCanvas()->Modified();
      Frame->fEcanvas2->GetCanvas()->Update(); 
      Frame->fEcanvas3->GetCanvas()->Modified();
      Frame->fEcanvas3->GetCanvas()->Update(); 
      Frame->fEcanvas4->GetCanvas()->Modified();
      Frame->fEcanvas4->GetCanvas()->Update(); 
      Frame->fEcanvas5->GetCanvas()->Modified();
      Frame->fEcanvas5->GetCanvas()->Update(); 
      Frame->fEcanvas6->GetCanvas()->Modified();
      Frame->fEcanvas6->GetCanvas()->Update(); 
      Frame->fEcanvas7->GetCanvas()->Modified();
      Frame->fEcanvas7->GetCanvas()->Update(); 
      
      read_ev++;
    }
  printf("out loop infinito for ;; TMONITOR \n");
  //while(read_ev!=10)
  //while(true);

  Frame->fEcanvas0->GetCanvas()->Modified();
  Frame->fEcanvas0->GetCanvas()->Update(); 
  Frame->fEcanvas1->GetCanvas()->Modified();
  Frame->fEcanvas1->GetCanvas()->Update(); 
  Frame->fEcanvas2->GetCanvas()->Modified();
  Frame->fEcanvas2->GetCanvas()->Update(); 
  Frame->fEcanvas3->GetCanvas()->Modified();
  Frame->fEcanvas3->GetCanvas()->Update(); 
  Frame->fEcanvas4->GetCanvas()->Modified();
  Frame->fEcanvas4->GetCanvas()->Update(); 
  Frame->fEcanvas5->GetCanvas()->Modified();
  Frame->fEcanvas5->GetCanvas()->Update(); 
  Frame->fEcanvas6->GetCanvas()->Modified();
  Frame->fEcanvas6->GetCanvas()->Update(); 
  Frame->fEcanvas7->GetCanvas()->Modified();
  Frame->fEcanvas7->GetCanvas()->Update(); 
  
  //   } while (forced_stop==false);
  
  rc = shmdt(shmbuf_address);
  if (rc==-1) {
    printf("TMONITOR main: shmdt() failed\n");
  }

  return (void*) 0;
  
}

Pixel_t color  = 0xADD8E6;
Pixel_t color1 = 0xFF00E6;

int main( int argc, char *argv[] ) 
{
  
  TApplication theApp("App", &argc, argv);
  Frame = new MyMainFrame(gClient->GetRoot(),500,500,1);
  Frame->SetWindowName(" LHEP Kleine Argontube DAQ(VME V1724) - Ver. 1.1");
  Frame->SetIconPixmap("../LNGS_GUI-1.0/lhep_logo.jpg");
  // Frame->MapSubwindows();
  Frame->SetBackgroundColor(color);
  // int EVTOT = atoi(argv[2]);
  // int N_CHANNEL = atoi(argv[3]);
  
  for(int i = 0; i< SAMPLES; i++) 
    {
      x[i] = i;
    }
  
  // create the TThread instances
  th1 = new TThread("th1",myth);

  //SetCancelDeferred();
  th1->Run();

  printf("*** TMONITOR Exit the program by selecting Quit from the File menu ***\n");
  theApp.Run();

  return 1;
}

