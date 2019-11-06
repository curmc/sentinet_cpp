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
    "cmd_vel", "command", "data", "realtime", true, true);

  a->init_comms("tcp://localhost:5571",
                "tcp://localhost:5580",
                "tcp://localhost:5555",
                "tcp://localhost:5581");

  // a->init_teensy_peripheral("192.168.0.2", 80);

  std::cout << sizeof(scpp::curmt::KermitKernel) << std::endl;
  a->start(std::chrono::milliseconds(10), std::chrono::seconds(0));

  a->kermit_quit();

  return 0;
}
