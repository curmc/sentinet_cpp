/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitControlModule
 * @created     : Saturday Oct 12, 2019 23:30:12 MDT
 */

#include "scpp/kernel/KermitControlModule.hpp"

namespace scpp {
namespace curmt {

static const std::string CMD_VEL = "tcp://localhost:5570";
static const std::string DATA_ADDR = "tcp://localhost:5556";
static const std::string COMMAND_ADDR = "tcp://localhost:5572";
static const std::string REAL_TIME_ADDR = "tcp://localhost:5573";

KermitControlModule::KermitControlModule()
{
  endpoints.cmd_addr = COMMAND_ADDR;
  endpoints.data_addr = DATA_ADDR;
  endpoints.cmd_vel_addr = CMD_VEL;
  endpoints.real_time_addr = REAL_TIME_ADDR;

  endpoints.cmd_id = "command";
  endpoints.data_topic = "data";
  endpoints.cmd_vel_topic = "cmd_vel";
  endpoints.real_time_topic = "real_time";

  message.cvel_buff = create_cmd_vel();
  message.data_buff = create_cmd_vel();
  message.command = create_buffer_ping();

  running = true;
}

KermitControlModule::~KermitControlModule()
{
  quit_kermit();
}

bool
KermitControlModule::start_kermit()
{
  __start__();

  // TODO make these into callbacks in the main object
  scpp::publish_params publish;
  publish.broker_frontend = endpoints.cmd_vel_addr;
  publish.topic = endpoints.cmd_vel_topic;
  publish.get_data = [this]() -> std::string {
    // In c you can take a temporary address of an rvlaue, but c++ wines
    // Maybe a std::move would fix this, I don't think it would
    message.cvel_buff.lin = __get_lin__();
    message.cvel_buff.ang = __get_ang__();
    cmd_vel_to_wire(&message.cvel_buff);
    return scpp::string((char const*)message.cvel_buff.buff.data, 27);
  };
  publish.period = std::chrono::milliseconds(10);

  scpp::subscribe_params subscribe;
  subscribe.socket_backend = endpoints.data_addr;
  subscribe.topic = endpoints.data_topic;
  subscribe.callback = [this](std::string& msg) -> void {
    std::cout << "Recieved a message!" << msg << std::endl;
    return;
  };

  // Start publishing
  spin(publish);
  spin(subscribe);

  return true;
}

bool
KermitControlModule::loop_kermit()
{
  while (1) {
    __update_state__();
  }
  return true;
}

bool
KermitControlModule::quit_kermit()
{
  __quit__();
  quit();
  return true;
}

}
}
