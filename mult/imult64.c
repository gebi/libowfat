#ifdef __x86_64__

/* WARNING: this only works if compiled with -fomit-frame-pointer */
void imult64() {
  asm volatile(
    "xchgq %rdx,%rsi\n"
    "movq %rdi,%rax\n"
    "imulq %rdx\n"
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

int imult64(int64 a,int64 b,int64* c) {
  int neg=(a<0);
  uint64 d;
  if (neg) a=-a;
  if (b<0) { neg^=1; b=-b; }
  if (!umult64(a,b,&d)) return 0;
  if (d > 0x7fffffffffffffffULL + neg) return 0;
  *c=(neg?-d:d);
  return 1;
}

#endif
