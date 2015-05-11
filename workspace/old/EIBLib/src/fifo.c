/*!
  \file fifo.c
  \brief this file implements the FIFO data structure for the soft realtime mode 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"

#define MAXELEMENTS(fifo) ( fifo->size / fifo->elemsize )

EIB7_ERR EIB_CreateFIFO(EIB_FIFO *fifo, int fifosize, int elemsize)
{
  memset(fifo, 0, sizeof(EIB_FIFO));

  fifo->data = (unsigned char *)malloc(fifosize);
  if(fifo->data == NULL)
    return EIB7_OutOfMemory;

  fifo->size = fifosize;
  fifo->elemsize = elemsize;

  return EIB7_NoError;
}

EIB7_ERR EIB_DestroyFIFO(EIB_FIFO *fifo)
{
  free(fifo->data);
  
  memset(fifo, 0, sizeof(EIB_FIFO));

  return EIB7_NoError;
}

EIB7_ERR EIB_GetFIFOSize(EIB_FIFO *fifo, unsigned long *fifosize)
{
  *fifosize = fifo->size;
  return EIB7_NoError;
}

EIB7_ERR EIB_SetFIFOSize(EIB_FIFO *fifo, int fifosize)
{
	unsigned char *data;

	data = (unsigned char *)malloc(fifosize);

	if(data == NULL) return EIB7_OutOfMemory;

	free(fifo->data);
	fifo->data = data;
	fifo->size = fifosize;
	fifo->start = fifo->end = 0;

	return EIB7_NoError;
}

int EIB_GetFIFOFreeElements(EIB_FIFO *fifo)
{
  int elems;
  int free;

  elems = EIB_GetFIFONumElements(fifo);

  free = MAXELEMENTS(fifo) - elems;
  free--;      /* one element must be empty to get no overflow error */

  if(free < 0)
     return 0;

  return free;
}


int EIB_GetFIFONumElements(EIB_FIFO *fifo)
{
  if(fifo->end >= fifo->start)
    return fifo->end - fifo->start;

  return MAXELEMENTS(fifo) - fifo->start + fifo->end;
}
    
EIB7_ERR EIB_InsertFIFO(EIB_FIFO *fifo, void *element, int size, int *num)
{
  if(size != fifo->elemsize)
    return EIB7_ElementSizeInv;
      
  memcpy(&fifo->data[fifo->end * fifo->elemsize], element, size);

  /* increment end data pointer with wraparound */
  fifo->end++;
  if(fifo->end >= MAXELEMENTS(fifo))
    fifo->end = 0;

  if(fifo->end == fifo->start)
    {
      fifo->overflow = 1;
      
      fifo->start++;
      if(fifo->start >= MAXELEMENTS(fifo))
	fifo->start = 0;
    }

  *num = EIB_GetFIFONumElements(fifo);

  return EIB7_NoError;
}

EIB7_ERR EIB_RetrieveFIFO(EIB_FIFO *fifo, void *element, int size)
{
  if(size != fifo->elemsize)
    return EIB7_ElementSizeInv;

  if(fifo->start == fifo->end)
    return EIB7_FIFOEmpty;
  
  memcpy(element, &fifo->data[fifo->start * fifo->elemsize], size);

  /* increment start data pointer with wraparound */
  fifo->start++;
  if(fifo->start >= MAXELEMENTS(fifo))
    fifo->start = 0;

  if(fifo->overflow)
    {
      fifo->overflow = 0;
      return EIB7_FIFOOverflow;
    }
  
  return EIB7_NoError;
}

EIB7_ERR EIB_GetElementPtrFIFO(EIB_FIFO *fifo, void **element)
{
  if(fifo->start == fifo->end)
    return EIB7_FIFOEmpty;

  if(fifo->overflow)
    {
      fifo->overflow = 0;
      return EIB7_FIFOOverflow;
    }
    
  *element = &fifo->data[fifo->start * fifo->elemsize];

  return EIB7_NoError;
}


EIB7_ERR EIB_RemoveElementFIFO(EIB_FIFO *fifo)
{
  if(fifo->start == fifo->end)
    return EIB7_FIFOEmpty;
  
  /* increment start data pointer with wraparound */
  fifo->start++;
  if(fifo->start >= MAXELEMENTS(fifo))
    fifo->start = 0;

  return EIB7_NoError;
}

EIB7_ERR EIB_ClearFIFO(EIB_FIFO *fifo)
{
  fifo->start = fifo->end = 0;

  return EIB7_NoError;
}

