#include <iostream>

#include "../../../include/md_static/md_static_array/md_static_array.hpp"
#include "../../tests.hpp"

int main() {
  Array<i8> s;
  {
    s = Array<i8>(10);
    assert(s.get_size() == 10);
    assert(s.get_shape_size() == 1);

    try {
      auto c = s[11];
    } catch (std::runtime_error e) {
      assert(std::string(e.what()).find("Index out of bounds") == 0);
    }
  }
  {
    s = Array<i8>(20, 123);
    assert(s.get_size() == 20);
    assert(s.get_shape_size() == 1);
    for (i8 i = 0; i < s.get_size(); ++i) {
      assert(s[i] == 123);
    }
  }
  // Testing
  {
    std::cout << "here\n";
    s = Array<i8>({20, 30, 40}, 123);

    assert(s.get_size() == 20 * 30 * 40);
    assert(s.get_shape_size() == 3);
    assert(s.get_shape()[0] == 20);
    assert(s.get_shape()[1] == 30);
    assert(s.get_shape()[2] == 40);

    usize index = 0;

    for (usize i = 0; i < s.get_shape()[0]; ++i) {
      for (usize j = 0; j < s.get_shape()[1]; ++j) {
        for (usize k = 0; k < s.get_shape()[2]; ++k) {
          assert(s[i][j][k] == 123);
          ++index;
        }
      }
    }
    assert(index == s.get_size());
  }

  return 0;
}
