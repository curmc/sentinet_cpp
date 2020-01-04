/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : serial
 * @created     : Wednesday Nov 27, 2019 21:17:31 MST
 */

#ifndef SERIAL_C_H

#define SERIAL_C_H

#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define delay(val) usleep(1000 * val)
#define DRIVE_COM 'd'
#define DEFAULT_BAUD 9600
#define BUFFER_SIZE 30

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    float lin;
    float ang;
  } cmd_vel_message;

  typedef struct
  {
    float v;
    float w;
    float w_err;
    float dt;
  } state_resp;

  typedef struct
  {
    int fd;
    cmd_vel_message msg;
    state_resp resp_msg;
    uint8_t buffer[BUFFER_SIZE];
  } teensy_device;

  /*
   * Serial
   * communication
   * functions
   */
  int serialport_init(const char* serialport, int baud);
  int serialport_close(int fd);
  int serialport_writebyte(int fd, uint8_t b);
  int serialport_write_len(int fd, const uint8_t* str);
  int serialport_write(int fd, const uint8_t* str, size_t bytes);
  int serialport_read(int fd, uint8_t* buf, size_t bytes, int timeout);

  int serialport_read_until(int fd,
                            uint8_t* buf,
                            uint8_t until,
                            size_t buf_max,
                            int timeout);

  int serialport_flush(int fd);

  int new_teensy_device(teensy_device* device, const char* serialport);

  int teensy_cleanup(teensy_device* device);

  int send_teensy_status(teensy_device* device, int lin, int ang);

  size_t send_drive(teensy_device* device, float lin_accel, float ang_accel);

  size_t read_teensy_response(teensy_device* device,
                              float* omega,
                              float* velocity,
                              float* w_err,
                              float* dt);

  int stop_robot(teensy_device* device);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard SERIAL_H */
