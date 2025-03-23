#pragma once
#ifndef _COS_HPP_
#define _COS_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::cos(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::cos(value); });
}

template <typename T> Array<T> Utils::cos(const ArraySlice<T> &values) {
  return Utils::cos<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
