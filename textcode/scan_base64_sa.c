#include "stralloc.h"
#include "textcode.h"
#include "haveinline.h"

static inline int dec(unsigned char x) {
  if (x>='A' && x<='Z') return x-'A';
  if (x>='a' && x<='z') return x-'a'+26;
  if (x>='0' && x<='9') return x-'0'+26+26;
  switch (x) {
  case '+': return 62;
  case '/': return 63;
  default: return -1;
  }
}

int scan_base64_sa(const char *src,stralloc* sa) {
  unsigned short tmp=0,bits=0;
  register const unsigned char* s=(const unsigned char*) src;
  for (;;) {
    int a=dec(*s);
    if (a<0) {
      while (*s=='=') ++s;
      break;
    }
    tmp=(tmp<<6)|a; bits+=6;
    ++s;
    if (bits>=8) {
      char dest=(tmp>>(bits-=8));
      if (!stralloc_catb(sa,&dest,1)) return 0;
    }
  }
  return 1;
}
