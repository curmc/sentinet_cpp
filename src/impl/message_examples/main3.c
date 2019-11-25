/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main3
 * @created     : Saturday Oct 12, 2019 18:43:39 MDT
 */

#include "scpp/data_message.h"

int
main()
{
  data_buffer buff;
  create_buffer(&buff);

  int i = 3;
  serialize_data(&buff, &i, sizeof(int), INT32);
  char* data = "Hello World";
  serialize_data(&buff, data, strlen(data), STRING);
  to_wire(&buff);

  print_message_raw(buff.data, get_byte_length(buff.data));

  return 0;
}
