/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : kermit
 * @created     : Friday Aug 30, 2019 15:10:01 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"
//#include "control/RandomPublisher.hpp"

int
main()
{
  auto a = std::make_unique<scpp::curmt::KermitKernel>(
    "cmd_vel", "command", "data", "real_map", true, true);

  a->init_comms("tcp://localhost:5571",
                "tcp://localhost:5556",
                "tcp://localhost:5557",
                "tcp://localhost:5558");

  std::cout<<sizeof(scpp::curmt::KermitKernel)<<std::endl;
  a->start(std::chrono::seconds(1));

  a->quit();
  return 0;
}
