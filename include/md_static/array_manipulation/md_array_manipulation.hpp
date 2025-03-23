#pragma once

#ifndef _MD_ARRAY_MANIPULATION_HPP_
#define _MD_ARRAY_MANIPULATION_HPP_

#include "../md_static_array/md_static_array.hpp"

struct MdArrayManipulate {
  /**
   * @brief Create array with entries zero
   * @tparam T type of array
   * @param size size to be created
   * @returns array of size size with all entries as zeros
   */
  template <typename T> static Array<T> zeros(const usize size);

  /**
   * @brief Create array with entries zero
   * @tparam T type of array
   * @param shape size to be created
   * @returns array of size size with all entries as zeros
   */
  template <typename T> static Array<T> zeros(const std::vector<usize> &shape);

  /**
   * @brief Create array with entries one
   * @tparam T type of array
   * @param size size to be created
   * @returns array of size size with all entries as one
   */
  template <typename T> static Array<T> ones(const usize size);

  /**
   * @brief Create array with entries one
   * @tparam T type of array
   * @param shape size to be created
   * @returns array of size size with all entries as one
   */
  template <typename T> static Array<T> ones(const std::vector<usize> &shape);

  /**
   * @brief Create an identity matrix
   * @tparam T type of array
   * @param rows total rows to create
   * @param cols cols to create (optional, will create square matrix if not
   * mentioned)
   * @param koffset filling ones will start from offset
   * @returns array of size size with all entries as zeros
   */
  template <typename T>
  static Array<T> eye(const usize rows, const usize cols = -1,
                      const usize koffset = 0);

  /**
   * @brief Return diagonal from a 2d array
   * @tparam T type of array
   * @param matrix 2d array from which we read diagonal from
   * @param koffset diagonal reading from start offset
   * @returns array of size size with all entries as zeros
   */
  template <typename T>
  static Array<T> mat_diag(const Array<T> &matrix, const usize koffset = 0);
  /**
   * @brief Return diagonal from a 2d array
   * @tparam T type of array
   * @param matrix 2d array from which we read diagonal from
   * @param koffset diagonal reading from start offset
   * @returns array of size size with all entries as zeros
   */
  template <typename T>
  static Array<T> mat_diag(const ArraySlice<T> &rows, const usize koffset = 0);

  /**
   * @brief Create array with entries as vandermonte
   * @tparam T type of array
   * @param array 1d array
   * @param n total number of columns (optional, will create a square
   * matrix)
   * @param increasing_order will arrange each rows with ascending order of
   * exponents when set to true
   * @returns array of size {|array|, n}
   */
  template <typename T>
  static Array<T> vandermonte(const Array<T> &array, const i64 n = -1,
                              const bool increasing_order = false);
  /**
   * @brief Create array with entries as vandermonte
   * @tparam T type of array
   * @param array 1d array
   * @param n total number of columns (optional, will create a square
   * matrix)
   * @param increasing_order will arrange each rows with ascending order of
   * exponents when set to true
   * @returns array of size {|array|, n}
   */
  template <typename T>
  static Array<T> vandermonte(const ArraySlice<T> &rows, const i64 n = -1,
                              const bool increasing_order = false);

  template <typename T>
  static Array<T> flip(const Array<T> &__ndarray, const i32 axis = -1);

  template <typename T>
  static Array<T> flip(const ArraySlice<T> &__ndarray, const i32 axis = -1);

  template <typename T3, typename T1, typename T2>
  static Array<T3> linspace(const Array<T1> &first, const Array<T2> &other,
                            const usize numbers = 50);

  template <typename T3, typename T1, typename T2>
  static Array<T3> linspace(const ArraySlice<T1> &first, const Array<T2> &other,
                            const usize numbers = 50);

  template <typename T3, typename T1, typename T2>
  static Array<T3> linspace(const ArraySlice<T1> &first,
                            const ArraySlice<T2> &other,
                            const usize           numbers = 50);

  template <typename T3, typename T1, typename T2>
  static Array<T3> linspace(const Array<T1> &first, const ArraySlice<T2> &other,
                            const usize numbers = 50);
};

typedef MdArrayManipulate Manip;

#endif
