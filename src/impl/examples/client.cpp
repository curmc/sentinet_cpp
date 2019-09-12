/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : client
 * @created     : Friday Aug 23, 2019 19:22:49 MDT
 */

#include "networking/zmq/ZMQControlClient.hpp"
#include "messages/common/DriveTrain.hpp"

int main() {
  auto a = std::make_unique<ZMQControlClient>();

  DriveTrain dt;

  a->initialize_client();
  std::string ret = a->request("tcp://localhost:5555", "hello");

  dt.parse_from_string(ret);

  std::cout << "Recieved a dt with linear = " << dt.get_linear() << " "
            << dt.get_angular() << std::endl;
  std::cout << "Proof: lin + ang = " << dt.get_linear() + dt.get_angular()
            << std::endl;

  std::cout << ret << std::endl;
  std::cout << dt.print() << std::endl;
  a->quit();

  return 0;
}
