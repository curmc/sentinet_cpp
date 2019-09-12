/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : Bluetooth
 * @created     : Saturday Aug 24, 2019 18:41:39 MDT
 */

#ifndef BLUETOOTH_HPP

#define BLUETOOTH_HPP

// C++ includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

class Bluetooth {
public:
  Bluetooth();
  virtual ~Bluetooth();

private:
  /* private data */
};

#endif /* end of include guard BLUETOOTH_HPP */
