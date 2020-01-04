/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : rmt_messages
 * @created     : Saturday Oct 26, 2019 17:06:35 MDT
 */

#ifndef RMT_MESSAGES_H

#define RMT_MESSAGES_H

// #define LIGHT_MESSAGES
#include "data_message.h"

/*
 * Message sent from the control unit to the kernel
 * Decides desired linear and angular
 * velocity or rotation
 */
typedef struct
{
  float lin;
  float ang;
  size_t indexes[2];
  data_buffer buff;
} cmd_vel;

/*
 * A data packet from
 * the given data keys
 */
#define TEENSY_MSG 1
#define CAMERA_MESSAGE 2
typedef struct
{
  int type;
  size_t indexes[1];
  data_buffer buff;
} sensor_msg;

typedef struct
{
  float x;
  float y;
  float theta;
  float dt;
  size_t indexes[5];
  sensor_msg msg;
} camera_msg;

typedef struct
{
  float v;
  float w;
  float dt;
  size_t indexes[4];
  sensor_msg msg;
} teensy_sensor_array;

typedef struct
{
  // The state vector
  float x;
  float y;
  float theta;
  float v;
  float w;

  size_t indexes[5];
  data_buffer buff;
} path_state;

typedef struct
{
  float lin;
  float ang;
  size_t indexes[2];
  data_buffer buff;
} teensy_msg;

cmd_vel
create_cmd_vel();

teensy_msg
create_teensy_msg();

path_state
create_path_state();

teensy_sensor_array
create_teensy_sensor_array();

camera_msg
create_camera_msg();

sensor_msg
create_sensor_msg();

int
sensor_msg_from_wire(sensor_msg* msg, const uint8_t* data);

// CMD_VEL
int
cmd_vel_to_wire(cmd_vel* vel);

int
cmd_vel_from_wire(cmd_vel* vel, const uint8_t* data);

// PATH_STATE
int
path_state_to_wire(path_state* pos);

int
path_state_from_wire(path_state* pos, const uint8_t* data);

// TEENSY_MSG
int
teensy_msg_to_wire(teensy_msg* msg);

int
teensy_msg_from_wire(teensy_msg* msg, const uint8_t* data);

// TEENSY SENSOR ARRAY
int
teensy_sensor_array_to_wire(teensy_sensor_array* msg);

int
teensy_sensor_array_from_wire(teensy_sensor_array* msg, const uint8_t* data);

// TEENSY SENSOR ARRAY
int
camera_msg_to_wire(camera_msg* msg);

int
camera_msg_from_wire(camera_msg* msg, const uint8_t* data);

#endif /* end of include guard RMT_MESSAGES_H */
