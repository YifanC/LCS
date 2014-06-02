#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _LHEP_USE_SHM_

#define  LHEP_USE_SHM_BUFFER 1
#define  LHEP_USE_SHM_DATA 1

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <errno.h>

#include <eib7.h>
#endif

#define DAQ_MAIN 1

#include "daq_main.h"
#include "daq_shm.h"
#include "daq_laser.h"

typedef int bool;
enum {false = 0, true = 1};

extern void* producer(void*);
extern void* consumer(void*);

extern int daq_vme_init(void);
extern EIB7_HANDLE initEIB(void);
extern int   closeEIB();
extern void  daq_vme_end(void);
//extern int   gzip_init(void);
//extern int   gzip_end(void);

#define NUM_THREADS 2

pthread_t tid[NUM_THREADS];      /* array of thread IDs */
sem_t emptyBuffers, fullBuffers;

bool debug;
//bool enable_gzip;

// signal handling
bool forced_stop;



void termination_handler (int signum) {
  if((signum == SIGUSR1) || (signum == SIGTERM) || (signum == SIGINT)) {
    forced_stop=true;
 
    /* unlock threads..... */
    sleep(1);
    sem_post(&emptyBuffers);
    sem_post(&fullBuffers);
    daq_vme_end();
    closeEIB();
  }
}

