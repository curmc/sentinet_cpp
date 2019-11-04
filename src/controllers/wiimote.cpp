// Include logic for state

#include "scpp/controllers/wii/wiimote.h"
#include "scpp/controllers/helpers/helper.h"
#include "scpp/controllers/wii/wiibutton.h"
#include <iostream>
#include <iomanip>

// Linux specific for sleep, change for windows portability?
#include <unistd.h>

using namespace std;

namespace scpp::curmt
{


// A lot of this code is written only expecting one wiimote, if use cases change then update code
Wiimote::Wiimote() :
    KermitControlModule(),
    constants(default_constants),
    angular(0.0f),
    linear(0.0f),
    state(DRIVING),
    found(0),
    connected(0),
    verbose(false)
{
    data = wiiuse_init(MAXWIIMOTES);
}

Wiimote::Wiimote(const BotConstants alternative_constants, bool verbose) :
    KermitControlModule(),
    constants(alternative_constants),
    angular(0.0f),
    linear(0.0f),
    state(DRIVING),
    found(0),
    connected(0),
    verbose(verbose)
{
    data = wiiuse_init(MAXWIIMOTES);
}

Wiimote::~Wiimote() 
{
    wiiuse_cleanup(data, MAXWIIMOTES);
}


/* KermitControlModule funcitons. I may want to remove the connect and loop functions and just put their bodies inside here */
int Wiimote::__start__()
{
    connect();
    return 1;
}

int Wiimote::__update_state__()
{
    loop(chrono::high_resolution_clock::now());
    return 1;
}

int Wiimote::__quit__()
{
    // Do I need to put anything in here? 
    // I'd rather use the destructor since I know that will absolutely be called to handle deallocation
    return 1;
}

inline float Wiimote::__get_lin__() const
{
    return linear;
}

inline float Wiimote::__get_ang__() const
{
    return angular;
}

inline KERMIT_STATE Wiimote::__get_state__() const
{
    return state;
}


/* Simple 'Am I connected?' function */
int Wiimote::heartbeat()
{
    if (!data) {
        cout << "HEARTBEAT FAILURE: Unexpected wiimote disconnect\n";
        return 0;
    }

    for (int i = 0; i < connected; i++) {
        if (data[i] && WIIMOTE_IS_CONNECTED(data[i])) {
            return 1;
        }
    }

    return 0;
}

int Wiimote::connect() 
{
    found = wiiuse_find(data, MAXWIIMOTES, 5);
    if (!found) 
    {
        cout << "No wiimotes found." << endl;
        return -1;
    }

    connected = wiiuse_connect(data, MAXWIIMOTES);
    if (connected) 
    {
        cout << "Connected to " << connected << "wiimote";
        (connected > 1) ? cout << "s.\n" : cout << ".\n";
    }
    else 
    {
        cout << "Failed to connect, although found " << found << "wiimote";
        (found > 1) ? cout << "s.\n" : cout << ".\n";

        return -1;
    }

    /*
	 *	Now set the LEDs and rumble for a second so it's easy
	 *	to tell which wiimotes are connected (just like the wii does).
     *  Also, only expecting one wiimote for now. 
	 */
	wiiuse_set_leds(data[0], WIIMOTE_LED_1);
	
    /* rumble is annoying */
    /*
    wiiuse_rumble(data[0], 1);

    sleep(1);

    wiiuse_rumble(data[0], 0);
    */
    // Saving battery
    wiiuse_motion_sensing(data[0], 0);
    wiiuse_set_ir(data[0], 0);
    wiiuse_set_motion_plus(data[0], 0);

    return 0;
}

int Wiimote::loop(high_resolution_clock::time_point now)
{
    poll();
    control(now);
    
    if (verbose) 
    {
        static int counter = 0;
        counter++;
        if (counter > 100) {
            counter = 0;
            output_status();
        }    
    }
    
    return 0;
}

int Wiimote::poll() 
{
    if (heartbeat()) 
    {
        if (wiiuse_poll(data, MAXWIIMOTES)) 
        {
            for (int i = 0; i < MAXWIIMOTES; i++) 
            {
                switch(data[i]->event) 
                {
                    case WIIUSE_EVENT:
                        /* removed as all it does is print */
                        event();
                        break;
                    
                    case WIIUSE_STATUS:
                        if (verbose)
                            output_status();
                        break;
                    
                    case WIIUSE_DISCONNECT:
                    case WIIUSE_UNEXPECTED_DISCONNECT:
                        //handle disconnects
                        break;

                    case WIIUSE_READ_DATA:
                        break;


                    default:
                        break;
                }
            }
        }
    }

    return 0;
}


int Wiimote::event()
{
    /* for now, if a button is pressed, report it */
    const auto output_pressed_button = [&](const WiiButton& button)
    {
        if (IS_PRESSED(data[0], button.button_num)) 
        {
            cout << button.button_str << " pressed\n";
        }
    };

    if (verbose)
        for_each(wii_buttons.begin(), wii_buttons.end(), output_pressed_button);

    return 0;
}

/* Function to handle setting linear and angular velocities, and changing state */
int Wiimote::control(const high_resolution_clock::time_point &now)
{
    duration<double, std::milli> ms_elapsed = now - previous;

    float linear_change = constants.linear_incriment * ms_elapsed.count();
    float angular_change = constants.angular_incriment * ms_elapsed.count();

    const auto incriment_velocity = [&](float velocity, float incriment, const unsigned &button_increase, const unsigned &button_decrease) -> float
    {
        if (IS_PRESSED(data[0], button_increase))
        {
            velocity += incriment;
        }
        else if (IS_PRESSED(data[0], button_decrease))
        {
            velocity -= incriment;
        }
        else
        {
            if (velocity > 0)
            {
                velocity -= incriment;
                velocity = zero_rounding(velocity, incriment);
            }
            else if (velocity < 0)
            {
                velocity += incriment;
                velocity = zero_rounding(velocity, incriment);
            }
        }

        return velocity;
    };

    const auto toggle_state = [&](KERMIT_STATE toggling, const unsigned &button)
    {
        if (IS_JUST_PRESSED(data[0], button))
        {
            state = !(state & toggling) ? state | toggling : state - toggling;
            /* Strange bug continuously toggles buttons until another one is pressed. Here's a fix */
            data[0]->btns &= WIIMOTE_BUTTON_ONE;
        }
    };

    linear = incriment_velocity(linear, linear_change, WIIMOTE_BUTTON_UP, WIIMOTE_BUTTON_DOWN);
    angular = incriment_velocity(angular, angular_change, WIIMOTE_BUTTON_RIGHT, WIIMOTE_BUTTON_LEFT);

    linear = clamp(linear, -constants.linear_max, constants.linear_max);
    angular = clamp(angular, -constants.angular_max, constants.angular_max);

    toggle_state(DRIVING, WIIMOTE_BUTTON_HOME);
    toggle_state(MINING, WIIMOTE_BUTTON_PLUS);
    toggle_state(DUMPING, WIIMOTE_BUTTON_MINUS);

    previous = now;

    return 0;
}

int Wiimote::output_status()
{
    const auto output_state = [&](KERMIT_STATE status, string name)
    {
        string output = (state & status) ? "1" : "0";
        cout << name << ": " << output << '\n';
    };
    
    cout << "---- CONTROLLER STATUS ----\n"
         << "battery:      " << data[0]->battery_level << '\n'
         << setprecision(4) << "Linear: " << linear 
         << "\nAngular: " << angular << '\n'
         << "STATE\n"; 
    
    output_state(DRIVING, "DRIVING");
    output_state(MINING, "MINING");
    output_state(DUMPING, "DUMPING");

    return 0;
}

} // End Namespace
