#pragma once
#ifndef _ARGMIN_HPP_
#define _ARGMIN_HPP_

#include "./md_static_array_utility.hpp"

template <typename T>
Array<usize> Utils::argmin(const Array<T> &values, const i32 axis) {
  if (axis == -1) {
    const usize size = values.get_size();
    Array<usize> result(1, 0);
    const u8 thread_count = ::s_thread_count;
    const usize threshold_size = ::s_threshold_size;

    if (thread_count == 1 || size <= threshold_size) {
      usize res = 0;

      for (usize index = 0; index < size; ++index) {
        if (values.array_[res] > values.array[index]) {
          res = index;
        }
      }

      result.array_[0] = res;
    } else {
      std::vector<std::thread> threads;
      std::vector<usize> accumulator(thread_count, 0);

      auto evaluate_argmin_ = [&accumulator, &values](const u8 thread_number,
                                                      const usize start,
                                                      const usize end) {
        usize res = 0;

        for (usize index = start; index < end; ++index) {
          if (values.array_[res] > values.array_[index]) {
            res = index;
          }
        }

        accumulator[thread_number] = res;
      };

      const usize block = size / thread_count;
      const u8 thread_but_one = thread_count - 1;
      usize index = 0, start = 0, end = block;

      for (; index < thread_but_one; ++index, start += block, end += block) {
        threads.emplace_back(evaluate_argmin_, index, start, end);
      }

      threads.emplace_back(evaluate_argmin_, thread_but_one, start, size);

      for (auto &thread : threads) {
        thread.join();
      }

      for (auto &result_th : accumulator) {
        result.array_[0] =
            values.array_[result_th] < values.array_[result.array_[0]]
                ? result_th
                : result.array_[0];
      }
    }

    return result;
  } else {
    if (axis < 0 || axis >= values.get_shape_size()) {
      throw std::runtime_error("Unknown axis requested for function map.");
    }

    std::vector<usize> result_shape;

    for (usize index = 0; index < values.get_shape_size(); ++index) {
      if (axis != index) {
        result_shape.emplace_back(values.shape[index]);
      }
    }

    Array<usize> result(result_shape, 0);

    const usize size = values.get_size();
    const usize skip_index = values.skip_vec[axis];
    const usize loop_index = axis - 1 >= 0 ? values.skip_vec[axis - 1] : size;
    const usize total_threads = ::s_thread_count;

    auto evaluate_argmin_ = [&values, &result, skip_index, loop_index,
                             total_threads, axis](const usize thread_number) {
      usize value_index = thread_number * loop_index;
      usize index = thread_number * skip_index;

      for (; index < result.get_size();
           index += (total_threads * skip_index),
           value_index += (total_threads * loop_index)) {
        for (usize loop_time = 0, axis_index = 0; loop_time < loop_index;
             loop_time += skip_index, ++axis_index) {
          for (usize block_index = 0; block_index < skip_index; ++block_index) {
            result.array_[index + block_index] =
                values.array_[value_index +
                              (result.array_[index + block_index] *
                               skip_index) +
                              block_index] <
                        values.array_[value_index + loop_time + block_index]
                    ? result.array_[index + block_index]
                    : axis_index;
          }
        }
      }
    };

    std::vector<std::thread> thread_pool;
    const usize dispatched_threads = std::min(result.get_size(), total_threads);

    for (usize index = 0; index < dispatched_threads; ++index) {
      thread_pool.emplace_back(std::thread(evaluate_argmin_, index));
    }

    for (auto &thread : thread_pool) {
      thread.join();
    }

    return result;
  }
}

template <typename T>
Array<usize> Utils::argmin(const ArraySlice<T> &values, const i32 axis) {
  return argmin<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      axis);
}

#endif
