#include "byte.h"
#include "buffer.h"
#include "scan.h"

int buffer_get_token_pred(buffer* b,char* x,unsigned int len,string_predicate p) {
  int blen;

  for (blen=0;blen<len;++blen) {
    register int r;
    if ((r=buffer_getc(b,x))<0) return r;
    if (r==0) break;
    if (p(x-blen,blen+1)) break;
    ++x;
  }
  return blen;
}
