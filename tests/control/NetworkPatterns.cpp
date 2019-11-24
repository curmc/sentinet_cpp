/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : NetworkPatterns
 * @created     : Saturday Nov 23, 2019 23:28:21 MST
 */

#include "NetworkPatterns.hpp"
#include "scpp/core/messages/pipe/PipeInterface.hpp"

namespace {

TEST(NetworkPatterns, PublisherContext) {
  {
    auto context = std::make_unique<BootstrapPublisherContext>();
    std::promise<void> exit_signal_1;

    // Some initializations
    context->set_sock_addr("tcp://localhost:5555");
    context->set_topic("test");
    context->set_period(std::chrono::microseconds(1));
    context->set_data_request_callback(
        [](void) -> std::string {
          return "Hello from test";
        });

    // Initialize the ZMQ Socket
    auto context_v = zmq::context_t(1);
    auto socket = std::make_unique<zmq::socket_t>(context_v, ZMQ_PUB);
    context->set_socket(std::move(socket));

    context->set_exit_signal(std::move(exit_signal_1.get_future()));
    std::future<void> background_publisher = std::async(&BootstrapPublisherContext::enter_thread, context.get());

    usleep(500);
    exit_signal_1.set_value();
    usleep(100);

    background_publisher.get();
  }
}


TEST(NetworkPatterns, ServerContext) {
  {
    auto context = std::make_unique<BootstrapServerContext>();
    std::promise<void> exit_signal_1;

    // Some initializations
    context->set_address("tcp://localhost:5555");
    context->set_callback(
        [](std::string& value) -> std::string {
          return "Recieved" + value; 
        });

    // Initialize the ZMQ Socket
    auto context_v = zmq::context_t(1);
    auto socket = std::make_unique<zmq::socket_t>(context_v, ZMQ_REP);
    context->set_socket(std::move(socket));

    context->set_exit_signal(std::move(exit_signal_1.get_future()));
    std::future<void> background_server = std::async(&BootstrapServerContext::enter_thread, context.get());

    usleep(500);
    exit_signal_1.set_value();
    usleep(100);

    background_server.get();
  }
}

TEST(NetworkPatterns, SubscriberContext) {
  {
    auto context = std::make_unique<BootstrapSubscriberContext>();
    std::promise<void> exit_signal_1;

    // Some initializations
    context->set_sock_addr("tcp://localhost:5555");
    context->set_callback(
        [](std::string& value) -> void {
          std::cout << "Recieved" + value << std::endl;
        });

    // Initialize the ZMQ Socket
    auto context_v = zmq::context_t(1);
    auto socket = std::make_unique<zmq::socket_t>(context_v, ZMQ_SUB);
    context->set_socket(std::move(socket));

    context->set_exit_signal(std::move(exit_signal_1.get_future()));
    std::future<void> background_subscriber = std::async(&BootstrapSubscriberContext::enter_thread, context.get());

    usleep(500);
    exit_signal_1.set_value();
    usleep(100);

    background_subscriber.get();
  }
}

static std::unique_ptr<scpp::core::PipeInterface> proxies;
void signalHandler(int signum) {
  if(proxies)
    proxies->stop();
}

TEST(NetworkPatterns, PUBSUBTest) {
  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);

  proxies = std::make_unique<scpp::core::PipeInterface>();
  proxies->create_pub_sub_endpoint("cmd_vel", "tcp://*:5570", "tcp://*:5571");

  {
    auto publisher = std::make_unique<BootstrapPublisherContext>();
    auto subscriber = std::make_unique<BootstrapSubscriberContext>();
    std::promise<void> exit_signal_1;
    std::promise<void> exit_signal_2;

    subscriber->set_sock_addr("tcp://localhost:5571");
    subscriber->set_callback(
      [](std::string& value) -> void {
        EXPECT_EQ(value, "TEST");
      });

    // Some initializations
    publisher->set_sock_addr("tcp://localhost:5570");
    publisher->set_topic("cmd_vel");
    publisher->set_period(std::chrono::microseconds(1));
    publisher->set_data_request_callback(
        [](void) -> std::string {
          return "TEST";
        });


    // Initialize the ZMQ Socket
    auto context_v = zmq::context_t(1);


    auto socket_pub = std::make_unique<zmq::socket_t>(context_v, ZMQ_PUB);
    publisher->set_socket(std::move(socket_pub));
    auto socket_sub = std::make_unique<zmq::socket_t>(context_v, ZMQ_SUB);
    subscriber->set_socket(std::move(socket_sub));


    publisher->set_exit_signal(std::move(exit_signal_1.get_future()));
    subscriber->set_exit_signal(std::move(exit_signal_2.get_future()));

    std::future<void> background_publisher = std::async(&BootstrapPublisherContext::enter_thread, publisher.get());

    std::future<void> background_subscriber = std::async(&BootstrapSubscriberContext::enter_thread, subscriber.get());

    usleep(1000000);
    exit_signal_1.set_value();
    exit_signal_2.set_value();
    usleep(100);


    background_subscriber.get();
    background_publisher.get();
  }
  proxies->stop();
}

// TEST(NetworkPatterns, RequesterContext) {
//   {
//     auto context = std::make_unique<BootstrapRequesterContext>();
//     std::promise<void> exit_signal_1;
//
//     // Some initializations
//     context->set_server("tcp://localhost:5555");
//     context->set_period(std::chrono::seconds(1));
//     context->set_data_request(
//         [](void) -> std::string {
//           return "Sending value";
//         });
//     context->set_callback(
//         [](std::string& value) -> void {
//           std::cout << "Recieved" + value << std::endl;
//         });
//
//     // Initialize the ZMQ Socket
//     auto context_v = zmq::context_t(1);
//     auto socket = std::make_unique<zmq::socket_t>(context_v, ZMQ_REQ);
//     context->set_socket(std::move(socket));
//
//     context->set_exit_signal(std::move(exit_signal_1.get_future()));
//     std::future<void> background_requester = std::async(&BootstrapRequesterContext::enter_thread, context.get());
//
//     usleep(500);
//     exit_signal_1.set_value();
//     usleep(100);
//
//     background_requester.get();
//   }
// }
//




}
