/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"

namespace scpp {
namespace curmt {
KermitKernel::KermitKernel(const std::string& drive_topic,
                           const std::string& cmd_topic,
                           const std::string& data_topic,
                           const std::string& real_map_topic,
                           const bool verbose,
                           const bool debug)
  : kermit()
{
  kermit.drive_topic = drive_topic;
  kermit.cmd_topic = cmd_topic;
  kermit.data_topic = data_topic;
  kermit.real_map_topic = real_map_topic;
  kermit.verbose = verbose;
  kermit.debug = debug;

  message.cvel_buffer = create_cmd_vel();

  // Temporary
  message.data_buffer_temp = create_cmd_vel();
}

KermitKernel::~KermitKernel() {}

bool
KermitKernel::init_comms(const std::string& drive_addr,
                         const std::string& cmd_addr,
                         const std::string& data_addr,
                         const std::string& real_map_addr)
{
  kermit.drive_addr = drive_addr;
  kermit.cmd_addr = cmd_addr;
  kermit.data_addr = data_addr;
  kermit.real_map_addr = real_map_addr;

  params.cmd_vel_p.socket_backend = drive_addr;
  params.cmd_vel_p.topic = kermit.drive_topic;
  params.cmd_vel_p.callback =
    std::bind(&KermitKernel::drive_message_subscribe_callback,
              this,
              std::placeholders::_1);

  params.real_map_p.broker_frontend = real_map_addr;
  params.real_map_p.topic = kermit.real_map_topic;
  params.real_map_p.period = std::chrono::seconds(1); // TODO
  params.real_map_p.get_data =
    std::bind(&KermitKernel::map_message_get_data, this);

  params.data_p.broker_frontend = data_addr;
  params.data_p.topic = kermit.data_topic;
  params.data_p.period = std::chrono::seconds(1);
  params.data_p.get_data =
    std::bind(&KermitKernel::data_message_get_data, this);

  params.command_p.address = cmd_addr;
  params.command_p.callback =
    std::bind(&KermitKernel::cmd_message_callback, this, std::placeholders::_1);
  return true;
}

void
KermitKernel::print_state()
{
  std::cout << "Linear = " << message.cvel_buffer.lin;
  std::cout << " Angular = " << message.cvel_buffer.ang << std::endl;
}

bool
KermitKernel::initialize_control_client()
{
  spin(params.cmd_vel_p);
  spin(params.real_map_p);
  spin(params.data_p);
  spin(params.command_p);
  return true;
}

bool
KermitKernel::start(const std::chrono::microseconds serial_period)
{
  initialize_control_client();
  for (int i = 0; i < 10; ++i) {
    // TODO should remove these if statements
    if (kermit.verbose) {
      print_state();
    }
    if (!kermit.debug) {
      send_data();
    }
    usleep(serial_period.count());
  }
  return true;
}

bool
KermitKernel::send_data()
{
  // TODO
  return true;
}

void
KermitKernel::drive_message_subscribe_callback(std::string& message_)
{
  cmd_vel_from_wire(&message.cvel_buffer, message_.c_str());
  LOG_INFO("cmd_vel recieved %s", message_.c_str());
  return; // TODO
}

std::string
KermitKernel::cmd_message_callback(std::string& message_)
{
  return "Not implimented " + message_;
}

std::string
KermitKernel::map_message_get_data(void)
{
  return "Not implimented ";
}

std::string
KermitKernel::data_message_get_data(void)
{
  return "Not implimented ";
}

} // namespace curmt
} // namespace scpp
