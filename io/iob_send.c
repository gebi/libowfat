#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#define BSD_SENDFILE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#ifdef __linux__
#include <alloca.h>
#endif

#include "iob_internal.h"

int64 iob_send(int64 s,io_batch* b) {
  io_entry* e,* last;
  struct iovec* v;
  int64 total,sent;
  long i;
  long headers;
#ifdef BSD_SENDFILE
  long trailers;
#endif

  if (b->bytesleft==0) return 0;
  last=array_start(&b->b)+array_bytes(&b->b);
  v=alloca(b->bufs*sizeof(struct iovec));
  total=0;
  for (;;) {
    if (!(e=array_get(&b->b,sizeof(io_entry),b->next)))
      return -1;		/* can't happen error */
#ifdef BSD_SENDFILE
    /* BSD sendfile can send headers and trailers.  If we run on BSD, we
    * should try to exploit this. */
    headers=trailers=0;
#endif
    for (i=0; e+i<last; ++i) {
      if (e[i].type==FROMFILE) break;
      v[i].iov_base=(char*)(e[i].buf+e[i].offset);
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
	v[i-1].iov_base=(char*)(e[i].buf+e[i].offset);
	v[i-1].iov_len=e[i].n-e[i].offset;
	++trailers;
      }
      hdr.headers=v; hdr.hdr_cnt=headers;
      hdr.trailers=v+headers; hdr.trl_cnt=trailers;
      r=sendfile(e[headers].fd,s,e[headers].offset,e[headers].n,&hdr,&sbytes,0);
      if (r==0)
	sent=b->bytesleft;
      else if (r==-1 && errno==EAGAIN)
	sent=sbytes;
      else
	sent=-1;
    } else
      sent=writev(s,v,headers);
#else
#ifdef TCP_CORK
    if (b->bufs && b->files && !b->next) {
      static int one=1;
      setsockopt(s,IPPROTO_TCP,TCP_CORK,&one,sizeof(one));
    }
#endif
    if (headers)
      sent=writev(s,v,headers);
    else
      sent=io_sendfile(s,e->fd,e->offset,e->n);
#endif
    if (sent>0)
      total+=sent;
    else
      if (!total) return -1;
    if (sent==b->bytesleft) {
      b->bytesleft=0;
#ifdef TCP_CORK
    if (b->bufs && b->files) {
      static int zero=0;
      setsockopt(s,IPPROTO_TCP,TCP_CORK,&zero,sizeof(zero));
    }
#endif
      break;
    } else if (sent>0) {
      int64 rest=sent;
      b->bytesleft-=rest;
      for (i=0; e+i<last; ++i) {
	if (e[i].n-e[i].offset<=rest) {
	  rest-=e[i].n-e[i].offset;
	  ++b->next;
	  if (!rest) break;
	} else {
	  e[i].offset+=rest;
	  goto abort;
	}
      }
    } else break;
  }
abort:
  return total;
}