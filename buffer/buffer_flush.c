#include "buffer.h"

extern ssize_t buffer_stubborn(ssize_t (*op)(int fd,const char* buf,size_t len,void* cookie),int fd,const char* buf, size_t len,void* cookie);

extern int buffer_flush(buffer* b) {
  register size_t p;
  register ssize_t r;
  if (!(p=b->p)) return 0; /* buffer already empty */
  r=buffer_stubborn(b->op.wopc,b->fd,b->x,p,b);
  if (r>0) {
    r=0;
    b->p=0;
  }
  return (int)r;
}
