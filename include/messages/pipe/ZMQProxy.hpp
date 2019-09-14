/**
 * @author      : theo (theo@varnsen)
 * @file        : ZMQProxy
 * @brief
 *
 *
 *
 * @created     : Thursday Sep 12, 2019 09:22:14 MDT
 * @bugs No Known Bugs
 */

#ifndef ZMQPROXY_HPP

#define ZMQPROXY_HPP

// C++ Includes

// Local Includes
#include "core/messages/pipe/ProxyInterface.hpp"

class ZMQProxy : ProxyInterface {
public:
  ZMQProxy() = delete;
  ZMQProxy(const std::string& id_, std::future<void> futureObj, const std::string& frontend_, const std::string& backend_, const int& context_) 


};

#endif /* end of include guard ZMQPROXY_HPP */
