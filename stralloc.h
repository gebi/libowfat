#ifndef STRALLOC_H
#define STRALLOC_H

#include <sys/cdefs.h>

typedef struct stralloc {
  char* s;
  unsigned int len;
  unsigned int a;
} stralloc;

/* stralloc_ready makes sure that sa has enough space allocated to hold
 * len bytes: If sa is not allocated, stralloc_ready allocates at least
 * len bytes of space, and returns 1. If sa is already allocated, but
 * not enough to hold len bytes, stralloc_ready allocates at least len
 * bytes of space, copies the old string into the new space, frees the
 * old space, and returns 1. Note that this changes sa.s. */
extern int stralloc_ready(stralloc* sa,unsigned int len);

/* stralloc_readyplus is like stralloc_ready except that, if sa is
 * already allocated, stralloc_readyplus adds the current length of sa
 * to len. */
extern int stralloc_readyplus(stralloc* sa,unsigned int len);

/* stralloc_copyb copies the string buf[0], buf[1], ..., buf[len-1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, stralloc_copyb leaves sa alone and returns 0. */
extern int stralloc_copyb(stralloc* sa,const char* buf,unsigned int len);

/* stralloc_copys copies a \0-terminated string from buf into sa,
 * without the \0. It is the same as
 * stralloc_copyb(&sa,buf,str_len(buf)). */
extern int stralloc_copys(stralloc* sa,const char* buf);

/* stralloc_copy copies the string stored in sa2 into sa. It is the same
 * as stralloc_copyb(&sa,sa2.s,sa2.len). sa2 must already be allocated. */
extern int stralloc_copy(stralloc* sa,stralloc* sa2);

/* stralloc_catb adds the string buf[0], buf[1], ... buf[len-1] to the
 * end of the string stored in sa, allocating space if necessary, and
 * returns 1. If sa is unallocated, stralloc_catb is the same as
 * stralloc_copyb. If it runs out of memory, stralloc_catb leaves sa
 * alone and returns 0. */
extern int stralloc_catb(stralloc* sa,const char* in,unsigned int len);

/* stralloc_cats is analogous to stralloc_copys */
extern int stralloc_cats(stralloc* sa,const char* in);

/* stralloc_cat is analogous to stralloc_copy */
extern int stralloc_cat(stralloc* sa,stralloc* in);

/* stralloc_append adds one byte buf[0] to the end of the string stored
 * in sa. It is the same as stralloc_catb(&sa,buf,1). */
extern int stralloc_append(stralloc* sa,const char* in); /* beware: this takes a pointer to 1 char */

/* stralloc_starts returns 1 if the \0-terminated string in buf, without
 * the terminating \0, is a prefix of the string stored in sa. Otherwise
 * it returns 0. sa must already be allocated. */
extern int stralloc_starts(stralloc* sa,const char* in);

/* stralloc_0 appends \0 */
#define stralloc_0(sa) stralloc_append(sa,"")

/* stralloc_catulong0 appends a '0' padded ASCII representation of in */
extern int stralloc_catulong0(stralloc* sa,unsigned long in,unsigned int n);

/* stralloc_catlong0 appends a '0' padded ASCII representation of in */
extern int stralloc_catlong0(stralloc* sa,signed long in,unsigned int n);

/* stralloc_free frees the storage associated with sa */
extern void stralloc_free(stralloc* sa);

#define stralloc_catlong(sa,l) (stralloc_catlong0((sa),(l),0))
#define stralloc_catuint0(sa,i,n) (stralloc_catulong0((sa),(i),(n)))
#define stralloc_catint0(sa,i,n) (stralloc_catlong0((sa),(i),(n)))
#define stralloc_catint(sa,i) (stralloc_catlong0((sa),(i),0))

#endif
