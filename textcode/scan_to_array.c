#include "str.h"
#include "array.h"
#include "textcode.h"

unsigned long scan_to_array(unsigned long (*func)(const char*,char*,unsigned long*),
			    const char* src,array* dest) {
  unsigned long scanned;
  unsigned long needed=str_len(src);
  char* x=((char*)array_start(dest))+array_bytes(dest);
  if (!array_allocate(dest,1,array_bytes(dest)+needed-1)) return 0;
  return func(src,x,&scanned);
}
