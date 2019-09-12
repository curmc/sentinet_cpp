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
#include "messages/common/CmdVel.hpp"

// C++ includes
#include <memory>
#include <chrono>

namespace Kermit {

constexpr int local_port = 5555;

class KermitKernel : public ZMQControlClient {
public:
  KermitKernel() = delete;
  KermitKernel(const std::string &drive_topic, const bool verbose = true);

  virtual ~KermitKernel();

  void set_drive_topic(const std::string &topic);

  void set_serial(const std::string &port, const int &baud);

  bool loop(const std::chrono::microseconds serial_period);

private:
  bool initialize_control_client(
      const std::string &address = "tcp://localhost:5555");

  void recieve_drive_message(const std::string &drive_message);

  void print_state();
  bool send_data();

  typedef struct KermitOutputs {

    std::unique_ptr<SerialPort> xbee;
    typedef struct {
      int lin;
      int ang;
    } drivetrain_t;
    drivetrain_t drive;

    KermitOutputs() {
      xbee = nullptr;
      drive = {0, 0};
    }
    bool verbose;

    char buffer[6];

  } KermitOutputs;

  typedef struct KermitMessageProperties {
    KermitMessageProperties() {
      gun_message_handler = std::make_unique<Gun>();
      drive_train_message_handler = std::make_unique<DriveTrain>();
    }
    std::unique_ptr<Gun> gun_message_handler;
    std::unique_ptr<DriveTrain> drive_train_message_handler;

    std::string buffer;

    std::string drive_topic;
    std::string gun_topic;
  } KermitMessageProperties;

  KermitOutputs kermit;
  KermitMessageProperties message;
};
} // namespace Kermit
#endif /* end of include guard KERMITKERNEL_HPP */
