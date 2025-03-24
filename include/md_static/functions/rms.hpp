#pragma once
#ifndef _RMS_HPP_
#define _RMS_HPP_
#include "./accumulate_and_merge.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> f128 Utils::rms(const Array<T> &values, const T init) {
  auto acc = [](const T prev_value, const T current_value) {
    return prev_value + (current_value * current_value);
  };
  auto merge = [](const T prev_value, const T current_value) {
    return prev_value + current_value;
  };

  const f64 len = (values.get_size() * 1.0);
  const T initialize = static_cast<T>(init);
  f128 mean_sq = accumulate_and_merge_fn(values, acc, merge, initialize) / len;

  return ::sqrt(mean_sq);
}

template <typename T>
f128 Utils::rms(const ArraySlice<T> &values, const T init) {
  return rms(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      init);
}

#endif
