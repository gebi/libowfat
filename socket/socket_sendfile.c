#include <unistd.h>
#include "socket.h"

#ifdef __linux__

#ifdef __GLIBC__
#include <sys/sendfile.h>
#else
#ifdef __dietlibc__
#include <sys/sendfile.h>
#else
#include <linux/unistd.h>
_syscall4(int,sendfile,int,out,int,in,long *,offset,unsigned long,count)
#endif
#endif

int socket_sendfile(int out,int in,uint32 offset,uint32 bytes) {
  return sendfile(out,in,&offset,bytes);
}

#else

#ifdef _HPUX_SOURCE

/* http://www.devresource.hp.com/STK/man/10.30/sendfile_2.html */
#include <sys/socket.h>
int socket_sendfile(int out,int in,uint32 offset,uint32 bytes) {
  return sendfile(out,in,offset,bytes,0,0);
}

#else

#define BUFSIZE 16384

int socket_sendfile(int out,int in,uint32 offset,uint32 bytes) {
  char buf[BUFSIZE];
  int n,m;
  int sent=0;
  if (lseek(in,offset,SEEK_SET) == -1)
    return -1;
  if ((n=read(in,buf,(bytes<BUFSIZE)?bytes:BUFSIZE))<0)
    return (sent?sent:-1);
  if ((m=write(out,buf,n))<0)
    return -1;
  return n;
}

#endif

#endif
