#include "fmt.h"
#include "scan.h"
#include "str.h"
#include "uint16.h"
#include "uint32.h"
#include "stralloc.h"
#include "socket.h"
#include "buffer.h"
#include "ip4.h"
#include <stdio.h>

#define rdtscl(low) \
     __asm__ __volatile__ ("rdtsc" : "=a" (low) : : "edx")

int main(int argc,char* argv[]) {
  char c;
  printf("%d\n",buffer_getc(buffer_0,&c));
  printf("%c\n",c);
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

