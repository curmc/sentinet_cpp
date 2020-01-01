/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : Localizer
 * @created     : Wednesday Jan 01, 2020 14:21:46 MST
 */

#include "scpp/controllers/apriltags/Localizer.hpp"
#include <mutex>


static std::mutex guard; 

namespace scpp {
namespace curmt {

Localizer::Localizer(bool verbose)
{
  network.camera_addr = camera_addr;
  network.camera_topic = camera_topic;
  
  network.camera_p.broker_frontend = camera_addr;
  network.camera_p.topic = camera_topic;
  network.camera_p.period = std::chrono::seconds(1);
  network.camera_p.get_data =
    std::bind(&Localizer::get_pos, this);

  message.loc = create_pos_localizer();
  message.size = message.loc.buff.byte_length;
  running = true;
}

bool Localizer::localizer_start(const std::chrono::seconds period_) {
  network.camera_p.period = period_;  
  spin(network.camera_p);
  return true;
}

bool Localizer::localizer_quit() {
  running = false;
  return quit();
}

void Localizer::set_pos(float x, float y, float z, float theta) {
  std::lock_guard<std::mutex> lock(guard);
  message.loc.x = x;
  message.loc.y = y;
  message.loc.z = z;
  message.loc.theta = theta;
}

std::string Localizer::get_pos(void) {
  std::lock_guard<std::mutex> lock(guard);
  pos_localizer_to_wire(&message.loc);
  return std::string(reinterpret_cast<char const*>(message.loc.buff.data), message.size);
}

void Localizer::print_state() {
  std::lock_guard<std::mutex> lock(guard);
  std::cout<<"x: "
    << message.loc.x
    << " y: "
    << message.loc.y 
    << " z: "
    << message.loc.z 
    << " theta: "
    << message.loc.theta << std::endl;
}


}
}
