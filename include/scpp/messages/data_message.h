/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : message_structs
 * @created     : Wednesday Oct 09, 2019 12:40:13 MDT
 */

#ifndef MESSAGE_STRUCTS_H

#define MESSAGE_STRUCTS_H

#define NO_NET_BYTE

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checksum.h"
#include "data.h"
#include "data_message_keys.h"

// DATA PACKET
/*
 * Data is simply a header followed by an array of data elements
 * data elements are simply two bytes (size and type_code)
 * followed by the actual data
 */
typedef struct
{
  // Protocol version stays the same for all message versions
  PROTOCOL protocol_version;

  // The op - code, the same for all similar message types
  OPCODE opcode;

  // Flags that denote functionality
  FUNCTIONAL_FLAG functional_flags;

  // A magic number for an ack response if necessary
  CHECKSUM checksum;

  // The length of the entire message in bytes
  BYTE_LENGTH byte_length;

  // The number of fields in the message (to index with)
  FIELDS fields;

  // The actual storage buffer (uint8_t is equivalent to byte)
  BYTE data[MESSAGE_SIZE];

  // Options for how to serialize data, not actually passed to the message
  SERIALIZE_FLAGS serialize_flags;
} data_buffer;

/**
 * Get the protocol version of the message
 * this is the one thing that will stay the same across all
 * versions of this protocol - everything else assumes you are working
 * with the same protocol
 */
#define get_protocol_version(val) get_protocol_version_impl(val)
#define get_protocol_version_impl(val) *((PROTOCOL*)&val[0])

/**
 * The opcode is the message type 1 - 255
 * See type_codes.h for the types of messages
 */
#define get_opcode(val) get_opcode_impl(val, get_protocol_version(val))

#define get_checksum(val) get_checksum_impl(val, get_protocol_version(val))

#define get_byte_length(val)                                                   \
  get_byte_length_impl(val, get_protocol_version(val))

#define get_number_fields(val)                                                 \
  get_number_fields_impl(val, get_protocol_version(val))

OPCODE
get_opcode_impl(const BYTE* buffer, const PROTOCOL protocol_version);

CHECKSUM
get_checksum_impl(const BYTE* buffer, const PROTOCOL protocol_version);

BYTE_LENGTH
get_byte_length_impl(const BYTE* buffer, const PROTOCOL protocol_version);

FIELDS
get_number_fields_impl(const BYTE* buffer, const PROTOCOL protocol_version);

int
serialize_data(data_buffer* buff,
               void* data,
               uint8_t num_bytes,
               char type_code);

/**
 * Puts the entire content of an data item from a message packet into
 * destination (size and type code)
 *
 * @param buff The buffer to read from
 * @param destination The destination to put the data into
 * @param index The index to get (i.e. 0 for first value, 1 for second etc)
 *
 * @return An element struct with an immutable pointer to the start of
 * the actual data, a char type code and the size of the elemetn
 */
typedef struct
{
  uint8_t size;
  char type_code;
  const BYTE* const data;
} element;
element
get_data(const BYTE* buff, void* destination, size_t index);

/**
 * Puts the meta fields declared at the top of data_buffer
 * into the actual buffer according to version number flags etc
 *
 * @param buffer - the hopefully serialized data buffer (checks
 * if the serialization flag is checked) with 0's in the header
 * (or not, no matter what it's getting overwritten)
 */
int
to_wire(data_buffer* buffer);

/**
 * @brief Create an empty initialized buffer
 * @return a new buffer that initializes constant values
 */
void
create_buffer(data_buffer*);

/**
 * @brief Simply print the hexidecimal message
 *
 * @param input The data input
 * @param size The size of the data
 */
void
print_message_raw(const BYTE* input, size_t size);

/**
 * @brief Print a message in a formatted fashion
 *
 * @param recieved_data The data recieved
 */
void
print_message_formatted(BYTE* recieved_data);
#endif /* end of include guard MESSAGE_STRUCTS_H */
