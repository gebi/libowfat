#include "fmt.h"
#include "stralloc.h"
#include "textcode.h"
#include "haveinline.h"

static inline unsigned int enc(unsigned char x) {
  return ((x-1)&077)+'!';
}

int fmt_uuencoded_sa(stralloc* sa,const char* src,unsigned int len) {
  unsigned int i;
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long tmp;
  while (len) {
    {
      register unsigned int diff;
      char c;
      if (len>45) { i=15; diff=45; } else { i=(len+2)/3; diff=len; }
      c=enc(diff);
      len-=diff;
      if (!stralloc_catb(sa,&c,1)) return 0;
    }
    for (; i; --i) {
      char dest[4];
      tmp=((unsigned long)s[0] << 16) +
	  ((unsigned long)s[1] << 8) +
	  ((unsigned long)s[2]);
      dest[0]=enc((tmp>>(3*6))&077);
      dest[1]=enc((tmp>>(2*6))&077);
      dest[2]=enc((tmp>>(1*6))&077);
      dest[3]=enc(tmp&077);
      s+=3;
      if (!stralloc_catb(sa,dest,4)) return 0;
    }
    if (!stralloc_catb(sa,"\n",1)) return 0;
  }
  return 1;
}
