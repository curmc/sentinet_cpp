/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : pipe
 * @created     : Friday Sep 20, 2019 20:47:05 MDT
 */

#include "core/messages/pipe/PipeInterface.hpp"
#include <sstream>

int main() {
  
  auto a = std::make_unique<scpp::core::PipeInterface>();

  std::stringstream ss;
  std::stringstream ss2;

  for(int i = 0; i < 80; ++i) {
    ss << "tcp://*:550" << i;
    ss2 << "tcp://*:650" << i;
    std::cout<<ss.str()<<" "<<ss2.str()<<std::endl;
    a->create_pub_sub_endpoint(ss.str(),
        ss.str(), ss.str());
    ss.clear();
    ss.str(std::string());
    ss2.clear();
    ss2.str(std::string());
  }

  sleep(10);

  a->stop("hello");

  sleep(3);
  a->stop();

  return 0;
}
