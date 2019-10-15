/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitControlModule
 * @created     : Saturday Oct 12, 2019 23:23:15 MDT
 */

#ifndef KERMITCONTROLMODULE_HPP

#define KERMITCONTROLMODULE_HPP

// Local includes
#include "control/ZMQControlClient.hpp"
#include "kernel/Defaults.hpp"
extern "C"
{
#include "data_message.h"
}

// C++ includes
#include <chrono>
#include <memory>

#define KERMIT_STATE uint8_t

// STATES
#define MINING (1 << 0)
#define DUMPING (1 << 1)

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
  KermitControlModule()
    : publish_channel(::scpp::curmt::defaults::cmd_vel_front_end)
  {}

  /**
   * @brief Overrides publish_channel proxy name
   *
   * @param publish_channel_ The address to publish on
   */
  KermitControlModule(const std::string& publish_channel_);

  /**
   * @brief Destructor
   *
   * Needs to stop the module if not stopped already
   */
  virtual ~KermitControlModule();

  /**
   * @brief Defaults start_kermit on the cmd_vel topic
   *
   * TODO Need to start up cmd channel
   *
   * @return Status
   */
  inline bool start_kermit()
  {
    return start_kermit(::scpp::curmt::defaults::cmd_vel_topic);
  }

  /**
   * @brief Start Kermit on the specified topic
   *
   * @param topic The name of the topic to publish cmd vel onto
   *
   * This function calls __start__ so that implimentation start occurs
   *
   * @return Status
   */
  bool start_kermit(const std::string& topic);

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

private:
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
  virtual inline float __get_lin__() { return 0; }
  virtual inline float __get_ang__() { return 0; }

  /**
   * @brief An 8 or more bit field that gets the robot
   * state ore'd to express state
   *
   * state |= MINING; // to set
   * state & MINING; // to get
   *
   * @return A state
   */
  virtual inline KERMIT_STATE __get_state__() { return 0; }

  // The channel to publish on (tcp://localhost:5555 default)
  std::string publish_channel;

  // The STACK implimented buffer to serialize data into
  data_buffer buff;

  // The STACK implimented buffer to serialize control protocols onto
  // ping_buffer ping_buff;
};

}
}
#endif /* end of include guard KERMITCONTROLMODULE_HPP */
