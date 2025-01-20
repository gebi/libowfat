#include <libowfat/compiler.h>
#include <libowfat/safemult.h>
#include <libowfat/array.h>

/* I'm not sure I understand what this function is good for */
void array_truncate(array* x,uint64 membersize,int64 len) {
  uint64 wanted;
  if (unlikely(len<0)) return;
  if (unlikely(!umult64(membersize,len,&wanted))) return;
  if (unlikely(wanted > x->initialized)) return;
  x->initialized=wanted;
}
