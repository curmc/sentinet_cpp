/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : kermit
 * @created     : Friday Aug 30, 2019 15:10:01 MDT
 */

#include "kernel/KermitKernel.hpp"
//#include "control/RandomPublisher.hpp"

int main() {
  auto a = std::make_unique<Kermit::KermitKernel>("drive");

  //  auto b = std::make_unique<RandomPublisher>();
  //  b->loop();

  a->set_serial("/dev/ttyUSB0", 9600);
  a->loop(std::chrono::milliseconds(1));

  //  b->quit();
  a->quit();
  return 0;
}
