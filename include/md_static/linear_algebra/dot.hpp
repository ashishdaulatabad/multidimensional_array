#pragma once
#ifndef _DOT_HPP_
#define _DOT_HPP_

#include "./mat_multiply.hpp"
#include "./md_linear_algebra.hpp"

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::dot(const Array<T1> &first, const Array<T2> &other,
                      const usize thread_count) {
  // Basically, compute dot product of vector:::
  // Last axis of vector first should be multiplied with second last
  // axis of other
  const usize first_shape = first.get_shape_size();
  const usize other_shape = other.get_shape_size();

  if (other_shape >= 2 && first_shape >= 2) {
    if (other_shape == 2 && first_shape == 2) {
      return Linalg::mat_multiply<T1, T2, T3>(first, other, thread_count);
    }

    if (first.shape[first_shape - 1] != other.shape[other_shape - 2]) {
      throw std::runtime_error(
          "Axis size do not match for dot multiplication.");
    }

    const usize overall_size = first_shape + other_shape - 2;
    std::vector<usize> overall_shape(overall_size);
    usize index = 0, i = 0;

    for (; i < first_shape - 1; ++i, ++index) {
      overall_shape[index] = first.shape[i];
    }

    for (i = 0; i < other_shape; ++i) {
      if (i != other_shape - 2) {
        overall_shape[index++] = other.shape[i];
      }
    }

    Array<T3> result(overall_shape, 0);
    const usize result_shape = result.get_shape_size();

    const usize m = first.shape[first_shape - 2];
    const usize n = first.shape[first_shape - 1];
    const usize p = other.shape[other_shape - 1];

    const usize res_base_matrix_size =
        (result.shape[result_shape - 1] * result.shape[result_shape - 2]);

    const usize other_base_matrix_size = n * p;

    u16 threads = thread_count;
    const usize skip_count = other.get_size() / n;

    auto evaulate_dot_ = [&result, &first, &other, m, n, p, threads,
                          other_base_matrix_size,
                          skip_count](const usize thread_number) {
      // We iterate over each row of first ndarray. Each $i^{th}$ thread
      // will handle alternate $i^{th}$ row.
      usize index = thread_number * skip_count;
      usize first_row = thread_number * n;

      for (; first_row < first.get_size(); first_row += (threads * n)) {
        // Considering a 2D - layer of n-dimensional array $other$, we
        // iterate over every 2D-layer.
        usize other_block = 0;
        const usize other_size = other.get_size();

        for (; other_block < other_size;
             other_block += other_base_matrix_size) {
          const auto f = other.array_[other_block];
          // Iterator over second row of first and column of other
          // array (note that $j$ and $other_col$ loop are swapped for
          // performance reasons, to fulfil offset, we add $other_col$
          // to $index$ in result array)
          for (usize j = 0; j < n; ++j) {
            const auto c = first.array_[first_row + j];
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
      thread_pool.emplace_back(std::thread(evaulate_dot_, i));
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
      return Linalg::inner<T3, T1, T2>(first, other, thread_count);
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

      auto evaluate_dot_ = [&first, &other, &result](const usize start,
                                                     const usize end) {
        usize res_index = start;
        usize shp = first.get_shape_size() - 1;
        // For each last axis of first array
        for (usize index = start * first.shape[shp];
             index < end * first.shape[shp]; index += first.shape[shp]) {
          // Iterate over the last axis of array other
          for (usize row = 0; row < other.shape[0]; ++row) {
            result.array_[res_index] +=
                (first.array_[index + row] * other.array_[row]);
          }
          ++res_index;
        }
      };

      std::vector<std::thread> thread_pool;

      const usize block = result.get_size() / thread_count;
      usize index = 0, start = 0, end = block;

      for (; index < thread_count - 1; ++index, start += block, end += block) {
        thread_pool.emplace_back(evaluate_dot_, start, end);
      }

      thread_pool.emplace_back(evaluate_dot_, start, result.get_size());

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

      auto evaluate_dot_ = [&first, &other, &result,
                            &thread_count](const usize thread_number) {
        usize shp = other.get_shape_size() - 2;
        usize res_index = thread_number * other.shape[shp + 1];
        usize rows = first.get_size();
        usize other_mat_size = other.shape[shp] * other.shape[shp + 1];

        // Traversing through each layer of matrix of other
        for (usize other_mat_index = 0; other_mat_index < other.get_size();
             other_mat_index += (thread_count * other_mat_size)) {
          // For each layer, calculate dot-product
          // Each element will be
          for (usize element = 0; element < other_mat_size; ++element) {
            result.array_[res_index + (element % rows)] +=
                first.array_[element / other.shape[shp + 1]] *
                other.array_[other_mat_index + element];
          }
          res_index += (thread_count * other.shape[shp + 1]);
        }
      };

      std::vector<std::thread> thread_pool;

      for (usize i = 0; i < thread_count; ++i) {
        thread_pool.emplace_back(evaluate_dot_, i);
      }

      for (auto &thread : thread_pool) {
        thread.join();
      }

      return result;
    }
  }
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::dot(const Array<T1> &first, const ArraySlice<T2> &other,
                      const usize threads) {
  return Linalg::dot<T3, T1, T2>(
      first, Array<T2>(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::dot(const ArraySlice<T1> &first, const Array<T2> &other,
                      const usize threads) {
  return Linalg::dot<T3, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset), other,
      threads);
}

template <typename T3, typename T1, typename T2>
Array<T3> Linalg::dot(const ArraySlice<T1> &first, const ArraySlice<T2> &other,
                      const usize threads) {
  return Linalg::dot<T3, T1, T2>(
      Array<T1>(*first.array_reference_, first.offset, first.shp_offset),
      Array<T2>(*other.array_reference_, other.offset, other.shp_offset),
      threads);
}

#endif
