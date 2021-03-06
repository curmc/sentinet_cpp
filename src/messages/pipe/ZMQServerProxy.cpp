/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ZMQServerProxy
 * @created     : Saturday Sep 14, 2019 13:29:23 MDT
 */

#include "scpp/messages/pipe/ZMQServerProxy.hpp"

namespace scpp {
namespace proxies {
ZMQServerProxy::ZMQServerProxy(const std::string& id_,
                               std::future<void> futureObj,
                               const std::string& frontend_,
                               const std::string& backend_,
                               int context_)
  : ProxyInterface(id_, std::move(futureObj), frontend_, backend_)
  , context(context_)
{
  frontend_sock = nullptr;
  backend_sock = nullptr;
}

ZMQServerProxy::~ZMQServerProxy()
{
  if (frontend_sock)
    frontend_sock->close();
  if (backend_sock)
    backend_sock->close();
}

bool
ZMQServerProxy::__start__()
{
  frontend_sock = std::make_unique<::zmq::socket_t>(context, ZMQ_ROUTER);
  backend_sock = std::make_unique<::zmq::socket_t>(context, ZMQ_DEALER);

  frontend_sock->bind(frontend);
  backend_sock->bind(backend);

  items[0] = { static_cast<void*>(*frontend_sock.get()), 0, ZMQ_POLLIN, 0 };
  items[1] = { static_cast<void*>(*backend_sock.get()), 0, ZMQ_POLLIN, 0 };
  return true;
}

bool
ZMQServerProxy::__spin__()
{
  if (!frontend_sock || !backend_sock)
    return true;

  zmq::message_t message;
  int more;

  zmq::poll(&items[0], 2, 100);

  if (items[0].revents & ZMQ_POLLIN) {
    while (1) {

      size_t more_size = sizeof(more);
      frontend_sock->recv(&message);
      std::cout << "Traffic:" << std::endl;
      frontend_sock->getsockopt(ZMQ_RCVMORE, &more, &more_size);
      backend_sock->send(message, more ? ZMQ_SNDMORE : 0);
      if (!more)
        break;
    }
  }
  if (items[1].revents & ZMQ_POLLIN) {
    while (1) {
      backend_sock->recv(&message);
      size_t more_size = sizeof(more);
      backend_sock->getsockopt(ZMQ_RCVMORE, &more, &more_size);
      frontend_sock->send(message, more ? ZMQ_SNDMORE : 0);
      if (!more)
        break;
    }
  }
  return true;
}

bool
ZMQServerProxy::__stop__()
{
  return true;
}

std::string
ZMQServerProxy::__get_type__()
{
  return "ZMQ ROUTER DEALER Proxy";
}
} // namespace proxies
} // namespace scpp
