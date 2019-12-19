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
  if (a)
    a->kermit_quit();
  std::cout << "Killed" << std::endl;
  exit(0);
}

char ipaddr[20] = "192.168.0.2";
char interface[20] = "eth0";

int
main(int argc, char* argv[])
{
  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);

  bool debug = true;
  bool verbose = false;
  int i = 0;
  int port = 80;

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
    if (!strcmp(argv[i], "--ip")) {
      if (++i < argc) {
        size_t len = strlen(argv[i]);
        memset(ipaddr, 0, 20);
        strncpy(ipaddr, argv[i], len > 20 ? 20 : len);
        continue;
      }
      std::cout << "Error please specify an ipaddr" << std::endl;
      return 1;
    }
    if (!strcmp(argv[i], "--port")) {
      if (++i < argc) {
        port = atoi(argv[i]);
        continue;
      }
      std::cout << "Error please specify a port number" << std::endl;
      return 1;
    }
    if (!strcmp(argv[i], "--interface")) {
      if (++i < argc) {
        size_t len = strlen(argv[i]);
        memset(interface, 0, 20);
        strncpy(interface, argv[i], len > 20 ? 20 : len);
        continue;
      }
      std::cout << "Error please specify a port number" << std::endl;
      return 1;
    }
    i++;
  }

  a = std::make_unique<scpp::curmt::KermitKernel>(
    "cmd_vel", "command", "data", "realtime", verbose, debug);

  a->init_comms("tcp://localhost:5571",
                "tcp://localhost:5580",
                "tcp://localhost:5555",
                "tcp://localhost:5581");
  std::cout << sizeof(scpp::curmt::KermitKernel) << std::endl;
  if (!debug) {
    a->init_teensy_peripheral("/dev/ttyACM0");
  }

  a->start(std::chrono::milliseconds(10), std::chrono::seconds(0));

  a->kermit_quit();
  return 0;
}
