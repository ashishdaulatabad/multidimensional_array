#pragma once
#ifndef _IDENTITY_HPP_
#define _IDENTITY_HPP_
#include "./md_linear_algebra.hpp"

template <typename T> Array<T> Linalg::identity(const usize n) {
  Array<T> result({n, n}, 0);

  for (usize index = 0; index < n * n; index += (n + 1)) {
    result.array_[index] = 1;
  }

  return result;
}

template <typename T> bool Linalg::is_identity(const Array<T> &other) {
  if (other.shp_size != 2 || other.shape[0] != other.shape[1]) {
    return false;
  }

  const usize n = other.shape[0];

  for (usize index = 0; index < other.get_size(); ++index) {
    if (other.array_[index] > 1 || other.array_[index] < 0) {
      return false;
    }
    if (other.array_[index] && (index % (n + 1) != 0)) {
      return false;
    }
    if (!other.array_[index] && (index % (n + 1) == 0)) {
      return false;
    }
  }

  return true;
}

template <typename T> bool Linalg::is_identity(const ArraySlice<T> &other) {
  return Linalg::is_identity(
      Array<T>(*other.array_reference, other.offset, other.shp_offset));
}

#endif
