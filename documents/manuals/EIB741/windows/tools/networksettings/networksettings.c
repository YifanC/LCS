/*!------------------------------------------------
*
*  network config programm for EIB7
*
*  \file    networksettings.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    28.10.2008
*  \version $Revision: 1.9 $
* 
*  \brief   EIB network configuration
*  
*  Content:
*  Programm to configurate network parameter of EIB.
*
-----------------------------------------------------*/


#include <eib7.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>



/* definitions */
#define MAX_TEXT_LEN          100    /* maximum size of console input string  */
#define CONN_TIMEOUT          5000   /* timeout for TCP connection in ms      */
#define MAX_MAC_LENGTH        6      /* length of MAC address in bytes        */



/* function declarations */
void CheckError(EIB7_ERR error);
int ReadIPAddress(unsigned long *ip_address);

/* do not use the function EIB7OpenNoIFCheck in EIB applications!! */
EIB7_ERR EIB7OpenNoIFCheck(unsigned long ip, EIB7_HANDLE *eib, long timeout, char *ident, unsigned long len);



/*! Main function:
    The Programm reads version numbers and the network settings
    from the EIB.
    \param argc not used
    \param argv not used
    \return status:    0 = error \n
                       1 = ok
*/
int main(int argc, char **argv)
{
	EIB7_HANDLE eib;                       /* EIB handle                  */
	char text[MAX_TEXT_LEN];               /* string input buffer         */
   char hostname[MAX_TEXT_LEN];           /* EIB hostname                */
	unsigned long ip;                      /* IP address of EIB           */
	unsigned long netmask;                 /* netmask of EIB              */
	unsigned long gateway;                 /* gateway of EIB              */
	EIB7_MODE dhcpmode;                    /* dhcp mode (on/off)          */
   unsigned long dhcptimeout;             /* timeout for DHCP client     */
	EIB7_BOOT_MODE bootmode;               /* currently used bootmode     */
	unsigned char mac[MAX_MAC_LENGTH];     /* array for MAC address bytes */
	EIB7_ERR err;                          /* error                       */
   int i;
   char ip_str[MAX_TEXT_LEN];
   char netmask_str[MAX_TEXT_LEN];
   char gateway_str[MAX_TEXT_LEN];
   char dhcpmode_str[MAX_TEXT_LEN];

	printf("Please insert IP address or hostname: ");
	scanf("%s", text);

	/* open connection to EIB */
	CheckError(EIB7GetHostIP(text, &ip));
	err = EIB7Open(ip, &eib, CONN_TIMEOUT, text, MAX_TEXT_LEN);
	if(err==EIB7_IFVersionInv)
		{
			CheckError(EIB7OpenNoIFCheck(ip, &eib, CONN_TIMEOUT, text, MAX_TEXT_LEN));
		}
		else
	  {
			CheckError(err);
	  }

	printf("\n**************************************************\n");

	/* read Ident-No. */
	CheckError(EIB7GetIdentNumber(eib, text, MAX_TEXT_LEN));
	printf("EIB         (ID: %s)\n\n", text);

	/* read network settings */
	CheckError(EIB7GetNetwork(eib, &ip, &netmask, &gateway, &dhcpmode));
	if(err==EIB7_NoCustNetwork)
	{
		strcpy(ip_str,"(default)");
		strcpy(netmask_str,"(default)");
		strcpy(gateway_str,"(default)");
		strcpy(dhcpmode_str,"(default)");
		strcpy(hostname,"(default)");
	}
	else
	{
		CheckError(EIB7GetHostname(eib, hostname, MAX_TEXT_LEN));
		sprintf(ip_str,"%d.%d.%d.%d", ((ip      >> 24) & 0xFF),
		                                ((ip      >> 16) & 0xFF), 
		                                ((ip      >>  8) & 0xFF),
		                                ((ip      >>  0) & 0xFF));
		sprintf(netmask_str,"%d.%d.%d.%d", ((netmask >> 24) & 0xFF),
		                                   ((netmask >> 16) & 0xFF), 
		                                   ((netmask >>  8) & 0xFF),
		                                   ((netmask >>  0) & 0xFF));
		sprintf(gateway_str,"%d.%d.%d.%d", ((gateway >> 24) & 0xFF),
		                                   ((gateway >> 16) & 0xFF), 
		                                   ((gateway >>  8) & 0xFF),
		                                   ((gateway >>  0) & 0xFF));
		if(dhcpmode == EIB7_MD_Enable)
			sprintf(dhcpmode_str,"enabled");
		else
			sprintf(dhcpmode_str,"disabled");
	}
	CheckError(EIB7GetMAC(eib, mac));

	printf("IP-Address: %s\n", ip_str);
	printf("Netmask:    %s\n", netmask_str);
	printf("Gateway:    %s\n", gateway_str);
	printf("DHCP:       %s\n", dhcpmode_str);
	printf("Hostname:   %s\n\n", hostname);

	printf("MAC:        %02X:%02X:%02X:%02X:%02X:%02X\n\n",
	       mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);

	/* read boot mode */
	CheckError(EIB7GetBootMode(eib, &bootmode));
	switch(bootmode)
	{
	case EIB7_BM_User:
		printf("Bootmode:   User firmware with user settings\n");
	break;

	case EIB7_BM_FactoryUser:
		printf("Bootmode:   Default firmware with user settings\n");
	break;

	case EIB7_BM_FactoryDefault:
		printf("Bootmode:   Default firmware with default settings\n");
	break;
	default:
		printf("Bootmode:   Invalid Mode (%d)\n", bootmode);
	}

	/* read version numbers */
	CheckError(EIB7GetVersion(eib, text, MAX_TEXT_LEN, EIB7_FW_Factory));
	printf("Factory firmware version: %s\n", text);
	CheckError(EIB7GetVersion(eib, text, MAX_TEXT_LEN, EIB7_FW_User));
	printf("User firmware version:    %s\n", text);

	printf("\n**************************************************\n\n");

   printf("Do you want to change the EIB network settings? (y/n)\n> ");
   scanf("%s", text);
   
   if(toupper(text[0]) != 'Y')
   {
	   /* close connection to EIB */
	   CheckError(EIB7Close(eib));

	   printf("\nPress ENTER to continue.");
	   getchar();     /* clear standard input stream */
	   getchar();     /* wait for ENTER */

	   exit(1);
   }

   /* read IP address, netmask, gateway */
   printf("\n**************************************************\n");
   printf("Old IP address: %s\n", ip_str);
   printf("New IP address: ");
   while(ReadIPAddress(&ip))
   {
     printf("wrong format!\nNew IP address: ");
   }
   printf("\n**************************************************\n");
   printf("Old netmask: %s\n", netmask_str);
   printf("New netmask: ");
   while(ReadIPAddress(&netmask))
   {
     printf("wrong format!\nNew netmask: ");
   }
   printf("\n**************************************************\n");
   printf("Old gateway: %s\n", gateway_str);
   printf("New gateway: ");
   while(ReadIPAddress(&gateway))
   {
     printf("wrong format!\nNew gateway: ");
   }

   /* read DHCP client settings */
   do
   {
      i = 0;
      printf("\nDo you want to enable the DHCP client? (y/n)\n> ");
      scanf("%s", text);
      if(toupper(text[0]) == 'Y')
         i = 1;
      if(toupper(text[0]) == 'N')
         i = 2;

      if(i == 0)
         printf("\ninvalid character\n");
   }
   while(i == 0);

   dhcptimeout = 10;

   if(i == 1)
   {
      dhcpmode = EIB7_MD_Enable;
      printf("\nPlease enter timeout for DHCP client in seconds\n> ");
      scanf("%d", &dhcptimeout);
   }
   else
      dhcpmode = EIB7_MD_Disable;

   /* read hostname */
   i = 0;
   do
   {
      printf("\nDo you want to change the hostname? (y/n)\n> ");
      scanf("%s", text);
      if(toupper(text[0]) == 'Y')
         i = 1;
      if(toupper(text[0]) == 'N')
         i = 2;

      if(i == 0)
         printf("\ninvalid character\n");
   }
   while(i == 0);

   if(i == 1)
   {
      printf("\n**************************************************\n");
      printf("Old hostname: %s\n", hostname);

      printf("New hostname: ");
      scanf("%s", text);
      if(strlen(text) > 31)
         text[31] = 0;

      CheckError(EIB7SetHostname(eib, text));
   }

   /* save new network settings */
   CheckError(EIB7SetNetwork(eib, ip, netmask, gateway, dhcpmode, dhcptimeout));

   /* print new network settings */
   printf("\nNetwork settings programmed successfully.\n");
   printf("\n**************************************************\n");
   CheckError(EIB7GetNetwork(eib, &ip, &netmask, &gateway, &dhcpmode));
	printf("IP-Address: %d.%d.%d.%d\n", ((ip      >> 24) & 0xFF),
	                                    ((ip      >> 16) & 0xFF), 
	                                    ((ip      >>  8) & 0xFF),
	                                    ((ip      >>  0) & 0xFF));
	printf("Netmask:    %d.%d.%d.%d\n", ((netmask >> 24) & 0xFF),
	                                    ((netmask >> 16) & 0xFF), 
	                                    ((netmask >>  8) & 0xFF),
	                                    ((netmask >>  0) & 0xFF));
	printf("Gateway:    %d.%d.%d.%d\n", ((gateway >> 24) & 0xFF),
	                                    ((gateway >> 16) & 0xFF), 
	                                    ((gateway >>  8) & 0xFF),
	                                    ((gateway >>  0) & 0xFF));
	if(dhcpmode == EIB7_MD_Enable)
		printf("DHCP:       enabled\n");
	else
		printf("DHCP:       disabled\n");
	CheckError(EIB7GetHostname(eib, hostname, MAX_TEXT_LEN));
	printf("Hostname:   %s\n\n", hostname);
   printf("\n**************************************************\n");

   printf("\nThis network settings will be used after");
   printf("\na reboot of the EIB.\n");

 	/* close connection to EIB */
	CheckError(EIB7Close(eib));

	printf("\nPress ENTER to continue.");
	getchar();     /* clear standard input stream */
	getchar();     /* wait for ENTER */

	exit(1);
}



