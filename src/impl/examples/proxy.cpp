/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : proxy
 * @created     : Wednesday Sep 11, 2019 17:43:33 MDT
 */

#include "control/zhelpers.hpp"

int main() {
  zmq::context_t ctx(1);
  zmq::socket_t frontend(ctx, /*ZMQ_ROUTER*/ ZMQ_XSUB);
  zmq::socket_t backend(ctx, /*ZMQ_DEALER*/ ZMQ_XPUB);
  frontend.bind("tcp://*:5570");
  backend.bind("tcp://*:5571");
  zmq::proxy(frontend, backend, nullptr);
  return 0;
}
