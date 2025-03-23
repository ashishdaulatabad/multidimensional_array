#pragma once
#ifndef _MD_STATIC_ARRAY_UTILITY_HPP_
#define _MD_STATIC_ARRAY_UTILITY_HPP_
#include <functional>

#include "./../md_static_array/md_static_array.hpp"

struct MdArrayUtility {
  /**
   * @brief Execute a one-to-one mapping function of an array,
   * @tparam T array type
   * @tparam _func variable function that maps the values
   * @param values values to map
   * @param function_exec function to execute
   * @returns Array of elements
   */
  template <typename T>
  static Array<T> map(const Array<T> &values,
                      const fn<T(const T &)> &function_exec);

  /**
   * @brief Execute a one-to-one mapping function of an array,
   * @tparam T array type
   * @tparam _func variable function that maps the values
   * @param values values to map
   * @param function_exec function to execute
   * @returns Array of elements
   */
  template <typename T>
  static Array<T> map(const ArraySlice<T> &values,
                      const fn<T(const T &)> &function_exec);

  /**
   * @brief Reduce array of values to a single value function.
   * @todo: Merge along mentioned axis
   * @tparam T type of array
   * @param values values to perform
   * @param function_exec function to execute (arguments are previous value
   * and current value)
   * @param init, value to initialize before accumulate.
   * @returns single value
   */
  template <typename T, typename fn_>
  static Array<T> reduce(const Array<T> &values, const fn_ &function_exec,
                         const T init = 0, const i32 axis = -1);

  /**
   * @brief Reduce array of values to a single value function.
   * @todo: Merge along mentioned axis
   * @tparam T type of array
   * @param values values to perform
   * @param function_exec function to execute (arguments are previous value
   * and current value)
   * @param init, value to initialize before accumulate.
   * @returns single value
   */
  template <typename T, typename fn_>
  static Array<T> reduce(const ArraySlice<T> &values, const fn_ &function_exec,
                         const T init = 0, const i32 axis = -1);

  /**
   * @brief Execute accumulate function, and merge these values based on
   * merge function
   * @todo: Merge along mentioned axis
   * @tparam T type of array
   * @param values values to perform
   * @param function_exec function to execute (arguments are previous value
   * and current value)
   * @param init, value to initialize before accumulate.
   * @returns single value
   */
  template <typename T, typename _func, typename _merge_func>
  static T accumulate_and_merge_fn(const Array<T> &values,
                                   const _func &function_exec,
                                   const _merge_func &merge_func, const T init);

  /**
   * @brief Execute accumulate function, and merge these values based on
   * merge function
   * @todo: Merge along mentioned axis
   * @tparam T type of array
   * @param values values to perform
   * @param function_exec function to execute (arguments are previous value
   * and current value)
   * @param init, value to initialize before accumulate.
   * @returns single value
   */
  template <typename T, typename _func, typename _merge_func>
  static T accumulate_and_merge_fn(const ArraySlice<T> &values,
                                   const _func &function_exec,
                                   const _merge_func &merge_func, const T init);

  /**
   * @brief sqrt of all values in list values
   * @param values list of values
   * @returns List of values
   */
  template <typename T> static Array<T> sqrt(const Array<T> &values);

  /**
   * @brief sqrt of all values in list values
   * @param values list of values
   * @returns List of values
   */
  template <typename T> static Array<T> sqrt(const ArraySlice<T> &values);

  /**
   * @brief absolute value of all values in list values
   * @param values list of values
   * @returns List of values
   */
  template <typename T> static Array<T> abs(const Array<T> &values);

  /**
   * @brief absolute value of all values in list values
   * @param values list of values
   * @returns List of values
   */
  template <typename T> static Array<T> abs(const ArraySlice<T> &values);

