/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : kermit_proxy
 * @created     : Sunday Oct 13, 2019 14:35:45 MDT
 */

#include "messages/pipe/ZMQPubSubProxy.hpp"
#include "messages/pipe/ZMQServerProxy.hpp"
#include "kernel/Defaults.hpp"

int
main() {
  std::promise<void> exit_publisher;
  std::promise<void> exit_server;

  scpp::proxies::ZMQPubSubProxy a(
      scpp::curmt::defaults::cmd_vel_topic, exit_publisher.get_future(),
      scpp::curmt::defaults::local_cmd_vel_front_end,
      scpp::curmt::defaults::local_cmd_vel_back_end, 1);

  scpp::proxies::ZMQPubSubProxy b(
      "server", exit_publisher.get_future(),
      scpp::curmt::defaults::local_serve_proxy,
      scpp::curmt::defaults::local_client_proxy, 1);

  std::thread t1(
      &scpp::proxies::ZMQPubSubProxy::start, &a, std::chrono::milliseconds(10));
  std::thread t2(
      &scpp::proxies::ZMQServerProxy::start, &b, std::chrono::milliseconds(10));

  sleep(20);
  std::cout << "Killing "<<std::endl;

  a.signal(0);
  b.signal(0);

  t1.join();
  t2.join();
  return 1;
}
