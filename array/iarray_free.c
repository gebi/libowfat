#include <stdlib.h>
#include "iarray.h"

void iarray_free(iarray* ia) {
  size_t i;
  for (i=0; i<ia->pagefence; ++i)
    if (ia->pages[i]) free(ia->pages[i]);
  free(ia->pages);
}
