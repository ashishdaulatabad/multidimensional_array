#pragma once
#ifndef _LOG2_HPP_
#define _LOG2_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::log2(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::log2(value); });
}

template <typename T> Array<T> Utils::log2(const ArraySlice<T> &values) {
  return Utils::log2<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
