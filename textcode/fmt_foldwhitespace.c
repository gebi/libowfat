#include "fmt.h"
#include "textcode.h"
#include "str.h"
#include "haveinline.h"

unsigned long fmt_foldwhitespace(char* dest,const char* src,unsigned long len) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long i;
  char c;
  for (i=0; i<len; ++i) {
    switch (c=s[i]) {
    case ' ': case '\t': case '\n': c='_'; break;
    }
    if (dest) dest[i]=c;
  }
  return len;
}
