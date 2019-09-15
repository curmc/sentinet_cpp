/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ControlClientInterface
 * @created     : Saturday Jul 27, 2019 09:54:19 MDT
 */

#ifndef ZMQCONTROLCLIENT_HPP

#define ZMQCONTROLCLIENT_HPP

// Local includes
#include "core/types.h"
#include "core/utils/logging.hpp"
#include "core/utils/strings.hpp"
#include "core/control/ControlClientInterface.hpp"
#include "control/zhelpers.hpp"
#include "control/NetworkPatterns.hpp"

/**
 * @brief A ZMQControl Client is an implementation of the Control Client
 * Interface. CCI by default is not multithreaded, nor is it as complex as
 * ZMQCC. ZMQ Control Client is structured in a way that allows an application
 * to push subscribers and publishers into a map of said subs and pubs. This
 * allows all messages to be passed in the same process (you don't need two
 * seperate files for a publisher and subscriber) This is useful for modular
 * applications. Eventually, sentinet will allow applications to be uploaded
 * from a central system that allocates resources to the local robot controller.
 * A few important things here. This is multithreaded, ZMQ takes care of race
 * conditions, however, it is strongly advised to read the documentation before
 * diving into the multithreaded aspects of ZMQ. Namely, sockets shouldn't share
 * threads. This is why ZMQ socket thread space exists, to seperate sockets from
 * threads.
 */

namespace utils {
namespace defaults {
constexpr auto DEFAULT_SOCKET_PREFIX = "default";
constexpr auto SERVER_TCP_PREFIX = "tcp://*";
constexpr auto SERVER_UDP_PREFIX = "udp://*";
constexpr auto LOCAL_HOST_UDP_PREFIX = "udp://127.0.0.1";
constexpr auto LOCAL_HOST_TCP_PREFIX = "tcp://127.0.0.1";
constexpr auto DEFAULT_ZMQ_CONTROL_NAME = "ZMQController";
} // namespace defaults
} // namespace utils

namespace scpp {
namespace net {

class ZMQControlClient : public ::scpp::core::ControlClientInterface
{
public:
  ZMQControlClient(int context_ = 1,
                   const scpp::string& yaml_system_file = "empty");
  ~ZMQControlClient() {}

  bool start(int i = 0) override;
  bool quit(int i = 0) override;

public:
  /**
   * @brief In order to publish or request concurrently, you MUST first
   * initialize publisher or client by default, client and publisher don't
   * exist. This would be a method put in the constructor to an implementation
   * specific. The reason is because these two could just cause high overhead.
   * Instead, initializing them explicitly allows the program to only open the
   * necessary sockets
   *
   * @return status
   */
  bool initialize_publisher(const scpp::string& address) override;

  /**
   * @brief Refer to initialize_publisher
   *
   * @return Status
   */
  bool initialize_client() override;

  /**
   * API Guide - implimentation - inherited methods from ControlClientInterface
   */
public:
  ///////////////////////////// PUBLISH /////////////////////////////////
  // Publishes on <this thread>
  bool publish(const scpp::string& topic, const scpp::string& message) override;

  // Does not execute above fnc - creates a new periodic publisher thread
  bool publish(const scpp::string sock_addr,
               const scpp::string topic,
               scpp::function<scpp::string(void)> get_data_to_publish,
               scpp::time::microseconds period) override;

  bool cancel_periodic_publisher(const scpp::string&) override;

  ///////////////////////////// REQUEST /////////////////////////////////
  scpp::string request(const scpp::string destination,
                       const scpp::string message) override;

  // Does not execute above fnc
  bool request(const scpp::string destination,
               const scpp::string id,
               scpp::function<scpp::string(void)> get_data_to_request,
               const scpp::function<void(scpp::string&)> callback,
               const scpp::time::microseconds period) override;

  bool cancel_periodic_request(const scpp::string&) override;

  ///////////////////////////// SUBSCRIBE /////////////////////////////////
  bool subscribe(const scpp::string sock_addr,
                 const scpp::string topic,
                 scpp::function<void(scpp::string&)> callback) override;

  bool cancel_subscription(const scpp::string& topic) override;

  ///////////////////////////// SERVE /////////////////////////////////
  bool serve(const scpp::string address,
             scpp::function<scpp::string(scpp::string&)> callback) override;

  bool terminate_server(const scpp::string& address) override;

  // Thread functions
private:
  static void periodic_publish_thread(
    scpp::unique_ptr<Publisher_Context> pub_context)
  {
    pub_context->enter_thread();
  }
  static void periodic_request_thread(
    scpp::unique_ptr<Requester_Context> req_context)
  {
    req_context->enter_thread();
  }
  static void subscription_thread(
    scpp::unique_ptr<Subscriber_Context> sub_context)
  {
    sub_context->enter_thread();
  }
  static void server_thread(scpp::unique_ptr<Server_Context> serv_thread)
  {
    serv_thread->enter_thread();
  }

private:
  // Assuming socket is already bound to an address
  static void concurrent_publish(scpp::unique_ptr<::zmq::socket_t> socket,
                                 const scpp::string& topic,
                                 const scpp::string& message);

  scpp::string concurrent_request(const scpp::string& server,
                                  scpp::unique_ptr<::zmq::socket_t> socket,
                                  const scpp::string& message);

  // Data structures
private:
  /**
   * @brief A Place to store all socket information
   * @note sockets are not to be shared between threads but contexts should be
   *
   * @note This is simply a ghost that wraps X_Context. It contains all
   * the elements that this object has control to change
   */
  typedef struct
  {
    // Might add more
    scpp::unique_ptr<scpp::thread> thread;
    scpp::promise<void> exit_signal;
    scpp::unique_ptr<::zmq::socket_t> socket;
  } socket_thread_space;

  /**
   * @brief The data structure of our sockets.
   *
   * @note - A software engineer may say this is bad, instead, I
   * should have a map that maps strings to X_Context from
   * Network Patterns, however, I believe when it comes to multithreading
   * I'd rather grant context permissions to each object in charge of the
   * thread. So a socket_thread_space is simply the aspects that this
   * object needs access to
   */
  typedef struct socket_data_s
  {
    scpp::unordered_map<scpp::string, socket_thread_space> subscribers;
    scpp::unordered_map<scpp::string, socket_thread_space> servers;
    scpp::unordered_map<scpp::string, socket_thread_space> periodic_clients;
    scpp::unordered_map<scpp::string, socket_thread_space> periodic_publishers;
  } socket_data;

  /**
   * @brief Individual data per entity
   */
  typedef struct
  {
    unsigned int supported_threads;
    scpp::string control_node_name;
  } control_meta_data;

private:
  // Meta information - unique
  control_meta_data meta;

  // Thread spaces - Shared
  socket_data thread_space;

  // Unique per control client (ideally the same between processes)
  ::zmq::context_t context;

  // Unique per control client
  scpp::unique_ptr<::zmq::socket_t> this_publisher;
  scpp::unique_ptr<::zmq::socket_t> this_client;

  // Helper Functions
private:
  // Honestly I was just too lazy to write out std::map ......
  template<typename T>
  inline socket_thread_space& create_socket(int type,
                                            T& map,
                                            const scpp::string identifier)
  {
    socket_thread_space socket_thread;
    map.emplace(identifier, scpp::move(socket_thread));
    map[identifier].socket = std::make_unique<::zmq::socket_t>(context, type);
    return map[identifier];
  }
};

// Initialize static socket thread space with default constructor

} // namespace net
} // namespace scpp
#endif /* end of include guard ZMQCONTROLCLIENT_HPP */
