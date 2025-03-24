#pragma once
#ifndef _MATRIX_POWER_HPP_
#define _MATRIX_POWER_HPP_

#include "./identity.hpp"
#include "./mat_multiply.hpp"
#include "./md_linear_algebra.hpp"

template <typename T, class T1>
Array<T> Linalg::matrix_power(const Array<T> &matrix, usize power) {
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

  Array<T> result = Linalg::identity<T>(matrix.get_shape()[0]), omat = matrix;

  while (power > 0) {
    if (power & 1) {
      result = Linalg::mat_multiply<T, T, T>(result, omat);
    }

    omat = Linalg::mat_multiply<T, T, T>(omat, omat);
    power >>= 1;
  }

  return result;
}

template <typename T, class T1>
Array<T> Linalg::matrix_power(const ArraySlice<T> &matrix, usize power) {
  return Linalg::matrix_power<T>(
      Array<T>(*matrix.array_reference_, matrix.offset, matrix.shp_offset),
      power);
}

#endif
