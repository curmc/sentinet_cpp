/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : TransformationFilter
 * @brief       : A brief description of the file
 *
 * A detailed description of the file
 *
 * @created     : Friday Oct 18, 2019 20:34:07 MDT
 * @license     : MIT
 */

#ifndef TRANSFORMATIONFILTER_HPP

#define TRANSFORMATIONFILTER_HPP

// C++ includes
#include <cmath>

// Local includes
#include "scpp/core/messages/pipe/FilterInterface.hpp"

struct clean_message
{
  uint8_t possitionx;
  uint8_t possitiony;
  float depth;
};

struct raw_data
{
  // ???  Sensor values?
  int distance_sensed;
};

class TransformationFilter : public ::scpp::core::FilterInterface
{
public:
  TransformationFilter(){};
  virtual ~TransformationFilter() = default;

  void convert(std::string& incomming_message) override;

private:
  clean_message transform(raw_data& data);
};

#endif /* end of include guard TRANSFORMATIONFILTER_HPP */
