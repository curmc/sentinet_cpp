/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : SCPPSession
 * @brief       : A brief description of the file
 *
 * A detailed description of the file
 *
 * @created     : Wednesday Oct 16, 2019 12:12:16 MDT
 * @license     : MIT
 */

#ifndef SCPPSESSION_HPP

#define SCPPSESSION_HPP

// #include "scpp/messages/pipe/ZMQPubSubProxy.hpp"
// #include "scpp/messages/pipe/ZMQServerProxy.hpp"
// #include "scpp/messages/pipe/SimpleFilter.hpp"
#include <vector>
#include <string>
#include <iostream>

class SCPPSession
{
public:
  SCPPSession ();
  virtual ~SCPPSession () = default;
// private:

  int parse_command_line(int argc, const char** argv);

  // int interpret_server(std::vector<std::string>& argv, size_t index);
  // int interpret_pubsub(std::vector<std::string>& argv, size_t index);
  // int start();

  typedef struct {
    size_t starting_pub_port_f;
    size_t starting_pub_port_b;
    size_t starting_req_port;
    size_t starting_serv_port;

    size_t num_pub;
    size_t num_serv;

    std::vector<std::string> pub_sub_frontends;
    std::vector<std::string> pub_sub_backends;
    std::vector<std::string> serv_req_frontends;
    std::vector<std::string> serv_req_backends;
  } properties;

  properties sess;
};

#endif /* end of include guard SCPPSESSION_HPP */

