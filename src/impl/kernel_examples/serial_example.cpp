/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : serial_example
 * @created     : Friday Aug 30, 2019 14:06:53 MDT
 */

#include "kernel/SerialPort.hpp"

int main() {
  auto a = std::make_unique<SerialPort>("/dev/ttyUSB0", 9600);
  a->open();
  a->write("Hello");
  a->close();

  // Write to stdout
  /**
   * Note that this isn't going to appear as pretty as it usually does
   * (i.e. with echo or other commands). This is because we're setting the
   * settings specifically for serial port writing, not stdout. Thus, there's
   * going to be weird gaps between everything, however we revert back to
   * the origional settings once the close call is called
   */
  auto b = std::make_unique<SerialPort>("/dev/stdout", 9600);
  b->open();
  b->write("Hello\n");
  b->close();

  b->open();
  b->write("NOOOOO\n");
  b->close();

  return 0;
}
