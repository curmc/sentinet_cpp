

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

  a->subscribe(
      "tcp://localhost:5571",
      "cmd_vel",
      [&](const std::string& value) -> void {
        std::cout<<"                    Recieved: "<<value <<std::endl;
        });


  sleep(5);
  a->quit();
  return 0;
}
