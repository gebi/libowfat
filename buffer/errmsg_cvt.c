#include <stdarg.h>
#include <sys/uio.h>
#include "errmsg.h"
#include "str.h"

/* the people who defined stdarg.h need to be taken behind the barn and shot */
int errmsg_cvt(struct iovec* x,const char* message, va_list a) {
  int i,j;
  j=0;
  if (argv0) {
    x[0].iov_base=(char*)argv0;
    x[0].iov_len=str_len(argv0);
    x[1].iov_base=": ";
    x[1].iov_len=2;
    j=2;
  }
  x[j].iov_base=(char*)message; x[j].iov_len=str_len(message); ++j;

  for (i=0; j<22; ++i) {
    const char* t=va_arg(a,const char*);
    if (!t) break;
    x[j].iov_base=(char*)t;
    x[j].iov_len=str_len(t);
    ++j;
  }
  x[j].iov_base="\n";
  x[j].iov_len=1;
  return j+1;
}
