#ifndef SOCKET_H
#define SOCKET_H

#include "uint16.h"
#include "uint32.h"

int socket_tcp4(void);
int socket_udp4(void);
int socket_tcp6(void);
int socket_udp6(void);

#define socket_tcp() socket_tcp4()
#define socket_udp() socket_udp4()

int socket_connect4(int s,const char* ip,uint16 port);
int socket_connect6(int s,const char* ip,uint16 port,uint32 scope_id);
int socket_connected(int s);
int socket_bind4(int s,const char* ip,uint16 port);
int socket_bind4_reuse(int s,const char* ip,uint16 port);
int socket_bind6(int s,const char* ip,uint16 port,uint32 scope_id);
int socket_bind6_reuse(int s,const char* ip,uint16 port,uint32 scope_id);
int socket_listen(int s,unsigned int backlog);
int socket_accept4(int s,char* ip,uint16* port);
int socket_accept6(int s,char* ip,uint16* port,uint32* scope_id);
int socket_recv4(int s,char* buf,unsigned int len,char* ip,uint16* port);
int socket_recv6(int s,char* buf,unsigned int len,char* ip,uint16* port,uint32* scope_id);
int socket_send4(int s,const char* buf,unsigned int len,const char* ip,uint16 port);
int socket_send6(int s,const char* buf,unsigned int len,const char* ip,uint16 port,uint32 scope_id);
int socket_local4(int s,char* ip,uint16* port);
int socket_local6(int s,char* ip,uint16* port,uint32* scope_id);
int socket_remote4(int s,char* ip,uint16* port);
int socket_remote6(int s,char* ip,uint16* port,uint32* scope_id);

/* enable sending udp packets to the broadcast address */
int socket_broadcast(int s);
/* join a multicast group on the given interface */
int socket_mcjoin4(int s,const char* groupip,const char* interface);
int socket_mcjoin6(int s,const char* groupip,int interface);
/* leave a multicast group on the given interface */
int socket_mcleave4(int s,const char* groupip);
int socket_mcleave6(int s,const char* groupip);
/* set multicast TTL/hop count for outgoing packets */
int socket_mcttl4(int s,char hops);
int socket_mchopcount6(int s,char hops);
/* enable multicast loopback */
int socket_mcloop4(int s,char hops);
int socket_mcloop6(int s,char hops);

void socket_tryreservein(int s,int size);

const char* socket_getifname(uint32 interface);
uint32 socket_getifidx(const char* ifname);

int socket_sendfile(int out,int in,uint32 offset,uint32 bytes);

int noipv6;

#endif
