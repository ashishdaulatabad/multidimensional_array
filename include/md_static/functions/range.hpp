#pragma once
#ifndef _RANGE_HPP_
#define _RANGE_HPP_

#include "./md_static_array_utility.hpp"

// To do: improve and update since it does not work for all cases
template <typename T, typename Ts, class T1>
Array<T> Utils::range(const T start, const T end, const Ts spacing) {
  usize size = 0, start_value = 0;
  Ts increment = 1;

  if (end < start && spacing > 0) {
    throw std::runtime_error(
        "Spacing given should be negative for ranges: [end (" +
        std::to_string(end) + ") < start (" + std::to_string(start) + ")]");
  }

  if (end == -1 && spacing == 0) {
    size = start;
    increment = 1;
  } else if (spacing == 0 && end > start) {
    const T value = end - start;
    const T max_value = std::max(value, static_cast<decltype(value)>(0));

    // Use standard ceil
    size = static_cast<usize>(std::ceil(max_value));
    start_value = start;
    increment = 1;
  } else {
    const double value = std::abs((end - start) / (spacing * 1.0));
    const T max_value = std::max(value, static_cast<f64>(0));

    // Use standard ceil
    size = static_cast<usize>(std::ceil(max_value));
    start_value = start;
    increment = spacing;
  }

  Array<T> result(size);

  auto eval_range_ = [&result](const T init, const usize start, const usize end,
                               const T incr) {
    T value = init;

    for (usize index = start; index < end; ++index, value += incr) {
      result.array_[index] = value;
    }
  };

  if (::s_thread_count == 1 || size < ::s_threshold_size) {
    eval_range_(start_value, 0, size, increment);
    return result;
  }

  const usize block = size / s_thread_count;
  const Ts b_increment = increment * block;

  std::vector<std::thread> thread_pool;
  T block_start = start_value;
  usize index = 0;

  for (; index < s_thread_count - 1; ++index, block_start += b_increment) {
    const usize istart = block * index;
    const usize iend = istart + block;
    thread_pool.emplace_back(eval_range_, block_start, istart, iend, increment);
  }

  const usize istart = block * index;
  const usize iend = size;
  thread_pool.emplace_back(eval_range_, block_start, istart, iend, increment);

  for (auto &thread : thread_pool) {
    thread.join();
  }

  return result;
}

#endif