  /**
   * @brief Sum of all the values in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> sum(const Array<T> &values, const T init = 0,
                      const i32 axis = -1);

  /**
   * @brief Sum of all the values in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> sum(const ArraySlice<T> &values, const T init = 0,
                      const i32 axis = -1);

  /**
   * @brief Mean of all the values in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<f64> mean(const Array<T> &values, const T init = 0,
                         const i32 axis = -1);

  /**
   * @brief Mean of all the values in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<f64> mean(const ArraySlice<T> &values, const T init = 0,
                         const i32 axis = -1);

  /**
   * @brief Maximum of all the values along given axis
   * @param values List of all values
   * @param axis Given axis along which function is checked
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> amax(const Array<T> &values, const i32 axis = -1);

  /**
   * @brief Maximum of all the values along given axis
   * @param values List of all values
   * @param axis Given axis along which function is checked
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> amax(const ArraySlice<T> &values, const i32 axis = -1);

  /**
   * @brief Maximum of all the values along given axis
   * @param values List of all values
   * @param axis Given axis along which function is checked
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<usize> argmax(const Array<T> &values, const i32 axis = -1);

  /**
   * @brief Maximum of all the values along given axis
   * @param values List of all values
   * @param axis Given axis along which function is checked
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T = usize>
  static Array<usize> argmax(const ArraySlice<T> &values, const i32 axis = -1);

  /**
   * @brief Minimum of all the values along given axis
   * @param values List of all values
   * @param axis Given axis along which function is checked
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> amin(const Array<T> &values, const i32 axis = -1);

  /**
   * @brief Minimum of all the values along given axis
   * @param values List of all values
   * @param axis Given axis along which function is checked
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> amin(const ArraySlice<T> &values, const i32 axis = -1);

  /**
   * @brief Minimum indexes of all the values along given axis
   * @param values List of all values
   * @param axis Given axis along which function is checked
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<usize> argmin(const Array<T> &values, const i32 axis = -1);

  /**
   * @brief Minimum indexes of all the values along given axis
   * @param values List of all values
   * @param axis Given axis along which function is checked
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<usize> argmin(const ArraySlice<T> &values, const i32 axis = -1);

  /**
   * @brief GCD of all the values combined in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> gcd(const Array<T> &values, const i32 axis = -1);

  /**
   * @brief GCD of all the values combined in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> gcd(const ArraySlice<T> &values, const i32 axis = -1);

  /**
   * @brief LCM of all the values combined in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> lcm(const Array<T> &values, const i32 axis = -1);

  /**
   * @brief LCM of all the values combined in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return Single number (or ndarray depending on axis)
   */
  template <typename T>
  static Array<T> lcm(const ArraySlice<T> &values, const i32 axis = -1);

  /**
   * @brief Root Mean Square of all the values in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return single number
   */
  template <typename T>
  static f128 rms(const Array<T> &values, const T init = 0);

  /**
   * @brief Root Mean Square of all the values in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return single number
   */
  template <typename T>
  static f128 rms(const ArraySlice<T> &values, const T init = 0);

  /**
   * @brief Standard Deviation of all the values in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return single number
   */
  template <typename T> static f128 std_dev(const Array<T> &values);

  /**
   * @brief Standard Deviation of all the values in the list values
   * @param values List of all values
   * @param init value to be initialized with
   * @return single number
   */
  template <typename T> static f128 std_dev(const ArraySlice<T> &values);

  /**
   * @brief compute log of values to the base 10
   */
  template <typename T> static Array<T> log10(const Array<T> &values);

  /**
   * @brief compute log of values to the base 10
   */
  template <typename T> static Array<T> log10(const ArraySlice<T> &values);

  /**
   * @brief compute log of values to the base 2
   */
  template <typename T> static Array<T> log2(const Array<T> &values);

  /**
   * @brief compute log of values to the base 2
   */
  template <typename T> static Array<T> log2(const ArraySlice<T> &values);

  /**
   * @brief compute natural logarithm of valuess
   */
  template <typename T> static Array<T> log(const Array<T> &values);

  /**
   * @brief compute natural logarithm of valuess
   */
  template <typename T> static Array<T> log(const ArraySlice<T> &values);

