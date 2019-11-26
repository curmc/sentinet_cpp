/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ping_message
 * @created     : Saturday Oct 12, 2019 22:48:48 MDT
 */

#include "scpp/messages/ping_message.h"

static int
check_packet_validity()
{
  return 1;
}

ping_buffer
create_buffer_ping()
{
  if (check_packet_validity()) {
    ping_buffer ret;
    ret = (ping_buffer){ .prot = (PROTOCOL_PING)PROTOCOL_VERSION,
                         .op = (OPCODE_PING)PING };
    return ret;
  }

  puts("Exiting buffer\n");
  return (ping_buffer){ 0 };
}

OPCODE_PING
get_opcode_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version)
{
  switch (protocol_version) {
    case (1): {
      PROTOCOL_PING ret = *(PROTOCOL_PING*)(buffer + OPPOS_PING);
      return ret <= MAX_PING_TYPE ? ret : 0;
    }
  }
  return 0;
}

TYPE_PING
get_type_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version)
{
  switch (protocol_version) {
    case (1): {

      TYPE_PING ret = *(TYPE_PING*)(buffer + TYPEPOS_PING);
      return ret <= MAX_PING_TYPE ? ret : 0;
    }
  }
  return 0;
}

CODE_PING
get_code_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version)
{
  switch (protocol_version) {
    case (1):
      return *(CODE_PING*)(buffer + CODEPOS_PING);
  }
  return 0;
}

CHECKSUM_PING
get_checksum_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version)
{
  switch (protocol_version) {
    case (1):
      return *(CHECKSUM_PING*)(buffer + CHECKPOS_PING);
  }
  return 0;
}

EXCESS_PING
get_excess_impl_ping(const BYTE* buffer, const PROTOCOL_PING protocol_version)
{
  switch (protocol_version) {
    case (1): {
      return *(EXCESS_PING*)(buffer + EXCESSPOS_PING);
    }
  }
  return 0;
}

int
to_wire_ping(ping_buffer* buffer)
{
  errno = 0;

  if (buffer) {
    PROTOCOL_PING* prot = (PROTOCOL_PING*)(buffer->data + PROTOPOS_PING);
    OPCODE_PING* op = (OPCODE_PING*)(buffer->data + OPPOS_PING);
    TYPE_PING* type = (TYPE_PING*)(buffer->data + TYPEPOS_PING);
    CODE_PING* code = (CODE_PING*)(buffer->data + CODEPOS_PING);
    CHECKSUM_PING* check = (CHECKSUM_PING*)(buffer->data + CHECKPOS_PING);
    EXCESS_PING* excess = (EXCESS_PING*)(buffer->data + EXCESSPOS_PING);

    *prot = buffer->prot;
    *op = buffer->op;
    *type = buffer->type;
    *code = buffer->code;
    *check = buffer->check;
    *excess = buffer->excess;
    return errno;
  }

  errno = EINVAL;

  perror("Invalid buffer argument");
  return errno;
}

int
serialize_from_ping(ping_buffer* destination, const BYTE* source)
{

  // Keep track of the item that failed
  errno = 0;

  if (destination && source) {
    destination->prot = get_protocol_version_ping(source);
    destination->op = get_opcode_ping(source);
    destination->type = get_type_ping(source);
    destination->code = get_code_ping(source);
    destination->check = get_checksum_ping(source);
    destination->excess = get_excess_ping(source);
    return errno;
  }

  errno = EINVAL;
  perror("Invalid destination or source ");
  return errno;
}
