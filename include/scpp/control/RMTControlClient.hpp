/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : RMTControlClient
 * @brief       : A brief description of the file
 *
 * A detailed description of the file
 *
 * @created     : Friday Jan 03, 2020 18:35:38 MST
 * @license     : MIT
 */

#ifndef RMTCONTROLCLIENT_HPP

#define RMTCONTROLCLIENT_HPP

#include "scpp/control/ZMQControlClient.hpp"

extern "C"
{
#include "scpp/messages/ping_message.h"
}

namespace scpp {
namespace curmt {

typedef struct
{
  uint8_t type;
  uint16_t code;
  uint64_t excess;
} ping;

class RMTControlClient : public ::scpp::net::ZMQControlClient
{
  // Constructor Destructor
public:
  RMTControlClient(std::string address);

  virtual ~RMTControlClient() = default;

  // Ping a server or node with a request
  ping ping_endpoint(std::string address, const ping request);

protected:
  // Default action taken to pings
  // not in the ping map
  virtual ping process_control_command_default(const ping& ping_);

  // Register a ping function
  bool register_ping_func(int type, std::function<ping(ping)>);

  // Networking
private:
  std::string ping_server_callback(std::string& message);

  // Bound to address
  const std::string server_address;

  // Server id for debugging (default to server_address)
  const std::string server_id;

  // Vector of ping callbacks
  std::unique_ptr<std::unordered_map<int, std::function<ping(ping)>>>
    ping_callbacks;

  struct
  {
    time_t start_time;
    uint64_t msgs_recieved;
  } telemetry;

  struct
  {
    ping_buffer incomming;
    ping_buffer outgoing;
    ping_buffer request;
    ping_buffer reply;
  } buffers;
};

}
}
#endif /* end of include guard RMTCONTROLCLIENT_HPP */
