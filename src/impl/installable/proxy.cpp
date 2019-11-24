/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : proxy
 * @created     : Wednesday Oct 16, 2019 10:18:52 MDT
 */

#include "scpp/messages/pipe/ZMQPubSubProxy.hpp"
#include "scpp/messages/pipe/ZMQServerProxy.hpp"
#include "scpp/kernel/Defaults.hpp"

int
main(int argc, char* argv[])
{
  std::string frontend = scpp::defaults::frontend;
  std::string backend = scpp::defaults::backend;
  std::string server = ""; // scpp::defaults::server_frontend;
  std::string client = ""; // scpp::defaults::server_backend;
  std::string topic = "";

  std::vector<std::string> args;

  int i = 1;
  while (i < argc)
    args.push_back(argv[i++]);

  for (i = 0; i < args.size(); ++i) {
    if (args[i] == "-f")
      if (++i < args.size())
        frontend = "tcp://*:" + args[i];
      else
        std::cout << "-f expected an integer port" << std::endl;
    if (args[i] == "-b")
      if (++i < args.size())
        backend = "tcp://*:" + args[i];
      else
        std::cout << "-d expected an integer port" << std::endl;
    if (args[i] == "-sf")
      if (++i < args.size())
        server = "tcp://*:" + args[i];
      else
        std::cout << "-sf expected an integer port" << std::endl;
    if (args[i] == "-t")
      if (++i < args.size())
        topic = args[i];
      else
        std::cout << "-t a valid topic name" << std::endl;
    if (args[i] == "-sb")
      if (++i < args.size())
        client = "tcp://*:" + args[i];
      else
        std::cout << "-sb expected an integer port" << std::endl;
    if (args[i] == "-h")
      std::cout << "Usage: \n-f frontendport\n-b backendport\n-sf "
                   "serverfrontendport\n-sb serverbackendport"
                << std::endl;
  }

  std::promise<void> pubsub;
  std::promise<void> servereq;

  scpp::proxies::ZMQPubSubProxy a(
    topic, pubsub.get_future(), frontend, backend, 1);

  scpp::proxies::ZMQServerProxy b(
    topic, pubsub.get_future(), frontend, backend, 1);

  std::unique_ptr<std::thread> vb();

  std::thread va(
    &scpp::proxies::ZMQPubSubProxy::start, &a, std::chrono::microseconds(10));

  if (server != "" && client != "") {

    vb = std::make_unique<std::thread>(
      &scpp::proxies::ZMQPubSubProxy::start, &a, std::chrono::microseconds(10));
  }

  while (1) {
  }
  return 0;
}
