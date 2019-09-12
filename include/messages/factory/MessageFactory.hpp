/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : MessageFactory
 * @created     : Friday Aug 23, 2019 16:19:13 MDT
 */

#ifndef MESSAGEFACTORY_HPP

#define MESSAGEFACTORY_HPP

#include "messages/common/CmdVel.hpp"
#include "messages/debug/DebugMessage.h"

class MessageFactory {
public:
  MessageFactory();
  virtual ~MessageFactory();

  virtual std::unique_ptr<DriveTrain> create_drive_train(const int32_t &lin,
                                                         const int32_t &ang);
  virtual std::unique_ptr<Camera> create_camera(const int32_t &ang);
  virtual std::unique_ptr<Gun> create_gun(const int32_t &lin,
                                          const int32_t &ang);
};

#endif /* end of include guard MESSAGEFACTORY_HPP */