  /**
   * @brief compute mod power of integers
   */
  template <typename T,
            class = typename std::enable_if<std::is_integral<T>::value>::type>
  static Array<T> mod_pow(const Array<T> &values, const usize power,
                          const usize mod);

  /**
   * @brief compute mod power of integers
   */
  template <typename T,
            class = typename std::enable_if<std::is_integral<T>::value>::type>
  static Array<T> mod_pow(const ArraySlice<T> &values, const usize power,
                          const usize mod);

  /**
   * @brief compute mod power of integer raise to array of values
   * @tparam T type of integer values in array
   * @param n a number value
   * @param values list of exponents
   * @param mod value to perform mod_power
   * @returns list of values
   */
  template <typename T,
            class = typename std::enable_if<std::is_integral<T>::value>::type>
  static Array<T> mod_pow(const u64 n, const Array<T> &values, const usize mod);

  /**
   * @brief compute mod power of integer raise to array of values
   * @tparam T type of integer values in array
   * @param n a number value
   * @param values list of exponents
   * @param mod value to perform mod_power
   * @returns list of values
   */
  template <typename T,
            class = typename std::enable_if<std::is_integral<T>::value>::type>
  static Array<T> mod_pow(const u64 n, const ArraySlice<T> &values,
                          const usize mod);

  /**
   * @brief compute power of values
   * @tparam T type of array
   * @param values array of values to compute
   * @param power exponent
   * @returns new arrays with value raise to power
   */
  template <typename T> static Array<T> pow(const Array<T> &values, f64 power);

  /**
   * @brief compute power of values
   * @tparam T type of array
   * @param values array of values to compute
   * @param power exponent
   * @returns new arrays with value raise to power
   */
  template <typename T>
  static Array<T> pow(const ArraySlice<T> &values, f64 power);

  /**
   * @brief compute sine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed sine values
   */
  template <typename T> static Array<T> sin(const Array<T> &values);

  /**
   * @brief compute sine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed sine values
   */
  template <typename T> static Array<T> sin(const ArraySlice<T> &values);

  /**
   * @brief compute sine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed sine values
   */
  template <typename T> static Array<T> cos(const Array<T> &values);

  /**
   * @brief compute sine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed sine values
   */
  template <typename T> static Array<T> cos(const ArraySlice<T> &values);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> tan(const Array<T> &values);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> tan(const ArraySlice<T> &values);

  /**
   * @brief compute cotangent of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cotangent values
   */
  template <typename T> static Array<T> cot(const Array<T> &values);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> cot(const ArraySlice<T> &values);

  /**
   * @brief compute secant of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed secant values
   */
  template <typename T> static Array<T> sec(const Array<T> &values);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> sec(const ArraySlice<T> &values);

  /**
   * @brief compute cosecant of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosecant values
   */
  template <typename T> static Array<T> cosec(const Array<T> &values);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> cosec(const ArraySlice<T> &values);

  /**
   * @brief compute inverse sin of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed inverse sin
   */
  template <typename T> static Array<T> arcsin(const Array<T> &values);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> arcsin(const ArraySlice<T> &values);

  /**
   * @brief compute inverse cos of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed inverse cos
   */
  template <typename T> static Array<T> arccos(const Array<T> &values);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> arccos(const ArraySlice<T> &values);

  /**
   * @brief compute tan inverse of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed inverse tan
   */
  template <typename T> static Array<T> arctan(const Array<T> &values);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> arctan(const ArraySlice<T> &values);

  /**
   * @brief Generate evenly spaced values
   * @tparam T type of array
   * @param start start value for the array, if end and spacing is not
   * specified, $start$ will be the size of the array
   * @param end upper limit of the array
   *
   */
  template <typename T, typename Ts = T,
            class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
  static Array<T> range(const T start, const T end = -1, const Ts spacing = 0);

