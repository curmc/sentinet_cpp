/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitNetworkInterface
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "scpp/kernel/KermitNetworkInterface.hpp"
#include <mutex>

static std::mutex guard;

namespace scpp {
namespace curmt {
KermitNetworkInterface::KermitNetworkInterface(bool verbose, bool debug)
  : kermit()
{

  // Initialize topics
  kermit.drive_topic = drive_topic;
  kermit.cmd_topic = cmd_topic;
  kermit.real_map_topic = real_map_topic;

  // Verbosity and debug mode
  kermit.verbose = verbose;
  kermit.debug = debug;

  running = true;
}

KermitNetworkInterface::~KermitNetworkInterface()
{
  teensy_cleanup(&teensy.dev);
}

bool
KermitNetworkInterface::kermit_quit()
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
KermitNetworkInterface::init_comms()
{
  // Create a new set of params

  // Set the addresses of proxies
  kermit.drive_addr = drive_addr;
  kermit.cmd_addr = cmd_addr;
  kermit.real_map_addr = real_map_addr;

  // The command velocity params for listening (a subscriber)
  params.cmd_vel_p.socket_backend = drive_addr;
  params.cmd_vel_p.topic = kermit.drive_topic;
  params.cmd_vel_p.callback =
    std::bind(&KermitNetworkInterface::drive_message_subscribe_callback,
              this,
              std::placeholders::_1);

  // The real time map params (a publisher)
  params.real_map_p.broker_frontend = real_map_addr;
  params.real_map_p.topic = kermit.real_map_topic;
  params.real_map_p.period = std::chrono::seconds(1); // TODO
  params.real_map_p.get_data =
    std::bind(&KermitNetworkInterface::map_message_get_data, this);

  // The command relay (a server)
  params.command_p.address = cmd_addr;
  params.command_p.callback =
    std::bind(&KermitNetworkInterface::cmd_message_callback, this, std::placeholders::_1);

  return true;
}

bool
KermitNetworkInterface::init_teensy_peripheral(const std::string& port)
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
KermitNetworkInterface::print_state()
{
  std::lock_guard<std::mutex> lock(guard);
  std::cout << "Linear = " << teensy.lin;
  std::cout << " Angular = " << teensy.ang << std::endl;
}

bool
KermitNetworkInterface::initialize_control_client()
{
  spin(params.cmd_vel_p);
  spin(params.real_map_p);
  spin(params.command_p);
  return true;
}

bool
KermitNetworkInterface::kermit_start(const std::chrono::microseconds serial_period,
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


void KermitNetworkInterface::update_teensy_message(int16_t lin, int16_t ang) {
  std::lock_guard<std::mutex> lock(guard);
  teensy.lin = lin;
  teensy.ang = ang;
}

}
}
