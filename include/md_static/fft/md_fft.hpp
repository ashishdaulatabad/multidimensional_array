#pragma once
#ifndef _MD_FFT_HPP_
#define _MD_FFT_HPP_

#include "../../md_complex/md_complex.hpp"
#include "../md_static_array/md_static_array.hpp"
#include "../md_static_array/md_static_axis_reference.hpp"

struct FFT {
private:
  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers
   */
  template <typename T> static Array<c64> fft_int(const Array<T> &other);

  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which FFT is to be computed
   * @return values of complex numbers
   */
  template <typename T> static Array<c64> fft_int(const ArraySlice<T> &other);

  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers
   */
  template <typename T> static Array<c64> fft_int(const Axis<T> &other);

  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which FFT is to be computed
   * @return values of complex numbers
   */
  static Array<c64> ifft_int(const Array<c64> &other);

  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which FFT is to be computed
   * @return values of complex numbers
   */
  static Array<c64> ifft_int(const ArraySlice<c64> &other);

  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which FFT is to be computed
   * @return values of complex numbers
   */
  static Array<c64> ifft_int(const Axis<c64> &other);

public:
  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers
   */
  template <typename T> static Array<c64> fft(const Array<T> &other);

  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which FFT is to be computed
   * @return values of complex numbers
   */
  template <typename T> static Array<c64> fft(const ArraySlice<T> &other);

  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which FFT is to be computed
   * @return values of complex numbers
   */
  template <typename T> static Array<T> ifft(const Array<c64> &other);

  /**
   * @note Source:
   * https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation
   * https://e-maxx.ru/algo/fft_multiply
   * @brief Compute 1 dimensional FFT on 1 dimensional array
   * @tparam T type of an array
   * @param other array for which FFT is to be computed
   * @return values of complex numbers
   */
  template <typename T> static Array<T> ifft(const ArraySlice<c64> &other);

  /**
   * @brief Compute 2 dimensional FFT on 2 dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers in 2D-array
   */
  template <typename T> static Array<c64> fft2(const Array<T> &other);

  /**
   * @brief Compute 2 dimensional FFT on 2 dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers in 2D-array
   */
  template <typename T> static Array<c64> fft2(const ArraySlice<T> &other);

  /**
   * @brief Compute 2 dimensional FFT on 2 dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers in 2D-array
   */
  template <typename T> static Array<T> ifft2(const Array<c64> &other);

  /**
   * @brief Compute 2 dimensional FFT on 2 dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers in 2D-array
   */
  template <typename T> static Array<T> ifft2(const ArraySlice<c64> &other);

  /**
   * @brief Compute n dimensional FFT on n dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers in 2D-array
   */
  template <typename T> static Array<c64> fftn(const Array<T> &other);

  /**
   * @brief Compute n dimensional FFT on n dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers in 2D-array
   */
  template <typename T> static Array<c64> fftn(const ArraySlice<T> &other);

  /**
   * @brief Compute n dimensional FFT on n dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers in nD-array
   */
  template <typename T> static Array<T> ifftn(const Array<c64> &other);

  /**
   * @brief Compute n dimensional FFT on n dimensional array
   * @tparam T type of an array
   * @param other array for which IFFT is to be computed
   * @return values of complex numbers in ND-array
   */
  template <typename T> static Array<T> ifftn(const ArraySlice<c64> &other);
};

#endif
