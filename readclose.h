#ifndef READCLOSE_H
#define READCLOSE_H

#include "stralloc.h"

extern int readclose_append(int fd,stralloc *buf,unsigned int initlen);
extern int readclose(int fd,stralloc *buf,unsigned int initlen);

#endif
