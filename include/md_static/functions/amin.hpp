#pragma once
#ifndef _AMIN_HPP_
#define _AMIN_HPP_

#include "./md_static_array_utility.hpp"
#include "./reduce.hpp"

template <typename T>
Array<T> Utils::amin(const Array<T> &values, const i32 axis) {
  return reduce<T>(
      values,
      [](const T &prev_value, const T &curr_value) {
        return prev_value < curr_value ? prev_value : curr_value;
      },
      values.get_size() > 0 ? values.array_[0] : 1, axis);
}

template <typename T>
Array<T> Utils::amin(const ArraySlice<T> &values, const i32 axis) {
  return amin(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      axis);
}

#endif
