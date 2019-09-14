/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : proxy
 * @created     : Wednesday Sep 11, 2019 17:43:33 MDT
 */

// #include "control/zhelpers.hpp"
#include "messages/pipe/ZMQPubSubProxy.hpp"
#include "messages/pipe/ZMQServerProxy.hpp"
#include <future>
#include <thread>

int main() {
  std::promise<void> exit_thing;
  std::promise<void> exit_thing_2;

  ZMQPubSubProxy a("hello", std::move(exit_thing.get_future()), "tcp://*:5570", "tcp://*:5571", 1);
  ZMQServerProxy b("hello2", std::move(exit_thing_2.get_future()), "tcp://*:5555", "tcp://*:5556", 1);

  std::thread v1(&ZMQPubSubProxy::start, &a, std::chrono::microseconds(10));
  std::thread v2(&ZMQPubSubProxy::start, &b, std::chrono::microseconds(10));

  sleep(10);
  std::cout<<"killing"<<std::endl;

  a.signal(0);
  b.signal(0);
  
  v1.join();
  v2.join();

  return 0;
}
