/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : DriveTrain
 * @created     : Friday Aug 23, 2019 16:05:46 MDT
 */

#include "messages/common/CmdVel.hpp"

DriveTrain::DriveTrain() {
  message = std::make_unique<movement::drive_train>();
}

DriveTrain::~DriveTrain() {}

std::string DriveTrain::print() const { return message->DebugString(); }

int32_t DriveTrain::get_angular() { return message->angular_vel(); }
int32_t DriveTrain::get_linear() { return message->linear_vel(); }

void DriveTrain::set_angular(const int32_t &ang) {
  message->set_angular_vel(ang);
}

void DriveTrain::set_linear(const int32_t &lin) {
  message->set_linear_vel(lin);
}

bool DriveTrain::serialize_to_string(std::string *output_string) const {
  message->SerializeToString(output_string);
  return true;
}

bool DriveTrain::parse_from_string(const std::string &value) {
  message->ParseFromString(value);
  return true;
}

bool DriveTrain::serialize_to_ostream(std::ostream *output) const {
  message->SerializeToOstream(output);
  return true;
}

bool DriveTrain::parse_from_istream(std::istream *input) {
  message->ParseFromIstream(input);
  return true;
}

Camera::Camera() { message = std::make_unique<movement::camera>(); }

Camera::~Camera() {}

std::string Camera::print() const { return message->DebugString(); }

int32_t Camera::get_angular() { return message->angular_vel(); }

void Camera::set_angular(const int32_t &ang) { message->set_angular_vel(ang); }

bool Camera::serialize_to_string(std::string *output_string) const {
  message->SerializeToString(output_string);
  return true;
}

bool Camera::parse_from_string(const std::string &value) {
  message->ParseFromString(value);
  return true;
}

bool Camera::serialize_to_ostream(std::ostream *output) const {
  message->SerializeToOstream(output);
  return true;
}

bool Camera::parse_from_istream(std::istream *input) {
  message->ParseFromIstream(input);
  return true;
}

Gun::Gun() { message = std::make_unique<movement::gun>(); }

Gun::~Gun() {}

std::string Gun::print() const { return message->DebugString(); }

int32_t Gun::get_left_mag() { return message->left_mag(); }
int32_t Gun::get_right_mag() { return message->right_mag(); }

void Gun::set_left_mag(const int32_t &ang) { message->set_left_mag(ang); }
void Gun::set_right_mag(const int32_t &ang) { message->set_right_mag(ang); }

bool Gun::serialize_to_string(std::string *output_string) const {
  message->SerializeToString(output_string);
  return true;
}

bool Gun::parse_from_string(const std::string &value) {
  message->ParseFromString(value);
  return true;
}

bool Gun::serialize_to_ostream(std::ostream *output) const {
  message->SerializeToOstream(output);
  return true;
}

bool Gun::parse_from_istream(std::istream *input) {
  message->ParseFromIstream(input);
  return true;
}
