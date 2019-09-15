/**
 * @author      : theo (theo@varnsen)
 * @file        : ZMQServerProxy
 * @brief
 *
 *
 *
 * @created     : Thursday Sep 12, 2019 10:29:34 MDT
 * @bugs No Known Bugs
 */

#ifndef ZMQSERVERPROXY_HPP

#define ZMQSERVERPROXY_HPP

// C++ Includes

// Local Includes
#include "core/messages/pipe/ProxyInterface.hpp"
#include "control/zhelpers.hpp"

namespace scpp {
namespace proxies {

class ZMQServerProxy : public ::scpp::core::ProxyInterface {
 public:
  ZMQServerProxy(const std::string&, std::future<void>, const std::string&,
                 const std::string&, int context);
  ~ZMQServerProxy();

 protected:
  bool __spin__() override;
  bool __start__() override;
  bool __stop__() override;
  std::string __get_type__() override;

 private:
  std::unique_ptr<::zmq::socket_t> frontend_sock;
  std::unique_ptr<::zmq::socket_t> backend_sock;
  ::zmq::context_t context;
  ::zmq::pollitem_t items[2];
};

}  // namespace proxies
}  // namespace scpp
#endif /* end of include guard ZMQSERVERPROXY_HPP */
