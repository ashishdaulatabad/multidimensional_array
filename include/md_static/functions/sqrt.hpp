#pragma once
#ifndef _SQRT_HPP_
#define _SQRT_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::sqrt(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::sqrt(value); });
}

template <typename T> Array<T> Utils::sqrt(const ArraySlice<T> &values) {
  return Utils::sqrt<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
