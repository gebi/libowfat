#include <stdarg.h>
#include <sys/types.h>
#ifdef __MINGW32__
#include "windows.h"
#include <unistd.h>

struct iovec {
  LPCVOID iov_base;
  DWORD iov_len;
};
#else
#include <sys/uio.h>
#endif
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

#ifdef __MINGW32__
  {
    int j;
    for (j=0; j<i+2; ++j)
      write(2,x[j].iov_base,x[j].iov_len);
  }
#else
  writev(2,x,i+2);
#endif
}
