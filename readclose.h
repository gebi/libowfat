#ifndef READCLOSE_H
#define READCLOSE_H

#include "stralloc.h"

int readclose_append(int fd,stralloc *buf,unsigned int initlen);
int readclose(int fd,stralloc *buf,unsigned int initlen);

#endif
