/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitControlModule
 * @created     : Saturday Oct 12, 2019 23:23:15 MDT
 */

#ifndef KERMITCONTROLMODULE_HPP

#define KERMITCONTROLMODULE_HPP

// Local includes
#include "scpp/control/ZMQControlClient.hpp"
#include "scpp/kernel/Defaults.hpp"

extern "C"
{
#include "scpp/data_message.h"
#include "scpp/rmt_messages.h"
#include "scpp/ping_message.h"
}

// C++ includes
#include <chrono>
#include <memory>

#define KERMIT_STATE uint8_t

// STATES
#define MINING (1 << 0)
#define DUMPING (1 << 1)
#define DRIVING (1 << 2)

namespace scpp {
namespace curmt {

class KermitControlModule : public ::scpp::net::ZMQControlClient
{
public:
  /**
   * @brief Default constructor
   *
   * Uses default cmd_vel_front_end for the actual project
   */
  KermitControlModule();

  /**
   * @brief Destructor
   *
   * Needs to stop the module if not stopped already
   */
  virtual ~KermitControlModule();

  /**
   * @brief Start Kermit on the specified topic
   *
   * @param topic The name of the topic to publish cmd vel onto
   *
   * This function calls __start__ so that implimentation start occurs
   *
   * @return Status
   */
  bool start_kermit();

  /**
   * @brief The main loop function
   *
   * TODO is it better for loop to be asynchronous?
   *
   * This function calls __update_state__ so that implimentation loop occurs
   *
   * @return Status
   */
  bool loop_kermit();

  /**
   * @brief Stop kermit main
   *
   * This function calls __quit__ so that implimentation quit occurs
   *
   * @return
   */
  bool quit_kermit();

protected:
  /**
   * @brief Implimentation start
   *
   * @return status
   */
  virtual int __start__() { return 0; }

  /**
   * @brief Implimentation quit
   *
   * override this if a control module needs to quit
   *
   * @return status
   */
  virtual int __quit__() { return 0; }

  /**
   * @brief Update robot state implimentation
   *
   * @return status
   */
  virtual int __update_state__() { return 0; }

  /**
   * @brief Implimentation getters
   */
  virtual float __get_lin__() const = 0;
  virtual float __get_ang__() const = 0;

  /**
   * @brief An 8 or more bit field that gets the robot
   * state ore'd to express state
   *
   * state |= MINING; // to set
   * state & MINING; // to get
   *
   * @return A state
   */
  virtual inline KERMIT_STATE __get_state__() const  { return 0; }

  // The channel to publish on (tcp://localhost:5555 default)
  typedef struct {
    std::string cmd_addr;
    std::string data_addr;
    std::string cmd_vel_addr;
    std::string real_time_addr;

    std::string cmd_id;
    std::string data_topic;
    std::string cmd_vel_topic;
    std::string real_time_topic;
  } KermitEndpoints;

  // The channel to publish on (tcp://localhost:5555 default)
  typedef struct {
    cmd_vel cvel_buff;
    cmd_vel data_buff;
    ping_buffer command;
  } KermitMessage;

  KermitEndpoints endpoints;
  KermitMessage message;

  std::atomic<bool> running;
};

}
}
#endif /* end of include guard KERMITCONTROLMODULE_HPP */
