#pragma once
#ifndef _OUTER_HPP_
#define _OUTER_HPP_

#include "./md_linear_algebra.hpp"

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::outer(const Array<T1> &first, const Array<T2> &other,
                        const usize threads) {
  Array<T3> result({first.get_size(), other.get_size()}, 0);

  auto outer_internal_ = [&result, &first, &other](const usize start,
                                                   const usize end) {
    const usize fsize = first.get_size(), osize = other.get_size();

    for (usize i = 0; i < fsize; ++i) {
      const auto x = first.array_[i];

      for (usize j = start; j < end; ++j) {
        result.array_[i * osize + j] = x * other.array_[j];
      }
    }
  };

  std::vector<std::thread> thread_pool;
  const usize block_size = other.get_size() / threads;
  usize index = 0, start = 0, end = block_size;

  for (; index < threads - 1; ++index, start += block_size, end += block_size) {
    thread_pool.emplace_back(outer_internal_, start, end);
  }

  thread_pool.emplace_back(outer_internal_, start, first.get_size());

  for (auto &thread : thread_pool) {
    thread.join();
  }

  return result;
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::outer(const ArraySlice<T1> &first, const Array<T2> &other,
                        const usize threads) {
  return Linalg::outer<T3, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset), other,
      threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::outer(const Array<T1> &first, const ArraySlice<T2> &other,
                        const usize threads) {
  return Linalg::outer<T3, T1, T2>(
      first, Array<T1>(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::outer(const ArraySlice<T1> &first,
                        const ArraySlice<T2> &other, const usize threads) {
  return Linalg::outer<T3, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T1>(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

#endif
