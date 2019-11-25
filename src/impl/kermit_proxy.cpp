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

const std::string CMD_VEL = "tcp://*:5570";
const std::string DATA_ADDR = "tcp://*:5556";
const std::string COMMAND_ADDR = "tcp://*:5572";
const std::string REAL_TIME_ADDR = "tcp://*:5573";

const std::string CMD_VEL_F = "tcp://*:5571";
const std::string DATA_ADDR_F = "tcp://*:5555";
const std::string COMMAND_ADDR_F = "tcp://*:5580";
const std::string REAL_TIME_ADDR_F = "tcp://*:5581";

int
main(int argc, char* argv[])
{
  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);

  int cmdvel = 0;
  int realtime = 0;
  int data = 0;
  int cmd = 0;
  int timer = 0;
  long time_period = 0;

  int i = 0;

  while (i < argc) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") || argc == 1) {
      std::cout << "Usage: kermit_proxy [options] --time [time_period]\n";
      std::cout << "Options:\n\n";
      std::cout << "    --help          Show this help menu\n";
      std::cout
        << "    --cmd_vel       Start a new publisher proxy on cmd_vel topic\n";
      std::cout << "    --rt            Start a new real time proxy\n";
      std::cout << "    --data          Start a new data proxy on topic data\n";
      std::cout << "    --command       Start a new command proxy\n";
      std::cout
        << "    --time          Specify the time period in seconds to run\n";
      std::cout << "                    -1 for infinite (ctrl+C to exit)\n";
      std::cout << "                    else any number for the number of "
                   "seconds to run\n";
      std::cout << "                    (with ctrl+c to exit\n";
      return 0;
    }
    if (!strcmp(argv[i], "--cmd_vel") && !cmdvel) {
      cmdvel = 1;
    }
    if (!strcmp(argv[i], "--rt") && !realtime) {
      realtime = 1;
    }
    if (!strcmp(argv[i], "--data") && !data) {
      data = 1;
    }
    if (!strcmp(argv[i], "--command") && !cmd) {
      data = 1;
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

  if (realtime) {
    proxies->create_pub_sub_endpoint(
      "realtime", REAL_TIME_ADDR_F, REAL_TIME_ADDR);
  }

  if (data) {
    proxies->create_pub_sub_endpoint("data", DATA_ADDR_F, DATA_ADDR);
  }

  if (cmd) {
    proxies->create_req_rep_endpoint("command", COMMAND_ADDR, COMMAND_ADDR_F);
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
