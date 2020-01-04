/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : RMTControlClient
 * @created     : Friday Jan 03, 2020 18:42:54 MST
 */

#include "scpp/control/RMTControlClient.hpp"

namespace scpp {
namespace curmt {

RMTControlClient::RMTControlClient(std::string address)
  : server_address(address)
  , server_id(address)
{
  buffers.incomming = create_buffer_ping();
  buffers.outgoing = create_buffer_ping();
  buffers.request = create_buffer_ping();
  buffers.reply = create_buffer_ping();

  ping_callbacks =
    std::make_unique<std::unordered_map<int, std::function<ping(ping)>>>();

  time(&telemetry.start_time);

  initialize_client();

  bind_server(server_address,
              std::bind(&scpp::curmt::RMTControlClient::ping_server_callback,
                        this,
                        std::placeholders::_1));
}

ping
RMTControlClient::ping_endpoint(std::string address, const ping request)
{

  // Copy data into request
  buffers.request.type = request.type;
  buffers.request.code = request.code;
  buffers.request.excess = request.excess;
  to_wire_ping(&buffers.request);

  // Form a string message
  char const* msg = reinterpret_cast<char const*>(&buffers.request.data[0]);
  std::string message(msg, PING_HEADER_SIZE);

  // Get the response from the server
  std::string response = this->request(address, message);
  BYTE* resp_msg = reinterpret_cast<BYTE*>(&response[0]);

  // If valid, return the response ping
  if (!serialize_from_ping(&buffers.reply, resp_msg)) {

    ping ret = (ping){ .type = buffers.reply.type,
                       .code = buffers.reply.code,
                       .excess = buffers.reply.excess };

    return ret;
  }

  // Invalid
  LOG_ERROR("Recieved invalid reply from server %s", address.c_str());
  return (ping){ 0, 0, 0 };
}

ping
RMTControlClient::process_control_command_default(const ping& ping_)
{

  ping ret = (ping){ .type = ACK, .code = 0x00, .excess = 0x00 };

  return ret;
}

std::string
RMTControlClient::ping_server_callback(std::string& message)
{
  int stat;

  BYTE* msg = reinterpret_cast<BYTE*>(&message[0]);

  if (!(stat = serialize_from_ping(&buffers.incomming, msg))) {
    ping resp = (ping){ .type = buffers.incomming.type,
                        .code = buffers.incomming.code,
                        .excess = buffers.incomming.excess };

    auto found = ping_callbacks->find((int)resp.type);
    ping ret;

    if (found != ping_callbacks->end())
      ret = found->second(resp);
    else
      ret = process_control_command_default(resp);

    // Create a new ping_message
    buffers.outgoing.type = ret.type;
    buffers.outgoing.code = ret.code;
    buffers.outgoing.excess = ret.excess;

  } else {

    buffers.outgoing.type = INVALID_REQUEST;
    buffers.outgoing.code = 0x00;
    buffers.outgoing.excess = 0x00;
  }

  to_wire_ping(&buffers.outgoing);

  // Return the data in the ping
  char const* ret_ping = reinterpret_cast<char const*>(buffers.outgoing.data);
  return std::string(ret_ping, PING_HEADER_SIZE);
}

static std::mutex lock;
bool
RMTControlClient::register_ping_func(int type,
                                     std::function<ping(ping)> callback)
{

  std::lock_guard<std::mutex> m_lock(lock);
  auto found = ping_callbacks->find(type);

  if (found == ping_callbacks->end()) {
    ping_callbacks->insert({ type, callback });
    return true;
  }
  return false;
}

}
}
