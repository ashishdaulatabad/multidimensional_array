#pragma once

#ifndef _CSV_HEADER_HPP_
#define _CSV_HEADER_HPP_
#include <chrono>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "./cell.hpp"
#include "./file_reader.hpp"
#include "./tables.hpp"

// Attributes to classify type of a number
enum NumberAttr { NEGATIVE = 0x01, REAL = 0x02 };

/**
 * @brief Parse scanned string number and return the required type
 * @param value scanned number as string
 * @param attr Attribute denoting type of number (either Positive or Negative,
 * integer or floating)
 * @return A variant of signed, unsigned integer or a real number
 */
Cell return_number(const std::string &value, const u8 attr) {
  switch (attr) {
  case NumberAttr::REAL:
  case NumberAttr::NEGATIVE:
  case (NumberAttr::NEGATIVE | NumberAttr::REAL):
    return Cell(atof(value.c_str()));

  default:
    bool is_neg = value[0] == '-';
    u8 i = value[0] == '+' || is_neg;
    u64 res = 0;

    while (i < value.size()) {
      res = ((res << 3) + (res << 1));
      res += (value[i++]);
      res -= '0';
    }

    return is_neg ? Cell(-res) : Cell(res);
  }
}

/**
 * @brief Read string enclosed between two commas or a comma and newline
 * @param iptr input of a file
 * @param prev_value prev character.
 * @return pair of string and a boolean value to let main loop know that a new
 * line is encountered or not.
 */
std::pair<std::string, bool> read_string_without_quotes(FileReader &fr,
                                                        const char prev_value) {
  bool end_of_string = false;
  char c;
  std::string value;
  value.push_back(prev_value);
  while (!end_of_string) {
    switch (c = fr.read_next_char()) {
    case ',':
    case '\n':
    case EOF:
      end_of_string = true;
      break;

    case ' ':
    case '\r':
      break;

    default:
      value.push_back(c);
      break;
    }
  }

  return {value, c == '\n' || c == EOF};
}

/**
 * @brief Read string enclosed between two commas or a comma and newline
 * @param iptr input of a file
 * @param prev_value prev character.
 * @return pair of string and a boolean value to let main loop know that a new
 * line is encountered or not.
 */
std::pair<std::string, bool>
read_string_without_quotes_mt(FileReader &fr, const char prev_value,
                              usize &__offset) {
  bool end_of_string = false;
  char c;
  std::string value;
  value.push_back(prev_value);
  while (!end_of_string) {
    switch (c = fr.read_char_offset(__offset)) {
    case ',':
    case '\n':
    case EOF:
      end_of_string = true;
      break;

    case ' ':
    case '\r':
      break;

    default:
      value.push_back(c);
      break;
    }
  }
  return {value, c == '\n' || c == EOF};
}

#define MAKE_CASE(R, SR)                                                       \
  case R:                                                                      \
    value.push_back(SR);                                                       \
    break;

/**
 * @brief Read string that are contained within quotes
 * @param iptr pointer to the file
 * @param quotes the quote with which this scan took place
 * @returns string wrapped in Cell Type
 */
std::pair<std::string, bool> read_string_with_quotes(FileReader &fr,
                                                     const char quotes) {
  bool end_of_string = false, end_of_scan = false, eol = false;
  char c;
  std::string value;
  while (!end_of_scan) {
    switch (c = fr.read_next_char()) {
    case '"':
    case '\'':
      end_of_string = (c == quotes);
      break;

    case ',':
    case '\n':
    case EOF:
      end_of_scan = true;
      eol = (c == '\n' || c == EOF);
      break;

    case '\\':
      switch (c = fr.read_next_char()) {
        MAKE_CASE('r', '\r')
        MAKE_CASE('t', '\t')
        MAKE_CASE('n', '\n')
        MAKE_CASE('b', '\b')
      case '"':
      case '\'':
        value.push_back(quotes);
        break;
      }

    case ' ':
    case '\r':
      break;

    default:
      if (!end_of_string) {
        value.push_back(c);
      }
      break;
    }
  }

  return {value, eol};
}

