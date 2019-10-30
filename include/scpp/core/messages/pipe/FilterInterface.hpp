/**
 * @author      : theo (theo@varnsen)
 * @file        : FilterInterface
 * @brief
 *
 *
 *
 * @created     : Thursday Sep 12, 2019 08:46:40 MDT
 * @bugs No Known Bugs
 */

#ifndef FILTERINTERFACE_HPP

#define FILTERINTERFACE_HPP

// C++ Includes
#include <string>

// Local Includes

namespace scpp {
namespace core {

class FilterInterface
{
public:
  FilterInterface(){};
  virtual ~FilterInterface() = default;

  /**
   * @brief mutable string alteratiopn
   *
   * @param incomming_message a muttable string
   */
  virtual clean_message convert(int angleOfDeviationY, int angleOfDeviationX, int actualHypotenuse) = 0;
};

} // namespace core
} // namespace scpp
#endif /* end of include guard FILTERINTERFACE_HPP */
