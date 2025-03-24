#pragma once
#ifndef _LU_DECOMPOSE_HPP_
#define _LU_DECOMPOSE_HPP_

#include "../functions/range.hpp"
#include "./md_linear_algebra.hpp"

template <typename Tres, typename T>
std::tuple<Array<Tres>, Array<Tres>, Array<Tres>, i32>
Linalg::lu_decompose(const Array<T> &matrix) {
  if (matrix.get_shape_size() != 2) {
    throw std::runtime_error("Given array should be of 2 dimension, found " +
                             std::to_string(matrix.get_shape_size()));
  }

  if (matrix.get_shape()[0] != matrix.get_shape()[1]) {
    throw std::runtime_error("Given matrix should be square.");
  }

  // Credits to algorithm: Rosetta code:
  // https://rosettacode.org/wiki/LU_decomposition#C++
  usize n = matrix.get_shape()[0];
  Array<Tres> L({n, n}, 0), U({n, n}, 0), P({n, n}, 0);
  Array<Tres> input(matrix);
  i32 sign = 1;

  Array<usize> permutation = MdArrayUtility::range<usize>(n);

  for (usize j = 0; j < n; ++j) {
    usize max_index = j;
    Tres max_value = std::abs(input.array_[n * permutation.array_[j] + j]);

    for (usize i = j + 1; i < n; ++i) {
      const Tres value = std::abs(input.array_[n * permutation.array_[i] + j]);

      if (value > max_value) {
        max_value = value;
        max_index = i;
      }
    }

    if (j != max_index) {
      sign = -sign;
      std::swap(permutation.array_[j], permutation.array_[max_index]);
    }

    usize jmax = permutation.array_[j];

#pragma omp parallel for
    for (usize i = j + 1; i < n; ++i) {
      usize imax = permutation.array_[i];
      input.array_[imax * n + j] /= input.array_[jmax * n + j];

      for (usize k = j + 1; k < n; ++k) {
        input.array_[imax * n + k] -=
            input.array_[imax * n + j] * input.array_[jmax * n + k];
      }
    }
  }

#pragma omp parallel for
  for (usize j = 0; j < n; ++j) {
    L.array_[j * n + j] = 1;

    for (usize i = j + 1; i < n; ++i) {
      L.array_[i * n + j] = input.array_[permutation.array_[i] * n + j];
    }

    for (usize i = 0; i <= j; ++i) {
      U.array_[i * n + j] = input.array_[permutation.array_[i] * n + j];
    }
  }

  for (usize index = 0; index < n; ++index) {
    P.array_[index * n + permutation.array_[index]] = 1;
  }

  return {L, U, P, sign};
}

template <typename Tres, typename T>
std::tuple<Array<Tres>, Array<Tres>, Array<Tres>, i32>
Linalg::lu_decompose(const ArraySlice<T> &matrix) {
  return Linalg::lu_decompose<Tres>(
      Array<T>(*matrix.array_reference_, matrix.offset, matrix.shp_offset));
}

#endif
