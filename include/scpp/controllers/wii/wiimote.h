#ifndef WIIMOTE_H
#define WIIMOTE_H

#include "scpp/kernel/KermitControlModule.hpp"

#include <wiiuse.h>
#include "scpp/controllers/helpers/bot_constants.h"
#include <chrono>

/* chrono for timing purposes, making linear and angular scaled by second */
using namespace std::chrono;

namespace scpp::curmt {

/* these are just some garbage default values, need to replace with real ones */
constexpr BotConstants default_constants =
  BotConstants(100.0f, 0.033f, 180.0f, 0.02f);

class Wiimote : public scpp::curmt::KermitControlModule
{
public:
  Wiimote();
  Wiimote(const BotConstants, bool verbose = false);
  ~Wiimote();

  int output_status();
  int heartbeat();

protected:
  /* KermitControlModule implimentation functions */
  int __start__() override;
  int __quit__() override;
  int __update_state__() override;

  /* Do we want these to be const correct? */
  float __get_lin__() const override;
  float __get_ang__() const override;
  KERMIT_STATE __get_state__() const override;

  int connect();
  int loop(high_resolution_clock::time_point);
  int poll();
  int control(const high_resolution_clock::time_point&);
  int event();

  const BotConstants constants;

  wiimote_t** data;

  float angular;
  float linear;

  high_resolution_clock::time_point previous;

  KERMIT_STATE state;

  int found;
  int connected;

  const bool verbose;

  static constexpr int MAXWIIMOTES{ 1 };
};

} // End namespace
#endif
