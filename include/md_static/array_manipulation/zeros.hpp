#pragma once
#ifndef _ZEROS_HPP_
#define _ZEROS_HPP_

#include "./md_array_manipulation.hpp"

template <typename T> Array<T> Manip::zeros(const usize size) {
  return Array<T>(size, 0);
}

template <typename T> Array<T> Manip::zeros(const std::vector<usize> &shape) {
  return Array<T>(shape, 0);
}

#endif
