/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : messages
 * @created     : Saturday Sep 14, 2019 11:04:17 MDT
 */

#include "scpp/messages/data_message.h"

static int
check_data_message_validity()
{
  return 1;
}

/**
 * Create buffer
 *
 * Checks if macro sizes fit into structure
 * Creates a new data_buffer with pointers in place
 * Sets protocol, opcode, and byte_length, everything else is 0
 */
void
create_buffer(data_buffer* buff)
{
  // Check for macro size validity
  if (!check_data_message_validity() || !buff) {
    printf("exiting create data buffer\n");
    exit(1);
  }

  memset(buff, 0, sizeof(data_buffer));

  buff->protocol_version = PROTOCOL_VERSION;
  buff->opcode = DATA_PACKET;
  buff->byte_length = DATA_HEADER_SIZE;
  printf("%ld\n", DATA_HEADER_SIZE);
  to_wire(buff);
}

/*
 * Gets the opcode from a byte buffer
 *
 * Needs to check protocol, i.e. what
 * version of smp are we using
 */
OPCODE
get_opcode_impl(const BYTE* buffer, const PROTOCOL protocol_version)
{
  switch (protocol_version) {

    // Protocol version 1
    case (1): {

      OPCODE ret = (*(OPCODE*)(buffer + OPPOS));
      return ret <= MAX_MESSAGE_TYPE ? 0 : ret;
    }
  }
  return 0;
}

/*
 * Gets the opcode from a byte buffer
 *
 * Needs to check protocol, i.e. what
 * version of smp are we using
 */
CHECKSUM
get_checksum_impl(const BYTE* buffer, const PROTOCOL protocol_version)
{
  switch (protocol_version) {
    // Protocol version 1
    case (1): {
      // TODO change network16 so that it doesn't matter the size
      CHECKSUM ret = *(CHECKSUM*)(buffer + MAGPOS);
#ifndef NO_NET_BYTE
      return host16(ret);
#else
      return ret;
#endif
    }
      // End of protocol version 1
  }
  return 0;
}

BYTE_LENGTH
get_byte_length_impl(const BYTE* buffer, const PROTOCOL protocol_version)
{
  switch (protocol_version) {

    // Protocol version 1
    case (0x1): {

      BYTE_LENGTH ret = *(BYTE_LENGTH*)(buffer + BYTEPOS);

#ifndef NO_NET_BYTE
      return ret <= MESSAGE_SIZE ? host32(ret) : 0;
#else
      return ret <= MESSAGE_SIZE ? ret : 0;
#endif
    }
      // End of protocol version 1
  }
  return 0;
}

FIELDS
get_number_fields_impl(const BYTE* buffer, const PROTOCOL protocol_version)
{
  switch (protocol_version) {

    case (1): {

      FIELDS ret = *(FIELDS*)(buffer + FLDPOS);

#ifndef NO_NET_BYTE
      return host16(ret);
#else
      return ret;
#endif
    }
      // End of protocol version 1
  }
  return 0;
}

/*
 * Goals of this function:
 *
 * Make as much inline to reduce function calls
 *
 * Utilize as many one op data copies rather than
 * long byte by byte memcpy
 *
 * Protect from buffer overflows by including a ton
 * of checkers
 */
