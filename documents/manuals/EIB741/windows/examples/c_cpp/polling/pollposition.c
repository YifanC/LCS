/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    pollposition.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    03.11.2009
*  \version $Revision: 1.8 $
* 
*  \brief   sample for polling mode
*  
*  Content:
*  Sample programm for the polling mode of
*  the EIB7. The program configures one axis
*  of the EIB7 and reads the position value
*  in a loop.
*
-----------------------------------------------------*/



#include <eib7.h>
#include <stdio.h>


#ifdef _WIN32
#include <windows.h>
#endif

#ifdef Linux
#include <signal.h>
#endif


#ifdef _WIN32
#define POS_SPEC_INCR  "Status word: 0x%04X Position (int): %010I64d  = %11.4f signal periods "
#define POS_SPEC_ENDAT "Status word: 0x%04X Position: %010I64d"
#else
#define POS_SPEC_INCR  "Status word: 0x%04X Position (int): %010lld  = %11.4f signal periods "
#define POS_SPEC_ENDAT "Status word: 0x%04X Position: %010lld"
#endif


/* definitions */
#define EIB_TCP_TIMEOUT       5000   /* timeout for TCP connection in ms     */
#define NUM_OF_AXIS           4      /* number of axes of the EIB            */
#define MAX_TEXT_LEN          200    /* maximum size of console input string */


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


/* function declarations */
void CheckError(EIB7_ERR error);

void PollPosition(EIB7_AXIS axis, int enc_type);


/* Polling mode demo program
   This program demonstrates the polling mode of the EIB. The program initialises 
   the axes of the EIB and reads the position data by calling the PollPosition
   function of this demo program.
*/
int main(int argc, char **argv)
{
   EIB7_HANDLE eib;              /* EIB handle              */
   unsigned long ip;             /* IP address of EIB       */
   char hostname[MAX_TEXT_LEN];  /* hostname string         */
   unsigned long num;            /* number of encoder axes  */
   EIB7_AXIS axis[NUM_OF_AXIS];  /* axes array              */
   char fw_version[20];          /* firmware version string */
   int enc_axis;                 /* actual axis index       */
   int enc_type;                 /* encoder type            */
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
   printf("Please select encoder axis (1..4): ");
   scanf("%i", &i);
   if(i<1 || i>4)
   {
      printf("wrong axis number\n");
      exit(1);
   }
   enc_axis=i-1;

   printf("Please select encoder type:\n");
   printf("  1 = 1 Vpp (incremental)\n");
   printf("  2 = EnDat 2.1\n");
   printf("  3 = EnDat 2.2\n");
   printf("->");
   scanf("%i", &i);
   if(i<1 || i>3)
   {
      printf("wrong encoder type\n");
      exit(1);
   }
   enc_type=i;

   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);

   /* get axes array */
   CheckError(EIB7GetAxis(eib, axis, NUM_OF_AXIS, &num));


   /* initialize selected axis */
   switch(enc_type)
   {
   case 1:
       /* 1 Vpp */
      fprintf(stderr, "Initializing encoder %d for 1 Vpp\n", enc_axis+1);
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
   break;

   case 2:
      /* EnDat 2.1 */
      fprintf(stderr, "Initializing encoder %d for EnDat 2.1\n", enc_axis+1);
      CheckError(EIB7InitAxis(axis[enc_axis],
                 EIB7_IT_EnDat21,
                 EIB7_EC_Linear,
                 EIB7_RM_None,
                 0,                    /* not used for EnDat */
                 0,                    /* not used for EnDat */
                 EIB7_HS_None,
                 EIB7_LS_None,
                 EIB7_CS_None,         /* not used for EnDat */
                 EIB7_BW_High,         /* not used for EnDat */
                 EIB7_CLK_Default,     /* we use the default clock */
                 EIB7_RT_Long,         /* not used for EnDat 2.1 */
                 EIB7_CT_Long          /* encoder with long calculation timeout */
                 ));
   break;

   case 3:
      /* EnDat 2.2 */
      fprintf(stderr, "Initializing encoder %d for EnDat 2.2\n", enc_axis+1);
      CheckError(EIB7InitAxis(axis[enc_axis],
                 /* with EnDat 2.2 we enable the EnDat propagation time compensation */
                 EIB7_IT_EnDat22 | EIB7_IT_EnDatDelayMeasurement,
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
   break;

   default:
   break;
   }

   /* call polling loop function */
   PollPosition(axis[enc_axis],enc_type);

   /* close connection to EIB */
   EIB7Close(eib);

   printf("\nStopped on user request\n");

   exit(1);
}


/* PollPosition
   This is the polling loop function. It demonstrates how to read out position
   data from the EIB in polling mode. The status word and position 
   value of one axis is displayed.
   */
void PollPosition(EIB7_AXIS axis, int enc_type)
{
   printf("\n\npress Ctrl-C to stop\n\n\r");

   /* read position continuously */
   while(!stop)
   {
      /* run till the user press Ctrl-C */
      unsigned short status;           /* status word                     */
      ENCODER_POSITION pos;            /* position value (integer)        */
      double pos_sp;                   /* position value (signal periods) */

      /* read position from EIB */
      CheckError(EIB7GetPosition(axis, &status, &pos));
      
      if(enc_type==1)
      {  /* incremental encoder? */
         CheckError(EIB7IncrPosToDouble(pos,&pos_sp));
         /* print position value */
         printf(POS_SPEC_INCR, status, pos, pos_sp);
      }
      else
      {
         /* print position value */
         printf(POS_SPEC_ENDAT, status, pos);
      }
      printf("\r");
   }
}


/* check error code
   This function prints the error code and a brief description to the standard error
   console. The program will be terminated afterwards.
   */
void CheckError(EIB7_ERR error)
{
   if(error != EIB7_NoError)
   {
      char mnemonic[32];
      char message[256];

      EIB7GetErrorInfo(error, mnemonic, 32, message, 256);

      fprintf(stderr, "\nError %08X (%s): %s\n", error, mnemonic, message);
      exit(0);
   }
}
