/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IARRAY_H
#define IARRAY_H

/* This header defines an indirect array for use with the io_* routines.
 * Assumptions:
 * - the elements are small (many fit on one page),
 * - the platform has an atomic compare-and-swap instruction
 * - the compiler supports it via __sync_val_compare_and_swap
 */

#include "uint64.h"
#include <stddef.h>
#ifdef __MINGW32__
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef struct _iarray_page {
  struct _iarray_page* next;
  char data[];
} iarray_page;

typedef struct {
  iarray_page* pages[16];
  size_t elemsize,elemperpage,bytesperpage,len;
} iarray;

void iarray_init(iarray* ia,size_t elemsize);
void* iarray_get(iarray* ia,size_t pos);
void* iarray_allocate(iarray* ia,size_t pos);
size_t iarray_length(iarray* ia);

/* WARNING: do not use the array during or after iarray_free, make sure
 * no threads are potentially doing anything with the iarray while it is
 * being freed! */
void iarray_free(iarray* ia);

#endif
