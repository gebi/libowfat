#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"
#include "ip6.h"
#include "haveip6.h"
#include "error.h"

int socket_accept6(int s,char ip[16],uint16 *port,uint32 *scope_id)
{
#ifdef LIBC_HAS_IP6
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif
  unsigned int dummy = sizeof sa;
  int fd;

  fd = accept(s,(struct sockaddr *) &sa,&dummy);
  if (fd == -1) return -1;

#ifdef LIBC_HAS_IP6
  if (sa.sin6_family==AF_INET) {
    struct sockaddr_in *sa4=(struct sockaddr_in*)&sa;
    byte_copy(ip,12,V4mappedprefix);
    byte_copy(ip+12,4,(char *) &sa4->sin_addr);
    uint16_unpack_big((char *) &sa4->sin_port,port);
    return fd;
  }
  byte_copy(ip,16,(char *) &sa.sin6_addr);
  uint16_unpack_big((char *) &sa.sin6_port,port);
  if (scope_id) *scope_id=sa.sin6_scope_id;

  return fd;
#else
  byte_copy(ip,12,V4mappedprefix);
  byte_copy(ip+12,4,(char *) &sa.sin_addr);
  uint16_unpack_big((char *) &sa.sin_port,port);
  if (scope_id) *scope_id=0;
  return fd;
#endif
}
