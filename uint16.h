/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef UINT16_H
#define UINT16_H

#include <inttypes.h>

typedef uint16_t uint16;
typedef int16_t int16;

#if (defined(__i386__) || defined(__x86_64__)) && !defined(NO_UINT16_MACROS)
#define uint16_pack(out,in) (*(uint16*)(out)=(in))
#define uint16_unpack(in,out) (*(out)=*(uint16*)(in))
#define uint16_read(in) (*(uint16*)(in))
void uint16_pack_big(char *out,uint16 in);
void uint16_unpack_big(const char *in,uint16* out);
uint16 uint16_read_big(const char *in);
#else

void uint16_pack(char *out,uint16 in);
void uint16_pack_big(char *out,uint16 in);
void uint16_unpack(const char *in,uint16* out);
void uint16_unpack_big(const char *in,uint16* out);
uint16 uint16_read(const char *in);
uint16 uint16_read_big(const char *in);

#endif

#endif
