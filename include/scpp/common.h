/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : common
 * @created     : Thursday Jan 02, 2020 16:03:42 MST
 */

#ifndef COMMON_H

#define COMMON_H


#include <memory>
#include <chrono>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <thread>

#include <cstdio>
#include <cstdarg>
#include <cstring>


////////////////////////// ADDRESS STUFF ///////////////
namespace addr {
const std::string camera_topic = "camera";
const std::string cmd_vel_topic = "cmd_vel";

// KERNEL ADDRESSES
namespace kernel {
const int CAMERA = 5568;
const int CMD_VEL = 5570;
const int CONTROL_KERNEL = 5572;
const int KERNEL_LOCALIZER = 5574;
}

// LOCALIZER ADDRESSES
namespace localizer {
const int KERNEL_LOCALIZER = 5576;
const int LOCALIZER_CONTROL = 5578;
}

// CONTROLLER ADDRESSES
namespace controller {
const int LOCALIZER_CONTROL = 5580;
const int CONTROL_KERNEL = 5582;
}
}

std::string to_bind_addr(const int port);
std::string to_conn_addr(const int port);



//////////////////// LOGGING /////////////////////
#define LOG_VERSION "0.1.0"
typedef void (*log_LockFn)(void* udata, int lock);

enum
{
  LLOG_TRACE,
  LLOG_DEBUG,
  LLOG_INFO,
  LLOG_WARN,
  LLOG_ERROR,
  LLOG_FATAL
};

#define LOG_TRACE(...) log_log(LLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) log_log(LLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) log_log(LLOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) log_log(LLOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) log_log(LLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LLOG_FATAL(...) log_log(LLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)


static void
log_log(int level, const char* file, int line, const char* fmt, ...);

static struct
{
  void* udata;
  log_LockFn lock;
  FILE* fp;
  int level;
  int quiet;
} L;

static const char* level_names[] = { "TRACE", "DEBUG", "INFO",
                                     "WARN",  "ERROR", "FATAL" };

#ifdef LOG_USE_COLOR
static const char* level_colors[] = { "\x1b[94m", "\x1b[36m", "\x1b[32m",
                                      "\x1b[33m", "\x1b[31m", "\x1b[35m" };
#endif

static void
lock(void)
{
  if (L.lock) {
    L.lock(L.udata, 1);
  }
}

static void
unlock(void)
{
  if (L.lock) {
    L.lock(L.udata, 0);
  }
}

static void
log_set_udata(void* udata)
{
  L.udata = udata;
}

static void
log_set_lock(log_LockFn fn)
{
  L.lock = fn;
}

static void
log_set_fp(FILE* fp)
{
  L.fp = fp;
}

static void
log_set_level(int level)
{
  L.level = level;
}

static void
log_set_quiet(int enable)
{
  L.quiet = enable ? 1 : 0;
}

static void
log_log(int level, const char* file_, int line, const char* fmt, ...)
{
  char file[20];
  int index = strlen(file_) - 1;
  int copy_index;
  int num_slashes = 0;
  int i;
  int num_backwards = 1;

  for (i = index; num_slashes < num_backwards && i >= 0; --i)
    if (file_[i] == '/')
      num_slashes++;
  copy_index = i;
  memcpy(file, &file_[copy_index + 1], index - copy_index);
  file[index - copy_index] = '\0';

  if (level < L.level) {
    return;
  }
  // const char* file = ::utils::fs::split_file_name(file_, 3).c_str();

  /* Acquire lock */
  lock();

  /* Get current time */
  time_t t = time(NULL);
  struct tm* lt = localtime(&t);

  /* Log to stderr */
  if (!L.quiet) {
    va_list args;
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
#ifdef LOG_USE_COLOR
    fprintf(stderr,
            "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
            buf,
            level_colors[level],
            level_names[level],
            file,
            line);
#else
    fprintf(stderr, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
#endif
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
  }

  /* Log to file */
  if (L.fp) {
    va_list args;
    char buf[32];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
    fprintf(L.fp, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
    va_start(args, fmt);
    vfprintf(L.fp, fmt, args);
    va_end(args);
    fprintf(L.fp, "\n");
    fflush(L.fp);
  }

  /* Release lock */
  unlock();
}

#endif
