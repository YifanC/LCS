/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    easy.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    02.11.2009
*  \version $Revision: 1.3 $
* 
*  \brief   easy sample
*  
*  Content:
*  Easy sample programm for reading position values
*  using axis 1 of EIB with an incremental encoder.
*  The IP-address of EIB has to be 192.168.1.2
*
-----------------------------------------------------*/


#include <eib7.h>
#include <stdio.h>


#ifdef _WIN32
#define POS_SPEC "Status word: 0x%04X Position: %010I64d "
#else
#define POS_SPEC "Status word: 0x%04X Position: %010lld "
#endif


/* definitions */
#define EIB_TCP_TIMEOUT       5000        /* timeout for TCP connection in ms */
#define NUM_OF_AXIS           4           /* number of axes of the EIB        */


/* function declarations */
void CheckError(EIB7_ERR error);

/* Polling mode demo program
   This program demonstrates the polling mode of the EIB. The program initialises 
   axis 1 of the EIB and reads the position data.
*/
int main(int argc, char **argv)
{
   EIB7_HANDLE eib;              /* EIB handle               */
   unsigned long ip;             /* IP address of EIB        */
   unsigned long num;            /* number of encoder axes   */
   EIB7_AXIS axis[NUM_OF_AXIS];  /* axes array               */
   char fw_version[20];          /* firmware version string  */
   char s[100];                  /* buffer for console input */

   /* open connection to EIB */
   CheckError(EIB7GetHostIP("192.168.1.2", &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);

   /* get axes array */
   CheckError(EIB7GetAxis(eib, axis, NUM_OF_AXIS, &num));

   /* initialize axis */
   CheckError(EIB7InitAxis(axis[0],
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
  
   /* loop for reading position values */
   do
   {
      unsigned short status;
      ENCODER_POSITION pos;

      /* read position from EIB */
      CheckError(EIB7GetPosition(axis[0], &status, &pos));
        
      /* print position value */
      printf(POS_SPEC, status, pos);
      printf("\n");
      printf("read position again (y/n)? ");
      scanf("%s",s);
     
   }while(s[0]=='y' || s[0]=='Y');

   /* close connection to EIB */
   EIB7Close(eib);
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
