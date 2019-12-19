/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : serial_test
 * @created     : Wednesday Nov 27, 2019 23:20:00 MST
 */

#include "scpp/kernel/serial.h"
#include <signal.h>
#include <stdlib.h>


teensy_device dev;

void signalHandler(int sig) {
  printf("Recieved: %d\n", sig);
  teensy_cleanup(&dev); 
  exit(0);
}

int
main()
{
  signal(SIGINT, signalHandler);

  int i = new_teensy_device(&dev, "/dev/ttyACM0");
  printf("%d\n", i);

  puts("Sending data now");

  uint8_t buffer[12];
  serialport_flush(dev.fd);
  for(int j = 0; j < 100; ++j){

    send_drive(&dev, 20, 0); 
    delay(100);
    serialport_read(dev.fd, buffer, 8, 2);

    int* a = (int*)buffer;
    int b = *a;
    a++;
    int c = *a;

    for(int i = 0; i < 8; ++i) {
      printf("%x ", buffer[i]);
    }
    printf("\n");

    printf("MSG: %d %d\n", b, c);
  }
  

  teensy_cleanup(&dev);
  return 1;
}
