#include "byte.h"
#include "stralloc.h"
#include "buffer.h"
#include <errno.h>

int buffer_get_token_sa(buffer* b,stralloc* sa,const char* charset,unsigned int setlen) {
  sa->len=0;
  for (;;) {
    char x;
    if (!stralloc_readyplus(sa,1)) goto nomem;
    switch  (buffer_getc(b,&x)) {
    case -1: return -1;
    case 0: return 0;
    }
    stralloc_append(sa,&x);
    if (byte_chr(charset,setlen,x)<setlen) break;
  }
  return 0;
nomem:
  errno=ENOMEM;
  return -1;
}
