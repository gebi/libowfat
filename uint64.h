#ifndef UINT64_H
#define UINT64_H

#include <endian.h>

#if __WORDSIZE == 64
typedef unsigned long uint64;
#else
typedef unsigned long long uint64;
#endif

#endif
