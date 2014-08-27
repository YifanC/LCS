/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    pulscounter.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    03.11.2009
*  \version $Revision: 1.2 $
* 
*  \brief   sample for triggering with the puls counter
*  
*  Content:
*  Sample programm for the puls counter of
*  the EIB. The program configures one axis
*  of the EIB for incremental encoders and
*  enables the recording mode. The trigger source
*  is the internal timer trigger. The puls counter
*  limits the trigger pulses. The trigger sequence
*  can be started witht the software trigger.
*
-----------------------------------------------------*/


#include <eib7.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#ifdef Linux
#include <signal.h>
#endif


/* definitions */
#define EIB_TCP_TIMEOUT    5000   /* timeout for TCP connection in ms      */
#define NUM_OF_AXIS        4      /* number of axes of the EIB             */
#define NUM_OF_IO          4      /* number of inputs and outputs          */
#define MAX_SRT_DATA       200    /* maximum size of recording data        */
#define MAX_TEXT_LEN       200    /* maximum size of console input string  */
#define TIMESTAMP_PERIOD   1000   /* Timestamp Period = 1 ms = 1000us      */
#define TRIGGER_PERIOD     10000  /* Trigger Period = 0.01 sec = 10000us   */
#define TRIGGER_PULS_COUNT 1000   /* number of recording samples           */


/* struct for recording mode data */
struct EncData
{
   ENCODER_POSITION position;             /* position value              */
   unsigned short status;                 /* status word                 */
   unsigned short TriggerCounter;         /* trigger counter value       */
   unsigned long Timestamp;               /* timestamp                   */
};


/* function declarations */
void CheckError(EIB7_ERR error);



/* PulsCounter demo program
   This program demonstrates triggering with the puls counter
   The program initialises one axis of the EIB for incremental
   encoder. The trigger source is the internal timer trigger.
   The trigger pulses are limited by the PulsCounter.
   */
