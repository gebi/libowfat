#include "fmt.h"
#include "textcode.h"
#include "haveinline.h"
#include "str.h"

unsigned long fmt_quotedprintable2(char* dest,const char* src,unsigned long len,const char* escapeme) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long written=0,i;
  for (i=0; i<len; ++i) {
    if (s[i]&0x80 || s[i]<' ' || s[i]=='=' || escapeme[str_chr(escapeme,s[i])]==s[i]) {
      if (dest) {
	dest[written]='=';
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

unsigned long fmt_quotedprintable(char* dest,const char* src,unsigned long len) {
  return fmt_quotedprintable2(dest,src,len,"");
}
