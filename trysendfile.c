#ifdef __hpux__
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <stdio.h>

int main() {
/*
      sbsize_t sendfile(int s, int fd, off_t offset, bsize_t nbytes,
                    const struct iovec *hdtrl, int flags);
*/
  struct iovec x[2];
  int fd=open("havesendfile.c",0);
  x[0].iov_base="header";
  x[0].iov_len=6;
  x[1].iov_base="footer";
  x[1].iov_len=6;
  sendfile(1 /* dest socket */,fd /* src file */,
           0 /* offset */, 23 /* nbytes */,
           x, 0);
  perror("sendfile");
  return 0;
}
#elif defined (__sun__) && defined(__svr4__)
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <stdio.h>

int main() {
  off_t o;
  o=0;
  sendfile(1 /* dest */, 0 /* src */,&o,23 /* nbytes */);
  perror("sendfile");
  return 0;
}
#else
#error unsupported architecture
#endif
