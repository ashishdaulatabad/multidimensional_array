#pragma once
#ifndef _EXP2_HPP_
#define _EXP2_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<double> Utils::exp2(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::pow(2, value); });
}

template <typename T> Array<double> Utils::exp2(const ArraySlice<T> &values) {
  return Utils::exp2<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
