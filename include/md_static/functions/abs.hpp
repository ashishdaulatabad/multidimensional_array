#pragma once
#ifndef _ABS_HPP_
#define _ABS_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::abs(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return std::abs(value); });
}

template <typename T> Array<T> Utils::abs(const ArraySlice<T> &values) {
  return Utils::abs<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
