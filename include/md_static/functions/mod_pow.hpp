#pragma once
#ifndef _MOD_POW_HPP_
#define _MOD_POW_HPP_
#include "./map.hpp"
#include "./md_static_array_utility.hpp"

template <typename T, class T1>
Array<T> Utils::mod_pow(const Array<T> &values, const usize power,
                        const usize mod) {
  switch (power) {
  case 0:
    return Array<T>(1);
  case 1:
    return Array<T>(values);
  default: {
    return Utils::map<T>(values, [power, mod](const T &value) {
      u64 result = 1ULL, val = value, pow = power;

      while (pow > 0) {
        if (pow & 1) {
          result *= value;
          result %= mod;
        }

        val *= value;
        val %= mod;
        pow >>= 1;
      }
      return result;
    });
  }
  }
}

template <typename T, typename T1>
Array<T> Utils::mod_pow(const u64 n, const Array<T> &values, const usize mod) {
  return Utils::map<T>(values, [n, mod](const T &value) {
    switch (value) {
    case 0:
      return static_cast<T>(1);
    case 1:
      return static_cast<T>(n);
    default: {
      u64 result = 1, pow = value, val = n;
      while (pow > 0) {
        if (pow & 1) {
          result *= value;
          result %= mod;
        }
        val *= value;
        val %= mod;
        pow >>= 1;
      }
      return result;
    }
    }
  });
}

template <typename T, typename T1>
Array<T> Utils::mod_pow(const u64 n, const ArraySlice<T> &values,
                        const usize mod) {
  return mod_pow<T, T1>(
      n, Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      mod);
}

template <typename T, typename T1>
Array<T> Utils::mod_pow(const ArraySlice<T> &values, const usize power,
                        const usize mod) {
  return mod_pow<T, T1>(
      Array<T>(*values.array_reference_, values.offset, values.shp_offset),
      power, mod);
}

#endif
