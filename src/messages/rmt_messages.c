/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : rmt_messages
 * @created     : Saturday Oct 26, 2019 17:06:35 MDT
 */

#include "scpp/messages/rmt_messages.h"

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
cmd_vel_from_wire(cmd_vel* vel, const uint8_t* data)
{
  if (!vel || !data) {
    printf("Error \n");
    return -1;
  }

  vel->lin = *(float*)(data + vel->indexes[0]);
  vel->ang = *(float*)(data + vel->indexes[1]);
  return 1;
}

pos_localizer
create_pos_localizer()
{

  pos_localizer pos;

  float tempx = 1.0;
  float tempy = 1.0;
  float tempz = 1.0;
  float temptheta = 1.0;

  pos.x = tempx;
  pos.y = tempy;
  pos.z = tempz;
  pos.theta = temptheta;

  create_buffer(&pos.buff);

  serialize_data(&pos.buff, &pos.x, sizeof(float), FLOAT);
  serialize_data(&pos.buff, &pos.y, sizeof(float), FLOAT);
  serialize_data(&pos.buff, &pos.z, sizeof(float), FLOAT);
  serialize_data(&pos.buff, &pos.theta, sizeof(float), FLOAT);

  to_wire(&pos.buff);

  pos.indexes[0] =
    (size_t)(get_data(pos.buff.data, &pos.x, 0).data - pos.buff.data);
  pos.indexes[1] =
    (size_t)(get_data(pos.buff.data, &pos.y, 1).data - pos.buff.data);
  pos.indexes[2] =
    (size_t)(get_data(pos.buff.data, &pos.z, 2).data - pos.buff.data);
  pos.indexes[3] =
    (size_t)(get_data(pos.buff.data, &pos.theta, 3).data - pos.buff.data);


  if (*(float*)(pos.buff.data + pos.indexes[0]) != tempx) {
    printf("Error\n");
    return (pos_localizer){ 0 };
  }
  if (*(float*)(pos.buff.data + pos.indexes[1]) != tempy) {
    printf("Error\n");
    return (pos_localizer){ 0 };
  }
  if (*(float*)(pos.buff.data + pos.indexes[2]) != tempz) {
    printf("Error\n");
    return (pos_localizer){ 0 };
  }
  if (*(float*)(pos.buff.data + pos.indexes[3]) != temptheta) {
    printf("Error\n");
    return (pos_localizer){ 0 };
  }

  pos.x = 0.0;
  pos.y = 0.0;
  pos.z = 0.0;
  pos.theta = 0.0;
  return pos;
}

int
pos_localizer_to_wire(pos_localizer* pos)
{
  if (!pos) {
    printf("Error\n");
    return -1;
  }
  float* tempx = (float*)(pos->buff.data + pos->indexes[0]);
  float* tempy = (float*)(pos->buff.data + pos->indexes[1]);
  float* tempz = (float*)(pos->buff.data + pos->indexes[2]);
  float* temptheta = (float*)(pos->buff.data + pos->indexes[3]);
  *tempx = pos->x;
  *tempy = pos->y;
  *tempz = pos->z;
  *temptheta = pos->theta;
  return 1;
}

int
pos_localizer_from_wire(pos_localizer* pos, const uint8_t* data)
{
  if (!pos || !data) {
    printf("Error \n");
    return -1;
  }

  pos->x= *(float*)(data + pos->indexes[0]);
  pos->y = *(float*)(data + pos->indexes[1]);
  pos->z = *(float*)(data + pos->indexes[2]);
  pos->theta = *(float*)(data + pos->indexes[3]);
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
teensy_msg_from_wire(teensy_msg* msg, const uint8_t* data)
{
  if (!msg || !data) {
    printf("Error \n");
    return -1;
  }
  msg->lin = *(int16_t*)(data + msg->indexes[0]);
  msg->ang = *(int16_t*)(data + msg->indexes[1]);
  return 1;
}
