/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : DebugMessage
 * @created     : Wednesday Jul 24, 2019 14:30:28 MDT
 */

#ifndef DEBUGMESSAGE_HPP

#define DEBUGMESSAGE_HPP

// Project Includes
#include "proto/debug/debug.pb.h"

// C++ Includes
#include <iostream>
#include <memory>
#include <string>

class DebugMessage {
public:
  DebugMessage();
  virtual ~DebugMessage();

  std::string print() const;
  std::string get_name();
  int get_id();
  std::string get_extra();

  void set_name(const std::string &);
  void set_id(int32_t);
  void set_extra(const std::string &extra);

public:
  virtual bool serialize_to_string(std::string *output_string) const;
  virtual bool parse_from_string(const std::string &value);
  virtual bool serialize_to_ostream(std::ostream *ouput) const;
  virtual bool parse_from_istream(std::istream *input);

private:
  std::unique_ptr<debug::DebugMessage> message;
  /* private data */
};

#endif /* end of include guard DEBUGMESSAGE_HPP */
