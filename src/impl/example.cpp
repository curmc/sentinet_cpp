

#include "networking/zmq/ZMQControlClient.hpp"

std::string value;

std::string &func() { return value; }

void func2(const std::string &val) { std::cout << val << std::endl; }

int main() {

  value = "Hi there";

  auto a = std::make_unique<ZMQControlClient>();

  a->publish("tcp://*:5555", "Topic name", func, std::chrono::seconds(1));

  a->subscribe("tcp://localhost:5555", "Topic name", func2);

  sleep(10);

  a->quit();

  return 0;
}
