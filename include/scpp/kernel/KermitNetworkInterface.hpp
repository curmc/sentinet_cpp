/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitNetworkInterface
 * @created     : Saturday Aug 24, 2019 16:27:47 MDT
 */

#ifndef KERMITNETWORKINTERFACE_HPP

#define KERMITNETWORKINTERFACE_HPP

// Local includes
#include "scpp/kernel/SerialPort.hpp"
#include "scpp/control/ZMQControlClient.hpp"
extern "C"
{
#include "scpp/messages/rmt_messages.h"
#include "scpp/messages/ping_message.h"
#include "scpp/kernel/serial.h"
}

// C++ includes
#include <memory>
#include <chrono>

namespace scpp {
namespace curmt {

const static std::string drive_topic = "cmd_vel";
const static std::string cmd_topic = "command";
const static std::string real_map_topic = "realtime";

const static std::string drive_addr = "tcp://localhost:5571";
const static std::string cmd_addr = "tcp://localhost:5580";
const static std::string real_map_addr = "tcp://localhost:5581";

/**
 * @brief Kermit Kernel is the low level kernel that is
 * driving the robot
 */
class KermitNetworkInterface : public ::scpp::net::ZMQControlClient
{
public:
  /*
   * No default constructor
   */
  KermitNetworkInterface(bool verbose, bool debug);

  // Default constructor
  virtual ~KermitNetworkInterface();

  bool init_comms();

  /**
   * @brief Starts listening on the topics and executes start sequences
   *
   * @param ethernet_period The period to publish ethernet data
   *
   * @return Status
   */
  bool kermit_start(const std::chrono::microseconds serial_period,
                    const std::chrono::seconds timer);

  bool kermit_quit();

  bool init_teensy_peripheral(const std::string& port);

protected:
  bool initialize_control_client();
  // Prints robot state
  void print_state();

  void update_teensy_message(int16_t lin, int16_t ang);

protected:
  // CALLBACKS
  // The subscription to cmd_vel topic
  virtual void drive_message_subscribe_callback(std::string& message) = 0;

  // The subscription to the command channel
  virtual std::string cmd_message_callback(std::string& message) = 0;

  // The get_data to the real time pipe channel
  virtual std::string map_message_get_data(void) = 0;

  // Ping callbacks
protected:
  // Handles lower utility pings and returns a new constructed response ping
  // Responds with ACK if ping is a state request
  virtual ping_buffer ping_handler(uint8_t type, uint16_t code, uint64_t excess) = 0;

protected:
  // bool send_data();

  struct 
  {
    // TODO will need more data here
    bool verbose;
    bool debug;

    // The topics / ids for each proxy
    std::string drive_topic;
    std::string cmd_topic;
    std::string real_map_topic;

    // The addresses of each proxy
    std::string drive_addr;
    std::string cmd_addr;
    std::string real_map_addr;
  } kermit;

private:
  struct 
  {
    scpp::subscribe_params cmd_vel_p;
    scpp::publish_params real_map_p;
    scpp::serve_params command_p;
  } params;

  struct 
  {
    teensy_device dev;
    int16_t lin;
    int16_t ang;
  } teensy;

  std::atomic<bool> running;
  std::unique_ptr<std::thread> async_sender;
};

} // namespace curmt
} // namespace scpp
#endif /* end of include guard KERMITNETWORKINTERFACE_HPP */
