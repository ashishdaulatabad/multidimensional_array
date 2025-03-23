#pragma once
#ifndef _MD_COMPLEX_HPP_
#define _MD_COMPLEX_HPP_

#include <cmath>
#include <ostream>

#include "../utility/math.hpp"
#include "../utility/static_checks.hpp"

template <typename T,
          class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
struct Complex {
  T real{};
  T img{};

  constexpr Complex() = default;

  constexpr Complex(const T other) : real(other) {}

  constexpr Complex(const T r, const T i) : real(r), img(i) {}

  template <typename T1>
  constexpr Complex(const Complex<T1> &other)
      : real(other.real), img(other.img) {}

  template <typename T1>
  inline constexpr Complex<T> &operator=(const Complex<T1> &other) {
    real = other.real;
    img = other.img;
    return *this;
  }

  inline constexpr operator T() const { return real; }

  template <typename T1> inline constexpr operator Complex<T1>() const {
    return Complex<T1>(real, img);
  }

  template <typename T1>
  inline constexpr Complex<T> &operator=(const T1 &other) {
    real = other;
    img = 0;
    return *this;
  }

  template <typename T1>
  inline constexpr auto operator+(const Complex<T1> &other) const {
    return Complex(real + other.real, img + other.img);
  }

  template <typename T1, class>
  inline constexpr auto operator+(const T1 &other) const {
    return Complex(real + other, img);
  }

  template <typename T1>
  inline constexpr Complex &operator+=(const Complex<T1> &other) {
    real += other.real;
    img += other.img;
    return *this;
  }

  template <typename T1> inline constexpr Complex &operator+=(const T1 &other) {
    real += other;
    return *this;
  }

  template <typename T1>
  inline constexpr auto operator-(const Complex<T1> &other) const {
    return Complex(real - other.real, img - other.img);
  }

  template <typename T1> inline constexpr auto operator-() const {
    return Complex(-real, -img);
  }

  template <typename T1>
  inline constexpr auto operator-(const T1 &other) const {
    return Complex(real - other, img);
  }

  template <typename T1>
  inline constexpr Complex &operator-=(const Complex<T1> &other) {
    real -= other.real;
    img -= other.img;
    return *this;
  }

  template <typename T1> inline constexpr Complex &operator-=(const T1 &other) {
    real -= other;
    return *this;
  }

  template <typename T1>
  inline constexpr auto operator*(const Complex<T1> &other) const {
    return Complex(real * other.real - img * other.img,
                   real * other.img + img * other.real);
  }

  template <typename T1>
  inline constexpr auto operator*(const T1 &other) const {
    return Complex<T>(real * other, img * other);
  }

  template <typename T1>
  inline constexpr Complex &operator*=(const Complex<T1> &other) {
    const auto temp = real * other.real - img * other.img;
    img = img * other.real + real * other.img;
    real = temp;
    return *this;
  }

  template <typename T1> inline constexpr Complex &operator*=(const T1 &other) {
    real *= other;
    img *= other;
    return *this;
  }

  template <typename T1> inline auto operator/(const Complex<T1> &other) const {
    const auto sq_abs = (other.real * other.real + other.img * other.img);
    return Complex<T>((real * other.real + img * other.img) / sq_abs,
                      (real * other.img - img * other.real) / sq_abs);
  }

  template <typename T1> inline auto operator/(const T1 &other) const {
    return Complex<T>(real / other, img / other);
  }

  template <typename T1> inline Complex &operator/=(const Complex<T1> &other) {
    const auto sq_abs = (other.real * other.real + other.img * other.img);
    const auto temp = real * other.real + img * other.img;
    img *= other.real;
    img += real * other.img;
    img /= sq_abs;
    real = temp / sq_abs;
    return *this;
  }

  inline constexpr std::pair<f64, f64> to_polar() const {
    return {abs(), ::atan(img / real)};
  }

  static inline constexpr Complex from_polar(const f64 rad, const f64 ang) {
    return {rad * ::cos(ang), rad * ::sin(ang)};
  }

  template <typename T1>
  static inline constexpr Complex<T1> conj(const Complex<T1> &other) {
    return Complex<T1>(other.real, -other.img);
  }

  inline constexpr f64 sq_abs() const { return real * real + img * img; }

  inline constexpr f64 abs() const { return ::sqrt(real * real + img * img); }

