#include "array.h"
#include "textcode.h"
#include <assert.h>
#include <unistd.h>

array a;

int main() {
  char buf[256];
  int i;
  for (i=0; i<256; ++i) buf[i]=i;

  fmt_to_array(fmt_uuencoded,&a,buf,256);
  assert(!array_failed(&a));
  write(1,array_start(&a),array_bytes(&a));
  array_trunc(&a);

  fmt_to_array(fmt_base64,&a,buf,256);
  assert(!array_failed(&a));
  write(1,array_start(&a),array_bytes(&a)); write(1,"\n",1);
  array_trunc(&a);

  strcpy(buf,"&Auml;cht fn&ouml;rdig.");
  size_t n;
  scan_html(buf,buf,&n);
  buf[n]=0;
  puts(buf);

  return 0;
}
