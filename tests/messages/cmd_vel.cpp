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


TEST(RMTMessageTest, CmdVel) {
  auto cvel_recv = create_cmd_vel();
  auto cvel_send = create_cmd_vel();
  uint8_t* message;

  cvel_recv.lin = 6.123152342;
  cvel_recv.ang = 8.012372139;

  EXPECT_EQ(-1, cmd_vel_to_wire(nullptr));
  EXPECT_EQ(1, cmd_vel_to_wire(&cvel_recv));
  
  message = (uint8_t*)cvel_recv.buff.data;

  // EXPECT_EQ(-1, cmd_vel_from_wire(&cvel_send, "test string")); // TODO CHECSUM
  EXPECT_EQ(-1, cmd_vel_from_wire(&cvel_send, nullptr));
  EXPECT_EQ(-1, cmd_vel_from_wire(nullptr, nullptr));
  EXPECT_EQ(1, cmd_vel_from_wire(&cvel_send, message));
  EXPECT_FLOAT_EQ(cvel_recv.lin, cvel_send.lin);
  EXPECT_FLOAT_EQ(cvel_recv.ang, cvel_send.ang);

  // Boundary Conditions
  cvel_recv.lin = 0;
  cvel_recv.ang = 0;

  EXPECT_EQ(1, cmd_vel_to_wire(&cvel_recv));
  
  message = (uint8_t*)cvel_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&cvel_send, message));
  EXPECT_FLOAT_EQ(cvel_recv.lin, cvel_send.lin);
  EXPECT_FLOAT_EQ(cvel_recv.ang, cvel_send.ang);


  cvel_recv.lin = FLT_MAX;
  cvel_recv.ang = FLT_MAX;
  EXPECT_EQ(1, cmd_vel_to_wire(&cvel_recv));
  
  message = (uint8_t*)cvel_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&cvel_send, message));
  EXPECT_FLOAT_EQ(cvel_recv.lin, cvel_send.lin);
  EXPECT_FLOAT_EQ(cvel_recv.ang, cvel_send.ang);


  cvel_recv.lin = FLT_MIN;
  cvel_recv.ang = FLT_MIN;
  EXPECT_EQ(1, cmd_vel_to_wire(&cvel_recv));
  
  message = (uint8_t*)cvel_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&cvel_send, message));
  EXPECT_FLOAT_EQ(cvel_recv.lin, cvel_send.lin);
  EXPECT_FLOAT_EQ(cvel_recv.ang, cvel_send.ang);



  cvel_recv.lin = FLT_MAX;
  cvel_recv.ang = FLT_MAX;
  EXPECT_EQ(1, cmd_vel_to_wire(&cvel_recv));
  
  message = (uint8_t*)cvel_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&cvel_send, message));
  EXPECT_FLOAT_EQ(cvel_recv.lin, cvel_send.lin);
  EXPECT_FLOAT_EQ(cvel_recv.ang, cvel_send.ang);


  cvel_recv.lin = FP_INFINITE;
  cvel_recv.ang = FP_INFINITE;
  EXPECT_EQ(1, cmd_vel_to_wire(&cvel_recv));
  
  message = (uint8_t*)cvel_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&cvel_send, message));
  EXPECT_FLOAT_EQ(cvel_recv.lin, cvel_send.lin);
  EXPECT_FLOAT_EQ(cvel_recv.ang, cvel_send.ang);


  cvel_recv.ang = FP_ZERO;
  cvel_recv.ang = FP_ZERO;
  EXPECT_EQ(1, cmd_vel_to_wire(&cvel_recv));
  
  message = (uint8_t*)cvel_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&cvel_send, message));
  EXPECT_FLOAT_EQ(cvel_recv.lin, cvel_send.lin);
  EXPECT_FLOAT_EQ(cvel_recv.ang, cvel_send.ang);


  cvel_recv.ang = -8.92318;
  cvel_recv.ang = -9.21309;
  EXPECT_EQ(1, cmd_vel_to_wire(&cvel_recv));
  
  message = (uint8_t*)cvel_recv.buff.data;

  EXPECT_EQ(1, cmd_vel_from_wire(&cvel_send, message));
  EXPECT_FLOAT_EQ(cvel_recv.lin, cvel_send.lin);
  EXPECT_FLOAT_EQ(cvel_recv.ang, cvel_send.ang);

}
