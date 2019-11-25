/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : data
 * @created     : Thursday Oct 03, 2019 08:17:01 MDT
 */

#include "scpp/data.h"
#include <stdio.h>

void
print_memory(void* data, unsigned int byte_size)
{
  for (unsigned int i = 0U; i < byte_size; ++i) {
    char j = *(((char*)data) + i);
    for (int k = 0; k < 8; ++k) {
      printf("%d", j & (1 << 7) ? 1 : 0);
      j <<= 1;
    }
    printf(" ");
  }
  printf("\n");
}

void
print_memory_hex(void* data, unsigned int byte_size)
{
  for (unsigned int i = 0U; i < byte_size; ++i) {
    printf("%x", *(((char*)data + i)));
  }
  printf("\n");
}

int
smallest_64_size(uint64_t data)
{
  if (data == (data & (0xFF | data >> 62)) &&
      data == (data | ((1L << 63) >> 55)))
    return 8;
  if (data == (data & (0xFFFF | data >> 62)) &&
      data == (data | ((1L << 63) >> 47)))
    return 16;
  if (data == (data & (0xFFFFFFFF | data >> 62)) &&
      data == (data | ((1L << 63) >> 31)))
    return 32;
  return 64;
}

int
smallest_32_size(uint32_t data)
{
  if (data == (data & 0xFF))
    return 8;
  if (data == (data & 0xFFFF))
    return 16;
  return 32;
}

int
smallest_16_size(uint16_t data)
{
  if (data == (data & 0xFF))
    return 8;
  return 16;
}

int
signed_smallest_64_size(int64_t data)
{
  if (data == (data | ((1L << 63) >> 55)))
    return 8;
  if (data == (data | ((1L << 63) >> 47)))
    return 16;
  if (data == (data | ((1L << 63) >> 31)))
    return 32;
  return 64;
}

int
signed_smallest_32_size(int32_t data)
{
  if (data == (data | ((1 << 31) >> 23)))
    return 8;
  if (data == (data | ((1L << 31) >> 15)))
    return 16;
  return 32;
}

int
signed_smallest_16_size(int16_t data)
{
  if (data == (data | (((1 << 31) & data) >> 23)))
    return 8;
  if (data == (data | (((1 << 31) & data) >> 15)))
    return 16;
  return 32;
}

uint8_t
bitswap8(uint8_t data)
{
  uint8_t count = sizeof(data) * 8;
  uint8_t reversed = data;
  data >>= 1;
  while (data) {
    reversed <<= 1;
    reversed |= data & 1;
    data >>= 1;
    count--;
  }
  reversed <<= count;
  return reversed;
}

uint16_t
bitswap16(uint16_t data)
{
  uint16_t count = sizeof(data) * 8;
  uint16_t reversed = data;
  data >>= 1;
  while (data) {
    reversed <<= 1;
    reversed |= data & 1;
    data >>= 1;
    count--;
  }
  reversed <<= count;
  return reversed;
}

uint32_t
bitswap32(uint32_t data)
{
  uint32_t count = sizeof(data) * 8;
  uint32_t reversed = data;
  data >>= 1;
  while (data) {
    reversed <<= 1;
    reversed |= data & 1;
    data >>= 1;
    count--;
  }
  reversed <<= count;
  return reversed;
}

uint64_t
bitswap64(uint64_t data)
{
  uint64_t count = sizeof(data) * 8;
  uint64_t reversed = data;
  data >>= 1;
  while (data) {
    reversed <<= 1;
    reversed |= data & 1;
    data >>= 1;
    count--;
  }
  reversed <<= count;
  return reversed;
}

uint16_t
host16(uint16_t data)
{
#if __RMT_BYTE_ORDER__ == __RMT_BIG_ENDIAN__
  return data;
#else
  return __bswap_constant_16_rmt(data);
#endif
}
uint32_t
host32(uint32_t data)
{
#if __RMT_BYTE_ORDER__ == __RMT_BIG_ENDIAN__
  return data;
#else
  return __bswap_constant_32_rmt(data);
#endif
}
uint64_t
host64(uint64_t data)
{
#if __RMT_BYTE_ORDER__ == __RMT_BIG_ENDIAN__
  return data;
#else
  return data;
  /** return __bswap_constant_64_rmt(data); */
#endif
}

void
reverse_bytes(char* data, size_t len)
{
  int start = 0;
  int end = len;
  char temp;
  while (start < end) {
    temp = data[start];
    data[start] = data[end];
    data[end] = temp;
    start++;
    end--;
  }
}

void*
memcpy_rmt(void* destination, const void* src, size_t len)
{
  char* d = destination;
  const char* s = src;
  while (len--)
    *d++ = *s++;
  return destination;
}

int
optimize_int(const char* const type_code,
             void const* const data,
             uint8_t* num_bytes)
{
  int nu_b = *num_bytes;
  switch (*type_code) {
    case (INT16): {
      *num_bytes = signed_smallest_16_size(*(int16_t*)data);
      break;
    }

    case (UINT16): {
      *num_bytes = smallest_16_size(*(uint16_t*)data);
      break;
    }

    case (INT32): {
      *num_bytes = signed_smallest_32_size(*(int32_t*)data);
      break;
    }

    case (UINT32): {
      *num_bytes = smallest_32_size(*(uint32_t*)data);
      break;
    }

    case (INT64): {
      *num_bytes = signed_smallest_64_size(*(int16_t*)data);
      break;
    }

    case (UINT64): {
      *num_bytes = smallest_64_size(*(uint64_t*)data);
      break;
    }
  }
  return *num_bytes == nu_b;
}
