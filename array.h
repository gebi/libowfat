#ifndef _ARRAY_H
#define _ARRAY_H

#include "uint64.h"
#include <errno.h>

typedef struct {
  char* p;
  int64 allocated, initialized; /* in bytes */

  /* p and allocated nonzero: array is allocated */
  /* p zero: array is unallocated */
  /* allocated < 0: array is failed */
} array;

void* array_allocate(array* x,int64 membersize,int64 pos);
void* array_get(array* x,int64 membersize,int64 pos);
void* array_start(const array* const x);
int64 array_length(const array* const x,int64 membersize);
int64 array_bytes(const array* const x);
void array_truncate(array* x,int64 membersize,int64 len);
void array_trunc(array* x);
void array_reset(array* x);
void array_fail(array* x);
int array_equal(const array* const x,const array* const y);
void array_cat(array* to,const array* const from);
void array_catb(array* to,const char* from,int64 len);
void array_cats(array* to,const char* from);
void array_cats0(array* to,const char* from);
void array_cat0(array* to);
void array_cate(array* to,const array* const from,int64 pos,int64 stop);

#endif
