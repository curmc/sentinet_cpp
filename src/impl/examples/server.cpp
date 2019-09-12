/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : server
 * @created     : Friday Aug 23, 2019 19:20:32 MDT
 */

#include "networking/zmq/ZMQControlClient.hpp"
#include "messages/common/DriveTrain.hpp"

int main() {
  auto a = std::make_unique<ZMQControlClient>();

  DriveTrain dt;

  dt.set_linear(5);
  dt.set_angular(6);

  std::string val_to_serve;
  dt.serialize_to_string(&val_to_serve);

  a->serve("tcp://*:5555",
           [&val_to_serve](const std::string &v) -> std::string {
             return val_to_serve;
           });

  std::cout << "Sleeping" << std::endl;
  sleep(10);
  std::cout << "Done Sleeping" << std::endl;
  a->quit();
  return 0;
}
