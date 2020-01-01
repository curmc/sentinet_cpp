/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"
#include <mutex>
#include <cmath>

static std::mutex guard;

namespace scpp {
namespace curmt {

static const float Kp = 1;
static const float Ki = 0;
static const float Kd = 0;

static float integral;
static float der;

static void convert_to_teensy(float desired_lin, float angular_magnitude, int16_t* lin, int16_t* ang) {

  // TODO -- minature pid controller
  *ang = static_cast<int16_t>(angular_magnitude);
  if(fabs(angular_magnitude) < 10.0)
    *lin = static_cast<int16_t>(desired_lin);
  else
    *lin = 0;
}


KermitKernel::KermitKernel(bool verbose, bool debug)
  : KermitNetworkInterface(verbose, debug)
{
  // Create a new buffer
  message.cvel_buffer = create_cmd_vel();
  message.cvel_buffer.lin = 0.0;
  message.cvel_buffer.ang = 0.0;
  message.cvel_buffer.lin = 0.0;
  message.cvel_buffer.ang = 0.0;
  update_teensy_message(0.0, 0.0);


  // Create a new ping buffer
  message.ping = create_buffer_ping();

  // Set the robot to not receving data at first
  message.receiving_cvels = false;
}

void
KermitKernel::drive_message_subscribe_callback(std::string& message_)
{
  // Interpret the incomming message from the string
  cmd_vel_from_wire(&message.cvel_buffer,
                    reinterpret_cast<const uint8_t*>(message_.c_str()));

  int16_t lin;
  int16_t ang;

  convert_to_teensy(message.cvel_buffer.lin, message.cvel_buffer.ang, &lin, &ang);

  if (message.receiving_cvels)
    update_teensy_message(lin, ang);
  else 
    update_teensy_message(0, 0);
}

std::string
KermitKernel::cmd_message_callback(std::string& message_)
{
  int stat;

  // Checks validity of the message
  if (!(stat = serialize_from_ping(&message.ping,
                           reinterpret_cast<BYTE*>(&message_[0])))) {

    LOG_INFO("Recieved Ping (type) (code) (excess) (%d %d %ld)", message.ping.type, message.ping.code, message.ping.excess);

    ping_buffer resp =
      ping_handler(message.ping.type, message.ping.code, message.ping.excess);

    to_wire_ping(&resp);
    return std::string(reinterpret_cast<char const*>(resp.data),
                       PING_HEADER_SIZE);
  }

  LOG_WARN("Recieved invalid ping, returned (%d)", stat);

  // Respond with invalid request
  ping_buffer resp = create_buffer_ping();
  resp.type = INVALID_REQUEST;
  resp.excess = 0x00;
  resp.code = 0x00;

  return std::string(reinterpret_cast<char const*>(resp.data),
                     PING_HEADER_SIZE);
}

std::string
KermitKernel::map_message_get_data(void)
{
  return "Not implimented ";
}

ping_buffer
KermitKernel::ping_handler(uint8_t type, uint16_t code, uint64_t excess)
{
  // Response ping
  ping_buffer resp = create_buffer_ping();

  switch (type) {
    case (UNDEF): {
      resp.type = INVALID_REQUEST;
      resp.excess = 0x00;
      resp.code = 0x00;
      return resp;
    }
    case (ACK): {
      resp.type = ACK;
      resp.code = 0x00;
      resp.excess = 0x00;
      return resp;
    }
    case (PING_ECHO): {
      resp.type = type;
      resp.code = code;
      resp.excess = excess;
      return resp;
    }
    case (INVALID_REQUEST): {
      resp.type = INVALID_REQUEST;
      resp.code = 0x00;
      resp.excess = 0x00;
      return resp;
    }
    case (REDIRECT_MESSAGE): { // TODO ???
      resp.type = INVALID_REQUEST;
      resp.code = 0x00;
      resp.excess = 0x00;
      return resp;
    }
    case (MESSAGE_REQUEST): {
      resp = message_request_handler(code, excess);
      return resp;
    }
    case (TIME_STAMP): {
      resp.type = ACK;
      resp.code = 0x00;
      resp.excess = 0x00;
      return resp;
    }
    case (TIME_EXCEEDED): {
      resp.type = ACK;
      resp.code = 0x00;
      resp.excess = 0x00;
      return resp;
    }
    case (STATE_REQUEST): {
      resp.type = ACK;
      resp.code = 0x00;
      resp.excess = 0x00;
      robot_ping_handler(code, excess);
      return resp;
    }
  }
  return resp;
}

// TODO
ping_buffer
KermitKernel::message_request_handler(uint16_t code, uint64_t excess)
{
  ping_buffer resp = create_buffer_ping();
  resp.type = ACK;
  return resp;
}

int
KermitKernel::robot_ping_handler(uint16_t code, uint64_t excess)
{
  switch (code) {
    case (STOP_EVERYTHING): {
      return stop_everything_handler(excess);
    }
    case (DUMP): {
      return dump_handler(excess);
    }
    case (MINE): {
      return mine_handler(excess);
    }
    case (MOVE_TO_DUMP): {
      return move_to_dump_handler(excess);
    }
    case (MOVE_TO_MINE): {
      return move_to_mine_handler(excess);
    }
    case (INIT_STATE): {
      return init_handler(excess);
    }
    case (CLEAN_EXIT): {
      return clean_exit_handler(excess);
    }
  }
  return 1;
}

int
KermitKernel::stop_everything_handler(uint64_t excess)
{
  message.receiving_cvels = false;
  update_teensy_message(0, 0);
  return excess;
}

int
KermitKernel::dump_handler(uint64_t excess)
{
  if (excess & ENTERING_STATE) {
    message.receiving_cvels = false;
    if (states.current_state) {
      LOG_ERROR("In the middle of another state, cannot enter dump");
      return -1;
    }
    LOG_INFO("Entering Dump State");
  }
  if (excess & START_DUMPING) {
    message.receiving_cvels = false;
    if (states.current_state & DUMP) {
      LOG_INFO("Starting Dumping");
      return 1;
    }
    LOG_ERROR("Cannot enter dumping, not in dumping state");
  }
  if (excess & CLEAN_END_DUMPING) {
    message.receiving_cvels = false;
  }
  if (excess & ABORT_DUMPING) {
    message.receiving_cvels = false;
  }
  if (excess & EXITING_STATE) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::mine_handler(uint64_t excess)
{
  if (excess & ENTERING_STATE) {
    message.receiving_cvels = false;
  }
  if (excess & START_MINING) {
    message.receiving_cvels = false;
  }
  if (excess & CLEAN_END_MINING) {
    message.receiving_cvels = false;
  }
  if (excess & ABORT_MINING) {
    message.receiving_cvels = false;
  }
  if (excess & EXITING_STATE) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::move_to_mine_handler(uint64_t excess)
{
  if (excess & ENTERING_STATE) {
    message.receiving_cvels = false;
  }
  if (excess & START_DRIVING) {
    message.receiving_cvels = true;
  }
  if (excess & STOP_DRIVING) {
    message.receiving_cvels = false;
  }
  if (excess & EXITING_STATE) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::move_to_dump_handler(uint64_t excess)
{
  if (excess & ENTERING_STATE) {
    message.receiving_cvels = false;
  }
  if (excess & START_DRIVING) {
    message.receiving_cvels = true;
  }
  if (excess & STOP_DRIVING) {
    message.receiving_cvels = false;
  }
  if (excess & EXITING_STATE) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::init_handler(uint64_t excess)
{
  if (excess & ENTERING_STATE) {
    message.receiving_cvels = false;
  }
  if (excess & SYS_ON) {
    message.receiving_cvels = false;
  }
  if (excess & ZERO_ALL) {
    message.receiving_cvels = false;
  }
  if (excess & DEPLOY_CAMERA) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::clean_exit_handler(uint64_t excess)
{
  if (excess & ENTERING_STATE) {
    message.receiving_cvels = false;
  }
  if (excess & RETRACT_ALL) {
    message.receiving_cvels = true;
  }
  if (excess & SYS_OFF) {
    message.receiving_cvels = false;
  }
  return excess;
}

} // namespace curmt
} // namespace scpp
