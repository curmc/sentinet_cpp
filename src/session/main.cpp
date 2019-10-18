/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : main
 * @created     : Thursday Oct 17, 2019 12:50:10 MDT
 */

#include "SCPPSession.hpp"

int main(int argc, const char** argv) {
  SCPPSession sess;

  sess.parse_command_line(argc, argv);

  std::cout<<sess.sess.num_pub<<" "<<sess.sess.num_serv<<" "<<sess.sess.starting_pub_port_b<<" "<<sess.sess.starting_pub_port_f;
}
