#pragma once
#ifndef _ARCSIN_HPP_
#define _ARCSIN_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::arcsin(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::asin(value); });
}

template <typename T> Array<T> Utils::arcsin(const ArraySlice<T> &values) {
  return Utils::arcsin<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
