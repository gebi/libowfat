#ifndef SCAN_H
#define SCAN_H

#include <sys/types.h>

#ifdef __dietlibc__
#include <sys/cdefs.h>
#endif

#ifndef __pure__
#define __pure__
#endif

/* interpret src as ASCII decimal number, write number to dest and
 * return the number of bytes that were parsed */
unsigned int scan_ulong(const char *src,unsigned long *dest);

/* interpret src as ASCII hexadecimal number, write number to dest and
 * return the number of bytes that were parsed */
unsigned int scan_xlong(const char *src,unsigned long *dest);

/* interpret src as ASCII octal number, write number to dest and
 * return the number of bytes that were parsed */
unsigned int scan_8long(const char *src,unsigned long *dest);

/* interpret src as signed ASCII decimal number, write number to dest
 * and return the number of bytes that were parsed */
unsigned int scan_long(const char *src,signed long *dest);

unsigned int scan_longlong(const char *src,signed long long *dest);
unsigned int scan_ulonglong(const char *src,unsigned long long *dest);
unsigned int scan_xlonglong(const char *src,unsigned long long *dest);
unsigned int scan_8longlong(const char *src,unsigned long long *dest);

unsigned int scan_uint(const char *src,unsigned int *dest);
unsigned int scan_xint(const char *src,unsigned int *dest);
unsigned int scan_8int(const char *src,unsigned int *dest);
unsigned int scan_int(const char *src,signed int *dest);

unsigned int scan_ushort(const char *src,unsigned short *dest);
unsigned int scan_xshort(const char *src,unsigned short *dest);
unsigned int scan_8short(const char *src,unsigned short *dest);
unsigned int scan_short(const char *src,signed short *dest);

/* interpret src as double precision floating point number,
 * write number to dest and return the number of bytes that were parsed */
unsigned int scan_double(const char *in, double *dest);

/* if *src=='-', set *dest to -1 and return 1.
 * if *src=='+', set *dest to 1 and return 1.
 * otherwise set *dest to 1 return 0. */
unsigned int scan_plusminus(const char *src,signed int *dest);

/* return the highest integer n<=limit so that isspace(in[i]) for all 0<=i<=n */
unsigned long scan_whitenskip(const char *in,unsigned long limit) __pure__;

/* return the highest integer n<=limit so that !isspace(in[i]) for all 0<=i<=n */
unsigned long scan_nonwhitenskip(const char *in,unsigned long limit) __pure__;

/* return the highest integer n<=limit so that in[i] is element of
 * charset (ASCIIZ string) for all 0<=i<=n */
unsigned long scan_charsetnskip(const char *in,const char *charset,unsigned long limit) __pure__;

/* return the highest integer n<=limit so that in[i] is not element of
 * charset (ASCIIZ string) for all 0<=i<=n */
unsigned long scan_noncharsetnskip(const char *in,const char *charset,unsigned long limit) __pure__;

/* try to parse ASCII GMT date; does not understand time zones. */
/* example dates:
 *   "Sun, 06 Nov 1994 08:49:37 GMT"
 *   "Sunday, 06-Nov-94 08:49:37 GMT"
 *   "Sun Nov  6 08:49:37 1994"
 */
unsigned int scan_httpdate(const char *in,time_t *t) __pure__;

/* a few internal function that might be useful independently */
/* convert from hex ASCII, return 0 to 15 for success or -1 for failure */
int scan_fromhex(unsigned char c);

#endif
