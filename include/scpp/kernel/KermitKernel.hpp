/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Saturday Aug 24, 2019 16:27:47 MDT
 */

#ifndef KERMITKERNEL_HPP

#define KERMITKERNEL_HPP

// Local includes
#include "scpp/kernel/SerialPort.hpp"
#include "scpp/control/ZMQControlClient.hpp"
extern "C"
{
#include "scpp/rmt_messages.h"
}

// C++ includes
#include <memory>
#include <chrono>

namespace scpp {
namespace curmt {

/**
 * @brief Kermit Kernel is the low level kernel that is
 * driving the robot
 */
class KermitKernel : public ::scpp::net::ZMQControlClient
{
public:
  /*
   * No default constructor
   */
  KermitKernel() = delete;

  /**
   * @brief Kermit Kernel publishes / subscribes /
   * serves / requests on these four topics
   *
   * @param drive_topic The name of the drive train topic
   * @param cmd_topic The name of the command topic
   * @param data_topic The name of the data topic
   * @param real_map_topic The name of the real time map topic
   * @param verbose Prints robot status to the screen
   * @param debug In debug mode, nothing is actually sent over serial / ethernet
   */
  KermitKernel(const std::string& drive_topic,
               const std::string& cmd_topic,
               const std::string& data_topic,
               const std::string& real_map_topic,
               const bool verbose,
               const bool debug = true);

  // Default constructor
  virtual ~KermitKernel();

  /**
   * @brief Initializes communications in one swoop
   *
   * @param drive_addr The address of the drive train proxy
   * @param cmd_addr The address of the command proxy
   * @param data_addr The address of the data proxy
   * @param real_map_addr The address of the real time map proxy
   *
   * @return Status
   */
  bool init_comms(const std::string& drive_addr,
                  const std::string& cmd_addr,
                  const std::string& data_addr,
                  const std::string& real_map_addr);

  /**
   * @brief Starts listening on the topics and executes start sequences
   *
   * @param ethernet_period The period to publish ethernet data
   *
   * @return Status
   */
  bool start(const std::chrono::microseconds ethernet_period);

  // Control Client stuff
private:
  bool initialize_control_client();
  // Prints robot state
  void print_state();

private:
  // CALLBACKS
  // The subscription to cmd_vel topic
  void drive_message_subscribe_callback(std::string& message);

  // The subscription to the command channel
  std::string cmd_message_callback(std::string& message);

  // The get_data to the real time pipe channel
  std::string map_message_get_data(void);

  // The get_data to the data channel
  std::string data_message_get_data(void);

private:
  bool send_data();

  typedef struct KermitOutputs
  {
    KermitOutputs() {}

    // TODO will need more data here

    bool verbose;
    bool debug;

    // The topics / ids for each proxy
    std::string drive_topic;
    std::string cmd_topic;
    std::string data_topic;
    std::string real_map_topic;

    // The addresses of each proxy
    std::string drive_addr;
    std::string cmd_addr;
    std::string data_addr;
    std::string real_map_addr;
  } KermitOutputs;

  typedef struct CCProperties
  {
    scpp::subscribe_params cmd_vel_p;
    scpp::publish_params real_map_p;
    scpp::publish_params data_p;
    scpp::serve_params command_p;
  } CCProperties;

  typedef struct KermitMessage
  {
    cmd_vel cvel_buffer;
    cmd_vel data_buffer_temp;

  } KermitMessage;

  KermitOutputs kermit;
  KermitMessage message;
  CCProperties params;
};
} // namespace curmt
} // namespace scpp
#endif /* end of include guard KERMITKERNEL_HPP */
