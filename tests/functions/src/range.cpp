#include <cassert>
#include <iostream>
#include <vector>

#include "../../../include/md_static/functions.hpp"
#include "../../../include/md_static/md_static_array/md_static_array.hpp"
#include "../../tests.hpp"

int main() {
  Array<u32> range_test = Utils::range<u32>(12);
  Array<u32> expected(std::vector<u32>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}));

  assert(Utils::compare(range_test, expected));

  range_test = Utils::range<u16>(25, 33);
  expected = std::vector<u32>({25, 26, 27, 28, 29, 30, 31, 32});

  assert(Utils::compare(range_test, expected));

  range_test = Utils::range<u16>(33, 25, -1);
  expected = std::vector<u32>({33, 32, 31, 30, 29, 28, 27, 26});

  assert(Utils::compare(range_test, expected));

  auto range_test2 = Utils::range<i16>(33, 25, -1);
  Array<i32> expected2(std::vector<i32>({33, 32, 31, 30, 29, 28, 27, 26}));

  std::cout << "range_test_2: " << range_test2 << '\n';

  assert(Utils::compare(range_test2, expected2));

  Array<f32> range_test3 = Utils::range<f32>(25, 33, 0.5);
  Array<f32> expected3(
      std::vector<f32>({25, 25.5, 26, 26.5, 27, 27.5, 28, 28.5, 29, 29.5, 30,
                        30.5, 31, 31.5, 32, 32.5}));

  assert(Utils::compare(range_test3, expected3));

  Array<f32> range_test4 = Utils::range<f32>(5, -5, -0.5);
  Array<f32> expected4(std::vector<f32>(
      {5, 4.5,  4,  3.5,  3,    2.5,  2,    1.5,  1,    0.5,
       0, -0.5, -1, -1.5, -2.0, -2.5, -3.0, -3.5, -4.0, -4.5}));

  assert(Utils::compare(range_test4, expected4));

  return 0;
}
