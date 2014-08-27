/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    intervalcounter.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    03.11.2009
*  \version $Revision: 1.3 $
* 
*  \brief   sample for the interval counter
*  
*  Content:
*  Sample programm for the interval counter of
*  the EIB. The encoder on axis 1 generates 
*  trigger pulses for axis 2. The position value
*  of the auxiliary axis and axis 2 will be 
*  displayed.
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

#ifdef _WIN32
#define POS_SPEC " %05u Axis2: 0x%04X, %010I64d  -  AuxAxis: 0x%04X, %010I64d"
#else
#define POS_SPEC  " %05u Axis2: 0x%04X, %010lld  -  AuxAxis: 0x%04X, %010lld"
#endif


/* definitions */
#define EIB_TCP_TIMEOUT   5000   /* timeout for TCP connection in ms                      */
#define NUM_OF_AXIS       4      /* number of axes of the EIB                             */
#define MAX_SRT_DATA      200    /* maximum size of soft realtime data                    */
#define MAX_TEXT_LEN      200    /* maximum size of console input string                  */
#define TIMESTAMP_PERIOD  1000   /* Timestamp Period = 1 ms = 1000us                      */
#define TRIGGER_PERIOD    5000   /* Trigger Period in counting steps of auxiliary axis    */


/* struct for soft realtime mode data */
struct EncData
{
   ENCODER_POSITION position;             /* position value              */
   unsigned short status;                 /* status word                 */
   unsigned short TriggerCounter;         /* trigger counter value       */
   unsigned long Timestamp;               /* timestamp                   */
   ENCODER_POSITION ref[2];               /* reference position values   */
};


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



/* IntervalCounter demo program
   This program demonstrates the IntervalCounter of the EIB.
   The program initialises all axes for incremental encoders.
   The position value of the auxiliary axis and axis 2 will
   be displayed.
   */
int main(int argc, char **argv)
{
   EIB7_HANDLE eib;                       /* EIB handle                  */
   unsigned long ip;                      /* IP address of EIB           */
   unsigned long num;                     /* number of encoder axes      */
   unsigned long TimestampTicks;          /* ticks per us (timestamp)    */
   unsigned long TimestampPeriod;         /* period of timestamp counter */
   EIB7_AXIS axis[NUM_OF_AXIS];           /* axes array                  */
   EIB7_DataPacketSection packet[5];      /* Data packet configuration   */
   char fw_version[20];                   /* firmware version string     */
   char hostname[MAX_TEXT_LEN];           /* hostname string             */
   unsigned char udp_data[MAX_SRT_DATA];  /* buffer for udp data packet  */
   struct EncData EncoderData;            /* data for axis               */
   struct EncData AuxData;                /* data for auxiliary axis     */
   unsigned long entries;                 /* entries read from FIFO      */
   void *field;                           /* pointer to data field       */
   unsigned long sz;                      /* size of data field          */
   EIB7_ERR error;                        /* error code                  */
   int i;


/* register console handler for program termination on user request */
#ifdef _WIN32
   SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );
#endif
#ifdef Linux
   signal(SIGINT, CtrlHandler);
   signal(SIGTERM, CtrlHandler);
