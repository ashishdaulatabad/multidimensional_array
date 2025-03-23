#pragma once
#ifndef _MD_MATH_HPP_
#define _MD_MATH_HPP_

#include <stdint.h>

#include <cmath>

using i8 = char;
using u8 = unsigned char;
using i16 = short int;
using u16 = short unsigned;
using i32 = int;
using u32 = unsigned;
using i64 = long long;
using u64 = unsigned long long;
using f32 = float;
using f64 = double;
using f128 = long double;
using usize = size_t;

namespace Usize {
constexpr usize min = 0;
constexpr usize max = ~0;
} // namespace Usize

namespace I8 {
constexpr i8 min = -128;
constexpr i8 max = 127;
} // namespace I8

namespace I16 {
constexpr i16 min = -32768;
constexpr i16 max = 32767;
} // namespace I16

namespace I32 {
constexpr i32 min = -2147483648;
constexpr i32 max = 2147483647;
} // namespace I32

namespace I64 {
constexpr i64 min = -9223372036854775807L - 1;
constexpr i64 max = 9223372036854775807L;
} // namespace I64

namespace U8 {
constexpr u8 min = 0U;
constexpr u8 max = 255U;
} // namespace U8

namespace U16 {
constexpr u16 min = 0U;
constexpr u16 max = 65535U;
} // namespace U16

namespace U32 {
constexpr u32 min = 0U;
constexpr u32 max = 4294967295U;
} // namespace U32

namespace U64 {
constexpr u64 min = 0U;
constexpr u64 max = 18446744073709551615ULL;
} // namespace U64

namespace F32 {
constexpr bool isnan(f32 v) { return isnanf(v); };
constexpr f32 nan = std::numeric_limits<f32>::quiet_NaN();
constexpr f32 inf = std::numeric_limits<f32>::infinity();
constexpr f32 ninf = inf * -1;
} // namespace F32

namespace F64 {
constexpr bool isnan(f64 v) { return std::isnan(v); };
constexpr f64 nan = std::numeric_limits<f64>::quiet_NaN();
constexpr f64 inf = std::numeric_limits<f64>::infinity();
constexpr f64 ninf = inf * -1;
} // namespace F64

namespace F128 {
constexpr bool isnan(f128 v) { return std::isnan(v); };
constexpr f128 nan = std::numeric_limits<f128>::quiet_NaN();
constexpr f128 inf = std::numeric_limits<f128>::infinity();
constexpr f128 ninf = inf * -1;
} // namespace F128

namespace Math {

constexpr f64 e = 2.71828182845904523536028747135266249775724709369995;
constexpr f64 pi = M_PI;
constexpr f64 pi_by_2 = M_PI_2;
constexpr f64 pi_2 = M_PI * 2;

/**
 * @brief Reverse at max 64-bits of a 64 bit number
 * @param n unsigned number
 * @param bit_size size to be reversed
 * @return reversed bit number
 */
constexpr inline usize reverse_bits(const usize n, const usize bit_size = 64) {
  usize rn = (n << 32) | (n >> 32);
  rn = ((rn << 16) & 0xFFFF0000FFFF0000) | ((rn >> 16) & 0x0000FFFF0000FFFF);
  rn = ((rn << 8) & 0xFF00FF00FF00FF00) | ((rn >> 8) & 0x00FF00FF00FF00FF);
  rn = ((rn << 4) & 0xF0F0F0F0F0F0F0F0) | ((rn >> 4) & 0x0F0F0F0F0F0F0F0F);
  rn = ((rn << 2) & 0xCCCCCCCCCCCCCCCC) | ((rn >> 2) & 0x3333333333333333);
  rn = ((rn << 1) & 0xAAAAAAAAAAAAAAAA) | ((rn >> 1) & 0x5555555555555555);
  return (rn >> (64 - bit_size));
}

/**
 * @brief Reverse at max 32-bits of a 32 bit number
 * @param n unsigned number
 * @param bit_size size to be reversed
 * @return reversed bit number
 */
constexpr inline u32 reverse_bits_32(const u32 n, const u32 bit_size = 32) {
  u32 rn = (n << 16) | (n >> 16);
  rn = ((rn << 8) & 0xFF00FF00) | ((rn >> 8) & 0x00FF00FF);
  rn = ((rn << 4) & 0xF0F0F0F0) | ((rn >> 4) & 0x0F0F0F0F);
  rn = ((rn << 2) & 0xCCCCCCCC) | ((rn >> 2) & 0x33333333);
  rn = ((rn << 1) & 0xAAAAAAAA) | ((rn >> 1) & 0x55555555);
  return (rn >> (32 - bit_size));
}

/**
 * @brief Reverse at max 16-bits of a 16 bit number
 * @param n unsigned number
 * @param bit_size size to be reversed
 * @return reversed bit number
 */
constexpr inline usize reverse_bits_16(const u16 n, const u16 bit_size = 16) {
  u16 rn = (rn << 8) | (rn >> 8);
  rn = ((rn << 4) & 0xF0F0) | ((rn >> 4) & 0x0F0F);
  rn = ((rn << 2) & 0xCCCC) | ((rn >> 2) & 0x3333);
  rn = ((rn << 1) & 0xAAAA) | ((rn >> 1) & 0x5555);
  return (rn >> (16 - bit_size));
}

/**
 * @brief Reverse at max 8-bits of a 8 bit number
 * @param n unsigned number
 * @param bit_size size to be reversed
 * @return reversed bit number
 */
constexpr inline u8 reverse_bits_8(const u8 n, const u8 bit_size = 8) {
  u8 rn = (rn << 4) | (rn >> 4);
  rn = ((rn << 2) & 0xCC) | ((rn >> 2) & 0x33);
  rn = ((rn << 1) & 0xAA) | ((rn >> 1) & 0x55);
  return (rn >> (8 - bit_size));
}

template <usize...> struct reversed_bits {};

/**
 * @brief Evaluate the reversed bits at compilation time.
 */
template <usize N, usize bit_size> struct reversed_bits<N, bit_size> {
  static constexpr usize value = reverse_bits(N, bit_size);
};

}; // namespace Math

#endif
