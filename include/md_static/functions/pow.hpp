#pragma once
#ifndef _POW_HPP_
#define _POW_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::pow(const Array<T> &values, f64 power) {
  return Utils::map<T>(values,
                       [power](const T value) { return ::pow(value, power); });
}

template <typename T>
Array<T> Utils::pow(const ArraySlice<T> &values, f64 power) {
  return Utils::pow<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      power);
}

#endif
