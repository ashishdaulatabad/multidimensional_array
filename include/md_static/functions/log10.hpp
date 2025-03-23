#pragma once
#ifndef _LOG10_HPP_
#define _LOG10_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::log10(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return ::log10(value); });
}

template <typename T> Array<T> Utils::log10(const ArraySlice<T> &values) {
  return Utils::log10<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
