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

#include "scpp/messages/pipe/ZMQPubSubProxy.hpp"
#include "scpp/messages/pipe/ZMQServerProxy.hpp"
#include "scpp/messages/pipe/SimpleFilter.hpp"


#ifndef COMMAND_FLAGS
#define FRONTEND_PUBSUB "-f"
#define BACKEND_PUBSUB "-b"
#define FRONTEND_SERVER "-fs"
#define BACKEND_SERVER "-bs"
#endif

/*
 * RULES
 *
 * -p denotes pub sub endpoint - 
 * -f denotes front end endpoint
 * -b denotes back end endpoint
 * -fs denotes front end server endpoint
 * -bs denotes back end server endpoint
 */

class SCPPSession
{
public:
  SCPPSession ();
  virtual ~SCPPSession ();
private:

  int parse_command_line(int argc, char** argv);

  int interpret_server(std::vector<std::string>& argv, size_t index);
  int interpret_pubsub(std::vector<std::string>& argv, size_t index);
  int start(); 
     
};

#endif /* end of include guard SCPPSESSION_HPP */

