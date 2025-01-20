#include "havealloca.h"
#include <libowfat/fmt.h>
#include <libowfat/byte.h>
#include <assert.h>

int main() {
  char* c=fmt_strm_alloca("foo"," bar","\n");
  assert(byte_equal(c,sizeof("foo bar\n"),"foo bar\n"));
}
