#include "fmt.h"
#include "textcode.h"
#include "str.h"
#include "haveinline.h"

static inline int issafe(unsigned char c) {
  const char safe[] = "$/.=~";
  return ((c>='A' && c<='Z') ||
	  (c>='a' && c<='z') ||
	  (c>='0' && c<='9') ||
	  safe[str_chr(safe,c)]);
}

unsigned long fmt_urlencoded2(char* dest,const char* src,unsigned long len,const char* escapeme) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long written=0,i;
  for (i=0; i<len; ++i) {
    if (!issafe(s[i]) || escapeme[str_chr(escapeme,s[i])]==s[i]) {
      if (dest) {
	dest[written]='%';
	dest[written+1]=fmt_tohex(s[i]>>4);
	dest[written+2]=fmt_tohex(s[i]&15);
      }
      written+=3;
    } else {
      if (dest) dest[written]=s[i]; ++written;
    }
  }
  return written;
}

unsigned long fmt_urlencoded(char* dest,const char* src,unsigned long len) {
  return fmt_urlencoded2(dest,src,len,"");
}
