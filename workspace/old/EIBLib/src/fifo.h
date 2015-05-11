#ifndef __FIFO__H__
#define __FIFO__H__

/*! \file fifo.h
    \brief This header file declares the necessary functions and
    data types for the platform-independant fifo implementation.
*/

#include "eib7.h"

/*! The FIFO data structure */
typedef struct EIB_FIFO
{
  long size;           /*!< Size of this FIFO's data area (in bytes) */
  long elemsize;       /*!< Size of one element in this FIFO (in bytes) */
  
  long start;          /*!< Index of the start element in the FIFO (oldest element). */
  long end;            /*!< Index of the next element to be inserted in the FIFO.
			 The FIFO is empty exactly when start == end */
  unsigned char *data; /*!< Pointer to the FIFO's data area */
  int overflow;        /*!< Flag to indicate an overflow of the FIFO data has ocurred */
} EIB_FIFO;


/*! Creates a FIFO with the requested sizes
   \param fifo a pointer to the FIFO structure
   \param fifosize the size of the FIFO (in bytes)
   \param elemsize the size of one fifo element (in bytes)
   \return EIB7_NoError if the FIFO could be allocated correctly
*/
EIB7_ERR EIB_CreateFIFO(EIB_FIFO *fifo, int fifosize, int elemsize);

/*! Frees all memory for the FIFO and clears the FIFO data structure
   \param fifo a pointer to the FIFO structure
   \return always returns EIB7_NoError
*/
EIB7_ERR EIB_DestroyFIFO(EIB_FIFO *fifo);

/*! Returns the size of the FIFO in bytes
   \param fifo a pointer to the FIFO structure
   \param fifosize pointer to a variable that receives the FIFO size
   \return always returns EIB7_NoError
*/
EIB7_ERR EIB_GetFIFOSize(EIB_FIFO *fifo, unsigned long *fifosize);

/*! Sets the size of the FIFO in bytes
   \param fifo a pointer to the FIFO structure
   \param fifosize the FIFO size
   \return EIB7_OutOfMemory if the memory could not be allocated
*/
EIB7_ERR EIB_SetFIFOSize(EIB_FIFO *fifo, int fifosize);

/*! Returns the number of elements currently in the FIFO
   \param fifo a pointer to the FIFO structure
   \return the number of elements in the FIFO
*/
int EIB_GetFIFONumElements(EIB_FIFO *fifo);


/*! Returns the number of free elements in the FIFO
   \param fifo a pointer to the FIFO structure
   \return the number of free elements in the FIFO
*/
int EIB_GetFIFOFreeElements(EIB_FIFO *fifo);


/*! Insert an element into the FIFO. If the size of the element does not match the
  element size of the FIFO, it is rejected. If the FIFO size is exceeded, the
  oldest element gets thrown away
  \param fifo a pointer to the FIFO structure
  \param element the data to insert
  \param size size (in bytes) of the element
  \param num pointer to a variable which receives the number of entries after the insert
  \return EIB7_NoError if the call was successful
*/
EIB7_ERR EIB_InsertFIFO(EIB_FIFO *fifo, void *element, int size, int *num);

/*! Retrieves an element from the FIFO. If the size of the element does not match the
  element size of the FIFO, an error is returned.
  \param fifo a pointer to the FIFO structure
  \param element pointer to the data area that receives the element data
  \param size size (in bytes) of the element
  \return EIB7_NoError if the call was successful
*/
EIB7_ERR EIB_RetrieveFIFO(EIB_FIFO *fifo, void *element, int size);

/*! Returns a pointer to the next element in the FIFO, so it can be accessed without actually
  copying it 
  \param fifo a pointer to the FIFO structure
  \param element pointer to a pointer that receives the element start address
  \return EIB7_NoError if the call was successful
*/
EIB7_ERR EIB_GetElementPtrFIFO(EIB_FIFO *fifo, void **element);

/*! Removes an element from the FIFO without copying it.
  \param fifo a pointer to the FIFO structure
  \return EIB7_NoError if the call was successful
*/
EIB7_ERR EIB_RemoveElementFIFO(EIB_FIFO *fifo);

/*! Clears all data from the fifo.
  \param fifo a pointer to the FIFO structure
  \return EIB7_NoError if the call was successful
*/
EIB7_ERR EIB_ClearFIFO(EIB_FIFO *fifo);

#endif /* __FIFO_H__ */
