
/**
 * @file ControlClientInterface
 * @brief A Control Client is the uer endpoint for network patterns
 *
 * A Control Client's sole job is to publish, subscribe, serve, and request.
 * What that means to an individual implimentation is up to the developer.
 * This is a pure virtual class, except for utility functions (implimentations
 * that make the main four methods easier). Implimenting a ControlClient
 * involves routing messages over some sort of network communication.
 *
 * CC's I would like to develop
 *
 * ZMQ
 * ROS / ROS2
 * RabbitMQ
 *
 * TODO
 * Need some sort of bridge service to connect over device interfaces
 *
 * @author  theo (theo@$Theo-Lenovo-Yoga-Arch)
 * @created  Saturday Jul 27, 2019 09:54:19 MDT
 */

#ifndef CONTROLINTERFACE_HPP

#define CONTROLINTERFACE_HPP

#include "scpp/common.h"

namespace scpp {
typedef struct serve_params
{
  serve_params()
  {
    sock_addr = "null";
    callback = [](std::string& val) -> std::string { return val; };
  }

  serve_params(const std::string& address_,
               std::function<std::string(std::string&)> callback_)
    : sock_addr(address_)
    , callback(callback_)
  {}

  serve_params(const serve_params& params)
    : sock_addr(params.sock_addr)
    , callback(params.callback)
  {}

  std::string sock_addr;
  std::function<std::string(std::string&)> callback;
} serve_params;

/**
 * @brief Pass the parameters as a struct, nicer format
 */
typedef struct publish_params
{
  publish_params()
  {
    sock_addr = "null";
    topic = "";
    get_data = [](void) -> std::string { return "No Publisher Data"; };
    period = std::chrono::microseconds(100);
  }

  publish_params(const std::string& broker, const std::string& topic_)
  {
    sock_addr = broker;
    topic = topic_;
    get_data = [](void) -> std::string { return "No Publisher Data"; };
    period = std::chrono::microseconds(100);
  }

  publish_params(const publish_params& pub)
    : sock_addr(pub.sock_addr)
    , topic(pub.topic)
    , get_data(pub.get_data)
    , period(pub.period)
  {}

  std::string sock_addr;
  std::string topic;
  std::function<std::string(void)> get_data;
  std::chrono::microseconds period;
} publish_params;

typedef struct subscribe_params
{
  subscribe_params()
  {
    sock_addr = "null";
    topic = "";
    callback = [this](std::string& val) -> void {
      LOG_DEBUG("Recieved %s on topic %s", val.c_str(), topic.c_str());
    };
  }
  subscribe_params(const std::string& sock_backend_, const std::string& topic_)
    : sock_addr(sock_backend_)
    , topic(topic_)
  {
    callback = [this](std::string& val) -> void {
      LOG_DEBUG("Recieved %s on topic %s", val.c_str(), topic.c_str());
    };
  }
  subscribe_params(const subscribe_params& params)
    : sock_addr(params.sock_addr)
    , topic(params.topic)
    , callback(params.callback)
  {}
  std::string sock_addr;
  std::string topic;
  std::function<void(std::string&)> callback;
} subscribe_params;

// typedef struct request_params
// {
//   request_params()
//   {
//     id = "REQUESTER";
//     sock_addr = "null";
//     get_data_to_request = [](void) -> std::string { return "null"; };
//     callback = [this](std::string& ret) -> void {
//       LOG_DEBUG("Requester %s recieved: %s", id.c_str(), ret.c_str());
//     };
//     period = std::chrono::microseconds(100);
//   }
//
//   request_params(const std::string& id_,
//                  const std::string& sock_addr_,
//                  const std::chrono::microseconds period_)
//     : id(id_)
//     , sock_addr(sock_addr_)
//     , period(period_)
//   {
//     get_data_to_request = [](void) -> std::string { return "null"; };
//     callback = [this](std::string& ret) -> void {
//       LOG_DEBUG("Requester recieved: %s", ret.c_str());
//     };
//   }
//
//   request_params(const request_params& params)
//       sock_addr(params.sock_addr)
//     , get_data_to_request(params.get_data_to_request)
//     , callback(params.callback)
//   {}
//
//   std::string sock_addr;
//   std::function<std::string(void)> get_data_to_request;
//   std::function<void(std::string&)> callback;
//   std::chrono::microseconds period;
// } request_params;
//
namespace core {
/**
 * @brief Control Client interface is the communication client in the Control
 * Base application
 *
 * This is a pure interface class. The object oriented approach is going to
 * allow flexibility with the type of communication clients that are
 * implimented. However, we should not get too carried away with an object
 * oriented approach. I thought about adding helper functions to this class, but
 * each Control client will have its own resources that it is utilizing. Thus,
 * I'm keeping only the base functions so that CCs have something to derrive
 * from. If anything, this is a template for how to publish onto a certain
 * control client.
 */
class ControlClientInterface
{
public:
  virtual ~ControlClientInterface() = default;

  /**
   * @brief A pure virtual way to start and stop a CC. This should be up to the
   * developer
   *
   * Start and stop are both dependent on the application, they are purely
   * virtual to encourage the developer to impliment a method for terminating
   * their CC.
   *
   * @return Status of start
   */
  virtual bool start(int params = 0) = 0;

  /**
   * @brief Quit should return satisfactorily and clean up all thread or
   * publishing objects
   *
   * @return Status of quit
   */
  virtual bool quit(int params = 0) = 0;

