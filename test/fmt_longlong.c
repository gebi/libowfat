#include <fmt.h>
#include <str.h>
#include <assert.h>

main() {
  char buf[1024];

  assert(fmt_longlong(0,12345)==5);
  assert(fmt_longlong(0,-12345)==6);
  assert(fmt_longlong(buf,12345)==5); buf[5]=0;
  assert(str_equal(buf,"12345"));
  assert(fmt_longlong(buf,-12345)==6); buf[6]=0;
  assert(str_equal(buf,"-12345"));

  assert(fmt_longlong(0,1234567890)==10);
  assert(fmt_longlong(0,-1234567890)==11);
  assert(fmt_longlong(buf,1234567890)==10); buf[10]=0;
  assert(str_equal(buf,"1234567890"));
  assert(fmt_longlong(buf,-1234567890)==11); buf[11]=0;
  assert(str_equal(buf,"-1234567890"));
}
