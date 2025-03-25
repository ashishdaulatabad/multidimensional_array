#pragma once
#ifndef _COLUMN_VIEW_HPP_
#define _COLUMN_VIEW_HPP_

#include <string>

#include "./cell.hpp"
#include "./md_static/md_static_array/md_static_array.hpp"
#include "./tables.hpp"

struct Table;

struct ColView {
public:
  // Pointer of the header name
  const char *name;
  // Index of the string (easy to identify from the table)
  u16 col_index;
  // Reference of the table ()
  Table *table;

  static const i32 total_threads = 8;

  /**
   * @brief returns the size of array
   */
  inline usize get_size() const { return table->get_size(); }

  /**
   * @brief return index of the value
   */
  inline Cell &operator[](const usize index) {
    return table->table[index][col_index];
  }

  /**
   * @brief Handle delete for ColView
   */
  ~ColView() {
    name = nullptr;
    table = nullptr;
  }

  /**
   * @brief Aggregates the values by combining previous values defined by the
   * function
   * @param function that aggregates previous value and current value
   * @param init initalizing value before combining
   * @returns Result
   */
  Cell aggregate(const fn<Cell(const Cell &, const Cell &)> &func,
                 const Cell &init);

  /**
   * @brief Aggregates (with multi-threading) the values by combining previous
   * defined by the function.
   * @param function that aggregates previous value and current value
   * @param init initalizing value before combining
   * @returns Result
   */
  Cell aggregate_mt(const fn<Cell(const Cell &, const Cell &)> &func,
                    const Cell &init);

  /**
   * @brief Maps the values and returns the new array.
   */
  template <typename T>
  MdStaticArray<T>
  st_map(const fn<T(const Cell &, const usize, const ColView &)> &func);

  /**
   * @brief Maps the values and returns the new array.
   * Uses multi-threading
   */
  template <typename T>
  MdStaticArray<T>
  st_map_mt(const fn<T(const Cell &, const usize, const ColView &)> &func);

  /**
   * @brief get values of a cell
   */
  template <typename T> friend T get_values(const Cell &);

  /**
   * @brief Maps the values and returns the new array.
   */
  template <typename T> MdStaticArray<T> st_map();

  /**
   * @brief Maps the values and returns the new array.
   */
  template <typename T> MdStaticArray<T> st_map_mt();
};

template <typename T,
          class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
T get_values(const Cell &value) {
  switch (value.index()) {
  case 0:
    return std::get<i64>(value);
  case 1:
    return std::get<u64>(value);
  case 2:
    return std::get<f64>(value);
  default:
    return 0;
  }
}

template <typename _T, class = typename std::enable_if<
                           std::is_same<_T, std::string>::value>::type>
std::string get_values(const Cell &value) {
  switch (value.index()) {
  case 3:
    return std::get<std::string>(value);
  default:
    return 0;
  }
}

Cell ColView::aggregate(const fn<Cell(const Cell &, const Cell &)> &func,
                        const Cell &init) {
  Cell start = init;
  for (auto &Table : table->table) {
    start = func(start, Table[col_index]);
  }

  return start;
}

Cell ColView::aggregate_mt(const fn<Cell(const Cell &, const Cell &)> &func,
                           const Cell &init) {
  std::vector<Cell> init_array(ColView::total_threads, init);
  std::vector<std::thread> thread_pool;

  auto aggregate_internal = [func, &init, &init_array, this](
                                const usize start, const uint8_t thread_count) {
    for (usize i = start; i < this->get_size(); i += thread_count) {
      init_array[start] = func(init_array[start], this->operator[](i));
    }
  };

  for (i32 i = 0; i < ColView::total_threads; ++i) {
    thread_pool.emplace_back(
        std::thread(aggregate_internal, i, ColView::total_threads));
  }

  for (auto &thrd : thread_pool) {
    thrd.join();
  }

  Cell final = init;
  for (i32 i = 0; i < ColView::total_threads; ++i) {
    final = func(final, init_array[i]);
  }

  return final;
}

template <typename T>
MdStaticArray<T>
ColView::st_map(const fn<T(const Cell &, const usize, const ColView &)> &func) {
  const usize size = this->get_size();
  MdStaticArray<_T> value(size);
  const ColView &ptr = *this;
  for (usize index = 0; index < size; ++index) {
    value[index] = func((*this)[index], index, ptr);
  }

  return value;
}

template <typename T>
MdStaticArray<T> ColView::st_map_mt(
    const fn<T(const Cell &, const usize, const ColView &)> &func) {
  const usize size = this->get_size();
  MdStaticArray<T> value(size);
  std::vector<std::thread> thread_pool;

  auto mp_internal = [&func, this, &value](const usize start, const usize end) {
    const ColView &ptr = *this;
    for (usize index = start; index < end; ++index) {
      value[index] = func(this->operator[](index), index, ptr);
    }
  };

  const usize block = size / ColView::total_threads;
  const uint8_t t_but_one = ColView::total_threads - 1;

  for (i32 i = 0; i < t_but_one; ++i) {
    thread_pool.emplace_back(
        std::thread(mp_internal, block * i, block * (i + 1)));
  }

  thread_pool.emplace_back(std::thread(mp_internal, block * t_but_one, size));

  for (auto &thrd : thread_pool) {
    thrd.join();
  }

  return value;
}

template <typename T> Array<T> ColView::st_map() {
  const usize size = this->get_size();
  Array<T> value(size);
  for (usize ptr = 0; ptr < size; ++ptr) {
    value[ptr] = get_values<_T>(this->operator[](ptr));
  }

  return value;
}

template <typename T> Array<T> ColView::st_map_mt() {
  const usize size = this->get_size();
  Array<T> value(size);
  std::vector<std::thread> thread_pool;

  auto mp_internal = [this, &value](const usize start, const usize end) {
    for (usize ptr = start; ptr < end; ++ptr) {
      value[ptr] = get_values<T>((*this)[ptr]);
    }
  };

  const usize block = size / ColView::total_threads;
  const u8 t_but_one = ColView::total_threads - 1;

  for (i32 i = 0; i < t_but_one; ++i) {
    thread_pool.emplace_back(
        std::thread(mp_internal, block * i, block * (i + 1)));
  }

  thread_pool.emplace_back(std::thread(mp_internal, block * t_but_one, size));

  for (auto &thrd : thread_pool) {
    thrd.join();
  }

  return value;
}

#endif
