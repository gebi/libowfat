#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"
#include "havesl.h"

int socket_connected(int s) {
  struct sockaddr si;
#ifdef HAVE_SOCKLEN_T
  socklen_t sl=sizeof si;
#else
  int sl=sizeof si;
#endif
  if (getpeername(s,&si,&sl))
    return 0;
  return 1;
}
