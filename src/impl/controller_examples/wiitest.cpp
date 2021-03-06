#include "scpp/controllers/wii/wiimote.h"
#include <iostream>
#include <chrono>

using namespace std;

int
main()
{
  scpp::curmt::Wiimote wii;
  wii.start_kermit();
  if (wii.heartbeat()) {
    cout << "Beginning Polling. No exit for now so ctrl-c ;)" << endl
         << "B to rumble, A to stop\n";
    wii.loop_kermit();
  } else {
    cout << "Unable to connect" << endl;
  }
  wii.quit_kermit();
  return 0;
}
