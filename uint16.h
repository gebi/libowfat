#ifndef UINT16_H
#define UINT16_H

#include <sys/cdefs.h>

typedef unsigned short uint16;

#ifndef __linux__
#define NO_UINT16_MACROS
#endif

#ifdef NO_UINT16_MACROS
extern void uint16_pack(char *out,uint16 in);
extern void uint16_pack_big(char *out,uint16 in);
extern void uint16_unpack(const char *in,uint16* out);
extern void uint16_unpack_big(const char *in,uint16* out);
#else

#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN

#define uint16_pack(out,in) (*(short*)(out)=(in))

extern void uint16_pack_big(char *out,uint16 in);

#define uint16_unpack(in,out) (*(out)=*(short*)(in))

extern void uint16_unpack_big(const char *in,uint16* out);

#else

extern void uint16_pack(char *out,uint16 in);

#define uint16_pack_big(out,in) (*(short*)(out)=(in))

extern void uint16_unpack(const char *in,uint16* out);

#define uint16_unpack_big(in,out) (*(out)=*(short*)(in))

#endif

#endif

#endif
