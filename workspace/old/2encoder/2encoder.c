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

#ifdef _WIN32
#define POS_SPEC "Trg-Cntr: %05u, Timestamp: %010lu, Status: 0x%04X, Pos: %010I64d"
#else
#define POS_SPEC "Trg-Cntr: %05u, Timestamp: %010lu, Status: 0x%04X, Pos[deg]: %f"
#endif


/* definitions */
#define EIB_TCP_TIMEOUT   5000   /* timeout for TCP connection in ms      */
#define NUM_OF_AXIS       4      /* number of axes of the EIB             */
#define NUM_OF_IO         4      /* number of inputs and outputs          */
#define MAX_SRT_DATA      200    /* maximum size of recording data    */
#define MAX_TEXT_LEN      200    /* maximum size of console input string  */
#define TIMESTAMP_PERIOD  1000   /* Timestamp Period = 1 ms = 1000us      */
#define TRIGGER_PERIOD    500000 /* Trigger Period = 0.5 sec = 500000us   */


/* struct for soft realtime mode data */
struct EncData
{
   ENCODER_POSITION position;             /* position value              */
   unsigned short status;                 /* status word                 */
   unsigned short TriggerCounter;         /* trigger counter value       */
   unsigned long Timestamp;               /* timestamp                   */
   ENCODER_POSITION refc;             /* position value              */   
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



/* Softrealtime demo program
   This program demonstrates the soft realtime mode of the EIB.
   The program initialises one axis of the EIB and reads the
   position data. The status word, the position value, the
   timestamp and the trigger counter are displayed.
   */
int main(int argc, char **argv)
{
   // Definitions MLuethi
   float RotaryPosDeg;
   float LinearPosDeg;
   ENCODER_POSITION RotaryCounts;
   ENCODER_POSITION RotaryInterp;
   ENCODER_POSITION LinearCounts;
   ENCODER_POSITION LinearCounts45deg = 13404540;
   
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


   //-------------------------------------------------------------------.
   struct EncData LinearEncoderData;            /* data to display             */
   struct EncData RotaryEncoderData;            /* data to display             */
   int LinearEncoder = 0;
   int RotaryEncoder = 1;
   ExtTrigger = 1;
   //-------------------------------------------------------------------

/* register console handler for program termination on user request */
#ifdef _WIN32
   SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );
#endif
#ifdef Linux
   signal(SIGINT, CtrlHandler);
   signal(SIGTERM, CtrlHandler);
#endif



   printf("use external trigger (y/n)? ");
   scanf("%s",TriggerConf);
   ExtTrigger = 0;
   if(TriggerConf[0]=='y' || TriggerConf[0]=='Y')
   {
      ExtTrigger = 1;
   }

