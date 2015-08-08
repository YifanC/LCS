/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    eib7info.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    03.11.2009
*  \version $Revision: 1.2 $
* 
*  \brief   EIB device information
*  
*  Content:
*  Sample programm to read the network settings
*  and device information from the EIB.
*
-----------------------------------------------------*/


#include <eib7.h>
#include <stdio.h>



/* definitions */
#define MAX_TEXT_LEN          100    /* maximum size of console input string  */
#define CONN_TIMEOUT          5000   /* timeout for TCP connection in ms      */
#define MAX_MAC_LENGTH        6      /* length of MAC address in bytes        */



/* function declarations */
void CheckError(EIB7_ERR error);



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
	unsigned long ip;                      /* IP address of EIB           */
	unsigned long netmask;                 /* netmask of EIB              */
	unsigned long gateway;                 /* gateway of EIB              */
	EIB7_MODE dhcpmode;                    /* dhcp mode (on/off)          */
	EIB7_BOOT_MODE bootmode;               /* currently used bootmode     */
	unsigned char mac[MAX_MAC_LENGTH];     /* array for MAC address bytes */

	printf("Please insert IP address or hostname: ");
	scanf("%s", text);

	/* open connection to EIB */
	CheckError(EIB7GetHostIP(text, &ip));
	CheckError(EIB7Open(ip, &eib, CONN_TIMEOUT, text, MAX_TEXT_LEN));

	printf("\n**************************************************\n");

	/* read Ident-No. */
	CheckError(EIB7GetIdentNumber(eib, text, MAX_TEXT_LEN));
	printf("EIB         (ID: %s)\n\n", text);

	/* read network settings */
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

	CheckError(EIB7GetHostname(eib, text, MAX_TEXT_LEN));
	printf("Hostname:   %s\n\n", text);

	CheckError(EIB7GetMAC(eib, mac));
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

	printf("\n**************************************************\n");

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