/**
 * @brief Read string that are contained within quotes
 * @param iptr pointer to the file
 * @param quotes the quote with which this scan took place
 * @returns string wrapped in Cell Type
 */
std::pair<std::string, bool>
read_string_with_quotes_mt(FileReader &fr, const char quotes, usize &__offset) {
  bool end_of_string = false, end_of_scan = false, eol = false;
  char c;
  std::string value;
  while (!end_of_scan) {
    switch (c = fr.read_char_offset(__offset)) {
    case '"':
    case '\'':
      end_of_string = (c == quotes);
      break;

    case ',':
    case '\n':
    case EOF:
      end_of_scan = true;
      eol = (c == '\n' || c == EOF);
      break;

    case '\\':
      switch (c = fr.read_next_char()) {
        MAKE_CASE('r', '\r')
        MAKE_CASE('t', '\t')
        MAKE_CASE('n', '\n')
        MAKE_CASE('b', '\b')
      case '"':
      case '\'':
        value.push_back(quotes);
        break;
        // Don't know what the default is:
      }

    case ' ':
    case '\r':
      break;

    default:
      if (!end_of_string) {
        value.push_back(c);
      }
      break;
    }
  }

  return {value, eol};
}

/**
 * @brief Read a potential number as Cell,
 * @param fr File Reader
 * @param p character that was scanned that lead to this function
 * @return scanned number or a string misinterpreted as a number
 */
std::pair<Cell, bool> read_number(FileReader &fr, const char p) {
  std::string value;
  value.push_back(p);
  bool is_neg = p == '-', is_dec = p == '.', is_exp = false, is_invalid = false,
       is_exp_neg = false, is_signed = p == '+' || p == '-',
       is_exp_signed = false, end_of_number = false, is_new_line = false;

  u8 number_attributes = 0;
  is_neg && (number_attributes |= NumberAttr::NEGATIVE);
  is_dec && (number_attributes |= NumberAttr::REAL);

  char c;
  while (!end_of_number) {
    switch (c = fr.read_next_char()) {
    // Read digits as it is.
    case '0' ... '9':
      value.push_back(c);
      break;

    // Read a sign: can either be on a exponent or before a
    // number.
    case '+':
    case '-':
      // 1. We scanned exponent but 'E' or 'e' was not the last part
      // scanned.
      // 2. We scanned the sign already before exponent or about to
      // scan twice after scanning 'e' or 'E'
      is_invalid = (!is_exp || (value.back() == 'e' && value.back() != 'E')) ||
                   ((is_signed && !is_exp) || (is_exp && is_exp_signed));
      number_attributes |= NumberAttr::REAL;
      if (is_exp) {
        is_exp_signed = true;
      }
      value.push_back(c);
      break;

    // Character encountered is a period.
    case '.':
      is_invalid = is_dec;
      is_dec = true;
      number_attributes |= NumberAttr::REAL;
      value.push_back(c);
      break;

    case 'E':
    case 'e':
      is_invalid = is_exp;
      is_exp = true;
      number_attributes |= NumberAttr::REAL;
      value.push_back(c);
      break;

    case '\n':
    case ',':
    case EOF:
      end_of_number = true;
      is_new_line = c == '\n' || c == EOF;
      break;

    case ' ':
    case '\r':
      break;

    default:
      is_invalid = true;
      value.push_back(c);
      break;
    }
  }
  return {is_invalid ? Cell(value) : return_number(value, number_attributes),
          is_new_line};
}

/**
 * @brief Read a potential number as Cell,
 * @param fr File Reader
 * @param p character that was scanned that lead to this function
 * @return scanned number or a string misinterpreted as a number
 */
