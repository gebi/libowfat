#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include "io_internal.h"

union fdmsg {
  struct cmsghdr h;
  char buf[CMSG_SPACE(sizeof(int))];
};

int64 io_receivefd(int64 sock) {
  struct iovec iov;
  struct msghdr msg;
  union fdmsg cmsg;
  struct cmsghdr* h;
  char x[100];
  char name[100];
  iov.iov_base=x;
  iov.iov_len=100;
  msg.msg_name=name;
  msg.msg_namelen=100;
  msg.msg_control=cmsg.buf;
  msg.msg_controllen=sizeof(union fdmsg);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_flags=0;
  h=CMSG_FIRSTHDR(&msg);
  h->cmsg_len=CMSG_LEN(sizeof(int));
  h->cmsg_level=SOL_SOCKET;
  h->cmsg_type=SCM_RIGHTS;
  *((int*)CMSG_DATA(h))=-1;
  if (recvmsg(sock,&msg,0)==-1)
    return -1;
  h=CMSG_FIRSTHDR(&msg);
  if (!h || h->cmsg_len!=CMSG_LEN(sizeof(int)) || h->cmsg_level!=SOL_SOCKET || h->cmsg_type!=SCM_RIGHTS) {
#ifdef EPROTO
    errno=EPROTO;
#else
    errno=EINVAL;
#endif
    return -1;
  }
  return *((int*)CMSG_DATA(h));
}
