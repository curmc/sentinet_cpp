/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : Session
 * @created     : Wednesday Oct 16, 2019 10:59:59 MDT
 */

#include "scpp/messages/pipe/ZMQPubSubProxy.hpp"
#include "scpp/messages/pipe/ZMQServerProxy.hpp"
#include "scpp/messages/pipe/SimpleFilter.hpp"

static std::vector<scpp::proxies::ZMQPubSubProxy*> publishers;
static std::vector<scpp::proxies::ZMQServerProxy*> servers;

int
create_server_proxy(const std::string& id, const std::string& frontend,
                    const std::string& backend)
{
  std::promise<void> futureobj;
  servers.push_back(new scpp::proxies::ZMQServerProxy(id, std::move(futureobj),
                                                      frontend, backend));
  return 1;
}
