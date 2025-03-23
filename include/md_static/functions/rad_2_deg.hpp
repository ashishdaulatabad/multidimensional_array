#pragma once
#ifndef _RAD_2_DEG_HPP_
#define _RAD_2_DEG_HPP_
#include <cmath>

#define M_PI 3.14159265358979323846
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::rad_2_deg(const Array<T> &ndarray) {
  return Utils::map<T>(ndarray,
                       [](const T value) { return value * 180.0 / M_PI; });
}

template <typename T> Array<T> Utils::rad_2_deg(const ArraySlice<T> &values) {
  return Utils::rad_2_deg<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif