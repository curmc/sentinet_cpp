/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : kermit_proxy
 * @created     : Sunday Oct 13, 2019 14:35:45 MDT
 */

#include "scpp/core/messages/pipe/PipeInterface.hpp"

const std::string CMD_VEL = "tcp://*:5570";
const std::string DATA_ADDR = "tcp://*:5556";
const std::string COMMAND_ADDR = "tcp://*:5572";
const std::string REAL_TIME_ADDR = "tcp://*:5573";

const std::string CMD_VEL_F = "tcp://*:5571";
const std::string DATA_ADDR_F = "tcp://*:5555";
const std::string COMMAND_ADDR_F = "tcp://*:5580";
const std::string REAL_TIME_ADDR_F = "tcp://*:5581";

int
main()
{
  auto proxies = std::make_unique<scpp::core::PipeInterface>();

  proxies->create_pub_sub_endpoint("cmd_vel", CMD_VEL, CMD_VEL_F);
  proxies->create_pub_sub_endpoint("data", DATA_ADDR_F, DATA_ADDR);
  proxies->create_pub_sub_endpoint("realtime", REAL_TIME_ADDR_F, REAL_TIME_ADDR);
  proxies->create_req_rep_endpoint("command", COMMAND_ADDR, COMMAND_ADDR_F);


  sleep(1000);
  std::cout << "Killing " << std::endl;

  proxies->stop();
  return 1;
}
