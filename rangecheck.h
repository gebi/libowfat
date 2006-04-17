#ifndef RANGECHECK_H
#define RANGECHECK_H

#include <inttypes.h>
#include <stddef.h>

/* return 0 for range error / overflow, 1 for ok */

#if defined(__GNUC__) && defined(__OPTIMIZE__)
#define __static extern
#else
#define __static static
#endif

/* does ptr point to one of buf[0], buf[1], ... buf[len-1]? */
__static inline int range_ptrinbuf(const void* buf,size_t len,const void* ptr) {
  register const char* c=(const char*)buf;	/* no pointer arithmetic on void* */
  return (c &&		/* is buf non-NULL? */
#if (__GNUC__ == 4) && (__GNUC_MINOR__ == 1)
	  ((uintptr_t)c)+len>(uintptr_t)c &&	/* gcc 4.1 miscompiles this test */
#else
	  c+len>c &&	/* catch integer overflows and fail if buffer is 0 bytes long */
			/* because then ptr can't point _in_ the buffer */
#endif
	  (uintptr_t)((const char*)ptr-c)<len);	/* this one is a little tricky.
     "ptr-c" checks the offset of ptr in the buffer is inside the buffer size.
     Now, ptr-c can underflow; say it is -1.  When we cast it to uintptr_t, it becomes
     a very large number. */
}

/* Is this a plausible buffer?
 * Check whether buf is NULL, and whether buf+len overflows.
 * Does NOT check whether buf has a non-zero length! */
__static inline int range_validbuf(const void* buf,size_t len) {
#if (__GNUC__ == 4) && (__GNUC_MINOR__ == 1)
  return (buf && (uintptr_t)buf+len>=(uintptr_t)buf);	/* gcc 4.1 miscompiles this test */
#else
  return (buf && (const char*)buf+len>=(const char*)buf);
#endif
}

/* is buf2[0..len2-1] inside buf1[0..len-1]? */
__static inline int range_bufinbuf(const void* buf1,size_t len1,const void* buf2,size_t len2) {
  return range_validbuf(buf1,len1) &&
         range_validbuf(buf2,len2) &&
	 buf1<=buf2 &&
	 (ptrdiff_t)buf1+len1>=(ptrdiff_t)buf2+len2;
}

/* does an array of "elements" members of size "membersize" starting at
 * "arraystart" lie inside buf1[0..len-1]? */
int range_arrayinbuf(const void* buf,size_t len,
		     const void* arraystart,size_t elements,size_t membersize);

/* does an ASCIIZ string starting at "ptr" lie in buf[0..len-1]? */
int range_strinbuf(const void* buf,size_t len,const void* stringstart);

/* does an UTF-16 string starting at "ptr" lie in buf[0..len-1]? */
int range_str2inbuf(const void* buf,size_t len,const void* stringstart);

/* does an UTF-32 string starting at "ptr" lie in buf[0..len-1]? */
int range_str4inbuf(const void* buf,size_t len,const void* stringstart);

#undef __static

#endif
