/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : MessageFactory
 * @created     : Friday Aug 23, 2019 16:22:35 MDT
 */

#include "messages/factory/MessageFactory.hpp"

MessageFactory::MessageFactory() {}

MessageFactory::~MessageFactory() {}

std::unique_ptr<DriveTrain>
MessageFactory::create_drive_train(const int32_t &lin, const int32_t &ang) {
  auto a = std::make_unique<DriveTrain>();
  a->set_linear(lin);
  a->set_angular(ang);
  return a;
}

std::unique_ptr<Camera> MessageFactory::create_camera(const int32_t &ang) {
  auto a = std::make_unique<Camera>();
  a->set_angular(ang);
  return a;
}

std::unique_ptr<Gun> MessageFactory::create_gun(const int32_t &left,
                                                const int32_t &right) {
  auto a = std::make_unique<Gun>();
  a->set_left_mag(left);
  a->set_right_mag(right);
  return a;
}
