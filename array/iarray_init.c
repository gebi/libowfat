#include "iarray.h"

void iarray_init(iarray* ia,size_t elemsize) {
  ia->elemsize=elemsize;
  ia->pages=0;
  ia->pagefence=0;
  if (elemsize<1024)
    ia->bytesperpage=4096;
  else if (elemsize<8192)
    ia->bytesperpage=65536;
  else
    ia->bytesperpage=elemsize;
  ia->elemperpage=ia->bytesperpage/elemsize;
  pthread_mutex_init(&ia->m,NULL);
}

