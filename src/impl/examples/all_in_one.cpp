
#include "control/ZMQControlClient.hpp"

int main() {

  auto a = std::make_unique<ZMQControlClient>();

  std::string value_to_publish("Hi there");

  a->publish(
      "tcp://localhost:5570", "Topic",
      [&]() -> std::string & {
        std::cout << "Seding " << value_to_publish << std::endl;
        return value_to_publish;
      },
      std::chrono::seconds(1));

  a->subscribe("tcp://localhost:5571", "Topic",
               [&](const std::string &value) -> void {
                 std::cout << "recieved " << value << std::endl;
               });

  sleep(10);
  a->quit();

  return 0;
}
