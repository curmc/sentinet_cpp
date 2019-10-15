/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimplePipe
 * @created     : Saturday Sep 14, 2019 18:25:43 MDT
 */

#include "scpp/messages/pipe/SimpleFilter2.hpp"

namespace scpp {
namespace filters {

void
SimpleFilter2::convert(std::string& message)
{
  message = "[Passed through filter 2] " + message;
}

} // namespace filters
} // namespace scpp
