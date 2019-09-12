/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : InputOutputInterface
 * @created     : Sunday Aug 25, 2019 14:56:39 MDT
 */

#ifndef INPUTOUTPUTINTERFACE_HPP

#define INPUTOUTPUTINTERFACE_HPP

// C++ includes
#include <string>

/**
 * @brief I think for the future I want to make this more complex. So an io
 * interface needs to also have an allocated buffer, that way, the way we handle
 * memory is the same across all interfaces. But for now, it's just a container
 * so  that input output interfaces can be abstracted in the future
 */
class InputOutputInterface {
public:
  virtual ~InputOutputInterface() = default;

  virtual bool write(const std::string &data) = 0;
  virtual bool write(const std::string &data, int bytes) = 0;

  virtual std::string read(int &num_bytes) = 0;
};

#endif /* end of include guard INPUTOUTPUTINTERFACE_HPP */