  /**
   * @brief Transpose of matrix: creates a new transposed array
   * @todo Perform for nd array
   * @tparam _T1 type of __first array
   * @tparam _T2 type of result array
   * @param __first first 2d array of shape (a1, a2) (m are
   * total dimensions of array a)
   * @returns new 2d array of shape (a2, a1)
   */
  template <typename _T1, typename _T2>
  static Array<_T2> transpose(const Array<_T1> &__other);

  /**
   * @brief compute cosine of values, given in radian
   * @tparam T type of array
   * @param values array of values to compute
   * @returns array of computed cosine values
   */
  template <typename T> static Array<T> transpose(const ArraySlice<T> &values);

  /**
   * @brief Cumulative sum of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @param axis Axis to perform cumulative sum on
   * @param thread_count
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> cumulative_sum(const Array<T> &ndarray, const i32 axis = -1,
                                 const usize thread_count = 16);

  /**
   * @brief Cumulative sum of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> cumulative_sum(const ArraySlice<T> &ndarray,
                                 const i32 axis = -1,
                                 const usize thread_count = 16);

  /**
   * @brief Cumulative sum of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @param axis Axis to perform cumulative sum on
   * @param thread_count
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> diff(const Array<T> &ndarray, const i32 axis = -1,
                       const usize thread_count = 16);

  /**
   * @brief Cumulative sum of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> diff(const ArraySlice<T> &ndarray, const i32 axis = -1,
                       const usize thread_count = 16);

  /**
   * @brief Cumulative product of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @param axis Axis to perform cumulative sum on
   * @param thread_count
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> cumulative_product(const Array<T> &ndarray,
                                     const i32 axis = -1,
                                     const usize thread_count = 16);

  /**
   * @brief Cumulative product of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> cumulative_product(const ArraySlice<T> &ndarray,
                                     const i32 axis = -1,
                                     const usize thread_count = 16);

  /**
   * @brief Cumulative product of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @param axis Axis to perform cumulative sum on
   * @param thread_count
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> nan_cumulative_sum(const Array<T> &ndarray,
                                     const i32 axis = -1,
                                     const usize thread_count = 16);

  /**
   * @brief Cumulative product of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> nan_cumulative_sum(const ArraySlice<T> &ndarray,
                                     const i32 axis = -1,
                                     const usize thread_count = 16);

  /**
   * @brief Cumulative product of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @param axis Axis to perform cumulative sum on
   * @param thread_count
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> nan_cumulative_product(const Array<T> &ndarray,
                                         const i32 axis = -1,
                                         const usize thread_count = 16);

  /**
   * @brief Cumulative product of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T>
  static Array<T> nan_cumulative_product(const ArraySlice<T> &ndarray,
                                         const i32 axis = -1,
                                         const usize thread_count = 16);

  /**
   * @brief Convert radian to degrees of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values
   * in
   * ndarray
   */
  template <typename T> static Array<T> rad_2_deg(const Array<T> &ndarray);

  /**
   * @brief Convert radian to degrees of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T> static Array<T> rad_2_deg(const ArraySlice<T> &ndarray);

  /**
   * @brief Convert radian to degree of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T> static Array<T> deg_2_rad(const Array<T> &ndarray);

  /**
   * @brief Convert radian to degree of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T> static Array<T> deg_2_rad(const ArraySlice<T> &ndarray);

  /**
   * @brief Calculate floor of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T> static Array<T> floor(const Array<T> &ndarray);

  /**
   * @brief Calculate floor of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T> static Array<T> floor(const ArraySlice<T> &ndarray);

  /**
   * @brief Calculate ceil of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T> static Array<T> ceil(const Array<T> &ndarray);

  /**
   * @brief Calculate ceil of an array.
   * @tparam T matrix type,
   * @param ndarray input array
   * @returns new array where each element is sum of all previous values in
   * ndarray
   */
  template <typename T> static Array<T> ceil(const ArraySlice<T> &ndarray);