int
serialize_data(data_buffer* buffer,
               void* data,
               uint8_t num_bytes,
               char type_code)
{
  errno = 0;
  BYTE* head;

  /*
   * Perform some pre safety checks so that buffer overflows are not possible
   * and so we have bounds checking
   */

  if (data && 3 + num_bytes + buffer->byte_length < MESSAGE_SIZE) {
    // A pointer to the head of the starting data for readability
    head = buffer->data + buffer->byte_length;

    // Copy num_bytes and type_code into head - normally this would be risky
    // (larger byte sizes etc) // TODO Will have to change this for backwards
    // compatability
    *head++ = (BYTE)num_bytes;
    *head++ = (BYTE)type_code;

    // Incriment byte_length and fields - gcc puts unnecessary optimizations
    // here
    buffer->byte_length += 3 + num_bytes;
    buffer->fields = buffer->fields + 1;

    /*
     * Making actual serialization calls inline
     * to optimize for speed based on type
     */
    switch (type_code) {

        // 32 first because I'm
        // thinking that's most common
      case (INT32):
      case (UINT32):
      case (FLOAT): {
        // You can't cast an lvalue,
        // so we need to declare a
        // temporary variable
        uint32_t* temp = (uint32_t*)head;
#ifndef NO_NET_BYTE
        *temp = network32(*(uint32_t*)data);
#else
        *temp = *(uint32_t*)data;
#endif
        head += sizeof(uint32_t);
      } break;
      case (CHAR):
      case (UINT8):
      case (INT8):
      case (BYTE_TYPE): {
        *head++ = *(BYTE*)data;
      } break;

      case (INT16):
      case (BOOL): {
        uint16_t* temp = (uint16_t*)head;
#ifndef NO_NET_BYTE
        *temp = network16(*(uint16_t*)data);
#else
        *temp = *(uint16_t*)data;
#endif
        head += sizeof(uint16_t);
      } break;

      // Optimized memcpy using longs first then bytes
      default: {

        // Use longs first
        uint64_t* sourcel = (uint64_t*)data;
        uint64_t* destl = (uint64_t*)head;

        // Go through and serialize in multiples of 4
        // Check first here offers a temporary solution to sourcel and destl
        if (!(*sourcel & 0xFFFFFFFC) && !(*destl & 0xFFFFFFFC)) {
          while (num_bytes >= 4) {
            *destl++ = *sourcel++;
            num_bytes -= 4;
          }
        }

        // Then use bytes
        head = (BYTE*)destl;
        BYTE* source = (BYTE*)sourcel;
        while (num_bytes--)
          *head++ = *source++;

      } break;
    }

    // Add a nullterminator
    *head = 0x00;

    // We good
    return 0;
  }
  return 0;
}

