/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : serial
 * @created     : Wednesday Nov 27, 2019 21:17:31 MST
 */

#ifndef SERIAL_C_H

#define SERIAL_C_H

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/*
Teensy 3.6 Specifications Notes:
  12 Mbit / Sec
*/
// Sending a command velocity packet
#define CMD_VEL_PACKET 0x1
#define LINEAR_DATA 0x1
#define ANGULAR_DATA 0X2

// Sending a command packet (mine / dump etc)
#define COMMAND_PACKET 0x2

#define MINE 0x1
#define START_MINING 0x1
#define CLEAN_STOP_MINING 0x2
#define HARD_STOP_MINING 0x3

#define DUMP 0x2
#define START_DUMPING 0x1
#define CLEAN_STOP_DUMPING 0x2
#define HARD_STOP_DUMPING 0x3

#define CLEAN_STOP 0x3

#define HARD_STOP 0x4

/*
 * Signals the next command to be sent
 * can be cmd_vel_packet, or command_packet
 */
int
signal_teensy36_header(int fd, uint8_t header);

/*
 * Sends linear and angular values to teensy
 */
int
send_teensy36_cmd_vel(int fd, int16_t linear, int16_t angular);

/*
 * Sends a command to teensy and receives ack
 */
int
teensy36_send_command(int fd, uint8_t command);

/*
 * Recieve an ack response from the
 * teensy36
 */
int
teensy36_recieve_ack(int fd);

int
xhci_hcd_write_teensy36(int fd, const uint8_t* buffer, size_t buffer_len);

int
xhci_hcd_read_teensy36(int fd, uint8_t* destination, size_t num_bytes);

// Resets the buffer (input and output)
#define reset_buffer(fd) tcflush(fd, TCIOFLUSH)

int
teensy36_fd_is_valid(int fd);

/*
 * Initializes USB port for teensy 3.6
 * @return file descriptor
 */
int
xhci_hcd_teensy36_opt(const char* port);

#endif /* end of include guard SERIAL_H */
