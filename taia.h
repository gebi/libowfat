#ifndef TAIA_H
#define TAIA_H

/* Times with 1 attosecond precision */

#include "tai.h"

/* A struct taia value is a number between 0 inclusive and 2^64
 * exclusive. The number is a multiple of 10^-18. The format of struct
 * taia is designed to speed up common operations; applications should
 * not look inside struct taia. */
struct taia {
  struct tai sec;
  unsigned long nano; /* 0...999999999 */
  unsigned long atto; /* 0...999999999 */
};

/* extract seconds */
extern void taia_tai(const struct taia *source,struct tai *dest);

/* get current time */
extern void taia_now(struct taia *);

/* return double-precision approximation; always nonnegative */
extern double taia_approx(const struct taia *);
/* return double-precision approximation of the fraction part;
 * always nonnegative */
extern double taia_frac(const struct taia *);

/* add source1 to source2 modulo 2^64 and put the result in dest.
 * The inputs and output may overlap */
extern void taia_add(struct taia *dest,const struct taia *source1,const struct taia *source2);
/* add secs seconds to source modulo 2^64 and put the result in dest. */
extern void taia_addsec(struct taia *dest,const struct taia *source,int secs);
/* subtract source2 from source1 modulo 2^64 and put the result in dest.
 * The inputs and output may overlap */
extern void taia_sub(struct taia *,const struct taia *,const struct taia *);
/* divide source by 2, rouding down to a multiple of 10^-18, and put the
 * result into dest.  The input and output may overlap */
extern void taia_half(struct taia *dest,const struct taia *source);
/* return 1 if a is less than b, 0 otherwise */
extern int taia_less(const struct taia *a,const struct taia *b);

#define TAIA_PACK 16
/* char buf[TAIA_PACK] can be used to store a TAI64NA label in external
 * representation, which can then be used to transmit the binary
 * representation over a network or store it on disk in a byte order
 * independent fashion */

/* convert a TAI64NA label from internal format in src to external
 * TAI64NA format in buf. */
extern void taia_pack(char *buf,const struct taia *src);
/* convert a TAI64NA label from external TAI64NA format in buf to
 * internal format in dest. */
extern void taia_unpack(const char *buf,struct taia *dest);

#define TAIA_FMTFRAC 19
/* print the 18-digit fraction part of t in decimal, without a decimal
 * point but with leading zeros, into the character buffer s, without a
 * terminating \0. It returns 18, the number of characters written. s
 * may be zero; then taia_fmtfrac returns 18 without printing anything.
 * */
extern unsigned int taia_fmtfrac(char *s,const struct taia *t);

/* initialize t to secs seconds. */
extern void taia_uint(struct taia *t,unsigned int secs);

#endif
