#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"
#include "ip6.h"
#include "haveip6.h"
#include "error.h"

int socket_recv6(int s,char *buf,unsigned int len,char ip[16],uint16 *port,uint32 *scope_id)
{
#ifdef LIBC_HAS_IP6
  struct sockaddr_in6 si;
#else
  struct sockaddr_in si;
#endif
  unsigned int Len = sizeof si;
  int r;

  byte_zero(&si,Len);
  if ((r = recvfrom(s,buf,len,0,(struct sockaddr *) &si,&Len))<0) return -1;

#ifdef LIBC_HAS_IP6
  if (noipv6) {
    struct sockaddr_in *si4=(struct sockaddr_in *)&si;
    byte_copy(ip,12,V4mappedprefix);
    byte_copy(ip+12,4,(char *) &si4->sin_addr);
    uint16_unpack_big((char *) &si4->sin_port,port);
    return r;
  }
  byte_copy(ip,16,(char *) &si.sin6_addr);
  uint16_unpack_big((char *) &si.sin6_port,port);
  if (scope_id) *scope_id=si.sin6_scope_id;
#else
  byte_copy(ip,12,(char *)V4mappedprefix);
  byte_copy(ip+12,4,(char *) &si.sin_addr);
  uint16_unpack_big((char *) &si.sin_port,port);
  if (scope_id) *scope_id=0;
#endif

  return r;
}
