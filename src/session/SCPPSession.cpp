/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : SCPPSession
 * @created     : Wednesday Oct 16, 2019 13:47:22 MDT
 */

#include "SCPPSession.hpp"

SCPPSession::SCPPSession()
{
  sess.starting_pub_port_b = 0;
  sess.starting_pub_port_f = 0;
  sess.starting_req_port = 0;
  sess.starting_serv_port = 0;
  sess.num_pub = 0;
  sess.num_serv = 0;
}

int
SCPPSession::parse_command_line(int argc, const char** argv)
{
  std::vector<std::string> args;

  int i = 1;
  while (i < argc)
    args.push_back(argv[i++]);

  for (i = 0; i < args.size(); ++i) {
    switch (args[i]) {
      case ("-f"): {
        // Set starting publisher port
        sess.starting_pub_port_f = (++i < args.size()) ? strtol(args[i]) : 5555;

        // Set number of publisher proxies if session.num_pub doesn't already
        // exist, otherwise 1
        sess.num_pub =
          (!sess.num_pub && ++i < args.size()) ? strtol(args[i]) : 0;

        // Either one of these is empty means invalid argument
        if (!sess.starting_pub_port_b)
          std::cout
            << "-f expected an integer port followed by number of ports\n";
        break;
      }

      case ("-b"): {
        // Set starting publisher port backend
        sess.starting_pub_port_b = ++i < args.size() ? strtol(args[i]) : 5555;

        // Set number of publishers if session.num_pub doesn't already exist,
        // otherwise 1
        sess.num_pub =
          (!sess.num_pub && ++i < args.size()) ? strtol(args[i]) : 0;

        // Either one of these is empty means invalid argument
        if (!sess.starting_pub_port_b)
          std::cout
            << "-b expected an integer port followed by number of ports\n";
        break;
      }
    }

    return i;
  }
}
