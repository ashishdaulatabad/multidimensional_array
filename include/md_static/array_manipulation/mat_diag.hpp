#pragma once
#ifndef _MAT_DIAG_HPP_
#define _MAT_DIAG_HPP_

#include "./md_array_manipulation.hpp"

template <typename T>
Array<T> Manip::mat_diag(const Array<T> &matrix, const usize offset) {
  if (matrix.get_shape_size() != 2) {
    throw std::runtime_error(
        "Given input should be of dimension 2. Found dimension " +
        std::to_string(matrix.get_shape_size()) + ".");
  }

  const usize rows = matrix.get_shape()[0];
  const usize cols = matrix.get_shape()[1];

  const usize min_row = std::min(rows, cols);
  const usize overall_size = std::min(min_row - offset, usize(0));

  Array<T> result(overall_size);

  for (usize index = offset, res_index = 0; res_index < result.get_size();
       index += (cols + 1), ++res_index) {
    result.array_[res_index] = matrix.array_[index];
  }

  return result;
}

template <typename T>
Array<T> Manip::mat_diag(const ArraySlice<T> &matrix, const usize offset) {
  return mat_diag<T>(
      Array<T>(*matrix.array_reference_, matrix.offset, matrix.shp_offset),
      offset);
}

#endif
