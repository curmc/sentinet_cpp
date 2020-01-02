/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : kermit_proxy
 * @created     : Sunday Oct 13, 2019 14:35:45 MDT
 */

#include "scpp/core/messages/pipe/PipeInterface.hpp"
#include <csignal>

static std::unique_ptr<scpp::core::PipeInterface> proxies;

void
signalHandler(int signum)
{
  if (proxies)
    proxies->stop();
  exit(0);
}


int
main(int argc, char* argv[])
{
  // Exit signals
  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);

  int cmdvel = 0;
  int camera = 0;
  int cmd = 0;
  int timer = 0;
  long time_period = 0;

  int i = 0;

  while (i < argc) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") || argc == 1) {
      std::cout
        << "\nUsage: kermit_proxy [options] --time [time_period]\n"
           "Options:\n\n"
           "    --help          Show this help menu\n\n"

           "    --cmd_vel       Start a new publisher proxy on cmd_vel "
           "topic\n\n"

           "    --camera          Start a new camera proxy on topic camera\n\n"

           "    --command       Start a new command proxy\n\n"

           "    --time          Specify the time period in seconds to run\n"
           "                    -1 for infinite (ctrl+C to exit)\n"
           "                    else any number for the number of \n"
           "                    seconds to run\n\n"
           "                    (with ctrl+c to exit\n\n";
      return 0;
    }
    if (!strcmp(argv[i], "--cmd_vel") && !cmdvel) {
      cmdvel = 1;
    }
    if (!strcmp(argv[i], "--camera") && !camera) {
      camera = 1;
    }
    if (!strcmp(argv[i], "--command") && !cmd) {
      cmd = 1;
    }
    if ((!strcmp(argv[i], "--time") || !strcmp(argv[i], "-t")) && !timer) {
      timer = 1;
      if (++i < argc) {
        time_period = strtol(argv[i], nullptr, 10);
        if (time_period)
          continue;
      }
      std::cout << "Error invalid time period specified, exiting\n";
      return 1;
    }
    i++;
  }

  proxies = std::make_unique<scpp::core::PipeInterface>();

  if (cmdvel) {
    proxies->create_pub_sub_endpoint("cmd_vel", CMD_VEL, CMD_VEL_F);
  }

  if (camera) {
    proxies->create_pub_sub_endpoint("camera", CAMERA_ADDR_F, CAMERA_ADDR);
  }

  if (cmd) {
    proxies->create_req_rep_endpoint("command_kernel", COMMAND_ADDR, COMMAND_ADDR_F);
    proxies->create_req_rep_endpoint("command_localizer", COMMAND_ADDR, COMMAND_ADDR_F);
    proxies->create_req_rep_endpoint("command_tracker", COMMAND_ADDR, COMMAND_ADDR_F);
  }

  if (time_period == -1) {
    for (;;)
      ;
  }

  sleep(time_period);
  std::cout << "Killing " << std::endl;

  proxies->stop();
  return 1;
}
