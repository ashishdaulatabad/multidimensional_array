#pragma once
#ifndef _INNER_HPP_
#define _INNER_HPP_

#include "../functions/accumulate_and_merge.hpp"
#include "./md_linear_algebra.hpp"

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::inner(const Array<T1> &first, const Array<T2> &other,
                        const usize threads) {
  const usize first_shape = first.get_shape_size();
  const usize other_shape = other.get_shape_size();

  if (first.get_shape()[first_shape - 1] !=
      other.get_shape()[other_shape - 1]) {
    throw std::runtime_error(
        "Last axis should be of same size: found " +
        std::to_string(first.get_shape()[first_shape - 1]) +
        " != " + std::to_string(other.get_shape()[other_shape - 1]));
  }

  const usize overall_shape = first_shape + other_shape - 2;

  std::vector<usize> resultant_shape(overall_shape);
  usize result_shape_index = 0;
  usize skip_number = 1;

  for (usize index = 0; index < first.get_shape_size() - 1; ++index) {
    resultant_shape[result_shape_index++] = first.shape[index];
  }

  for (usize index = 0; index < other.get_shape_size() - 1; ++index) {
    resultant_shape[result_shape_index++] = other.shape[index];
    skip_number *= other.shape[index];
  }

  // do this separately.
  if (resultant_shape.size() == 0) {
    if (s_threshold_size < first.get_size()) {
      std::vector<T3> value(threads, 0);

      auto evaluate_inner_ = [&first, &other, &value](const usize thread_number,
                                                      const usize start,
                                                      const usize end) {
        for (usize i = start; i < end; ++i) {
          value[thread_number] += first.array_[i] * other.array_[i];
        }
      };

      const usize blocks = first.get_size() / threads;
      std::vector<std::thread> thread_pool;
      usize start = 0, end = blocks, index = 0;

      for (; index < threads - 1; ++index, start += blocks, end += blocks) {
        thread_pool.emplace_back(evaluate_inner_, index, start, end);
      }

      end = first.get_size();
      thread_pool.emplace_back(evaluate_inner_, threads - 1, start, end);

      for (auto &thread : thread_pool) {
        thread.join();
      }

      T3 result = 0;

      for (auto &res_part : value) {
        result += res_part;
      }

      return Array<T3>(1, result);
    } else {
      T3 value = 0;

      for (usize i = 0; i < other.get_size(); ++i) {
        value += first.array_[i] * other.array_[i];
      }

      return Array<T3>(1, value);
    }
  } else {
    Array<T3> result(resultant_shape, 0);

    const usize row = first.shape[first.get_shape_size() - 1];

    auto evaluate_inner_ = [&result, &first, &other, row,
                            skip_number](const usize thread_number,
                                         const usize total_threads) {
      usize first_start = thread_number * row;
      usize index = thread_number * skip_number; // result start
      usize i = first_start;

      for (; i < first.get_size() && index < result.get_size();
           i += (total_threads * row)) {
        usize j = 0;

        for (; j < other.get_size() && index < result.get_size(); j += row) {
          for (usize k = 0; k < row; ++k) {
            result.array_[index] += first.array_[i + k] * other.array_[j + k];
          }

          ++index;
        }
        index += ((total_threads - 1) * skip_number);
      }
    };

    std::vector<std::thread> thread_pool;

    for (usize index = 0; index < threads; ++index) {
      thread_pool.emplace_back(evaluate_inner_, index, threads);
    }

    for (auto &thread : thread_pool) {
      thread.join();
    }

    return result;
  }
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::inner(const ArraySlice<T1> &first, const Array<T2> &other,
                        const usize threads) {
  return Linalg::inner<T3, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset), other,
      threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::inner(const ArraySlice<T1> &first,
                        const ArraySlice<T2> &other, const usize threads) {
  return Linalg::inner<T3, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T1>(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::inner(const Array<T1> &first, const ArraySlice<T2> &other,
                        const usize threads) {
  return Linalg::inner<T3, T1, T2>(
      first, Array<T1>(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

#endif
