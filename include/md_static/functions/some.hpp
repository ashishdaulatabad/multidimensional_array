#pragma once
#ifndef _SOME_HPP_
#define _SOME_HPP_

#include "./md_static_array_utility.hpp"

template <typename T>
bool Utils::some(const Array<T> &ndarray, const fn<bool(const T &)> &function,
                 const usize threads) {
  if (ndarray.get_size() < s_threshold_size) {
    usize index = 0;

    while (index < ndarray.get_size() && !function(ndarray.array_[index++]))
      ;
    return index < ndarray.get_size();
  } else {
    std::vector<std::thread> thread_pool;
    std::vector<bool> thread_results(threads);

    auto eval_every_ = [&ndarray, &function,
                        &thread_results](const usize thread_number,
                                         const usize start, const usize end) {
      usize index = start;

      while (index < end && !function(ndarray.array_[index++]))
        ;

      thread_results[thread_number] = (index < end);
    };

    usize index = 0;
    const usize block = ndarray.get_size() / threads;

    for (; index < threads - 1; ++index) {
      const usize start = block * index;
      const usize end = start + block;
      thread_pool.emplace_back(eval_every_, index, start, end);
    }

    thread_pool.emplace_back(eval_every_, index, index, ndarray.get_size());

    for (auto &thread : thread_pool) {
      thread.join();
    }

    for (usize index = 0; index < thread_results.size(); ++index) {
      if (thread_results[index]) {
        return true;
      }
    }

    return false;
  }
}

template <typename T>
bool Utils::some(const ArraySlice<T> &ndarray,
                 const fn<bool(const T &)> &function, const usize threads) {
  return some<T>(
      Array<T>(*ndarray.array_reference_, ndarray.offset, ndarray.shp_offset),
      function, threads);
}
#endif
