/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : ZMQCC
 * @brief       : A brief description of the file
 *
 * A detailed description of the file
 *
 * @created     : Friday Dec 13, 2019 15:08:43 MST
 * @license     : MIT
 */

#ifndef ZMQCC_HPP

#define ZMQCC_HPP

#include "scpp/core/control/ControlClientInterface.hpp"
#include "scpp/core/types.h"
#include "scpp/control/zhelpers.hpp"
#include "scpp/core/utils/logging.hpp"

namespace scpp {
namespace net {

class ZMQCC : public ::scpp::core::ControlClientInterface
{
public:
  ZMQCC(int context_ = 1);
  virtual ~ZMQCC() = default;

  bool start(int i = 0) override;
  bool quit(int i = 0) override;

  bool initialize_publisher(const scpp::string& address) override;
  bool initialize_client() override;

public:
  bool publish(const scpp::string& topic, const scpp::string& message) override;

  bool publish(const scpp::string broker_frontend,
               const scpp::string topic,
               scpp::function<scpp::string(void)> get_data_to_publish,
               scpp::time::microseconds period) = override;

private:
  /* private data */
};

#endif /* end of include guard ZMQCC_HPP */
}
}
