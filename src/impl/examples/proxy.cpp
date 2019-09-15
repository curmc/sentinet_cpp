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

  ZMQPubSubProxy a("hello", std::move(exit_thing.get_future()), "tcp://*:5553", "tcp://*:5555", 1);

  std::thread v1(&ZMQPubSubProxy::start, &a, std::chrono::microseconds(10));

  sleep(20);
  std::cout<<"killing"<<std::endl;

  a.signal(0);
  
  v1.join();
  return 0;
}
