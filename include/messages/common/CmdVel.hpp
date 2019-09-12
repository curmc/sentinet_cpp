/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : DriveTrain
 * @created     : Friday Aug 23, 2019 15:59:12 MDT
 */

#ifndef CMDVEL_HPP

#define CMDVEL_HPP

// C++ includes
#include <iostream>
#include <memory>
#include <string>

// Local includes
#include "proto/common/cmd_vel.pb.h"

class DriveTrain {
public:
  DriveTrain();
  virtual ~DriveTrain();

  std::string print() const;
  int32_t get_angular();
  int32_t get_linear();

  void set_angular(const int32_t &ang);
  void set_linear(const int32_t &lin);

public:
  virtual bool serialize_to_string(std::string *output_string) const;
  virtual bool parse_from_string(const std::string &value);
  virtual bool serialize_to_ostream(std::ostream *output) const;
  virtual bool parse_from_istream(std::istream *input);

private:
  std::unique_ptr<movement::drive_train> message;

  /* private data */
};

class Camera {
public:
  Camera();
  virtual ~Camera();

  std::string print() const;
  int32_t get_angular();

  void set_angular(const int32_t &ang);

public:
  virtual bool serialize_to_string(std::string *output_string) const;
  virtual bool parse_from_string(const std::string &value);
  virtual bool serialize_to_ostream(std::ostream *output) const;
  virtual bool parse_from_istream(std::istream *input);

private:
  std::unique_ptr<movement::camera> message;

  /* private data */
};

class Gun {
public:
  Gun();
  virtual ~Gun();

  std::string print() const;

  int32_t get_left_mag();
  int32_t get_right_mag();

  void set_left_mag(const int32_t &ang);
  void set_right_mag(const int32_t &ang);

public:
  virtual bool serialize_to_string(std::string *output_string) const;
  virtual bool parse_from_string(const std::string &value);
  virtual bool serialize_to_ostream(std::ostream *output) const;
  virtual bool parse_from_istream(std::istream *input);

private:
  std::unique_ptr<movement::gun> message;

  /* private data */
};

#endif /* end of include guard CMDVEL_HPP */