/*! This function checks the status of an EIB driver function
    and displays a messsage if an error occurred. 
  \param error error-code
*/
void CheckError(EIB7_ERR error)
{
	if(error != EIB7_NoError)
	{
		char mnemonic[32];
		char message[256];

		EIB7GetErrorInfo(error, mnemonic, 32, (char*)message, 256);

		printf("\nError %08x (%s): %s\n", error, mnemonic, message);
		exit(0);
	}
}

/*! This function reads an IP-Address from the console. 
    \param ip_address IP-Address which was scanned (return value)
    \return status:    0 = ok \n
                       1 = scanning error (wrong format)
*/
int ReadIPAddress(unsigned long *ip_address)
{
	char text[MAX_TEXT_LEN];               /* string input buffer         */
	char *pstr1,*pstr2;                    /* string pointers             */
	int nums         = 0;                  /* numbers in the input string */
	int error        = 0;                  /* scanning error detected     */
	unsigned long ip = 0;                  /* IP address                  */
	long num         = 0;                  /* scanned integer value       */
	
	scanf("%s", text);                     /* read string from console    */
	pstr1=text;
	while(*pstr1!=0 && !error)             /* loop: scan 4 separate values*/
	{
		num=strtol(pstr1,&pstr2,10);         /* convert one integer         */
		nums++;
		if(num<0 || num>255 || pstr1==pstr2) /* range check                 */
		  error = 1;
		else
		  ip=(ip << 8) | num;                /* insert it into IP-variable  */
		if(*pstr2=='.')                      /* '.' = separator             */
		  pstr2++;
		if(*pstr2==0 && nums!=4)          /* term. char when scan complete? */
		  error = 1;
		if(nums>4)
		  error = 1;
		pstr1=pstr2;                         /* set pointer to next number  */
	}
	*ip_address=ip;
	return(error);
}
