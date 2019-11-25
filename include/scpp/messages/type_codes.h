/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : type_codes
 * @created     : Saturday Sep 28, 2019 16:29:11 MDT
 */

#ifndef TYPE_CODES
#define TYPE_CODES

// Default protocol version
#ifndef PROTOCOL_VERSION
#define PROTOCOL_VERSION 1
#endif

// A nameless variable for our null terminator so that
// we can pass it to memcpy without initializing
// Isn't this cool???
#define NULL_TERMINATOR                                                        \
  (int) { 0x00 }

#define BYTE uint8_t

// Endian flags for endian byte for messages
#ifndef ENDIAN_TYPES
#define ENDIAN_TYPES
#define ENDIAN_BIG 'b'
#define ENDIAN_LITTLE 'l'
#endif

/*
 * Message types
 */
#define UNDEFINED 0
#define DATA_PACKET 1
#define PING 2
#define MAX_MESSAGE_TYPE PING

/*
 * Type codes
 */
#define INVALID '\0'
#define CHAR 'c'
#define BYTE_TYPE 'y'
#define BOOL 'b'
#define INT8 'p'
#define UINT8 'r'
#define INT16 'n'
#define UINT16 'q'
#define INT INT32
#define INT32 'i'
#define UINT32 'u'
#define INT64 'x'
#define UINT64 't'
#define FLOAT 'f'
#define DOUBLE 'd'
#define STRING 's'
#define OBJECT 'o'
#define ARRAY 'a'

#endif /* end of include guard TYPE_CODES_H */
