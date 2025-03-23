#pragma once
#ifndef _COSH_HPP_
#define _COSH_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<f64> Utils::cosh(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::cosh(value); });
}

template <typename T> Array<f64> Utils::cosh(const ArraySlice<T> &values) {
  return Utils::cosh<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
