/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    matchIncrEnDat.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    03.11.2009
*  \version $Revision: 1.1 $
* 
*  \brief   sample for EnDat01
*  
*  Content:
*  Sample programm for EnDat01 encoders. The
*  EnDat position value and the incremental position
*  value will be latched simultaneously.
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
#define POS_SPEC "Status word: 0x%04X Position: %010I64d"
#else
#define POS_SPEC "Status word: 0x%04X Position: %010lld"
#endif


/* definitions */
#define EIB_TCP_TIMEOUT       5000   /* timeout for TCP connection in ms     */
#define NUM_OF_AXIS           4      /* number of axes of the EIB            */
#define MAX_TEXT_LEN          200    /* maximum size of console input string */




/* function declarations */
void CheckError(EIB7_ERR error);


/* Polling mode demo program
   This program latches the EnDat position value and the incremental position value
   of an EnDat01 encoder simultaneously. Therefore an EnDat01 encoder must be
   connected to axis 1.
*/
int main(int argc, char **argv)
{
   EIB7_HANDLE eib;              /* EIB handle                    */
   unsigned long ip;             /* IP address of EIB             */
   char hostname[MAX_TEXT_LEN];  /* hostname string               */
   unsigned long num;            /* number of encoder axes        */
   EIB7_AXIS axis[NUM_OF_AXIS];  /* axes array                    */
   char fw_version[20];          /* firmware version string       */
   unsigned short statusEnDat;   /* EnDat status word             */
   unsigned short statusIncr;    /* incremental status word       */
   ENCODER_POSITION posEnDat;    /* EnDat position value          */
   ENCODER_POSITION posIncr;     /* incremental position value    */


   /* ask for parameters */
   printf("Please enter IP address or hostname: ");
   scanf("%s", hostname);

   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);

   /* get axes array */
   CheckError(EIB7GetAxis(eib, axis, NUM_OF_AXIS, &num));

   /* EnDat01 */
   fprintf(stderr, "Initializing encoder 1 for EnDat01\n");
   CheckError(EIB7InitAxis(axis[0],
              EIB7_IT_EnDat01,
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

   CheckError(EIB7ReadEnDatIncrPos(axis[0], &statusEnDat, &posEnDat, &statusIncr, &posIncr));
   printf("EnDat: ");
   printf(POS_SPEC, statusEnDat, posEnDat);
   printf("\nIncr:  ");
   printf(POS_SPEC, statusIncr, posIncr);

   /* close connection to EIB */
   EIB7Close(eib);

	printf("\n\nPress ENTER to continue.");
	getchar();     /* clear standard input stream */
	getchar();     /* wait for ENTER */

   exit(1);
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
