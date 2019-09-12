/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SerialPort
 * @created     : Saturday Aug 24, 2019 16:53:00 MDT
 */

#include "kernel/SerialPort.hpp"
#include <iostream>

SerialPort::SerialPort(const std::string &port, const int &baud,
                       const int &bytes)
    : close_signals({SIGQUIT, SIGKILL, SIGINT}), properties(bytes) {

  // Setters
  if (!set_port(port))
    properties.port = "";
  if (!set_baud(baud))
    properties.baud = 0;

  // Open signal handler
  for (auto i : close_signals)
    signal(i, SerialPort::signal_handler);
}

SerialPort::~SerialPort() {
  if (properties.buffer)
    delete[] properties.buffer;
  close(); // close handles the fact that we might have closed already
}

bool SerialPort::set_port(const std::string &port) {
  properties.port = std::string(port);
  return access(properties.port.c_str(), F_OK) != -1;
}

bool SerialPort::set_baud(const int &baud) {
  switch (baud) {
  case 4800:
    properties.baud = B4800;
    return true;
  case 9600:
    properties.baud = B9600;
    return true;
  case 19200:
    properties.baud = B19200;
    return true;
  case 38400:
    properties.baud = B38400;
    return true;
  case 57600:
    properties.baud = B57600;
    return true;
  case 115200:
    properties.baud = B115200;
    return true;
  }
  properties.baud = 0;
  return false;
}

bool SerialPort::write(const std::string &message) {
  if (!write_impl(properties.fd, message.c_str(), sizeof(message.c_str())))
    return false;
  return true;
}

bool SerialPort::write(const std::string &message, int bytes) {
  if (!write_impl(properties.fd, message.c_str(), bytes))
    return false;
  return true;
}

std::string SerialPort::read(int &num_bytes) {

  /**
   * Putting this here because ideally, a programmer is going to account
   * for buffer size. This is the optimal solution and all the automated
   * junk below shouldn't be executed unless num_bytes is greater than
   * properties.bytes
   */
  if (num_bytes < properties.bytes) {
    if (!read_impl(properties.fd, properties.buffer, num_bytes))
      return "ERROR";
    properties.buffer[num_bytes] = '\0';
    return std::string(properties.buffer);
  }

  const auto repetitions = properties.bytes % num_bytes + 1;
  std::string ret;

  // write to buffer, attach to ret. Ideally, repetitions should be 1
  // A programmer can make this happen intentionally
  for (auto i = 0; num_bytes >= 0 && i < repetitions; ++i) {
    // the number of bytes we need to read is less, so we return on the next
    if (num_bytes <= properties.bytes) {
      if (!read_impl(properties.fd, properties.buffer, num_bytes))
        return "ERROR";
      properties.buffer[num_bytes] = '\0';
      ret += std::string(properties.buffer);
      return ret;
      // The number of bytes we need to read is greater than available space
    } else {
      // Read and place data into buffer, but only bytes amount
      if (!read_impl(properties.fd, properties.buffer, properties.bytes))
        return "ERROR";
      properties.buffer[properties.bytes] = '\0';
      ret += std::string(properties.buffer);
      num_bytes -= properties.bytes;
    }
  }
  return "ERROR";
}

bool SerialPort::open() {

  // I know this is redundant especially if we reconnect, but this isn't a
  // streaming process
  if (access(properties.port.c_str(), F_OK) == -1)
    return false;
  properties.fd = ::open(properties.port.c_str(), O_RDWR | O_NOCTTY);

  if (properties.fd == -1)
    return false;

  if (tcgetattr(properties.fd, &properties.toptions) < 0)
    return false;

  if (properties.baud == 0)
    return false;

  // Make a copy to revert to origional
  properties.old_options = properties.toptions;

  cfsetispeed(&properties.toptions, properties.baud);
  cfsetospeed(&properties.toptions, properties.baud);

  properties.toptions.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
  properties.toptions.c_cflag |= CS8 | CREAD | CLOCAL;
  properties.toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
  properties.toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  properties.toptions.c_oflag &= ~OPOST;

  properties.toptions.c_cc[VMIN] = 0;
  properties.toptions.c_cc[VTIME] = 20;

  if (tcsetattr(properties.fd, TCSANOW, &properties.toptions) < 0)
    return false;
  properties.state = OPEN;

  check_signal();

  return true;
}

bool SerialPort::close() {
  // if already close
  if (properties.state == CLOSED)
    return true;

  // A small gap here, but necessary
  properties.state = CLOSING;

  // Set the options back to their old options
  if (tcsetattr(properties.fd, TCSANOW, &properties.old_options) < 0)
    std::cout << "Error returning origional options" << std::endl;

  if (::close(properties.fd) < 0)
    return false;

  // fd should be cleanly closed
  properties.state = CLOSED;
  return true;
}

bool SerialPort::scan(int max_times) {
  properties.state = SCANNING;
  for (int i = 0; i < max_times; i++) {

    // First, handle any signals
    check_signal();

    // Check if port is an available file
    if (access(properties.port.c_str(), F_OK) != -1)
      return open();

    // I don't want to be spamming scan a lot.
    usleep(20000);
  }
  return false;
}

bool SerialPort::reconnect(int max_retries) {
  close();
  return scan(max_retries);
}

bool SerialPort::write_impl(const int &fd, const char *message,
                            const int &num_bytes) {
  return ::write(fd, message, num_bytes) != -1;
}

bool SerialPort::read_impl(const int &fd, char *buffer, const int &num_bytes) {
  return ::write(fd, buffer, num_bytes) != -1;
}

void SerialPort::signal_handler(int sig_num) { signalStatus = sig_num; }

inline void SerialPort::check_signal() {
  if (signalStatus != 0) {
    if (properties.state == SCANNING || properties.state == OPEN)
      close();
    exit(signalStatus);
  }
  // do nothing
}
