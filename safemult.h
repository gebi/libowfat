/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef _SAFEMULT_H
#define _SAFEMULT_H

#include "uint16.h"
#include "uint32.h"
#include "uint64.h"

/* return 0 for overflow, 1 for ok */
int umult16(uint16 a,uint16 b,uint16* c);
int imult16( int16 a, int16 b, int16* c);

int umult32(uint32 a,uint32 b,uint32* c);
int imult32( int32 a, int32 b, int32* c);

int umult64(uint64 a,uint64 b,uint64* c);
int imult64( int64 a, int64 b, int64* c);

#endif