#endif


   /* ask for parameters */
   printf("Please enter IP address or hostname: ");
   scanf("%s", hostname);

   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);

   /* read axes array */
   CheckError(EIB7GetAxis(eib, axis, NUM_OF_AXIS, &num));

   for(i=0; i<NUM_OF_AXIS; i++)
   {
      /* init axis: 1 Vpp */
      printf("Initializing encoder %d for 1 Vpp\n", i+1);
      CheckError(EIB7InitAxis(axis[i],
                 EIB7_IT_Incremental,
                 EIB7_EC_Linear,
                 EIB7_RM_None,         /* reference marks not used */
                 0,                    /* reference marks not used */
                 0,                    /* reference marks not used */
                 EIB7_HS_None,
                 EIB7_LS_None,
                 EIB7_CS_CompActive,   /* signal compensation on   */
                 EIB7_BW_High,         /* signal bandwidth: high   */
                 EIB7_CLK_Default,     /* not used for incremental interface */
                 EIB7_RT_Long,         /* not used for incremental interface */
                 EIB7_CT_Long          /* not used for incremental interface */
               ));

      CheckError(EIB7SetTimestamp(axis[i], EIB7_MD_Enable));
      CheckError(EIB7AxisTriggerSource(axis[i], EIB7_AT_TrgIC));
   }
      
   TimestampPeriod = TIMESTAMP_PERIOD;
   CheckError(EIB7GetTimestampTicks(eib, &TimestampTicks));
   TimestampPeriod *= TimestampTicks;
   CheckError(EIB7SetTimestampPeriod(eib, TimestampPeriod));

   /* configure data packet */
   CheckError(EIB7AddDataPacketSection(packet, 0, EIB7_DR_Global, EIB7_PDF_TriggerCounter));
   CheckError(EIB7AddDataPacketSection(packet, 1, EIB7_DR_Encoder2, EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp));
   CheckError(EIB7AddDataPacketSection(packet, 2, EIB7_DR_AUX, EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp));
   CheckError(EIB7ConfigDataPacket(eib, packet, 3));

   /* set up trigger */
   CheckError(EIB7AuxAxisTriggerSource(eib, EIB7_AT_TrgIC));
   CheckError(EIB7MasterTriggerSource(eib, EIB7_AT_TrgIC));
   CheckError(EIB7SetIntervalCounterInterpolation(eib, EIB7_ICF_10x, EIB7_ICE_4x));
   CheckError(EIB7SetIntervalCounterTrigger(eib, EIB7_ICM_Periodic, EIB7_ICS_Current, 0, TRIGGER_PERIOD));

   /* enable Streaming mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_Streaming));

   /* enable trigger */
   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgIC));

   printf("\n\nMove encoder on axis 1 to generate trigger pulses\n\n");
   printf("\npress Ctrl-C to stop\n\n");

   while(!stop)
   {
      /* run till the user press Ctrl-C */

      /* read data packet from FIFO */
      error = EIB7ReadFIFOData(eib, udp_data, 1, &entries, 0);
      if(error==EIB7_FIFOOverflow)
      {
         printf("FIFO Overflow error detected, clearing FIFO.\n");
         EIB7ClearFIFO(eib);
      }

      if(entries > 0)
      {
         /* read trigger counter from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Global, 
                    EIB7_PDF_TriggerCounter, &field, &sz));
         EncoderData.TriggerCounter = *(unsigned short *)field;

         /* read timestamp for axis 2 from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder2,
                    EIB7_PDF_Timestamp, &field, &sz));
         EncoderData.Timestamp = *(unsigned long *)field;

         /* read position value for axis 2 from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder2,
                    EIB7_PDF_PositionData, &field, &sz));
         EncoderData.position = *(ENCODER_POSITION *)field;

         /* read status word for axis 2 from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder2,
                    EIB7_PDF_StatusWord, &field, &sz));
         EncoderData.status = *(unsigned short *)field;

         /* read timestamp for auxiliary axis from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_AUX,
                    EIB7_PDF_Timestamp, &field, &sz));
         AuxData.Timestamp = *(unsigned long *)field;

         /* read position value for auxiliary axis from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_AUX,
                    EIB7_PDF_PositionData, &field, &sz));
         AuxData.position = *(ENCODER_POSITION *)field;

         /* read status word for auxiliary axis from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_AUX,
                    EIB7_PDF_StatusWord, &field, &sz));
         AuxData.status = *(unsigned short *)field;

         /* print status word and position value */
         printf(POS_SPEC, EncoderData.TriggerCounter,
                          EncoderData.status, EncoderData.position,
                          AuxData.status, AuxData.position);
         printf("\n");
      }

#ifdef _WIN32          /* wait for 50 ms to minmize processor load */
      Sleep(10);
#else
      usleep(50000);
#endif

   } /* end of loop */

   /* disable trigger */
   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Disable, EIB7_TS_All));
   CheckError(EIB7SetIntervalCounterTrigger(eib, EIB7_ICM_Disable, EIB7_ICS_Current, 0, 0));

   /* disable SoftRealtime mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_Polling));

   /* close connection to EIB */
   EIB7Close(eib);

   printf("\nStopped on user request\n");

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
