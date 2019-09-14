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

class FilterInterface {
public:
  FilterInterface(){};
  virtual ~FilterInterface() = default;

  /**
   * @brief mutable string alteratiopn
   *
   * @param incomming_message a muttable string
   */
  virtual void convert(std::string &incomming_message) = 0;
};

#endif /* end of include guard FILTERINTERFACE_HPP */
