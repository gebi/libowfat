#include <stdarg.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "errmsg.h"
#include "str.h"
#include <string.h>
#include <errno.h>

extern int errmsg_cvt(struct iovec* x,const char* message, va_list a);
extern void errmsg_writesys(int fd,const char* message,va_list list);

void errmsg_infosys(const char* message, ...) {
  va_list a;
  va_start(a,message);
  errmsg_writesys(1,message,a);
  va_end(a);
}
