/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ZMQPubSubProxy
 * @created     : Saturday Sep 14, 2019 13:29:23 MDT
 */

#include "messages/pipe/ZMQPubSubProxy.hpp"

ZMQPubSubProxy::ZMQPubSubProxy(const std::string& id_, std::future<void> futureObj, const std::string& frontend_, const std::string& backend_, int context_)
  : ProxyInterface(id_, std::move(futureObj), frontend_, backend_), 
    context(context_)
{
  frontend_sock = nullptr;
  backend_sock = nullptr;
}

ZMQPubSubProxy::~ZMQPubSubProxy()
{
  if(frontend_sock)
    frontend_sock->close();
  if(backend_sock)
    backend_sock->close();
}

bool ZMQPubSubProxy::__start__() {
  frontend_sock = std::make_unique<::zmq::socket_t>(context, ZMQ_SUB);
  backend_sock = std::make_unique<::zmq::socket_t>(context, ZMQ_PUB);

  frontend_sock->bind(frontend);
  frontend_sock->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  backend_sock->bind(backend);

  item = {static_cast<void*>(*frontend_sock.get()), 0, ZMQ_POLLIN, 0};
  return true;
}

bool ZMQPubSubProxy::__spin__() {
  if(!frontend_sock || !backend_sock)
    return true;
  ::zmq::message_t message;
  int more;

  ::zmq::poll(&item, 1, 100);

  if(item.revents & ZMQ_POLLIN) {
    while(1) {
      frontend_sock->recv(&message);
      size_t more_size = sizeof(more);
      frontend_sock->getsockopt(ZMQ_RCVMORE, &more, &more_size);
      backend_sock->send(message, more ? ZMQ_SNDMORE : 0);
      if(!more)
        break;
    }
  }
  return true;
}

bool ZMQPubSubProxy::__stop__() {
  return true;
}

std::string ZMQPubSubProxy::__get_type__() {
  return "ZMQ XPUB-XSUB Proxy";
}
