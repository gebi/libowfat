/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef FMT_H
#define FMT_H

/* for size_t: */
#include <stddef.h>
/* for time_t: */
#include <sys/types.h>

#define FMT_LONG  41 /* enough space to hold -2^127 in decimal, plus \0 */
#define FMT_ULONG 40 /* enough space to hold 2^128 - 1 in decimal, plus \0 */
#define FMT_8LONG 44 /* enough space to hold 2^128 - 1 in octal, plus \0 */
#define FMT_XLONG 33 /* enough space to hold 2^128 - 1 in hexadecimal, plus \0 */
#define FMT_LEN ((char *) 0) /* convenient abbreviation */

/* The formatting routines do not append \0!
 * Use them like this: buf[fmt_ulong(buf,number)]=0; */

/* convert signed src integer -23 to ASCII '-','2','3', return length.
 * If dest is not NULL, write result to dest */
size_t fmt_long(char *dest,signed long src);

/* convert unsigned src integer 23 to ASCII '2','3', return length.
 * If dest is not NULL, write result to dest */
size_t fmt_ulong(char *dest,unsigned long src);

/* convert unsigned src integer 0x23 to ASCII '2','3', return length.
 * If dest is not NULL, write result to dest */
size_t fmt_xlong(char *dest,unsigned long src);

/* convert unsigned src integer 023 to ASCII '2','3', return length.
 * If dest is not NULL, write result to dest */
size_t fmt_8long(char *dest,unsigned long src);

size_t fmt_longlong(char *dest,signed long long src);
size_t fmt_ulonglong(char *dest,unsigned long long src);
size_t fmt_xlonglong(char *dest,unsigned long long src);

#define fmt_uint(dest,src) fmt_ulong(dest,src)
#define fmt_int(dest,src) fmt_long(dest,src)
#define fmt_xint(dest,src) fmt_xlong(dest,src)
#define fmt_8int(dest,src) fmt_8long(dest,src)

/* Like fmt_ulong, but prepend '0' while length is smaller than padto.
 * Does not truncate! */
size_t fmt_ulong0(char *,unsigned long src,size_t padto);

#define fmt_uint0(buf,src,padto) fmt_ulong0(buf,src,padto)

/* convert src double 1.7 to ASCII '1','.','7', return length.
 * If dest is not NULL, write result to dest */
size_t fmt_double(char *dest, double d,int max,int prec);

/* if src is negative, write '-' and return 1.
 * if src is positive, write '+' and return 1.
 * otherwise return 0 */
size_t fmt_plusminus(char *dest,int src);

/* if src is negative, write '-' and return 1.
 * otherwise return 0. */
size_t fmt_minus(char *dest,int src);

/* copy str to dest until \0 byte, return number of copied bytes. */
size_t fmt_str(char *dest,const char *src);

/* copy str to dest until \0 byte or limit bytes copied.
 * return number of copied bytes. */
size_t fmt_strn(char *dest,const char *src,size_t limit);

/* "foo" -> "  foo"
 * write padlen-srclen spaces, if that is >= 0.  Then copy srclen
 * characters from src.  Truncate only if total length is larger than
 * maxlen.  Return number of characters written. */
size_t fmt_pad(char* dest,const char* src,size_t srclen,size_t padlen,size_t maxlen);

/* "foo" -> "foo  "
 * append padlen-srclen spaces after dest, if that is >= 0.  Truncate
 * only if total length is larger than maxlen.  Return number of
 * characters written. */
size_t fmt_fill(char* dest,size_t srclen,size_t padlen,size_t maxlen);

/* 1 -> "1", 4900 -> "4.9k", 2300000 -> "2.3M" */
size_t fmt_human(char* dest,unsigned long long l);

/* 1 -> "1", 4900 -> "4.8k", 2300000 -> "2.2M" */
size_t fmt_humank(char* dest,unsigned long long l);

/* "Sun, 06 Nov 1994 08:49:37 GMT" */
size_t fmt_httpdate(char* dest,time_t t);

/* internal functions, may be independently useful */
char fmt_tohex(char c);

#define fmt_strm(b,...) fmt_strm_internal(b,__VA_ARGS__,(char*)0)
size_t fmt_strm_internal(char* dest,...);

#ifndef MAX_ALLOCA
#define MAX_ALLOCA 100000
#endif
#define fmt_strm_alloca(a,...) ({ size_t len=fmt_strm((char*)0,a,__VA_ARGS__)+1; char* c=(len<MAX_ALLOCA?alloca(len):0); if (c) c[fmt_strm(c,a,__VA_ARGS__)]=0; c;})

#endif
