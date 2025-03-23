#pragma once
#ifndef _SIN_HPP_
#define _SIN_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::sin(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::sin(value); });
}

template <typename T> Array<T> Utils::sin(const ArraySlice<T> &values) {
  return Utils::sin<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
