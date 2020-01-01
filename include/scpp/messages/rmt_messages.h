/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : rmt_messages
 * @created     : Saturday Oct 26, 2019 17:06:35 MDT
 */

#ifndef RMT_MESSAGES_H

#define RMT_MESSAGES_H

// #define LIGHT_MESSAGES
#include "data_message.h"

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
  float x;
  float y;
  float z;
  float theta;
  size_t indexes[4];
  data_buffer buff;
} pos_localizer;

typedef struct
{
  int16_t lin;
  int16_t ang;
  size_t indexes[2];
  data_buffer buff;
} teensy_msg;


pos_localizer
create_pos_localizer();

cmd_vel
create_cmd_vel();

imu_data
create_imu_data();

teensy_msg
create_teensy_msg();

// CMD_VEL
int
cmd_vel_to_wire(cmd_vel* vel);

int
cmd_vel_from_wire(cmd_vel* vel, const uint8_t* data);

// POS_LOCALIZER
int
pos_localizer_to_wire(pos_localizer* pos);

int
pos_localizer_from_wire(pos_localizer* pos, const uint8_t* data);

// TEENSY_MSG
int
teensy_msg_to_wire(teensy_msg* msg);

int
teensy_msg_from_wire(teensy_msg* msg, const uint8_t* data);

// IMU_DATA
int
imu_data_to_wire(imu_data* msg);

int
imu_data_from_wire(imu_data* msg, const uint8_t* data);

#endif /* end of include guard RMT_MESSAGES_H */
