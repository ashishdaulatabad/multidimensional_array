#pragma once
#ifndef _ARCTAN_HPP_
#define _ARCTAN_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::arctan(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::atan(value); });
}

template <typename T> Array<T> Utils::arctan(const ArraySlice<T> &values) {
  return Utils::arctan<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
