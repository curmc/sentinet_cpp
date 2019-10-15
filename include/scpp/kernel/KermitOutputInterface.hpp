/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitOutputInterface
 * @created     : Sunday Oct 13, 2019 22:04:46 MDT
 */

#ifndef KERMITOUTPUTINTERFACE_HPP

#define KERMITOUTPUTINTERFACE_HPP

class KermitOutputInterface
{
public:
  virtual ~KermitOutputInterface() = default;

  virtual bool send_data(float lin, float ang) = 0;

private:
};

#endif /* end of include guard KERMITOUTPUTINTERFACE_HPP */
