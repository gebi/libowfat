#include "fmt.h"
#include "stralloc.h"
#include "textcode.h"
#include "str.h"
#include "haveinline.h"

static inline int tohex(char c) {
  return c>9?c-10+'a':c+'0';
}

int fmt_hexdump_sa(stralloc* sa,const char* src,unsigned int len) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long i;
  for (i=0; i<len; ++i) {
    char dest[2];
    dest[0]=tohex(s[i]>>4);
    dest[1]=tohex(s[i]&15);
    if (!stralloc_catb(sa, dest, 2)) return 0;
  }
  return 1;
}
