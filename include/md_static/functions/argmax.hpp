#pragma once
#ifndef _ARGMAX_HPP_
#define _ARGMAX_HPP_

#include "./md_static_array_utility.hpp"

template <typename T>
Array<usize> Utils::argmax(const Array<T> &values, const i32 axis) {
  if (axis == -1) {
    Array<usize> result(1, 0);

    const usize size = values.get_size();
    const usize threshold_size = ::s_threshold_size;
    const u8 thread_count = ::s_thread_count;

    if (thread_count == 1 || size <= threshold_size) {
      usize result_index = 0;

      for (usize index = 0; index < size; ++index) {
        if (values.array_[result_index] < values.array_[index]) {
          result_index = index;
        }
      }

      result.array_[0] = result_index;
    } else {
      std::vector<std::thread> thread_pool;
      std::vector<usize> accumulator(thread_count, 0);

      auto evaluate_arg_max_ = [&accumulator, &values](const u8 thread_number,
                                                       const usize start,
                                                       const usize end) {
        usize result = 0;

        for (usize index = start; index < end; ++index) {
          if (values.array_[result] < values.array_[index]) {
            result = index;
          }
        }

        accumulator[thread_number] = result;
      };

      const usize block = size / thread_count;
      const u8 thread_but_one = thread_count - 1;
      usize index = 0;

      for (; index < thread_but_one; ++index) {
        const usize start = block * index;
        const usize end = start + block;
        thread_pool.emplace_back(evaluate_arg_max_, index, start, end);
      }

      const usize start = block * thread_but_one;
      const usize end = size;
      thread_pool.emplace_back(evaluate_arg_max_, thread_but_one, start, end);

      for (auto &thread : thread_pool) {
        thread.join();
      }

      for (auto &result_th : accumulator) {
        result.array_[0] =
            values.array_[result_th] > values.array_[result.array_[0]]
                ? result_th
                : result.array_[0];
      }
    }
    return result;
  } else {
    if (axis < 0 || axis >= values.get_shape_size()) {
      throw std::runtime_error("Unknown axis requested for function map.");
    }

    std::vector<usize> resultant_shape;

    for (usize index = 0; index < values.get_shape_size(); ++index) {
      if (axis != index) {
        resultant_shape.emplace_back(values.shape[index]);
      }
    }

    Array<usize> result(resultant_shape, 0);

    const usize skip_index = values.skip_vec[axis];
    const usize size = values.get_size();
    const usize loop_index = axis - 1 >= 0 ? values.skip_vec[axis - 1] : size;
    const usize total_threads = ::s_thread_count;

    // Todo: Understand what this is...
    auto evaluate_arg_min_ = [&values, &result, skip_index, loop_index,
                              total_threads, axis](const usize thread_number) {
      usize value_index = thread_number * loop_index;
      usize index = thread_number * skip_index;

      for (; index < result.get_size();
           index += (total_threads * skip_index),
           value_index += (total_threads * loop_index)) {
        usize loop_time = 0, axis_index = 0;

        for (; loop_time < loop_index; loop_time += skip_index, ++axis_index) {
          for (usize block_index = 0; block_index < skip_index; ++block_index) {
            result.array_[index + block_index] =
                values.array_[value_index +
                              (result.array_[index + block_index] *
                               skip_index) +
                              block_index] >
                        values.array_[value_index + loop_time + block_index]
                    ? result.array_[index + block_index]
                    : axis_index;
          }
        }
      }
    };

    std::vector<std::thread> thread_pool;
    const usize dispatch_threads = std::min(result.get_size(), total_threads);

    for (usize index = 0; index < dispatch_threads; ++index) {
      thread_pool.emplace_back(evaluate_arg_min_, index);
    }

    for (auto &thread : thread_pool) {
      thread.join();
    }

    return result;
  }
}

template <typename T>
Array<usize> Utils::argmax(const ArraySlice<T> &values, const i32 axis) {
  return argmax<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      axis);
}

#endif
