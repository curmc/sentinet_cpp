/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ZMQPubSubProxy
 * @created     : Saturday Sep 14, 2019 13:29:23 MDT
 */

#include "scpp/messages/pipe/ZMQPubSubProxy.hpp"

namespace scpp {
namespace proxies {

ZMQPubSubProxy::ZMQPubSubProxy(const std::string& id_,
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

ZMQPubSubProxy::~ZMQPubSubProxy()
{
  if (frontend_sock) {
    frontend_sock->close();
  }
  if (backend_sock) {
    backend_sock->close();
  }
}

bool
ZMQPubSubProxy::__start__()
{
  frontend_sock = std::make_unique<::zmq::socket_t>(context, ZMQ_SUB);
  backend_sock = std::make_unique<::zmq::socket_t>(context, ZMQ_PUB);

  frontend_sock->bind(frontend);
  frontend_sock->setsockopt(ZMQ_SUBSCRIBE, "topic", 0);
  backend_sock->bind(backend);

  item = { static_cast<void*>(*frontend_sock.get()), 0, ZMQ_POLLIN, 0 };
  return true;
}

bool
ZMQPubSubProxy::__spin__()
{
  if (!frontend_sock || !backend_sock)
    return true;

  ::zmq::poll(&item, 1, 100);

  if (item.revents & ZMQ_POLLIN) {
    std::string topic = s_recv(*frontend_sock);
    std::string req = s_recv(*frontend_sock);
    if (!adding_filter) {
      for (auto i = 0U; i < filters.size(); ++i)
        filters[i]->convert(req);
      std::cout << "Traffic: \n";
      for (size_t i = 0; i < req.size(); ++i) {
        printf("%x ", req[i]);
      }
      printf("\n");

      s_sendmore(*backend_sock, topic);
      s_send(*backend_sock, req);
    } else {
      s_sendmore(*backend_sock, topic);
      s_send(*backend_sock, req);
    }
  }
  return true;
}

bool
ZMQPubSubProxy::__stop__()
{
  // if (frontend_sock)
  //   zmq_close(frontend_sock.get());
  // if (backend_sock)
  //   zmq_close(backend_sock.get());
  // std::this_thread::sleep_for(std::chrono::seconds(2));
  //   // backend_sock->close();
  // frontend_sock.release();
  // backend_sock.release();
  return true;
}

std::string
ZMQPubSubProxy::__get_type__()
{
  return "ZMQ PUB-SUB Proxy";
}
} // namespace proxies
} // namespace scpp
