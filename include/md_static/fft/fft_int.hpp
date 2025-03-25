#pragma once
#ifndef _FFT_INT_HPP_
#define _FFT_INT_HPP_

#include "../../utility/math.hpp"
#include "./fft/c64.hpp"
#include "./md_fft.hpp"

template <typename T> Array<c64> FFT::fft_int(const Array<T> &other) {
  usize n = other.get_size();
  usize i = 0;
  Array<c64> input(n);
  if ((n & 1)) {
    for (usize index = 0; index < n; ++index) {
      input.array_[index] = other.array_[index];
    }

    dft_subarray_inplace(input.array_, 0, n);
    return input;
  } else {
    // Get last zero numbers
    const usize ls = ((n ^ (n - 1)) + 1) >> 1;
    Array<usize> indexes(n, 0);
    usize j = 1;
    i = n;

    while ((i & 1) == 0) {
      for (usize k = (i >> 1); k < i; ++k) {
        indexes.array_[k] = j;
      }
      i >>= 1;
      j <<= 1;
    }

    for (usize k = 1; k < i; ++k) {
      indexes.array_[k] = indexes.array_[k - 1] + ls;
      indexes.array_[k + (n >> 1)] = indexes.array_[k] + 1;
    }
    for (usize index = i; index < (n >> 1); index <<= 1) {
      for (usize k = 0; k < index; ++k) {
        indexes.array_[k + index] += indexes.array_[k];
        indexes.array_[k + index + (n >> 1)] = indexes.array_[k + index] + 1;
      }
    }

    for (usize index = 0; index < n; ++index) {
      input.array_[index] = other.array_[indexes.array_[index]];
    }

    if (i > 1) {
      for (usize index = 0; index < n; index += i) {
        dft_subarray_inplace(input.array_, index, index + i);
      }
    }
  }

  fft_inplace(input.array_, n, i);

  return input;
}

template <typename T> Array<c64> FFT::fft_int(const Axis<T> &other) {
  usize n = other.get_size();
  usize i = 0;
  Array<c64> input(n);

  if ((n & 1) || n <= 64) {
    for (usize index = 0; index < n; ++index) {
      input.array_[index] = other[index];
    }
    // return __dft_ret_internal(input, 0, n);
    dft_subarray_inplace(input.array_, 0, n);
    return input;
  } else {
    // Get last zero numbers
    const usize ls = ((n ^ (n - 1)) + 1) >> 1;

    Array<usize> indexes(n, 0);

    usize j = 1;
    i = n;
    while ((i & 1) == 0) {
      for (usize k = (i >> 1); k < i; ++k) {
        indexes.array_[k] = j;
      }
      i >>= 1;
      j <<= 1;
    }

    for (usize k = 1; k < i; ++k) {
      indexes.array_[k] = indexes.array_[k - 1] + ls;
      indexes.array_[k + (n >> 1)] = indexes.array_[k] + 1;
    }
    for (usize index = i; index < (n >> 1); index <<= 1) {
      for (usize k = 0; k < index; ++k) {
        indexes.array_[k + index] += indexes.array_[k];
        indexes.array_[k + index + (n >> 1)] = indexes.array_[k + index] + 1;
      }
    }

    for (usize index = 0; index < n; ++index) {
      input.array_[index] = other[indexes.array_[index]];
    }
  }

  if (i > 1) {
    for (usize index = 0; index < n; index += i) {
      dft_subarray_inplace(input.array_, index, index + i);
    }

    fft_inplace(input.array_, n, i);
  } else {
    fft_inplace(input.array_, n, i);
  }

  return input;
}

template <typename T> Array<c64> FFT::fft_int(const ArraySlice<T> &other) {
  return fft_int(
      Array(*other.array_reference_, other.offset, other.shp_offset));
}

#endif
