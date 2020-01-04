/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : common
 * @created     : Thursday Jan 02, 2020 16:23:58 MST
 */

#include "scpp/common.h"

std::string
to_bind_addr(const int port)
{
  std::string ret("tcp://*:");
  return ret + std::to_string(port);
}
std::string
to_conn_addr(const int port)
{
  std::string ret("tcp://localhost:");
  return ret + std::to_string(port);
}
