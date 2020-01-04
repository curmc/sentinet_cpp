/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : AprilDetector
 * @created     : Wednesday Jan 01, 2020 11:33:21 MST
 */

#include "scpp/controllers/apriltags/AprilDetector.hpp"

namespace scpp {
namespace curmt {

AprilDetector::AprilDetector(const int threads,
                             bool show_stream_,
                             const std::string& tag_type,
                             bool verbose)
  : Localizer(verbose)
  , multithreaded(threads)
{
  initialize_tag(tag_type);
}

bool
AprilDetector::sync_start()
{
  video.cap = VideoCapture("/dev/video0");
  if (!video.cap.isOpened()) {
    LOG_ERROR("Couldnt open video device");
    return false;
  }

  localizer_start(std::chrono::seconds(1));
  return true;
}

bool
AprilDetector::async_start()
{
  return false;
}

bool
AprilDetector::loop()
{
  video.cap >> video.frame;
  cvtColor(video.frame, video.gray, COLOR_BGR2GRAY);

  image_u8_t image = { .width = video.gray.cols,
                       .height = video.gray.rows,
                       .stride = video.gray.cols,
                       .buf = video.gray.data };

  zarray_t* detections = apriltag_detector_detect(tag.td, &image);

  if (video.show_stream) {
    for (int i = 0; i < zarray_size(detections); ++i) {
      apriltag_detection_t* det;
      zarray_get(detections, i, &det);
      line(video.frame,
           Point(det->p[0][0], det->p[0][1]),
           Point(det->p[1][0], det->p[1][1]),
           Scalar(0, 0xff, 0),
           2);
      line(video.frame,
           Point(det->p[0][0], det->p[0][1]),
           Point(det->p[3][0], det->p[3][1]),
           Scalar(0, 0, 0xff),
           2);
      line(video.frame,
           Point(det->p[1][0], det->p[1][1]),
           Point(det->p[2][0], det->p[2][1]),
           Scalar(0xff, 0, 0),
           2);
      line(video.frame,
           Point(det->p[2][0], det->p[2][1]),
           Point(det->p[3][0], det->p[3][1]),
           Scalar(0xff, 0, 0),
           2);

      std::stringstream ss;
      ss << det->id;
      String text = ss.str();
      int fontface = FONT_HERSHEY_SCRIPT_SIMPLEX;
      double fontscale = 1.0;
      int baseline;
      Size textsize = getTextSize(text, fontface, fontscale, 2, &baseline);
      putText(
        video.frame,
        text,
        Point(det->c[0] - textsize.width / 2, det->c[1] + textsize.height / 2),
        fontface,
        fontscale,
        Scalar(0xff, 0x99, 0),
        2);
    }
  }

  apriltag_detections_destroy(detections);

  if (video.show_stream) {
    imshow("Tag Detections", video.frame);
    if (waitKey(30) >= 0)
      return false;
  }

  if (verbose)
    print_state();

  set_pos(4.5, 5.6, 7.8, 9.1);

  return true;
}

AprilDetector::~AprilDetector()
{
  localizer_quit();
  destroy_tag();
  destroy_video();
}

void
AprilDetector::destroy_video()
{
  return;
}

void
AprilDetector::destroy_tag()
{
  if (tag.detections)
    apriltag_detections_destroy(tag.detections);
  if (tag.td)
    apriltag_detector_destroy(tag.td);

  tag.detections = nullptr;
  tag.td = nullptr;

  if (!tag.tf) {
    tag.type = 0;
    return;
  }

  switch (tag.type) {
    case (0):
      break;
    case (1):
      tag36h11_destroy(tag.tf);
      break;
    case (2):
      tag25h9_destroy(tag.tf);
      break;
    case (3):
      tag16h5_destroy(tag.tf);
      break;
    case (4):
      tagCircle21h7_destroy(tag.tf);
      break;
    case (5):
      tagCircle49h12_destroy(tag.tf);
      break;
    case (6):
      tagStandard41h12_destroy(tag.tf);
      break;
    case (7):
      tagStandard52h13_destroy(tag.tf);
      break;
    case (8):
      tagCustom48h12_destroy(tag.tf);
      break;
  }

  tag.tf = nullptr;
  tag.type = 0;
}

bool
AprilDetector::initialize_tag(const std::string& type)
{
  if (type == "tag36h11") {
    tag.tf = tag36h11_create();
    tag.type = 1;
  } else if (type == "tag25h9") {
    tag.tf = tag25h9_create();
    tag.type = 2;
  } else if (type == "tag16h5") {
    tag.tf = tag16h5_create();
    tag.type = 3;
  } else if (type == "tagCircle21h7") {
    tag.tf = tagCircle21h7_create();
    tag.type = 4;
  } else if (type == "tagCircle49h12") {
    tag.tf = tagCircle49h12_create();
    tag.type = 5;
  } else if (type == "tagStandard41h12") {
    tag.tf = tagStandard41h12_create();
    tag.type = 6;
  } else if (type == "tagStandard52h13") {
    tag.tf = tagStandard52h13_create();
    tag.type = 7;
  } else if (type == "tagCustom48h12") {
    tag.tf = tagCustom48h12_create();
    tag.type = 8;
  } else {
    LOG_ERROR("Error invalid tag type: %s", type.c_str());
    return false;
  }

  tag.td = apriltag_detector_create();
  apriltag_detector_add_family(tag.td, tag.tf);

  tag.td->quad_decimate = 2.0;
  tag.td->quad_sigma = 0.0;
  tag.td->debug = 0; // video.show_stream;
  tag.td->refine_edges = 1;

  // if(!multithreaded)
  //   tag.td->nthreads = multithreaded;
  // else
  tag.td->nthreads = 1;

  return true;
}

}
}
