/**
 *  @file random
 *  @brief A library for random generation functions
 *
 *  Creates random things
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 18:47:53 MDT
 */

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <sstream>

namespace utils {
namespace random {
/**
 * @brief Generates a random char
 *
 * @return A random char
 */
static inline unsigned char random_char() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);
  return static_cast<unsigned char>(dis(gen));
}
/**
 * @brief Generates a random hex
 *
 * @param len The length of the hex
 *
 * @return A random hex
 */
static inline std::string generate_hex(const unsigned int len) {
  std::stringstream ss;
  for (unsigned int i = 0; i < len; i++) {
    auto rc = random_char();
    std::stringstream hexstream;
    hexstream << std::hex << int(rc);
    auto hex = hexstream.str();
    ss << (hex.length() < 2 ? '0' + hex : hex);
  }
  return ss.str();
}
} // namespace random
} // namespace utils

#endif /* end of include guard RANDOM_HPP */
