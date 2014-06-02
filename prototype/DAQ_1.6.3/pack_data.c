/*************************************************************************/
/*                                                                       */
/*   encode and split data chunks of                                     */
/*   16 short (16 bit) words where ony the first 10 bits are meaningful  */
/*   into 5 long (32 bits) words                                         */
/*                                                                       */
/*            size=number of long words must be multiple of  8 !!        */
/*                                                                       */
/*                                            AC    04-02-2006           */
/*                                                                       */
/*************************************************************************/
#include <stdio.h>
#include <stdlib.h>

int pack_data(unsigned short stop, unsigned short size, unsigned long *buffer, unsigned long dout[]) {
  
  int i,j1,dsize,index,offset;
  unsigned short *a = (unsigned short*) buffer; // (buffer + (size/8 - stop) * 8);
 
  j1 = 0;
  dsize = size * 2;

  offset = (stop + 1) * 16;  // 8 (long/sample) * 2 (short/long)

  for(i=0; i<dsize; i+=16) {

    index = (i+offset)%dsize;

/*     printf(" stop = %d       size = %d     index = %d    a = 0x%8x\n",stop,size,index,a); */

/*     dout[j1++]=((a[index   ] & 0x03FF) << 22) | ((a[index+ 1] & 0x03FF) << 12) | ((a[index+ 2] & 0x03FF) << 2) |  (a[index+15] & 0x0003); */
/*     dout[j1++]=((a[index+ 3] & 0x03FF) << 22) | ((a[index+ 4] & 0x03FF) << 12) | ((a[index+ 5] & 0x03FF) << 2) | ((a[index+15] & 0x000c) >> 2); */
/*     dout[j1++]=((a[index+ 6] & 0x03FF) << 22) | ((a[index+ 7] & 0x03FF) << 12) | ((a[index+ 8] & 0x03FF) << 2) | ((a[index+15] & 0x0030) >> 4); */
/*     dout[j1++]=((a[index+ 9] & 0x03FF) << 22) | ((a[index+10] & 0x03FF) << 12) | ((a[index+11] & 0x03FF) << 2) | ((a[index+15] & 0x00c0) >> 6); */
/*     dout[j1++]=((a[index+12] & 0x03FF) << 22) | ((a[index+13] & 0x03FF) << 12) | ((a[index+14] & 0x03FF) << 2) | ((a[index+15] & 0x0300) >> 8); */

    dout[j1++]=((a[index   ] & 0x03FF) << 16) | (a[index+ 1] & 0x03FF);
    dout[j1++]=((a[index+ 2] & 0x03FF) << 16) | (a[index+ 3] & 0x03FF);
    dout[j1++]=((a[index+ 4] & 0x03FF) << 16) | (a[index+ 5] & 0x03FF);
    dout[j1++]=((a[index+ 6] & 0x03FF) << 16) | (a[index+ 7] & 0x03FF);
    dout[j1++]=((a[index+ 8] & 0x03FF) << 16) | (a[index+ 9] & 0x03FF);
    dout[j1++]=((a[index+10] & 0x03FF) << 16) | (a[index+11] & 0x03FF);
    dout[j1++]=((a[index+12] & 0x03FF) << 16) | (a[index+13] & 0x03FF);
    dout[j1++]=((a[index+14] & 0x03FF) << 16) | (a[index+15] & 0x03FF);

  }

  return j1;
}
