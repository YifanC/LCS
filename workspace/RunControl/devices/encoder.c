/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    encoder.c
*  \author  Matthias Luethi
*  \date    01.09.2014
*  \version $Revision: 0.1
* 
*  \brief   Software to control the EIB741 Encoder in
*			for the LCS system 
*  
*  Content:
*	This code configures the encoder controller in 
*	specified ways. One can send the data to the 
*	laser data assembler via zmq-messages or use it 
*	to debug the communications. Run it as:
*	
*	$ ./encoder -configuration via commandline
*
*	with flags:
*	 -p 			print position values
*	 -ps 			print triggerstamp and timestamp
*	 -t TIME_US		use internal trigger with a trigger
*					time difference of TIME_US
*	 -s 			initialize the zmq server    
*	 -r 			do a reference run
-----------------------------------------------------*/

// includes for encoder control
#include <eib7.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>


// includes for zmq messaging
#include <assert.h>
#include <zmq.h>

// includes for timing
#include <sys/time.h> 
#include <time.h> 
#include <unistd.h> 


#ifdef _WIN32
#include <windows.h>
#endif

#ifdef Linux
#include <signal.h>
#endif

#ifdef _WIN32
#define POS_SPEC "Trg-Cntr: %05u, Timestamp: %010lu, Status: 0x%04X, Pos: %010I64d"
#else
#define POS_SPEC_COUNTS "Trg-Cntr: %05u, Timestamp: %010lu, Status: 0x%04X, Pos: %010lld"
#define POS_SPEC_DEG "Trg-Cntr: %05u, Timestamp: %010lu, Status: 0x%04X, Pos[deg]: %f"
#endif


/* definitions */
#define EIB_TCP_TIMEOUT   5000   /* timeout for TCP connection in ms      */
#define NUM_OF_AXIS       2      /* number of axes of the EIB             */
#define NUM_OF_IO         4      /* number of inputs and outputs          */
#define MAX_SRT_DATA      200    /* maximum size of recording data    */
#define MAX_TEXT_LEN      200    /* maximum size of console input string  */
#define TIMESTAMP_PERIOD  1000   /* Timestamp Period = 1 ms = 1000us      */


/* struct for soft realtime mode data */
struct EncData
{
   ENCODER_POSITION position;             /* position value              */
   unsigned short status;                 /* status word                 */
   unsigned short TriggerCounter;         /* trigger counter value       */
   unsigned long Timestamp;               /* timestamp                   */
   ENCODER_POSITION refc;             /* position value              */   
};

// Structure of the data sent to the laser data assembler

struct LaserEvent {
	float SystemTime_sec;		// epoch time in seconds
	float SystemTime_usec;		// This is the rest of the elapsed time
	float RotaryPosDeg;          //Position Rotary Encoder 
	float LinearPosDeg;          //Position Linear Encoder 
	float TriggerCount;    //Trigger Counter by Heidenhain Encoder
};

struct EncoderInfo {
    unsigned int ID;
    unsigned int Status;
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
int main(int argc, char *argv[])
{
   // Definitions MLuethi
   float RotaryPosDeg;
   float LinearPosDeg;
   ENCODER_POSITION RotaryCounts;
   ENCODER_POSITION RotaryInterp;
   ENCODER_POSITION LinearCounts;
   ENCODER_POSITION LinearCounts45deg = 13404540;
   
   EIB7_MODE active;                      /* EIB741 mode for reference marks */
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
   char DataConf[MAX_TEXT_LEN];			  /* input string                */
   char TriggerConf[MAX_TEXT_LEN];        /* input string                */
   char RefRunConfig[MAX_TEXT_LEN];       /* input string                */

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
   // Initialize the laser data evet
   struct LaserEvent EventData;

   EventData.SystemTime_sec = -1;
   EventData.SystemTime_usec = -1;
   EventData.RotaryPosDeg = -9999.0;
   EventData.LinearPosDeg = -9999.0;
   EventData.TriggerCount = -1.;

   struct EncoderInfo EncoderInfo;
   EncoderInfo.ID = 2;
   EncoderInfo.Status = -1;

   // initialize buffers 
   unsigned char BufferInfo[ sizeof (EncoderInfo) ];
   memcpy(&BufferInfo, &EncoderInfo, sizeof(EncoderInfo));

   unsigned char BufferData[ sizeof (EventData) ];
   memcpy(&BufferData,&EventData, sizeof(EventData));

   char BufferReply [2];

   void *context;
   void *encoder; 
   
   struct timeval SystemTime;
   //-------------------------------------------------------------------

	// Configuration init
   int ExtTrigger = 1;                        /* activate external trigger   */
   int RefRun = 0;
   int SendData = 0;
   int PrintData = 0;
   int PrintTimeOnly = 0;
   int TriggerPeriod = 100;	/* Trigger Period in ms  */

 /* register console handler for program termination on user request */
#ifdef _WIN32
   SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );
#endif
#ifdef Linux
   signal(SIGINT, CtrlHandler);
   signal(SIGTERM, CtrlHandler);
#endif

