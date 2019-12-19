/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"
#include <mutex>

static std::mutex guard;

namespace scpp {
namespace curmt {
KermitKernel::KermitKernel(const std::string& drive_topic,
                           const std::string& cmd_topic,
                           const std::string& data_topic,
                           const std::string& real_map_topic,
                           const bool verbose,
                           const bool debug)
  : kermit()
{

  // Initialize topics
  kermit.drive_topic = drive_topic;
  kermit.cmd_topic = cmd_topic;
  kermit.data_topic = data_topic;
  kermit.real_map_topic = real_map_topic;

  // Verbosity and debug mode
  kermit.verbose = verbose;
  kermit.debug = debug;

  // Create a new buffer
  message.cvel_buffer = create_cmd_vel();
  message.cvel_buffer.lin = 0.0;
  message.cvel_buffer.ang = 0.0;
  message.cvel_buffer.lin = 0.0;
  message.cvel_buffer.ang = 0.0;

  // Create a new data buffer
  message.data_buffer_temp = create_cmd_vel();

  // Create a new ping buffer
  message.ping = create_buffer_ping();

  // Set the robot to not receving data at first
  message.receiving_cvels = false;

  running = true;
}

KermitKernel::~KermitKernel()
{
  teensy_cleanup(&teensy.dev);
}

bool
KermitKernel::kermit_quit()
{
  // NO MUTEX LOCK
  // Causes a lock because both this
  // and read is locked
  running = false;
  if (async_sender) {
    async_sender->join();
    teensy_cleanup(&teensy.dev);
  }
  return quit();
}

bool
KermitKernel::init_comms(const std::string& drive_addr,
                         const std::string& cmd_addr,
                         const std::string& data_addr,
                         const std::string& real_map_addr)
{
  // Create a new set of params

  // Set the addresses of proxies
  kermit.drive_addr = drive_addr;
  kermit.cmd_addr = cmd_addr;
  kermit.data_addr = data_addr;
  kermit.real_map_addr = real_map_addr;

  // The command velocity params for listening (a subscriber)
  params.cmd_vel_p.socket_backend = drive_addr;
  params.cmd_vel_p.topic = kermit.drive_topic;
  params.cmd_vel_p.callback =
    std::bind(&KermitKernel::drive_message_subscribe_callback,
              this,
              std::placeholders::_1);

  // The real time map params (a publisher)
  params.real_map_p.broker_frontend = real_map_addr;
  params.real_map_p.topic = kermit.real_map_topic;
  params.real_map_p.period = std::chrono::seconds(1); // TODO
  params.real_map_p.get_data =
    std::bind(&KermitKernel::map_message_get_data, this);

  // The data sender (a publisher)
  params.data_p.broker_frontend = data_addr;
  params.data_p.topic = kermit.data_topic;
  params.data_p.period = std::chrono::seconds(1);
  params.data_p.get_data =
    std::bind(&KermitKernel::data_message_get_data, this);

  // The command relay (a server)
  params.command_p.address = cmd_addr;
  params.command_p.callback =
    std::bind(&KermitKernel::cmd_message_callback, this, std::placeholders::_1);

  return true;
}

bool
KermitKernel::init_teensy_peripheral(const std::string& port)
{
  new_teensy_device(&teensy.dev, port.c_str());

  if (!kermit.debug) {
    async_sender = std::make_unique<std::thread>([this](void) -> bool {
      while (running) {
        send_drive(&teensy.dev, teensy.lin, teensy.ang);
        delay(100);
      }
      return true;
    });
  }

  return true;
}

void
KermitKernel::print_state()
{
  std::lock_guard<std::mutex> lock(guard);
  std::cout << "Linear = " << teensy.lin;
  std::cout << " Angular = " << teensy.ang << std::endl;
}

bool
KermitKernel::initialize_control_client()
{
  spin(params.cmd_vel_p);
  spin(params.real_map_p);
  spin(params.data_p);
  spin(params.command_p);
  return true;
}

bool
KermitKernel::start(const std::chrono::microseconds serial_period,
                    const std::chrono::seconds time_alive)
{
  using namespace std::chrono;

  std::atomic<bool> infinite(false);

  // If you pass 0 seconds / micro / milli,
  // it causes an infinite loop
  // hacky, but works
  if (time_alive == seconds(0)) {
    infinite = true;
  }

  initialize_control_client();
  steady_clock::time_point time_now = steady_clock::now();

  int i = 0;

  while (running && (infinite || steady_clock::now() - time_now < time_alive)) {
    if (!(i % 100) && kermit.verbose) {
      print_state();
    }
    usleep(serial_period.count());
    i++;
  }
  return true;
}

void
KermitKernel::drive_message_subscribe_callback(std::string& message_)
{
  std::lock_guard<std::mutex> lock(guard);

  // TODO I have a choice here, rn, everytime we get a message,
  // we deserialize it. Other option is to drop the message if receiving_cvels
  // is false need to think about it though

  // Interpret the incomming message from the string
  cmd_vel_from_wire(&message.cvel_buffer,
                    reinterpret_cast<const uint8_t*>(message_.c_str()));

  if (message.receiving_cvels) {
    teensy.lin = (int16_t)message.cvel_buffer.lin;
    teensy.ang = (int16_t)message.cvel_buffer.ang;
  } else {
    teensy.lin = 0;
    teensy.ang = 0;
  }
}

std::string
KermitKernel::cmd_message_callback(std::string& message_)
{
  std::lock_guard<std::mutex> lock(guard);
  std::cout << "Recieved ping\n";
  // Interpret the incomming message
  if (!serialize_from_ping(&message.ping,
                           reinterpret_cast<BYTE*>(&message_[0]))) {
    std::cout << "valid ping\n";
    std::cout << message.ping.type << " " << message.ping.code << " "
              << message.ping.excess << std::endl;

    ping_buffer resp =
      ping_handler(message.ping.type, message.ping.code, message.ping.excess);

    to_wire_ping(&resp);
    return std::string(reinterpret_cast<char const*>(resp.data),
                       PING_HEADER_SIZE);
  }

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

std::string
KermitKernel::data_message_get_data(void)
{
  // message.cvel_buffer.lin = 5.6;
  // message.cvel_buffer.ang = 10.5;
  cmd_vel_to_wire(&message.cvel_buffer);
  // print_message_formatted(message.cvel_buffer.buff.data);
  return std::string((char const*)(message.cvel_buffer.buff.data), 27);
}

ping_buffer
KermitKernel::ping_handler(uint8_t type, uint16_t code, uint64_t excess)
{
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
  teensy.lin = 0;
  teensy.ang = 0;
  return excess;
}

int
KermitKernel::dump_handler(uint64_t excess)
{
  if (excess & ENTERING_DUMP_STATE) {
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
  if (excess & EXITING_DUMP_STATE) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::mine_handler(uint64_t excess)
{
  if (excess & ENTERING_MINE_STATE) {
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
  if (excess & EXITING_MINE_STATE) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::move_to_mine_handler(uint64_t excess)
{
  if (excess & ENTERING_MOVE_TO_MINE) {
    message.receiving_cvels = false;
  }
  if (excess & START_DRIVING) {
    message.receiving_cvels = true;
  }
  if (excess & STOP_DRIVING) {
    message.receiving_cvels = false;
  }
  if (excess & EXITING_MOVE_TO_MINE) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::move_to_dump_handler(uint64_t excess)
{
  if (excess & ENTERING_MOVE_TO_DUMP) {
    message.receiving_cvels = false;
  }
  if (excess & START_DRIVING) {
    message.receiving_cvels = true;
  }
  if (excess & STOP_DRIVING) {
    message.receiving_cvels = false;
  }
  if (excess & EXITING_MOVE_TO_MINE) {
    message.receiving_cvels = false;
  }
  return excess;
}

int
KermitKernel::init_handler(uint64_t excess)
{
  if (excess & ENTERING_INIT_STATE) {
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
  if (excess & ENTERING_CLEAN_EXIT) {
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
