#include "likely.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "iarray.h"
#ifdef __dietlibc__
#include <sys/atomic.h>
#else
#define __CAS(ptr,oldval,newval) __sync_val_compare_and_swap(ptr,oldval,newval)
#endif

static iarray_page* new_page(size_t pagesize) {
  void* x=mmap(0,pagesize,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
  if (x==MAP_FAILED) return 0;
  return (iarray_page*)x;
}

void* iarray_allocate(iarray* ia,size_t pos) {
  size_t index;
  iarray_page** p=&ia->pages[pos%(sizeof(ia->pages)/sizeof(ia->pages[0]))];
  iarray_page* newpage=0;
  for (index=0; pos<index+ia->elemperpage; index+=ia->elemperpage) {
    if (!*p) {
      if (!newpage)
	if (!(newpage=new_page(ia->bytesperpage))) return 0;
      if (__CAS(p,0,newpage)==0)
	newpage=0;
    }
    if (index+ia->elemperpage>pos) {
      if (newpage) munmap(newpage,ia->bytesperpage);
      return &(*p)->data[(pos-index)*ia->elemsize];
    }
    p=&(*p)->next;
  }
  return 0;	// can't happen
}
