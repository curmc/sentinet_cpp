/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : data
 * @created     : Monday Nov 25, 2019 13:11:09 MST
 */

extern "C" {
#include "scpp/messages/data_message.h"
}
#include "gtest/gtest.h"

namespace {
TEST(Messages, Data) {
  std::string data = "Hello World";

  // For code coverage
  print_memory((void*)&data[0], strlen(&data[0]));
  print_memory_hex((void*)&data[0], strlen(&data[0]));


  // TODO Fix the methods and then add tests here
}

TEST(Messages, DataMessage) {
  data_buffer buff;
  create_buffer(&buff);
  
  uint8_t data8 = 5;
  uint16_t data16 = 5;
  uint32_t data32 = 5;
  uint64_t data64 = 5;
  float datafloat = 5.6697928323094823049823947234028734;
  std::string message = "Hello World, This is a really long string to serialize that should be pretty large";
  

  EXPECT_EQ(serialize_data(&buff, (void*)&data8, sizeof(uint8_t), UINT8), 0);
  EXPECT_EQ(serialize_data(&buff, (void*)&data16, sizeof(uint16_t), UINT16), 0);
  EXPECT_EQ(serialize_data(&buff, (void*)&data32, sizeof(uint32_t), UINT32), 0);
  EXPECT_EQ(serialize_data(&buff, (void*)&data64, sizeof(uint64_t), UINT64), 0);
  EXPECT_EQ(serialize_data(&buff, (void*)&datafloat, sizeof(uint64_t), FLOAT), 0);
  EXPECT_EQ(serialize_data(&buff, (void*)&message[0], strlen(&message[0]), STRING), 0);
  EXPECT_EQ(to_wire(&buff), 0);
  print_message_formatted(buff.data);

  uint8_t* incomming = (uint8_t*)buff.data;

  uint8_t data8clone;
  uint16_t data16clone;
  uint32_t data32clone; 
  uint64_t data64clone;
  float datafloatclone;
  char buffer[256];
  memset(buffer, 0, 256); // A Bit of a Hack - terminates buffer in a nullterminator without effort on my part
  
  get_data(incomming, &data8clone, 0);  
  get_data(incomming, &data16clone, 1);  
  get_data(incomming, &data32clone, 2);  
  get_data(incomming, &data64clone, 3);  
  get_data(incomming, &datafloatclone, 4);
  get_data(incomming, buffer, 5);  

  std::string messageclone(buffer);

  EXPECT_EQ(data8clone, data8);
  EXPECT_EQ(data16clone, data16);
  EXPECT_EQ(data32clone, data32);
  EXPECT_EQ(data64clone, data64);
  EXPECT_FLOAT_EQ(datafloatclone, datafloat);
  EXPECT_EQ(messageclone, message);
}



}
