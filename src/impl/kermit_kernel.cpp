/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : kermit
 * @created     : Friday Aug 30, 2019 15:10:01 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"
//#include "control/RandomPublisher.hpp"
#include <csignal>

static std::unique_ptr<scpp::curmt::KermitKernel> a;

void
signalHandler(int signum)
{
  a.reset();
  exit(0);
}

int
main(int argc, char* argv[])
{
  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);

  bool debug = true;
  bool verbose = false;
  int i = 0;

  while (i < argc) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") || argc == 1) {
      std::cout
        << "\nUsage: kermit_kernel [options] --debug [off/on]\n"
           "Options:\n\n"
           "   --help        Show this help menu\n\n"

           "   --debug       On or off (defaults to on) \n"
           "                 If Off, kermit kernel will attempt to make a\n"
           "                 connect to the ip address of the teensy\n\n"

           "   --verbose     Show linear and angular values of kermit "
           "consistantly\n\n"

           "   --ip          If Debug is on, this does nothing. Otherwise, "
           "specify\n"
           "                 the ip address of the teensy (default "
           "192.168.0.2)\n\n"

           "   --port        If Debug is on, this does nothing. Otherwise, "
           "specify\n"
           "                 the port of the teensy (default 80)\n\n"

           "   --interface   If Debug is on, this does nothing. Otherwise, "
           "specify\n"
           "                 the interface to use (defaults to eth0)\n\n";
      return 0;
    }
    if (!strcmp(argv[i], "--debug")) {
      if (++i < argc) {
        debug = strcmp(argv[i], "off") ? true : false;
        continue;
      }
      std::cout << "Error, please specify debug on or off" << std::endl;
      return 1;
    }
    if (!strcmp(argv[i], "--verbose")) {
      verbose = true;
    }
    i++;
  }

  a = std::make_unique<scpp::curmt::KermitKernel>(verbose, debug);
  a->set_serial_attributes("/dev/ttyACM0", std::chrono::milliseconds(100));
  a->set_alive_time(std::chrono::seconds(0));
  a->start();

  return 0;
}
