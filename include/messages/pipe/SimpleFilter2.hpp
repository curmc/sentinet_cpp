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

namespace scpp {
namespace filters {

class SimpleFilter2 : public ::scpp::core::FilterInterface
{
public:
  SimpleFilter2(){};
  virtual ~SimpleFilter2() = default;

  void convert(std::string& incomming_message) override;
};
} // namespace filters
} // namespace scpp

#endif /* end of include guard SIMPLEFILTER2_HPP */