   printf("do reference run (y/n)? ");
   scanf("%s",RefRunConfig);
   RefRun = 0;
   if(RefRunConfig[0]=='y' || RefRunConfig[0]=='Y')
   {
      RefRun = 1;
   }
   
   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);

   /* read axes array */
   CheckError(EIB7GetAxis(eib, axis, NUM_OF_AXIS, &num));

   /* initialize axis 1 (linear encoder) for EnDat 2.2*/
   fprintf(stderr, "Initializing axis %d for EnDat 2.2\n", LinearEncoder+1);
   CheckError(EIB7InitAxis(axis[LinearEncoder],
   /* with EnDat 2.2 we enable the EnDat propagation time compensation */
      EIB7_IT_EnDat22 ,//| EIB7_IT_EnDatDelayMeasurement,
      EIB7_EC_Linear,
      EIB7_RM_None,
      0,                    /* not used for EnDat */
      0,                    /* not used for EnDat */
      EIB7_HS_None,
      EIB7_LS_None,
      EIB7_CS_None,         /* not used for EnDat */
      EIB7_BW_High,         /* not used for EnDat */
      EIB7_CLK_Default,     /* we use the default clock */
      EIB7_RT_Long,         /* long EnDat recovery time I */
      EIB7_CT_Long          /* encoder with long calculation timeout */
   ));

   /* initialize axis 2 (rotary encoder) for 1 Vpp*/
   fprintf(stderr, "Initializing axis %d for 1 Vpp\n", RotaryEncoder+1);
   CheckError(EIB7InitAxis(axis[RotaryEncoder],
              EIB7_IT_Incremental,
              EIB7_EC_Rotary,
              EIB7_RM_DistanceCoded,         /* reference marks not used */
              20000,                    /* reference marks not used */
              1000,                    /* reference marks not used */
              EIB7_HS_None,
              EIB7_LS_None,
              EIB7_CS_CompActive,   /* signal compensation on   */
              EIB7_BW_High,         /* signal bandwidth: high   */
              EIB7_CLK_Default,     /* not used for incremental interface */
              EIB7_RT_Long,         /* not used for incremental interface */
              EIB7_CT_Long          /* not used for incremental interface */
              ));

   // Configure Timestamp
   TimestampPeriod = TIMESTAMP_PERIOD;
   CheckError(EIB7GetTimestampTicks(eib, &TimestampTicks));
   TimestampPeriod *= TimestampTicks;
   CheckError(EIB7SetTimestampPeriod(eib, TimestampPeriod));
   CheckError(EIB7SetTimestamp(axis[LinearEncoder], EIB7_MD_Enable));
   CheckError(EIB7SetTimestamp(axis[RotaryEncoder], EIB7_MD_Enable));
   
   /* configure data packet */
   CheckError(EIB7AddDataPacketSection(packet, 0, EIB7_DR_Global, EIB7_PDF_TriggerCounter));
   CheckError(EIB7AddDataPacketSection(packet, 1, EIB7_DR_Encoder2, (EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_ReferencePos | EIB7_PDF_DistCodedRef)));
   CheckError(EIB7AddDataPacketSection(packet, 2, EIB7_DR_Encoder1, (EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp)));
   CheckError(EIB7ConfigDataPacket(eib, packet, 3));
   


   /* set up trigger */
   if(ExtTrigger)
   {
      /* enable external trigger */
      unsigned long ilen, olen;

      printf("using external trigger\n");
      /* get IO handles for configuration */
      CheckError(EIB7GetIO(eib, input, NUM_OF_IO, &ilen, output, NUM_OF_IO, &olen));
      /* enable trigger input for axis 1, enable termination resistor */
      CheckError(EIB7InitInput(input[0], EIB7_IOM_Trigger, EIB7_MD_Enable));

      CheckError(EIB7AxisTriggerSource(axis[LinearEncoder], EIB7_AT_TrgInput1));
      CheckError(EIB7AxisTriggerSource(axis[RotaryEncoder], EIB7_AT_TrgInput1));
      CheckError(EIB7MasterTriggerSource(eib, EIB7_AT_TrgInput1));
   }
   else
   {
      /* enable internal trigger */
      printf("using internal timer trigger\n");
      /* set timer trigger period */
      CheckError(EIB7GetTimerTriggerTicks(eib, &TimerTicks));
      TimerPeriod = TRIGGER_PERIOD;
      TimerPeriod *= TimerTicks;
      CheckError(EIB7SetTimerTriggerPeriod(eib, TimerPeriod));
      
      CheckError(EIB7AxisTriggerSource(axis[LinearEncoder], EIB7_AT_TrgTimer));
      CheckError(EIB7AxisTriggerSource(axis[RotaryEncoder], EIB7_AT_TrgTimer));
      CheckError(EIB7MasterTriggerSource(eib, EIB7_AT_TrgTimer));
   }

   /* enable SoftRealtime mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_SoftRealtime));
   
   if (RefRun == 1) {
      /* Do a reference movement for the rotarty encoder (get two reference marks) */
      CheckError(EIB7StartRef(axis[RotaryEncoder], EIB7_RP_RefPos2)); // start waiting for 2 reference marks on the rotary axis
      active = 1;   
      while(active==1) {
         CheckError(EIB7GetRefActive(axis[RotaryEncoder], &active));
         printf("waiting for reference run...\r");
      }
   }

   if(ExtTrigger)
   {
		CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgInput1));
	 }
   else
   {
      CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgTimer));
   }
   

   printf("\n\npress Ctrl-C to stop\n\n");

   while(!stop)
   {
      /* run till the user press Ctrl-C */

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
         LinearEncoderData.TriggerCounter = *(unsigned short *)field;

         // read data from linear encoder (EIB7_DR_Encoder1)
         /* read timestamp from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder1,
                    EIB7_PDF_Timestamp, &field, &sz));
         LinearEncoderData.Timestamp = *(unsigned int  *)field;

         /* read position value from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder1,
                    EIB7_PDF_PositionData, &field, &sz));
         LinearEncoderData.position = *(ENCODER_POSITION *)field;

         /* read status word from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder1,
                    EIB7_PDF_StatusWord, &field, &sz));
         LinearEncoderData.status = *(unsigned short *)field;

         // read data from roatary encoder (EIB7_DR_Encoder2)
         /* read trigger counter from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Global,
                    EIB7_PDF_TriggerCounter, &field, &sz));
         RotaryEncoderData.TriggerCounter = *(unsigned short *)field;

         /* read timestamp from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder2,
                    EIB7_PDF_Timestamp, &field, &sz));
         RotaryEncoderData.Timestamp = *(unsigned int *)field;

         /* read position value from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder2,
                    EIB7_PDF_PositionData, &field, &sz));
         RotaryEncoderData.position = *(ENCODER_POSITION *)field;

         /* read coded reference position value from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder2,
                    EIB7_PDF_DistCodedRef, &field, &sz));
         RotaryEncoderData.refc = *(ENCODER_POSITION *)field;


         /* read status word from data packet */
         CheckError(EIB7GetDataFieldPtr(eib, udp_data, EIB7_DR_Encoder2,
                    EIB7_PDF_StatusWord, &field, &sz));
         RotaryEncoderData.status = *(unsigned short *)field;

         /* calculate rotary position */
         RotaryCounts = (RotaryEncoderData.position-RotaryEncoderData.refc)>>12;
         RotaryInterp = RotaryEncoderData.position & 0xFFF;
         RotaryPosDeg = (float) RotaryCounts*360/20000;  
		 
         /* calculate vertical position */
         LinearCounts = LinearEncoderData.position  - 7728478;
         LinearPosDeg = (float) LinearCounts/69994.7111;

         /* print status word and position value */
		 
		 printf("Linear Encoder Data: ");
         printf(POS_SPEC, LinearEncoderData.TriggerCounter, LinearEncoderData.Timestamp,
                          LinearEncoderData.status, LinearPosDeg);
         printf("\n");
         printf("Rotary Encoder Data: ");
         printf(POS_SPEC, RotaryEncoderData.TriggerCounter, RotaryEncoderData.Timestamp,
                          RotaryEncoderData.status, RotaryPosDeg);
         printf("\n");

                  
         
         FILE * pFile;
         pFile = fopen ("laser.scad","w");
         if (pFile!=NULL)
         {
            fprintf(pFile,"include <TPC.scad>\nlaser(%f,%f);\n",RotaryPosDeg,LinearPosDeg);
            fclose (pFile);
         }



      }
#ifdef _WIN32          /* wait for 50 ms to minmize processor load */
      Sleep(50);
#else
      usleep(50);
#endif

   } /* end of loop */

   /* disable trigger */
   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Disable, EIB7_TS_All));

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
