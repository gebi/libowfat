#include <stdarg.h>
#include <sys/uio.h>
#include "errmsg.h"
#include "str.h"

extern int errmsg_cvt(struct iovec* x,const char* message, va_list a);

void errmsg_warn(const char* message, ...) {
  struct iovec x[23];
  va_list a;
  va_start(a,message);
  writev(2,x,errmsg_cvt(x,message,a));
  va_end(a);
}
