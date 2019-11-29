/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : serial_test
 * @created     : Wednesday Nov 27, 2019 23:20:00 MST
 */

#include "scpp/kernel/serial.h"

int main() {
  int fd = xhci_hcd_teensy36_opt("/dev/ttyACM0");
  
  char buff[2];
  write(fd, "hi\0", 3);
  read(fd, buff, 2);

  /** int ack = (int)send_teensy36_cmd_vel(fd, 5, 5); */
  printf("%s\n", buff);
  if(fd != -1) {  
    close(fd);
  }


  return 1;
}
