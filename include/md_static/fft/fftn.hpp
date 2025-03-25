#pragma once
#ifndef _FFTN_HPP_
#define _FFTN_HPP_

#include "./fft.hpp"
#include "./fft2.hpp"
#include "./fft_int.hpp"
#include "./md_fft.hpp"

template <typename T> Array<c64> FFT::fftn(const Array<T> &ndarray) {
  switch (ndarray.get_shape_size()) {
  case 1:
    return FFT::fft(ndarray);
  case 2:
    return FFT::fft2(ndarray);
  default:
    break;
  }

  Array<c64> result(ndarray);
  // Perform ndarray FFT
  for (usize k = 0; k < ndarray.get_shape_size(); ++k) {
    const usize loop = result.get_axis_reference(k).get_total_axes();

    for (usize index = 0; index < loop; ++index) {
      auto axis_ref = result.get_nth_axis_reference(k, index);
      axis_ref = fft_int(axis_ref);
    }
  }

  return result;
}

template <typename T> Array<c64> FFT::fftn(const ArraySlice<T> &ndarray) {
  return FFT::fftn(
      Array<T>(*ndarray.array_reference_, ndarray.offset, ndarray.shp_offset));
}

#endif
