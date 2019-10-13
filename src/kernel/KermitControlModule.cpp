/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitControlModule
 * @created     : Saturday Oct 12, 2019 23:30:12 MDT
 */

#include "kernel/KermitControlModule.hpp"

namespace scpp {
namespace curmt {


KermitControlModule::KermitControlModule(std::string publish_channel_)
{
  buff = create_buffer();
  publish_channel = publish_channel_;
}

KermitControlModule::~KermitControlModule()
{
  quit_kermit();
}

bool KermitControlModule::set_linear(const float linear) {
  state.linear = linear;
  return true;
}

bool KermitControlModule::set_angular(const float angular) {
  state.angular = angular;
  return true;
}

bool KermitControlModule::set_dumping(bool val) {
  state.dumping = val;
  return true;
}

bool KermitControlModule::set_mining(bool val) {
  state.mining = val;
  return true;
}

bool KermitControlModule::trigger_mining() {
  state.mining = !state.mining;
  return state.mining;
}

bool KermitControlModule::trigger_dumping() {
  state.dumping = !state.dumping;
  return state.dumping;
}

bool KermitControlModule::start_kermit(std::string topic) {
  __start__();

  scpp::publish_params publish;
  publish.broker_frontend = publish_channel;
  publish.topic = topic;
  publish.get_data = [this] () -> std::string {
    memset(&buff, 0, sizeof(buff)); // TODO add functionality in serializeation library to fix this
    serialize_data(&buff, &state.linear, sizeof(state.linear), FLOAT);
    serialize_data(&buff, &state.angular, sizeof(state.angular), FLOAT);
    to_wire(&buff);
    return scpp::string((const char*)buff.data, (size_t)buff.byte_length);
  };
  publish.period = std::chrono::milliseconds(10);
  spin(publish);

  return true;
}

bool KermitControlModule::loop_kermit() {
  while(1) {
    __update_state__();
  }
  return true;
}

bool KermitControlModule::quit_kermit() {
  __quit__();
  quit();
  return true;
}


}
}
