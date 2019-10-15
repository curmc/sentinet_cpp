/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : PipeInterface
 * @created     : Saturday Sep 14, 2019 12:46:54 MDT
 */

#include "scpp/core/messages/pipe/PipeInterface.hpp"

#include <memory>
#include <future>
#include <string>

namespace scpp {
namespace core {

PipeInterface::PipeInterface() {}

bool
PipeInterface::stop()
{
  for (auto& a : proxies.proxies) {
    a.second.proxy->stop();
    if (a.second.t_space.joinable())
      a.second.t_space.join();
  }
  return true;
}

bool
PipeInterface::create_pub_sub_endpoint(const std::string& id,
                                       const std::string& frontend,
                                       const std::string& backend)
{

  auto found = proxies.proxies.find(id);
  if (found != proxies.proxies.end()) {
    LOG_ERROR(
      "Invalid request to build a pub sub proxy on id %s, already exists",
      id.c_str());
    return false;
  }

  proxy_t proxy;
  std::promise<void> futureObj;
  proxy.exit_signal = std::move(futureObj);

  proxy.proxy = std::make_unique<scpp::proxies::ZMQPubSubProxy>(
    id, proxy.exit_signal.get_future(), frontend, backend, 1);

  proxies.proxies[id] = std::move(proxy);
  proxies.proxies[id].t_space =
    std::thread(&scpp::proxies::ZMQPubSubProxy::start,
                proxies.proxies[id].proxy.get(),
                std::chrono::microseconds(10));

  return true;
}

bool
PipeInterface::create_req_rep_endpoint(const std::string& id,
                                       const std::string& frontend,
                                       const std::string& backend)
{

  auto found = proxies.proxies.find(id);
  if (found != proxies.proxies.end()) {
    LOG_ERROR(
      "Invalid request to build a pub sub proxy on id %s, already exists",
      id.c_str());
    return false;
  }

  proxy_t proxy;
  std::promise<void> futureObj;
  proxy.exit_signal = std::move(futureObj);

  proxy.proxy = std::make_unique<scpp::proxies::ZMQServerProxy>(
    id, proxy.exit_signal.get_future(), frontend, backend, 1);

  proxies.proxies[id] = std::move(proxy);
  proxies.proxies[id].t_space =
    std::thread(&scpp::proxies::ZMQPubSubProxy::start,
                proxies.proxies[id].proxy.get(),
                std::chrono::microseconds(10));

  return true;
}

bool
PipeInterface::stop(const std::string id)
{
  locker.lock();
  auto found = proxies.proxies.find(id);
  if (found != proxies.proxies.end()) {
    locker.unlock();
    return false;
  }

  proxies.proxies[id].proxy->stop();
  locker.unlock();
  return true;
}

bool
PipeInterface::kill(const std::string id)
{
  auto found = proxies.proxies.find(id);

  if (found != proxies.proxies.end()) {
    LOG_ERROR("Invalid proxy stop request on proxy %c", id.c_str());
    return false;
  }

  proxies.proxies[id].proxy->kill();
  return true;
}

int
PipeInterface::signal(const std::string id, const int32_t signal_val)
{
  auto found = proxies.proxies.find(id);

  if (found != proxies.proxies.end()) {
    LOG_ERROR("Invalid proxy stop request on proxy %c", id.c_str());
    return false;
  }

  proxies.proxies[id].proxy->signal(signal_val);
  return true;
}

bool
PipeInterface::register_signal(const std::string id,
                               const int32_t signal_val,
                               std::function<int(void)> func)
{

  auto found = proxies.proxies.find(id);

  if (found != proxies.proxies.end()) {
    LOG_ERROR("Invalid proxy stop request on proxy %c", id.c_str());
    return false;
  }

  proxies.proxies[id].proxy->register_signal(signal_val, func);
  return true;
}

bool
PipeInterface::set_filter(const std::string id,
                          std::unique_ptr<FilterInterface> filter)
{
  auto found = proxies.proxies.find(id);

  if (found != proxies.proxies.end()) {
    LOG_ERROR("Invalid proxy filter change request on proxy %c", id.c_str());
    return false;
  }

  proxies.proxies[id].proxy->add_filter(std::move(filter));
  return true;
}

}
}
