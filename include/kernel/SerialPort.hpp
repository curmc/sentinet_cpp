/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SerialPort
 * @created     : Saturday Aug 24, 2019 16:29:35 MDT
 */

#ifndef SERIALPORT_HPP

#define SERIALPORT_HPP

// Local Includes
#include "core/kernel/InputOutputInterface.hpp"

// C++ includes
#include <vector>
#include <thread>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define DEFAULT_BYTES 255

static volatile sig_atomic_t signalStatus = 0;

class SerialPort : public InputOutputInterface {
public:
  SerialPort() = delete;
  SerialPort(const std::string &port, const int &baud,
             const int &bytes = DEFAULT_BYTES);

  virtual ~SerialPort();

  bool set_port(const std::string &port);
  bool set_baud(const int &baud);

  bool write(const std::string &message);
  bool write(const std::string &message, int bytes);
  std::string read(int &num_bytes);

  bool open();
  bool close();
  bool reconnect(int max_retries);

  // combines the top two
  bool scan(int max_times);

private:
  static bool write_impl(const int &fd, const char *message,
                         const int &num_bytes);
  static bool read_impl(const int &fd, char *buffer, const int &num_bytes);

  static void signal_handler(int sig_num);
  inline void check_signal();

  enum state_t { CLOSING, CLOSED, SCANNING, OPEN };

  typedef struct SerialProperties {
    SerialProperties() = delete;
    SerialProperties(const int &num_bytes) {
      buffer = new char[num_bytes + 1];
      bytes = num_bytes;
    }

    state_t state;
    int fd;
    std::string port;
    speed_t baud;
    int bytes;
    char *buffer;
    termios toptions;
    termios old_options;
  } SerialProperties;

  const std::vector<int> close_signals;
  SerialProperties properties;
};

#endif /* end of include guard SERIALPORT_HPP */
