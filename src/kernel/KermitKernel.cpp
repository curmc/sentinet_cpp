/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"

namespace scpp {
namespace curmt {
KermitKernel::KermitKernel(const std::string& drive_topic,
                           const std::string& publish_channel,
                           const std::string& serve_channel,
                           const bool verbose)
  : kermit()
{
  kermit.publish_channel = publish_channel;
  kermit.serve_channel = serve_channel;
  kermit.drive_topic = drive_topic;
  kermit.verbose = verbose;
  // Haven't used that new word in a while now
  kermit.buffer = new char[5];
}

KermitKernel::~KermitKernel()
{
  // kermit.xbee->close();
  delete[] kermit.buffer;
}

void
KermitKernel::set_serial(const std::string& port, const int& baud)
{
  kermit.xbee = std::make_unique<SerialPort>(port, baud);
  // kermit.xbee->open();
}

void
KermitKernel::print_state()
{
  std::cout << "Linear = " << kermit.drive.lin;
  std::cout << " Angular = " << kermit.drive.ang << std::endl;
}

bool
KermitKernel::start(const std::chrono::microseconds serial_period)
{
  initialize_control_client();
  for (int i = 0; i < 10; ++i) {
    if (kermit.verbose) {
      print_state();
    }
    // send_data();
    usleep(serial_period.count());
  }
  return true;
}

bool
KermitKernel::send_data()
{
  // This is two copies, want to decrease to 1
  // 1. Copy into kermit.buffer
  // 2. Copy into serial.buffer
  if (!kermit.xbee)
    return false;
  kermit.buffer[0] = 'd';
  kermit.buffer[1] = kermit.drive.lin;
  kermit.buffer[2] = kermit.drive.ang;
  kermit.buffer[3] = 0; // I origionally had gun buffer, that'd be here
  kermit.buffer[4] = 0;
  return kermit.xbee->write(kermit.buffer); // string?
}

void
KermitKernel::drive_message_subscribe_callback(std::string& message)
{
  LOG_INFO("I recieved %s", message.c_str());
  return; // TODO
}

std::string
KermitKernel::command_channel_server_callback(std::string& message)
{
  return "Not implemented " + message;
}

bool
KermitKernel::initialize_control_client()
{
  auto fp = std::bind(&KermitKernel::drive_message_subscribe_callback,
                      this,
                      std::placeholders::_1);

  auto server = std::bind(&KermitKernel::command_channel_server_callback,
                          this,
                          std::placeholders::_1);

  subscribe_params temp_sb;
  serve_params temp_sr;

  temp_sb.callback = fp;
  temp_sb.socket_backend = kermit.publish_channel;
  temp_sb.topic = kermit.drive_topic;

  temp_sr.callback = server;
  temp_sr.address = kermit.serve_channel;

  spin(temp_sr);
  spin(temp_sb);
  return true;
}
} // namespace curmt
} // namespace scpp