element
get_data(const BYTE* data, void* destination, size_t index)
{
  errno = 0;
  FIELDS fields;
  BYTE* data_head;
  uint8_t size;
  unsigned int i;
  unsigned int index_;

  /* Do safety checks TODO - I have to many, making it cluttered */

  // Check if data is a valid buffer
  if (!data || !(data + FLDPOS)) {
    errno = EINVAL;
    perror("Invalid data buffer, fields index does not exist ");
    return (element){ 0 };
  }

  // Check that destination is a valid argument
  if (!destination) {
    errno = EINVAL;
    perror("Invalid destination argument, destination is null ");
    return (element){ 0 };
  }

  // number of fields in the buffer
  fields = network16(*((FIELDS*)(data + FLDPOS)));

  // Check the value of the field
  if (fields >= MAX_FIELDS || index >= fields) {
    errno = ERANGE;

    perror("Invalid index, higher than the number of message fields");
    return (element){ 0 };
  }

  // The head of the data segment of the message
  data_head = ((BYTE*)(data + DTPOS));

  // Check to see that that index exists
  if (!data_head) {
    errno = ERANGE;
    perror("Invalid buffer. corrupted or not correct size ");
    return (element){ 0 };
  }

  index_ = 0U;
  i = 0U;

  // Each itteration is jumping to the next data element
  while (index_ < index && data_head + i) {
    // The size of the current data element
    size = *((BYTE*)(data_head + i++));

    // i is now at the head of the actual data segment
    if (!(data_head + i++)) {
      errno = ERANGE;
      perror("Invalid data segment ");
      return (element){ 0 };
    }

    i += size; // size plus 3 technically

    if (*(data_head + i++)) {
      errno = EBADMSG;
      perror("Invalid null terminator after data segment ");
      return (element){ 0 };
    }

    index_++;
  }

  // data_head is now at the byte that denotes size for our desired
  // value here -> [size][type][data.....]

  // If we exited the loop with a bad itterator
  if (!(data_head + i)) {
    errno = ERANGE;
    perror("Invalid range on index ");
    return (element){ 0 };
  }

  // Size of the resultant data type
  size = *(data_head + i);

  // Make sure both destination and data_head can fit the return data
  if (!((BYTE*)destination + size || !(data_head + i + 2 + size))) {
    errno = EINVAL;
    perror("Invalid destination val, found data, but destination isn't large "
           "enough to hold it ");
    return (element){ 0 };
  }

  element ret = { .size = size,
                  .type_code = *(data_head + i + 1),
                  .data = data_head + i + 2 };

  switch (ret.type_code) {
    case (INT32):
    case (UINT32):
    case (FLOAT): {
      uint32_t* temp = (uint32_t*)destination;
#ifndef NO_NET_BYTE
      *temp = host32(*(uint32_t*)(data_head + i + 2));
#else
      *temp = *(uint32_t*)(data_head + i + 2);
#endif
    } break;

    case (CHAR):
    case (UINT8):
    case (INT8):
    case (BYTE_TYPE): {
      uint8_t* temp = (uint8_t*)destination;
      *temp = *(data_head + i + 2);
    } break;

    case (INT16):
    case (BOOL): {
      uint16_t* temp = (uint16_t*)destination;
#ifndef NO_NET_BYTE
      *temp = network16(*(uint16_t*)(data_head + i + 2));
#else
      *temp = *(uint16_t*)(data_head + i + 2);
#endif
    } break;

    default: {
      // Use longs first
      uint64_t const* sourcel = (uint64_t const*)(data_head + i + 2);
      uint64_t* destl = (uint64_t*)destination;

      if (!(*sourcel & 0xFFFFFFFC) && !(*destl & 0xFFFFFFFC)) {
        while (size >= 4) {
          *destl++ = *sourcel++;
          size -= 4;
        }
      }

      // Then use bytes
      BYTE* source = (BYTE*)sourcel;
      BYTE* head = (BYTE*)destl;
      while (size--)
        *head++ = *source++;

    } break;
  }

  return ret;
}

int
to_wire(data_buffer* buffer)
{
  errno = 0;
  if (!buffer)
    return 1;

  PROTOCOL* prot = (PROTOCOL*)(buffer->data + PROTPOS);
  OPCODE* op = (OPCODE*)(buffer->data + OPPOS);
  FUNCTIONAL_FLAG* func = (FUNCTIONAL_FLAG*)(buffer->data + FUNCPOS);
  CHECKSUM* check = (CHECKSUM*)(buffer->data + MAGPOS);
  BYTE_LENGTH* byte = (BYTE_LENGTH*)(buffer->data + BYTEPOS);
  FIELDS* fields = (FIELDS*)(buffer->data + FLDPOS);

  *prot = buffer->protocol_version;
  *op = buffer->opcode;
  *func = network16(buffer->functional_flags);
  *check = network16(buffer->checksum);
  *byte = network32(buffer->byte_length);
  *fields = network16(buffer->fields);

  return errno;
}

void
print_message_raw(const BYTE* input, size_t size)
{
  for (size_t i = 0U; i < size; ++i)
    printf("%x ", input[i]);
  printf("\n");
}

void
print_message_formatted(BYTE* recieved_data)
{
  char buffer[256];
  printf("HEADER: %d %d %d %d %d\n",
         get_protocol_version(recieved_data),
         get_opcode(recieved_data),
         get_checksum(recieved_data),
         get_byte_length(recieved_data),
         get_number_fields(recieved_data));
  for (int i = 0; i < get_number_fields(recieved_data); ++i) {
    element temp = get_data(recieved_data, buffer, i);
    printf("%d %c\n", temp.size, temp.type_code);
    printf("     ");
    print_message_raw(temp.data, temp.size + 1);
  }
}
