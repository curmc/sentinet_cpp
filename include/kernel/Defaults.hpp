/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : Defaults
 * @created     : Sunday Oct 13, 2019 14:35:58 MDT
 */

#ifndef DEFAULTS_HPP

#define DEFAULTS_HPP

#include <string>

namespace scpp {
namespace curmt {
namespace defaults {
std::string cmd_vel_front_end = "tcp://localhost:5555";
std::string cmd_vel_back_end = "tcp://localhost:5556";
std::string serve_proxy = "tcp://localhost:5557";
std::string client_proxy = "tcp://localhost:5558";

std::string local_cmd_vel_front_end = "tcp://*:5555";
std::string local_cmd_vel_back_end = "tcp://*:5556";
std::string local_serve_proxy = "tcp://*:5557";
std::string local_client_proxy = "tcp://*:5558";

const std::string cmd_vel_topic = "cmd_vel";
}
}
}

#endif /* end of include guard DEFAULTS_HPP */