  template <typename T1> inline Complex &operator/=(const T1 &other) {
    real /= other;
    img /= other;
    return *this;
  }

  template <typename T1,
            typename = typename std::enable_if<is_native<T1>::value>::type>
  inline constexpr bool operator==(const Complex<T1> other) {
    return other.real == real && other.img == img;
  }

  template <typename T1,
            typename = typename std::enable_if<is_native<T1>::value>::type>
  inline constexpr bool operator==(const T1 other) {
    return other == real && img == 0;
  }

  template <typename T1,
            typename = typename std::enable_if<is_native<T1>::value>::type>
  inline constexpr bool operator!=(const Complex<T1> &other) {
    return other.real != real || other.img != img;
  }

  template <typename T1,
            typename = typename std::enable_if<is_native<T1>::value>::type>
  inline constexpr bool operator!=(const T1 &other) {
    return other != real || img != 0;
  }

  friend std::ostream &operator<<(std::ostream &op, const Complex &c) {
    if (c.img < 0) {
      op << c.real << '-' << (-c.img) << 'j';
    } else {
      op << c.real << '+' << c.img << 'j';
    }
    return op;
  }
};

template <typename T> std::string to_string(const Complex<T> &c) {
  std::string ans = "";
  if (c.img < 0) {
    ans = std::to_string(c.real) + "-" + std::to_string(-c.img) + "j";
  } else {
    ans = std::to_string(c.real) + "+" + std::to_string(c.img) + "j";
  }
  return ans;
}

namespace std {

template <typename T>
inline constexpr Complex<T> sqrt(const Complex<T> &other) {
  const auto polar = other.to_polar();
  return Complex<T>::from_polar(std::sqrt(polar.first), polar.second / 2);
}

template <typename T>
inline constexpr Complex<T> pow(const Complex<T> &other, f64 value) {
  const auto [__R, __A] = other.to_polar();
  return Complex<T>::from_polar(std::pow(__R, value), __A * value);
}

template <typename T> inline constexpr Complex<T> abs(const Complex<T> &other) {
  return other.abs();
}
} // namespace std

template <typename T, typename T1,
          class = typename std::enable_if<is_native<T1>::value>::type>
inline constexpr Complex<T> operator+(const T1         &other,
                                      const Complex<T> &first) {
  return Complex<T>(other + first.real, first.img);
}

template <typename T, typename T1,
          class = typename std::enable_if<is_native<T1>::value>::type>
inline constexpr Complex<T> operator-(const T1         &other,
                                      const Complex<T> &first) {
  return Complex<T>(other - first.real, -first.img);
}

template <typename T, typename T1,
          class = typename std::enable_if<is_native<T1>::value>::type>
inline constexpr Complex<T> operator*(const T1         &other,
                                      const Complex<T> &first) {
  return Complex<T>(first.real * other, first.img * other);
}

template <typename T, typename T1,
          class = typename std::enable_if<is_native<T1>::value>::type>
inline constexpr Complex<T> operator/(const T1         &other,
                                      const Complex<T> &first) {
  const auto sq_abs = first.real * first.real + first.img * first.img;
  return Complex<T>((other * first.real) / sq_abs,
                    -((other * first.img) / sq_abs));
}

inline constexpr Complex<f128> operator"" _i(f128 img) {
  Complex<f128> c;
  c.real = 0;
  c.img = img;
  return c;
}

inline constexpr Complex<u64> operator"" _i(u64 img) {
  Complex<u64> c;
  c.real = 0;
  c.img = img;
  return c;
}

using cdouble = Complex<f64>;
using clongdouble = Complex<f128>;
using cfloat = Complex<f32>;

using cuint64 = Complex<u64>;
using cuint32 = Complex<u32>;
using cuint16 = Complex<u16>;
using cuint8 = Complex<u8>;

using cint64 = Complex<i64>;
using cint32 = Complex<i32>;
using cint16 = Complex<i16>;
using cint8 = Complex<i8>;

template <> struct std::is_fundamental<cdouble> : std::true_type {};
template <> struct std::is_fundamental<clongdouble> : std::true_type {};

typedef cdouble     c64;
typedef clongdouble c128;
typedef cfloat      c32;
typedef cint8       ci8;
typedef cint16      ci16;
typedef cint32      ci32;
typedef cint64      ci64;
typedef cuint8      cu8;
typedef cuint16     cu16;
typedef cuint32     cu32;
typedef cuint64     cu64;

#endif
