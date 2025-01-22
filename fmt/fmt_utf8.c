#ifndef INTERNAL
#include <libowfat/fmt.h>
#endif

size_t fmt_utf8(char *dest,uint32_t n) {
  size_t i,j;
  if (n<=0x7f) {
    if (dest) *dest=(char)n;
    return 1;
  }
  for (i=0x3f,j=0; i<=0x7fffffff; i=(i<<5)|0x1f, ++j) {
    if (i>=n) {
      if (dest) {
	size_t k=j*6;
	// gcc -fanalyze warns here that j-1 might underflow, leading to
	// undefined behavior because right shift by more than integer
	// width is undefined. That can't happen because both n>0x7f and
	// i>=n would have to be true and we initialize i as 0x3f and
	// with each iteration j is incremented. It's a false positive.
	*dest++=(char)(((signed char)0xc0 >> (j-1)) | (char)(n >> k));
	while (k) {
	  *dest++=(char)(0x80 | ((n >> (k-6)) & 0x3f));
	  k-=6;
	}
      }
      return j+1;
    }
    if (i==0x7fffffff) return 0;
  }
  /* we were asked to encode a value that cannot be encoded */
  return 0;
}

#if defined(__i386__) || defined(__x86_64__)

// on architectures where unaligned writes are ok and don't carry a
// performance pentalty, it may make sense to not write each byte
// individually but write a 32-bit word at the end. The downside is that
// this will write scratch bytes over the bytes following the utf-8. If
// you allocated a few more bytes scratch space, and did not expect
// those bytes to be untouched, you can use this function.
// I did a small benchmark:
//   for (i=j=0; i<1000; i+=fmt_utf8(buf+i,i));
// Regular fmt_utf8: 13k cpu cycles, fmt_utf_scratch: 11k cpu cycles.

size_t fmt_utf8_scratch(char *dest,uint32_t n) {
  size_t i;
  unsigned int j;
  if (n<=0x7f) {
    if (dest) *dest=(char)n;
    return 1;
  }
  for (i=0x3f,j=0; i<=0x7fffffff; i=(i<<5)|0x1f, ++j) {
    if (i>=n) {
      if (dest) {
	uint64_t buf=0;
	while (n>0x3f) {
	  buf = (buf << 8) + (n & 0x3f) + 0x80;
	  n >>= 6;
	}
	buf = (buf << 8) + (unsigned char)((char)0xc0 >> (j-1)) + n;
	*(uint64_t*)dest = buf;
      }
      return j+1;
    }
    if (i==0x7fffffff) return 0;
  }
  /* we were asked to encode a value that cannot be encoded */
  return 0;
}
#endif

/* unit tested via scan/scan_utf8.c */
