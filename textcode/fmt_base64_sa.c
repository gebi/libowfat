#include "fmt.h"
#include "stralloc.h"
#include "textcode.h"
#include "haveinline.h"

int fmt_base64_sa(stralloc *sa,const char* src,unsigned int len) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned short bits=0,temp=0;
  unsigned long i;
  char dest;
  for (i=0; i<len; ++i) {
    temp<<=8; temp+=s[i]; bits+=8;
    while (bits>6) {
      dest=base64[((temp>>(bits-6))&63)];
      if (!stralloc_catb(sa, &dest, 1)) return 0;
      bits-=6;
    }
  }
  if (bits) {
    temp<<=(6-bits);
    dest=base64[temp&63];
    if (!stralloc_catb(sa, &dest, 1)) return 0;
  }
  while (sa->len&3) { 
    if (!stralloc_catb(sa, "=", 1)) return 0;
  }
  return 1;
}
