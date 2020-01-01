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

// C++ includes
#include <memory>
#include <chrono>

namespace scpp {
namespace curmt {

/**
 * @brief Kermit Kernel is the low level kernel that is
 * driving the robot
 */
class KermitKernel : public ::scpp::curmt::KermitNetworkInterface
{
public:
  /*
   * No default constructor
   */
  KermitKernel(bool verbose, bool debug);
  virtual ~KermitKernel() = default;

protected:
  // CALLBACKS
  // The subscription to cmd_vel topic
  void drive_message_subscribe_callback(std::string& message) override;

  // The subscription to the command channel
  std::string cmd_message_callback(std::string& message) override;

  // The get_data to the real time pipe channel
  std::string map_message_get_data(void) override;


  // Ping callbacks
private:
  // Handles lower utility pings and returns a new constructed response ping
  // Responds with ACK if ping is a state request
  ping_buffer ping_handler(uint8_t type, uint16_t code, uint64_t excess) override;

  ping_buffer message_request_handler(uint16_t code, uint64_t excess);

  // Handles ping message with type 8 (state request ping)
  int robot_ping_handler(uint16_t code, uint64_t excess);

  // Handlers for each state
  int stop_everything_handler(uint64_t excess);
  int dump_handler(uint64_t excess);
  int mine_handler(uint64_t excess);
  int move_to_mine_handler(uint64_t excess);
  int move_to_dump_handler(uint64_t excess);
  int init_handler(uint64_t excess);
  int clean_exit_handler(uint64_t excess);

private:
  // bool send_data();
  struct 
  {
    std::atomic<bool> receiving_cvels;
    cmd_vel cvel_buffer;
    ping_buffer ping;

  } message;

  struct 
  {
    uint8_t current_state;

    uint8_t hard_stop;
    uint8_t dump;
    uint8_t mine;
    uint8_t move_to_mine;
    uint8_t move_to_dump;
    uint8_t init;
    uint8_t clean_exit;
  } states;
};

} // namespace curmt
} // namespace scpp
#endif /* end of include guard KERMITKERNEL_HPP */
