/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "scpp/kernel/KermitKernel.hpp"
#include <mutex>

#define INTERFACE "enp0s20f0u1"

static std::mutex guard;

namespace scpp {
namespace curmt {
KermitKernel::KermitKernel(const std::string& drive_topic,
                           const std::string& cmd_topic,
                           const std::string& data_topic,
                           const std::string& real_map_topic,
                           const bool verbose,
                           const bool debug)
  : kermit()
{

  // Initialize topics
  kermit.drive_topic = drive_topic;
  kermit.cmd_topic = cmd_topic;
  kermit.data_topic = data_topic;
  kermit.real_map_topic = real_map_topic;

  // Verbosity and debug mode
  kermit.verbose = verbose;
  kermit.debug = debug;

  // Create a new buffer
  message.cvel_buffer = create_cmd_vel();
  message.cvel_buffer.lin = 0.0;
  message.cvel_buffer.ang = 0.0;

  // Create a new data buffer
  message.data_buffer_temp = create_cmd_vel();

  // Create a new ping buffer
  message.ping = create_buffer_ping();
}

KermitKernel::~KermitKernel() { 
  if(teensy.sockfd)
    close(teensy.sockfd);
}

bool
KermitKernel::init_comms(const std::string& drive_addr,
                         const std::string& cmd_addr,
                         const std::string& data_addr,
                         const std::string& real_map_addr)
{

  // Create a new set of params

  // Set the addresses of proxies
  kermit.drive_addr = drive_addr;
  kermit.cmd_addr = cmd_addr;
  kermit.data_addr = data_addr;
  kermit.real_map_addr = real_map_addr;
  
  // The command velocity params for listening (a subscriber)
  params.cmd_vel_p.socket_backend = drive_addr;
  params.cmd_vel_p.topic = kermit.drive_topic;
  params.cmd_vel_p.callback =
    std::bind(&KermitKernel::drive_message_subscribe_callback,
              this,
              std::placeholders::_1);

  // The real time map params (a publisher)
  params.real_map_p.broker_frontend = real_map_addr;
  params.real_map_p.topic = kermit.real_map_topic;
  params.real_map_p.period = std::chrono::seconds(1); // TODO
  params.real_map_p.get_data =
    std::bind(&KermitKernel::map_message_get_data, this);

  // The data sender (a publisher)
  params.data_p.broker_frontend = data_addr;
  params.data_p.topic = kermit.data_topic;
  params.data_p.period = std::chrono::seconds(1);
  params.data_p.get_data =
    std::bind(&KermitKernel::data_message_get_data, this);

  // The command relay (a server)
  params.command_p.address = cmd_addr;
  params.command_p.callback =
    std::bind(&KermitKernel::cmd_message_callback, this, std::placeholders::_1);

  teensy.sockfd = 0;
  return true;
}

bool 
KermitKernel::init_teensy_peripheral(const std::string& ip_addr, int port){

  teensy.sockfd = socket(AF_INET, SOCK_STREAM, 0); 

  if(teensy.sockfd < 0) {
    perror("Socket ");
    kermit.debug = true;
    return false;
  }

  memset(&teensy.ifr, 0, sizeof(teensy.ifr));

  snprintf(teensy.ifr.ifr_name, sizeof(teensy.ifr.ifr_name), INTERFACE);

  if(setsockopt(teensy.sockfd, SOL_SOCKET, SO_BINDTODEVICE, (void*)&teensy.ifr, sizeof(teensy.ifr)) < 0) {
    perror("Interface ");
    kermit.debug = true;
    return false;
  }

  bzero(&teensy.dest, sizeof(teensy.dest));

  teensy.dest.sin_family = AF_INET;
  teensy.dest.sin_port = htons(port);

  if(inet_aton(ip_addr.c_str(), (in_addr*)&teensy.dest.sin_addr.s_addr) == 0) {
    perror(ip_addr.c_str());
    kermit.debug = true;
    return false;
  }

  if(connect(teensy.sockfd, (struct sockaddr*)&teensy.dest, sizeof(teensy.dest)) != 0) {
    perror("Connection");
    kermit.debug = true;
    return false;
  }

  return true;
}


void
KermitKernel::print_state()
{
  std::lock_guard<std::mutex> lock(guard);
  std::cout << "Linear = " << message.cvel_buffer.lin;
  std::cout << " Angular = " << message.cvel_buffer.ang << std::endl;
}

bool
KermitKernel::initialize_control_client()
{
  spin(params.cmd_vel_p);
  spin(params.real_map_p);
  spin(params.data_p);
  spin(params.command_p);
  return true;
}

bool
KermitKernel::start(const std::chrono::microseconds serial_period, const std::chrono::seconds time_alive)
{
  using namespace std::chrono;

  std::atomic<bool> infinite(false);
  if(time_alive == std::chrono::seconds(0)) {
    infinite = true;
  }

  initialize_control_client();
  steady_clock::time_point time_now = steady_clock::now();
  
  while(infinite || std::chrono::steady_clock::now() - time_now < time_alive) {
    if (kermit.verbose) {
      print_state();
    }
    if (!kermit.debug) {
      send_data();
    }
    usleep(serial_period.count());
  }
  return true;
}

bool
KermitKernel::send_data()
{
  // std::lock_guard<std::mutex> lock(guard);
  int16_t* temp = (int16_t*)teensy.send;
  *temp++ = message.cvel_buffer.lin;
  *temp = message.cvel_buffer.ang;
  
  teensy.send[4] = '\0';
  teensy.send[3] = '\0';

  std::cout<<"Writing"<<std::endl;
  write(teensy.sockfd, "hit\0", 4);
  read(teensy.sockfd, teensy.recv, 4);

  if(kermit.verbose) {
    std::cout<<"Responded with "<<*(int16_t*)teensy.recv<<" "<<*(int16_t*)(teensy.recv + 2)<<std::endl;
  }
  return true;
}

void
KermitKernel::drive_message_subscribe_callback(std::string& message_)
{
  std::lock_guard<std::mutex> lock(guard);
  cmd_vel_from_wire(&message.cvel_buffer, message_.c_str());
  return; // TODO
}

std::string
KermitKernel::cmd_message_callback(std::string& message_)
{
  std::cout<<"Recieved "<<message_<<std::endl;
  if(!serialize_from_ping(&message.ping, reinterpret_cast<BYTE*>(&message_[0]))) {
    message.ping.type = 5;
    message.ping.code = 6;
    message.ping.check = 9;
    to_wire_ping(&message.ping);
    return std::string(reinterpret_cast<char const*>(message.ping.data), PING_HEADER_SIZE);
  }
  return "Nooop" + message_;
}

std::string
KermitKernel::map_message_get_data(void)
{
  return "Not implimented ";
}

std::string
KermitKernel::data_message_get_data(void)
{
  // message.cvel_buffer.lin = 5.6;
  // message.cvel_buffer.ang = 10.5;
  cmd_vel_to_wire(&message.cvel_buffer);
  print_message_formatted(message.cvel_buffer.buff.data);
  return std::string((char const*)(message.cvel_buffer.buff.data), 27);
}

} // namespace curmt
} // namespace scpp
