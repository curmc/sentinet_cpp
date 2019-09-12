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

// Local Includes
#include "core/messages/pipe/FilterInterface.hpp"

class PipeInterface {
  public:
    PipeInterface() {}
    virtual ~PipeInterface() = default;
  
  public:
    virtual bool create_pub_sub_endpoint(const std::string& id, const std::string& frontend, const std::string& backend) = 0;
    virtual bool create_req_rep_endpoint(const std::string& id, const std::string& frontend, const std::string& backend) = 0;
    virtual bool set_filter(const std::string& id, const FilterInterface& filter) = 0;
}

#endif /* end of include guard PIPEINTERFACE_HPP */

