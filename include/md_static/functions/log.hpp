#pragma once
#ifndef _LOG_HPP_
#define _LOG_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::log(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::log(value); });
}

template <typename T> Array<T> Utils::log(const ArraySlice<T> &values) {
  return Utils::log<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
