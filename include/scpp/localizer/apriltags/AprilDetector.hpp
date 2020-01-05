/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : april_pos_estimator
 * @created     : Tuesday Dec 31, 2019 18:23:20 MST
 */

#ifndef APRIL_POS_ESTIMATOR_H

#define APRIL_POS_ESTIMATOR_H

#include "opencv2/opencv.hpp"
#include "scpp/localizer/PosLocalizer.hpp"

extern "C"
{
#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>
#include <apriltag/tag25h9.h>
#include <apriltag/tag16h5.h>
#include <apriltag/tagCircle21h7.h>
#include <apriltag/tagCircle49h12.h>
#include <apriltag/tagCustom48h12.h>
#include <apriltag/tagStandard41h12.h>
#include <apriltag/tagStandard52h13.h>
}

using namespace cv;

#define DEFAULT_TAG "tag36h11"

namespace scpp {
namespace curmt {

class AprilDetector : public scpp::curmt::PosLocalizer
{
public:
  AprilDetector() = delete;

  /**
   * @brief Default Constructor
   *
   * @param threads Asynchronous (0 means synchronous, anything above is
   * concurrent)
   * @param show_stream_ if true, shows the camera stream (imshow)
   * @param tag_type The april tag type
   */
  AprilDetector(const int threads = 0,
                bool show_stream_ = false,
                const std::string& tag_type = DEFAULT_TAG,
                bool verbose = true);

  // DEFAULT Destructor
  ~AprilDetector();

  bool sync_start();
  bool async_start();
  bool loop();

private:
  void destroy_video();

  void destroy_tag();
  bool initialize_tag(const std::string& type);

  struct
  {
    VideoCapture cap;
    Mat frame;
    Mat gray;
    bool show_stream;
  } video;

  struct
  {
    int type;
    apriltag_family_t* tf;
    apriltag_detector_t* td;
    zarray_t* detections;
  } tag = { 0, nullptr, nullptr, nullptr };

  const int multithreaded;
};

}
}

#endif /* end of include guard APRIL_POS_ESTIMATOR_H */
