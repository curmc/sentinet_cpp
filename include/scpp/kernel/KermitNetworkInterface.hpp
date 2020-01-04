/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitNetworkInterface
 * @created     : Saturday Aug 24, 2019 16:27:47 MDT
 */

#ifndef KERMITNETWORKINTERFACE_HPP

#define KERMITNETWORKINTERFACE_HPP

// Local includes
#include "scpp/control/RMTControlClient.hpp"

extern "C"
{
#include "scpp/messages/ping_message.h"
#include "scpp/kernel/serial.h"
}

namespace scpp {
namespace curmt {

class KermitNetworkInterface : protected ::scpp::curmt::RMTControlClient
{
public:
  /*
   * Default constructor already binds
   * to sock addr
   */
  KermitNetworkInterface(bool verbose, bool debug);

  // Default constructor
  virtual ~KermitNetworkInterface();

protected:
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
  void print_state();

  void update_teensy_message(float lin, float ang);
  void get_teensy_measured_vals(float* v, float* w, float* w_err, float* dt);

protected:
  // The subscription to cmd_vel topic
  virtual void drive_message_subscribe_callback(std::string& message) = 0;

  // The publisher to the localizer
  virtual std::string localizer_cmd_get_data(void) = 0;

protected:
  // bool send_data();

  struct
  {
    bool verbose;
    bool debug;
  } kermit;

private:
  bool init_comms();

  struct
  {
    teensy_device dev;
    float lin;
    float ang;
    float v;
    float w;
    float w_err;
    float dt;
  } teensy;

  std::atomic<bool> running;
  std::unique_ptr<std::thread> async_sender;
};

} // namespace curmt
} // namespace scpp
#endif /* end of include guard KERMITNETWORKINTERFACE_HPP */
