#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include "havealloca.h"
#include "iob_internal.h"
#include "havebsdsf.h"

int64 iob_send(int64 s,io_batch* b) {
  iob_entry* e,* last;
  struct iovec* v;
  int64 total,sent;
  long i;
  long headers;
#ifdef HAVE_BSDSENDFILE
#define SENDFILE 1
  long trailers;
#endif

  if (b->bytesleft==0) return 0;
  last=(iob_entry*)(((char*)array_start(&b->b))+array_bytes(&b->b));
  v=alloca(b->bufs*sizeof(struct iovec));
  total=0;
  for (;;) {
    if (!(e=array_get(&b->b,sizeof(iob_entry),b->next)))
      return -3;		/* can't happen error */
#ifdef HAVE_BSDSENDFILE
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
#ifdef HAVE_BSDSENDFILE
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
      else if (r==-1 && errno==EAGAIN) {
	if (!(sent=sbytes)) {
	  sent=-1;
	  goto eagain;
	}
      } else
	sent=-3;
    } else {
      sent=writev(s,v,headers);
      if (sent==-1) {
	if (errno!=EAGAIN)
	  sent=-3;
	else {
eagain:
	  io_eagain(s);
	  return -1;
	}
      }
    }
#else
#ifdef TCP_CORK
    if (b->bufs && b->files && !b->next) {
      static int one=1;
      setsockopt(s,IPPROTO_TCP,TCP_CORK,&one,sizeof(one));
    }
#endif
    if (headers) {
      sent=writev(s,v,headers);
      if (sent==-1) {
	if (errno==EAGAIN) {
	  io_eagain(s);
	  return -1;
	}
	sent=-3;
      }
    } else
      sent=io_sendfile(s,e->fd,e->offset,e->n);
#endif
    if (sent>0)
      total+=sent;
    else
      return total?total:sent;
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
	if (e[i].n<=rest) {
	  rest-=e[i].n;
	  ++b->next;
	  if (!rest) break;
	} else {
	  e[i].offset+=rest;
	  e[i].n-=rest;
	  goto abort;
	}
      }
    } else break;
  }
abort:
  return total;
}
