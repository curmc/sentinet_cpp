/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ping_message
 * @created     : Saturday Oct 12, 2019 22:29:57 MDT
 */

#ifndef PING_MESSAGE_H

#define PING_MESSAGE_H

#include "data.h"
#include "ping_message_keys.h"
#include <errno.h>

// PING PACKET
/*
 * A ping packet is a type of packet that is
 * modeled after the ICMP packet
 * All it contains is a code and a checksum
 */
typedef struct
{
  // Protocol version stays the same for all messge versions
  PROTOCOL_PING prot;

  // The message code - what type of message is this
  OPCODE_PING op;

  // The ICMP type
  TYPE_PING type;

  // Code - a subcategory of opcode
  CODE_PING code;

  // A simple checksum that validates the packets
  CHECKSUM_PING check;

  // Depending on the opcode and code, this might contain information
  EXCESS_PING excess;

  // The actual message
  BYTE data[PING_MESSAGE_SIZE];
} ping_buffer;

#define get_protocol_version_ping(val) get_protocol_version_impl_ping(val)
#define get_protocol_version_impl_ping(val) *((BYTE*)&val[0])

#define get_opcode_ping(val)                                                   \
  get_opcode_impl_ping(val, get_protocol_version_ping(val))

#define get_type_ping(val)                                                     \
  get_type_impl_ping(val, get_protocol_version_ping(val))

#define get_code_ping(val)                                                     \
  get_code_impl_ping(val, get_protocol_version_ping(val))

#define get_checksum_ping(val)                                                 \
  get_checksum_impl_ping(val, get_protocol_version_ping(val))

#define get_excess_ping(val)                                                   \
  get_excess_impl_ping(val, get_protocol_version_ping(val))

OPCODE_PING
get_opcode_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version);

TYPE_PING
get_type_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version);

CODE_PING
get_code_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version);

CHECKSUM_PING
get_checksum_impl_ping(const BYTE* buffer,
                       const PROTOCOL_PING protocol_version);

EXCESS_PING
get_excess_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version);

int
to_wire_ping(ping_buffer* buffer);

ping_buffer
create_buffer_ping();

int
serialize_from_ping(ping_buffer* destination, const BYTE* source);

#endif /* end of include guard PING_MESSAGE_H */
