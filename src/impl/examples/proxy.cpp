/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : proxy
 * @created     : Wednesday Sep 11, 2019 17:43:33 MDT
 */

// #include "control/zhelpers.hpp"
#include "scpp/messages/pipe/ZMQPubSubProxy.hpp"
#include "scpp/messages/pipe/ZMQServerProxy.hpp"
#include "scpp/messages/pipe/SimpleFilter.hpp"
#include "scpp/messages/pipe/SimpleFilter2.hpp"
#include <future>
#include <thread>

int
main()
{
  std::promise<void> exit_thing;
  std::promise<void> exit_thing_2;

  scpp::proxies::ZMQPubSubProxy a(
    "cmd_vel", exit_thing.get_future(), "tcp://*:5570", "tcp://*:5571", 1);

  // a.add_filter(std::make_unique<scpp::filters::SimpleFilter>());
  // a.add_filter(std::make_unique<scpp::filters::SimpleFilter2>());

  std::thread v1(
    &scpp::proxies::ZMQPubSubProxy::start, &a, std::chrono::microseconds(10));

  sleep(60);
  std::cout << "killing" << std::endl;

  a.signal(0);

  v1.join();
  return 0;
}
