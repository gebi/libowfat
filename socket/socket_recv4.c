#include <sys/types.h>
#include <sys/param.h>
#ifndef __MINGW32__
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "windoze.h"
#include "socket.h"
#include "havesl.h"

int socket_recv4(int s,char *buf,unsigned int len,char ip[4],uint16 *port) {
  struct sockaddr_in si;
  socklen_t Len = sizeof si;
  int r;

  if ((r = recvfrom(s,buf,len,0,(struct sockaddr *) &si,&Len))<0) return winsock2errno(-1);
  if (ip) *(uint32*)ip = *(uint32*)&si.sin_addr;
  if (port) uint16_unpack_big((char *) &si.sin_port,port);
  return r;
}
