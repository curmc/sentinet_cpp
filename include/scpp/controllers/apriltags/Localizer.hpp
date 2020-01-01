/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : Localizer
 * @brief       : A brief description of the file
 *
 * A detailed description of the file
 *
 * @created     : Wednesday Jan 01, 2020 14:00:17 MST
 * @license     : MIT
 */

#ifndef LOCALIZER_HPP

#define LOCALIZER_HPP

#include "scpp/control/ZMQControlClient.hpp"

extern "C" {
#include "scpp/messages/rmt_messages.h"
}

namespace scpp {
namespace curmt {

const static std::string camera_topic = "camera";
const static std::string camera_addr = "tcp://localhost:5555";

class Localizer : public scpp::net::ZMQControlClient
{
  public:
    Localizer() = delete;

    Localizer (bool verbose = false);
    virtual ~Localizer () = default;
    
    bool localizer_start(const std::chrono::seconds period_);
    bool localizer_quit();

  protected:
    void set_pos(float x, float y, float z, float theta);
    inline void stop_running() { running = false; }
    void print_state();
    bool verbose;

  private:
    // The publisher callback
    std::string get_pos(void);

  private:
    struct {
      pos_localizer loc;
      size_t size;
    } message;

    struct {
      scpp::publish_params camera_p; 
      std::string camera_addr;
      std::string camera_topic;
    } network;

    volatile bool running;
};

}
}

#endif /* end of include guard LOCALIZER_HPP */

