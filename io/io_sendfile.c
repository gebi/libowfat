/* http://delegate.uec.ac.jp:8081/club/mma/~shimiz98/misc/sendfile.html */
#define _FILE_OFFSET_BITS 64
#include "io_internal.h"
#include "havebsdsf.h"
#include "havesendfile.h"

#if defined(HAVE_BSDSENDFILE)
#define SENDFILE 1
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

int64 io_sendfile(int64 s,int64 fd,uint64 off,uint64 n) {
  off_t sbytes;
  int r=sendfile(fd,s,off,n,0,&sbytes,0);
  if (r==-1)
    return (errno==EAGAIN?(sbytes?sbytes:-1):-3);
  return n;
}

#elif defined(HAVE_SENDFILE)

#ifdef __hpux__

#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>

int64 io_sendfile(int64 out,int64 in,uint64 off,uint64 bytes) {
  return sendfile64(out,in,off,bytes,0,0);
}

#elif defined (__sun__) && defined(__svr4__)

#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>

int64 io_sendfile(int64 out,int64 in,uint64 off,uint64 bytes) {
  off64_t o=off;
  return sendfile64(out,in,&o,bytes);
}

#elif defined(_AIX)

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

int64 io_sendfile(int64 out,int64 in,uint64 off,uint64 bytes) {
  struct sf_parms p;
  int destfd=out;
  p.header_data=0;
  p.header_length=0;
  p.file_descriptor=in;
  p.file_offset=off;
  p.file_bytes=bytes;
  p.trailer_data=0;
  p.trailer_length=0;
  if (send_file(&destfd,&p,0)>=0)
    return p.bytes_sent;
  if (errno==EAGAIN)
    return -1;
  else
    return -3;
}

#elif defined(__linux__)

#if defined(__GLIBC__)
#include <sys/sendfile.h>
#elif defined(__dietlibc__)
#include <sys/sendfile.h>
#else
#include <linux/unistd.h>
_syscall4(int,sendfile,int,out,int,in,long *,offset,unsigned long,count)
#endif

int64 io_sendfile(int64 s,int64 fd,uint64 off,uint64 n) {
  off_t o=off;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),s);
  off_t i=sendfile(s,fd,&o,n);
  if (i==-1) {
    if (e) {
      e->canwrite=0;
      e->next_write=-1;
    }
    if (errno!=EAGAIN) i=-3;
  }
  return i;
}
#endif

#else

#include <unistd.h>
#include <sys/mman.h>

#define BUFSIZE 16384

int64 io_sendfile(int64 out,int64 in,uint64 off,uint64 bytes) {
  char buf[BUFSIZE];
  int n,m;
  uint64 sent=0;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),out);
  if (e) {
    const char* c;
    long left;
    do {
      if (e->mmapped) {
	/* did we already map the right chunk? */
	if (off>=e->mapofs && off<e->mapofs+e->maplen)
	  goto mapok;	/* ok; mmapped the right chunk*/
	munmap(e->mmapped,e->maplen);
      }
      e->mapofs=off&0xffffffffffff0000ull;
      if (e->mapofs+0x10000>off+bytes)
	e->maplen=off+bytes-e->mapofs;
      else
	e->maplen=0x10000;
      if ((e->mmapped=mmap(0,e->maplen,PROT_READ,MAP_SHARED,in,e->mapofs))==MAP_FAILED) {
	e->mmapped=0;
	goto readwrite;
      }
  mapok:
      c=(const char*)(e->mmapped)+(off&0xffff);
      left=e->maplen-(off&0xffff);
      while (left>0) {
	m=write(out,c,left);
	if (m==-1) {
	  e->canwrite=0;
	  e->next_write=-1;
	  return errno==EAGAIN?(sent?sent:-1):-3;
	}
	if (m==0) return sent;
	sent+=m;
	left-=m;
	bytes-=m;
	off+=m;
      }
    } while (bytes);
    return sent;
  }
readwrite:
  if (lseek(in,off,SEEK_SET) != off)
    return -1;
  while (bytes>0) {
    char* tmp=buf;
    if ((n=read(in,tmp,(bytes<BUFSIZE)?bytes:BUFSIZE))<=0)
      return (sent?sent:-1);
    while (n>0) {
      if ((m=write(out,tmp,n))<0) {
	if (m==-1) {
	  if (e) {
	    e->canwrite=0;
	    e->next_write=-1;
	  }
	  return errno==EAGAIN?(sent?sent:-1):-3;
	}
	goto abort;
      }
      sent+=m;
      n-=m;
      tmp+=m;
    }
  }
abort:
  return sent;
}

#endif
