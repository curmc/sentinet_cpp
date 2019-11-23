#include "wiiuse.h"
#include <string>
#include <array>

/* Could do all this constexpr with c++17 and string_view */
class WiiButton
{
public:
  WiiButton(const unsigned num, const std::string str)
    : button_num(num)
    , button_str(str)
  {
  }
  ~WiiButton() = default;
  const unsigned button_num;
  const std::string button_str;
};

const std::array<WiiButton, 11> wii_buttons = {
  WiiButton(WIIMOTE_BUTTON_A, "A"),
  WiiButton(WIIMOTE_BUTTON_B, "B"),
  WiiButton(WIIMOTE_BUTTON_UP, "UP"),
  WiiButton(WIIMOTE_BUTTON_DOWN, "DOWN"),
  WiiButton(WIIMOTE_BUTTON_LEFT, "LEFT"),
  WiiButton(WIIMOTE_BUTTON_RIGHT, "RIGHT"),
  WiiButton(WIIMOTE_BUTTON_MINUS, "MINUS"),
  WiiButton(WIIMOTE_BUTTON_PLUS, "PLUS"),
  WiiButton(WIIMOTE_BUTTON_ONE, "ONE"),
  WiiButton(WIIMOTE_BUTTON_TWO, "TWO"),
  WiiButton(WIIMOTE_BUTTON_HOME, "HOME")
};
