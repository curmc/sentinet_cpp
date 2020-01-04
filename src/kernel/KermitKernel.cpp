/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"

static std::mutex guard;

namespace scpp {
namespace curmt {

KermitKernel::KermitKernel(bool verbose, bool debug)
  : KermitNetworkInterface(verbose, debug)
{
  // Initialize messages
  message.cvel_buffer = create_cmd_vel();
  message.teensy_buffer = create_teensy_sensor_array();

  // Set the robot to not receving data at first
  receiving_cvels = false;

  memset(&states, 0, sizeof(states));

  update_teensy_message(0.0, 0.0);

  // Some default attributes
  set_alive_time(std::chrono::seconds(0));
  set_serial_attributes("/dev/ttyACM0", std::chrono::milliseconds(100));
  register_ping_func(STATE_REQUEST,
                     std::bind(&scpp::curmt::KermitKernel::robot_ping_handler,
                               this,
                               std::placeholders::_1));
}

void
KermitKernel::drive_message_subscribe_callback(std::string& message_)
{

  // Interpret the incomming message from the string
  cmd_vel_from_wire(&message.cvel_buffer,
                    reinterpret_cast<const uint8_t*>(message_.c_str()));

  // Extract the information from the recieved message
  float linear = message.cvel_buffer.lin;
  float angular = message.cvel_buffer.ang;

  // If we aren't accepting cmd_vels
  // drop the message
  if (!receiving_cvels) {
    update_teensy_message(0, 0);
    return;
  }

  // Otherwise, recieve the message and send it to the teensy
  update_teensy_message(linear, angular);
  return;
}

std::string
KermitKernel::localizer_cmd_get_data(void)
{
  float* v = &message.teensy_buffer.v;
  float* w = &message.teensy_buffer.w;
  // TODO - rn, teensy msg doesnt use w_err
  float w_err = 0.0;
  float* dt = &message.teensy_buffer.dt;

  // Update teensy outgoing message
  get_teensy_measured_vals(v, w, &w_err, dt);

  // update teensy message and put it on the wire
  teensy_sensor_array_to_wire(&message.teensy_buffer);

  char const* handle =
    reinterpret_cast<char const*>(message.teensy_buffer.msg.buff.data);

  size_t size = message.teensy_buffer.msg.buff.byte_length;

  return std::string(handle, size);
}

bool
KermitKernel::start(int context)
{
  init_teensy_peripheral(s_port);
  return kermit_start(s_period, time_alive);
}

bool
KermitKernel::set_serial_attributes(
  const std::string port,
  const std::chrono::microseconds serial_period)
{
  s_port = port;
  s_period = serial_period;
  return true;
}

bool
KermitKernel::set_alive_time(const std::chrono::seconds timer)
{
  time_alive = timer;
  return true;
}

ping
KermitKernel::robot_ping_handler(ping p)
{
  ping ret = { .type = ACK, .code = 0x00, .excess = 0x00 };

  switch (p.code) {
    case (STOP_EVERYTHING):
      stop_everything_handler(p.excess, ret);
      break;
    case (DUMP):
      dump_handler(p.excess, ret);
      break;
    case (MINE):
      mine_handler(p.excess, ret);
      break;
    case (MOVE_TO_DUMP):
      move_to_dump_handler(p.excess, ret);
      break;
    case (MOVE_TO_MINE):
      move_to_mine_handler(p.excess, ret);
      break;
    case (INIT_STATE):
      init_handler(p.excess, ret);
      break;
    case (CLEAN_EXIT):
      clean_exit_handler(p.excess, ret);
      break;
  }
  return ret;
}

int
KermitKernel::stop_everything_handler(uint64_t excess, ping& p)
{
  receiving_cvels = false;
  update_teensy_message(0, 0);
  return excess;
}

int
KermitKernel::dump_handler(uint64_t excess, ping& p)
{
  if (excess & ENTERING_STATE) {
    receiving_cvels = false;
    LOG_INFO("Entering Dump State");
  }
  if (excess & START_DUMPING) {
    receiving_cvels = false;
  }
  if (excess & CLEAN_END_DUMPING) {
    receiving_cvels = false;
  }
  if (excess & ABORT_DUMPING) {
    receiving_cvels = false;
  }
  if (excess & EXITING_STATE) {
    receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::mine_handler(uint64_t excess, ping& p)
{
  if (excess & ENTERING_STATE) {
    receiving_cvels = false;
  }
  if (excess & START_MINING) {
    receiving_cvels = false;
  }
  if (excess & CLEAN_END_MINING) {
    receiving_cvels = false;
  }
  if (excess & ABORT_MINING) {
    receiving_cvels = false;
  }
  if (excess & EXITING_STATE) {
    receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::move_to_mine_handler(uint64_t excess, ping& p)
{
  if (excess & ENTERING_STATE) {
    receiving_cvels = false;
  }
  if (excess & START_DRIVING) {
    receiving_cvels = true;
  }
  if (excess & STOP_DRIVING) {
    receiving_cvels = false;
  }
  if (excess & EXITING_STATE) {
    receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::move_to_dump_handler(uint64_t excess, ping& p)
{
  if (excess & ENTERING_STATE) {
    receiving_cvels = false;
  }
  if (excess & START_DRIVING) {
    receiving_cvels = true;
  }
  if (excess & STOP_DRIVING) {
    receiving_cvels = false;
  }
  if (excess & EXITING_STATE) {
    receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::init_handler(uint64_t excess, ping& p)
{
  if (excess & ENTERING_STATE) {
    receiving_cvels = false;
  }
  if (excess & SYS_ON) {
    receiving_cvels = false;
  }
  if (excess & ZERO_ALL) {
    receiving_cvels = false;
  }
  if (excess & DEPLOY_CAMERA) {
    receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::clean_exit_handler(uint64_t excess, ping& p)
{
  if (excess & ENTERING_STATE) {
    receiving_cvels = false;
  }
  if (excess & RETRACT_ALL) {
    receiving_cvels = true;
  }
  if (excess & SYS_OFF) {
    receiving_cvels = false;
  }
  return excess;
}

} // namespace curmt
} // namespace scpp
