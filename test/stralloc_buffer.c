#include "stralloc.h"
#include "buffer.h"

main() {
  static stralloc sa;
  /* static makes sure sa is initialized and empty;
   * use stralloc_init to initialize and stralloc_copys(&sa,"") to empty */
  if (buffer_get_token_sa(buffer_0,&sa," \t\n",3)==0) {
    buffer_putsa(buffer_1,&sa);
    buffer_putnlflush(buffer_1);
  }
}
