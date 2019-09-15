/**
 * @author      : theo (theo@varnsen)
 * @file        : MessagePipeInterface
 * @brief
 *
 *
 *
 * @created     : Thursday Sep 12, 2019 08:40:45 MDT
 * @bugs No Known Bugs
 */

#ifndef PIPEINTERFACE_HPP

#define PIPEINTERFACE_HPP

// C++ Includes
#include <string>
#include <memory>
#include <unordered_map>

// Local Includes
#include "core/messages/pipe/FilterInterface.hpp"
#include "core/messages/pipe/ProxyInterface.hpp"

namespace scpp {
namespace core {

class PipeInterface
{
public:
  PipeInterface();
  virtual ~PipeInterface() = default;

public:
  virtual bool create_pub_sub_endpoint(const std::string& id,
                                       const std::string& frontend,
                                       const std::string& backend,
                                       const bool start_on_creation = true);

  virtual bool create_req_rep_endpoint(const std::string& id,
                                       const std::string& frontend,
                                       const std::string& backend,
                                       const bool start_on_creation = true);

  virtual bool stop(const std::string& id);

  virtual bool kill(const std::string& id);

  virtual int signal(const std::string& id, const int32_t signal_val);

  virtual bool register_signal(const std::string& id,
                               const int32_t signal_val,
                               std::function<int(void)>);

  virtual bool set_filter(const std::string& id,
                          std::unique_ptr<FilterInterface> filter);

private:
  typedef struct proxy_s
  {
    std::promise<void> exit_signal;
    std::unique_ptr<ProxyInterface> proxy;
  } proxy_t;

  typedef struct
  {
    std::unordered_map<std::string, proxy_t> proxies;
  } PipeInterfaceProperties;

  PipeInterfaceProperties proxies;
};
} // namespace core
} // namespace scpp
#endif /* end of include guard PIPEINTERFACE_HPP */
