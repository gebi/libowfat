#include "socket.h"
#ifndef __MINGW32__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif
#include <errno.h>

int socket_fastopen(int s) {
#ifdef TCP_FASTOPEN
  return setsockopt(s,SOL_TCP,TCP_FASTOPEN,(int[]){ 5 }, sizeof(int));
#else
#ifdef ENOPROTOOPT
  errno=ENOPROTOOPT;
#else
  errno=ENOSYS;
#endif
  return -1;
#endif
}
