/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : DebugMessage
 * @created     : Wednesday Jul 24, 2019 11:05:31 MDT
 */

// project includes
#include "messages/debug/DebugMessage.h"

DebugMessage::DebugMessage() {
  message = std::make_unique<debug::DebugMessage>();
}

DebugMessage::~DebugMessage() {}

std::string DebugMessage::print() const { return message->DebugString(); }

std::string DebugMessage::get_name() { return message->name(); }

int DebugMessage::get_id() { return message->id(); }

std::string DebugMessage::get_extra() { return message->extra(); }

void DebugMessage::set_name(const std::string &name) {
  message->set_name(name);
}

void DebugMessage::set_id(int32_t id) { message->set_id(id); }

void DebugMessage::set_extra(const std::string &extra) {
  message->set_extra(extra);
}

bool DebugMessage::serialize_to_string(std::string *output_string) const {
  message->SerializeToString(output_string);
  return true;
}

bool DebugMessage::parse_from_string(const std::string &value) {
  message->ParseFromString(value);
  return true;
}

bool DebugMessage::serialize_to_ostream(std::ostream *output) const {
  message->SerializeToOstream(output);
  return true;
}

bool DebugMessage::parse_from_istream(std::istream *input) {
  message->ParseFromIstream(input);
  return true;
}
