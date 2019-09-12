/**
 *  @file string
 *  @brief A library for string manipulation
 *
 *  Strings are always fun to play with, feel free to add
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 18:47:53 MDT
 */

#ifndef UTILS_STRING_HPP
#define UTILS_STRING_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

namespace utils {
namespace strings {
/**
 * @brief Returns the upper case of a string
 *
 * @param value The string to upper caseify
 *
 * @return An upper caseified string
 */
static inline std::string upper(const std::string &value) {
  auto val = value;
  std::transform(val.begin(), val.end(), val.begin(), ::toupper);
  return val;
}

/**
 * @brief Returns a lower case string
 *
 * @param value The string to lowerify
 *
 * @return A lowerified string
 */
static inline std::string lower(const std::string &value) {
  auto val = value;
  std::transform(val.begin(), val.end(), val.begin(), ::tolower);
  return val;
}
/**
 * @brief Parses a string for a specified deliminator
 *
 * @param value The string to pass
 * @param delim The deliminator to parse it by
 *
 * @return A vector of strings that were origionally seperated by the delim
 */
static inline std::vector<std::string> parse(const std::string &value,
                                             const char delim) {
  std::vector<std::string> values;
  auto pos = 0U;
  auto size = value.size();
  while (pos < size) {
    std::string temp;
    while (pos < size && value[pos] != delim)
      temp += value[pos++];
    pos++;
    values.push_back(temp);
  }
  return values;
}
/*
 * Example usages of Join
 *
 * join("hello", 5, 9.12, "world")
 * hello59.12world
 *
 * join_d(join("*", ',', 0), "helo", "world", 7)
 * hello*,0world*,07
 */
// Helper for join
template <typename T> static inline T join(T value) { return value; }

// Join a list of strings / ints / whatever (streamable) no deliminator
template <typename T, typename... ArgsT>
static inline std::string join(T value, ArgsT... strings) {
  std::stringstream ss;
  ss << value << join(strings...);
  return ss.str();
}

// Helper for join_d
template <typename T, typename D> static inline T join_d(D delim, T value) {
  return value;
}
// Join a list of strings / ints / whatever (streamable) with a deliminator
template <typename Delim, typename T, typename... ArgsT>
static inline std::string join_d(Delim delim, T value, ArgsT... strings) {
  std::stringstream ss;
  ss << value << delim << join_d(delim, strings...);
  return ss.str();
}

} // namespace strings
} // namespace utils
#endif
