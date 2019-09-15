/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimplePipe
 * @created     : Saturday Sep 14, 2019 18:25:43 MDT
 */

#include "messages/pipe/SimpleFilter2.hpp"

void SimpleFilter2::convert(std::string& message) {
  message = "[Passed through filter 2] " + message;
}
