#pragma once
#ifndef _COT_HPP_
#define _COT_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::cot(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return 1 / ::tan(value); });
}

template <typename T> Array<T> Utils::cot(const ArraySlice<T> &values) {
  return Utils::cot<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
