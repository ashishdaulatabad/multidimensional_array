#pragma once
#ifndef _TABLE_COLVIEW_COMMON_HPP_
#define _TABLE_COLVIEW_COMMON_HPP_

#include "./column_view.hpp"
#include "./tables.hpp"

ColView Table::operator[](const std::string &val) {
  u16 index = std::find(header.begin(), header.end(), val) - header.begin();
  return {header[index].c_str(), index, this};
}

template <typename T1, typename T2>
std::pair<MdStaticArray<T1>, MdStaticArray<T2>>
Table::split_dep_and_indep_variables(
    const std::string &classifier_header_name) {
  const usize index =
      std::find(header.begin(), header.end(), classifier_header_name) -
      header.begin();

  if (index >= header.size()) {
    return {MdStaticArray<T1>(), MdStaticArray<T2>()};
  }

  std::vector<u16> other_indices;
  for (i32 i = 0; i < col_size; ++i) {
    if (index != i) {
      other_indices.push_back(i);
    }
  }

  MdStaticArray<T1> values(
      std::vector<usize>({table.size(), other_indices.size()}), 0);
  MdStaticArray<T2> classifier_array(table.size());

  usize t_index = 0;
  for (i32 i = 0; i < table.size(); ++i) {
    for (auto &cindex : other_indices) {
      values[t_index++] = get_values<T1>(table[i][cindex]);
    }
    classifier_array[i] = get_values<T2>(table[i][index]);
  }

  return {values, classifier_array};
}

#endif