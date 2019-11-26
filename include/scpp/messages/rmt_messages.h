/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : rmt_messages
 * @created     : Saturday Oct 26, 2019 17:06:35 MDT
 */

#ifndef RMT_MESSAGES_H

#define RMT_MESSAGES_H

// #define LIGHT_MESSAGES
#include "scpp/data_message.h"

typedef struct
{
  float lin;
  float ang;
  size_t indexes[2];
  data_buffer buff;
} cmd_vel;

typedef struct
{
  float x_accel;
  float y_accel;
  float theta_accel;
  size_t indexes[3];
  data_buffer buff;
} imu_data;

typedef struct
{
  int16_t lin;
  int16_t ang;
  size_t indexes[2];
  data_buffer buff;
} teensy_msg;

cmd_vel
create_cmd_vel();

imu_data
create_imu_data();

int
cmd_vel_to_wire(cmd_vel* vel);

int
cmd_vel_from_wire(cmd_vel* vel, const uint8_t* data);

teensy_msg
create_teensy_msg();

int
teensy_msg_to_wire(teensy_msg* msg);

int
teensy_msg_from_wire(teensy_msg* msg, const uint8_t* data);

int
imu_data_to_wire(imu_data* msg);

int
imu_data_from_wire(imu_data* msg, const uint8_t* data);

#endif /* end of include guard RMT_MESSAGES_H */
