#include <stdarg.h>
#include <sys/uio.h>
#include "errmsg.h"
#include "str.h"
#include <string.h>
#include <errno.h>

extern int errmsg_cvt(struct iovec* x,const char* message, va_list a);

void errmsg_warnsys(const char* message, ...) {
  struct iovec x[25];
  va_list a;
  int i;
  va_start(a,message);
  i=errmsg_cvt(x,message,a);

  x[i-1].iov_base=": ";
  x[i-1].iov_len=2;

  x[i].iov_base=strerror(errno);
  x[i].iov_len=str_len(x[i].iov_base);

  x[i+1].iov_base="\n";
  x[i+1].iov_len=1;

  writev(2,x,i+2);
  va_end(a);
}
