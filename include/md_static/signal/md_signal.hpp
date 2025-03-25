#pragma once
#ifndef __MD_SIGNAL_HPP__
#define __MD_SIGNAL_HPP__

#include "../md_static_array/md_static_array.hpp"

struct Signal {
  /**
   * @brief Perform 1D direct convolution.
   * @tparam T resultant type
   * @tparam T1 first type
   * @tparam T2 second input type
   * @param input first 1D array
   * @param other other 1D array
   * @returns result of size len(input) + len(other) - 1
   */
  template <typename T, typename T1, typename T2>
  static Array<T> convolve1d(const Array<T1> &input, const Array<T2> &other);

  // template <typename T, typename T1>
  // Array<T> convolve1d(const ArraySlice<T1>& slice);
};

#endif