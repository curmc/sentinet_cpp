/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : checksum
 * @created     : Sunday Oct 13, 2019 13:06:20 MDT
 */

#include "scpp/messages/checksum.h"

/*
 * A checksum is simply a number that is
 * computed
 */

// Right now, this just counts the number of bits
uint16_t
create_checksum(void* data, size_t byte_size)
{
  int ret = 0;
  size_t index = 0U;
  uint8_t* temp = data;
  while (index < byte_size) {
    for (uint8_t value = *temp; value; value >>= 1)
      if (value & 0x1)
        ret++;
    temp++;
    index++;
  }
  return ret;
}

// Right now, this just gets the checksum from the incomming data
int
validate_checksum(void* data, size_t byte_size, uint16_t checksum)
{
  return checksum == create_checksum(data, byte_size);
}
