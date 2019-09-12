/**
 * @author      : theo (theo@varnsen)
 * @file        : ZMQMessagePipe
 * @brief 
 *
 *
 *
 * @created     : Thursday Sep 12, 2019 08:54:52 MDT
 * @bugs No Known Bugs
 */

#ifndef ZMQMESSAGEPIPE_HPP

#define ZMQMESSAGEPIPE_HPP

// C++ Includes

// Local Includes
#include "core/messages/pipe/PipeInterface.hpp"

class ZMQPipe {
  public:
    ZMQPipe() {}
    ~ZMQPipe() {}

  public:
    bool create_pub_sub_endpoint(const std::string& id, const std::string& frontend, const std::string& backend) override;
    bool create_req_rep_endpoint(const std::string& id, const std::string& frontend, const std::string& backend) override;

    bool set_filter(const std::string& id, std::unique_ptr<FilterInterface> filter) override;

    /**
     * @brief Does a two way signal handler, ideally the child process
     * will send a signal back that means it has cleanly exited
     * 
     * @param id The id of the message pipe to kill
     *
     * @return 
     */
    bool kill_cleanly(const std::string& id);
    void kill(const std::string& id);

  private:
  
    typedef std::function<bool(std::unique_ptr<FilterInterface>)> filter_setter;
    typedef std::function<bool(const int32_t)> signal_setter;
    
    struct process_function_table {
      int32_t pid;
      filter_setter set_filter;
      signal_setter set_signal;
    };

    struct management {
      // table of message pipes
      std::map<const std::string, process_function_table> process_table;
      void send_signal(const std::string& id, int32_t signal_val);
    };
}



#endif /* end of include guard ZMQMESSAGEPIPE_HPP */

