#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"

int socket_send4(int s,const char *buf,unsigned int len,const char ip[4],uint16 port) {
  struct sockaddr_in si;

  byte_zero(&si,sizeof si);
  si.sin_family = AF_INET;
  uint16_pack_big((char*) &si.sin_port,port);
  *((uint32*)&si.sin_addr) = *((uint32*)ip);
  return sendto(s,buf,len,0,(struct sockaddr *) &si,sizeof si);
}
