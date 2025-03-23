#pragma once
#ifndef _ARCCOS_HPP_
#define _ARCCOS_HPP_
#include <cmath>

#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::arccos(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return acos(value); });
}

template <typename T> Array<T> Utils::arccos(const ArraySlice<T> &values) {
  return Utils::arccos<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
