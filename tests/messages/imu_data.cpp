/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : rmt
 * @created     : Tuesday Nov 26, 2019 11:41:37 MST
 */

#include "gtest/gtest.h"
#include <cmath>
extern "C" {
#include "scpp/messages/rmt_messages.h"
}


TEST(RMTMessageTest, IMUTest) {
  auto imu_recv = create_cmd_vel();
  auto imu_send = create_cmd_vel();
  uint8_t* message;

  imu_recv.lin = 6.123152342;
  imu_recv.ang = 8.012372139;

  EXPECT_EQ(-1, cmd_vel_to_wire(nullptr));
  EXPECT_EQ(1, cmd_vel_to_wire(&imu_recv));
  
  message = (uint8_t*)imu_recv.buff.data;

  // EXPECT_EQ(-1, cmd_vel_from_wire(&imu_send, "test string")); // TODO CHECSUM
  EXPECT_EQ(-1, cmd_vel_from_wire(&imu_send, nullptr));
  EXPECT_EQ(-1, cmd_vel_from_wire(nullptr, nullptr));
  EXPECT_EQ(1, cmd_vel_from_wire(&imu_send, message));
  EXPECT_FLOAT_EQ(imu_recv.lin, imu_send.lin);
  EXPECT_FLOAT_EQ(imu_recv.ang, imu_send.ang);

  // Boundary Conditions
  imu_recv.lin = 0;
  imu_recv.ang = 0;

  EXPECT_EQ(1, cmd_vel_to_wire(&imu_recv));
  
  message = (uint8_t*)imu_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&imu_send, message));
  EXPECT_FLOAT_EQ(imu_recv.lin, imu_send.lin);
  EXPECT_FLOAT_EQ(imu_recv.ang, imu_send.ang);


  imu_recv.lin = FLT_MAX;
  imu_recv.ang = FLT_MAX;
  EXPECT_EQ(1, cmd_vel_to_wire(&imu_recv));
  
  message = (uint8_t*)imu_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&imu_send, message));
  EXPECT_FLOAT_EQ(imu_recv.lin, imu_send.lin);
  EXPECT_FLOAT_EQ(imu_recv.ang, imu_send.ang);


  imu_recv.lin = FLT_MIN;
  imu_recv.ang = FLT_MIN;
  EXPECT_EQ(1, cmd_vel_to_wire(&imu_recv));
  
  message = (uint8_t*)imu_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&imu_send, message));
  EXPECT_FLOAT_EQ(imu_recv.lin, imu_send.lin);
  EXPECT_FLOAT_EQ(imu_recv.ang, imu_send.ang);



  imu_recv.lin = FLT_MAX;
  imu_recv.ang = FLT_MAX;
  EXPECT_EQ(1, cmd_vel_to_wire(&imu_recv));
  
  message = (uint8_t*)imu_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&imu_send, message));
  EXPECT_FLOAT_EQ(imu_recv.lin, imu_send.lin);
  EXPECT_FLOAT_EQ(imu_recv.ang, imu_send.ang);


  imu_recv.lin = FP_INFINITE;
  imu_recv.ang = FP_INFINITE;
  EXPECT_EQ(1, cmd_vel_to_wire(&imu_recv));
  
  message = (uint8_t*)imu_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&imu_send, message));
  EXPECT_FLOAT_EQ(imu_recv.lin, imu_send.lin);
  EXPECT_FLOAT_EQ(imu_recv.ang, imu_send.ang);


  imu_recv.ang = FP_ZERO;
  imu_recv.ang = FP_ZERO;
  EXPECT_EQ(1, cmd_vel_to_wire(&imu_recv));
  
  message = (uint8_t*)imu_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&imu_send, message));
  EXPECT_FLOAT_EQ(imu_recv.lin, imu_send.lin);
  EXPECT_FLOAT_EQ(imu_recv.ang, imu_send.ang);


  imu_recv.ang = -8.92318;
  imu_recv.ang = -9.21309;
  EXPECT_EQ(1, cmd_vel_to_wire(&imu_recv));
  
  message = (uint8_t*)imu_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&imu_send, message));
  EXPECT_FLOAT_EQ(imu_recv.lin, imu_send.lin);
  EXPECT_FLOAT_EQ(imu_recv.ang, imu_send.ang);

}
