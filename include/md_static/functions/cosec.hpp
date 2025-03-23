#pragma once
#ifndef _COSEC_HPP_
#define _COSEC_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::cosec(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return 1 / ::sin(value); });
}

template <typename T> Array<T> Utils::cosec(const ArraySlice<T> &values) {
  return Utils::cosec<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
