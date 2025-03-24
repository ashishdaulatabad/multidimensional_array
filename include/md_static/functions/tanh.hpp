#pragma once
#ifndef _TANH_HPP_
#define _TANH_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<f64> Utils::tanh(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::tanh(value); });
}

template <typename T> Array<f64> Utils::tanh(const ArraySlice<T> &values) {
  return Utils::tanh<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