int main(int argc, char **argv)
{
   EIB7_HANDLE eib;                       /* EIB handle                  */
   unsigned long ip;                      /* IP address of EIB           */
   unsigned long num;                     /* number of encoder axes      */
   unsigned long TimerTicks;              /* timer ticks per us          */
   unsigned long TimerPeriod;             /* timer trigger period        */
   unsigned long TimestampTicks;          /* ticks per us (timestamp)    */
   unsigned long TimestampPeriod;         /* period of timestamp counter */
   EIB7_AXIS axis[NUM_OF_AXIS];           /* axes array                  */
   EIB7_DataPacketSection packet[2];      /* Data packet configuration   */
   char fw_version[20];                   /* firmware version string     */
   char hostname[MAX_TEXT_LEN];           /* hostname string             */
   int enc_axis;                          /* actual axis index           */
   int axis_num;                          /* selected axis               */
   unsigned long length;                  /* number of samples           */
   unsigned long recstatus;               /* status of recording mode    */
   unsigned long progress;                /* progress of recording mode  */
   EIB7_DataRegion region;                /* data packet region          */
   unsigned char udp_data[MAX_SRT_DATA];  /* buffer for udp data packet  */
   struct EncData EncoderData;            /* data to display             */
   unsigned long entries;                 /* entries read from FIFO      */
   void *field;                           /* pointer to data field       */
   unsigned long sz;                      /* size of data field          */
   EIB7_ERR error;                        /* error code                  */


   /* ask for parameters */
   printf("Please enter IP address or hostname: ");
   scanf("%s", hostname);
   printf("Please select encoder axis (1..4): ");
   scanf("%i", &axis_num);
   if((axis_num<1) || (axis_num>4))
   {
      printf("wrong axis number\n");
      exit(1);
   }
   enc_axis=axis_num-1;

   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);

   /* read axes array */
   CheckError(EIB7GetAxis(eib, axis, NUM_OF_AXIS, &num));

   /* init axis: 1 Vpp */
   printf("Initializing encoder %d for 1 Vpp\n", axis_num);
   CheckError(EIB7InitAxis(axis[enc_axis],
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
   
   TimestampPeriod = TIMESTAMP_PERIOD;
   CheckError(EIB7GetTimestampTicks(eib, &TimestampTicks));
   TimestampPeriod *= TimestampTicks;
   CheckError(EIB7SetTimestampPeriod(eib, TimestampPeriod));
   CheckError(EIB7SetTimestamp(axis[enc_axis], EIB7_MD_Enable));

   /* configure data packet */
   switch(axis_num)
   {
   case 1:
      region = EIB7_DR_Encoder1;
      break;
   case 2:
      region = EIB7_DR_Encoder2;
      break;
   case 3:
      region = EIB7_DR_Encoder3;
      break;
   case 4:
      region = EIB7_DR_Encoder4;
      break;
   }
   CheckError(EIB7AddDataPacketSection(packet, 0, EIB7_DR_Global, EIB7_PDF_TriggerCounter));
   CheckError(EIB7AddDataPacketSection(packet, 1, region, EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp));
   CheckError(EIB7ConfigDataPacket(eib, packet, 2));

   CheckError(EIB7AxisTriggerSource(axis[enc_axis], EIB7_AT_TrgPuls));
   CheckError(EIB7MasterTriggerSource(eib, EIB7_AT_TrgPuls));


   /* set up trigger */
   /* set timer trigger period */
   CheckError(EIB7GetTimerTriggerTicks(eib, &TimerTicks));
   TimerPeriod = TRIGGER_PERIOD;
   TimerPeriod *= TimerTicks;
   CheckError(EIB7SetTimerTriggerPeriod(eib, TimerPeriod));

   /* config the PulsCounter for 1000 pulses from the timer trigger */
   CheckError(EIB7ConfigPulsCounter(eib, EIB7_PS_TrgSW1, EIB7_PT_TrgTimer, TRIGGER_PULS_COUNT));

   /* enable Recording mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_RecordingRoll));

   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, (EIB7_TS_TrgTimer | EIB7_TS_TrgPuls)));

	printf("\nPress ENTER to start trigger.\n\n");
	getchar();     /* clear standard input stream */
	getchar();     /* wait for ENTER */
   
   CheckError(EIB7SoftwareTrigger(eib, EIB7_ST_SWtrigger1));
   printf("Trigger started...\n");

   do
   {
      CheckError(EIB7GetRecordingStatus(eib, &length, &recstatus, &progress));

#ifdef _WIN32          /* wait for 1000 ms to minmize processor load */
      Sleep(1000);
#else
      usleep(1000000);
#endif

      printf("%d Samples in memory\n", length);
   }
   while(length < TRIGGER_PULS_COUNT);


   printf("... finished            \n\n");
   printf("%d Samples in memory\n", length);



   /* disable trigger */
   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Disable, EIB7_TS_All));

   /* disable recording mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_Polling));

   /* start data transfer */
   CheckError(EIB7TransferRecordingData(eib, EIB7_MD_Enable, 0, 0xFFFFFFFF));

   entries = 1;

   while(entries > 0)
   {
      /* read data packet from FIFO */
      error = EIB7ReadFIFOData(eib, udp_data, 1, &entries, 100);
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

         /* read timestamp from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, enc_axis + 1,
                    EIB7_PDF_Timestamp, &field, &sz));
         EncoderData.Timestamp = *(unsigned long *)field;

         /* read position value from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, enc_axis + 1,
                    EIB7_PDF_PositionData, &field, &sz));
         EncoderData.position = *(ENCODER_POSITION *)field;

         /* read status word from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, enc_axis + 1,
                    EIB7_PDF_StatusWord, &field, &sz));
         EncoderData.status = *(unsigned short *)field;

      }

   } /* end of loop */

   /* stop data transfer in any case */
   CheckError(EIB7TransferRecordingData(eib, EIB7_MD_Disable, 0, 0xFFFFFFFF));


   /* close connection to EIB */
   EIB7Close(eib);

	printf("\nPress ENTER to continue.");
	getchar();     /* clear standard input stream */

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
