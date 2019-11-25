/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : data
 * @created     : Wednesday Oct 02, 2019 12:07:51 MDT
 */

#ifndef DATA_H

#define DATA_H

#include "type_codes.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#define __RMT_BIG_ENDIAN__ 0
#define __RMT_LITTLE_ENDIAN__ 1
#define __RMT_BYTE_ORDER__ 0

// PRINTER METHODS
void
print_memory(void* data, unsigned int byte_size);
void
print_memory_hex(void* data, unsigned int byte_size);

// SWAP INT BITS
#define bitswap(val)                                                           \
  if (sizeof(val) == 8)                                                        \
    bitswap8(val);                                                             \
  else if (sizeof(val) == 16)                                                  \
    bitswap16(val);                                                            \
  else if (sizeof(val) == 32)                                                  \
    bitswap16(val);                                                            \
  else if (sizeof(val) == 64)                                                  \
  bitswap64(val)
uint8_t
bitswap8(uint8_t data);
uint16_t
bitswap16(uint16_t data);
uint32_t
bitswap32(uint32_t data);
uint64_t
bitswap64(uint64_t data);

// TO HOST FUNCTIONS
#define network16 host16
uint16_t
host16(uint16_t data);
#define network32 host32
uint32_t
host32(uint32_t data);
#define network64 host64
uint64_t
host64(uint64_t data);

// Generic to host / to networks
#define to_network to_host
#if __RMT_BYTE_ORDER__ == __RMT_BIG_ENDIAN__
#define to_host(val, size)
#elif __RMT_BYTE_ORDER__ == __RMT_LITTLE_ENDIAN__
#define to_host(val, size) reverse_bytes(val, size)
#endif

void
inline_memcpy(void* destination, const void* source, size_t len);

// COMPACT INTS
int
smallest_16_size(uint16_t data);
int
smallest_32_size(uint32_t data);
int
smallest_64_size(uint64_t data);
int
signed_smallest_16_size(int16_t data);
int
signed_smallest_32_size(int32_t data);
int
signed_smallest_64_size(int64_t data);

// STANDARD LIBRARY, BUT HERE
void*
memcpy_rmt(void* destination, const void* src, size_t len);

// REVERSE THE BYTES OF A VALUE
void
reverse_bytes(char* data, size_t len);

// OPTIMIZE AN INTEGER
int
optimize_int(const char* const type_code,
             void const* const data,
             uint8_t* num_bytes);

/**
 * These are taken directly from glibc/bits/byteswap.h
 */
#define __bswap_constant_16_rmt(x)                                             \
  ((__uint16_t)((((x) >> 8) & 0xff) | (((x)&0xff) << 8)))

#define __bswap_constant_32_rmt(x)                                             \
  ((((x)&0xff000000u) >> 24) | (((x)&0x00ff0000u) >> 8) |                      \
   (((x)&0x0000ff00u) << 8) | (((x)&0x000000ffu) << 24))

#define __bswap_constant_64_rmt(x)                                             \
  ((((x)&0xff00000000000000ull) >> 56) | (((x)&0x00ff000000000000ull) >> 40) | \
   (((x)&0x0000ff0000000000ull) >> 24) | (((x)&0x000000ff00000000ull) >> 8) |  \
   (((x)&0x00000000ff000000ull) << 8) | (((x)&0x0000000000ff0000ull) << 24) |  \
   (((x)&0x000000000000ff00ull) << 40) | (((x)&0x00000000000000ffull) << 56))

#endif /* end of include guard DATA_H */
