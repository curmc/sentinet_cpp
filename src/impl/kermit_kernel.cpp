/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : kermit_kernel
 * @created     : Sunday Oct 13, 2019 14:35:33 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"
#include "scpp/kernel/Defaults.hpp"

int
main()
{
  auto a = std::make_unique<scpp::curmt::KermitKernel>(
    scpp::curmt::defaults::cmd_vel_topic,
    scpp::curmt::defaults::cmd_vel_front_end,
    scpp::curmt::defaults::cmd_vel_back_end);

  a->set_serial("/dev/ttyACM0", 9600);
  a->start(std::chrono::seconds(1));
  sleep(100);
  a->quit();
}
