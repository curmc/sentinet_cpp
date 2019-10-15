/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimpleFilter
 * @created     : Saturday Sep 14, 2019 18:24:03 MDT
 */

#ifndef SIMPLEFILTER_HPP

#define SIMPLEFILTER_HPP

// C++ includes

// Local Includes
#include "scpp/core/messages/pipe/FilterInterface.hpp"

namespace scpp {
namespace filters {

class SimpleFilter : public ::scpp::core::FilterInterface
{
public:
  SimpleFilter(){};
  virtual ~SimpleFilter() = default;

  void convert(std::string& incomming_message) override;
};
} // namespace filters
} // namespace scpp

#endif /* end of include guard SIMPLEFILTER_HPP */
