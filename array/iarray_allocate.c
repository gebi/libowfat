#include "likely.h"
#include <stdlib.h>
#include "iarray.h"

void* iarray_allocate(iarray* ia,size_t pos) {
  size_t y;
  /* first the easy case without locking */
  if (__likely((y=pos/ia->elemperpage) < ia->pagefence && ia->pages[y]))
    return ia->pages[y]+(pos%ia->elemperpage)*ia->elemsize;
  /* the case where ia->pages == NULL is implicit */

  pthread_mutex_lock(&ia->m);

  if (__unlikely(y >= ia->pagefence)) {
    char** np;
    /* The data structure is an array of pointer to pages.
     * Each page holds at least one element of the array.
     * Here we realloc the array of pointers.  Each element in this
     * array is only 4 or 8 bytes, so we should allocate a few more than
     * we need to cut down on future reallocs. */
    size_t z=(y+512)&-512;		/* round up to multiple of 512 */
    /* It may seem as if there can be no integer overflow in the
     * indirect index, because then the array would not fit into the
     * address space in the first place, but remember that this is a
     * sparse array.  Someone might just pass in an unreasonable large
     * index and have large elements, too */
    if (z==0) goto unlockandfail;	/* integer overflow */
    np=realloc(ia->pages,z*ia->bytesperpage);
    if (!np) goto unlockandfail;
    ia->pagefence=z;
    ia->pages=np;
  }

  /* at this point we know the slot exists */
  /* through a race between the early-out above and the
   * pthread_mutex_lock, the page pointer to it could be non-NULL,
   * however */
  if (__unlikely(ia->pages[y]==0 && (ia->pages[y]=malloc(ia->bytesperpage))==0)) {
unlockandfail:
    pthread_mutex_unlock(&ia->m);
    return 0;
  }

  pthread_mutex_unlock(&ia->m);

  return ia->pages[y] + (pos%ia->elemperpage)*ia->elemsize;
}
