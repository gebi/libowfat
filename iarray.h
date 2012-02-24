/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IARRAY_H
#define IARRAY_H

#warning DO NOT USE THIS YET.  It may look thread-safe but it is not!

#include "uint64.h"
#include <stddef.h>
#ifdef __MINGW32__
#include <windows.h>
#else
#include <pthread.h>
#endif

/* this is an indirect array; it only reallocs the indirect index, not
 * the whole array.  The actual data does not move.  So there is no need
 * to lock the array for read accesses. */

typedef struct {
  char** pages;
  size_t elemsize,pagefence,elemperpage,bytesperpage;
  /* pagefence is the number of pages + 1,
   * i.e. the first out of bounds index in "pages" */
#ifdef __MINGW32__
  CRITICAL_SECTION cs;
#else
  pthread_mutex_t m;
#endif
} iarray;

void iarray_init(iarray* ia,size_t elemsize);
void* iarray_get(iarray* ia,size_t pos);
void* iarray_allocate(iarray* ia,size_t pos);

/* WARNING: do not use the array during or after iarray_free, make sure
 * no threads are potentially doing anything with the iarray while it is
 * being freed! */
void iarray_free(iarray* ia);

#endif
