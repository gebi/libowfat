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
  struct msghdr msg = {0};
  struct cmsghdr *cmsg;
  struct iovec  iov;
  char buf[CMSG_SPACE(sizeof(int))];
  iov.iov_len=1;
  iov.iov_base="x";
  msg.msg_control = buf;
  msg.msg_controllen = sizeof buf;
  msg.msg_iov=&iov;
  msg.msg_iovlen=1;
  msg.msg_name=0;
  msg.msg_namelen=0;
  cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(sizeof(int));
  *((int*)CMSG_DATA(cmsg))=fd;
  msg.msg_controllen = cmsg->cmsg_len;
  return sendmsg(sock,&msg,0);
}
