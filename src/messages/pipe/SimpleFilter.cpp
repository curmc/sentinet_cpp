/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimplePipe
 * @created     : Saturday Sep 14, 2019 18:25:43 MDT
 */

#include "messages/pipe/SimpleFilter.hpp"

namespace scpp {
namespace filters {
void SimpleFilter::convert(std::string& message) {
  message = "[Passed through simple filter 1 ] " + message;
}
}
}
