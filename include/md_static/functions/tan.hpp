#pragma once
#ifndef _TAN_HPP_
#define _TAN_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::tan(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::tan(value); });
}

template <typename T> Array<T> Utils::tan(const ArraySlice<T> &values) {
  return Utils::tan<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
