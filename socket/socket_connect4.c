#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "byte.h"
#include "socket.h"
#include "uint16.h"
#include "uint32.h"

int socket_connect4(int s,const char *ip,uint16 port) {
  struct sockaddr_in si;
  byte_zero(&si,sizeof(si));
  si.sin_family=AF_INET;
  uint16_pack_big((char*) &si.sin_port,port);
  *((uint32*)&si.sin_addr) = *((uint32*)ip);
  return (connect(s,(struct sockaddr*)&si,sizeof(si)));
}