std::pair<Cell, bool> read_number_mt(FileReader &fr, const char p,
                                     usize &__offset) {
  std::string value;
  value.push_back(p);
  bool is_neg = p == '-', is_dec = p == '.', is_exp = false, is_invalid = false,
       is_exp_neg = false, is_signed = p == '+' || p == '-',
       is_exp_signed = false, end_of_number = false, is_new_line = false;

  u8 number_attributes = 0;
  is_neg && (number_attributes |= NumberAttr::NEGATIVE);
  is_dec && (number_attributes |= NumberAttr::REAL);

  char c;
  while (!end_of_number) {
    switch (c = fr.read_char_offset(__offset)) {
    // Read digits as it is.
    case '0' ... '9':
      value.push_back(c);
      break;

    // Read a sign: can either be on a exponent or before a
    // number.
    case '+':
    case '-':
      // 1. We scanned exponent but 'E' or 'e' was not the last part
      // scanned.
      // 2. We scanned the sign already before exponent or about to
      // scan twice after scanning 'e' or 'E'
      is_invalid = (!is_exp || (value.back() == 'e' && value.back() != 'E')) ||
                   ((is_signed && !is_exp) || (is_exp && is_exp_signed));
      number_attributes |= NumberAttr::REAL;
      if (is_exp) {
        is_exp_signed = true;
      }
      value.push_back(c);
      break;

    // Character encountered is a period.
    case '.':
      is_invalid = is_dec;
      is_dec = true;
      number_attributes |= NumberAttr::REAL;
      value.push_back(c);
      break;

    // An exponent was encountered.
    case 'E':
    case 'e':
      is_invalid = is_exp;
      is_exp = true;
      number_attributes |= NumberAttr::REAL;
      value.push_back(c);
      break;

    // The character encountered is either:
    // 1. comma
    // 2. end of line
    // 3. eof
    case '\n':
    case ',':
    case EOF:
      end_of_number = true;
      is_new_line = c == '\n' || c == EOF;
      break;

    case ' ':
    case '\r':
      break;

    default:
      is_invalid = true;
      value.push_back(c);
      break;
    }
  }
  return {is_invalid ? Cell(value) : return_number(value, number_attributes),
          is_new_line};
}

/**
 * @brief Read CSV using multithreading
 *
 * Premise is: for total threads 'total_thrd', we'll be scanning starting from
 * line `thrd_number` and skipping `total_thrd` line.
 * @param fr File Reader for reading the files
 * @param start start of the file.
 * @param end Column size calculated
 * @param table Actual table that'll be used to store
 * @param thrd_number, process identifier
 * @param total_thrd Total threads that runs the same function
 * @return void
 */
void read_csv_multi_thread(FileReader &fr, const usize start,
                           const u16 col_size, std::vector<Cell *> &table,
                           const u16 thrd_number, const u16 total_thrd) {
  bool eof = false, is_n = false;
  char c = '\0';
  std::string value;
  usize st = start;
  u16 index = 0, rot = 0;
  Cell *row = new Cell[col_size], cell;

  while (!eof) {
    // Read alternate n lines when n threads are present.
    // For e.g., 1st thread will read 1st line,
    // 2nd thread will read 2nd line, and so on.
    // ...
    // nth thread will read nth line.
    // 1st thread will read (n+1)th line
    if (rot > 0) {
      c = fr.read_char_offset(st);
      while (c != '\n' && c != EOF) {
        c = fr.read_char_offset(st);
      }
      eof = (c == EOF);
      rot = (rot + 1) % total_thrd;
    } else {
      switch (c = fr.read_char_offset(st)) {
      // Read string enclosed with quotes (single or double)
      case '"':
      case '\'':
        std::tie(value, is_n) = read_string_with_quotes_mt(fr, c, st);
        row[index++] = value;
        if (is_n) {
          table.emplace_back(row);
          row = new Cell[col_size];
          index = 0;
          rot = (rot + 1) % total_thrd;
        }
        break;

      // Read strings within commas or a comma and a new line
      case 'a' ... 'z':
      case 'A' ... 'Z':
      case '_':
        std::tie(value, is_n) = read_string_without_quotes_mt(fr, c, st);
        row[index++] = value;
        if (is_n) {
          table.emplace_back(row);
          row = new Cell[col_size];
          index = 0;
          rot = (rot + 1) % total_thrd;
        }
        break;

      // Read integer
      case '0' ... '9':
      case '.':
      case '+':
      case '-':
        std::tie(cell, is_n) = read_number_mt(fr, c, st);
        row[index++] = cell;
        if (is_n) {
          table.emplace_back(row);
          row = new Cell[col_size];
          index = 0;
          rot = (rot + 1) % total_thrd;
        }
        break;

      // Don't scan \r or spaces.
      case ' ':
      case '\r':
        break;

      // Read a next line
      case '\n':
        table.emplace_back(row);
        row = new Cell[col_size];
        index = 0;
        rot = (rot + 1) % total_thrd;
        break;

      // Read a separator
      case ',':
        row[index++] = None{};
        break;

      // End of a file
      case EOF:
        eof = true;
        rot = 0;
        break;
      }
    }
  }
}

