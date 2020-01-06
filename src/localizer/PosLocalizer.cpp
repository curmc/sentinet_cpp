/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : PosLocalizer
 * @created     : Sunday Jan 05, 2020 11:28:13 MST
 */

#include "scpp/localizer/PosLocalizer.hpp"

static std::mutex guard;

namespace scpp {
namespace curmt {
PosLocalizer::PosLocalizer(bool verbose)
  : RMTControlClient(to_bind_addr(addr::camera::ADDRESS))
{
  pos.msg = create_camera_msg();

  publish_params p;
  p.sock_addr = to_conn_addr(addr::camera::LOCALIZER);
  p.period = std::chrono::seconds(1);
  p.topic = addr::localizer_topic;
  p.get_data = std::bind(&scpp::curmt::PosLocalizer::get_pos, this);

  spin(p);
}

void
PosLocalizer::set_pos(float x, float y, float theta)
{
  std::lock_guard<std::mutex> lock(guard);
  pos.x = x;
  pos.y = y;
  pos.theta = theta;

  return;
}

void
PosLocalizer::print_state()
{
  printf("(x, y, theta) (%f %f %f)\n", pos.x, pos.y, pos.theta);
}

std::string
PosLocalizer::get_pos(void)
{
  std::lock_guard<std::mutex> lock(guard);
  pos.msg.x = pos.x;
  pos.msg.y = pos.y;
  pos.msg.theta = pos.theta;

  camera_msg_to_wire(&pos.msg);

  size_t size = pos.msg.msg.buff.byte_length;
  char const* msg = reinterpret_cast<char const*>(pos.msg.msg.buff.data);
  return std::string(msg, size);
}

}
}
