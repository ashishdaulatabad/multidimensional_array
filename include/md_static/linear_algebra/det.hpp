#pragma once
#ifndef _C_DET_HPP_
#define _C_DET_HPP_

#include "./md_linear_algebra.hpp"

template <typename T> f128 Linalg::det(const Array<T> &matrix) {
  const auto [L, U, P, sign] = Linalg::lu_decompose<T>(matrix);
  const usize n = matrix.get_shape()[0];
  f128 det = 1;

  for (usize index = 0; index < n; ++index) {
    det *= U.array_[index * n + index];
  }

  return sign * det;
}

template <typename T> f128 Linalg::det(const ArraySlice<T> &matrix) {
  return Linalg::det(
      Array<T>(*matrix.array_reference_, matrix.offset, matrix.shp_offset));
}

#endif
