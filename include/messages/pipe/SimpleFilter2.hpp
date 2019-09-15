/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimpleFilter2
 * @created     : Saturday Sep 14, 2019 18:24:03 MDT
 */

#ifndef SIMPLEFILTER2_HPP

#define SIMPLEFILTER2_HPP

// C++ includes

// Local Includes
#include "core/messages/pipe/FilterInterface.hpp"

class SimpleFilter2 : public FilterInterface 
{
    public:
      SimpleFilter2 () {};
      virtual ~SimpleFilter2 () = default;

      void convert(std::string& incomming_message) override;
};

#endif /* end of include guard SIMPLEFILTER2_HPP */

