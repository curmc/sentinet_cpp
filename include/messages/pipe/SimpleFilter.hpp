/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimpleFilter
 * @created     : Saturday Sep 14, 2019 18:24:03 MDT
 */

#ifndef SIMPLEFILTER_HPP

#define SIMPLEFILTER_HPP

// C++ includes

// Local Includes
#include "core/messages/pipe/FilterInterface.hpp"

class SimpleFilter : public FilterInterface 
{
    public:
      SimpleFilter () {};
      virtual ~SimpleFilter () = default;

      void convert(std::string& incomming_message) override;
};

#endif /* end of include guard SIMPLEFILTER_HPP */

