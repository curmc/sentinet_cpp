/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : serial_test
 * @created     : Wednesday Nov 27, 2019 23:20:00 MST
 */

#include "scpp/kernel/serial.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

teensy_device dev;

FILE* fd_lin;
FILE* fd_ang;

volatile int running;

void
signalHandler(int sig)
{
  running = 0;
}

static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL){
      printf("No input\n");
        return NO_INPUT;
    }


    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}


int
main()
{

  running = 1;
  
  fd_lin = fopen("./linear.dat", "w");
  fd_ang = fopen("./angular.dat", "w");

  signal(SIGINT, signalHandler);

  int i = new_teensy_device(&dev, "/dev/ttyACM0");
  printf("%d\n", i);

  puts("Sending data now");

  uint8_t buffer[12];
  uint8_t buffer2[12];
  /** uint8_t buffer3[12]; */
  serialport_flush(dev.fd);

  char ch[10];
  
  int count = 0;
  int a = 0;

  while(running) {
    send_drive(&dev, 0, 0);
    delay(100);
  
    serialport_read_until(dev.fd, buffer, 'l', 9, 5);
    memset(buffer, 0, 12);
    a = serialport_read_until(dev.fd, buffer, 'k', 9, 5);
    buffer[a - 2] = '\0';

    serialport_read_until(dev.fd, buffer2, 'a', 9, 5);
    memset(buffer2, 0, 12);
    a = serialport_read_until(dev.fd, buffer2, 'b', 9, 5);
    buffer2[a - 2] = '\0';

    /** serialport_read_until(dev.fd, buffer3, 'c', 9, 5); */
    /** memset(buffer3, 0, 12); */
    /** a = serialport_read_until(dev.fd, buffer3, 'd', 9, 5); */
    /** buffer3[a - 2] = '\0'; */

    /** printf("%s\n", buffer); */
    printf("Moving: %s\n", buffer2);
    /** printf("Desired: %s\n", buffer3); */

    fprintf(fd_ang, "%d %s\n", count, buffer2);
    fprintf(fd_lin, "%d %s\n", count, buffer);
    count++;
  }
  
  fclose(fd_ang);
  fclose(fd_lin);
  teensy_cleanup(&dev);
  return 1;
}
