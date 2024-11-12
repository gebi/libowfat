/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef _CLAMP_H
#define _CLAMP_H

#include <stdckdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

/* Idea from Stefan Reuther, fe5vsq.17c.1@stefan.msgid.phost.de */
#define PP_NARG(...) (sizeof((_Bool[]){__VA_ARGS__})/sizeof(_Bool))

#define clamp_add(...) clamp_addn(PP_NARG(__VA_ARGS__),__VA_ARGS__)

#define clamp_addn(a, ...) \
  a==0?0: \
  a==1?a: \
  a==2?clamp_add2(__VA_ARGS__): \
  a==3?clamp_add3(__VA_ARGS__,0): \
  a==4?clamp_add4(__VA_ARGS__,0,0): \
  clamp_add2plus(a,__VA_ARGS__)
#define clamp_add2(a,b,...) ({size_t ret; ckd_add(&ret,a,b)?(size_t)-1:ret;})
#define clamp_add3(a,b,c,...) clamp_add2(a,clamp_add2(b,c))
#define clamp_add4(a,b,c,d,...) clamp_add2(clamp_add2(a,b),clamp_add2(c,d))
#define clamp_add2plus(a,b,...) ({size_t ret; ckd_add(&ret,a,b)?(size_t)-1:__clamp_addn(a-2,ret,__VA_ARGS__);})

static inline size_t __clamp_addn(size_t n, ...) {
  va_list l;
  size_t ret;
  va_start(l,n);
  for (ret=0; n>0; --n) {
    if (ckd_add(&ret,ret,va_arg(l,size_t))) {
      ret=(size_t)-1;
      break;
    }
  }
  va_end(l);
  return ret;
}

#define clamp_mul(...) clamp_muln(PP_NARG(__VA_ARGS__),__VA_ARGS__)

#define clamp_muln(a, ...) \
  a==0?0: \
  a==1?a: \
  a==2?clamp_mul2(__VA_ARGS__): \
  a==3?clamp_mul3(__VA_ARGS__,0): \
  a==4?clamp_mul4(__VA_ARGS__,0,0): \
  clamp_mul2plus(a,__VA_ARGS__)
#define clamp_mul2(a,b,...) ({size_t ret; ckd_mul(&ret,a,b)?(size_t)-1:ret;})
#define clamp_mul3(a,b,c,...) clamp_mul2(a,clamp_mul2(b,c))
#define clamp_mul4(a,b,c,d,...) clamp_mul2(clamp_mul2(a,b),clamp_mul2(c,d))
#define clamp_mul2plus(a,b,...) ({size_t ret; ckd_mul(&ret,a,b)?(size_t)-1:__clamp_muln(a-2,ret,__VA_ARGS__);})

static inline size_t __clamp_muln(size_t n, ...) {
  va_list l;
  size_t ret;
  va_start(l,n);
  for (ret=0; n>0; --n) {
    if (ckd_mul(&ret,ret,va_arg(l,size_t))) {
      ret=(size_t)-1;
      break;
    }
  }
  va_end(l);
  return ret;
}

static inline size_t clamp_hdrarray(size_t hdrsize,size_t nelems,size_t elemsize) {
  return clamp_add(hdrsize,clamp_mul(nelems,elemsize));
}

#endif
