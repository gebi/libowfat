#include <libowfat/fmt.h>
#include <libowfat/byte.h>
#include <assert.h>

int main() {
  char buf[100];
  assert(fmt_xmlescape(buf,0)==0);
  assert(fmt_xmlescape(buf,23)==1);
  assert(fmt_xmlescape(buf,2342)==fmt_utf8(NULL,2342));
  assert(fmt_xmlescape(buf,'&')==5 && byte_equal(buf,5,"&amp;"));
  assert(fmt_xmlescape(buf,'<')==4 && byte_equal(buf,4,"&lt;"));
  assert(fmt_xmlescape(buf,'\t')==1 && buf[0]==9);
  assert(fmt_xmlescape(buf,8)==5 && byte_equal(buf,5,"&#x8;"));
  return 0;
}