/**
 * @brief Read file concurrently (using memory mapping)
 * Note: This function is tailored for Windows systems, so the functions differs
 * for linux/macOS systems
 * @param filepath file to read
 * @param thrd_count thread initialized for reading a file
 * @returns Table
 */
Table read_csv(const char *filepath, const u8 thrd_count = 16) {
  FileReader fr;
  // Loads Memory mapped file
  fr.load_file(filepath);

  // Get file size
  usize file_size = fr.size;
  // Calculate column size
  char c;
  Table table;
  usize start_ptr = 0, col_size = 1;
  bool is_n = false, is_eof = false;
  std::string str_values;
  // Read header (make it user decision, default is true)
  while (!is_n) {
    c = fr.read_char_offset(start_ptr);
    switch (c) {
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
      std::tie(str_values, is_n) =
          read_string_without_quotes_mt(fr, c, start_ptr);
      table.header.emplace_back(str_values);
      break;

    case '\'':
    case '"':
      std::tie(str_values, is_n) = read_string_with_quotes_mt(fr, c, start_ptr);
      table.header.emplace_back(str_values);
      break;

    case EOF:
    case '\n':
      is_n = true;
      break;

    default:
      break;
    }
  }
  table.col_size = col_size = table.header.size();
  table.type = std::vector<u8>(col_size);

  /// Initialize the start points for each threads (except for start thread
  /// which starts from zero)
  usize points[thrd_count];
  points[0] = start_ptr;
  for (i32 i = 1; i < thrd_count; ++i) {
    usize start = points[i - 1] + 1; // Next character will start from + 1
    while (fr.buffer[start] != '\n')
      ++start;
    points[i] = start + 1;
  }
  // Create Cell Tables.
  std::vector<std::vector<Cell *>> tables(thrd_count);

  // Create thread vector to manage all the threads
  std::vector<std::thread> st;

  // Push all the threads to array
  for (i32 i = 0; i < thrd_count; ++i) {
    st.emplace_back(std::thread(read_csv_multi_thread, std::ref(fr), points[i],
                                col_size, std::ref(tables[i]), i + 1,
                                thrd_count));
  }

  // wait for all the threads to finish
  for (i32 i = 0; i < thrd_count; ++i) {
    st[i].join();
  }

  usize total_size = 0;
  for (auto &x : tables) {
    total_size += x.size();
  }
  table.table = std::vector<Cell *>(total_size);
  // Merge all the tables.
  // Here we are not considering the order in which they were inserted
  for (usize i = 0; i < total_size; ++i) {
    table.table[i] = tables[i % thrd_count][i / thrd_count];
  }

  fr.close();

  return table;
}

#endif
