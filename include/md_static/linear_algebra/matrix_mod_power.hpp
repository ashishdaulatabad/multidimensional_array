#pragma once
#ifndef _MATRIX_MOD_POWER_HPP_
#define _MATRIX_MOD_POWER_HPP_

#include "./identity.hpp"
#include "./mat_mod_multiply.hpp"
#include "./md_linear_algebra.hpp"

template <typename T, class T1>
Array<T> Linalg::matrix_mod_power(const Array<T> &matrix, const usize pow,
                                  const usize mod) {
  if (matrix.get_shape_size() != 2) {
    throw std::runtime_error(
        "Given input should be of dimension 2. Found dimension " +
        std::to_string(matrix.get_shape_size()) + ".");
  }

  if (matrix.get_shape()[0] != matrix.get_shape()[1]) {
    throw std::runtime_error(
        "Given input matrix should be square. Found dimension (" +
        std::to_string(matrix.get_shape()[0]) + ", " +
        std::to_string(matrix.get_shape()[1]) + ").");
  }
  auto power = pow;
  Array<T> result = Linalg::identity<T>(matrix.get_shape()[0]), omat = matrix;

  while (power > 0) {
    if (power & 1) {
      result = Linalg::mat_mod_multiply(result, omat, mod);
    }

    omat = Linalg::mat_mod_multiply(omat, omat, mod);
    power >>= 1;
  }

  return result;
}

template <typename T, class T1>
Array<T> Linalg::matrix_mod_power(const ArraySlice<T> &matrix,
                                  const usize power, const usize mod) {
  return Linalg::matrix_mod_power<T>(
      Array<T>(*matrix.array_reference_, matrix.offset, matrix.shp_offset),
      power, mod);
}

#endif
