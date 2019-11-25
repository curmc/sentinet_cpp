/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : ZMQControlClient
 * @created     : Saturday Nov 23, 2019 17:12:12 MST
 */

#include "scpp/control/ZMQControlClient.hpp"
#include "gtest/gtest.h"


class BootstrapZMQControlClient : public scpp::net::ZMQControlClient {
  public:
    BootstrapZMQControlClient(int context_)
      : scpp::net::ZMQControlClient(context_) {}
    virtual ~BootstrapZMQControlClient () = default;
};
