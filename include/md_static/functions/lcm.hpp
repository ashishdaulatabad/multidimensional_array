#pragma once
#ifndef _LCM_HPP_
#define _LCM_HPP_

#include "./md_static_array_utility.hpp"
#include "./reduce.hpp"

template <typename T> T __gcd2(T a, T b) {
  return b == 0 ? a : __gcd2(b, a % b);
}

template <typename T>
Array<T> Utils::lcm(const Array<T> &values, const i32 axis) {
  return reduce<T>(
      values,
      [](const T &prev_value, const T &curr_value) {
        if (prev_value == 0 || curr_value == 0) {
          return prev_value == 0 ? prev_value : curr_value;
        }
        return (prev_value / __gcd2(prev_value, curr_value)) * curr_value;
      },
      1, axis);
}

template <typename T>
Array<T> Utils::lcm(const ArraySlice<T> &values, const i32 axis) {
  return lcm(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      axis);
}

#endif
