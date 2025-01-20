#include <libowfat/array.h>
#include <libowfat/str.h>

void array_cats(array* to,const char* from) {
  array_catb(to,from,str_len(from));
}
