#include "rangecheck.h"
#include <assert.h>
#include <stdio.h>

int main() {
  char buf[1000];

  /* does range_ptrinbuf check all the incoming pointer cases right? */
  assert(range_ptrinbuf(buf,sizeof(buf),0)==0);
  assert(range_ptrinbuf(buf,sizeof(buf),buf-1)==0);
  assert(range_ptrinbuf(buf,sizeof(buf),buf)==1);
  assert(range_ptrinbuf(buf,sizeof(buf),buf+50)==1);
  assert(range_ptrinbuf(buf,sizeof(buf),buf+sizeof(buf))==0);

  /* what if we give it an invalid buffer? */
  assert(range_ptrinbuf(0,sizeof(buf),0)==0);
  assert(range_ptrinbuf(buf,(unsigned long)-1,buf+1)==0);

  /* see if range_validbuf works */
  assert(range_validbuf(buf,sizeof(buf))==1);
  assert(range_validbuf(0,sizeof(buf))==0);
  assert(range_validbuf(buf,(unsigned long)-1)==0);

  /* range_bufinbuf */
  assert(range_bufinbuf(buf,sizeof(buf),buf,sizeof(buf))==1);
  assert(range_bufinbuf(buf,sizeof(buf),buf,sizeof(buf)+1)==0);
  assert(range_bufinbuf(buf,sizeof(buf),buf-1,sizeof(buf))==0);
  assert(range_bufinbuf(buf-1,sizeof(buf)+1,buf,sizeof(buf))==1);
  assert(range_bufinbuf(buf-1,sizeof(buf)+2,buf,sizeof(buf))==1);
  assert(range_bufinbuf(0,sizeof(buf),(void*)1,1)==0);
  assert(range_bufinbuf(buf,(unsigned long)-1,buf,1)==0);
  /* the quintessential integer overflow exploit scenario */
  assert(range_bufinbuf(buf,sizeof(buf),buf+10,(unsigned long)-5)==0);
  assert(range_bufinbuf(buf,sizeof(buf),buf+10,sizeof(buf))==0);
  assert(range_bufinbuf(buf,sizeof(buf),buf+10,sizeof(buf)-10)==1);

  /* range_arrayinbuf */
  assert(range_arrayinbuf(buf,sizeof(buf),buf,1,10)==1);
  assert(range_arrayinbuf(buf,sizeof(buf),buf+sizeof(buf)-10,1,10)==1);
  assert(range_arrayinbuf(buf,sizeof(buf),buf-1,1,10)==0);
  assert(range_arrayinbuf(buf,sizeof(buf),buf+1,1,1000)==0);
  assert(range_arrayinbuf(buf,sizeof(buf),buf-1,1,1002)==0);
  /* now overflow the array parameters */
  assert(range_arrayinbuf(buf,sizeof(buf),buf+10,0x10000,0x10000)==0);
  assert(range_arrayinbuf(buf,sizeof(buf),buf+10,0x80000000,2)==0);
  assert(range_arrayinbuf(buf,sizeof(buf),buf+10,0xffffffff,1)==0);

  /* range_strinbuf */
  assert(range_strinbuf(buf,sizeof(buf),0)==0);
  assert(range_strinbuf(buf,sizeof(buf),buf+sizeof(buf))==0);
  {
    char* x="fnord";
    assert(range_strinbuf(x,5,x)==0);
    assert(range_strinbuf(x,6,x)==1);
    assert(range_strinbuf(x,6,x+5)==1);
    assert(range_strinbuf(x,6,x+6)==0);
  }

  /* range_str2inbuf */
  assert(range_str2inbuf(buf,sizeof(buf),0)==0);
  assert(range_str2inbuf(buf,sizeof(buf),buf+sizeof(buf))==0);
  {
    uint16_t y[6];
    int i;
    for (i=0; i<7; ++i) y[i]="fnord"[i];
    assert(range_str2inbuf(y,5*2,y)==0);
    assert(range_str2inbuf(y,5*2+1,y)==0);
    assert(range_str2inbuf(y,sizeof(y),y)==1);
    assert(range_str2inbuf(y,sizeof(y),y+5)==1);
    assert(range_str2inbuf(y,sizeof(y),y+6)==0);
  }

  /* range_str4inbuf */
  assert(range_str4inbuf(buf,sizeof(buf),0)==0);
  assert(range_str4inbuf(buf,sizeof(buf),buf+sizeof(buf))==0);
  {
    uint32_t y[6];
    int i;
    for (i=0; i<7; ++i) y[i]="fnord"[i];
    assert(range_str4inbuf(y,5*4,y)==0);
    assert(range_str4inbuf(y,5*4+3,y)==0);
    assert(range_str4inbuf(y,sizeof(y),y)==1);
    assert(range_str4inbuf(y,sizeof(y),y+5)==1);
    assert(range_str4inbuf(y,sizeof(y),y+6)==0);
  }


  puts("all tests ok");
  return 0;
}
