/**
 * @author      : theo (theo@varnsen)
 * @file        : ZMQPubSubProxy
 * @brief
 *
 *
 *
 * @created     : Thursday Sep 12, 2019 10:29:34 MDT
 * @bugs No Known Bugs
 */

#ifndef ZMQPUBSUBPROXY_HPP

#define ZMQPUBSUBPROXY_HPP

// C++ Includes

// Local Includes
#include "scpp/core/messages/pipe/ProxyInterface.hpp"
#include "scpp/control/zhelpers.hpp"

namespace scpp {
namespace proxies {

class ZMQPubSubProxy : public ::scpp::core::ProxyInterface
{
public:
  ZMQPubSubProxy(const std::string&,
                 std::future<void>,
                 const std::string&,
                 const std::string&,
                 int context = 1);
  ~ZMQPubSubProxy();

protected:
  bool __spin__() override;
  bool __start__() override;
  bool __stop__() override;
  std::string __get_type__() override;

private:
  std::unique_ptr<::zmq::socket_t> frontend_sock;
  std::unique_ptr<::zmq::socket_t> backend_sock;
  ::zmq::context_t context;
  ::zmq::pollitem_t item;
};

} // namespace proxies
} // namespace scpp

#endif /* end of include guard ZMQPUBSUBPROXY_HPP */
