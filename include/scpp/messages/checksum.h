/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : checksum
 * @created     : Sunday Oct 13, 2019 13:34:42 MDT
 */

#ifndef CHECKSUM_H

#define CHECKSUM_H

#include <inttypes.h>
#include <stddef.h>

uint16_t
create_checksum(void* data, size_t byte_size);
int
validate_checksum(void* data, size_t byte_size, uint16_t checksum);

#endif /* end of include guard CHECKSUM_H */
