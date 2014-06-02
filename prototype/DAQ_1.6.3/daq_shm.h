
#ifdef DAQ_MAIN

int read_ev, written_ev;
short int lost_ev;
sem_t emptyBuffers, fullBuffers;
pthread_mutex_t sig_mutex;
int handled_signal;

#else

extern int read_ev, written_ev;
extern short int lost_ev;
extern sem_t emptyBuffers, fullBuffers;
extern pthread_mutex_t sig_mutex;
extern int handled_signal;

#endif

#ifdef _LHEP_USE_SHM_

#ifdef LHEP_USE_SHM_BUFFER

#define LHEP_SHMBUF_SEM_KEY 1940345
#define LHEP_SHMBUF_SHM_KEY 4032913

#endif

#ifdef LHEP_USE_SHM_DATA

#define SHM_DATA_SIZE 1000
#define LHEP_SHMDAT_SEM_KEY 1940346
#define LHEP_SHMDAT_SHM_KEY 4032914

#endif

#ifdef DAQ_MAIN

#ifdef LHEP_USE_SHM_BUFFER

int shmbuf_semid, shmbuf_shmid;
void* shmbuf_address;

#endif

#ifdef LHEP_USE_SHM_DATA

int shmdat_semid, shmdat_shmid;
void* shmdat_address;

#endif

struct sembuf operation[2];

#else

#ifdef LHEP_USE_SHM_BUFFER

extern int shmbuf_semid, shmbuf_shmid;
extern void* shmbuf_address;

#endif

#ifdef LHEP_USE_SHM_DATA

extern int shmdat_semid, shmdat_shmid;
extern void* shmdat_address;

#endif

extern struct sembuf operation[2];

#endif

#endif
