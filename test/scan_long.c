#include <scan.h>
#include <fmt.h>
#include <buffer.h>
#include <assert.h>

int main() {
  char buf[1024];
  unsigned long long int i;
  if (sizeof(unsigned long) != 4)
    return 0;
  for (i=1; i<0xfffffffffull; i+=i+1) {
    int k;
    unsigned long test;
    buf[k=fmt_ulonglong(buf,i)]=0;
    buffer_puts(buffer_1,buf); buffer_putnlflush(buffer_1);
    if (buf[scan_ulong(buf,&test)])
      /* scan did not like the whole number */
      assert(i>0xffffffffull);
    else
      assert(i<=0xffffffffull);
  }
  return 0;
}
