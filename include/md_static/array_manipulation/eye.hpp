#pragma once
#ifndef _EYE_HPP_
#define _EYE_HPP_

#include "./md_array_manipulation.hpp"

template <typename T>
Array<T> Manip::eye(const usize rows, const usize cols, const usize koff) {
  const usize r = rows, c = (cols == -1 ? rows : cols), incr = c + 1;
  Array<T> result({rows, cols}, 0);

  for (usize index = koff; index < result.get_size(); index += incr) {
    result.array_[index] = 1;
  }

  return result;
}

#endif
