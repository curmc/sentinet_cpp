/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : april_example
 * @created     : Wednesday Jan 01, 2020 13:28:39 MST
 */

#include "scpp/localizer/apriltags/AprilDetector.hpp"

using namespace scpp::curmt;

int
main()
{
  srand(time(NULL));

  AprilDetector det(0, true, "tag36h11", true);
  det.sync_start();

  while (true) {
    if (!det.loop())
      break;
  }
  return 0;
}
