#include "messages/pipe/SimpleFilter.hpp"

#include "wiimote.h"
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) 
{
    scpp::curmt::Wiimote wii;
    wii.start_kermit("aaaa");
    if (wii.heartbeat()) 
    {
        cout << "Beginning Polling. No exit for now so ctrl-c ;)" << endl << "B to rumble, A to stop\n";
        wii.loop_kermit();
    }
    else 
    {
        cout << "Unable to connect" << endl;
    }
    wii.quit_kermit();
    return 0;
}