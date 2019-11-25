/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : NetworkPatterns
 * @created     : Saturday Nov 23, 2019 23:28:21 MST
 */

#include "ZMQControlClient.hpp"
#include "scpp/core/messages/pipe/PipeInterface.hpp"

namespace {

static std::unique_ptr<scpp::core::PipeInterface> proxies;
void signalHandler(int signum) {
  if(proxies)
    proxies->stop();
}

TEST(ZMQControlClientTest, LifeCycle) {
  auto client = std::make_unique<BootstrapZMQControlClient>(1);

  EXPECT_TRUE(client->initialize_publisher("tcp://localhost:5555"));
  EXPECT_FALSE(client->initialize_publisher("tcp://loalhost:5556"));
  EXPECT_TRUE(client->initialize_client());
  EXPECT_FALSE(client->initialize_client());

  EXPECT_TRUE(client->start());
  
  // Publishers publish blindly, doesn't matter if endpoint is up or not
  EXPECT_TRUE(client->publish("cmd_vel", "test"));

  // Requesters should return false if the server is down
  EXPECT_EQ(client->request("tcp://localhost:5557", "hello"), "No response");

  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);

  proxies = std::make_unique<scpp::core::PipeInterface>();

  proxies->create_pub_sub_endpoint("cmd_vel", "tcp://*:5577", "tcp://*:5578");
  proxies->create_req_rep_endpoint("test", "tcp://*:5581", "tcp://*:5580");

  {
    EXPECT_TRUE(client->publish("tcp://localhost:5577", "cmd_vel",
          [] (void) -> std::string {
              return "TEST";
          }, std::chrono::microseconds(1)));

    EXPECT_TRUE(client->subscribe("tcp://localhost:5578", "cmd_vel",
          [] (std::string value) -> void {
              EXPECT_EQ(value, "TEST");
          }));

    EXPECT_TRUE(client->serve("tcp://localhost:5580", 
          [] (std::string& value) -> std::string {
            EXPECT_EQ(value, "TEST");
            return value + "RESP";
          }));

    std::string resp = client->request("tcp://localhost:5581", "TEST");
    EXPECT_EQ(resp, "TESTRESP");

    sleep(1);
  }

  proxies->stop();
  EXPECT_TRUE(client->quit());
}

}
