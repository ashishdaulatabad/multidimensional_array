#pragma once
#ifndef _SUM_HPP_
#define _SUM_HPP_
#include "./md_static_array_utility.hpp"
#include "./reduce.hpp"

template <typename T>
Array<T> Utils::sum(const Array<T> &values, const T init, const i32 axis) {
  return reduce(
      values,
      [](const T prev_value, const T current_value) {
        return prev_value + current_value;
      },
      init, axis);
}

template <typename T>
Array<T> Utils::sum(const ArraySlice<T> &values, const T init, const i32 axis) {
  return sum(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      init, axis);
}

#endif
