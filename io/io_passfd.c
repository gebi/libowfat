#ifdef __MINGW32__
#include "io_internal.h"
#include <errno.h>
int io_passfd(int64 sock,int64 fd) {
  errno=EINVAL;
  return -1;
}
#else

#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include "io_internal.h"

union fdmsg {
  struct cmsghdr h;
  /* on NetBSD, CMSG_SPACE is not constant */
/*  char buf[CMSG_SPACE(sizeof(int))]; */
  char buf[1000];
};

int io_passfd(int64 sock,int64 fd) {
  struct msghdr msg;
  struct iovec  iov;
#ifdef CMSG_LEN
  struct cmsghdr *cmsg;
  char buf[CMSG_SPACE(sizeof(int))];
#endif
  iov.iov_len=1;
  iov.iov_base="x";
  msg.msg_iov=&iov;
  msg.msg_iovlen=1;
  msg.msg_name=0;
  msg.msg_namelen=0;
#ifdef CMSG_LEN
  msg.msg_control = buf;
  msg.msg_controllen = sizeof(buf);
  cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(sizeof(int));
  msg.msg_controllen = cmsg->cmsg_len;
  *((int*)CMSG_DATA(cmsg))=fd;
#else
  msg.msg_accrights = (char*)&fd;
  msg.msg_accrightslen = sizeof(fd);
#endif
  return sendmsg(sock,&msg,0)>=0?0:-1;
}
#endif
