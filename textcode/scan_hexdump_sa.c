#include "fmt.h"
#include "stralloc.h"
#include "textcode.h"
#include "haveinline.h"

static inline int fromhex(char c) {
  if (c>='0' && c<='9') return c-'0';
  if (c>='A' && c<='F') return c-'A'+10;
  if (c>='a' && c<='f') return c-'a'+10;
  return -1;
}

int scan_hexdump_sa(const char *src,stralloc *sa) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long i;
  for (i=0; s[i]; ++i) {
    char dest;
    int j=fromhex(s[i]);
    if (j<0) break;
    dest=j<<4;
    j=fromhex(s[i+1]);
    if (j<0) break;
    dest|=j;
    ++i;
    if (!stralloc_catb(sa, &dest, 1)) return 0;
  }
  return 1;
}
