

#include <scpp/control/ZMQControlClient.hpp>

#include <string>

int main() {

  auto a = std::make_unique<scpp::net::ZMQControlClient>();

  std::string value_to_publish = "Hello";

  a->publish(
      "tcp://localhost:5570",

      "cmd_vel",

      [&]() -> std::string & {
        std::cout << "Sending " << value_to_publish << std::endl;

        return value_to_publish;
      },

      std::chrono::seconds(1));

  sleep(3);

  a->quit();

  return 0;
}
