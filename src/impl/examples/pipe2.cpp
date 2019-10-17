/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : pipe
 * @created     : Friday Sep 20, 2019 20:47:05 MDT
 */

#include "scpp/core/messages/pipe/PipeInterface.hpp"
#include "scpp/messages/pipe/SimpleFilter.hpp"
#include <sstream>

int
main()
{

  auto a = std::make_unique<scpp::core::PipeInterface>();

  a->create_pub_sub_endpoint("hello", "tcp://*:5570", "tcp://*:5571");

  // a->set_filter("2", std::make_unique<scpp::filters::SimpleFilter>());
  sleep(10);

  sleep(3);
  a->stop();

  return 0;
}
