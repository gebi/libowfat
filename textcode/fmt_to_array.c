#include "array.h"
#include "textcode.h"

void fmt_to_array(unsigned long (*func)(char*,const char*,unsigned long),
		  array* a,const char* src,unsigned long len) {
  unsigned long needed=func(0,src,len);
  if (array_allocate(a,1,array_bytes(a)+needed-1)) {
    char* x=((char*)array_start(a))+array_bytes(a)-needed;
    func(x,src,len);
  } else
    array_fail(a);
}
