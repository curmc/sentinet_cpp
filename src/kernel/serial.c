#include "scpp/kernel/serial.h"

#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <memory.h> // strcpy and memcpy

int
serialport_init(const char* serialport, int baud)
{
  struct termios toptions;
  int fd;

  // fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
  fd = open(serialport, O_RDWR | O_NONBLOCK);

  if (fd == -1) {
    perror("serialport_init: Unable to open port ");
    return -1;
  }

  // int iflags = TIOCM_DTR;
  // ioctl(fd, TIOCMBIS, &iflags);     // turn on DTR
  // ioctl(fd, TIOCMBIC, &iflags);    // turn off DTR

  if (tcgetattr(fd, &toptions) < 0) {
    perror("serialport_init: Couldn't get term attributes");
    return -1;
  }

  speed_t brate = baud; // let you override switch below if needed
  switch (baud) {
    case 4800:
      brate = B4800;
      break;
    case 9600:
      brate = B9600;
      break;
#ifdef B14400
    case 14400:
      brate = B14400;
      break;
#endif
    case 19200:
      brate = B19200;
      break;
#ifdef B28800
    case 28800:
      brate = B28800;
      break;
#endif
    case 38400:
      brate = B38400;
      break;
    case 57600:
      brate = B57600;
      break;
    case 115200:
      brate = B115200;
      break;
  }
  cfsetispeed(&toptions, brate);
  cfsetospeed(&toptions, brate);

  // 8N1
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  // no flow control
  toptions.c_cflag &= ~CRTSCTS;

  // toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset

  toptions.c_cflag |= CREAD | CLOCAL; // turn on READ & ignore ctrl lines
  toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

  toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
  toptions.c_oflag &= ~OPOST;                          // make raw

  // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
  toptions.c_cc[VMIN] = 0;
  toptions.c_cc[VTIME] = 0;
  // toptions.c_cc[VTIME] = 20;

  tcsetattr(fd, TCSANOW, &toptions);
  if (tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
    perror("init_serialport: Couldn't set term attributes");
    return -1;
  }

  return fd;
}

//
int
serialport_close(int fd)
{
  return close(fd);
}

//
int
serialport_writebyte(int fd, uint8_t b)
{
  int n = write(fd, &b, 1);
  if (n != 1)
    return -1;
  return 0;
}

//
int
serialport_write_len(int fd, const uint8_t* str)
{
  int len = strlen((char*)str);
  int n = write(fd, str, len);
  if (n != len) {
    perror("serialport_write: couldn't write whole string\n");
    return -1;
  }
  return 0;
}

//
int
serialport_write(int fd, const uint8_t* str, size_t bytes)
{
  int n = write(fd, str, bytes);
  if (n != bytes) {
    perror("serialport_write: couldn't write whole string\n");
    return -1;
  }
  tcdrain(fd);
  return 0;
}

int
serialport_read(int fd, uint8_t* buf, size_t bytes, int timeout)
{
  char b[1]; // read expects an array, so we give it a 1-byte array
  int i = 0;
  for (size_t i = 0U; i < bytes && timeout > 0; ++i) {
    int n = read(fd, b, 1);
    if (n == -1)
      return -1; // couldn't read
    if (n == 0) {
      usleep(1 * 1000); // wait 1 msec try again
      timeout--;
      if (timeout == 0)
        return -2;
      continue;
    }
#ifdef SERIALPORTDEBUG
    printf("serialport_read_until: i=%d, n=%d b='%c'\n", i, n, b[0]); // debug
#endif
    buf[i] = b[0];
  }
  return 0;
}

//
int
serialport_read_until(int fd,
                      uint8_t* buf,
                      uint8_t until,
                      size_t buf_max,
                      int timeout)
{
  char b[1]; // read expects an array, so we give it a 1-byte array
  int i = 0;
  do {
    int n = read(fd, b, 1); // read a char at a time
    if (n == -1)
      return -1; // couldn't read
    if (n == 0) {
      usleep(1 * 1000); // wait 1 msec try again
      timeout--;
      if (timeout == 0)
        return -2;
      continue;
    }
#ifdef SERIALPORTDEBUG
    printf("serialport_read_until: i=%d, n=%d b='%c'\n", i, n, b[0]); // debug
#endif
    buf[i] = b[0];
    i++;
  } while (b[0] != until && i < buf_max && timeout > 0);

  buf[i] = 0; // null terminate the string
  return 0;
}

//
int
serialport_flush(int fd)
{
  sleep(2); // required to make flush work, for some reason
  return tcflush(fd, TCIOFLUSH);
}

int
new_teensy_device(teensy_device* device, const char* serialport)
{
  device->fd = 0;
  device->msg.lin = 0;
  device->msg.ang = 0;
  memset(device->buffer, 0, BUFFER_SIZE);

  device->fd = serialport_init(serialport, 9600);
  return device->fd;
}

static int
teensy_send_data(teensy_device* device)
{
  device->buffer[0] = 'd';

  int* temp = (int*)&device->buffer[1];
  *temp++ = device->msg.lin;
  *temp++ = device->msg.ang;

  uint8_t* next = (uint8_t*)temp;
  *next++ = '\n';
  *next++ = '\0';

  size_t size = next - device->buffer;

  printf("Sending: ");
  for (int i = 0; i < size; ++i) {
    printf("%x ", device->buffer[i]);
  }
  printf("\n");

  serialport_write(device->fd, device->buffer, size);
  return size;
}

size_t
send_drive(teensy_device* device, int lin, int ang)
{
  device->msg.lin = lin;
  device->msg.ang = ang;
  return teensy_send_data(device);
}

size_t
send_float_drive(teensy_device* device, float lin, float ang)
{
  device->msg.lin = (int)lin;
  device->msg.ang = (int)ang;
  return teensy_send_data(device);
}

int
stop_robot(teensy_device* device)
{
  device->msg.lin = 0;
  device->msg.ang = 0;
  return teensy_send_data(device);
}

int
teensy_cleanup(teensy_device* device)
{
  return serialport_close(device->fd);
}
