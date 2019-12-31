/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : april
 *
 * The driver for april tags and post request detection
 *
 * @created     : Sunday Oct 27, 2019 17:37:08 MDT
 */

// C++ Includes
#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#include <iostream>

// April tags includes
extern "C"
{
#include <apriltag/apriltag.h>
#include <apriltag/common/getopt.h>
#include <apriltag/tag16h5.h>
#include <apriltag/tag25h9.h>
#include <apriltag/tag36h11.h>
#include <apriltag/tagCircle21h7.h>
#include <apriltag/tagCircle49h12.h>
#include <apriltag/tagCustom48h12.h>
#include <apriltag/tagStandard41h12.h>
#include <apriltag/tagStandard52h13.h>
}

int
main(int argc, char* argv[])
{
  getopt_t* getopt = getopt_create();

  getopt_add_bool(getopt, 'h', "help", 0, "Show this help");
  getopt_add_bool(getopt, 'd', "debug", 1, "Enable debugging output (slow)");
  getopt_add_bool(getopt, 'q', "quiet", 0, "Reduce output");
  getopt_add_string(getopt, 'f', "family", "tag36h11", "Tag family to use");
  getopt_add_int(getopt, 't', "threads", "1", "Use this many CPU threads");
  getopt_add_double(
    getopt, 'x', "decimate", "2.0", "Decimate input image by this factor");
  getopt_add_double(getopt, 'b', "blur", "0.0", "Apply low-pass blur to input");
  getopt_add_bool(getopt,
                  '0',
                  "refine-edges",
                  1,
                  "Spend more time trying to align edges of tags");

  if (!getopt_parse(getopt, argc, argv, 1) || getopt_get_bool(getopt, "help")) {
    printf("Usage: %s [options]\n", argv[0]);
    getopt_do_usage(getopt);
    exit(0);
  }

  // Initialize camera
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "Couldn't open video capture device" << std::endl;
    return -1;
  }

  // Initialize tag detector with options
  apriltag_family_t* tf = NULL;
  const char* famname = getopt_get_string(getopt, "family");
  if (!strcmp(famname, "tag36h11")) {
    tf = tag36h11_create();
  } else if (!strcmp(famname, "tag25h9")) {
    tf = tag25h9_create();
  } else if (!strcmp(famname, "tag16h5")) {
    tf = tag16h5_create();
  } else if (!strcmp(famname, "tagCircle21h7")) {
    tf = tagCircle21h7_create();
  } else if (!strcmp(famname, "tagCircle49h12")) {
    tf = tagCircle49h12_create();
  } else if (!strcmp(famname, "tagStandard41h12")) {
    tf = tagStandard41h12_create();
  } else if (!strcmp(famname, "tagStandard52h13")) {
    tf = tagStandard52h13_create();
  } else if (!strcmp(famname, "tagCustom48h12")) {
    tf = tagCustom48h12_create();
  } else {
    printf("Unrecognized tag family name. Use e.g. \"tag36h11\".\n");
    exit(-1);
  }

  apriltag_detector_t* td = apriltag_detector_create();
  apriltag_detector_add_family(td, tf);
  td->quad_decimate = getopt_get_double(getopt, "decimate");
  td->quad_sigma = getopt_get_double(getopt, "blur");
  td->nthreads = getopt_get_int(getopt, "threads");
  td->debug = getopt_get_bool(getopt, "debug");
  td->refine_edges = getopt_get_bool(getopt, "refine-edges");

  int detecting = 0;
  cv::Mat frame, gray;
  while (true) {
    cap >> frame;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Make an image_u8_t header for the cv::Mat data
    image_u8_t im = { .width = gray.cols,
                      .height = gray.rows,
                      .stride = gray.cols,
                      .buf = gray.data };

    zarray_t* detections = apriltag_detector_detect(td, &im);
    auto num_detects = zarray_size(detections);

    std::cout << num_detects << " tags detected" << std::endl;
    int temp = detecting;
    if (num_detects > 0) {
      detecting = 1;
    } else {
      detecting = 0;
    }

    if (temp == 1 && detecting == 0) {
      char dest[90];
      tag_point_to_post_request(&tag, dest);
      update_tag_point_timer(&tag);

      if (curl) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dest);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
          fprintf(stderr,
                  "curl easy perform failed : %s \n",
                  curl_easy_strerror(res));
          break;
        }
      } else {
        break;
      }

      printf("%s\n", dest);
    }

    // Draw detection outlines
    for (int i = 0; i < zarray_size(detections); i++) {
      apriltag_detection_t* det;
      zarray_get(detections, i, &det);
      line(frame,
           cv::Point(det->p[0][0], det->p[0][1]),
           cv::Point(det->p[1][0], det->p[1][1]),
           cv::Scalar(0, 0xff, 0),
           2);
      line(frame,
           cv::Point(det->p[0][0], det->p[0][1]),
           cv::Point(det->p[3][0], det->p[3][1]),
           cv::Scalar(0, 0, 0xff),
           2);
      line(frame,
           cv::Point(det->p[1][0], det->p[1][1]),
           cv::Point(det->p[2][0], det->p[2][1]),
           cv::Scalar(0xff, 0, 0),
           2);
      line(frame,
           cv::Point(det->p[2][0], det->p[2][1]),
           cv::Point(det->p[3][0], det->p[3][1]),
           cv::Scalar(0xff, 0, 0),
           2);

      std::stringstream ss;
      ss << det->id;
      cv::String text = ss.str();
      int fontface = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
      double fontscale = 1.0;
      int baseline;
      cv::Size textsize =
        cv::getTextSize(text, fontface, fontscale, 2, &baseline);
      cv::putText(frame,
                  text,
                  cv::Point(det->c[0] - textsize.width / 2,
                            det->c[1] + textsize.height / 2),
                  fontface,
                  fontscale,
                  cv::Scalar(0xff, 0x99, 0),
                  2);
    }
    apriltag_detections_destroy(detections);

    cv::imshow("Tag Detections", frame);
    if (cv::waitKey(30) >= 0)
      break;
  }
  if (curl)
    curl_easy_cleanup(curl);
  curl_global_cleanup();

  apriltag_detector_destroy(td);

  if (!strcmp(famname, "tag36h11")) {
    tag36h11_destroy(tf);
  } else if (!strcmp(famname, "tag25h9")) {
    tag25h9_destroy(tf);
  } else if (!strcmp(famname, "tag16h5")) {
    tag16h5_destroy(tf);
  } else if (!strcmp(famname, "tagCircle21h7")) {
    tagCircle21h7_destroy(tf);
  } else if (!strcmp(famname, "tagCircle49h12")) {
    tagCircle49h12_destroy(tf);
  } else if (!strcmp(famname, "tagStandard41h12")) {
    tagStandard41h12_destroy(tf);
  } else if (!strcmp(famname, "tagStandard52h13")) {
    tagStandard52h13_destroy(tf);
  } else if (!strcmp(famname, "tagCustom48h12")) {
    tagCustom48h12_destroy(tf);
  }

  getopt_destroy(getopt);

  return 0;
}
