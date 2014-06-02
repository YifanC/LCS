/*!------------------------------------------------
*
*  Laser position aquisition for MediumAT 
*
*  \file    daq_laser.c
*  \author  Matthias Luethi
*  \date    03.06.2013
*  \version $Revision: 0.1 $
* 
*  \brief   functions which manage the communication
*           between daq and the Heidenhain rotary 
*           encoder. Largely based on example
*           "softrealtime.c" from Heidenhain.
*       
*
-----------------------------------------------------*/

#include <eib7.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h> // for sleep()

#include <signal.h>

#include "daq_main.h"

/* definitions */
#define EIB_TCP_TIMEOUT   5000   /* timeout for TCP connection in ms      */
#define NUM_OF_AXIS       4      /* number of axes of the EIB             */
#define NUM_OF_IO         4      /* number of inputs and outputs          */
#define MAX_SRT_DATA      200    /* maximum size of soft realtime data    */
#define MAX_TEXT_LEN      200    /* maximum size of console input string  */
#define TIMESTAMP_PERIOD  1000   /* Timestamp Period = 1 ms = 1000us      */
#define TRIGGER_PERIOD    50000 /* Trigger Period = 0.5 sec = 500000us   */


#define POS_SPEC "Trg-Cntr: %05u, Timestamp: %010lu, Status: 0x%04X, Pos: %010lld"

// global variable
EIB7_HANDLE eib;

/* struct for soft realtime mode data */
struct EncData
{
   ENCODER_POSITION position;             /* position value              */
   unsigned short status;                 /* status word                 */
   unsigned short TriggerCounter;         /* trigger counter value       */
   unsigned long Timestamp;               /* timestamp                   */
   ENCODER_POSITION refc;                 /* coded reference position value  */
};

/* function declarations */
int  initEIB(void);                 // Init connection to EIB741
int  closeEIB(void);
void CheckError(EIB7_ERR error);
int readPos(ev_laser *);


/* global variable for console handler to stop on user request */
static int stop = 0;


/* handler for console to catch user inputs */
void CtrlHandler(int sig)
{
   if(sig==SIGINT)
     stop = 1;
}

// Initializes the EIB741 Controller for Axis1: Linear Encoder, Axis2: Rotary Encoder

int initEIB(void)
{
   EIB7_MODE RefActive;                        /* EIB741 mode for reference marks */
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
   char hostname[MAX_TEXT_LEN] = "192.168.1.2";  /* hostname string             */
   int ExtTrigger;                        /* activate external trigger   */
                                          // ext trigger on/off - if off use the internal timer as a trigger
   
   int LinearEncoder = 0;                 // linear encoder on X11
   int RotaryEncoder = 1;                 // rotary encoder on X12
   
   printf("\n");





   ExtTrigger = 1;
   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB741 firmware version: %s\n", fw_version);

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
   CheckError(EIB7AddDataPacketSection(packet, 1, EIB7_DR_Encoder2, (EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_DistCodedRef) ));
   CheckError(EIB7AddDataPacketSection(packet, 2, EIB7_DR_Encoder1, (EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp) ));
   CheckError(EIB7ConfigDataPacket(eib, packet, 3));

  
  /* set up trigger */
  CheckError(EIB7ResetTriggerCounter(eib));
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

   /* Do a reference movement for the rotarty encoder (get two reference marks) */
   CheckError(EIB7StartRef(axis[RotaryEncoder], 2)); // start waiting for 2 reference marks on the rotary axis  
   RefActive = 1;
   while(RefActive==1) {
      printf("waiting for reference marks   \r");
      fflush(stdout);
      sleep(1);
      printf("waiting for reference marks.  \r");
      fflush(stdout);
      sleep(1);
      printf("waiting for reference marks.. \r");
      fflush(stdout);
      sleep(1);
      printf("waiting for reference marks...\r");
      fflush(stdout);
      CheckError(EIB7GetRefActive(axis[RotaryEncoder], &RefActive));
   }


      if(ExtTrigger)
      {
			   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgInput1));
	    }
      else
      {
         CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgTimer));
      }
   return 1;
}


int  closeEIB()
{
   /* disable trigger */
   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Disable, EIB7_TS_All));

   /* disable SoftRealtime mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_Polling));

   EIB7Close(eib);
   printf("\nClose EIB741\n");
   return 0;
}

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

int readPos(ev_laser *PtrLaser)
{
   unsigned long entries;                 /* entries read from FIFO      */
   unsigned char udp_data[MAX_SRT_DATA];  /* buffer for udp data packet  */
   EIB7_ERR error;                        /* error code                  */
   void *field;                           /* pointer to data field       */
   unsigned long sz;                      /* size of data field          */
   struct EncData LinearEncoderData;            /* data to display             */
   struct EncData RotaryEncoderData;            /* data to display             */ 

   {
      /* run till the user press Ctrl-C */

      /* read data packet from FIFO */
      error = EIB7ReadFIFOData(eib, udp_data, 1, &entries, -1);
      if(error==EIB7_FIFOOverflow)
      {
         printf("FIFO Overflow error detected, clearing FIFO.\n");
         EIB7ClearFIFO(eib);
      }

      if(entries >= 1)
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

         /* print status word and position value */
         //printf("Linear Encoder Data: ");
         //printf(POS_SPEC, LinearEncoderData.TriggerCounter, LinearEncoderData.Timestamp,
         //                 LinearEncoderData.status, LinearEncoderData.position);
         //printf("\n");
         //printf("Rotary Encoder Data: ");
         //printf(POS_SPEC, RotaryEncoderData.TriggerCounter, RotaryEncoderData.Timestamp,
         //                 RotaryEncoderData.status, RotaryEncoderData.position);
         //printf("\n");

         

         // Write Date into struct
         PtrLaser->PosVert = LinearEncoderData.position;
         PtrLaser->TriggerStampVert = LinearEncoderData.TriggerCounter;
         PtrLaser->StatusVert = LinearEncoderData.status;
         PtrLaser->TimestampVert = LinearEncoderData.Timestamp;

         PtrLaser->PosHor = RotaryEncoderData.position;
         PtrLaser->PosRefHor =  RotaryEncoderData.refc;
         PtrLaser->TriggerStampHor = RotaryEncoderData.TriggerCounter;
         PtrLaser->StatusHor = RotaryEncoderData.status;
         PtrLaser->TimestampHor = RotaryEncoderData.Timestamp;

      }
      return 1;
   }
   return 0;
}









