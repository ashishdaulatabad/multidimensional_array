#pragma once
#ifndef _FLOOR_HPP_
#define _FLOOR_HPP_
#include <cmath>

#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::floor(const Array<T> &array) {
  return Utils::map<T>(array, [](const T value) { return ::floor(value); });
}

template <typename T> Array<T> Utils::floor(const ArraySlice<T> &values) {
  return Utils::floor<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif