#ifndef IP4_H
#define IP4_H

extern unsigned int ip4_scan(const char *src,char *ip);
extern unsigned int ip4_fmt(char *dest,const char *ip);

#define IP4_FMT 20

#endif
