/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : rmt_messages
 * @created     : Saturday Oct 26, 2019 17:06:35 MDT
 */

#include "scpp/messages/rmt_messages.h"

////////////////////////////// CMD VEL /////////////////////////////
cmd_vel
create_cmd_vel()
{
  cmd_vel vel = (cmd_vel){ .lin = 0.0, .ang = 0.0 };

  create_buffer(&vel.buff);

  serialize_data(&vel.buff, &vel.lin, sizeof(float), FLOAT);
  serialize_data(&vel.buff, &vel.ang, sizeof(float), FLOAT);

  to_wire(&vel.buff);

  vel.indexes[0] =
    (size_t)(get_data(vel.buff.data, &vel.lin, 0).data - vel.buff.data);

  vel.indexes[1] =
    (size_t)(get_data(vel.buff.data, &vel.ang, 1).data - vel.buff.data);

  return vel;
}

int
cmd_vel_to_wire(cmd_vel* vel)
{
  float* templin = (float*)(vel->buff.data + vel->indexes[0]);
  float* tempang = (float*)(vel->buff.data + vel->indexes[1]);
  *templin = vel->lin;
  *tempang = vel->ang;
  return 1;
}

int
cmd_vel_from_wire(cmd_vel* vel, const uint8_t* data)
{
  vel->lin = *(float*)(data + vel->indexes[0]);
  vel->ang = *(float*)(data + vel->indexes[1]);
  return 1;
}

////////////////////////////// TEENSY /////////////////////////////
teensy_msg
create_teensy_msg()
{
  teensy_msg msg = (teensy_msg){ .lin = 0.0, .ang = 0.0 };

  create_buffer(&msg.buff);

  serialize_data(&msg.buff, &msg.lin, sizeof(float), FLOAT);
  serialize_data(&msg.buff, &msg.ang, sizeof(int16_t), FLOAT);

  to_wire(&msg.buff);

  msg.indexes[0] =
    (size_t)(get_data(msg.buff.data, &msg.lin, 0).data - msg.buff.data);
  msg.indexes[1] =
    (size_t)(get_data(msg.buff.data, &msg.ang, 1).data - msg.buff.data);

  return msg;
}

int
teensy_msg_to_wire(teensy_msg* msg)
{
  float* templin = (float*)(msg->buff.data + msg->indexes[0]);
  float* tempang = (float*)(msg->buff.data + msg->indexes[1]);
  *tempang = msg->ang;
  *templin = msg->lin;
  return 1;
}

int
teensy_msg_from_wire(teensy_msg* msg, const uint8_t* data)
{
  msg->lin = *(float*)(data + msg->indexes[0]);
  msg->ang = *(float*)(data + msg->indexes[1]);
  return 1;
}

path_state
create_path_state()
{
  path_state path =
    (path_state){ .x = 0.0, .y = 0.0, .theta = 0.0, .v = 0.0, .w = 0.0 };

  create_buffer(&path.buff);

  serialize_data(&path.buff, &path.x, sizeof(float), FLOAT);
  serialize_data(&path.buff, &path.y, sizeof(float), FLOAT);
  serialize_data(&path.buff, &path.theta, sizeof(float), FLOAT);
  serialize_data(&path.buff, &path.v, sizeof(float), FLOAT);
  serialize_data(&path.buff, &path.w, sizeof(float), FLOAT);

  to_wire(&path.buff);
  path.indexes[0] =
    (size_t)(get_data(path.buff.data, &path.x, 0).data - path.buff.data);

  path.indexes[1] =
    (size_t)(get_data(path.buff.data, &path.y, 1).data - path.buff.data);

  path.indexes[2] =
    (size_t)(get_data(path.buff.data, &path.theta, 2).data - path.buff.data);

  path.indexes[3] =
    (size_t)(get_data(path.buff.data, &path.v, 3).data - path.buff.data);

  path.indexes[4] =
    (size_t)(get_data(path.buff.data, &path.w, 4).data - path.buff.data);

  return path;
}

int
path_state_to_wire(path_state* path)
{
  float* x = (float*)(path->buff.data + path->indexes[0]);
  float* y = (float*)(path->buff.data + path->indexes[1]);
  float* theta = (float*)(path->buff.data + path->indexes[2]);
  float* v = (float*)(path->buff.data + path->indexes[3]);
  float* w = (float*)(path->buff.data + path->indexes[4]);

  *x = path->x;
  *y = path->y;
  *theta = path->theta;
  *v = path->v;
  *w = path->w;
  return 1;
}

int
path_state_from_wire(path_state* msg, const uint8_t* data)
{
  msg->x = *(float*)(data + msg->indexes[0]);
  msg->y = *(float*)(data + msg->indexes[1]);
  msg->theta = *(float*)(data + msg->indexes[2]);
  msg->v = *(float*)(data + msg->indexes[3]);
  msg->w = *(float*)(data + msg->indexes[4]);
  return 1;
}

