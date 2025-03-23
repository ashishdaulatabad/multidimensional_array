#pragma once
#ifndef _DIFF_HPP_
#define _DIFF_HPP_

#include "./md_static_array_utility.hpp"

template <typename T>
Array<T> Utils::diff(const Array<T> &ndarray, const i32 axis,
                     const usize thread_count) {
  if (axis == -1) {
    Array<T> result(ndarray.get_size() - 1);
#pragma omp parallel for
    for (usize index = 0; index < ndarray.get_size() - 1; ++index) {
      result.array_[index] = ndarray.array_[index + 1] - ndarray.array_[index];
    }

    return result;
  }

  if (axis >= ndarray.get_shape_size()) {
    throw std::runtime_error("Unknown axis " + std::to_string(axis) +
                             " requested for diff.");
  }

  std::vector<usize> overall_shape;

  for (usize index = 0; index < ndarray.get_shape_size(); ++index) {
    overall_shape.emplace_back(ndarray.get_shape()[index] - (index == axis));
  }

  Array<T> result(overall_shape, 0);

  const usize diff_index = ndarray.skip_vec[axis];

  const usize loop_times =
      (axis - 1 <= ndarray.get_shape_size() ? ndarray.skip_vec[axis - 1]
                                            : ndarray.get_size());

  auto __perform_diff_parallel = [&ndarray, &result, diff_index,
                                  loop_times](const usize thread_number,
                                              const usize total_threads) {
    const usize start_value = loop_times * thread_number;
    usize breaker = thread_number * diff_index;
    for (usize index = start_value; index < ndarray.get_size();
         index += (total_threads * loop_times)) {
      for (usize d_index = index; d_index < index + loop_times - diff_index;
           ++d_index) {
        result.array_[d_index - breaker] =
            ndarray.array_[d_index + diff_index] - ndarray.array_[d_index];
      }
      breaker += (total_threads * diff_index);
    }
  };

  std::vector<std::thread> thread_pool;

  for (usize index = 0; index < thread_count; ++index) {
    thread_pool.emplace_back(
        std::thread(__perform_diff_parallel, index, thread_count));
  }

  for (auto &thread : thread_pool) {
    thread.join();
  }

  return result;
}

template <typename T>
Array<T> Utils::diff(const ArraySlice<T> &ndarray, const i32 axis,
                     const usize thread_count) {
  return diff<T>(
      Array<T>(*ndarray.array_reference_, ndarray.offset, ndarray.shp_offset),
      axis, thread_count);
}

#endif