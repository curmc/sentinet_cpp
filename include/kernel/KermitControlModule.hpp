/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitControlModule
 * @created     : Saturday Oct 12, 2019 23:23:15 MDT
 */

#ifndef KERMITCONTROLMODULE_HPP

#define KERMITCONTROLMODULE_HPP

// Local includes
#include "control/ZMQControlClient.hpp"
extern "C" {
#include "data_message.h"
}


// C++ includes
#include <chrono>
#include <memory>

namespace scpp {
namespace curmt {

class KermitControlModule : public ::scpp::net::ZMQControlClient
{
public:
  KermitControlModule(std::string publish_channel_);
  virtual ~KermitControlModule();

  bool set_linear(const float linear);
  bool set_angular(const float angular);

  bool zero_motors();

  bool set_dumping(bool val);
  bool set_mining(bool val);

  bool trigger_dumping();
  bool trigger_mining();

  bool start_kermit(std::string topic);
  bool loop_kermit();
  bool quit_kermit();

private:

  virtual bool __start__() = 0;
  virtual bool __quit__() = 0;
  virtual bool __update_state__() = 0;

  typedef struct
  {
    float linear;
    float angular;
    char mining : 1;
    char dumping : 1;
  } kermit_state;

  std::string publish_channel;
  data_buffer buff;
  kermit_state state;
};

}
}
#endif /* end of include guard KERMITCONTROLMODULE_HPP */
