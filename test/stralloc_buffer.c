#include "stralloc.h"
#include "buffer.h"

main() {
  static stralloc sa;
  if (buffer_get_token_sa(buffer_0,&sa," \t\n",3)==0) {
    buffer_putsa(buffer_1,&sa);
    buffer_putnlflush(buffer_1);
  }
}
