/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : ping
 * @created     : Friday Nov 01, 2019 16:43:00 MDT
 */

#include "scpp/ping_message.h"

int
main()
{
  ping_buffer buff = create_buffer_ping();

  buff.type = 1;
  buff.code = 2;
  buff.check = 3;
  buff.excess = 4;

  to_wire_ping(&buff);

  ping_buffer buff2 = create_buffer_ping();

  serialize_from_ping(&buff2, buff.data);

  printf("%d %d %d %d\n", buff2.type, buff2.code, buff2.check, buff2.excess);

  return 1;
}
