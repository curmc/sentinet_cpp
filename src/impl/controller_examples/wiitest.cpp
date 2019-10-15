#include "messages/pipe/SimpleFilter.hpp"

#include "wiimote.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace scpp::curmt;

int main(int argc, char* argv[]) 
{
    Wiimote wii;
    int error = wii.connect();
    if (!error) 
    {
        cout << "Beginning Polling. No exit for now so ctrl-c ;)" << endl << "B to rumble, A to stop\n";
        while(wii.heartbeat()) 
        {
            wii.loop(chrono::high_resolution_clock::now());
        }
    }
    else 
    {
        cout << "Unable to connect" << endl;
    }

    return 0;
}