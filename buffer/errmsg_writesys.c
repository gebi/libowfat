#include <stdarg.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "errmsg.h"
#include "str.h"
#include <string.h>
#include <errno.h>

extern int errmsg_cvt(struct iovec* x,const char* message, va_list a);

void errmsg_writesys(int fd,const char* message,va_list list) {
  struct iovec x[25];
  int i;
  i=errmsg_cvt(x,message,list);

  x[i-1].iov_base=": ";
  x[i-1].iov_len=2;

  x[i].iov_base=strerror(errno);
  x[i].iov_len=str_len(x[i].iov_base);

  x[i+1].iov_base="\n";
  x[i+1].iov_len=1;

  writev(2,x,i+2);
}
