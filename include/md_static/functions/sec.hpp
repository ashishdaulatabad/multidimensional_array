#pragma once
#ifndef _SEC_HPP_
#define _SEC_HPP_
#include <cmath>

#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> Array<T> Utils::sec(const Array<T> &values) {
  return Utils::map<T>(values, [](const T &value) { return 1 / ::cos(value); });
}

template <typename T> Array<T> Utils::sec(const ArraySlice<T> &values) {
  return Utils::sec<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
