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
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <unistd.h>
#include "scpp/messages/rmt_messages.h"
#include "scpp/messages/ping_message.h"
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
  bool start(const std::chrono::microseconds ethernet_period,
             const std::chrono::seconds timer);

  // TODO - this should be derived from KCM
  bool kermit_quit();

  /**
   * @brief Initializes teensy endpoint or sets to debug if false
   *
   * @param ip_addr The ip address of the teensy
   *
   * @return Status - if false, changes debuf to true
   */
  bool init_teensy_peripheral(const std::string& ip_addr,
                              int port,
                              const std::string& interface);

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

  // Ping callbacks
private:
  // Handles lower utility pings and returns a new constructed response ping
  // Responds with ACK if ping is a state request
  ping_buffer ping_handler(uint8_t type, uint16_t code, uint64_t excess);

  ping_buffer message_request_handler(uint16_t code, uint64_t excess);

  // Handles ping message with type 8 (state request ping)
  int robot_ping_handler(uint16_t code, uint64_t excess);

  // Handlers for each state
  int stop_everything_handler(uint64_t excess);
  int dump_handler(uint64_t excess);
  int mine_handler(uint64_t excess);
  int move_to_mine_handler(uint64_t excess);
  int move_to_dump_handler(uint64_t excess);
  int init_handler(uint64_t excess);
  int clean_exit_handler(uint64_t excess);

private:
  // bool send_data();

  typedef struct KermitProperties
  {
    KermitProperties() {}

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
  } KermitProperties;

  typedef struct CCProperties
  {
    scpp::subscribe_params cmd_vel_p;
    scpp::publish_params real_map_p;
    scpp::publish_params data_p;
    scpp::serve_params command_p;
  } CCProperties;

  typedef struct KermitMessage
  {
    std::atomic<bool> receiving_cvels;
    cmd_vel cvel_buffer;
    cmd_vel data_buffer_temp;
    ping_buffer ping;

  } KermitMessage;

  typedef struct TeensyEndpoint
  {
    int sockfd;
    struct sockaddr_in dest;
    struct ifreq ifr;

    uint8_t send[64];
    uint8_t recv[64];

    int16_t lin;
    int16_t ang;
  } TeensyEndpoint;

  typedef struct KermitState
  {
    uint8_t current_state;

    uint8_t hard_stop;
    uint8_t dump;
    uint8_t mine;
    uint8_t move_to_mine;
    uint8_t move_to_dump;
    uint8_t init;
    uint8_t clean_exit;
  } KermitStates;

  TeensyEndpoint teensy;

  KermitProperties kermit;

  KermitMessage message;

  CCProperties params;

  KermitStates states;

  std::atomic<bool> running;
  std::unique_ptr<std::thread> async_sender;
};

} // namespace curmt
} // namespace scpp
#endif /* end of include guard KERMITKERNEL_HPP */
