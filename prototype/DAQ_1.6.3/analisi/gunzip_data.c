#include <stdio.h>
#include <stdlib.h>

#include "gziplib.h"

/* from daq_main.h */

typedef struct {
  unsigned short    header_size;
  unsigned short    header_version;
  unsigned long     run_number;
  unsigned long     date_time;
  unsigned short    daq_version;
  unsigned short    hw_setup_id;
  unsigned long     number_of_events;
} run_header;

typedef struct {
  unsigned short header_size;
  unsigned short header_version;
  unsigned long event_number;
  unsigned long event_time;
  unsigned long number_of_pmt;
  unsigned long event_size;
} event_header;


#define RUN_HEADER_SIZE  5  // in longwords
#define EV_HEADER_SIZE   5  // in longwords

int main(void) {

  char buffer[1000000];
  unsigned long event_index[100000];
  unsigned long out[1000000];
  unsigned long magic;

  int rh_size;
  int eh_size;
  int iev, ret, no, nbout;
  int isample,ipmt;
  unsigned short high,low;
  unsigned short c1,c2;

  FILE *fin;

  run_header   rh;
  event_header eh;

  rh_size = sizeof(rh);
  eh_size = sizeof(eh);

  no = 1000000;

  gunzip_init();
  
  fin = fopen("ziptest.dat","r");
  if(fin == NULL) {printf("Error opening input file !! \n"); return -1;}

  ret = fread(&magic, 4, 1, fin);
  if(ret != 1) {printf("Error reading magic !! \n"); return -1;}

  ret = fread(&rh, rh_size, 1, fin);
  if(ret != 1) {printf("Error reading run header !! \n"); return -1;}
 
  printf("magic =  %.4lx \n",      magic);
 
  printf("rh.header_size =  %u \n",      rh.header_size);
  printf("rh.header_version =  %u \n",   rh.header_version);
  printf("rh.run_number =  %lu \n",       rh.run_number);
  printf("rh.date_time =  %lu \n",        rh.date_time);
  printf("rh.daq_version =  %u \n",      rh.daq_version);
  printf("rh.hw_setup_id =  %u \n",      rh.hw_setup_id);
  printf("rh.number_of_events =  %lu \n", rh.number_of_events);

  if(0) {

    fseek(fin, -rh.number_of_events*sizeof(event_index[0]), SEEK_END);
    fread(event_index, sizeof(event_index[0])*rh.number_of_events, 1, fin);

    fseek(fin, event_index[5], SEEK_SET);

    ret = fread(&eh, eh_size, 1, fin);
    if(ret != 1) {printf("Error reading event header for event %i \n", iev); return -1;}

    printf("eh.header_size =  %u \n",    eh.header_size);
    printf("eh.header_version =  %u \n", eh.header_version);
    printf("eh.event_number =  %lu \n",   eh.event_number);
    printf("eh.event_time =  %lu \n",     eh.event_time);
    printf("eh.number_of_pmt =  %lu \n",  eh.number_of_pmt);
    printf("eh.event_size =  %lu \n",     eh.event_size);

    ret = fread(buffer, eh.event_size, 1, fin);
    if(ret != 1) {printf("Error reading event %i \n", iev); return -1;}

    nbout = gunzip_data(eh.event_size, &buffer[0], no, (unsigned char*) out);

    for(ipmt=0; ipmt<eh.number_of_pmt; ipmt++) {
      for(isample=0; isample<8192; isample++) {
	low  = (out[isample+ipmt*8192] & 0xFFFF0000) >> 16;
	high =  out[isample+ipmt*8192] & 0x0000FFFF;
	c1 = low  >> 2;
	c2 = high >> 2;
	printf(" %d  %d  %d  %d \n ", iev, ipmt, isample*2  , c1);
	printf(" %d  %d  %d  %d \n ", iev, ipmt, isample*2+1, c2);
      }
    }  

    printf("  ------->  %ld \n", out[15]);

    printf(" nbytes out = %d \n", nbout);

  } else {

  for(iev=0; iev<rh.number_of_events; iev++) 
    {

      ret = fread(&eh, eh_size, 1, fin);
      if(ret != 1) {printf("Error reading event header for event %i \n", iev); return -1;}
      
      printf("eh.header_size =  %u \n",    eh.header_size);
      printf("eh.header_version =  %u \n", eh.header_version);
      printf("eh.event_number =  %lu \n",   eh.event_number);
      printf("eh.event_time =  %lu \n",     eh.event_time);
      printf("eh.number_of_pmt =  %lu \n",  eh.number_of_pmt);
      printf("eh.event_size =  %lu \n",     eh.event_size);
      

      ret = fread(buffer, eh.event_size, 1, fin);
      if(ret != 1) {printf("Error reading event %i \n", iev); return -1;}
      
      nbout = gunzip_data(eh.event_size, &buffer[0], no, (unsigned char*) out);
    
      for(ipmt=0; ipmt<eh.number_of_pmt; ipmt++) {
	for(isample=0; isample<8192; isample++) {
	  low  = (out[isample+ipmt*8192] & 0xFFFF0000) >> 16;
	  high =  out[isample+ipmt*8192] & 0x0000FFFF;
	  c1 = low  >> 2;
	  c2 = high >> 2;
	  printf(" %d  %d  %d  %d \n ", iev, ipmt, isample*2  , c1);
	  printf(" %d  %d  %d  %d \n ", iev, ipmt, isample*2+1, c2);
	}
      }  
    }
  } 

  gunzip_end();
  
  return 0;
  
}

