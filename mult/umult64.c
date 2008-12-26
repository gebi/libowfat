#if defined(__x86_64__) && defined(__OPTIMIZE__)

/* WARNING: this only works if compiled with -fomit-frame-pointer */
void umult64() {
  asm volatile(
    "xchgq %rdx,%rsi\n"
    "movq %rdi,%rax\n"
    "mulq %rdx\n"
    "jc 1f\n"	/* overflow */
    "movq %rax,(%rsi)\n"
    "xorq %rax,%rax\n"
    "inc %rax\n"
    "ret\n"
    "1:\n"
    "xorq %rax,%rax\n"
    /* the closing ret is renerated by gcc */
    );
}

#else

#include "safemult.h"

#if defined(__GNUC__) && (defined(__x86_64__) || defined(__ia64__) || defined(__powerpc64__) || defined(__alpha__) || defined(__mips64__) || defined(__sparc64__))

int umult64(uint64 a,uint64 b,uint64* c) {
  __uint128_t x=((__uint128_t)a)*b;
  if ((*c=(uint64)x) != x) return 0;
  return 1;
}

#else

/* return 1 for overflow, 0 for ok */
int umult64(uint64 a,uint64 b,uint64* c) {
  uint32 ahi=a>>32;
  uint32 alo=(a&0xffffffff);
  uint32 bhi=b>>32;
  uint32 blo=(b&0xffffffff);

  // a=ahi*x+alo, b=bhi*x+blo
  // a*b = (ahi*x+alo) * (bhi*x+blo)
  //     = ahi*x*bhi*x + ahi*x*blo + alo*bhi*x + alo*blo

  // -> overflow if ahi*bhi != zero */
  if (ahi && bhi) return 0;

  a=(uint64)(ahi)*blo+(uint64)(alo)*bhi;
  if (a>0xffffffff) return 0;
  {
    uint64 x=(uint64)(alo)*blo;
    if (x+(a<<32) < x) return 0;
    *c=x+(a<<32);
  }
  return 1;
}

#endif

#endif
