#include "fmt.h"
#include "textcode.h"
#include "str.h"
#include "haveinline.h"

unsigned long fmt_cescape2(char* dest,const char* src,unsigned long len,const char* escapeme) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long written=0,i;
  char c;
  for (i=0; i<len; ++i) {
    switch (s[i]) {
    case '\a': c='a'; goto doescape;
    case '\b': c='b'; goto doescape;
    case 0x1b: c='e'; goto doescape;
    case '\f': c='f'; goto doescape;
    case '\n': c='n'; goto doescape;
    case '\r': c='r'; goto doescape;
    case '\t': c='t'; goto doescape;
    case '\v': c='v'; goto doescape;
    case '\\':
	c='\\';
      doescape:
	if (dest) {
	  dest[written]='\\';
	  dest[written+1]=c;
	}
	written+=2;
	break;
    default:
	if (s[i]<' ' || escapeme[str_chr(escapeme,s[i])]==s[i]) {
	  if (dest) {
	    dest[written]='\\';
	    dest[written+1]='x';
	    dest[written+2]=fmt_tohex(s[i]>>4);
	    dest[written+3]=fmt_tohex(s[i]&0xf);
	  }
	  written+=4;
	} else {
	  if (dest) dest[written]=s[i];
	  ++written;
	}
	break;
    }
  }
  return written;
}

unsigned long fmt_cescape(char* dest,const char* src,unsigned long len) {
  return fmt_cescape2(dest,src,len,"");
}
