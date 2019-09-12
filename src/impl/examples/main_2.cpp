/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main
 * @created     : Wednesday Jul 24, 2019 11:24:28 MDT
 */

#include "networking/zmq/ZMQControlClient.hpp"

int main() {

  auto a = std::make_unique<ZMQControlClient>();

  std::string value("Hello from publisher 3");

  a->subscribe("tcp://localhost:5555", "Topic_name",
               [&](const std::string &value) -> void {
                 std::cout << "Recieved " << value << " From Topic1"
                           << std::endl;
               });

  a->subscribe(
      "tcp://localhost:5556", "Topic2", [&](const std::string &value) -> void {
        std::cout << "Recieved " << value << " From topic2" << std::endl;
      });

  a->publish(
      "tcp://*:5559", "topic_outside", [&]() -> std::string & { return value; },
      std::chrono::milliseconds(900));

  a->publish("Topic name", "hello");

  sleep(6);

  a->quit();
  return 0;
}
