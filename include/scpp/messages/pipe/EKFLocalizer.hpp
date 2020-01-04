/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimpleFilter
 * @created     : Saturday Sep 14, 2019 18:24:03 MDT
 */

#ifndef SIMPLE_LOCALIZER

#define SIMPLE_LOCALIZER

// C++ includes

// Local Includes
#include "scpp/core/messages/pipe/FilterInterface.hpp"

extern "C"
{
#include "scpp/messages/rmt_messages.h"
}

namespace scpp {
namespace filters {

class Simple_Localizer : public ::scpp::core::FilterInterface
{
public:
  Simple_Localizer(){};
  virtual ~Simple_Localizer() = default;

  int convert(std::string& incomming_message) override;

private:
  void teensy_update(float v, float w, float dt);
  void camera_update(float x, float y, float theta, float dt);

  struct
  {
    path_state msg;
    teensy_sensor_array sensor;
    camera_msg camera;
  } message;

  struct
  {
    float x;
    float y;
    float theta;
    float v;
    float w;
  } state;
};
} // namespace filters
} // namespace scpp

#endif /* end of include guard SIMPLEFILTER_HPP */
