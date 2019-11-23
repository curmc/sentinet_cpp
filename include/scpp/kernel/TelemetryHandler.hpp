/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : TelemetryHandler
 * @brief       : A brief description of the file
 *
 * A detailed description of the file
 *
 * @created     : Tuesday Oct 29, 2019 17:23:28 MDT
 * @license     : MIT
 */

#ifndef TELEMETRYHANDLER_HPP

#define TELEMETRYHANDLER_HPP

#include <curl/curl.h>
#include <stdio.h>

class TelemetryHandler
{
public:
  TelemetryHandler();
  virtual ~TelemetryHandler();

private:
  CURL* curl;
  CURLcode res;

  typedef struct
  {
  }
};

#endif /* end of include guard TELEMETRYHANDLER_HPP */
