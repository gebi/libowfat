#include "fmt.h"
#include "stralloc.h"
#include "textcode.h"
#include "str.h"
#include "haveinline.h"

static inline int tohex(char c) {
  return c>9?c-10+'A':c+'0';
}

int fmt_urlencoded_sa(stralloc *sa,const char* src,unsigned int len) {
  const char unsafe[]=" %<>\"#{}|\\^~[]`;/?:@=&";
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long i;
  for (i=0; i<len; ++i) {
    if (s[i]&0x80 || unsafe[str_chr(unsafe,s[i])]==s[i]) {
      char dest[3] = {'%'};
      dest[1]=tohex(s[i]>>4);
      dest[2]=tohex(s[i]&15);
      if (!stralloc_catb(sa, &dest, 3)) return 0;
    } else {
      if (!stralloc_catb(sa, s+i, 1)) return 0;
    }
  }
  return 1;
}