///////////////////////////////////////// TEENSY SENSOR ARRAY
/////////////////////////////////
teensy_sensor_array
create_teensy_sensor_array()
{
  teensy_sensor_array ret = (teensy_sensor_array){
    .v = 0.0, .w = 0.0, .dt = 0.0, .msg = create_sensor_msg()
  };
  ret.msg.type = TEENSY_MSG;

  serialize_data(&ret.msg.buff, &ret.v, sizeof(float), FLOAT);
  serialize_data(&ret.msg.buff, &ret.w, sizeof(float), FLOAT);
  serialize_data(&ret.msg.buff, &ret.dt, sizeof(float), FLOAT);

  to_wire(&ret.msg.buff);

  ret.indexes[0] =
    (size_t)(get_data(ret.msg.buff.data, &ret.v, 1).data - ret.msg.buff.data);

  ret.indexes[1] =
    (size_t)(get_data(ret.msg.buff.data, &ret.w, 2).data - ret.msg.buff.data);

  ret.indexes[2] =
    (size_t)(get_data(ret.msg.buff.data, &ret.dt, 3).data - ret.msg.buff.data);

  return ret;
}

int
teensy_sensor_array_to_wire(teensy_sensor_array* msg)
{
  int* type = (int*)(msg->msg.buff.data + msg->msg.indexes[0]);
  float* v = (float*)(msg->msg.buff.data + msg->indexes[0]);
  float* w = (float*)(msg->msg.buff.data + msg->indexes[1]);
  float* dt = (float*)(msg->msg.buff.data + msg->indexes[2]);

  *type = msg->msg.type;
  *v = msg->v;
  *w = msg->w;
  *dt = msg->dt;

  return 1;
}

int
teensy_sensor_array_from_wire(teensy_sensor_array* msg, const uint8_t* data)
{
  int* id = (int*)(data + msg->msg.indexes[0]);
  if (*id != TEENSY_MSG) {
    printf("Error, not  a teensy message");
    return -1;
  }

  msg->v = *(float*)(data + msg->indexes[0]);
  msg->w = *(float*)(data + msg->indexes[1]);
  msg->dt = *(float*)(data + msg->indexes[2]);

  return 1;
}

/////////////////////////////////////// CAMERA
///////////////////////////////////////////
int
camera_msg_to_wire(camera_msg* msg)
{
  int* type = (int*)(msg->msg.buff.data + msg->msg.indexes[0]);
  float* x = (float*)(msg->msg.buff.data + msg->indexes[0]);
  float* y = (float*)(msg->msg.buff.data + msg->indexes[1]);
  float* theta = (float*)(msg->msg.buff.data + msg->indexes[2]);
  float* dt = (float*)(msg->msg.buff.data + msg->indexes[3]);

  *type = msg->msg.type;
  *x = msg->x;
  *y = msg->y;
  *theta = msg->theta;
  *dt = msg->dt;

  return 1;
}

int
camera_msg_from_wire(camera_msg* msg, const uint8_t* data)
{
  int* id = (int*)(data + msg->msg.indexes[0]);
  if (*id != CAMERA_MESSAGE) {
    printf("Error, not  a teensy message");
    return -1;
  }

  msg->x = *(float*)(data + msg->indexes[0]);
  msg->y = *(float*)(data + msg->indexes[1]);
  msg->theta = *(float*)(data + msg->indexes[2]);
  msg->dt = *(float*)(data + msg->indexes[3]);

  return 1;
}

camera_msg
create_camera_msg()
{

  camera_msg ret = (camera_msg){
    .x = 0.0, .y = 0.0, .theta = 0.0, .dt = 0.0, .msg = create_sensor_msg()
  };
  ret.msg.type = CAMERA_MESSAGE;

  serialize_data(&ret.msg.buff, &ret.x, sizeof(float), FLOAT);
  serialize_data(&ret.msg.buff, &ret.y, sizeof(float), FLOAT);
  serialize_data(&ret.msg.buff, &ret.theta, sizeof(float), FLOAT);
  serialize_data(&ret.msg.buff, &ret.dt, sizeof(float), FLOAT);

  to_wire(&ret.msg.buff);

  // Get data starting at 1 (cause of sensor type)
  ret.indexes[0] =
    (size_t)(get_data(ret.msg.buff.data, &ret.y, 1).data - ret.msg.buff.data);

  ret.indexes[1] = (size_t)(get_data(ret.msg.buff.data, &ret.theta, 2).data -
                            ret.msg.buff.data);

  ret.indexes[2] =
    (size_t)(get_data(ret.msg.buff.data, &ret.dt, 3).data - ret.msg.buff.data);

  return ret;
}

sensor_msg
create_sensor_msg()
{
  sensor_msg ret = (sensor_msg){ .type = 0 };

  create_buffer(&ret.buff);

  serialize_data(&ret.buff, &ret.type, sizeof(int), INT32);

  to_wire(&ret.buff);

  ret.indexes[0] =
    (size_t)(get_data(ret.buff.data, &ret.type, 0).data - ret.buff.data);
  return ret;
}

int
sensor_msg_from_wire(sensor_msg* msg, const uint8_t* data)
{
  int* id = (int*)(data + msg->indexes[0]);
  printf("%ld\n", msg->indexes[0]);

  printf("%d\n", *id);

  if (*id == CAMERA_MESSAGE)
    msg->type = CAMERA_MESSAGE;
  else if (*id == TEENSY_MSG)
    msg->type = TEENSY_MSG;
  else
    return -1;

  return msg->type;
}
