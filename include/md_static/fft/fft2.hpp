#pragma once
#ifndef _FFT2_HPP_
#define _FFT2_HPP_

#include "./fft_int.hpp"
#include "./md_fft.hpp"

template <typename T> Array<c64> FFT::fft2(const Array<T> &mat) {
  if (mat.get_shape_size() != 2) {
    throw std::runtime_error("Expected dimension to be 2, found " +
                             std::to_string(mat.get_shape_size()));
  }

  // Perform FFT row-wise
  Array<c64> result(mat);

  for (usize index = 0; index < result.get_shape()[0]; ++index) {
    Axis<c64> axis_ = result.get_nth_axis_reference(1, index);
    axis_ = fft_int(axis_);
  }

  for (usize index = 0; index < result.get_shape()[1]; ++index) {
    Axis<c64> axis_ = result.get_nth_axis_reference(0, index);
    axis_ = fft_int(axis_);
  }

  return result;
}

template <typename T> Array<c64> FFT::fft2(const ArraySlice<T> &array_ref) {
  return FFT::fft2(Array<T>(*array_ref.array_reference_, array_ref.offset,
                            array_ref.shp_offset));
}

#endif
