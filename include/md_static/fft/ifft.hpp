#pragma once
#ifndef _IFFT_HPP_
#define _IFFT_HPP_

#include "../../utility/math.hpp"
#include "./ifft/c64.hpp"
#include "./md_fft.hpp"

/**
 * @note Source:
 * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
 * https://e-maxx.ru/algo/fft_multiply
 */
template <typename T> Array<T> FFT::ifft(const Array<c64> &other) {
  usize n = other.get_size();
  usize i = 0;
  Array<c64> input(n, 0);
  if ((n & 1) || n <= 64) {
    for (usize index = 0; index < n; ++index) {
      input.array_[index] = other.array_[index];
    }
    // __idft_internal(input, 0, n);
    idft_subarray_inplace(input.array_, 0, n);
    return Array<T>(input);
  } else {
    // Get last zero numbers
    const usize ls = ((n ^ (n - 1)) + 1) >> 1;
    Array<usize> indexes(n, 0);

    i = n;
    usize j = 1;

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
  }

  if (i > 1) {
    for (usize index = 0; index < n; index += i) {
      idft_subarray_inplace_without_div(input.array_, index, index + i);
    }

    ifft_inplace(input.array_, n, i);
  } else {
    ifft_inplace(input.array_, n, i);
  }

  // __perform_fft_in_place(input, i);

  return Array<T>(input);
}

template <typename T> Array<T> FFT::ifft(const ArraySlice<c64> &values) {
  return ifft<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
