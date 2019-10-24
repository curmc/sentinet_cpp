/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : all_in_one_type_two
 * @created     : Wednesday Sep 11, 2019 17:56:50 MDT
 */

#include "scpp/control/ZMQControlClient.hpp"
#include "scpp/core/utils/logging.hpp"

std::mutex t;

void
func(std::string& val)
{
  std::cout << "Recieved " << val << std::endl;
}

int
main()
{
  std::string value("Well hello there");

  scpp::subscribe_params b;

  b.callback = func;

  b.callback = [](std::string& val) -> void {
    std::cout << "Recieved: " << val << std::endl;
  };
  b.topic = "drive";
  b.socket_backend = "tcp://localhost:5555";

  //
  // a.broker_frontend = "tcp://localhost:5570";
  // a.topic = "topic";
  //
  // a.get_data = [&value](void) -> std::string {
  //   std::cout << "Sending: " << value << std::endl;
  //   return value;
  // };
  // a.period = std::chrono::seconds(1);
  //
  // scpp::subscribe_params b;
  // b.callback = [](std::string& val) -> void {
  //   std::cout << "Recieved " << val << std::endl;
  // };
  // b.socket_backend = "tcp://localhost:5571";
  // b.topic = "topic";
  //
  auto cc = std::make_unique<scpp::net::ZMQControlClient>();
  //
  cc->spin(b);
  // cc->spin(a);
  //
  sleep(10);

  cc->quit();

  return 0;
}
