#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#define BSD_SENDFILE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include <alloca.h>

#include "iob_internal.h"

int64 iob_send(int64 s,io_batch* b) {
  io_entry* e,* last;
  struct iovec* v;
  int64 sent;
  long i;
  long headers;
#ifdef BSD_SENDFILE
  long trailers;
#endif

  if (b->bytesleft==0) return 0;
  last=array_start(&b->b)+array_bytes(&b->b);
  if (!(e=array_get(&b->b,sizeof(io_entry),b->next)))
    return -1;		/* can't happen error */
  v=alloca(b->bufs*sizeof(struct iovec));
#ifdef BSD_SENDFILE
  /* BSD sendfile can send headers and trailers.  If we run on BSD, we
   * should try to exploit this. */
  headers=trailers=0;
#endif
  for (i=0; e+i<last; ++i) {
    if (e[i].type==FROMFILE) break;
    v[i].iov_base=e[i].buf+e[i].offset;
    v[i].iov_len=e[i].n-e[i].offset;
  }
  headers=i;
#ifdef BSD_SENDFILE
  if (e[i].type==FROMFILE) {
    off_t sbytes;
    struct sf_hdtr hdr;
    int r;
    for (++i; e+i<last; ++i) {
      if (e[i].type==FROMFILE) break;
      v[i-1].iov_base=e[i].buf+e[i].offset;
      v[i-1].iov_len=e[i].n-e[i].offset;
      ++trailers;
    }
    hdr.headers=v; hdr.hdr_cnt=headers;
    hdr.trailers=v+headers; hdr.trl_cnt=trailers;
    r=sendfile(e[headers].fd,s,e[headers].off,nbytes,&hdr,&sbytes,0);
    if (r==0)
      sent=b->bytesleft;
    else if (r==-1 && errno==EAGAIN)
      sent=sbytes;
    else
      sent=-1;
  } else
    sent=writev(s,v,headers);
#else
  if (headers)
    sent=writev(s,v,headers);
  else
    sent=io_sendfile(s,e->fd,e->offset,e->n);
#endif
  if (sent==b->bytesleft)
    b->bytesleft=0;
  else if (sent>0) {
    int64 rest=sent;
    b->bytesleft-=rest;
    for (i=0; e+i<last; ++i) {
      if (e[i].n-e[i].offset<rest) {
	rest-=e[i].n-e[i].offset;
	++b->next;
      } else {
	e[i].offset+=rest;
	break;
      }
    }
  }
  return sent;
}
