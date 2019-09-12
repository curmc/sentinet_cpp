/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : Telemetry
 * @created     : Friday Aug 23, 2019 18:54:10 MDT
 */

#ifndef TELEMETRY_HPP

#define TELEMETRY_HPP

// C++ includes
#include <memory>
#include <iostream>
#include <string>

// Local includes
#include "proto/telemetry/telemetry.pb.h"

class Telemetry {
public:
  Telemetry();
  virtual ~Telemetry();

  std::string print() const;

  int32_t get_time_alive();
  std::string get_description();

  enum endpoint { PUBLISHERS, SUBSCRIBERS, CLIENTS, SERVERS };

  int32_t get_number_endpoints(endpoint end);
  int32_t get_number_messages(endpoint end);

public:
  virtual bool serialize_to_string(std::string *output_string);
  virtual bool parse_from_string(const std::string &value);
  virtual bool serialize_from_ostream(std::ostream *output);
  virtual bool parse_from_istream(std::istream *input);

private:
  std::unique_ptr<telemetry::telemetry> message;
};

#endif /* end of include guard TELEMETRY_HPP */
