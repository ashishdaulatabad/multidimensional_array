#pragma once
#ifndef _CELL_HPP_
#define _CELL_HPP_

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <variant>

#include "./md_static/md_static_array/md_static_array.hpp"
#include "./utility/math.hpp"

typedef struct None {
} None;

typedef struct Err {
  u8 err;
} Err;

/// @brief Variant of a type (only works on C++17 and above, use flag
/// `-std=c++17`)
/// @todo perform basic add, subtract operation
typedef std::variant<i64, u64, f64, std::string, None> Cell;

// Error enum to display the error messages
enum Error { END, UNEXPECTED_TOKEN, INVALID_OPERATION };

// Enum for marking data type for each column
enum DataType {
  INTEGER = 0b00001,
  UNSIGNED_INTEGER = 0b00010,
  REAL_NUMBER = 0b00100,
  STRING = 0b01000,
  NONE = 0b10000
};

struct ColView;
struct Table;

/**
 * @brief Output operator for displaying None Type
 * @param op Output stream object
 * @param none None object
 * @return op
 */
std::ostream &operator<<(std::ostream &op, const None &none) {
  op << "None";
  return op;
}

/**
 * @brief Output operator for displaying Variant type
 * @param op Output stream object
 * @param value value of variant type
 * @return op
 */
std::ostream &operator<<(std::ostream &op, const Cell &value) {
  std::visit([&op](const auto &x) { op << x; }, value);
  return op;
}
//----------------------- + operator ----------------------//

/**
 * @brief operator +
 */
Cell operator+(const Cell &first, const Cell &second) {
  switch (first.index()) {
  case 0:
    return second + std::get<i64>(first);
  case 1:
    return second + std::get<u64>(first);
  case 2:
    return second + std::get<f64>(first);
  default:
    return Cell(None{});
  }
}

Cell operator-(const Cell &first) {
  switch (first.index()) {
  case 0:
    return Cell(-std::get<i64>(first));
  case 1:
    return Cell(static_cast<usize>(-std::get<u64>(first)));
  case 2:
    return Cell(-std::get<f64>(first));
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator +
 */
Cell operator-(const Cell &first, const Cell &second) {
  switch (first.index()) {
  case 0:
    return -second + std::get<i64>(first);
  case 1:
    return -second + std::get<u64>(first);
  case 2:
    return -second + std::get<f64>(first);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator +
 */
Cell operator*(const Cell &first, const Cell &second) {
  switch (first.index()) {
  case 0:
    return second * std::get<i64>(first);
  case 1:
    return second * std::get<u64>(first);
  case 2:
    return second * std::get<f64>(first);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator +
 */
Cell operator/(const Cell &first, const Cell &second) {
  switch (first.index()) {
  case 0:
    return std::get<i64>(first) / second;
  case 1:
    return std::get<u64>(first) / second;
  case 2:
    return std::get<f64>(first) / second;
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator +
 */
Cell operator%(const Cell &first, const Cell &second) {
  switch (first.index()) {
  case 0:
    return std::get<i64>(first) % second;
  case 1:
    return std::get<u64>(first) % second;
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator +
 */
Cell operator+(const Cell &first, const i64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) + second);
  case 1:
    return Cell(std::get<u64>(first) + second);
  case 2:
    return Cell(std::get<f64>(first) + second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator +
 */
Cell operator+(const Cell &first, const u64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) + second);
  case 1:
    return Cell(std::get<u64>(first) + second);
  case 2:
    return Cell(std::get<f64>(first) + second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator +
 */
Cell operator+(const Cell &first, const f64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) + second);
  case 1:
    return Cell(std::get<u64>(first) + second);
  case 2:
    return Cell(std::get<f64>(first) + second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator +
 */
Cell operator+(const Cell &first, const std::string &second) {
  switch (first.index()) {
  case 3:
    return Cell(std::get<std::string>(first) + second);
  default:
    return Cell(None{});
  }
}

//----------------------- - operator ----------------------//

/**
 * @brief operator -
 */
Cell operator-(const Cell &first, const i64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) - second);
  case 1:
    return Cell(std::get<u64>(first) - second);
  case 2:
    return Cell(std::get<f64>(first) - second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator -
 */
Cell operator-(const Cell &first, const u64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) - second);
  case 1:
    return Cell(std::get<u64>(first) - second);
  case 2:
    return Cell(std::get<f64>(first) - second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator -
 */
Cell operator-(const Cell &first, const f64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) - second);
  case 1:
    return Cell(std::get<u64>(first) - second);
  case 2:
    return Cell(std::get<f64>(first) - second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator *
 */
Cell operator*(const Cell &first, const i64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) * second);
  case 1:
    return Cell(std::get<u64>(first) * second);
  case 2:
    return Cell(std::get<f64>(first) * second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator *
 */
Cell operator*(const Cell &first, const u64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) * second);
  case 1:
    return Cell(std::get<u64>(first) * second);
  case 2:
    return Cell(std::get<f64>(first) * second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator *
 */
Cell operator*(const Cell &first, const f64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) * second);
  case 1:
    return Cell(std::get<u64>(first) * second);
  case 2:
    return Cell(std::get<f64>(first) * second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator/(const Cell &first, const i64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) / second);
  case 1:
    return Cell(std::get<u64>(first) / second);
  case 2:
    return Cell(std::get<f64>(first) / second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator/(const Cell &first, const u64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) / second);
  case 1:
    return Cell(std::get<u64>(first) / second);
  case 2:
    return Cell(std::get<f64>(first) / second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator/(const Cell &first, const f64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) / second);
  case 1:
    return Cell(std::get<u64>(first) / second);
  case 2:
    return Cell(std::get<f64>(first) / second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator/(const i64 second, const Cell &first) {
  switch (first.index()) {
  case 0:
    return Cell(second / std::get<i64>(first));
  case 1:
    return Cell(second / std::get<u64>(first));
  case 2:
    return Cell(second / std::get<f64>(first));
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator/(const u64 second, const Cell &first) {
  switch (first.index()) {
  case 0:
    return Cell(second / std::get<i64>(first));
  case 1:
    return Cell(second / std::get<u64>(first));
  case 2:
    return Cell(second / std::get<f64>(first));
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator/(const f64 second, const Cell &first) {
  switch (first.index()) {
  case 0:
    return Cell(second / std::get<i64>(first));
  case 1:
    return Cell(second / std::get<u64>(first));
  case 2:
    return Cell(second / std::get<f64>(first));
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator%(const Cell &first, const i64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) % second);
  case 1:
    return Cell(std::get<u64>(first) % second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator%(const Cell &first, const u64 second) {
  switch (first.index()) {
  case 0:
    return Cell(std::get<i64>(first) / second);
  case 1:
    return Cell(std::get<u64>(first) / second);
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator%(const i64 second, const Cell &first) {
  switch (first.index()) {
  case 0:
    return Cell(second % std::get<i64>(first));
  case 1:
    return Cell(second % std::get<u64>(first));
  default:
    return Cell(None{});
  }
}

/**
 * @brief operator /
 */
Cell operator%(const u64 second, const Cell &first) {
  switch (first.index()) {
  case 0:
    return Cell(second % std::get<i64>(first));
  case 1:
    return Cell(second % std::get<u64>(first));
  default:
    return Cell(None{});
  }
}

#endif
