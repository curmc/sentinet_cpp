/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : kermit
 * @created     : Friday Aug 30, 2019 15:10:01 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"
//#include "control/RandomPublisher.hpp"

#define DEBUG

int
main()
{
  auto a = std::make_unique<scpp::curmt::KermitKernel>(
    "drive", "tcp://localhost:5555", "tcp://localhost:5556");

  //  auto b = std::make_unique<RandomPublisher>();
  //  b->loop();

  a->set_serial("./this", 9600);
  a->start(std::chrono::seconds(1));

  //  b->quit();
  a->quit();
  return 0;
}
