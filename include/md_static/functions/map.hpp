#pragma once
#ifndef _MAP_HPP_
#define _MAP_HPP_
#include "./md_static_array_utility.hpp"

template <typename T>
Array<T> Utils::map(const Array<T> &values,
                    const fn<T(const T &)> &function_exec) {
  const usize size = values.get_size();

  std::vector<usize> shp;
  for (usize index = 0; index < values.get_shape_size(); ++index) {
    shp.emplace_back(values.shape[index]);
  }

  Array<T> result(shp, 0);

  const u8 thread_count = ::s_thread_count;
  const usize threshold_size = ::s_threshold_size;

  if (thread_count == 1 || size <= threshold_size) {
    for (usize index = 0; index < size; ++index) {
      result.array_[index] = function_exec(values.array_[index]);
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(thread_count);
    auto _add_int = [&result, &values, &function_exec](const usize start,
                                                       const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = function_exec(values.array_[index]);
      }
    };

    const usize block = size / thread_count;
    const u8 thread_but_one = thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
Array<T> Utils::map(const ArraySlice<T> &values,
                    const fn<T(const T &)> &function_exec) {
  return map<T>(
      Array(*values.array_reference_, values.offset, values.shp_offset),
      function_exec);
}

#endif
