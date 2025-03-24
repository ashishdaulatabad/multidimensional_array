#pragma once
#ifndef _MULTI_DOT_HPP_
#define _MULTI_DOT_HPP_

#include "./dot.hpp"
#include "./md_linear_algebra.hpp"

template <typename Tfinal, typename T1, typename T2, typename... arg>
Array<Tfinal> Linalg::multi_dot(const Array<T1> &first, const Array<T2> &other,
                                const Array<arg> &...arguments) {
  Array<Tfinal> result = Linalg::dot<Tfinal, T1, T2>(first, other);
  return Linalg::multi_dot<Tfinal, Tfinal, arg...>(result, arguments...);
}

template <typename Tfinal, typename T1, typename T2, typename... arg>
Array<Tfinal> Linalg::multi_dot(const ArraySlice<T1> &first,
                                const ArraySlice<T2> &other,
                                const Array<arg> &...arguments) {
  Array<Tfinal> result = Linalg::dot<Tfinal, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
  return Linalg::multi_dot<Tfinal, Tfinal, arg...>(result, arguments...);
}

template <typename Tfinal, typename T1, typename T2, typename... arg>
Array<Tfinal> Linalg::multi_dot(const ArraySlice<T1> &first,
                                const Array<T2> &other,
                                const Array<arg> &...arguments) {
  Array<Tfinal> result = Linalg::dot<Tfinal, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      other);
  return Linalg::multi_dot<Tfinal, Tfinal, arg...>(result, arguments...);
}

template <typename Tfinal, typename T1, typename T2, typename... arg>
Array<Tfinal> Linalg::multi_dot(const Array<T1> &first,
                                const ArraySlice<T2> &other,
                                const Array<arg> &...arguments) {
  Array<Tfinal> result = Linalg::dot<Tfinal, T1, T2>(
      first,
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
  return Linalg::multi_dot<Tfinal, Tfinal, arg...>(result, arguments...);
}

////////////////////////////////////////////////////////////////////////////////

template <typename Tfinal, typename T1, typename T2, typename... arg>
Array<Tfinal> Linalg::multi_dot(const Array<T1> &first, const Array<T2> &other,
                                const ArraySlice<arg> &...arguments) {
  Array<Tfinal> result = Linalg::dot<Tfinal, T1, T2>(first, other);
  return Linalg::multi_dot<Tfinal, Tfinal, arg...>(result, arguments...);
}

template <typename Tfinal, typename T1, typename T2, typename... arg>
Array<Tfinal> Linalg::multi_dot(const ArraySlice<T1> &first,
                                const ArraySlice<T2> &other,
                                const ArraySlice<arg> &...arguments) {
  Array<Tfinal> result = Linalg::dot<Tfinal, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
  return Linalg::multi_dot<Tfinal, Tfinal, arg...>(result, arguments...);
}

template <typename Tfinal, typename T1, typename T2, typename... arg>
Array<Tfinal> Linalg::multi_dot(const ArraySlice<T1> &first,
                                const Array<T2> &other,
                                const ArraySlice<arg> &...arguments) {
  Array<Tfinal> result = Linalg::dot<Tfinal, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      other);
  return Linalg::multi_dot<Tfinal, Tfinal, arg...>(result, arguments...);
}

template <typename Tfinal, typename T1, typename T2, typename... arg>
Array<Tfinal> Linalg::multi_dot(const Array<T1> &first,
                                const ArraySlice<T2> &other,
                                const ArraySlice<arg> &...arguments) {
  Array<Tfinal> result = Linalg::dot<Tfinal, T1, T2>(
      first,
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
  return Linalg::multi_dot<Tfinal, Tfinal, arg...>(result, arguments...);
}

template <typename Tfinal, typename T1, typename T2>
Array<Tfinal> Linalg::multi_dot(const Array<T1> &first,
                                const Array<T2> &other) {
  return Linalg::dot<Tfinal, T1, T2>(first, other);
}

template <typename Tfinal, typename T1, typename T2>
Array<Tfinal> Linalg::multi_dot(const ArraySlice<T1> &first,
                                const ArraySlice<T2> &other) {
  return Linalg::dot<Tfinal, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
}

template <typename Tfinal, typename T1, typename T2>
Array<Tfinal> Linalg::multi_dot(const ArraySlice<T1> &first,
                                const Array<T2> &other) {
  return Linalg::dot<Tfinal, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      other);
}

template <typename Tfinal, typename T1, typename T2>
Array<Tfinal> Linalg::multi_dot(const Array<T1> &first,
                                const ArraySlice<T2> &other) {
  return Linalg::dot<Tfinal, T1, T2>(
      first,
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset));
}

#endif
