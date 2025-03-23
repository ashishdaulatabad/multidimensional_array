#pragma once
#ifndef _STD_DEV_HPP_
#define _STD_DEV_HPP_
#include <cmath>

#include "./accumulate_and_merge.hpp"
#include "./md_static_array_utility.hpp"

template <typename T> f128 Utils::std_dev(const Array<T> &values) {
  f128 fmean = mean(values);
  const T size = static_cast<T>(values.get_size());
  const T init = static_cast<T>(0);

  const auto acc = [&fmean](const T prev_value, const T current_value) {
    const T dev = (fmean - current_value);
    return prev_value + dev * dev;
  };

  const auto merge = [](const T prev_value, const T current_value) {
    return prev_value + current_value;
  };

  f128 mean_sq_err = accumulate_and_merge_fn(values, acc, merge, init) / size;
  return ::sqrt(mean_sq_err);
}

template <typename T> f128 Utils::std_dev(const ArraySlice<T> &values) {
  return std_dev<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
