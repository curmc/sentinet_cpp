#ifndef HELPER_H
#define HELPER_H

namespace scpp {
namespace curmt {

/* Maintains that value is within bounds */
template <typename T>
constexpr T
clamp(const T& value, const T& min, const T& max)
{
  return (value > max) ? max : (value < min) ? min : value;
}

/* brings small numbers to zero instead of bouncing back and forth */
template <typename T>
constexpr T
zero_rounding(T value, T change)
{
  T d_value = value + change;
  if ((value > 0 && d_value < 0) || (value < 0 && d_value > 0))
    return 0;
  return value;
}

} // End Namespace
}
#endif
