#pragma once
#ifndef _CEIL_HPP_
#define _CEIL_HPP_
#include <cmath>

#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::ceil(const Array<T> &ndarray) {
  return Utils::map<T>(ndarray, [](const T value) { return ::ceil(value); });
}

template <typename T> Array<T> Utils::ceil(const ArraySlice<T> &values) {
  return Utils::ceil<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif