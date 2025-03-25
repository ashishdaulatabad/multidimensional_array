#pragma once
#ifndef _IFFT_INT_HPP_
#define _IFFT_INT_HPP_

#include "../../utility/math.hpp"
#include "./ifft/c64.hpp"
#include "./md_fft.hpp"

/**
 * @note Source:
 * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
 * https://e-maxx.ru/algo/fft_multiply
 */
Array<c64> FFT::ifft_int(const Array<c64> &other) {
  usize n = other.get_size();
  usize i = 0;
  Array<c64> input(n, 0);
  if ((n & 1)) {
    for (usize index = 0; index < other.get_size(); ++index) {
      input.array_[index] = other.array_[index];
    }
    idft_subarray_inplace_without_div(input.array_, 0, n);
    return input;
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

    if (i > 1) {
      for (usize index = 0; index < n; index += i) {
        idft_subarray_inplace_without_div(input.array_, index, index + i);
      }
    }
  }

  ifft_inplace(input.array_, n, i);

  return input;
}

Array<c64> FFT::ifft_int(const Axis<c64> &other) {
  usize n = other.get_size();
  usize i = 0;
  Array<c64> input(n, 0);
  if ((n & 1) || n <= 64) {
    for (usize index = 0; index < other.get_size(); ++index) {
      input.array_[index] = other[index];
    }
    idft_subarray_inplace_without_div(input.array_, 0, n);
    return input;
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
      input.array_[index] = other[indexes.array_[index]];
    }
  }

  if (i > 1) {
    for (usize index = 0; index < n; index += i) {
      idft_subarray_inplace_without_div(input.array_, index, index + i);
    }
    ifft_inplace_without_div(input.array_, n, i);
  } else {
    ifft_inplace_without_div(input.array_, n, i);
  }

  return input;
}

// template <typename T>
Array<c64> FFT::ifft_int(const ArraySlice<c64> &values) {
  return FFT::ifft_int(
      Array<c64>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
