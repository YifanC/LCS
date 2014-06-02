#include "GUI_schema.h"

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>

#include <QApplication>
#include <pthread.h>
//#include <qslider.h>
#include <qlcdnumber.h>
//#include <qpushbutton.h>
#include <QSpinBox>
//#include <qlineedit.h>
//#include <qstring.h>
#include <QIcon>


#define NPMT 12
#define DATA_SIZE 4096   // samples
#define PACK_SIZE 2*DATA_SIZE*5/16   
#define EVENT_SIZE PACK_SIZE*NPMT

#define N_BUFFERS 10
#define BUFFER_SIZE N_BUFFERS*EVENT_SIZE 

#define SHM_DATA_SIZE 1000
#define WARP_SHMDAT_SEM_KEY 1940346
#define WARP_SHMDAT_SHM_KEY 4032914

using namespace std;

//extern QLCDNumber *LCD, *LCD_lost;
extern QSpinBox *EventSpinBox;
extern QSpinBox *LCDSpinBox, *LCDlostSpinBox;;
//extern QLCDNumber *rbMT_3, *rbMT_4, *rbMT_5, *rbMT_6, *rbMT_7, *rbMT_8,*rbMT_9;
//ButtonsGroups buttonsgroups;
//bool     shm_del;

void* LCD_th(void*) 
{
  bool debug = false;
  //int oldstate;
  // int oldtype;
  
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL);
 
  //  SHM FOR GUI
  key_t shmdat_sem_key, shmdat_shm_key;
  
  shmdat_sem_key = (key_t) WARP_SHMDAT_SEM_KEY;
  shmdat_shm_key = (key_t) WARP_SHMDAT_SHM_KEY;
  
  int shmdat_semid=-1, shmdat_shmid=-1;
  
  void* shmdat_address;
  unsigned short* shmdat_r=NULL;
  
  struct sembuf operation[2];

  int rc;
  printf("sono nel thread LCD \n");

  // get semaphores 
  printf("LCD thread... getting shmdat semaphore...");
  retry_dsem:
  shmdat_semid = semget( shmdat_sem_key, 1, 0666);
  if(debug)printf("\n LCD shmdatTH semID= %d",shmdat_semid);
  if(shmdat_semid > 0 ) goto ok_dsem;
  usleep(200000);
  goto retry_dsem;
 ok_dsem:
  printf("LCD OK\n");
  
  printf("\n LCD getting shmdat shared memory segment...");
 retry_dshm:
  shmdat_shmid = shmget(shmdat_shm_key, 0, 0644);
  if(debug)printf("\n shmdatTH shmID= %d",shmdat_shmid);
  if(shmdat_shmid > 0) goto ok_dshm;
  usleep(200000);
  goto retry_dshm;
 ok_dshm: 
  
 // Attach the shared memory segment to the server process.       
  shmdat_address = shmat(shmdat_shmid, NULL, 0);
  if ( shmdat_address==NULL ) {
    printf("LCD main: shmatTH() failed\n");
  }

 shmdat_r = ((unsigned short*) shmdat_address);

 //#ifdef WARP_USE_SHM_DATA
 if(debug)printf("shmdat_r[2] = %d", (int)shmdat_r[2]);
 
 int LCD_flag = 0;
 do{
    LCD_flag++;
    
    pthread_testcancel();
    
    operation[0].sem_num = 0; // Operate on the sem     
    operation[0].sem_op =  0; // Check for zero         
    operation[0].sem_flg = IPC_NOWAIT;
    
    operation[1].sem_num = 0; // Operate on the sem     
    operation[1].sem_op =  1; // Increment              
    operation[1].sem_flg = IPC_NOWAIT;
    
    rc = semop( shmdat_semid, operation, 2 );
    if(debug)printf("semop shm GUI= %d", rc);
    if (rc < 0) goto exit_shm_data;
    
    // if(LCD_flag == 1)
    //   {
    //   rbMT_3->display((int)shmdat_r[3]);
    //   rbMT_4->display((int)shmdat_r[4]);
    //   rbMT_5->display((int)shmdat_r[5]);
    //   rbMT_6->display((int)shmdat_r[6]);
    //   rbMT_7->display((int)shmdat_r[7]);
    //   rbMT_8->display((int)shmdat_r[8]);
    //   rbMT_9->display((int)shmdat_r[9]);
    //   }
    
    LCDSpinBox->setValue((int)shmdat_r[0]);
    //     LCD_lost->display((int)shmdat_r[2]);
    
operation[0].sem_num = 0; // Operate on the sem     
operation[0].sem_op = -1; // Decrement the semval by one   
    operation[0].sem_flg = IPC_NOWAIT;
    
    rc = semop( shmdat_semid, operation, 1 );
    
    
    exit_shm_data:
    
    
    //      if((int)shmdat_r[0] == EventSpinBox->value() )
//	 {
//	 LCD->display((int)shmdat_r[0]);
//	 break;
//	 }
    usleep(500000);
    
 }while( (int)shmdat_r[0]!=EventSpinBox->value() );
     
     // exit_shm_data:
     // if(debug)
 printf("\n sono fuori dal ciclo infinito LCD\n ");
 
 return (void *)0;
}



int main( int argc, char **argv )
{
  // if(argc==1){
  //     printf("\nUSAGE: Put 1 if you want to clean SHM Memory and Buffer - Put 0 if not \n");
  //     exit(1);
  //   }
  //shm_del = atoi(argv[1]);
  //  QIcon::QIcon ( const QString & fileName )
  //const QIcon* UniBe = new QIcon("../LHEP_GUI-1.0/UniBe_logo.jpg");
  printf("bigi");
  QApplication a( argc, argv );
  
  MyWidget buttonsgroups;
  buttonsgroups.resize( 500, 250 );
  buttonsgroups.setWindowTitle("LHEP Argontube prototype DAQ - made by B. Rossi");
  //  buttonsgroups.setCaption( "LHEP Middle Argontube DAQ - made by B. Rossi" );
  //QIcon ico= QIcon("lhep_logo.jpg");
  buttonsgroups.setWindowIcon(QIcon("UniBe_logo.jpg"));
  //a.setWindowIcon("../LHEP_GUI-1.0/UniBe_logo.jpg");
  buttonsgroups.show();
  
  return  a.exec();
}
