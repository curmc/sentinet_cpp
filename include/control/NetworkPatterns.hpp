/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : NetworkPatterns
 * @created     : Wednesday Sep 11, 2019 15:03:54 MDT
 */

#ifndef NETWORKPATTERNS_HPP

#define NETWORKPATTERNS_HPP

// C++ includes
#include <memory>
#include <string>
#include <chrono>
#include <future>

// Local includes
#include "control/zhelpers.hpp"
#include "core/utils/logging.hpp"

class Publisher_Context {

private:
  typedef struct thread_properties {
    std::string sock_addr;
    std::future<void> exit_signal;
    std::unique_ptr<::zmq::socket_t> socket;
    std::string topic;
    std::function<std::string(void)> data;
    std::chrono::microseconds period;
  } thread_properties;

  static void periodic_publish_thread(thread_properties &properties);

  thread_properties properties;

public:
  Publisher_Context() {}
  ~Publisher_Context() {}

  inline void set_sock_addr(const std::string &sock_addr) {
    properties.sock_addr = sock_addr;
  }

  inline void set_exit_signal(std::future<void> exit_signal) {
    properties.exit_signal = std::move(exit_signal);
  }

  inline void set_socket(std::unique_ptr<::zmq::socket_t> socket) {
    properties.socket = std::move(socket);
  }

  inline void set_topic(const std::string &topic) { properties.topic = topic; }

  inline void
  set_data_request_callback(std::function<std::string(void)> callback) {
    properties.data = callback;
  }

  inline void set_period(const std::chrono::microseconds period) {
    properties.period = period;
  }

  inline void enter_thread() { periodic_publish_thread(properties); }
};

class Server_Context {

private:
  typedef struct thread_properties {
    std::string sock_addr;
    std::future<void> exit_signal;
    std::unique_ptr<::zmq::socket_t> socket;
    std::function<std::string(std::string &)> callback;
  } thread_properties;

  static void server_thread(thread_properties &properties);

  thread_properties properties;

public:
  Server_Context() {}
  ~Server_Context() {}

  inline void set_exit_signal(std::future<void> exit_signal) {
    properties.exit_signal = std::move(exit_signal);
  }

  inline void set_socket(std::unique_ptr<::zmq::socket_t> socket) {
    properties.socket = std::move(socket);
  }

  inline void set_address(const std::string &address) {
    properties.sock_addr = address;
  }

  inline void set_callback(std::function<std::string(std::string &)> callback) {
    properties.callback = callback;
  }

  inline void enter_thread() { server_thread(properties); }
};

class Subscriber_Context {

private:
  typedef struct thread_properties {
    std::string topic;
    std::string address;
    std::future<void> exit_signal;
    std::unique_ptr<::zmq::socket_t> socket;
    std::function<void(std::string &)> callback;
  } thread_properties;

  static void subscription_thread(thread_properties &properties);

  thread_properties properties;

public:
  Subscriber_Context() {}
  ~Subscriber_Context() {}

  inline void set_exit_signal(std::future<void> exit_signal) {
    properties.exit_signal = std::move(exit_signal);
  }

  inline void set_sock_addr(const std::string &address) {
    properties.address = address;
  }

  inline void set_topic(const std::string &topic) { properties.topic = topic; }

  inline void set_socket(std::unique_ptr<::zmq::socket_t> socket) {
    properties.socket = std::move(socket);
  }

  inline void set_callback(std::function<void(std::string &)> callback) {
    properties.callback = callback;
  }

  inline void enter_thread() { subscription_thread(properties); }
};

class Requester_Context {
private:
  typedef struct thread_properties {
    std::future<void> exit_signal;
    std::unique_ptr<::zmq::socket_t> socket;
    std::string server;
    std::function<std::string(void)> get_data;
    std::function<void(std::string &)> callback;
    std::chrono::microseconds period;
  } thread_properties;

  static void requester_thread(thread_properties &properties);

  thread_properties properties;

public:
  Requester_Context() {}
  ~Requester_Context() {}

  inline void set_exit_signal(std::future<void> exit_signal) {
    properties.exit_signal = std::move(exit_signal);
  }

  inline void set_socket(std::unique_ptr<::zmq::socket_t> socket) {
    properties.socket = std::move(socket);
  }

  inline void set_data_request(std::function<std::string(void)> callback) {
    properties.get_data = callback;
  }

  inline void set_callback(std::function<void(std::string &)> callback) {
    properties.callback = callback;
  }

  inline void set_period(const std::chrono::microseconds period) {
    properties.period = period;
  }

  inline void enter_thread() { requester_thread(properties); }
};

#endif /* end of include guard NETWORKPATTERNS_HPP */
