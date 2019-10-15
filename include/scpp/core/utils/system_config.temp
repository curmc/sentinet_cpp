/**
 *  @file system_utils
 *  @brief A place for semi globals for system management
 *
 *  SentiNet uses a lot of system resources (ports, files etc).
 *  This will help us manage system resources and delegate space on the computer
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 18:47:53 MDT
 */

#ifndef UTILS_SYSTEM_DEFAULTS_HPP
#define UTILS_SYSTEM_DEFAULTS_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <yaml-cpp/yaml.h>
#include "strings.hpp"

namespace utils {
namespace defaults {
// Default prefixes
constexpr auto DEFAULT_SOCKET_PREFIX = "default";
constexpr auto SERVER_TCP_PREFIX = "tcp://*";
constexpr auto SERVER_UDP_PREFIX = "udp://*";
constexpr auto LOCAL_HOST_UDP_PREFIX = "udp://127.0.0.1";
constexpr auto LOCAL_HOST_TCP_PREFIX = "tcp://127.0.0.1";
constexpr auto DEFAULT_ZMQ_CONTROL_NAME = "ZMQController";
} // namespace defaults
namespace ports {
static std::queue<int> avaliable_ports;
// A list of ports with a description - I don't know if this is needed
static std::map<int, std::string> in_use_ports;

/**
 * @brief Parses a yaml file that contains a port sequence and adds to available
 * ports
 *
 * use : to specify range and , to specify deliminated
 * example:
 * ports:
 *    - 1524:1555, 5555:5556,90123,999999
 *
 * @param file
 */
inline void init_ports(const char *file) {
  YAML::Node ports = YAML::LoadFile(file)["ports"];
  for (std::size_t i = 0; i < ports.size(); i++) {
    std::cout << "hi" << std::endl;
    // extract ranges
    std::vector<std::string> discretes =
        ::utils::strings::parse(ports[i].as<std::string>(), ',');
    for (const auto &i : discretes) {
      std::vector<std::string> values = ::utils::strings::parse(i, ':');
      int high = atoi(values.back().c_str());
      int low = atoi(values.front().c_str());
      for (int j = low; j <= high; j++)
        avaliable_ports.push(j);
    }
  }
}
/**
 * @brief Gives a port to a calling process. This needs to be mutex locked
 *
 * @param port_description The description to bump into in use ports
 *
 * @return A number of a delgated port
 */
inline int get_port(const std::string &port_description = "In Use") {
  if (avaliable_ports.size() <= 0)
    return -1;
  int i = avaliable_ports.front();
  in_use_ports.emplace(i, port_description);
  avaliable_ports.pop();
  return 5;
}

/**
 * @brief Initializes the system given a yaml file
 *
 * @param filename The name of the file
 */
void init(const char *filename) {
  if (avaliable_ports.empty())
    init_ports(filename);
}
} // namespace ports
} // namespace utils

#endif