   // go into command line mode 
   if (argc <= 1)
   {
	   // Init Socket to talk to client

	   PrintData = 1;  
	   printf("Send data to  (y/n)? ");
	   scanf("%s",DataConf);
	   if(DataConf[0]=='y' || DataConf[0]=='Y')
	   {
	      SendData = 1;
	   }

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
	}
	// check the arguments
	else
	{
   		int opt;
		while ((opt = getopt(argc, argv, "sp::t:r")) != -1)
		{
			switch(opt) {
				case 's':
					printf("Will send zmq messages\n");
					SendData = 1;
					break;
				case 'p':
					
					if (optarg == NULL) 
					{
						printf("Will print position readings long\n");
						PrintData = 1;
						PrintTimeOnly = 0;
						break;
					}
					if (!strcmp("s",optarg))
					{
						printf("Will print position readings short\n");
						PrintData = 0;
						PrintTimeOnly = 1;
						break;
					}
				case 't':
					ExtTrigger = 0;
					TriggerPeriod = atoi(optarg);
					printf("Using internal Trigger with a period of %ims\n",TriggerPeriod);
					break;
				case 'r':
					RefRun = 1;
					break;
			}
			
		}
	}


	// Initializing the zmq connection if required
	if(SendData == 1)
	   {
		   context = zmq_ctx_new ();
		   encoder = zmq_socket (context, ZMQ_REQ);
		   int rc = zmq_connect (encoder, "ipc:///tmp/laser-in.ipc");
		   assert (rc == 0);
		   // Send initial empty message to assembler so he knows we are alive
		   printf("Sending Hello to assembler\n");
		   zmq_send (encoder,&BufferInfo, sizeof(BufferInfo) , ZMQ_SNDMORE);
		   zmq_send (encoder, &BufferData, sizeof(BufferData), 0);
		   zmq_recv (encoder, BufferReply, 2, 0);

		   printf ("Received:%c \n", BufferReply[1]);
		  
		   // sleep for some time before continue, remove before operation!!!
		   usleep(1000);
	   }


   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);



   /* read axes array */
   CheckError(EIB7GetAxis(eib, axis, NUM_OF_AXIS, &num));

   CheckError(EIB7ResetTriggerCounter(eib));

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
      TimerPeriod = TriggerPeriod*1000;
      TimerPeriod *= TimerTicks;
      CheckError(EIB7SetTimerTriggerPeriod(eib, TimerPeriod));
      
      CheckError(EIB7AxisTriggerSource(axis[LinearEncoder], EIB7_AT_TrgTimer));
      CheckError(EIB7AxisTriggerSource(axis[RotaryEncoder], EIB7_AT_TrgTimer));
      CheckError(EIB7MasterTriggerSource(eib, EIB7_AT_TrgTimer));
   }

   /* enable SoftRealtime mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_SoftRealtime));
   


   printf("\n\npress Ctrl-C to stop\n\n");

   if (RefRun == 1) {
      CheckError(EIB7ClearRefStatus(axis[RotaryEncoder]));

      /* Do a reference movement for the rotarty encoder (get two reference marks) */
      CheckError(EIB7StartRef(axis[RotaryEncoder], EIB7_RP_RefPos2)); // start waiting for 2 reference marks on the rotary axis
      active = 1;   
      while(active==1) {
         CheckError(EIB7GetRefActive(axis[RotaryEncoder], &active));
	 if (stop == 1){
		CheckError(EIB7StopRef(axis[RotaryEncoder]));
		break;
	}
         printf("waiting for reference run...\n");
	 usleep(100000);
      }
   }

   //CheckError(EIB7ResetTriggerCounter(axis[RotaryEncoder]));

   if(ExtTrigger)
   {
		CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgInput1));
	 }
   else
   {
      CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgTimer));
   }
   

   //CheckError(EIB7ResetTriggerCounter(axis[RotaryEncoder]));


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

      	 /* Get current system time of this trigger event*/
         gettimeofday (&SystemTime, NULL);

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
         EventData.RotaryPosDeg = ((float) RotaryCounts )*360/20000;  
		 
         /* calculate vertical position */
         LinearCounts = LinearEncoderData.position  - 7728478;
	 EventData.LinearPosDeg = (float) LinearEncoderData.position;
         //EventData.LinearPosDeg = ((float) LinearCounts) /69994.7111;

	     /* Add time to the struct which is sent to the server */
   	     //EventData.SystemTime_sec = (float) (SystemTime.tv_sec - 1431636031);
   	     EventData.SystemTime_sec = (float) (SystemTime.tv_sec - 1471533965);

   	     EventData.SystemTime_usec = (float) SystemTime.tv_usec;
		 // Check if trigger counters are equal as they should
         if (LinearEncoderData.TriggerCounter != RotaryEncoderData.TriggerCounter) {
         	printf("Trigger counts of the two encoders are different, something is wrong here!\n");
         	printf("Linear trigger count: %05u \n",LinearEncoderData.TriggerCounter);
         	printf("Rotary trigger count: %05u \n",RotaryEncoderData.TriggerCounter);
         	printf("Therefore I exit");
         	
         	/* disable trigger */
   			CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Disable, EIB7_TS_All));

   			/* disable SoftRealtime mode */
   			CheckError(EIB7SelectMode(eib, EIB7_OM_Polling));
   			/* close connection to EIB */
   			EIB7Close(eib);

         	exit(0);

         }

         /* write trigger count */
         EventData.TriggerCount = (float) LinearEncoderData.TriggerCounter;


         /* print status word and position value */
         if (PrintData == 1)
         {
		 printf("Linear Encoder Data: ");
	         printf(POS_SPEC_COUNTS, LinearEncoderData.TriggerCounter, LinearEncoderData.Timestamp,
	                          LinearEncoderData.status,LinearEncoderData.position);

	         //printf(POS_SPEC_DEG, LinearEncoderData.TriggerCounter, LinearEncoderData.Timestamp,
	         //                 LinearEncoderData.status,EventData.LinearPosDeg);
	         
		 printf("\n");
	         printf("Rotary Encoder Data: ");
	         printf(POS_SPEC_DEG, RotaryEncoderData.TriggerCounter, RotaryEncoderData.Timestamp,
	                          RotaryEncoderData.status, EventData.RotaryPosDeg);
	         printf("\n");
	         printf("System Time: ");
	         printf("Epoch Time[s]: %03ld : %03ld [us]\n",SystemTime.tv_sec,SystemTime.tv_usec);

	     }
	     if (PrintTimeOnly == 1)
	     {
	     	printf("%05u;%03ld;%03ld\n",RotaryEncoderData.TriggerCounter,SystemTime.tv_sec,SystemTime.tv_usec);
	     }
         /* Send the data */
         if(SendData == 1)
         {
	         EncoderInfo.Status = 0;
	         memcpy(&BufferData,&EventData, sizeof(EventData));
	         memcpy(&BufferInfo, &EncoderInfo, sizeof(EncoderInfo));

	         zmq_send (encoder,&BufferInfo, sizeof(BufferInfo) , ZMQ_SNDMORE);
	         zmq_send (encoder, &BufferData, sizeof(BufferData), 0);
	         zmq_recv (encoder, BufferReply, 2, 0);

	         //printf ("Received Reply");
	     }
      }
#ifdef _WIN32          /* wait for 50 ms to minmize processor load */
      Sleep(50);
#else
      usleep(5);
#endif

   } /* end of loop */


   printf("\nStopped on user request\n");
   // shut down the encoder / server correctely
   if(SendData == 1)
   {
	   // Close the zmq stuff
	   zmq_close (encoder);
	   zmq_ctx_destroy (context);
	}

   /* disable trigger */
   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Disable, EIB7_TS_All));

   /* disable SoftRealtime mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_Polling));

   /* close connection to EIB */
   EIB7Close(eib);
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

