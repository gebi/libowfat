#include <assert.h>
#include "array.h"
#include "byte.h"

main() {
  static array x,y;
  array_cats(&x,"fnord");
  array_cats(&y,"foobar");
  array_cat(&x,&y);
  array_fail(&y);
  array_cat(&y,&x);
  assert(byte_equal(x.p,11,"fnordfoobar"));
}
