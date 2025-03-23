#pragma once
#ifndef _VANDERMONTE_HPP_
#define _VANDERMONTE_HPP_

#include "./md_array_manipulation.hpp"

template <typename T>
Array<T> Manip::vandermonte(const Array<T> &rows, const i64 n,
                            const bool increasing_order) {
  if (rows.get_shape_size() != 1) {
    throw std::runtime_error("Given array must be 1-dimensional; found: " +
                             std::to_string(rows.get_shape_size()));
  }

  const usize loop = (n != -1 ? n : rows.get_size());

  Array<T> result({rows.get_size(), loop}, 1);

  if (::s_thread_count == 1 || ::s_threshold_size > result.get_size()) {
    if (!increasing_order) {
      for (usize row = 0; row < rows.get_size(); ++row) {
        const auto c = rows.array_[row];

        for (i32 index = loop - 2; index >= 0; --index) {
          const usize result_index = row * loop + index;
          result.array_[result_index] = result.array_[result_index + 1] * c;
        }
      }
    } else {
      for (usize row = 0; row < rows.get_size(); ++row) {
        const auto c = rows.array_[row];

        for (usize index = 1; index < loop; ++index) {
          const usize result_index = row * loop + index;
          result.array_[result_index] = result.array_[result_index - 1] * c;
        }
      }
    }
  } else {
    auto gen_vander_ = [&rows, loop, &result,
                        increasing_order](const usize start, const usize end) {
      if (!increasing_order) {
        for (usize row = start; row < end; ++row) {
          const auto c = rows.array_[row];

          for (i32 index = loop - 2; index >= 0; --index) {
            const usize result_index = row * loop + index;
            result.array_[result_index] = result.array_[result_index + 1] * c;
          }
        }
      } else {
        for (usize row = start; row < end; ++row) {
          const auto c = rows.array_[row];

          for (usize index = 1; index < loop; ++index) {
            const usize result_index = row * loop + index;
            result.array_[result_index] = result.array_[result_index - 1] * c;
          }
        }
      }
    };

    const usize block_size = rows.get_size() / ::s_thread_count;
    std::vector<std::thread> thread_pool;
    usize index = 0, next = 1, thread_counts = ::s_thread_count;

    for (; index < thread_counts - 1; ++index, ++next) {
      const usize start = index * block_size;
      const usize end = next * block_size;
      thread_pool.emplace_back(gen_vander_, start, end);
    }

    const usize start = (thread_counts - 1) * block_size;
    const usize end = rows.get_size();
    thread_pool.emplace_back(gen_vander_, start, end);

    for (auto &thread : thread_pool) {
      thread.join();
    }
  }

  return result;
}

template <typename T>
Array<T> Manip::vandermonte(const ArraySlice<T> &rows, const i64 n,
                            const bool increasing_order) {
  return vandermonte<T>(
      Array<T>(*rows.array_reference_, rows.offset, rows.shp_offset), n,
      increasing_order);
}
#endif
