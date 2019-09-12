/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main
 * @created     : Wednesday Jul 24, 2019 11:24:28 MDT
 */

#include "networking/zmq/ZMQControlClient.hpp"
#include <unistd.h>

int main() {

  std::cout << "Hi there" << std::endl;
  std::unique_ptr<ZMQControlClient> a = std::make_unique<ZMQControlClient>();

  // Create some string references
  std::string value("hi from publisher 1");
  std::string value2("hi from publisher 2");

  // Set up a publisher that publishes every 1 second
  a->publish(
      "tcp://*:5555", "Topic_name", [&]() -> std::string & { return value; },
      std::chrono::seconds(1));

  // Set up a publisher that publishes every 500 milliseconds
  a->publish(
      "tcp://*:5556", "Topic2", [&]() -> std::string & { return value2; },
      std::chrono::milliseconds(500));

  // Set up a subscriber to listen on topic_outside topic, connected to
  // publisher on port 5559
  a->subscribe("tcp://localhost:5559", "topic_outside",
               [&](const std::string &value) -> void {
                 std::cout << "Recieved " << value
                           << " From subscriber topic_outside" << std::endl;
               });

  // All of the above are asynchronous, so this could be a while(1){ do stuff }
  // loop
  sleep(10);

  // Terminate everything
  a->quit();
  return 0;
}
