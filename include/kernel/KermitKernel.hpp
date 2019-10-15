/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Saturday Aug 24, 2019 16:27:47 MDT
 */

#ifndef KERMITKERNEL_HPP

#define KERMITKERNEL_HPP

// Local includes
#include "kernel/SerialPort.hpp"
#include "control/ZMQControlClient.hpp"

// C++ includes
#include <memory>
#include <chrono>

namespace scpp {
namespace curmt {

class KermitKernel : public ::scpp::net::ZMQControlClient
{
public:
  KermitKernel() = delete;

  KermitKernel(const std::string& drive_topic,
               const std::string& publish_channel,
               const std::string& serve_channel,
               const bool verbose = true);

  virtual ~KermitKernel();

  void set_serial(const std::string& port, const int& baud);

  bool start(const std::chrono::microseconds serial_period);

  void print_state();

  // Control Client stuff
private:
  bool initialize_control_client();

  // The subscription to cmd_vel topic
  void drive_message_subscribe_callback(std::string& message);

  // The main server callback
  std::string command_channel_server_callback(std::string& message);

private:
  bool send_data();

  typedef struct KermitOutputs
  {
    std::unique_ptr<SerialPort> xbee;
    typedef struct
    {
      int lin;
      int ang;
    } drivetrain_t;
    drivetrain_t drive;

    KermitOutputs()
    {
      xbee = nullptr;
      drive = { 0, 0 };
    }
    bool verbose;

    std::string drive_topic;
    std::string publish_channel;
    std::string serve_channel;
    char* buffer;
  } KermitOutputs;

  KermitOutputs kermit;
};
} // namespace curmt
} // namespace scpp
#endif /* end of include guard KERMITKERNEL_HPP */
