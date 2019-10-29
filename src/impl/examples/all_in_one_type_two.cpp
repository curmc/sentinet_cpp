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

  
  auto a = std::make_unique<scpp::net::ZMQControlClient>(true);
  a->initialize_client();

  scpp::serve_params b;

  b.address = "tcp://localhost:5580";
  b.callback = [&value](std::string&) -> std::string {
    std::cout<<"Made it"<<std::endl;
    return "hello";
  };

  a->spin(b);
  sleep(2);

  std::cout<<"here"<<std::endl;
  a->request("tcp://localhost:5572", "helloooo");
  std::cout<<"here2"<<std::endl;
  a->request("tcp://localhost:5572", "helloooo");

  sleep(2);


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

  a->quit();

  return 0;
}
