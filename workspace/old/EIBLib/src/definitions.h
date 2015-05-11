#ifndef __DEFINITIONS__H__
#define __DEFINITIONS__H__

/*! \file definitions.h
    \brief This header file declares the fixed constants for the EIB driver.
*/

#define TCP_DEFAULT_PORT 1050
#define STREAMING_DEFAULT_PORT 1060
#define TCP_DEFAULT_TIMEOUT 2000
#define TCP_MIN_TIMEOUT 100
#define DHCP_DEFAULT_TIMEOUT 10
#define FIFO_MIN_SIZE 2000
#define FIFO_DEFAULT_SIZE 262144


/*! The allignment for the position data packets specifies the smallest increment for the size of a packet */
#define POSDATA_ALLIGN_REC    4
#define POSDATA_ALLIGN_STRM   4
#define POSDATA_ALLIGN_SRT    2
#define POSDATA_ALLIGN_POLL   1

/*! The interface version for the compatibility check. We only
  check a major and minor version. The rule is:
  - different major versions are never compatible
  - different minor versions are comaptible, if the minor version
  of the firmware is higher than that of the driver.
*/
#define EIB7_MAJOR_VERSION 2
/*! The minor version number of the interface. */
#define EIB7_MINOR_VERSION 2 

#ifndef TRUE
#define TRUE (1)
#define FALSE (0)
#endif

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

/*! The version of the driver */
#define DRIVER_VERSION_NUMBER 0x01010C01

/*! The ID string of the driver */
#define DRIVER_ID_STRING "66005004"

#endif /*__DEFINITIONS__H__*/