  /**
   * @brief Just another form of communication - quicker and faster flag
   *
   * @note This has the potential to be a rubish method, however, I'd like every
   * CC to have a flag implimentation
   *
   * @param flag The flag number
   *
   * @return a number - 1 for now, but could be helpful for status etc.
   */
  virtual int flag(int flag = 0) { return flag; }

  /**
   * Core API
   * main methods are
   *
   * publish
   * subscribe
   * serve
   * request
   */

  /**
   * @brief Initialize the publisher that this CC can use
   *
   * @param sock_addr The back end socket
   *
   * @return  Status
   */
  virtual bool initialize_publisher(const std::string& sock_addr)
  {
    return sock_addr == "default";
  }

  /**
   * @brief Initialize the client that this CC can use
   *
   * @param sock_addr The back end socket
   *
   * @return  Status
   */
  virtual bool initialize_client() { return false; }

  ////////////////////////// Publish Methods /////////////////////////////
  /**
   * Simple publisher - publishes message to topic one time
   *
   * @note the publisher connects to the address that we provided through init
   *
   * @param  topic Topic name, this can be a topic to a proxy, in which case
   * the message needs an envelope]
   * @param  message     The data we want to send
   *
   * @return             Status of the publish
   */
  virtual bool publish(const std::string& topic,
                       const std::string& message) = 0;

  // Set a periodic publish / request. i.e. developer attatches a callback to
  // the function to periodically occur
  /**
   * Periodic publisher - publishes message obtained at get_data_to_publish and
   * publishes every period
   *
   * @note it's ok to copy
   *
   * @param  topic [Topic to publish onto. Note that this is not const as it can
   * be changed]
   * @param  get_data_to_publish [Callback called to obtain the message we want
   * to send]
   * @param  period [Polling period to obtain the message from endpoint]
   *
   * @return       [status of the publish]
   */
  virtual bool publish(const std::string sock_addr,
                       const std::string topic,
                       std::function<std::string(void)> get_data_to_publish,
                       const std::chrono::microseconds period) = 0;

  inline bool publish(::scpp::publish_params& params)
  {
    return publish(
      params.sock_addr, params.topic, params.get_data, params.period);
  }

  inline bool spin(::scpp::publish_params& params) { return publish(params); }

  virtual bool cancel_periodic_publisher(const std::string&) = 0;

  ////////////////////////// Request Methods /////////////////////////////
  /**
   * Requests a string from an endpoint
   *
   * @param  sock_addr [sock_addr address of the server]
   * @param  message     [Message to send in request (can be just a blank
   * string)]
   *
   * @return             [The response from the server]
   */
  virtual std::string request(const std::string sock_addr,
                              const std::string message) = 0;

  /**
   * @brief Make A request at a regular period
   *
   * @param  sock_addr              [sock_addr of server]
   * @param  get_data_to_request [Address of the data that we will send] //TODO
   *
   * Probably a better way to do thisw
   *
   * @param  action_to_recieved_data  [What to do to our recieved data]
   * @return                          [Status of request, returns 1 is ends
   * badly]
   */
  virtual bool request(const std::string sock_addr,
                       const std::string id,
                       std::function<std::string(void)> get_data_to_request,
                       std::function<void(std::string&)> callback,
                       const std::chrono::microseconds period) = 0;

  // inline bool request(::scpp::request_params& params)
  // {
  //   return request(params.sock_addr,
  //                  params.id,
  //                  params.get_data_to_request,
  //                  params.callback,
  //                  params.period);
  // }
  //
  // inline bool spin(::scpp::request_params& params) { return request(params);
  // }

  /**
   * @brief Cancel a periodic_requester
   *
   * @param The sock_addr of the requester / requesters
   *
   * @return The
   */
  virtual bool cancel_periodic_request(const std::string&) = 0;

  ////////////////////////// Subscribe Methods /////////////////////////////
  /**
   * Subscribe to a topic with a specified callback
   *
   * @note This is an asynchronous control loop, this method splits off into
   * another threads
   *
   * @note Visbly to a developer, this should just look like
   * CC.subscribe(address, topic, callback). For Most implimentations, you'll
   * have to order subscribers by id, but this base method should not utilize an
   * id
   *
   * @param  topic     Topic to subscribe to
   * @param  callback  The callback function to execute on subscription
   *
   * @return           Status of the subscription if it ends
   */
  virtual bool subscribe(const std::string frontend_broker,
                         const std::string topic,
                         std::function<void(std::string&)> callback) = 0;

  inline bool subscribe(::scpp::subscribe_params& params)
  {
    return subscribe(params.sock_addr, params.topic, params.callback);
  }

  inline bool spin(::scpp::subscribe_params& params)
  {
    return subscribe(params);
  }

  virtual bool cancel_subscription(const std::string& topic) = 0;

  ////////////////////////// Serve Methods /////////////////////////////
  /**
   * Host a service at the specified address
   *
   * @note This (like subscribe) is asynchronous and enters into another control
   * loop
   *
   * @param  address [Address to bind to]
   * @param  [name]  [What to do to incomming messages from clients]
   *
   * @return         [Staus if the server dies]
   */
  virtual bool serve(const std::string address,
                     std::function<std::string(std::string&)> callback) = 0;

  inline bool serve(::scpp::serve_params& params)
  {
    return serve(params.sock_addr, params.callback);
  }

  inline bool spin(::scpp::serve_params& params) { return serve(params); }

  virtual bool terminate_server(const std::string& address) = 0;
};

} // namespace core
} // namespace scpp
#endif /* end of include guard CONTROLINTERFACE_HPP */
