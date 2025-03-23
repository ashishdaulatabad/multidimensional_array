#pragma once
#ifndef _COMPARE_HPP_
#define _COMPARE_HPP_

#include "./md_static_array_utility.hpp"

template <typename T1, typename T2>
bool Utils::compare(const Array<T1> &first, const Array<T2> &other) {
  if (first.get_shape_size() != other.get_shape_size()) {
    return false;
  }

  for (usize i = 0; i < first.get_shape_size(); ++i) {
    if (first.get_shape()[i] != other.get_shape()[i]) {
      return false;
    }
  }

  if constexpr (MdTypeInfer::is_any_one_floating<T1, T2>::value) {
    for (usize i = 0; i < first.get_size(); ++i) {
      if (std::abs(first.array_[i] - other.array_[i]) >
          1e-8 + 1e-6 * std::abs(other.array_[i])) {
        return false;
      }
    }

    return true;
  }

  for (usize i = 0; i < first.get_size(); ++i) {
    if (first.array_[i] != other.array_[i]) {
      return false;
    }
  }

  return true;
}

#endif
