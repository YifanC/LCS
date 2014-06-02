
typedef struct {
  unsigned int    header_size;
  unsigned int    header_version;
  unsigned int    run_number;
  unsigned int    daq_version;
  unsigned int    number_of_events;
  unsigned int    date_time_start;
  unsigned int    date_time_stop;
  unsigned int    event_size;
  unsigned int    number_of_boards;
  unsigned int    HV_Cat;
  unsigned int    HV_Ind;
  unsigned int    HV_Col;
  unsigned int    DownSample;
  unsigned int    nsamples;
} run_header;

typedef struct {
  unsigned short header_size;
  unsigned short header_version;
  unsigned int   event_number;
  unsigned int   event_time;
  unsigned int   number_of_boards;
  unsigned int   number_of_samples;
  unsigned int   event_size;
  float laser_energy;

} ev_header;

typedef struct {
  long long PosHor;
  long long PosRefHor;
  unsigned short TriggerStampHor;
  unsigned short StatusHor;
  unsigned int TimestampHor;
  long long PosVert;
  unsigned short TriggerStampVert;
  unsigned short StatusVert;
  unsigned int TimestampVert;
} ev_laser;

#define RUN_HEADER_SIZE 30             // in 32BitWords
#define EV_HEADER_SIZE   4             // in 32BitWords
#define PEDESTAL_SUB     1             // Pedestal subtraction (only for display)
//#define N_BOARD         16   //cchsu          // Number of boards
#define N_BOARD         8             // Number of boards
//#define SAMPLES       2048           // samples number per channel
//int SAMPLES;
#define N_CHANNEL        8 //            // Number of channels per board
#define HEADER           8             // Header x board in 16bitW
#define BOARD_SIZE  SAMPLES * N_CHANNEL            // 4096 in 16bitW
#define EVENT_SIZE (BOARD_SIZE + HEADER) * N_BOARD // ev size in 16bit words
#define N_BUFFERS        10
#define BUFFER_SIZE N_BUFFERS * EVENT_SIZE          //in 16bitW



#ifdef DAQ_MAIN
int blt_size;
int blt_size_32;
int buffer_size;
int event_size;

#else
extern int blt_size;
extern int blt_size_32;
extern int buffer_size;
extern int event_size;

#endif

// Struct for daq_main parameters
typedef struct {
  unsigned int runnum;
  unsigned int nevents;
  unsigned int nchannel;        // nchannel per boards
  unsigned int nsamples;
  unsigned int DAQ_time_window;
  unsigned int nPost;
  unsigned int Monitor_Mode_value;
  int          Save_File;
  int          TCP_connection;
  int          Laser;
  unsigned int board_number;
  unsigned int DACLevel;
  unsigned int DownSample;
  unsigned int AnMonInv;
  unsigned int AnMonOffSign;
  unsigned int AnMonOffVal;
  unsigned int AnMonMAG;
  unsigned int HV_Cat;
  unsigned int HV_Ind;
  unsigned int HV_Col;
  unsigned int MajMo_Threshold; // ML
} daq_params;

#ifdef DAQ_MAIN

daq_params      daqpar;

#else

extern daq_params      daqpar;

#endif
