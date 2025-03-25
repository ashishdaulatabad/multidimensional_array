#pragma once
#ifndef _VDOT_HPP_
#define _VDOT_HPP_

#include "../../md_complex/md_complex.hpp"
#include "../functions/map.hpp"
#include "./md_linear_algebra.hpp"

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::vdot(const Array<T1> &first, const Array<T2> &other,
                       const usize threads) {
  return Linalg::dot<T3>(first, other, threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::vdot(const ArraySlice<T1> &first, const Array<T2> &other,
                       const usize threads) {
  return Linalg::dot<T3>(first, other, threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::vdot(const ArraySlice<T1> &first, const ArraySlice<T2> &other,
                       const usize threads) {
  return Linalg::dot<T3>(first, other, threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::vdot(const Array<T1> &first, const ArraySlice<T2> &other,
                       const usize threads) {
  return Linalg::dot<T3>(first, other, threads);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::vdot(const Array<Complex<T1>> &first, const Array<T2> &other,
                       const usize threads) {
  // Basically, compute dot product of vector:::
  // Last axis of vector first should be multiplied with second last
  // axis of other
  if (other.get_shape_size() >= 2 && first.get_shape_size() >= 2) {
    if (other.get_shape_size() == 2 && first.get_shape_size() == 2) {
      return Linalg::mat_multiply<T3, Complex<T1>, T2>(
          Utils::map<Complex<T1>>(first,
                                  [](const Complex<T1> &__value) {
                                    return Complex<T1>::conj(__value);
                                  }),
          other, threads);
    }
    if (first.shape[first.shp_size - 1] != other.shape[other.shp_size - 2]) {
      throw std::runtime_error(
          "Axis size do not match for dot multiplication.");
    }
    const usize overall_size =
        first.get_shape_size() - 2 + other.get_shape_size();

    std::vector<usize> overall_shape(overall_size);
    usize index = 0;
    for (usize i = 0; i < first.get_shape_size() - 1; ++i) {
      overall_shape[index++] = first.shape[i];
    }

    for (usize i = 0; i < other.get_shape_size(); ++i) {
      if (i != other.get_shape_size() - 2) {
        overall_shape[index++] = other.shape[i];
      }
    }

    Array<T3> result(overall_shape, 0);
    const usize res_base_matrix_size =
        (result.shape[result.shp_size - 1] * result.shape[result.shp_size - 2]);

    const usize m = first.shape[first.shp_size - 2];
    const usize n = first.shape[first.shp_size - 1];
    const usize p = other.shape[other.shp_size - 1];

    const usize other_base_matrix_size = n * p;

    index = 0;
    usize skip_count = other.get_size() / n;

    auto __perform_dot_parallel = [&result, &first, &other, m, n, p, threads,
                                   other_base_matrix_size,
                                   skip_count](const usize thread_number) {
      // We iterate over each row of first ndarray. Each $i^{th}$ thread
      // will handle alternate $i^{th}$ row.
      usize index = thread_number * skip_count;
      for (usize first_row = thread_number * n; first_row < first.get_size();
           first_row += (threads * n)) {
        // Considering a 2D - layer of n-dimensional array $other$, we
        // iterate over every 2D-layer.
        for (usize other_block = 0; other_block < other.get_size();
             other_block += other_base_matrix_size) {
          const auto f = other.array_[other_block];
          // Iterator over second row of first and column of other
          // array (note that $j$ and $other_col$ loop are swapped for
          // performance reasons, to fulfil offset, we add $other_col$
          // to $index$ in result array)
          for (usize j = 0; j < n; ++j) {
            const auto c = Complex<T1>::conj(first.array_[first_row + j]);
            // Iterator over last axis of other.
            for (usize other_col = 0; other_col < p; ++other_col) {
              result.array_[index + other_col] +=
                  c * other.array_[other_block + j * p + other_col];
            }
          }
          // Index is skipped by p, because the row is processed for
          // every column
          index += p;
        }
        // Since we use threads, we let other threads fill remaining
        // (threads - 1) rows and skip to next ones
        index += ((threads - 1) * skip_count);
      }
    };
    std::vector<std::thread> thread_pool;

    for (usize i = 0; i < threads; ++i) {
      thread_pool.emplace_back(std::thread(__perform_dot_parallel, i));
    }

    for (auto &thread : thread_pool) {
      thread.join();
    }

    return result;
  } else {
    if (other.get_shape_size() == 1 && first.get_shape_size() == 1) {
      if (other.shape[0] != first.shape[0]) {
        throw std::runtime_error(
            "Axis size do not match for dot multiplication.");
      }

      // A single valued answer.
      return Linalg::inner<T3, Complex<T1>, T2>(
          MdArrayUtility::map<Complex<T1>>(first,
                                           [](const Complex<T1> &__value) {
                                             return Complex<T1>::conj(__value);
                                           }),
          other, threads);
    } else if (other.get_shape_size() == 1) {
      // Note: first does have $n$ dimensions
      if (other.shape[0] != first.shape[first.get_shape_size() - 1]) {
        throw std::runtime_error(
            "Axis size do not match for dot multiplication.");
      }

      std::vector<usize> overall_shape(first.get_shape_size() - 1);
      for (usize index = 0; index < first.get_shape_size() - 1; ++index) {
        overall_shape[index] = first.shape[index];
      }
      Array<T3> result(overall_shape, 0);

      auto __perform_dot_parallel = [&first, &other, &result](const usize start,
                                                              const usize end) {
        usize res_index = start;
        usize shp = first.get_shape_size() - 1;
        // For each last axis of first array
        for (usize index = start * first.shape[shp];
             index < end * first.shape[shp]; index += first.shape[shp]) {
          // Iterate over the last axis of array other
          for (usize row = 0; row < other.shape[0]; ++row) {
            result.array_[res_index] +=
                (Complex<T1>::conj(first.array_[index + row]) *
                 other.array_[row]);
          }
          ++res_index;
        }
      };

      const usize block = result.get_size() / threads - 1;
      std::vector<std::thread> thread_pool;
      for (i32 i = 0; i < threads - 1; ++i) {
        thread_pool.emplace_back(__perform_dot_parallel, block * i,
                                 block * (i + 1));
      }
      thread_pool.emplace_back(__perform_dot_parallel, block * (threads - 1),
                               result.get_size());

      for (auto &thread : thread_pool) {
        thread.join();
      }
      return result;
    } else {
      if (other.shape[other.get_shape_size() - 2] != first.shape[0]) {
        throw std::runtime_error(
            "Axis size do not match for dot multiplication.");
      }

      std::vector<usize> overall_shape(other.get_shape_size() - 1);
      usize shp_index = 0;
      for (usize index = 0; index < other.get_shape_size(); ++index) {
        if (index != other.get_shape_size() - 2) {
          overall_shape[shp_index++] = other.shape[index];
        }
      }
      Array<T3> result(overall_shape, 0);

      auto __perform_dot_parallel = [&first, &other, &result,
                                     &threads](const usize thread_number) {
        usize shp = other.get_shape_size() - 2;
        usize res_index = thread_number * other.shape[shp + 1];
        usize rows = first.get_size();
        usize other_mat_size = other.shape[shp] * other.shape[shp + 1];
        // Traversing through each layer of matrix of other
        for (usize other_mat_index = 0; other_mat_index < other.get_size();
             other_mat_index += (threads * other_mat_size)) {
          // For each layer, calculate dot-product
          // Each element will be
          for (usize element = 0; element < other_mat_size; ++element) {
            result.array_[res_index + (element % rows)] +=
                Complex<T1>::conj(
                    first.array_[element / other.shape[shp + 1]]) *
                other.array_[other_mat_index + element];
          }
          res_index += (threads * other.shape[shp + 1]);
        }
      };

      std::vector<std::thread> thread_pool;

      for (usize i = 0; i < threads; ++i) {
        thread_pool.emplace_back(__perform_dot_parallel, i);
      }

      for (auto &thread : thread_pool) {
        thread.join();
      }
      return result;
    }
  }
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::vdot(const ArraySlice<Complex<T1>> &first,
                       const Array<T2> &other, const usize threads) {
  return Linalg::vdot<T3>(
      Array(*first.array_reference_, first.offset, first.shp_offset), other,
      threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::vdot(const ArraySlice<Complex<T1>> &first,
                       const ArraySlice<T2> &other, const usize threads) {
  return Linalg::vdot<T3>(
      Array(*first.array_reference_, first.offset, first.shp_offset),
      Array(*other.array_reference_, other.offset, other.shp_offset), threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::vdot(const Array<Complex<T1>> &first,
                       const ArraySlice<T2> &other, const usize threads) {
  return Linalg::vdot<T3>(
      first, Array(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

#endif
