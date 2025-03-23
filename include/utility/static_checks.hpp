#pragma once

#include "math.hpp"
#include <type_traits>
#ifndef __STATIC_CHECK_HPP__
#define __STATIC_CHECK_HPP__

class false_type;

/**
 * @brief Template to check if type is from a variadic list
 * @tparam T1 value to check
 * @tparam ... variadic list
 */
template <typename T1, typename...> struct is_any_one : std::false_type {};

/**
 * @brief Template to check if type is from a variadic list
 * @tparam T1 value to check
 * @tparam T2 value with which
 * @tparam ... variadic list
 */
template <typename T1, typename T2, typename... args>
struct is_any_one<T1, T2, args...> {
  static constexpr bool value =
      std::is_same<T1, T2>::value || is_any_one<T1, args...>::value;
};

/**
 * @brief Template to check if type is one of the native types
 * @tparam T value to check
 */
template <typename T> struct is_native {
  static constexpr bool value = is_any_one<T, i32, i64, i16, i8, u8, u16, u32,
                                           u64, f32, f64, f128, usize>::value;
};

#endif
