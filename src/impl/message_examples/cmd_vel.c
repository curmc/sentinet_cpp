/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : cmd_vel
 * @created     : Saturday Oct 26, 2019 18:00:09 MDT
 */

#include "scpp/rmt_messages.h"

int
main()
{
  cmd_vel a = create_cmd_vel();
  // Not necessary
  cmd_vel_to_wire(&a);
  printf("%d %d\n", a.indexes[0], a.indexes[1]);

  a.lin = 5.01928;
  a.ang = 6.1982;

  cmd_vel_to_wire(&a);

  cmd_vel b = create_cmd_vel();
  cmd_vel_from_wire(&b, a.buff.data);

  printf("%f %f\n", b.lin, b.ang);

  teensy_msg msg = create_teensy_msg();

  teensy_msg_to_wire(&msg);
  printf("%d %d\n", msg.indexes[0], msg.indexes[1]);
  msg.lin = 5;
  msg.ang = 6;

  teensy_msg_to_wire(&msg);

  teensy_msg q = create_teensy_msg();
  teensy_msg_from_wire(&q, msg.buff.data);

  printf("%i %i\n", q.lin, q.ang);
  printf("%i %i\n", msg.lin, msg.ang);
  return 1;
}
