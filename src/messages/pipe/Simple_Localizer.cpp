/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : Simple_Localizer
 * @created     : Saturday Jan 04, 2020 09:39:01 MST
 */

#include <iostream>
#include "scpp/messages/pipe/Simple_Localizer.hpp"

namespace scpp {
namespace filters {

Simple_Localizer::Simple_Localizer()
{
  message.msg = create_path_state();
  message.sensor = create_teensy_sensor_array();
  message.camera = create_camera_msg();

  memset(&state, 0, sizeof(state));
}

int
Simple_Localizer::convert(std::string& incomming_message)
{
  sensor_msg msg = create_sensor_msg();
  const uint8_t* data = reinterpret_cast<uint8_t*>(&incomming_message[0]);
  int ret;

  // Recieved a teensy message
  if ((ret = sensor_msg_from_wire(&msg, data)) == TEENSY_MSG) {
    teensy_sensor_array_from_wire(&message.sensor, data);
    teensy_update(message.sensor.v, message.sensor.w, message.sensor.dt);

    // Recieved a camera message
  } else if (ret == CAMERA_MESSAGE) {
    camera_msg_from_wire(&message.camera, data);
    camera_update(message.camera.x,
                  message.camera.y,
                  message.camera.theta,
                  message.camera.dt);
  } else {
    ret = -1;
  }

  // Update outgoing message
  message.msg.x = state.x, message.msg.y = state.y;
  message.msg.theta = state.theta;
  message.msg.v = state.v;
  message.msg.w = state.w;

  // Serialize data
  path_state_to_wire(&message.msg);

  // Update the incomming message
  size_t size = message.msg.buff.byte_length;
  char const* ptr = reinterpret_cast<char const*>(message.msg.buff.data);

  incomming_message = std::string(ptr, size);

  return ret;
}

void
Simple_Localizer::teensy_update(float v, float w, float dt)
{
  state.v = v;
  state.w = w;
}

void
Simple_Localizer::camera_update(float x, float y, float theta, float dt)
{
  state.x = x;
  state.y = y;
  state.theta = theta;
}

}
}
