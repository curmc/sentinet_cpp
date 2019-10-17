/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : SCPPSession
 * @created     : Wednesday Oct 16, 2019 13:47:22 MDT
 */

#include "scpp/session/SCPPSession.hpp"

SCPPSession::SCPPSession() {
}

int SCPPSession::parse_command_line(int argc, char** argv) {
  std::vector<std::string> args;
  int i = 0;
  while(i < argc) 
    args[i] = argv[i++];

  if(!i) {
    
  }
  return i;
}
