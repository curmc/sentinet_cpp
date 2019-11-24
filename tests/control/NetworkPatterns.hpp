/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : ZMQControlClient
 * @created     : Saturday Nov 23, 2019 17:12:12 MST
 */

#include "scpp/control/NetworkPatterns.hpp"
#include "gtest/gtest.h"


class BootstrapPublisherContext : public scpp::net::Publisher_Context {
  public:
    BootstrapPublisherContext() {}
    virtual ~BootstrapPublisherContext () = default;
    
    void periodic_publish_thread(scpp::net::Publisher_Context::thread_properties& properties) {
      ::scpp::net::Publisher_Context::periodic_publish_thread(properties);
    }

    auto get_properties() {
      return std::move(properties);
    }
};

class BootstrapServerContext : public scpp::net::Server_Context {
  public:
    BootstrapServerContext() {}
    virtual ~BootstrapServerContext () = default;
    
    void server_thread(scpp::net::Server_Context::thread_properties& properties) {
      ::scpp::net::Server_Context::server_thread(properties);
    }

    auto get_properties() {
      return std::move(properties);
    }
};

class BootstrapSubscriberContext : public scpp::net::Subscriber_Context {
  public:
    BootstrapSubscriberContext () {}
    virtual ~BootstrapSubscriberContext () = default;
    
    void subscription_thread(scpp::net::Subscriber_Context::thread_properties& properties) {
      ::scpp::net::Subscriber_Context::subscription_thread(properties);
    }

    auto get_properties() {
      return std::move(properties);
    }
};

class BootstrapRequesterContext : public scpp::net::Requester_Context {
  public:
    BootstrapRequesterContext() {}
    virtual ~BootstrapRequesterContext() = default;

    void requester_thread(scpp::net::Requester_Context::thread_properties& properties) {
      ::scpp::net::Requester_Context::requester_thread(properties);
    }

    auto get_properties(){
      return std::move(properties);
    }
};
