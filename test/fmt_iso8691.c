#include <fmt.h>
#include <assert.h>
#include <unistd.h>

int main() {
  char buf[1024];
  write(1,buf,fmt_iso8601(buf,0));
  write(1,"\n",1);
  return 0;
}
