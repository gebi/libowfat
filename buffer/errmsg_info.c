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

extern int errmsg_cvt(struct iovec* x,const char* message, va_list a);

void errmsg_info(const char* message, ...) {
  struct iovec x[23];
  va_list a;
  va_start(a,message);
#ifdef __MINGW32__
  {
    int i,j;
    j=errmsg_cvt(x,message,a);
    for (i=0; i<j; ++i)
      write(1,x[i].iov_base,x[i].iov_len);
  }
#else
  writev(1,x,errmsg_cvt(x,message,a));
#endif
  va_end(a);
}
