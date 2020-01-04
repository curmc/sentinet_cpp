/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Saturday Aug 24, 2019 16:27:47 MDT
 */

#ifndef KERMITKERNEL_HPP

#define KERMITKERNEL_HPP

// Local includes
#include "scpp/kernel/KermitNetworkInterface.hpp"

extern "C"
{
#include "scpp/messages/rmt_messages.h"
#include "scpp/messages/ping_message.h"
}

namespace scpp {
namespace curmt {

/**
 * @brief Kermit Kernel is the low level kernel that is
 * driving the robot
 */
class KermitKernel : private ::scpp::curmt::KermitNetworkInterface
{
public:
  /*
   * No default constructor
   */
  KermitKernel(bool verbose, bool debug);
  virtual ~KermitKernel() { kermit_quit(); }

public:
  // Set the serial attributes (port and period)
  bool set_serial_attributes(const std::string port,
                             const std::chrono::microseconds serial_period);

  // Set the amount of alive time
  bool set_alive_time(const std::chrono::seconds timer);

  // start
  bool start(int context = 0) override;

protected:
  // CALLBACKS
  void drive_message_subscribe_callback(std::string& message) override;
  std::string localizer_cmd_get_data(void) override;

  // Ping callbacks
private:
  // Registered for type STATE_REQUEST
  ping robot_ping_handler(ping);

  // Ping handlers
  int stop_everything_handler(uint64_t excess, ping& resp);
  int dump_handler(uint64_t excess, ping& resp);
  int mine_handler(uint64_t excess, ping& resp);
  int move_to_mine_handler(uint64_t excess, ping& resp);
  int move_to_dump_handler(uint64_t excess, ping& resp);
  int init_handler(uint64_t excess, ping& resp);
  int clean_exit_handler(uint64_t excess, ping& resp);

private:
  // bool send_data();
  struct
  {
    cmd_vel cvel_buffer;
    teensy_sensor_array teensy_buffer;

  } message;

  struct
  {
    uint8_t hard_stop : 1;
    uint8_t dump : 1;
    uint8_t mine : 1;
    uint8_t move_to_mine : 1;
    uint8_t move_to_dump : 1;
    uint8_t init : 1;
    uint8_t clean_exit : 1;
  } states;

  std::chrono::seconds time_alive;
  std::string s_port;
  std::chrono::microseconds s_period;
  std::atomic<bool> receiving_cvels;
};

} // namespace curmt
} // namespace scpp

#endif /* end of include guard KERMITKERNEL_HPP */
