/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : Telemetry
 * @created     : Friday Aug 23, 2019 19:11:24 MDT
 */

#include "messages/telemetry/Telemetry.hpp"

Telemetry::Telemetry() { message = std::make_unique<telemetry::telemetry>(); }

Telemetry::~Telemetry() {}

std::string Telemetry::print() const { return message->DebugString(); }

int32_t Telemetry::get_time_alive() { return message->time_alive(); }

std::string Telemetry::get_description() { return message->description(); }

int32_t Telemetry::get_number_endpoints(endpoint end) {
  switch (end) {
  case PUBLISHERS:
    return message->publishers().num_endpoints();
  case SUBSCRIBERS:
    return message->subscribers().num_endpoints();
  case CLIENTS:
    return message->clients().num_endpoints();
  case SERVERS:
    return message->servers().num_endpoints();
  }
  return -1;
}

int32_t Telemetry::get_number_messages(endpoint end) {
  switch (end) {
  case PUBLISHERS:
    return message->publishers().num_messages();
  case SUBSCRIBERS:
    return message->subscribers().num_messages();
  case CLIENTS:
    return message->clients().num_messages();
  case SERVERS:
    return message->servers().num_messages();
  }
  return -1;
}

bool Telemetry::serialize_to_string(std::string *output_string) {
  message->SerializeToString(output_string);
  return true;
}

bool Telemetry::parse_from_string(const std::string &value) {
  message->ParseFromString(value);
  return true;
}

bool Telemetry::serialize_from_ostream(std::ostream *output) {
  message->SerializeToOstream(output);
  return true;
}

bool Telemetry::parse_from_istream(std::istream *input) {
  message->ParseFromIstream(input);
  return true;
}
