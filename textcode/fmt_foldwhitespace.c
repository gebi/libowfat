#include <libowfat/fmt.h>
#include <libowfat/textcode.h>
#include <libowfat/str.h>
#include "haveinline.h"

size_t fmt_foldwhitespace(char* dest,const char* src,size_t len) {
  register const unsigned char* s=(const unsigned char*) src;
  size_t i;
  char c;
  for (i=0; i<len; ++i) {
    switch (c=s[i]) {
    case ' ': case '\t': case '\n': c='_'; break;
    }
    if (dest) dest[i]=c;
  }
  return len;
}
