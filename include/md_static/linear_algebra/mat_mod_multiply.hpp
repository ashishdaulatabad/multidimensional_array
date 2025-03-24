#pragma once
#ifndef _MAT_MOD_MULTIPLY_HPP_
#define _MAT_MOD_MULTIPLY_HPP_

#include "./md_linear_algebra.hpp"

template <typename T3, typename T1, typename T2, class T>
Array<T3> Linalg::mat_mod_multiply(const Array<T1> &first,
                                   const Array<T2> &other, const usize mod,
                                   const usize threads) {
  if (first.get_shape_size() != 2 || other.get_shape_size() != 2) {
    throw std::runtime_error("Matrix dimension do not match.");
  }

  if (first.get_shape()[1] != other.get_shape()[0]) {
    throw std::runtime_error(
        "Total column of first matrix are not same as total rows in "
        "second "
        "matrix.");
  }
  const usize fshape0 = first.get_shape()[0], fshape1 = first.get_shape()[1],
              oshape0 = other.get_shape()[0], oshape1 = other.get_shape()[1];

  usize k_bound = 0, i_bound = 0, j_bound = 0;

  Array<T3> result({first.get_shape()[0], other.get_shape()[1]}, 0);
  Array<T2> other_t({oshape1, oshape0}, 0);

  for (usize i = 0; i < oshape1; ++i) {
    for (usize j = 0; j < oshape0; ++j) {
      other_t.array_[i * oshape0 + j] = other.array_[j * oshape1 + i];
    }
  }

  /// This loop is kept outside due to performance reasons.
  /// Split i or j into blocks
  constexpr usize block_size = 32;

  auto mod_mul_internal_ = [&first, &other_t, block_size, &result, mod, fshape0,
                            fshape1, oshape0,
                            oshape1](const usize start, const usize end) {
    usize i_bound = 0, j_bound = 0;

    for (usize i_block = start; i_block < end; i_block += block_size) {
      i_bound = std::min(i_block + block_size, fshape0);

      for (usize j_block = 0; j_block < oshape1; j_block += block_size) {
        j_bound = std::min(j_block + block_size, oshape1);

        for (usize i = i_block; i < i_bound; i += 4) {
          for (usize j = j_block; j < j_bound; j += 4) {
            T3 answer00 = 0;
            T3 answer01 = 0;
            T3 answer02 = 0;
            T3 answer03 = 0;

            T3 answer10 = 0;
            T3 answer11 = 0;
            T3 answer12 = 0;
            T3 answer13 = 0;

            T3 answer20 = 0;
            T3 answer21 = 0;
            T3 answer22 = 0;
            T3 answer23 = 0;

            T3 answer30 = 0;
            T3 answer31 = 0;
            T3 answer32 = 0;
            T3 answer33 = 0;

            for (usize k = 0; k < fshape1; ++k) {
              T1 c = first.array_[i * fshape1 + k];
              answer00 += c * other_t.array_[j * oshape1 + k];
              answer00 %= mod;
              answer01 += c * other_t.array_[(j + 1) * oshape1 + k];
              answer01 %= mod;
              answer02 += c * other_t.array_[(j + 2) * oshape1 + k];
              answer02 %= mod;
              answer03 += c * other_t.array_[(j + 3) * oshape1 + k];
              answer03 %= mod;

              c = first.array_[(i + 1) * fshape1 + k];
              answer10 += c * other_t.array_[j * oshape1 + k];
              answer10 %= mod;
              answer11 += c * other_t.array_[(j + 1) * oshape1 + k];
              answer11 %= mod;
              answer12 += c * other_t.array_[(j + 2) * oshape1 + k];
              answer12 %= mod;
              answer13 += c * other_t.array_[(j + 3) * oshape1 + k];
              answer13 %= mod;

              c = first.array_[(i + 2) * fshape1 + k];
              answer20 += c * other_t.array_[j * oshape1 + k];
              answer20 %= mod;
              answer21 += c * other_t.array_[(j + 1) * oshape1 + k];
              answer21 %= mod;
              answer22 += c * other_t.array_[(j + 2) * oshape1 + k];
              answer22 %= mod;
              answer23 += c * other_t.array_[(j + 3) * oshape1 + k];
              answer23 %= mod;

              c = first.array_[(i + 3) * fshape1 + k];
              answer30 += c * other_t.array_[j * oshape1 + k];
              answer30 %= mod;
              answer31 += c * other_t.array_[(j + 1) * oshape1 + k];
              answer31 %= mod;
              answer32 += c * other_t.array_[(j + 2) * oshape1 + k];
              answer32 %= mod;
              answer33 += c * other_t.array_[(j + 3) * oshape1 + k];
              answer33 %= mod;
            }
            result.array_[i * oshape0 + j] = answer00;
            result.array_[i * oshape0 + j + 1] = answer01;
            result.array_[i * oshape0 + j + 2] = answer02;
            result.array_[i * oshape0 + j + 3] = answer03;

            result.array_[(i + 1) * oshape0 + j] = answer10;
            result.array_[(i + 1) * oshape0 + j + 1] = answer11;
            result.array_[(i + 1) * oshape0 + j + 2] = answer12;
            result.array_[(i + 1) * oshape0 + j + 3] = answer13;

            result.array_[(i + 2) * oshape0 + j] = answer20;
            result.array_[(i + 2) * oshape0 + j + 1] = answer21;
            result.array_[(i + 2) * oshape0 + j + 2] = answer22;
            result.array_[(i + 2) * oshape0 + j + 3] = answer23;

            result.array_[(i + 3) * oshape0 + j] = answer30;
            result.array_[(i + 3) * oshape0 + j + 1] = answer31;
            result.array_[(i + 3) * oshape0 + j + 2] = answer32;
            result.array_[(i + 3) * oshape0 + j + 3] = answer33;
          }
        }
      }
    }
  };

  if (first.get_size() > s_threshold_size || threads > 1) {
    usize blocks = first.shape[0] / threads;
    std::vector<std::thread> thread_pool;
    usize index = 0, start = 0, end = blocks;

    for (; index < threads - 1; ++index, start += blocks, end += blocks) {
      thread_pool.emplace_back(mod_mul_internal_, start, end);
    }

    thread_pool.emplace_back(mod_mul_internal_, start, other.shape[1]);

    for (auto &thread : thread_pool) {
      thread.join();
    }
  } else {
    mod_mul_internal_(0, first.get_shape()[0]);
  }

  return result;
}

template <typename T3, typename T1, typename T2, class T>
Array<T3> Linalg::mat_mod_multiply(const ArraySlice<T1> &first,
                                   const Array<T2> &other, const usize mod,
                                   const usize threads) {
  return Linalg::mat_multiply<T3, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset), other,
      threads);
}

template <typename T3, typename T1, typename T2, class T>
Array<T3> Linalg::mat_mod_multiply(const Array<T1> &first,
                                   const ArraySlice<T2> &other, const usize mod,
                                   const usize threads) {
  return Linalg::mat_multiply<T3, T1, T2>(
      first, Array<T2>(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

template <typename T3, typename T1, typename T2, class T>
Array<T3> Linalg::mat_mod_multiply(const ArraySlice<T1> &first,
                                   const ArraySlice<T2> &other, const usize mod,
                                   const usize threads) {
  return Linalg::mat_multiply<T3, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

#endif
