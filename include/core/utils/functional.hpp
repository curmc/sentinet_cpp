/**
 *  @file functional
 *  @brief A callback library for publisher subscribers
 *
 *  In the future, I would like a series of default callbacks that
 *  can be set by the system
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 18:47:53 MDT
 */

#ifndef FUNCTIONAL_HPP

#define FUNCTIONAL_HPP

namespace util {
namespace functional {

/**
 * @brief I don't know what this does or why it's here
 *
 * @param itterations no idea
 * @param cb I don't know
 * @param start Sorry
 *
 * @return Some type of vector. The template syntax is wrong even
 */
std::vector<T> generate(int itterations, void (*cb)(int), int &start = 1) {
  vector<int> sequence;
  for (int i = start; i < start + itterations; i++) {
    sequence.push_back((*cb)(i));
  }
  return sequence;
}
} // namespace functional
} // namespace util

#endif /* end of include guard FUNCTIONAL_HPP */
