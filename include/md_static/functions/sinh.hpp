#pragma once
#ifndef _SINH_HPP_
#define _SINH_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<f64> Utils::sinh(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::sinh(value); });
}

template <typename T> Array<f64> Utils::sinh(const ArraySlice<T> &values) {
  return Utils::sinh<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
