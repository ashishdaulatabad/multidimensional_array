#pragma once
#ifndef _TRANSPOSE_HPP_
#define _TRANSPOSE_HPP_
#include "./md_static_array_utility.hpp"

template <typename T1, typename T2>
Array<T2> Utils::transpose(const Array<T1> &first) {
  if (first.get_shape_size() != 2) {
    throw std::runtime_error("Matrix Error: Expected dimension 2.");
  }

  Array<T2> result({first.get_shape()[1], first.get_shape()[0]}, 0);

  for (usize res_index = 0; res_index < first.get_shape()[1]; ++res_index) {
    for (usize f_index = res_index, incr = 0; f_index < first.get_size();
         f_index += first.get_shape()[1], ++incr) {
      result.array_[res_index * first.get_shape()[0] + incr] =
          first.array_[f_index];
    }
  }

  return result;
}

template <typename T> Array<T> Utils::transpose(const ArraySlice<T> &values) {
  return Utils::transpose<T>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset));
}

#endif
