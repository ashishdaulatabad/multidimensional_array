#pragma once
#ifndef _ACCUMULATE_AND_MERGE_HPP_
#define _ACCUMULATE_AND_MERGE_HPP_
#include <vector>

#include "./md_static_array_utility.hpp"

template <typename T, typename function, typename merge_function>
T Utils::accumulate_and_merge_fn(const Array<T> &values,
                                 const function &function_exec,
                                 const merge_function &merge_func,
                                 const T init) {
  const usize size = values.get_size();
  T result = init;
  const u8 thread_count = ::s_thread_count;
  const usize threshold_size = ::s_threshold_size;

  if (thread_count == 1 || size <= threshold_size) {
    for (usize index = 0; index < size; ++index) {
      result = function_exec(result, values.array_[index]);
    }
  } else {
    std::vector<std::thread> st;
    std::vector<T> accumulator(thread_count);
    auto acc_merge_ = [&accumulator, &values,
                       &function_exec](const u8 thread_number,
                                       const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        accumulator[thread_number] =
            function_exec(accumulator[thread_number], values.array_[index]);
      }
    };

    const usize block = size / thread_count;
    const u8 thread_but_one = thread_count - 1;

    for (usize i = 0; i < thread_but_one; ++i) {
      st.emplace_back(acc_merge_, i, block * i, block * (i + 1));
    }

    st.emplace_back(acc_merge_, thread_but_one, block * thread_but_one, size);

    for (auto &th : st) {
      th.join();
    }

    for (auto &result_th : accumulator) {
      result = merge_func(result, result_th);
    }
  }

  return result;
}

template <typename T, typename function, typename merge_function>
T Utils::accumulate_and_merge_fn(const ArraySlice<T> &values,
                                 const function &function_exec,
                                 const merge_function &merge_func,
                                 const T init) {
  return Utils::accumulate_and_merge_fn<T, function, merge_function>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      function_exec, merge_func, init);
}

#endif
