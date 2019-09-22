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
#include "messages/pipe/ZMQPubSubProxy.hpp"
#include "messages/pipe/ZMQServerProxy.hpp"

namespace scpp {
namespace core {

class PipeInterface
{
public:
  PipeInterface();
  virtual ~PipeInterface() = default;

public:

  /**
   * @brief Create a publisher subscribe proxy
   */
  virtual bool create_pub_sub_endpoint(const std::string& id,
                                       const std::string& frontend,
                                       const std::string& backend);

  /**
   * @brief Create a req rep proxy
   */
  virtual bool create_req_rep_endpoint(const std::string& id,
                                       const std::string& frontend,
                                       const std::string& backend);

  /**
   * Stop the whole thing
   */
  virtual bool stop();

  /**
   * Stop an id
   */
  virtual bool stop(const std::string id);

  /** 
   * Hopefuly we don't use this, but kill a thing
   */
  virtual bool kill(const std::string id);

  /**
   * Things also have the ability to send signals
   */
  virtual int signal(const std::string id, const int32_t signal_val);

  /**
   * Things also have the ability to register signals
   */
  virtual bool register_signal(const std::string id,
                               const int32_t signal_val,
                               std::function<int(void)>);

  /**
   * Add a filter to a proxy
   */
  virtual bool set_filter(const std::string id,
                          std::unique_ptr<FilterInterface> filter);


private:
  typedef struct proxy_s
  {
    std::promise<void> exit_signal;
    std::unique_ptr<ProxyInterface> proxy;
    std::thread t_space;
  } proxy_t;

  typedef struct
  {
    std::unordered_map<std::string, proxy_t> proxies;
  } PipeInterfaceProperties;

  PipeInterfaceProperties proxies;
  std::mutex locker;
};
} // namespace core
} // namespace scpp
#endif /* end of include guard PIPEINTERFACE_HPP */
