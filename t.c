#include "fmt.h"
#include "scan.h"
#include "str.h"
#include "uint16.h"
#include "uint32.h"
#include "stralloc.h"
#include "socket.h"
#include "buffer.h"
#include "ip4.h"
#include "mmap.h"
#include "open.h"
#include "textcode.h"
#include <stdio.h>
#include <unistd.h>

#define rdtscl(low) \
     __asm__ __volatile__ ("rdtsc" : "=a" (low) : : "edx")

int main(int argc,char* argv[]) {
  unsigned long size;
  char* buf=mmap_read(argv[1],&size);
  if (buf) {
    unsigned int x=fmt_yenc(0,buf,size);
    unsigned int y;
    char* tmp=malloc(x+1);
    y=fmt_yenc(tmp,buf,size);
    write(1,tmp,x);
  }
#if 0
  char buf[100];
  char buf2[100];
  unsigned int len,len2;
  buf[fmt_yenc(buf,"http://localhost/~fefe",22)]=0;
  buffer_puts(buffer_1,buf);
  buffer_putsflush(buffer_1,"\n");
  if ((buf[len2=scan_yenc(buf,buf2,&len)])!='\n') {
    buffer_putsflush(buffer_2,"parse error!\n");
    return 1;
  }
  buffer_put(buffer_1,buf2,len2);
  buffer_putsflush(buffer_1,"\n");
  return 0;
#endif
#if 0
  char buf[100];
  char buf2[100];
  unsigned int len,len2;
  buf[fmt_base64(buf,"foo:bar",7)]=0;
  buffer_puts(buffer_1,buf);
  buffer_putsflush(buffer_1,"\n");
  if ((buf[len2=scan_base64(buf,buf2,&len)])!=0) {
    buffer_putsflush(buffer_2,"parse error!\n");
    return 1;
  }
  buffer_put(buffer_1,buf2,len2);
  buffer_putsflush(buffer_1,"\n");
  return 0;
#endif
#if 0
  unsigned long size;
  char* buf=mmap_read(argv[1],&size);
  if (buf) {
    unsigned int x=fmt_uuencoded(0,buf,size);
    unsigned int y;
    char* tmp=malloc(x+1);
    y=fmt_uuencoded(tmp,buf,size);
    write(1,tmp,x);
  }
#endif
#if 0
  char buf[]="00000000000000000000000000000001";
  char ip[16];
  if (scan_ip6_flat(buf,ip) != str_len(buf))
    buffer_putsflush(buffer_2,"parse error!\n");
#endif
#if 0
  int fd=open_read("t.c");
  buffer b;
  char buf[1024];
  char line[20];
  int i;
  buffer_init(&b,read,fd,buf,1024);
  i=buffer_getline(&b,line,19);
  buffer_puts(buffer_1,"getline returned ");
  buffer_putulong(buffer_1,i);
  buffer_puts(buffer_1,"\n");
  buffer_puts(buffer_1,line);
  buffer_flush(buffer_1);
#endif
#if 0
  buffer_putulong(buffer_1,23);
//  buffer_putspace(buffer_1);
  buffer_putsflush(buffer_1,"\n");
//  buffer_flush(buffer_1);
#endif
#if 0
  long a,b,c;
  char buf[4096];
  char buf2[4096];
  memcpy(buf,buf2,4096);
  byte_copy(buf,4096,buf2);
  rdtscl(a);
  memcpy(buf,buf2,4096);
  rdtscl(b);
  byte_copy(buf,4096,buf2);
  rdtscl(c);
  printf("memcpy: %d - byte_copy: %d\n",b-a,c-b);
#endif
#if 0
  char ip[16];
  int i;
  if ((i=scan_ip6(argv[1],ip))) {
    char buf[128];
    buf[fmt_ip6(buf,ip)]=0;
    puts(buf);
  }
#endif
#if 0
  char buf[100];
  strcpy(buf,"foobarbaz");
  buf[fmt_fill(buf,3,5,100)]=0;
  printf("\"%s\"\n",buf);
#endif
#if 0
  unsigned long len;
  char *c=mmap_read("/etc/passwd",&len);
  printf("got map %p of len %lu\n",c,len);
#endif
#if 0
  char c;
  printf("%d\n",buffer_getc(buffer_0,&c));
  printf("%c\n",c);
#endif
#if 0
  char buf[100]="01234567890123456789012345678901234567890123456789";
  long a,b,c;
#endif
#if 0
  buf[ip4_fmt(buf,ip4loopback)]=0;
  buffer_puts(buffer_1small,buf);
  buffer_flush(buffer_1small);
#endif

#if 0
  buf[0]=0;
  buf[fmt_8long(buf,0)]=0;
  puts(buf);
  rdtscl(a);
  c=str_len(buf);
  rdtscl(b);
  /*byte_zero_djb(buf,j); */
//  printf("\n%lu %d\n",b-a,c);
#endif
#if 0
  buffer_puts(buffer_1small,"hello, world\n");
  buffer_flush(buffer_1small);
#endif
#if 0
  int s=socket_tcp4();
  char ip[4]={127,0,0,1};
  int t=socket_connect4(s,ip,80);
#endif
#if 0
  char buf[100]="foo bar baz fnord   ";
  char buf2[100]="foo braz fnord";
  long a,b,c;
  long i=0,j=0,k=0;
  double d;
  uint32 l,m,n;
  stralloc sa={0};
  stralloc_copys(&sa,"fnord");
  stralloc_catlong0(&sa,-23,5);
  stralloc_append(&sa,"\n");
  printf("%d %d\n",str_equal("fnord","fnord1"),str_equal("fnord1","fnord"));
  write(1,sa.s,sa.len);
  printf("%d %d\n",stralloc_starts(&sa,"fnord"),stralloc_starts(&sa,"fnord\na"));

  l=0xdeadbeef;
  uint32_pack_big((char*)&m,l);
  uint32_unpack_big((char*)&m,&n);
  printf("%x %x %x\n",l,m,n);

  rdtscl(a);
/*  i=scan_double("3.1415",&d); */
  rdtscl(b);
  /*byte_zero_djb(buf,j); */
  rdtscl(c);
  printf("%lu %lu\n",b-a,c-b);
#endif
}

