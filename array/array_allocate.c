#include "array.h"

#if 0
      static array x;
      t *p;
      int64 pos;

      p = array_allocate(&x,sizeof(t),pos);

   array_allocate makes sure that enough bytes are allocated in x for at
   least pos+1 objects of type t. (The size of t must be positive;
   otherwise the effects are undefined.) If not enough bytes are
   allocated (or x is unallocated), array_allocate allocates more bytes,
   moving the dynamically allocated region if necessary. array_allocate
   often allocates somewhat more bytes than necessary, to save time
   later.

   array_allocate then makes sure that the number of bytes initialized
   covers at least those pos+1 objects. If not enough bytes are
   initialized, array_allocate initializes more bytes (setting them to
   0), up to exactly the end of the pos+1st object.

   array_allocate then returns a pointer to the pos+1st object; i.e.,
   object number pos, with objects numbered starting at 0. This pointer
   can be used to change or inspect the object. The pointer can continue
   to be used through subsequent calls to array_get, array_start,
   array_length, and array_bytes, but it must not be used after any
   other operations on this array.

   If something goes wrong, array_allocate returns 0, setting errno
   appropriately, without touching x. In particular, array_allocate
   returns 0 if

     * x has failed, or
     * pos is negative, or
     * not enough memory is available.

   array_allocate does not change x to have failed; if you want to do
   that, use array_fail.
#endif

void* array_allocate(array* x,int64 membersize,int64 pos) {
  int64 wanted;
  if (membersize<128)
    wanted=(pos+127)&(-128ll);	/* round up to multiple of 128 */
  else
    wanted=(pos+4095)&(-4096ll); /* round up to 4k pages */
  /* detect numeric overflow */
  if (wanted<0) return 0;
  wanted=membersize*(pos+1);
}
