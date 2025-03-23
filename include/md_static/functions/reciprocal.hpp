#pragma once
#ifndef _RECIPROCAL_HPP_
#define _RECIPROCAL_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<double> Utils::reciprocal(const Array<T> &values) {
  return Utils::map<T>(values,
                       [](const T &value) { return 1 / (value * 1.0); });
}

template <typename T>
Array<double> Utils::reciprocal(const ArraySlice<T> &values) {
  return Utils::sec<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
