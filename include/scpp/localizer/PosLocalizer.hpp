/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : PosLocalizer
 * @brief       : A brief description of the file
 *
 * A detailed description of the file
 *
 * @created     : Wednesday Jan 01, 2020 14:00:17 MST
 * @license     : MIT
 */

#ifndef POSLOCALIZER_HPP

#define POSLOCALIZER_HPP

#include "scpp/control/RMTControlClient.hpp"

extern "C"
{
#include "scpp/messages/rmt_messages.h"
}

namespace scpp {
namespace curmt {

class PosLocalizer : public scpp::curmt::RMTControlClient
{
public:
  PosLocalizer() = delete;

  PosLocalizer(bool verbose = false);
  virtual ~PosLocalizer() = default;

protected:
  // Sets msg safely
  void set_pos(float x, float y, float theta);
  // Prints x y and theta
  void print_state();

protected:
  bool verbose;

private:
  // The publisher callback
  std::string get_pos(void);

private:
  struct
  {
    float x;
    float y;
    float theta;
    camera_msg msg;
  } pos;
};

}
}

#endif /* end of include guard POSLOCALIZER_HPP */
