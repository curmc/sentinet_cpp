/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimpleNumberFilter
 * @created     : Friday Sep 13, 2019 16:16:43 MDT
 */

#include "messages/pipe/SimpleNumberFilter.hpp"

namespace messages {
namespace pipes {

SimpleNumberFilter::SimpleNumberFilter()
{
  message = std::make_unique<::SimpleNumber>();
  message->set_num1(0);
  message->set_num2(0);
}

std::string SimpleNumberFilter::convert(const std::string& incomming_message) {
  // Parse from the incomming message
  message->parse_from_string(incomming_message);

  // Set the new values
  message->set_num1(message->get_num1() + message->get_num2());
  message->set_num2(0);

  // Serialize our return string
  message->serialize_to_string(&message_str);
  return message_str;
}

}
}
