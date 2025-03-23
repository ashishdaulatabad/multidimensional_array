#include <cassert>
#include <iostream>
#include <vector>

#include "../../../include/md_static/functions.hpp"
#include "../../../include/md_static/md_static_array/md_static_array.hpp"
#include "../../tests.hpp"

int main() {
  Array<u32> range_test = Utils::range<u32>(12);
  auto result = Utils::argmax(range_test);
  std::cout << result << std::endl;
  assert(result[0] == 11);

  return 0;
}
