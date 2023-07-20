/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef UINT64_H
#define UINT64_H

#include <inttypes.h>	/* uint64_t, int64_t */
#include <stddef.h>	/* size_t */

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t uint64;
typedef int64_t int64;

#if (defined(__i386__) || defined(__x86_64__)) && !defined(NO_UINT64_MACROS)
#define uint64_pack(out,in) (*(uint64*)(out)=(in))
#define uint64_unpack(in,out) (*(out)=*(uint64*)(in))
#define uint64_read(in) (*(uint64*)(in))
void uint64_pack_big(char *out,uint64 in);
void uint64_unpack_big(const char *in,uint64* out);
uint64 uint64_read_big(const char *in);
#else

void uint64_pack(char *out,uint64 in);
void uint64_pack_big(char *out,uint64 in);
void uint64_unpack(const char *in,uint64* out);
void uint64_unpack_big(const char *in,uint64* out);
uint64 uint64_read(const char *in);
uint64 uint64_read_big(const char *in);

#endif

static inline size_t fmt_uint64(char* out, uint64 in) {
  if (out) uint64_pack(out,in);
  return 8;
}

static inline size_t fmt_uint64_big(char* out, uint64 in) {
  if (out) uint64_pack_big(out,in);
  return 8;
}

static inline size_t scan_uint64(const char* in, size_t len, uint64_t* b) {
  if (len<8) return 0;
  *b = uint64_read(in);
  return 8;
}

static inline size_t scan_uint64_big(const char* in, size_t len, uint64_t* b) {
  if (len<8) return 0;
  *b = uint64_read_big(in);
  return 8;
}

#ifdef __cplusplus
}

#ifndef uint64_pack
inline void uint64_pack(unsigned char* out,uint64 in) {
  uint64_pack(reinterpret_cast<char*>(out), in);
}

inline void uint64_unpack(const unsigned char *in,uint64* out) {
  uint64_unpack(reinterpret_cast<const char*>(in), out);
}

inline void uint64_pack(uint64* out,uint64 in) {
  uint64_pack(reinterpret_cast<char*>(out), in);
}

inline void uint64_unpack(uint64* *in,uint64* out) {
  uint64_unpack(reinterpret_cast<const char*>(in), out);
}

inline uint64 uint64_read(const unsigned char *in) {
  return uint64_read(reinterpret_cast<const char*>(in));
}

inline uint64 uint64_read(const uint64 *in) {
  return uint64_read(reinterpret_cast<const char*>(in));
}

#endif

inline uint64 uint64_read_big(const unsigned char *in) {
  return uint64_read_big(reinterpret_cast<const char*>(in));
}

inline uint64 uint64_read_big(const uint64 *in) {
  return uint64_read_big(reinterpret_cast<const char*>(in));
}

inline void uint64_pack_big(unsigned char* out,uint64 in) {
  uint64_pack_big(reinterpret_cast<char*>(out), in);
}

inline void uint64_unpack_big(const unsigned char *in,uint64* out) {
  uint64_unpack_big(reinterpret_cast<const char*>(in), out);
}

inline void uint64_pack_big(uint64* out,uint64 in) {
  uint64_pack_big(reinterpret_cast<char*>(out), in);
}

inline void uint64_unpack_big(uint64* *in,uint64* out) {
  uint64_unpack_big(reinterpret_cast<const char*>(in), out);
}

inline uint64 uint_read(const uint64* in) {
  return uint64_read(reinterpret_cast<const char*>(in));
}

inline uint64 uint_read_big(const uint64* in) {
  return uint64_read_big(reinterpret_cast<const char*>(in));
}
#endif

#endif
