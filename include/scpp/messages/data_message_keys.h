/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : data_message_keys
 * @created     : Sunday Oct 13, 2019 16:30:20 MDT
 */

#ifndef DATA_MESSAGE_KEYS_H

#define DATA_MESSAGE_KEYS_H

#define LIGHT_MESSAGES
#include "type_codes.h"

// DATA MESSAGE SIZE - SIZE OF THE MESSAGE BUFFER
#ifndef MESSAGE_SIZE
#ifndef LIGHT_MESSAGES
#define MESSAGE_SIZE (1 << 12)
#else
#define MESSAGE_SIZE (1 << 8)
#endif
#endif

#ifndef MESSAGE_SIZE
#ifndef LIGHT_MESSAGES
#define SMALL_MESSAGE_SIZE 128
#else
#define SMALL_MESSAGE_SIZE 48
#endif
#endif

// ADDRESS POSITIONS - position of each value in the data_message struct
#if PROTOCOL_VERSION == 1
#define PROTOCOL uint8_t
#define OPCODE uint8_t
#define FUNCTIONAL_FLAG uint16_t
#define CHECKSUM uint16_t
#define BYTE_LENGTH uint32_t
#define FIELDS uint16_t
#define SERIALIZE_FLAGS uint64_t

// The easily readable version
#define PROTPOS 0
#define OPPOS PROTPOS + sizeof(PROTOCOL)
#define FUNCPOS OPPOS + sizeof(OPCODE)
#define MAGPOS FUNCPOS + sizeof(FUNCTIONAL_FLAG)
#define BYTEPOS MAGPOS + sizeof(CHECKSUM)
#define FLDPOS BYTEPOS + sizeof(BYTE_LENGTH)
#define NULLTERMPOS FLDPOS + sizeof(FIELDS)
#define DTPOS NULLTERMPOS + 1
#endif

// SIZE OF A MESSAGE HEADER
#define DATA_HEADER_SIZE DTPOS

#ifndef MAX_FIELDS
#ifndef LIGHT_MESSAGES
#define MAX_FIELDS (1 << 10)
#else
#define MAX_FIELDS (1 << 5)
#endif
#endif

// FUNCTIONALITY FLAGS
#ifndef FUNCTIONAL_FLAGS
#define FUNCTIONAL_FLAGS
#define SSFCHK (1 << 0) // Serialization safety check
#define DSFCHK (1 << 1) // Deserialization safety check
#define OPDT (1 << 2)   // Optimize data types
#define SERIALIZED (1 << 3)
#endif

#endif /* end of include guard DATA_MESSAGE_KEYS_H */
