#pragma once
#ifndef _IFFT2_HPP_
#define _IFFT2_HPP_

#include "./ifft_int.hpp"
#include "./md_fft.hpp"

template <typename T> Array<T> FFT::ifft2(const Array<c64> &mat) {
  if (mat.get_shape_size() != 2) {
    throw std::runtime_error("Expected dimension to be 2, found " +
                             std::to_string(mat.get_shape_size()));
  }

  Array<c64> result(mat);

  for (usize index = 0; index < result.shape[0]; ++index) {
    auto axis_reference = result.get_nth_axis_reference(1, index);
    axis_reference = ifft_int(axis_reference);
  }

  for (usize index = 0; index < result.shape[1]; ++index) {
    auto axis_reference = result.get_nth_axis_reference(0, index);
    axis_reference = ifft_int(axis_reference);
  }

  result /= result.get_size();

  return Array<T>(result);
}

template <typename T> Array<T> FFT::ifft2(const ArraySlice<c64> &array_ref) {
  return FFT::ifft2(Array<T>(*array_ref.array_reference_, array_ref.offset,
                             array_ref.shp_offset));
}

#endif
