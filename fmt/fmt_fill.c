#include "fmt.h"

/* "foo" -> "foo  "
 *  Copy srclen characters from src.  write padlen-srclen spaces, if
 *  that is >= 0.  Truncate only if total length is larger than maxlen.
 *  Return number of characters written. */
unsigned int fmt_fill(char* dest,const char* src,unsigned int srclen,unsigned int padlen,unsigned int maxlen) {
  int todo;
  char* olddest=dest;
  char* max=dest+maxlen;
  for (todo=srclen; todo>0; --todo) {
    if (dest>max) break;
    *dest=*src; ++dest; ++src;
  }
  for (todo=padlen-srclen; todo>0; --todo) {
    if (dest>max) break;
    *dest=' '; ++dest;
  }
  return dest-olddest;
}
