/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ping_message_keys
 * @brief       : Defines all ping macros
 *
 * Pings utilize or'ed bits to communicate between two
 * nodes - this file defines all of these codes and type codes
 *
 * @created     : Sunday Oct 13, 2019 20:52:42 MDT
 */

#ifndef PING_MESSAGE_KEYS_H

#define PING_MESSAGE_KEYS_H

#include "type_codes.h"

#define PING_HEADER_SIZE 11

#ifndef PING_TYPES

/*
 * Undefined message type, drop and send an invalid request
 */
#define UNDEF 0

/*
 * Some cases simply stop communication, thus
 * they require just a plain ack as a nop
 */
#define ACK 1

/*
 * Simply echo the message back
 */
#define PING_ECHO 2

/*
 * Was sent an invalid request
 */
#define INVALID_REQUEST 3

#define INVALID_MESSAGE

/*
 * Send the next message recieved to the given address
 * Sends invalid request if host is not up
 */
#define REDIRECT_MESSAGE 4

/*
 * Tells client to send a given message type back
 * Usefull for a time stamp request
 */
#define MESSAGE_REQUEST 5

/*
 * Request sent is over - stop trying and drop all packets related
 * to the message recieved
 */
#define TIME_EXCEEDED 6

/*
 * Simply a time stamp
 */
#define TIME_STAMP 7

// CURMT Specialization
/*
 * A request to change state - code has the ore'd bits for what state to change
 * and excess is ore'd for each state. If there are multiple state change
 * requests, we treat each byte of excess as a seperate request for a seperate
 * state
 */
#define STATE_REQUEST 8

#define DATA_REQUEST
#define DATA_PING

#define MAX_PING_TYPE STATE_REQUEST
#endif

#ifndef RMT_STATES
/*
 * The following are all not necessarily states that would be sent, i.e.
 * they can just be notifications that we are entering a new state.
 * For example, move to mine might not do anything, just make sure auger
 * is stowed.
 *
 * One thing to note is this is going in the code, and meta information for each
 * state is going in the excess. So for example, we could or the code bit with
 * MOVE_TO_DUMP and send it, but then the robot replies saying it can't do that
 * because auger is not deployed.
 */
#define STOP_EVERYTHING 1
#define DUMP (1 << 1)
#define MINE (1 << 2)
#define MOVE_TO_DUMP (1 << 3)
#define MOVE_TO_MINE (1 << 4)
#define INIT_STATE (1 << 5)
#define CLEAN_EXIT (1 << 6)
#define MICRO_MOVEMENT (1 << 7)
#endif

// DUMP FLAGS
#define ENTERING_DUMP_STATE (1 << 0)
#define START_DUMPING (1 << 1)
#define CLEAN_END_DUMPING (1 << 2)
#define ABORT_DUMPING (1 << 3)
#define EXITING_DUMP_STATE (1 << 4)

// MINE FLAGS
#define ENTERING_MINE_STATE (1 << 0)
#define START_MINING (1 << 1)
#define CLEAN_END_MINING (1 << 2)
#define EXITING_MINE_STATE (1 << 3)

// MOVE TO MINE FLAG
#define ENTERING_MOVE_TO_MINE (1 << 0)
#define START_DRIVING (1 << 1)
#define STOP_DRIVING (1 << 2)
#define EXITING_MOVE_TO_MINE (1 << 3)

// MOVE TO DUMP FLAG
#define ENTERING_MOVE_TO_DUMP (1 << 0)
#define START_DUMPING (1 << 1)
#define CLEAN_END_DUMPING (1 << 2)
#define EXITING_MOVE_TO_DUMP (1 << 3)

// INIT STATE
#define ENTERING_INIT_STATE (1 << 0)
#define SYS_ON (1 << 1)
#define ZERO_ALL (1 << 2)
#define DEPLOY_CAMERA (1 << 3)

// CLEAN EXIT
#define ENTERING_CLEAN_EXIT (1)
#define STOP_ALL (1 << 1)
#define RETRACT_ALL (1 << 2)
#define SYS_OFF (1 << 3)

#if PROTOCOL_VERSION == 1
#define PROTOCOL_PING uint8_t
#define OPCODE_PING uint8_t
#define TYPE_PING uint8_t
#define CODE_PING uint16_t
#define CHECKSUM_PING uint16_t
#define EXCESS_PING uint64_t

#define PROTOPOS_PING 0
#define OPPOS_PING PROTOPOS_PING + sizeof(PROTOCOL_PING)
#define TYPEPOS_PING OPPOS_PING + sizeof(OPCODE_PING)
#define CODEPOS_PING TYPEPOS_PING + sizeof(TYPE_PING)
#define CHECKPOS_PING CODEPOS_PING + sizeof(CODE_PING)
#define EXCESSPOS_PING CHECKPOS_PING + sizeof(CHECKSUM_PING)

#define PING_MESSAGE_SIZE EXCESSPOS_PING + sizeof(EXCESS_PING)
#endif
#endif /* end of include guard PING_MESSAGE_KEYS_H */
