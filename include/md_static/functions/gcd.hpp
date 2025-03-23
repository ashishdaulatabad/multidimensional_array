#pragma once
#ifndef _GCD_HPP_
#define _GCD_HPP_

#include "./md_static_array_utility.hpp"
#include "./reduce.hpp"

template <typename T> T __gcd(T a, T b) { return b == 0 ? a : __gcd(b, a % b); }

template <typename T>
Array<T> Utils::gcd(const Array<T> &values, const i32 axis) {
  return reduce<T>(
      values,
      [](const T &prev_value, const T &curr_value) {
        return __gcd(prev_value, curr_value);
      },
      values.get_size() > 0 ? values.array_[0] : 1, axis);
}

template <typename T>
Array<T> Utils::gcd(const ArraySlice<T> &values, const i32 axis) {
  return gcd(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      axis);
}

#endif
