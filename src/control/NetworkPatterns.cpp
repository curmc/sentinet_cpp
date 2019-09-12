/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : NetworkPatterns
 * @created     : Wednesday Sep 11, 2019 15:27:00 MDT
 */

// Local Includes
#include "control/NetworkPatterns.hpp"

/**
 * These are the thread functions, what each socket will do
 *
 * Notice that I am recieving the stuff before hand then entering the loop,
 * I did this so that I own a copy of each data element when I enter the thread
 * I guess I may be a little paranoid of race conditions
 */

void Publisher_Context::periodic_publish_thread(thread_properties &properties) {
  // Grab the socket
  auto socket = std::move(properties.socket);
  socket->connect(properties.sock_addr);

  // Grab the exit signal
  auto exit_signal = std::move(properties.exit_signal);

  // Grab the callback
  auto get_data = properties.data;

  // Grab the topic
  auto topic = properties.topic;

  // Grab the period
  auto period = properties.period;

  LOG_INFO("Publisher has just begun publishing to topic %s",
           properties.topic.c_str());

  auto start = std::chrono::steady_clock::now();
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    start = std::chrono::steady_clock::now();
    s_sendmore(*socket, topic);
    s_send(*socket, get_data());

    // Definitely a better way to do this, also note that
    // this gets out of sync easily
    // maybe have a is multiple of checker so that even though clock actually
    // ticks on real world clock cycle
    auto wait_time = period + start - std::chrono::steady_clock::now();
    if (wait_time > std::chrono::milliseconds::zero())
      std::this_thread::sleep_for(wait_time);
  }

  LOG_INFO("Publisher thread has been terminated on topic %s", topic.c_str());
}

void Subscriber_Context::subscription_thread(thread_properties &properties) {

  auto socket = std::move(properties.socket);
  socket->connect(properties.address);
  socket->setsockopt(ZMQ_SUBSCRIBE, properties.topic.c_str(),
                     properties.topic.length());

  auto callback = properties.callback;

  auto exit_signal = std::move(properties.exit_signal);

  std::string preallocated_message_string;

  ::zmq::pollitem_t item = {static_cast<void *>(*socket.get()), 0, ZMQ_POLLIN,
                            0};
  LOG_INFO("Subscriber thread has been started on topic %s",
           properties.topic.c_str());

  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    zmq::poll(&item, 1, 100);
    if (item.revents & ZMQ_POLLIN) {
      preallocated_message_string = s_recv(*socket); // topic name
      preallocated_message_string = s_recv(*socket);
      callback(preallocated_message_string);
    }
  }

  LOG_INFO("Subscriber thread has been terminated on topic %s",
           properties.topic.c_str());
}

void Requester_Context::requester_thread(thread_properties &properties) {
  // Grab the socket
  auto socket = std::move(properties.socket);

  // Grab the exit signal
  auto exit_signal = std::move(properties.exit_signal);

  // Grab the address
  auto address = properties.server;

  // Grab the period
  auto period = properties.period;

  // Grab the callback
  auto callback = properties.callback;
  auto get_data = properties.get_data;

  LOG_INFO("New requester thread has begun");

  std::string preallocated_request_string;
  auto start = std::chrono::steady_clock::now();

  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    start = std::chrono::steady_clock::now();

    socket->connect(address);
    s_send(*socket, get_data());
    preallocated_request_string = s_recv(*socket);
    callback(preallocated_request_string);
    socket->disconnect(address);

    auto wait_time = period + start - std::chrono::steady_clock::now();
    if (wait_time > std::chrono::milliseconds::zero())
      std::this_thread::sleep_for(wait_time);
  }

  LOG_INFO("A request thread has ended");
}

void Server_Context::server_thread(thread_properties &properties) {
  // Set up the socket
  auto socket = std::move(properties.socket);
  socket->connect(properties.sock_addr);

  auto exit_signal = std::move(properties.exit_signal);

  auto callback = properties.callback;

  // Create a zmq poller to check on the socket
  ::zmq::pollitem_t item = {static_cast<void *>(*socket.get()), 0, ZMQ_POLLIN,
                            0};

  LOG_INFO("Server thread attached to address %s has begun",
           properties.sock_addr.c_str());

  std::string preallocated_request_string;
  std::string preallocated_server_string;

  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    zmq_poll(&item, 1, 100);
    if (item.revents & ZMQ_POLLIN) {
      preallocated_request_string = s_recv(*socket);
      preallocated_server_string = callback(preallocated_server_string);
      s_send(*socket, preallocated_server_string);
    }
  }

  LOG_INFO("Server thread at address %s has ended",
           properties.sock_addr.c_str());
}
