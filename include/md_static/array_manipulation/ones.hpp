#pragma once
#ifndef _ONES_HPP_
#define _ONES_HPP_

#include "./md_array_manipulation.hpp"

template <typename T> Array<T> Manip::ones(const usize size) {
  return Array<T>(size, 1);
}

template <typename T> Array<T> Manip::ones(const std::vector<usize> &shape) {
  return Array<T>(shape, 1);
}

#endif
