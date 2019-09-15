/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : NetworkPatterns
 * @created     : Wednesday Sep 11, 2019 15:03:54 MDT
 */

#ifndef NETWORKPATTERNS_HPP

#define NETWORKPATTERNS_HPP

// Local includes
#include "core/types.h"
#include "control/zhelpers.hpp"
#include "core/utils/logging.hpp"

namespace scpp {
namespace net {

class Publisher_Context {

private:
  typedef struct thread_properties {
    scpp::string sock_addr;
    scpp::future<void> exit_signal;
    scpp::unique_ptr<::zmq::socket_t> socket;
    scpp::string topic;
    scpp::function<scpp::string(void)> data;
    scpp::time::microseconds period;
  } thread_properties;

  static void periodic_publish_thread(thread_properties &properties);

  thread_properties properties;

public:
  Publisher_Context() {}
  ~Publisher_Context() {}

  inline void set_sock_addr(const scpp::string &sock_addr) {
    properties.sock_addr = sock_addr;
  }

  inline void set_exit_signal(scpp::future<void> exit_signal) {
    properties.exit_signal = scpp::move(exit_signal);
  }

  inline void set_socket(scpp::unique_ptr<::zmq::socket_t> socket) {
    properties.socket = scpp::move(socket);
  }

  inline void set_topic(const scpp::string &topic) { properties.topic = topic; }

  inline void
  set_data_request_callback(scpp::function<scpp::string(void)> callback) {
    properties.data = callback;
  }

  inline void set_period(const scpp::time::microseconds period) {
    properties.period = period;
  }

  inline void enter_thread() { periodic_publish_thread(properties); }
};

class Server_Context {

private:
  typedef struct thread_properties {
    scpp::string sock_addr;
    scpp::future<void> exit_signal;
    scpp::unique_ptr<::zmq::socket_t> socket;
    scpp::function<scpp::string(scpp::string &)> callback;
  } thread_properties;

  static void server_thread(thread_properties &properties);

  thread_properties properties;

public:
  Server_Context() {}
  ~Server_Context() {}

  inline void set_exit_signal(scpp::future<void> exit_signal) {
    properties.exit_signal = scpp::move(exit_signal);
  }

  inline void set_socket(scpp::unique_ptr<::zmq::socket_t> socket) {
    properties.socket = scpp::move(socket);
  }

  inline void set_address(const scpp::string &address) {
    properties.sock_addr = address;
  }

  inline void set_callback(scpp::function<scpp::string(scpp::string &)> callback) {
    properties.callback = callback;
  }

  inline void enter_thread() { server_thread(properties); }
};

class Subscriber_Context {

private:
  typedef struct thread_properties {
    scpp::string topic;
    scpp::string address;
    scpp::future<void> exit_signal;
    scpp::unique_ptr<::zmq::socket_t> socket;
    scpp::function<void(scpp::string &)> callback;
  } thread_properties;

  static void subscription_thread(thread_properties &properties);

  thread_properties properties;

public:
  Subscriber_Context() {}
  ~Subscriber_Context() {}

  inline void set_exit_signal(scpp::future<void> exit_signal) {
    properties.exit_signal = scpp::move(exit_signal);
  }

  inline void set_sock_addr(const scpp::string &address) {
    properties.address = address;
  }

  inline void set_topic(const scpp::string &topic) { properties.topic = topic; }

  inline void set_socket(scpp::unique_ptr<::zmq::socket_t> socket) {
    properties.socket = scpp::move(socket);
  }

  inline void set_callback(scpp::function<void(scpp::string &)> callback) {
    properties.callback = callback;
  }

  inline void enter_thread() { subscription_thread(properties); }
};

class Requester_Context {
private:
  typedef struct thread_properties {
    scpp::future<void> exit_signal;
    scpp::unique_ptr<::zmq::socket_t> socket;
    scpp::string server;
    scpp::function<scpp::string(void)> get_data;
    scpp::function<void(scpp::string &)> callback;
    scpp::time::microseconds period;
  } thread_properties;

  static void requester_thread(thread_properties &properties);

  thread_properties properties;

public:
  Requester_Context() {}
  ~Requester_Context() {}

  inline void set_exit_signal(scpp::future<void> exit_signal) {
    properties.exit_signal = scpp::move(exit_signal);
  }

  inline void set_socket(scpp::unique_ptr<::zmq::socket_t> socket) {
    properties.socket = scpp::move(socket);
  }

  inline void set_data_request(scpp::function<scpp::string(void)> callback) {
    properties.get_data = callback;
  }

  inline void set_callback(scpp::function<void(scpp::string &)> callback) {
    properties.callback = callback;
  }

  inline void set_period(const scpp::time::microseconds period) {
    properties.period = period;
  }

  inline void enter_thread() { requester_thread(properties); }
};

}
}
#endif /* end of include guard NETWORKPATTERNS_HPP */
