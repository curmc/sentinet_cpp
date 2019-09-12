/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : Socket
 * @created     : Saturday Aug 24, 2019 18:46:51 MDT
 */

#ifndef SOCKET_HPP

#define SOCKET_HPP

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string>
#include <memory>

class Socket {
public:
  Socket();
  virtual ~Socket();

  // https://www.gnu.org/software/libc/manual/html_node/Creating-a-Socket.html#Creating-a-Socket
  bool open();
  // https://www.gnu.org/software/libc/manual/html_node/Setting-Address.html#Setting-Address
  bool close(int how = 0);

  // https://www.gnu.org/software/libc/manual/html_node/Listening.html#Listening
  bool listen(int queue_size);
  // https://www.gnu.org/software/libc/manual/html_node/Connecting.html#Connecting
  bool connect(const std::string &address);

private:
  /**
   * Options include:
   * SOCK_STREAM -> TCP
   * SOCK_DGRAM -> UDP
   * SOCK_RAW -> Raw (no need usually)
   * https://www.gnu.org/software/libc/manual/html_node/Communication-Styles.html#Communication-Styles
   */
  bool set_sock_type(const int &sock_type);

  /**
   * Options include:
   * AF_LOCAL ->
   * https://www.gnu.org/software/libc/manual/html_node/Local-Namespace-Details.html#Local-Namespace-Details
   * AF_UNIX -> same as AF_LOCAL
   * AF_FILE -> same as AF_LOCAL
   *
   * MAIN:
   * AF_INET6 -> ipv6
   * AF_INET -> ipv4
   *
   * AF_UNSPEC -> no address format
   * https://www.gnu.org/software/libc/manual/html_node/Address-Formats.html#Address-Formats
   */
  bool set_sock_domain(const int &sock_domain);

  /**
   * https://www.gnu.org/software/libc/manual/html_node/Socket-Addresses.html#Socket-Addresses
   * https://www.gnu.org/software/libc/manual/html_node/Setting-Address.html#Setting-Address
   */
  bool set_sock_address(const std::string &addres);
  bool set_sock_address(const sockaddr &addr); // copy a socket address
  bool set_sock_port(int port);

private:
  typedef struct SocketMeta {
    int domain;
    int type;
    int protocol;
    int port;
  } SocketMeta;

  typedef struct SocketProperties {
    SocketProperties() = delete;
    int sockfd;
    sockaddr address;
    socklen_t addrlen;
  } SocketProperties;
};

#endif /* end of include guard SOCKET_HPP */
