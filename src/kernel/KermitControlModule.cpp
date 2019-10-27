/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitControlModule
 * @created     : Saturday Oct 12, 2019 23:30:12 MDT
 */

#include "scpp/kernel/KermitControlModule.hpp"

namespace scpp {
namespace curmt {

KermitControlModule::KermitControlModule(const std::string& publish_channel_)
{
  publish_channel = publish_channel_;
}

KermitControlModule::~KermitControlModule()
{
  quit_kermit();
}

bool
KermitControlModule::start_kermit(const std::string& topic)
{
  __start__();

  scpp::publish_params publish;
  publish.broker_frontend = publish_channel;
  publish.topic = topic;
  publish.get_data = [this]() -> std::string {
    // TODO add functionality in serializeation library to fix this
    memset(&buff, 0, sizeof(buff));

    // In c you can take a temporary address of an rvlaue, but c++ whines
    // Maybe a std::move would fix this, I don't think it would
    float templin = __get_lin__();
    float tempang = __get_ang__();

    serialize_data(&buff, &templin, sizeof(float), FLOAT);
    serialize_data(&buff, &tempang, sizeof(float), FLOAT);
    to_wire(&buff);
    return scpp::string((const char*)buff.data, (size_t)buff.byte_length);
  };

  publish.period = std::chrono::milliseconds(10);
  // Start publishing
  spin(publish);

  return true;
}

bool
KermitControlModule::loop_kermit()
{
  while (1) {
    __update_state__();
  }
  return true;
}

bool
KermitControlModule::quit_kermit()
{
  __quit__();
  quit();
  return true;
}

}
}
