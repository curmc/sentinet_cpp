
#include "scpp/control/ZMQControlClient.hpp"

namespace scpp {
namespace net {
ZMQControlClient::ZMQControlClient(int context_)
  : context(context_)
{
  // By default, no pub or client
  this_publisher = nullptr;
  this_client = nullptr;

  // Name the node
  meta.control_node_name = addr::zmq_control_name;

  // For meta information, store the number of threadsd possible
  meta.supported_threads = std::thread::hardware_concurrency();
}

bool
ZMQControlClient::start(int)
{
  return true;
}

bool
ZMQControlClient::quit(int)
{
  // TODO - make this look prettier
  for (auto const& i : thread_space.subscribers)
    cancel_subscription(i.first);
  for (auto const& i : thread_space.servers)
    terminate_server(i.first);
  for (auto const& i : thread_space.periodic_publishers)
    cancel_periodic_publisher(i.first);
  for (auto const& i : thread_space.periodic_clients)
    cancel_periodic_request(i.first);
  return true;
}

bool
ZMQControlClient::initialize_publisher(const std::string& address)
{
  if (this_publisher)
    return false;
  this_publisher = std::make_unique<::zmq::socket_t>(context, ZMQ_PUB);
  this_publisher->connect(address);
  return true;
}

bool
ZMQControlClient::initialize_client()
{
  if (this_client)
    return false;
  this_client = std::make_unique<::zmq::socket_t>(context, ZMQ_REQ);
  return true;
}

bool
ZMQControlClient::publish(const std::string& topic, const std::string& message)
{
  if (!this_publisher) {
    LOG_ERROR("No Concurrent Publisher In This Control Thread");
    return false;
  }
  concurrent_publish(std::move(this_publisher), topic, message);
  return true; // TODO change this
}

bool
ZMQControlClient::publish(const std::string sock_addr,
                          const std::string topic,
                          std::function<std::string(void)> get_data_to_publish,
                          std::chrono::microseconds period)
{
  // If already exists do nothing
  auto&& found = thread_space.periodic_publishers.find(sock_addr);
  if (found != thread_space.periodic_publishers.end()) {
    std::cout << "Topic publisher already exists" << std::endl;
    return false;
  }

  // Store a new socket by key sock_addr
  // val is now a socket_thread_space
  auto&& val =
    create_socket(ZMQ_PUB, thread_space.periodic_publishers, sock_addr);

  // Create a future promise to exit the thread
  auto futureObj = val.exit_signal.get_future();
  auto pub_context = std::make_unique<Publisher_Context>();

  // Set publisher context properties
  pub_context->set_sock_addr(sock_addr);
  pub_context->set_exit_signal(std::move(futureObj));
  pub_context->set_socket(std::move(val.socket));
  pub_context->set_topic(topic);
  pub_context->set_data_request_callback(get_data_to_publish);
  pub_context->set_period(period);

  // Create a new thread - attatched to periodic  publisher
  val.thread = std::make_unique<std::thread>(
    &ZMQControlClient::periodic_publish_thread, std::move(pub_context));
  return true;
}

bool
ZMQControlClient::cancel_periodic_publisher(const std::string& reference)
{
  auto found = thread_space.periodic_publishers.find(reference);
  if (found != thread_space.periodic_publishers.end()) {
    thread_space.periodic_publishers[reference].exit_signal.set_value();
    // if (thread_space.periodic_publishers[reference].thread->joinable())
    thread_space.periodic_publishers[reference].thread->join();
    return true;
  }
  return false;
}

std::string
ZMQControlClient::request(const std::string destination,
                          const std::string message)
{
  if (!this_client) {
    LOG_ERROR("No Concurrent Requester In This Control Thread");
    return "";
  }
  return concurrent_request(destination, std::move(this_client), message);
}

bool
ZMQControlClient::request(
  const std::string destination,
  const std::string id,
  std::function<std::string(void)> get_data_to_request,
  std::function<void(std::string&)> action_to_recieved_data,
  const std::chrono::microseconds period)
{
  // See if element is allready in map
  auto&& found = thread_space.periodic_clients.find(id);
  if (found != thread_space.periodic_clients.end()) {
    LOG_WARN("Destination already exists");
    return false;
  }

  // create and assign the address of val (a socket_thraed_space) to the map
  // val is now a socket_thread_space
  auto&& val = create_socket(ZMQ_REQ, thread_space.periodic_clients, id);

  // Create a future promise to exit
  auto&& futureObj = val.exit_signal.get_future();
  auto req_context = std::make_unique<Requester_Context>();

  req_context->set_exit_signal(std::move(futureObj));
  req_context->set_socket(std::move(val.socket));
  req_context->set_data_request(get_data_to_request);
  req_context->set_callback(action_to_recieved_data);
  req_context->set_period(period);
  req_context->set_server(destination);

  // Our main thread
  val.thread = std::make_unique<std::thread>(
    &ZMQControlClient::periodic_request_thread, std::move(req_context));
  return true;
}

bool
ZMQControlClient::cancel_periodic_request(const std::string& reference)
{
  auto found = thread_space.periodic_clients.find(reference);
  if (found != thread_space.periodic_clients.end()) {
    thread_space.periodic_clients[reference].exit_signal.set_value();
    // if (thread_space.periodic_clients[reference].thread->joinable())
    thread_space.periodic_clients[reference].thread->join();
    return true;
  }
  return false;
}

bool
ZMQControlClient::subscribe(const std::string sock_addr,
                            const std::string topic,
                            std::function<void(std::string&)> callback)
{
  auto&& found = thread_space.subscribers.find(topic);

  if (found != thread_space.subscribers.end()) {
    LOG_WARN("Subscriber Already Exists");
    return false;
  }

  auto&& val = create_socket(ZMQ_SUB, thread_space.subscribers, sock_addr);

  auto&& futureObj = val.exit_signal.get_future();

  auto sub_context = std::make_unique<Subscriber_Context>();

  sub_context->set_exit_signal(std::move(futureObj));
  sub_context->set_socket(std::move(val.socket));
  sub_context->set_callback(callback);
  sub_context->set_sock_addr(sock_addr);
  sub_context->set_topic(topic);

  val.thread = std::make_unique<std::thread>(
    &ZMQControlClient::subscription_thread, std::move(sub_context));

  return true;
}

bool
ZMQControlClient::cancel_subscription(const std::string& reference)
{
  auto found = thread_space.subscribers.find(reference);
  if (found != thread_space.subscribers.end()) {
    thread_space.subscribers[reference].exit_signal.set_value();
    // if (thread_space.subscribers[reference].thread->joinable())
    thread_space.subscribers[reference].thread->join();
    return true;
  }
  return false;
}

bool
ZMQControlClient::serve(const std::string address,
                        std::function<std::string(std::string&)> callback)
{
  auto&& found = thread_space.servers.find(address);
  if (found != thread_space.servers.end()) {
    std::cout << "Already have a server there bud" << std::endl;
    return false;
  }

  auto&& val = create_socket(ZMQ_REP, thread_space.servers, address);
  auto&& futureObj = val.exit_signal.get_future();

  auto serv_context = std::make_unique<Server_Context>();

  serv_context->set_address(address);
  serv_context->set_exit_signal(std::move(futureObj));
  serv_context->set_socket(std::move(val.socket));
  serv_context->set_callback(callback);

  val.thread = std::make_unique<std::thread>(&ZMQControlClient::server_thread,
                                             std::move(serv_context));
  return true;
}

bool
ZMQControlClient::bind_server(const std::string address,
                              std::function<std::string(std::string&)> callback)
{
  auto&& found = thread_space.servers.find(address);
  if (found != thread_space.servers.end()) {
    std::cout << "Already have a server there bud" << std::endl;
    return false;
  }

  auto&& val = create_socket(ZMQ_REP, thread_space.servers, address);
  auto&& futureObj = val.exit_signal.get_future();

  auto serv_context = std::make_unique<Server_Context>(true);

  serv_context->set_address(address);
  serv_context->set_exit_signal(std::move(futureObj));
  serv_context->set_socket(std::move(val.socket));
  serv_context->set_callback(callback);

  val.thread = std::make_unique<std::thread>(&ZMQControlClient::server_thread,
                                             std::move(serv_context));
  return true;
}

bool
ZMQControlClient::terminate_server(const std::string& reference)
{
  auto found = thread_space.servers.find(reference);
  if (found != thread_space.servers.end()) {
    thread_space.servers[reference].exit_signal.set_value();
    // if (thread_space.servers[reference].thread->joinable())
    thread_space.servers[reference].thread->join();
    return true;
  }
  return false;
}

void
ZMQControlClient::concurrent_publish(std::unique_ptr<::zmq::socket_t> socket,
                                     const std::string& topic,
                                     const std::string& message)
{
  s_sendmore(*socket, topic);
  s_send(*socket, message);
}

#define REQUEST_RETRIES 3
std::string
ZMQControlClient::concurrent_request(const std::string& server,
                                     std::unique_ptr<::zmq::socket_t> socket,
                                     const std::string& message)
{
  socket->connect(server);
  int linger = 0;
  socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
  int retries_left = REQUEST_RETRIES;
  std::string reply = "No response";

  while (retries_left) {
    s_send(*socket, message);
    bool expect_reply = true;
    while (expect_reply) {
      zmq::pollitem_t items[] = {
        { static_cast<void*>(*socket), 0, ZMQ_POLLIN, 0 }
      };
      zmq::poll(&items[0], 1, 100);
      if (items[0].revents & ZMQ_POLLIN) {
        reply = s_recv(*socket);
        expect_reply = false;
        socket.reset();
        initialize_client();

        return reply;
      } else if (--retries_left == 0) {
        LOG_ERROR("Request cannot reach server, dropping request");
        expect_reply = false;
        break;
      } else {
        LOG_WARN("No response from server, retrying");

        socket.reset();

        socket = std::make_unique<::zmq::socket_t>(context, ZMQ_REQ);

        socket->connect(server);
        int linger = 0;
        socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
        s_send(*socket, message);
      }
    }
  }

  // I found it nicer to just delete
  // the client everytime
  socket.reset();
  initialize_client();

  return reply;
}

} // namespace net
} // namespace scpp
