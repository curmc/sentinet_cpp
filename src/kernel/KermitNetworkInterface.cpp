/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitNetworkInterface
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "scpp/kernel/KermitNetworkInterface.hpp"

static std::mutex guard;

using namespace addr::kernel;

namespace scpp {
namespace curmt {
KermitNetworkInterface::KermitNetworkInterface(bool verbose, bool debug)
  : RMTControlClient(to_bind_addr(ADDRESS))
{
  kermit = { .verbose = verbose, .debug = debug };
  running = true;
}

KermitNetworkInterface::~KermitNetworkInterface()
{
  teensy_cleanup(&teensy.dev);
}

bool
KermitNetworkInterface::kermit_quit()
{
  // NO Mutex lock here
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
  scpp::subscribe_params cmd_vel;
  scpp::publish_params localizer;

  // The command velocity params for listening (a subscriber)
  cmd_vel.sock_addr = to_conn_addr(addr::kernel::CMD_VEL);
  cmd_vel.topic = addr::cmd_vel_topic;
  cmd_vel.callback =
    std::bind(&KermitNetworkInterface::drive_message_subscribe_callback,
              this,
              std::placeholders::_1);

  localizer.sock_addr = to_conn_addr(addr::kernel::LOCALIZER);
  std::cout << localizer.sock_addr << std::endl;
  std::cout << addr::kernel::LOCALIZER << std::endl;
  localizer.period = std::chrono::seconds(1);
  localizer.topic = addr::localizer_topic;
  localizer.get_data =
    std::bind(&KermitNetworkInterface::localizer_cmd_get_data, this);

  spin(cmd_vel);
  spin(localizer);
  LOG_INFO("Kernel: (Localizer) (cmd_vel) (%s) (%s)",
           localizer.sock_addr.c_str(),
           cmd_vel.sock_addr.c_str());
  return true;
}

bool
KermitNetworkInterface::init_teensy_peripheral(const std::string& port)
{

  if (!kermit.debug) {
    new_teensy_device(&teensy.dev, port.c_str());
    async_sender = std::make_unique<std::thread>([this](void) -> bool {
      while (running) {
        // Need mutexes on these babies
        send_drive(&teensy.dev, teensy.lin, teensy.ang);
        read_teensy_response(
          &teensy.dev, &teensy.w, &teensy.v, &teensy.w_err, &teensy.dt);
        printf("%f %f %f %f\n", teensy.w, teensy.v, teensy.w_err, teensy.dt);
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
KermitNetworkInterface::kermit_start(
  const std::chrono::microseconds serial_period,
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

  if (!init_comms())
    return false;

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
KermitNetworkInterface::update_teensy_message(float lin, float ang)
{
  std::lock_guard<std::mutex> lock(guard);
  teensy.lin = lin;
  teensy.ang = ang;
}

void
KermitNetworkInterface::get_teensy_measured_vals(float* v,
                                                 float* w,
                                                 float* w_err,
                                                 float* dt)
{
  std::lock_guard<std::mutex> lock(guard);
  *v = teensy.v;
  *w = teensy.w;
  *w_err = teensy.w_err;
  *dt = teensy.dt;
}

}
}
