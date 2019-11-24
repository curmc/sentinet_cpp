/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : NetworkPatterns
 * @created     : Saturday Nov 23, 2019 23:28:21 MST
 */

#include "NetworkPatterns.hpp"

TEST(NetworkPatterns, PublisherContext) {
  {
    auto context = std::make_unique<BootstrapPublisherContext>();
    std::promise<void> exit_signal_1;

    // Some initializations
    context->set_sock_addr("tcp://localhost:5555");
    context->set_topic("test");
    context->set_period(std::chrono::seconds(1));
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

// TEST(NetworkPatterns, SubscriberContext) {
//   {
//     auto context = std::make_unique<BootstrapSubscriberContext>();
//     std::promise<void> exit_signal_1;
//
//     // Some initializations
//     context->set_address("tcp://localhost:5555");
//     context->set_callback(
//         [](std::string& value) -> void {
//           std::cout << "Recieved" + value << std::endl;
//         });
//
//     // Initialize the ZMQ Socket
//     auto context_v = zmq::context_t(1);
//     auto socket = std::make_unique<zmq::socket_t>(context_v, ZMQ_SUB);
//     context->set_socket(std::move(socket));
//
//     context->set_exit_signal(std::move(exit_signal_1.get_future()));
//     std::future<void> background_subscriber = std::async(&BootstrapSubscriberContext::enter_thread, context.get());
//
//     usleep(500);
//     exit_signal_1.set_value();
//     usleep(100);
//
//     background_subscriber.get();
//   }
// }
