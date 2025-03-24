#pragma once
#ifndef _MATRIX_CHAIN_MULTIPLY_HPP_
#define _MATRIX_CHAIN_MULTIPLY_HPP_

#include "./mat_multiply.hpp"
#include "./md_linear_algebra.hpp"

template <typename Tf, typename T1, typename T2, typename... arg>
Array<Tf> Linalg::matrix_chain_multiply(const Array<T1> &first,
                                        const Array<T2> &other,
                                        const Array<arg> &...arguments) {
  Array<Tf> result = Linalg::mat_multiply<Tf, T1, T2>(first, other);
  return Linalg::matrix_chain_multiply<Tf, Tf, arg...>(result, arguments...);
}

template <typename Tf, typename T1, typename T2, typename... arg>
Array<Tf> Linalg::matrix_chain_multiply(const ArraySlice<T1> &first,
                                        const ArraySlice<T2> &other,
                                        const Array<arg> &...arguments) {
  Array<Tf> result = Linalg::mat_multiply<Tf, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
  return Linalg::matrix_chain_multiply<Tf, Tf, arg...>(result, arguments...);
}

template <typename Tf, typename T1, typename T2, typename... arg>
Array<Tf> Linalg::matrix_chain_multiply(const ArraySlice<T1> &first,
                                        const Array<T2> &other,
                                        const Array<arg> &...arguments) {
  Array<Tf> result = Linalg::mat_multiply<Tf, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      other);
  return Linalg::matrix_chain_multiply<Tf, Tf, arg...>(result, arguments...);
}

template <typename Tf, typename T1, typename T2, typename... arg>
Array<Tf> Linalg::matrix_chain_multiply(const Array<T1> &first,
                                        const ArraySlice<T2> &other,
                                        const Array<arg> &...arguments) {
  Array<Tf> result = Linalg::mat_multiply<Tf, T1, T2>(
      first,
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
  return Linalg::matrix_chain_multiply<Tf, Tf, arg...>(result, arguments...);
}

////////////////////////////////////////////////////////////////////////////////

template <typename Tf, typename T1, typename T2, typename... arg>
Array<Tf> Linalg::matrix_chain_multiply(const Array<T1> &first,
                                        const Array<T2> &other,
                                        const ArraySlice<arg> &...arguments) {
  Array<Tf> result = Linalg::mat_multiply<Tf, T1, T2>(first, other);
  return Linalg::matrix_chain_multiply<Tf, Tf, arg...>(result, arguments...);
}

template <typename Tf, typename T1, typename T2, typename... arg>
Array<Tf> Linalg::matrix_chain_multiply(const ArraySlice<T1> &first,
                                        const ArraySlice<T2> &other,
                                        const ArraySlice<arg> &...arguments) {
  Array<Tf> result = Linalg::mat_multiply<Tf, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
  return Linalg::matrix_chain_multiply<Tf, Tf, arg...>(result, arguments...);
}

template <typename Tf, typename T1, typename T2, typename... arg>
Array<Tf> Linalg::matrix_chain_multiply(const ArraySlice<T1> &first,
                                        const Array<T2> &other,
                                        const ArraySlice<arg> &...arguments) {
  Array<Tf> result = Linalg::mat_multiply<Tf, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      other);
  return Linalg::matrix_chain_multiply<Tf, Tf, arg...>(result, arguments...);
}

template <typename Tf, typename T1, typename T2, typename... arg>
Array<Tf> Linalg::matrix_chain_multiply(const Array<T1> &first,
                                        const ArraySlice<T2> &other,
                                        const ArraySlice<arg> &...arguments) {
  Array<Tf> result = Linalg::mat_multiply<Tf, T1, T2>(
      first,
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
  return Linalg::matrix_chain_multiply<Tf, Tf, arg...>(result, arguments...);
}

template <typename Tf, typename T1, typename T2>
Array<Tf> Linalg::matrix_chain_multiply(const Array<T1> &first,
                                        const Array<T2> &other) {
  return Linalg::mat_multiply<Tf, T1, T2>(first, other);
}

template <typename Tf, typename T1, typename T2>
Array<Tf> Linalg::matrix_chain_multiply(const ArraySlice<T1> &first,
                                        const ArraySlice<T2> &other) {
  return Linalg::mat_multiply<Tf, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
}

template <typename Tf, typename T1, typename T2>
Array<Tf> Linalg::matrix_chain_multiply(const ArraySlice<T1> &first,
                                        const Array<T2> &other) {
  return Linalg::mat_multiply<Tf, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      other);
}

template <typename Tf, typename T1, typename T2>
Array<Tf> Linalg::matrix_chain_multiply(const Array<T1> &first,
                                        const ArraySlice<T2> &other) {
  return Linalg::mat_multiply<Tf, T1, T2>(
      first,
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
}

#endif
