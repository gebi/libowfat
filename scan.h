/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef SCAN_H
#define SCAN_H

/* for size_t: */
#include <stddef.h>
/* for time_t: */
#include <sys/types.h>

#ifndef __pure__
#define __pure__
#endif

/* interpret src as ASCII decimal number, write number to dest and
 * return the number of bytes that were parsed */
size_t scan_ulong(const char *src,unsigned long *dest);

/* interpret src as ASCII hexadecimal number, write number to dest and
 * return the number of bytes that were parsed */
size_t scan_xlong(const char *src,unsigned long *dest);

/* interpret src as ASCII octal number, write number to dest and
 * return the number of bytes that were parsed */
size_t scan_8long(const char *src,unsigned long *dest);

/* interpret src as signed ASCII decimal number, write number to dest
 * and return the number of bytes that were parsed */
size_t scan_long(const char *src,signed long *dest);

size_t scan_longlong(const char *src,signed long long *dest);
size_t scan_ulonglong(const char *src,unsigned long long *dest);
size_t scan_xlonglong(const char *src,unsigned long long *dest);
size_t scan_8longlong(const char *src,unsigned long long *dest);

size_t scan_uint(const char *src,unsigned int *dest);
size_t scan_xint(const char *src,unsigned int *dest);
size_t scan_8int(const char *src,unsigned int *dest);
size_t scan_int(const char *src,signed int *dest);

size_t scan_ushort(const char *src,unsigned short *dest);
size_t scan_xshort(const char *src,unsigned short *dest);
size_t scan_8short(const char *src,unsigned short *dest);
size_t scan_short(const char *src,signed short *dest);

/* interpret src as double precision floating point number,
 * write number to dest and return the number of bytes that were parsed */
size_t scan_double(const char *in, double *dest);

/* if *src=='-', set *dest to -1 and return 1.
 * if *src=='+', set *dest to 1 and return 1.
 * otherwise set *dest to 1 return 0. */
size_t scan_plusminus(const char *src,signed int *dest);

/* return the highest integer n<=limit so that isspace(in[i]) for all 0<=i<=n */
size_t scan_whitenskip(const char *in,size_t limit) __pure__;

/* return the highest integer n<=limit so that !isspace(in[i]) for all 0<=i<=n */
size_t scan_nonwhitenskip(const char *in,size_t limit) __pure__;

/* return the highest integer n<=limit so that in[i] is element of
 * charset (ASCIIZ string) for all 0<=i<=n */
size_t scan_charsetnskip(const char *in,const char *charset,size_t limit) __pure__;

/* return the highest integer n<=limit so that in[i] is not element of
 * charset (ASCIIZ string) for all 0<=i<=n */
size_t scan_noncharsetnskip(const char *in,const char *charset,size_t limit) __pure__;

/* try to parse ASCII GMT date; does not understand time zones. */
/* example dates:
 *   "Sun, 06 Nov 1994 08:49:37 GMT"
 *   "Sunday, 06-Nov-94 08:49:37 GMT"
 *   "Sun Nov  6 08:49:37 1994"
 */
size_t scan_httpdate(const char *in,time_t *t) __pure__;

/* a few internal function that might be useful independently */
/* convert from hex ASCII, return 0 to 15 for success or -1 for failure */
int scan_fromhex(unsigned char c);

#endif
