#pragma once
#ifndef _REDUCE_HPP_
#define _REDUCE_HPP_
#include <vector>

#include "./md_static_array_utility.hpp"

template <typename T, typename fn_>
Array<T> Utils::reduce(const Array<T> &values, const fn_ &function_exec,
                       const T init, const i32 axis) {
  if (axis == -1) {
    const usize size = values.get_size();
    Array<T> result(1, init);
    const u8 thread_count = ::s_thread_count;
    const usize threshold_size = ::s_threshold_size;

    if (thread_count == 1 || size <= threshold_size) {
      T res = init;

      for (usize index = 0; index < size; ++index) {
        res = function_exec(res, values.array_[index]);
      }

      result.array_[0] = res;
    } else {
      std::vector<std::thread> threads;
      std::vector<T> accumulator(thread_count, init);

      auto accumulate = [&accumulator, &values, &function_exec,
                         init](const u8 thread_number, const usize start,
                               const usize end) {
        T result = init;

        for (usize index = start; index < end; ++index) {
          result = function_exec(result, values.array_[index]);
        }

        accumulator[thread_number] = result;
      };

      const usize block = size / thread_count;
      const u8 thread_but_one = thread_count - 1;
      usize index = 0, start = 0, end = block;

      for (; index < thread_but_one; ++index, start += block, end += block) {
        threads.emplace_back(accumulate, index, start, end);
      }

      threads.emplace_back(accumulate, index, start, size);

      for (auto &thread : threads) {
        thread.join();
      }

      for (auto &thread_result : accumulator) {
        result.array_[0] = function_exec(result.array_[0], thread_result);
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

    Array<T> result(result_shape, init);

    const usize size = values.get_size();
    const usize skip_index = values.skip_vec[axis];
    const usize loop_index = axis - 1 >= 0 ? values.skip_vec[axis - 1] : size;
    const usize total_threads = ::s_thread_count;

    const usize dispatched_threads = std::min(result.get_size(), total_threads);

    auto reduce_par_ = [&values, &function_exec, &result, skip_index,
                        loop_index, total_threads](const usize thread_number) {
      usize value_index = thread_number * loop_index;
      usize index = thread_number * skip_index;
      const usize index_stride = total_threads * skip_index;
      const usize val_index_stride = total_threads * loop_index;
      const usize size = result.get_size();

      for (; index < size;
           index += index_stride, value_index += val_index_stride) {
        for (usize loop_time = 0; loop_time < loop_index;
             loop_time += skip_index) {
          for (usize block_index = 0; block_index < skip_index; ++block_index) {
            result.array_[index + block_index] = function_exec(
                result.array_[index + block_index],
                values.array_[value_index + loop_time + block_index]);
          }
        }
      }
    };

    std::vector<std::thread> thread_pool;

    for (usize index = 0; index < dispatched_threads; ++index) {
      thread_pool.emplace_back(reduce_par_, index);
    }

    for (auto &thread : thread_pool) {
      thread.join();
    }

    return result;
  }
}

template <typename T, typename fn_>
Array<T> Utils::reduce(const ArraySlice<T> &values, const fn_ &function_exec,
                       const T init, const i32 axis) {
  return reduce<T, fn_>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      function_exec, init, axis);
}

#endif
