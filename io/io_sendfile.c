#define _FILE_OFFSET_BITS 64
#include "io_internal.h"

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

int64 io_sendfile(int64 s,int64 fd,uint64 off,uint64 n) {
  off_t sbytes;
  int r=sendfile(fd,s,off,n,0,&sbytes,0);
  if (r==0) return n;
  if (r==-1)
    return (errno==EAGAIN?sbytes:-1);
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
  return sendfile(s,fd,&o,n);
}

#else

#define BUFSIZE 16384

int64 io_sendfile(int64 out,int64 in,uint64 off,uint64 bytes) {
  char buf[BUFSIZE];
  int n,m;
  uint64 sent=0;
  if (lseek(in,off,SEEK_SET) != off)
    return -1;
  while (bytes>0) {
    char* tmp=buf;
    if ((n=read(in,tmp,(bytes<BUFSIZE)?bytes:BUFSIZE))<=0)
      return (sent?sent:-1);
    while (n>0) {
      if ((m=write(out,tmp,n))<0)
	goto abort;
      sent+=m;
      n-=m;
      tmp+=m;
    }
  }
abort:
  return sent;
}

#endif
