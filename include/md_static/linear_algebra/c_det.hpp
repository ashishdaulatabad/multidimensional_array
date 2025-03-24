#pragma once
#ifndef _DET_HPP_
#define _DET_HPP_

#include "../../md_complex/md_complex.hpp"
#include "./md_linear_algebra.hpp"

template <typename T> c128 Linalg::c_det(const Array<T> &mat) {
  auto [L, U, P, sign] = Linalg::lu_decompose<T>(mat);
  const usize n = mat.get_shape()[0];
  c128 det = 1;

  for (usize index = 0; index < n; ++index) {
    det *= U.array_[index * n + index];
  }

  return sign * det;
}

template <typename T> c128 Linalg::c_det(const ArraySlice<T> &mat) {
  return Linalg::det(
      Array<T>(*mat.array_reference_, mat.offset, mat.shp_offset));
}

#endif
