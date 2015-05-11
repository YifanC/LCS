/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    softrealtime_endat.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    03.11.2009
*  \version $Revision: 1.2 $
* 
*  \brief   sample for soft realtime mode
*           with EnDat encoders
*  
*  Content:
*  Sample programm for the soft realtime mode of
*  the EIB. The program configures one axis
*  of the EIB for EnDat encoders and
*  enables the soft realtime mode. The EIB can
*  be triggered by the internal timer trigger or
*  by an external trigger signal.
*
-----------------------------------------------------*/


#include <eib7.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef Linux
#include <signal.h>
#endif




/* function declarations */
void CheckError(EIB7_ERR error);

/* global variable for console handler to stop on user request */
static int stop = 0;


#ifdef _WIN32

/* handler for console to catch user inputs */
BOOL CtrlHandler( DWORD fdwCtrlType )
{
   if(fdwCtrlType == CTRL_C_EVENT)
   {
      stop = TRUE;
      return TRUE;
   }

   return FALSE;
}
#endif

#ifdef Linux

/* handler for console to catch user inputs */
void CtrlHandler(int sig)
{
   if(sig==SIGINT)
     stop = 1;
}
#endif



/* Softrealtime demo program
   This program demonstrates the soft realtime mode of the EIB.
   The program initialises one axis of the EIB and reads the
   position data. The status word, the position value, the
   timestamp and the trigger counter are displayed.
   */
int main(int argc, char **argv)
{

   
   EIB7_MODE active;                        /* EIB741 mode for reference marks */
   EIB7_HANDLE eib;                       /* EIB handle                  */
   unsigned long ip;                      /* IP address of EIB           */
   unsigned long num;                     /* number of encoder axes      */
   unsigned long TimerTicks;              /* timer ticks per us          */
   unsigned long TimerPeriod;             /* timer trigger period        */
   unsigned long TimestampTicks;          /* ticks per us (timestamp)    */
   unsigned long TimestampPeriod;         /* period of timestamp counter */
   EIB7_AXIS axis[NUM_OF_AXIS];           /* axes array                  */
   EIB7_IO input[NUM_OF_IO];              /* IO array (input)            */
   EIB7_IO output[NUM_OF_IO];             /* IO array (output)           */
   EIB7_DataPacketSection packet[3];      /* Data packet configuration   */
   char fw_version[20];                   /* firmware version string     */
   char hostname[MAX_TEXT_LEN] = "192.168.1.2";           /* hostname string             */
   char TriggerConf[MAX_TEXT_LEN];        /* input string                */
   char RefRunConfig[MAX_TEXT_LEN];        /* input string                */
   int ExtTrigger;                        /* activate external trigger   */
   int RefRun;
   int enc_axis;                          /* actual axis index           */
   EIB7_DataRegion region;                /* actual region               */
   unsigned char udp_data[MAX_SRT_DATA];  /* buffer for udp data packet  */
   unsigned long entries;                 /* entries read from FIFO      */
   void *field;                           /* pointer to data field       */
   unsigned long sz;                      /* size of data field          */
   EIB7_ERR error;                        /* error code                  */

/* register console handler for program termination on user request */
#ifdef _WIN32
   SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );
#endif
#ifdef Linux
   signal(SIGINT, CtrlHandler);
   signal(SIGTERM, CtrlHandler);
#endif

   printf("Trying to reset EIB7");

   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);

   CheckError(EIB7Reset(eib));
   

   printf("\nHopefully the reset worked\n");

   exit(1);
}


/* check error code
   This function prints the error code an a brief description to the standard error console.
   The program will be terminated afterwards.
   */
void CheckError(EIB7_ERR error)
{
   if(error != EIB7_NoError)
   {
      char mnemonic[32];
      char message[256];

      EIB7GetErrorInfo(error, mnemonic, 32, message, 256);

      fprintf(stderr, "\nError %08x (%s): %s\n", error, mnemonic, message);
      exit(0);
   }
}