int main(int argc, char *argv[]) {
    
   unsigned int *buffer;
  int rc;
  //  bool last_state;
 
#ifdef _LHEP_USE_SHM_

#ifdef LHEP_USE_SHM_BUFFER 
  key_t shmbuf_sem_key, shmbuf_shm_key;
  unsigned short b_array[2];
#endif

#ifdef LHEP_USE_SHM_DATA
  key_t shmdat_sem_key, shmdat_shm_key; 
  unsigned short d_array[2];
#endif

  // struct shmid_ds shmid_struct;
#endif

  struct sigaction new_action;
     
  /* Set up the structure to specify the new action. */
  new_action.sa_handler = termination_handler;
  sigemptyset (&new_action.sa_mask);
  new_action.sa_flags = 0;
     
  sigaction (SIGUSR1, &new_action, NULL);
  sigaction (SIGTERM, &new_action, NULL);

  rc = 0;
  debug = false;
  //enable_gzip = false;

/*   avtime=0.0; */

/*   if(enable_gzip) { */
/*     rc = gzip_init(); */
/*     if(rc != 0) { */
/*       printf("Error in gzip_init() !!\n"); */
/*       return -1; */
/*     } */
/*   } */

  int header, number_of_loop, loop;
  // run_header  rh;
  //unsigned int DAQ_time_window;
  
  daqpar.nevents            = atoi(argv[1]); 
  daqpar.nchannel           = atoi(argv[2]); 
  daqpar.runnum             = atoi(argv[3]); 
  number_of_loop            = atoi(argv[4]); 
  daqpar.DAQ_time_window    = atoi(argv[5]); 
  daqpar.nsamples           = atoi(argv[6]); 
  daqpar.nPost              = atoi(argv[7]); 
  daqpar.Monitor_Mode_value = atoi(argv[8]); 
  daqpar.Save_File          = atoi(argv[9]); // from GUI ON/OFF Save Raw Data File
  loop                      = daqpar.runnum + number_of_loop;
  header                    = EV_HEADER_SIZE;           // in 32 bit words 
  daqpar.board_number       = atoi(argv[10]);         
  daqpar.DACLevel           = atoi(argv[11]);      
  daqpar.DownSample         = atoi(argv[12]);     
  daqpar.AnMonInv           = atoi(argv[13]);
  daqpar.AnMonOffSign       = atoi(argv[14]);
  daqpar.AnMonOffVal        = atoi(argv[15]);
  daqpar.AnMonMAG           = atoi(argv[16]);
  
  daqpar.HV_Cat             = atoi(argv[17]);
  daqpar.HV_Ind             = atoi(argv[18]);
  daqpar.HV_Col             = atoi(argv[19]);

  daqpar.TCP_connection     = atoi(argv[20]); // from GUI ON/OFF TCP Connection (energy meter)
  daqpar.Laser              = atoi(argv[21]); // from GUI ON/OFF Laser position

  daqpar.MajMo_Threshold    = atoi(argv[22]);
  
  if(debug)
    {
      printf("daqpar.runnum   = %iu \n" , daqpar.runnum);
      printf("daqpar.nchannel = %iu \n" , daqpar.nchannel);
      printf("daqpar.nevents  = %iu \n" , daqpar.nevents);
      printf("loops           = %d \n" ,  number_of_loop);
    }
  
  //daqpar.nsamples = SAMPLES;
  blt_size_32 = ( (daqpar.nsamples / 2) * daqpar.nchannel) + header; //  2052*32bitWords for 1 board  (  8ch)
  blt_size    = blt_size_32 * 4;                                 //  8208 byte per board (8 ch)
  event_size  = blt_size_32 * daqpar.board_number;               //  8208*32bitWords for 4 boards ( 32ch)
  buffer_size = N_BUFFERS * event_size;                          // 82080*32bitwords for 10 full event size
  
  printf("\ndaqpar.board_number= %iu", daqpar.board_number);
  if(debug)
    {
      printf("blt_size    = %d bytes\n", blt_size);
      printf("blt_size_32 = %d 32bitW\n", blt_size_32);
      printf("event_size  = %d 32bitW\n", event_size);
      printf("buffer_size = %d 32bitW\n", buffer_size);
    }

  buffer = malloc(buffer_size*4);   // Buffer size in bytes
  printf("Buffer %p", buffer);
 if (buffer==NULL){
    printf("Memory allocation failed");
    exit (1);
  }
  
  // Init EIB
  if(!((bool)daqpar.Laser)) initEIB();

  rc = daq_vme_init();
  if(rc != 0) {
    printf("Error in daq_vme_init() !!\n");
    return -1;
  }

#ifdef _LHEP_USE_SHM_
#ifdef LHEP_USE_SHM_BUFFER

  /* keys for shared memory and semaphores */
  shmbuf_sem_key = (key_t) LHEP_SHMBUF_SEM_KEY;
  shmbuf_shm_key = (key_t) LHEP_SHMBUF_SHM_KEY;

  
  /* create 2 semaphores for shared memory buffer */
  shmbuf_semid = semget( shmbuf_sem_key, 2, 0666 | IPC_CREAT | IPC_EXCL );

  if( (shmbuf_semid == -1) && (errno == EEXIST) )
    shmbuf_semid = semget( shmbuf_sem_key, 2, 0666);

  if(shmbuf_semid == -1) {
    if(debug) printf("main: shmbuf semget() failed\n");
    return -1;
  }

  /* initialize sem 0 (empty) to SHM_BUFFERS and sem 1 (full) to 0 */
  b_array[0] = 10;
  b_array[1] =  0;

  /* The '1' on this command is a no-op, because the SETALL command
     is used.                                                      */
  rc = semctl( shmbuf_semid, 1, SETALL, b_array);
  if(rc == -1) {
    if(debug) printf("main: shmbuf semctl() initialization failed\n");
    return -1;
  }
  printf("still here after step 6\n");//wtf here in this area it crashes
  
  if(debug)printf("4 x buffersize = %d (328320bytes)\n",4* buffer_size);
  shmbuf_shmid = shmget(shmbuf_shm_key, 4*buffer_size, 0644 | IPC_CREAT | IPC_EXCL);

  if( (shmbuf_shmid == -1) && (errno == EEXIST) )
    shmbuf_shmid = shmget(shmbuf_shm_key, 4*buffer_size, 0644);

  if (shmbuf_shmid == -1) {
    if(debug) printf("main: shmbuf shmget() failed\n");
    return -1;
  }
printf("still here after step 7\n");
  /* Attach the shared memory segment to the server process.       */
  shmbuf_address = shmat(shmbuf_shmid, NULL, 0);
  if ( shmbuf_address==NULL ) {
    if(debug) printf("main: shmat() failed\n");
    return -1;
  }
#endif

#ifdef LHEP_USE_SHM_DATA

  shmdat_sem_key = (key_t) LHEP_SHMDAT_SEM_KEY;
  shmdat_shm_key = (key_t) LHEP_SHMDAT_SHM_KEY;


  /* create 1 semaphore for shared memory data */
  shmdat_semid = semget( shmdat_sem_key, 1, 0666 | IPC_CREAT | IPC_EXCL );

  if( (shmdat_semid == -1) && (errno == EEXIST) )
    shmdat_semid = semget( shmdat_sem_key, 1, 0666);

  if ( shmdat_semid == -1 && errno != EEXIST) {
    if(debug) printf("main: shmdat semget() failed\n");
    return -1;
  }

  d_array[0] = 0;

  rc = semctl( shmdat_semid, 0, SETALL, d_array);
  if(rc == -1) {
    if(debug) printf("main: shmdat semctl() initialization failed\n");
    return -1;
  }

  shmdat_shmid = shmget(shmdat_shm_key, SHM_DATA_SIZE, 0644 | IPC_CREAT | IPC_EXCL);

  if( (shmdat_shmid == -1) && (errno == EEXIST) )
    shmdat_shmid = shmget(shmdat_shm_key, SHM_DATA_SIZE, 0644);

  if (shmdat_shmid == -1) {
    if(debug) printf("main: shmdat shmget() failed\n");
    return -1;
  }

  shmdat_address = shmat(shmdat_shmid, NULL, 0);
  if ( shmdat_address==NULL ) {
    if(debug) printf("main: shmat() failed\n");
    return -1;
  }

#endif   /* LHEP_USE_SHM_DATA */

#endif /* _LHEP_USE_SHM_ */

  read_ev=0; written_ev=0; lost_ev=0;
  forced_stop = false; 
  
  /* read/write semaphores*/
  sem_init(&emptyBuffers,   0, N_BUFFERS);
  sem_init(&fullBuffers,    0, 0);
  
  pthread_create(&tid[0], NULL, producer, &buffer[0]);
  pthread_create(&tid[1], NULL, consumer, &buffer[0]);
  
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  
#ifdef LHEP_USE_SHM_BUFFER
  rc = shmdt(shmbuf_address); //detach shared buffer 
  if (rc==-1) 
    {
     if(debug) printf("main: shmdt() failed\n");
     return -1;
    }
  if(debug) printf("main: shmbuf shmdt()\n");
#endif  
  
#ifdef LHEP_USE_SHM_DATA 
  rc = shmdt(shmdat_address); //detach shared data
  if (rc==-1) 
    {
      if(debug) printf("main: shmdat shmdt() failed\n");
      return -1;
    }
  if(debug) printf("main: shmdat shmdt()\n");
#endif
  

  daq_vme_end();
  if(!((bool)daqpar.Laser)) closeEIB();
  
/*   if(enable_gzip) { */
/*     rc = gzip_end(); */
/*     if(rc != 0) { */
/*       printf("Error in gzip_end() !!\n"); */
/*       return -1; */
/*     } */
/*   } */
  
  free(buffer);
  
  printf("\nmain() reporting that all threads have terminated\n");
  printf(" acquired  = %d \n",written_ev);
  printf(" processed = %d \n",read_ev);
  printf("      lost = %d \n",lost_ev);
  printf("  deadtime = %f \n",(1.0 - ((float) read_ev)/((float) (written_ev+lost_ev)))*100.0);
/*   printf("event rate = %f \n",1.0/(avtime/((float)(written_ev+lost_ev)))*1000000.0); */

  return 0;
}


