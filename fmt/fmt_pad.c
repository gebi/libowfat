#include "fmt.h"

/* "foo" -> "  foo"
 *  write padlen-srclen spaces, if that is >= 0.  Then copy srclen
 *  characters from src.  Truncate only if total length is larger than
 *  maxlen.  Return number of characters written. */
unsigned int fmt_pad(char* dest,const char* src,unsigned int srclen,unsigned int padlen,unsigned int maxlen) {
  int todo;
  char* olddest=dest;
  char* max=dest+maxlen;
  for (todo=padlen-srclen; todo>0; --todo) {
    if (dest>max) break;
    *dest=' '; ++dest;
  }
  for (todo=srclen; todo>0; --todo) {
    if (dest>max) break;
    *dest=*src; ++dest; ++src;
  }
  return dest-olddest;
}
