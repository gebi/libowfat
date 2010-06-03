/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IP4_H
#define IP4_H

unsigned int scan_ip4(const char *src,char *ip);
unsigned int fmt_ip4(char *dest,const char *ip);

/* for djb backwards compatibility */
#define ip4_scan scan_ip4
#define ip4_fmt fmt_ip4

#define IP4_FMT 20
#define FMT_IP4 20

extern const char ip4loopback[4]; /* = {127,0,0,1};*/

#endif
