/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : types
 * @created     : Sunday Sep 15, 2019 10:02:16 MDT
 */

#ifndef TYPES_H

#define TYPES_H

// C Includes
#include<string>
#include<thread>
#include<memory>
#include<chrono>
#include<functional>
#include<future>
#include<unordered_map>

namespace scpp {

typedef std::string string;
typedef std::thread thread;

namespace time = std::chrono;



// https://en.cppreference.com/w/cpp/language/type_alias
template<typename T>
using function = std::function<T>;
template<typename T>
using future = std::future<T>;
template<typename T>
using promise = std::promise<T>;
template<typename T>
using unique_ptr = std::unique_ptr<T>;
template<typename T>
using shared_ptr = std::shared_ptr<T>;
template<typename T>
using weak_ptr = std::weak_ptr<T>;
template<typename T, typename C>
using unordered_map = std::unordered_map<T, C>;

template<class T>
typename std::remove_reference<T>::type&& move( T&& t) noexcept {
  return std::move(t);
}

template<class T>
unique_ptr<T> make_unique(std::size_t size) {
  return std::make_unique<T>(size);
}
}

#endif /* end of include guard TYPES_H */

