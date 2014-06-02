#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _LHEP_USE_SHM_

#define  LHEP_USE_SHM_BUFFER 1
#define  LHEP_USE_SHM_DATA 1

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#endif

#include "daq_main.h"
#include "daq_shm.h"

typedef int bool;
enum {false = 0, true = 1};

struct tm *timeinfo;

extern bool debug;
//extern bool enable_gzip;
extern bool forced_stop;
const  unsigned int magic_code = 0xFCD3;

//extern int gzip_data(int nb, char* p, int no, char* out);

ev_header   eh;
ev_laser    laser;

void * consumer(void* bb)
{
  unsigned int *data             = bb;
  int            raddr            =  0;
  char           run_number_s[50] = "/data/Run_00000000";
  //char           run_number_s[50] = "/hd_TB/Data/Run_00000000";
  FILE          *fout;
  time_t         s_time;
  
#ifdef LHEP_USE_SHM_BUFFER
  int            rc;
  int            i;
  int            shmwaddr = 0;
  unsigned int *shm_w;
 /*  unsigned short* shm_w_file; */
#endif
     
  run_header  rh;

  
  time(&s_time);
  
  rh.header_size       = RUN_HEADER_SIZE*4;   // in bytes
  rh.header_version    = 101;
  rh.run_number        = daqpar.runnum;
  rh.number_of_events  = daqpar.nevents;
  rh.date_time_start   = s_time; 
  rh.date_time_stop    = s_time; 
  rh.daq_version       = 101;
  rh.event_size        = event_size*4;    // in bytes
  rh.number_of_boards  = daqpar.board_number/2; // cchsu why divide 2 ? 
//  rh.number_of_boards  = daqpar.board_number; // cchsu  

  rh.HV_Cat            = daqpar.HV_Cat;
  rh.HV_Ind            = daqpar.HV_Ind;
  rh.HV_Col            = daqpar.HV_Col;
  rh.DownSample        = daqpar.DownSample;
  rh.nsamples          = daqpar.nsamples;
   
  eh.header_size       = EV_HEADER_SIZE*4;   // in bytes    
  eh.header_version    = 222;
  eh.number_of_boards  = daqpar.nchannel;
  eh.number_of_samples = daqpar.nsamples;

  
  unsigned int Save_File = 0;
  Save_File = (bool)daqpar.Save_File;   
  //printf("\ndaqpar.Save_File = %i",daqpar.Save_File);

  if(Save_File == 0){ 
    sprintf(&run_number_s[11],"%.08u",daqpar.runnum);
    //sprintf(&run_number_s[16],"%.08lu",daqpar.runnum);
  }
  
  fout = fopen(run_number_s,"w"); 
  if(fout == NULL) return (void*)0;
  printf("N_boards: %i\n", rh.number_of_boards);
  fwrite(&rh, rh.header_size, 1, fout);
  if(debug){
    unsigned int he_index = ftell(fout); //pointer to the row of the FILE 
    printf("Writer - Run_Header_index = %u\n",he_index);
  }

  //  int k=0;
 do {
    read_ev++;
    sem_wait(&fullBuffers);
        
    if(forced_stop == true) break;
    
    //if(debug) printf("consumer called with raddr = %d \n",raddr);
    
    // Write to the file Event time TAG
    fwrite(&eh.event_time, 4, 1, fout);
    // Write to the file the TCP Connection value (Energy of the laser)
    fwrite(&eh.laser_energy, 4, 1, fout);

    // Write Laser Position 
    
    // vertical encoder
    fwrite(&laser.StatusVert, 2, 1, fout);
    fwrite(&laser.TriggerStampVert, 2, 1, fout);
    fwrite(&laser.TimestampVert, 4, 1, fout);
    fwrite(&laser.PosVert, 8, 1, fout); 

    // horizontal encoder
    fwrite(&laser.StatusHor, 2, 1, fout);
    fwrite(&laser.TriggerStampHor, 2, 1, fout);
    fwrite(&laser.TimestampHor, 4, 1, fout);
    fwrite(&laser.PosHor, 8, 1, fout);
    fwrite(&laser.PosRefHor, 8, 1, fout);

    // timeinfo = localtime ( &eh.event_time );
    // printf ( "\nEvent TRG time and date: %s", asctime (timeinfo) );
    // printf("\nevent time = %lx", eh.event_time );

    // Write to file Event Data
    eh.event_size = event_size*4; 
    fwrite(&data[raddr], eh.event_size, 1, fout);
    //eh.event_time  
    if(debug){
      unsigned int data_index = ftell(fout); //pointer to the row of the FILE 
      printf("Writer - Data_index = %u\n",data_index);
    }
#ifdef LHEP_USE_SHM_BUFFER
    
    operation[0].sem_num = 0; /* Operate on the empty sem     */
    operation[0].sem_op = -1; /* Decrement the semval by one   */
    operation[0].sem_flg = IPC_NOWAIT;
    
    rc = semop( shmbuf_semid, operation, 1 );
    //if(debug)printf("\nrc semop begin = %i",rc);
    if (rc >= 0) {
      
      //if(debug) printf("\nrc OK for event %d\n",read_ev);
      
      shm_w = ((unsigned int*) shmbuf_address);
      /*  shm_w_file = ((unsigned short*) shmbuf_address); */
      
      /*  if(debug){
	  printf("shmwaddr = %d\n", shmwaddr);
	  printf("shm_writer = %p\n", shm_w);
	  }*/
      for(i=0; i<event_size; i++) 
	{
	  shm_w[shmwaddr+i]= data[raddr+i];
	  //if(debug)if(i<5 || i>(event_size-5) )printf("\nshw_m[%i]=%lx", i,shm_w[shmwaddr+i]);
	}
      
      /*   if(debug){
	int f = 0;
	int g = 0;
	int l = 0;
    	for(f = 0; f<16; f++){
	  for(l=0; l<1; l++)printf("\nEvent = %i \t board = %i shm_w[0]=%lx ", k, f, data[raddr+(f*g)+l] );
	  g+= 2052;
	}
	printf("\n");
	k++;
	}*/
      
      
      shmwaddr+=event_size;
      //if(debug)printf("\n shmaddr = %i",shmwaddr);
      if(shmwaddr == buffer_size) shmwaddr=0;
      
      operation[0].sem_num = 1; /* Operate on the full sem     */
      operation[0].sem_op =  1; /* Increment the semval by one   */
      operation[0].sem_flg = 0;
      
      rc = semop( shmbuf_semid, operation, 1 );
      //  if(debug)printf("\nrc semop fine = %i",rc);
    } else {
      // if(debug) printf("rc < 0 for event %d\n",read_ev);
    }
    
#endif

    raddr += event_size;
    if(raddr==buffer_size) raddr=0;
    
    sem_post(&emptyBuffers);
    // if(emptyBuf<2)printf("\nCON EmptyBuf=%i", emptyBuf);
    
    //if(debug) printf("  read addr= %d       read ev = %d \n",raddr,read_ev);
    //if(debug) printf("  written ev= %d      forced_stop = %d \n",written_ev,forced_stop);

  } while (read_ev < daqpar.nevents);
  
  rewind(fout);
  rh.number_of_events = read_ev; 
  time(&s_time); 
  rh.date_time_stop = s_time; 
  fwrite(&rh, rh.header_size, 1, fout);

  fclose(fout);

  forced_stop = true;
  /* unlock producer if buffer is full....*/

  sleep(1);
  sem_post(&emptyBuffers);

  return (void *)0;
}   
