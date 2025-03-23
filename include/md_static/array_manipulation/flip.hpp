#pragma once
#ifndef _FLIP_HPP_
#define _FLIP_HPP_

#include "./md_array_manipulation.hpp"

template <typename T>
Array<T> Manip::flip(const Array<T> &ndarray, const i32 axis) {
  if (axis == -1) {
    Array<T> result(ndarray);

    for (usize index = 0; index < result.get_size() / 2; ++index) {
      const auto temp = result.array_[index];
      result.array_[index] = result.array_[result.get_size() - 1 - index];
      result.array_[result.get_size() - 1 - index] = temp;
    }

    return result;
  }

  if (axis >= ndarray.get_shape_size()) {
    throw std::runtime_error("Unknown axis " + std::to_string(axis) +
                             " requested for operation flip");
  }

  Array<T> result(ndarray);
  const usize total_axes = result.get_axis_reference(axis).get_total_axes();

  if (s_thread_count == 1 || s_threshold_size > ndarray.get_size()) {
    for (usize index = 0; index < total_axes; ++index) {
      const auto axis_ref = result.get_nth_axis_reference(axis, index);

      for (usize i = 0; i < axis_ref.get_size() - i; ++i) {
        std::swap(axis_ref[i], axis_ref[axis_ref.get_size() - 1 - i]);
      }
    }
  } else {
    auto flip_arr_int_ = [&result, axis](const usize start, const usize end) {
      auto axis_ref = result.get_nth_axis_reference(axis, start);

      for (usize index = start; index < end; ++index) {
        for (usize i = 0; i < axis_ref.get_size() - i; ++i) {
          std::swap(axis_ref[i], axis_ref[axis_ref.get_size() - 1 - i]);
        }

        axis_ref.switch_to_next_axis_index();
      }
    };

    std::vector<std::thread> thread_pool;
    const usize total_axes_for_single_thread = total_axes / s_thread_count;
    usize index = 0, next = 1;

    for (; index < s_thread_count - 1; ++index, ++next) {
      const usize start = total_axes_for_single_thread * index;
      const usize end = total_axes_for_single_thread * next;
      thread_pool.emplace_back(flip_arr_int_, start, end);
    }

    const usize last_start_point = total_axes_for_single_thread * index;
    const usize last_end_point = total_axes;

    thread_pool.emplace_back(flip_arr_int_, last_start_point, last_end_point);

    for (auto &thread : thread_pool) {
      thread.join();
    }
  }

  return result;
}

#endif
