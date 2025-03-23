#pragma once
#ifndef _EVERY_HPP_
#define _EVERY_HPP_

#include "./md_static_array_utility.hpp"

template <typename T>
bool Utils::every(const Array<T> &ndarray, const fn<bool(const T &)> &function,
                  const usize threads) {
  if (ndarray.get_size() < s_threshold_size || threads == 1) {
    usize index = 0;
    for (; index < ndarray.get_size() && function(ndarray.array_[index]);
         ++index) {
    }
    return index == ndarray.get_size();
  } else {
    std::vector<std::thread> thread_pool;
    std::vector<bool> thread_results(threads);

    fn<void(const usize, const usize, const usize)> perform_every_parallel_ =
        [&ndarray, &function, &thread_results](
            const usize thread_number, const usize start, const usize end) {
          usize index = start;
          for (; index < end && function(ndarray.array_[index]); ++index) {
          }
          thread_results[thread_number] = (index == end);
        };
    const usize block = ndarray.get_size() / threads;

    for (usize index = 0; index < threads - 1; ++index) {
      thread_pool.emplace_back(std::thread(perform_every_parallel_, index,
                                           block * index, block * (index + 1)));
    }

    thread_pool.emplace_back(std::thread(perform_every_parallel_, threads - 1,
                                         block * (threads - 1),
                                         ndarray.get_size()));

    for (auto &thread : thread_pool) {
      thread.join();
    }

    for (usize index = 0; index < thread_results.size(); ++index) {
      if (!thread_results[index]) {
        return false;
      }
    }

    return true;
  }
}

template <typename T>
bool Utils::every(const ArraySlice<T> &__ndarray_reference,
                  const fn<bool(const T &)> &function, const usize threads) {
  return every<T>(Array<T>(*__ndarray_reference.array_reference_,
                           __ndarray_reference.offset,
                           __ndarray_reference.shp_offset),
                  function, threads);
}
#endif
