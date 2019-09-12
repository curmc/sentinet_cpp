/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "kernel/KermitKernel.hpp"

namespace Kermit {

KermitKernel::KermitKernel(const std::string &drive_topic, const bool verbose)
    : kermit(), message() {
  set_drive_topic(drive_topic);
  kermit.verbose = verbose;
}

KermitKernel::~KermitKernel() { kermit.xbee->close(); }

void KermitKernel::set_drive_topic(const std::string &topic) {
  message.drive_topic = std::string(topic);
}

void KermitKernel::set_serial(const std::string &port, const int &baud) {
  kermit.xbee = std::make_unique<SerialPort>(port, baud);
  kermit.xbee->open();
}

void KermitKernel::print_state() {
  std::cout << "Linear = " << kermit.drive.lin;
  std::cout << " Angular = " << kermit.drive.ang << std::endl;
}

bool KermitKernel::loop(const std::chrono::microseconds serial_period) {
  initialize_control_client("tcp://localhost:5555");
  for (int i = 0; i < 20; i++) {
    if (kermit.verbose)
      print_state();
    send_data();
    usleep(serial_period.count());
  }
  return true;
}

bool KermitKernel::send_data() {
  // This is two copies, want to decrease to 1
  // 1. Copy into kermit.buffer
  // 2. Copy into serial.buffer
  if (!kermit.xbee)
    return false;
  kermit.buffer[0] = 'd';
  kermit.buffer[1] = kermit.drive.lin;
  kermit.buffer[2] = kermit.drive.ang;
  kermit.buffer[3] = 0; // I origionally had gun buffer, that'd be here
  kermit.buffer[4] = 0;
  return kermit.xbee->write(kermit.buffer); // string?
}

void KermitKernel::recieve_drive_message(const std::string &drive_message) {
  message.drive_train_message_handler->parse_from_string(drive_message);
  kermit.drive.lin = message.drive_train_message_handler->get_linear();
  kermit.drive.ang = message.drive_train_message_handler->get_angular();
}

bool KermitKernel::initialize_control_client(const std::string &address) {
  auto fp = std::bind(&KermitKernel::recieve_drive_message, this,
                      std::placeholders::_1);
  subscribe(address, message.drive_topic, fp);
  return true;
}
} // namespace Kermit
