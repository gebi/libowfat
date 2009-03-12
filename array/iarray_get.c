#include "iarray.h"

void* iarray_get(iarray* ia,size_t pos) {
  char* x;
  size_t y;
  if (!ia->pages) return 0;
  y=pos/ia->elemperpage;
  if (y>=ia->pagefence) return 0;
  x=ia->pages[y];
  if (!x) return 0;
  return x+(pos%ia->elemperpage)*ia->elemsize;
}
