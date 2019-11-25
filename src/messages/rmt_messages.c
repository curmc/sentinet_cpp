/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : rmt_messages
 * @created     : Saturday Oct 26, 2019 17:06:35 MDT
 */

#include "scpp/rmt_messages.h"

cmd_vel
create_cmd_vel()
{

  cmd_vel vel;

  float templin = 1.0;
  float tempang = 1.0;
  vel.lin = templin;
  vel.ang = tempang;
  create_buffer(&vel.buff);

  serialize_data(&vel.buff, &vel.lin, sizeof(float), FLOAT);
  serialize_data(&vel.buff, &vel.ang, sizeof(float), FLOAT);

  to_wire(&vel.buff);

  vel.indexes[0] =
    (size_t)(get_data(vel.buff.data, &vel.lin, 0).data - vel.buff.data);
  vel.indexes[1] =
    (size_t)(get_data(vel.buff.data, &vel.ang, 1).data - vel.buff.data);

  if (*(float*)(vel.buff.data + vel.indexes[0]) != templin) {
    printf("Error\n");
    return (cmd_vel){ 0 };
  }

  if (*(float*)(vel.buff.data + vel.indexes[1]) != tempang) {
    printf("Error\n");
    return (cmd_vel){ 0 };
  }

  vel.lin = 0.0;
  vel.ang = 0.0;
  return vel;
}

int
cmd_vel_to_wire(cmd_vel* vel)
{
  if (!vel) {
    printf("Error\n");
    return -1;
  }
  float* templin = (float*)(vel->buff.data + vel->indexes[0]);
  float* tempang = (float*)(vel->buff.data + vel->indexes[1]);
  *templin = vel->lin;
  *tempang = vel->ang;
  return 1;
}

int
cmd_vel_from_wire(cmd_vel* vel, const char* data)
{
  if (!vel || !(data + vel->buff.byte_length)) {
    printf("Error \n");
    return -1;
  }

  vel->lin = *(float*)(data + vel->indexes[0]);
  vel->ang = *(float*)(data + vel->indexes[1]);
  return 1;
}

teensy_msg
create_teensy_msg()
{

  teensy_msg msg;

  int8_t templin = 1;
  int8_t tempang = 1;

  msg.lin = templin;
  msg.ang = tempang;

  create_buffer(&msg.buff);

  serialize_data(&msg.buff, &msg.lin, sizeof(int16_t), INT16);
  serialize_data(&msg.buff, &msg.ang, sizeof(int16_t), INT16);

  to_wire(&msg.buff);

  msg.indexes[0] =
    (size_t)(get_data(msg.buff.data, &msg.lin, 0).data - msg.buff.data);
  msg.indexes[1] =
    (size_t)(get_data(msg.buff.data, &msg.ang, 1).data - msg.buff.data);

  if (*(int16_t*)(msg.buff.data + msg.indexes[0]) != templin) {
    printf("Error\n");
    return (teensy_msg){ 0 };
  }

  if (*(int16_t*)(msg.buff.data + msg.indexes[1]) != tempang) {
    printf("Error\n");
    return (teensy_msg){ 0 };
  }

  msg.lin = 0;
  msg.ang = 0;

  return msg;
}

int
teensy_msg_to_wire(teensy_msg* msg)
{
  if (msg) {
    int16_t* templin = (int16_t*)(msg->buff.data + msg->indexes[0]);
    int16_t* tempang = (int16_t*)(msg->buff.data + msg->indexes[1]);
    *tempang = msg->ang;
    *templin = msg->lin;
    return 1;
  }
  printf("NOO");
  return 0;
}

int
teensy_msg_from_wire(teensy_msg* msg, const char* data)
{
  if (!msg || !(data + msg->buff.byte_length)) {
    printf("Error \n");
    return -1;
  }
  msg->lin = *(int16_t*)(data + msg->indexes[0]);
  msg->ang = *(int16_t*)(data + msg->indexes[1]);
  return 1;
}
