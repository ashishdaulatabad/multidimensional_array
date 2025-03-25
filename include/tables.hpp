#pragma once
#ifndef _TABLES_HPP_
#define _TABLES_HPP_
#include <ostream>
#include <vector>

#include "./cell.hpp"

/**
 * @brief Table struct for easier data handling
 */
struct Table {
public:
  // Header for the table
  std::vector<std::string> header;
  // Actual table
  std::vector<Cell *> table;
  // Column type marking
  std::vector<u8> type;
  // Table size
  u16 col_size;
  // Display info
  u8 first_last_display = 5;

  // Get size
  inline usize get_size() const { return table.size(); }

  // get col size
  inline usize get_col_size() const { return col_size; }

  /**
   * @brief Get header column
   * @param val string
   * @returns Column View of the Table
   * @note Changing views within ColView results in changes in
   * main table (to perform, clone)
   */
  ColView operator[](const std::string &val);

  /**
   * @brief Arrange the table
   */
  template <typename T1, typename T2>
  std::pair<MdStaticArray<T1>, MdStaticArray<T2>>
  split_dep_and_indep_variables(const std::string &classifier_header_name);

  /**
   * @brief Display ostream operator for displaying few table entries
   * @param op ostream object
   * @param table Table
   */
  friend std::ostream &operator<<(std::ostream &op, const Table &table) {
    for (auto &header_name : table.header) {
      op << std::setw(15) << header_name;
    }
    op << "\n";
    if (table.table.size() > table.first_last_display * 2) {
      for (i32 i = 0; i < table.first_last_display; ++i) {
        for (auto j = 0; j < table.col_size; ++j) {
          op << std::setw(15) << table.table[i][j];
        }
        op << '\n';
      }
      op << '\n';
      for (i32 i = 0; i < 3; ++i) {
        for (auto j = 0; j < table.col_size; ++j) {
          op << std::setw(15) << "....";
        }
        op << '\n';
      }
      op << '\n';
      for (i32 i = table.table.size() - table.first_last_display;
           i < table.table.size(); ++i) {
        for (auto j = 0; j < table.col_size; ++j) {
          op << std::setw(15) << table.table[i][j];
        }
        op << '\n';
      }
      op << '\n';
    } else {
      for (i32 i = 0; i < table.table.size(); ++i) {
        for (auto j = 0; j < table.col_size; ++j) {
          op << std::setw(15) << table.table[i][j];
        }
        op << '\n';
      }
    }
    return op;
  }
};

#endif
