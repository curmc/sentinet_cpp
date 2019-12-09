#include "scpp/kernel/serial.h"

static uint8_t input_buffer[256];
static uint8_t output_buffer[256];

int
signal_teensy36_header(int fd, uint8_t header)
{
  if (header == CMD_VEL_PACKET || header == COMMAND_PACKET) {
    output_buffer[0] = header;
    return xhci_hcd_write_teensy36(fd, output_buffer, 1);
  }
  printf("Error, invalid header\n");
  return -1;
}

static int
send_teensy36_linear(int fd, int16_t linear)
{
  output_buffer[0] = LINEAR_DATA;
  xhci_hcd_write_teensy36(fd, output_buffer, 1);
  int16_t* head = (int16_t*)output_buffer;
  *head = linear;
  return xhci_hcd_write_teensy36(fd, output_buffer, 2);
}

static int
send_teensy36_angular(int fd, int16_t angular)
{
  output_buffer[0] = ANGULAR_DATA;
  xhci_hcd_write_teensy36(fd, output_buffer, 1);
  int16_t* head = (int16_t*)output_buffer;
  *head = angular;
  return xhci_hcd_write_teensy36(fd, output_buffer, 2);
}

int
send_teensy36_cmd_vel(int fd, int16_t linear, int16_t angular)
{
  if (signal_teensy36_header(fd, CMD_VEL_PACKET) != -1) {
    send_teensy36_linear(fd, linear);
    send_teensy36_angular(fd, angular);
    return read(fd, input_buffer, 1);
    /** return teensy36_recieve_ack(fd); */
  }
  return -1;
}

int
teensy36_recieve_ack(int fd)
{
  printf("Reading\n");
  int ret = xhci_hcd_read_teensy36(fd, input_buffer, 1);
  printf("Reading\n");
  return input_buffer[1];
}

int
teensy36_send_command(int fd, uint8_t command)
{
  if (signal_teensy36_header(fd, COMMAND_PACKET) != -1) {
    output_buffer[0] = command;
    xhci_hcd_write_teensy36(fd, output_buffer, 1);
    return teensy36_recieve_ack(fd);
  }
  return -1;
}

int
xhci_hcd_write_teensy36(int fd, const uint8_t* buffer, size_t buffer_len)
{
  printf("here\n");
  int ret = write(fd, (void*)buffer, buffer_len);
  tcdrain(fd);
  return ret;
}

int
xhci_hcd_read_teensy36(int fd, uint8_t* destination, size_t num_bytes)
{
  return read(fd, (void*)destination, num_bytes);
}

int
teensy36_fd_is_valid(int fd)
{
  return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

int
xhci_hcd_teensy36_opt(const char* port)
{
  int fd;

  if (access(port, F_OK) == -1) {
    fprintf(stderr, "Cannot access port: %d : %s\n", errno, strerror(errno));
    return -1;
  }

  fd = open(port, O_RDWR | O_NOCTTY);
  if (fd == -1) {
    fprintf(stderr, "Cannot open port: %d : %s\n", errno, strerror(errno));
    return -1;
  }

  if (!isatty(fd)) {
    close(fd);
    fprintf(
      stderr, "Error, port is not a tty: %d : %s\n", errno, strerror(errno));
  }

  fcntl(fd, F_SETFL, 0);

  struct termios teensy_opt;
  if (tcgetattr(fd, &teensy_opt) < 0) {
    close(fd);
    fprintf(
      stderr, "init serial port error: %d : %s\n", errno, strerror(errno));
    return -1;
  }

  // USB speed is always 12 Mbits / s
  speed_t brate = B9600;
  if (cfsetispeed(&teensy_opt, brate) < 0 ||
      cfsetospeed(&teensy_opt, brate) < 0) {
    close(fd);
    fprintf(stderr, "Speed error: %d : %s\n", errno, strerror(errno));
    return -1;
  }

  teensy_opt.c_cflag = teensy_opt.c_cflag & ~PARENB & ~CSTOPB & ~CSIZE |
                       CS8 & ~CRTSCTS | CREAD | CLOCAL;
  teensy_opt.c_iflag = teensy_opt.c_iflag & ~IXON & ~IXOFF & ~IXANY & ICANON &
                       ECHO & ~ECHOE & ~ISIG;
  teensy_opt.c_oflag = teensy_opt.c_oflag & ~OPOST;

  teensy_opt.c_cc[VMIN] = 0;
  teensy_opt.c_cc[VTIME] = 20;

  if (tcsetattr(fd, TCSANOW, &teensy_opt) < 0) {
    /** if(tcsetattr(fd, TCSAFLUSH, &teensy_opt) < 0) { */
    close(fd);
    fprintf(
      stderr, "Setting attributes failed: %d : %s\n", errno, strerror(errno));
    return -1;
  }

  if (ioctl(fd, TIOCEXCL, NULL) < 0) {
    close(fd);
    fprintf(stderr,
            "Cannot grant exclusive access: %d : %s\n",
            errno,
            strerror(errno));
    return -1;
  }

  return fd;
}
