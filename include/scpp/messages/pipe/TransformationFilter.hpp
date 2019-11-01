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

struct clean_message{
  int32_t positionx;
  int32_t positiony;
  float depth;
};

class TransformationFilter : public ::scpp::core::FilterInterface
{
<<<<<<< HEAD
public:
  TransformationFilter(){};
  virtual ~TransformationFilter() = default;

  void convert(std::string& incomming_message) override;

private:
  clean_message transform(raw_data& data);
=======
  public:
    TransformationFilter (){};
    virtual ~TransformationFilter () = default;
    void convert(int angleOfDeviationY, int angleOfDeviationX, int actualHypotenuse) override;
  private:
    clean_message transform(raw_data& data);
    clean_message values;

    
>>>>>>> Changes made for imu code hopefully this will work.
};

#endif /* end of include guard TRANSFORMATIONFILTER_HPP */
