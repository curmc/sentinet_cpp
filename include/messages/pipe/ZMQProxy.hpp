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

class ZMQProxy {
  public:
    ZMQProxy() = delete;

    ZMQProxy(const int context_ = 1, const std::string& frontend_addr, const std::string& backend_addr) 
      : context(context_), frontend(context, ZMQ_XSUB), backend(context, ZMQ_XPUB) {
        frontend.bind(frontend_addr);
        backend.bind(backend_addr);
        zmq::proxy(frontend, backend, nullptr);
    }

    ~ZMQProxy() = default;

  private:
    zmq::socket_t frontend;
    zmq::socket_t backend;
    zmq::context_t  context;
};


#endif /* end of include guard ZMQPROXY_HPP */

