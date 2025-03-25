#pragma once
#ifndef _IFFTN_HPP_
#define _IFFTN_HPP_

#include "./ifft.hpp"
#include "./ifft2.hpp"
#include "./ifft_int.hpp"
#include "./md_fft.hpp"

template <typename T> Array<T> FFT::ifftn(const Array<c64> &ndarray) {
  switch (ndarray.get_shape_size()) {
  case 1:
    return FFT::ifft<T>(ndarray);
  case 2:
    return FFT::ifft2<T>(ndarray);
  default:
    break;
  }

  Array<c64> result(ndarray);

  for (usize k = 0; k < ndarray.get_shape_size(); ++k) {
    const usize loop = result.get_axis_reference(k).get_total_axes();

    for (usize index = 0; index < loop; ++index) {
      auto axis_ref = result.get_nth_axis_reference(k, index);
      axis_ref = ifft_int(axis_ref);
    }
  }

  result /= result.get_size();

  return result;
}

template <typename T> Array<T> FFT::ifftn(const ArraySlice<c64> &ndarray) {
  return FFT::ifftn<T>(Array<c64>(*ndarray.array_reference_, ndarray.offset,
                                  ndarray.shp_offset));
}

#endif
