#pragma once
#ifndef _MD_STATIC_ARRAY_OP_HPP_
#define _MD_STATIC_ARRAY_OP_HPP_
#include <vector>

#include "./md_static_array.hpp"

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::add_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < size; ++index) {
      result.array_[index] = array_[index] + other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] + other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::add_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(get_size());
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] + other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] + other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::sub_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < size; ++index) {
      result.array_[index] = array_[index] - other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] - other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::sub_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] - other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] - other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::sub_iointernal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = other - array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = other - array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::mul_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] * other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] * other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::mul_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] * other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] * other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::div_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] / other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] / other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::div_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] / other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] / other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::div_iointernal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = other / array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = other / array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::mod_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] % other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] % other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::mod_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] % other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] % other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::mod_iointernal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = other % array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = other % array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename T1>
void MdStaticArray<T>::add_self_internal_(const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] += other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] += other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::sub_self_internal_(const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] -= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] -= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::mul_self_internal_(const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] *= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] *= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::div_self_internal_(const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] /= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] /= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::mod_self_internal_(const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] %= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] %= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename T1>
void MdStaticArray<T>::add_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] += other;
      // std::cout << "here " << index << " " << size << std::endl;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] += other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::sub_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] -= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] -= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::mul_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] *= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] *= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::div_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] /= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] /= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::mod_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] %= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] %= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::and_bit_self_internal_(const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] &= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] &= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::and_bit_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] &= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] &= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::or_bit_self_internal_(const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] |= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] |= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::or_bit_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] |= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] |= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::xor_bit_self_internal_(const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] ^= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] ^= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::xor_bit_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] ^= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] ^= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::lshft_bit_self_internal_(
    const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] <<= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] <<= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::lshft_bit_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] <<= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] <<= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::rshft_bit_self_internal_(
    const MdStaticArray<T1> &other) {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] >>= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] >>= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

template <typename T>
template <typename T1>
void MdStaticArray<T>::rshft_bit_self_iinternal_(const T1 &other) {
  // assert that sizes are equal
  const usize size = get_size();
  if (::s_thread_count == 1 || get_size() <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      array_[index] >>= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [this, &other](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        array_[index] >>= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_leq_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] <= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] <= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename ghost>
MdStaticArray<bool>
MdStaticArray<T>::comp_geq_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] >= other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] >= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_eq_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] == other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] == other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_l_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] < other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] < other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_g_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] > other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] > other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_neq_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] != other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] != other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_leq_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] <= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] <= other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_geq_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] >= other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] >= other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_eq_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] == other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] == other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool> MdStaticArray<T>::comp_l_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] < other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] < other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool> MdStaticArray<T>::comp_g_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] > other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] > other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1>
MdStaticArray<bool>
MdStaticArray<T>::comp_neq_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<bool> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < size; ++index) {
      result.array_[index] = array_[index] != other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] != other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::and_bit_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] & other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] & other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::and_bit_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] & other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] & other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::or_bit_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] | other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] | other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::or_bit_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] ^ other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] ^ other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::xor_bit_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] ^ other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] ^ other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2> MdStaticArray<T>::xor_bit_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] ^ other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] ^ other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

// Bit shifting operation
template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::lshft_bit_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] << other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] << other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::lshft_bit_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] << other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] << other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::lshft_bit_iointernal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = other << array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = other << array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::rshft_bit_internal_(const MdStaticArray<T1> &other) const {
  // assert that sizes are equal
  if (!is_same_shape(other)) {
    throw std::runtime_error("Dimensions do not match.");
  }
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] >> other.array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] >> other.array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::rshft_bit_iinternal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = array_[index] >> other;
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = array_[index] >> other;
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T>
template <typename T1, typename T2>
MdStaticArray<T2>
MdStaticArray<T>::rshft_bit_iointernal_(const T1 &other) const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T2> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = other >> array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto _add_int = [&result, this, &other](const usize start,
                                            const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = other >> array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(_add_int, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(_add_int, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

template <typename T> MdStaticArray<T> MdStaticArray<T>::ng_internal_() const {
  // assert that sizes are equal
  const usize size = get_size();
  MdStaticArray<T> result(size);
  result.init_shape(shape, shp_size);
  if (::s_thread_count == 1 || size <= s_threshold_size) {
    for (usize index = 0; index < get_size(); ++index) {
      result.array_[index] = -array_[index];
    }
  } else {
    std::vector<std::thread> st;
    st.reserve(::s_thread_count);
    auto __ng = [&result, this](const usize start, const usize end) {
      for (usize index = start; index < end; ++index) {
        result.array_[index] = -array_[index];
      }
    };

    const usize block = size / s_thread_count;
    const i8 thread_but_one = s_thread_count - 1;
    for (i32 i = 0; i < thread_but_one; ++i) {
      st.emplace_back(std::thread(__ng, block * i, block * (i + 1)));
    }

    st.emplace_back(std::thread(__ng, block * thread_but_one, size));

    for (auto &th : st) {
      th.join();
    }
  }
  return result;
}

#endif
