#pragma once
#ifndef __CONVOLVE_1D_HPP__
#define __CONVOLVE_1D_HPP__

#include "./md_signal.hpp"

template <typename T, typename T1, typename T2>
Array<T> Signal::convolve1d(const Array<T1> &input, const Array<T2> &other) {
  if (input.get_shape_size() != 1) {
    throw std::runtime_error(
        "Expected 1 dimensional array as first argument, found " +
        std::to_string(input.get_shape_size()));
  }
  if (other.get_shape_size() != 1) {
    throw std::runtime_error(
        "Expected 1 dimensional array as second argument, found " +
        std::to_string(other.get_shape_size()));
  }

  Array<T> result({input.get_size() + other.get_size() - 1}, 0);

  for (i32 i = 0; i < input.get_size(); ++i) {
    for (i32 j = 0; j < other.get_size() && i - j >= 0; ++j) {
      result.array_[i] += other.array_[j] * input.array_[i - j];
    }
  }

  for (i32 i = input.get_size(); i < result.get_size(); ++i) {
    for (i32 j = i - input.get_size() + 1; j < other.get_size(); ++j) {
      result.array_[i] += other.array_[j] * input.array_[i - j];
    }
  }
  return result;
}

#endif
