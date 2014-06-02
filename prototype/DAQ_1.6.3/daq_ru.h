
typedef struct {
  unsigned long runnum;
  unsigned long nevents;
  unsigned long npmt;
  unsigned long nsamples;
} daq_params;

#ifdef DAQ_MAIN

daq_params      daqpar;

#else

extern daq_params      daqpar;

#endif