  /**
   * @brief Check if every value satisfies the condition
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @param _func function that test for every element
   * @returns boolean value
   */
  template <typename T>
  static bool every(const Array<T> &ndarray, const fn<bool(const T &)> &,
                    const usize threads = 16);

  /**
   * @brief Check if every value satisfies the condition
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @param _func function that test for every element
   * @returns boolean value
   */
  template <typename T>
  static bool every(const ArraySlice<T> &slice, const fn<bool(const T &)> &,
                    const usize threads = 16);

  /**
   * @brief Check if at least one value satisfies the condition
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @param _func function that test for every element
   * @returns boolean value
   */
  template <typename T>
  static bool some(const Array<T> &ndarray, const fn<bool(const T &)> &,
                   const usize threads = 16);

  /**
   * @brief Check if at least one value satisfies the condition
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @param _func function that test for every element
   * @returns boolean value
   */
  template <typename T>
  static bool some(const ArraySlice<T> &slice, const fn<bool(const T &)> &,
                   const usize threads = 16);

  /**
   * @brief Compute 2 raised to value from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array

   * @returns boolean value
   */
  template <typename T> static Array<f64> exp2(const Array<T> &ndarray);

  /**
   * @brief Compute 2 raised to value from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array

   * @returns boolean value
   */
  template <typename T> static Array<f64> exp2(const ArraySlice<T> &ndarray);

  /**
   * @brief Compute reciprocal from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @returns boolean value
   */
  template <typename T> static Array<f64> reciprocal(const Array<T> &slice);

  /**
   * @brief Compute reciprocal from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @returns boolean value
   */
  template <typename T>
  static Array<f64> reciprocal(const ArraySlice<T> &slice);

  /**
   * @brief Compute sinh from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @returns boolean value
   */
  template <typename T> static Array<f64> sinh(const Array<T> &ndarray);

  /**
   * @brief Compute sinh from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @returns boolean value
   */
  template <typename T> static Array<f64> sinh(const ArraySlice<T> &ndarray);

  /**
   * @brief Compute cosh from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @returns boolean value
   */
  template <typename T> static Array<f64> cosh(const Array<T> &slice);

  /**
   * @brief Compute cosh from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @returns boolean value
   */
  template <typename T> static Array<f64> cosh(const ArraySlice<T> &slice);

  /**
   * @brief Compute tanh from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @returns boolean value
   */
  template <typename T> static Array<f64> tanh(const Array<T> &ndarray);

  /**
   * @brief Compute tanh from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array

   * @returns boolean value
   */
  template <typename T> static Array<f64> tanh(const ArraySlice<T> &ndarray);

  /**
   * @brief Compare if two arrays are equal
   * @tparam T array type
   * @param first n-dimensional array
   * @param other n-dimensional array
   * @returns boolean value
   */
  template <typename T1, typename T2>
  static bool compare(const Array<T1> &first, const Array<T2> &other);

  /**
   * @brief Compute tanh from array of values
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @param func function that test for every element
   * @returns boolean value
   */
  template <typename T>
  static void sort(
      Array<T> &,
      const fn<bool(const T first, const T second)> &comp =
          [](const T first, const T sec) { return first < sec; },
      const i32 axis = -1);

  /**
   * @brief Sort array in place along the axis - To Do
   * @todo implementation
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @param comp function that test for every element
   * @returns boolean value
   */
  template <typename T>
  static void sort(
      ArraySlice<T> &,
      const fn<bool(const T first, const T second)> &comp =
          [](const T first, const T sec) { return first < sec; },
      const i32 axis = -1);

  /**
   * @brief Sort array in place along the axis - To Do
   * @todo implementation
   * @tparam T array type
   * @param ndarray n-dimensional array
   * @param comp function that test for every element
   * @returns boolean value
   */
  template <typename T>
  static void sort(Axis<T> &, Array<T> &,
                   const fn<bool(const T first, const T second)> &comp);
};

typedef MdArrayUtility Utils;

#endif
