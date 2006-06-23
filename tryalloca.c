#include <stdlib.h>
#ifdef A
#include <alloca.h>
#else
#include <malloc.h>
#endif

main() {
  char* c=alloca(23);
}
