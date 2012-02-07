#ifndef READCLOSE_H
#define READCLOSE_H

#include "stralloc.h"

#ifdef __cplusplus
extern "C" {
#endif

int readclose_append(int fd,stralloc *buf,size_t initlen);
int readclose(int fd,stralloc *buf,size_t initlen);

#ifdef __cplusplus
}
#endif

#endif
