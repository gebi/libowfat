#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"
#include "havesl.h"

int socket_recv4(int s,char *buf,unsigned int len,char ip[4],uint16 *port) {
  struct sockaddr_in si;
  socklen_t Len = sizeof si;
  int r;

  if ((r = recvfrom(s,buf,len,0,(struct sockaddr *) &si,&Len))<0) return -1;
  *(uint32*)ip = *(uint32*)&si.sin_addr;
  uint16_unpack_big((char *) &si.sin_port,port);
  return r;
}
