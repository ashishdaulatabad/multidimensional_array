#pragma once
#ifndef _MD_STATIC_AXIS_REFERENCE_HPP_
#define _MD_STATIC_AXIS_REFERENCE_HPP_

#include "./md_static_array.hpp"

template <class T> struct MdStaticAxisReference {
  const MdStaticArray<T> *arr_ptr_{nullptr};
  i32 axis_{0};
  usize stride_{0};
  usize total_number_of_axes_{0};
  usize current_axes_{0};
  usize start_{0};
  i32 axis_size_{0};

public:
  constexpr MdStaticAxisReference() {
    arr_ptr_ = nullptr;
    axis_ = 0;
    stride_ = 0;
    total_number_of_axes_ = 0;
    current_axes_ = start_ = axis_size_ = 0;
  }

  constexpr MdStaticAxisReference(const MdStaticArray<T> &other) {
    for (usize index = 0; index < axis_size_; ++index) {
      (*this)[index] = other.array_[index];
    }
  }

  constexpr MdStaticAxisReference &operator=(const MdStaticArray<T> &other) {
    for (usize index = 0; index < axis_size_; ++index) {
      (*this)[index] = other.array_[index];
    }

    return *this;
  }

  constexpr MdStaticAxisReference(const MdStaticArray<T> &new_array,
                                  const usize a) {
    arr_ptr_ = &new_array;
    axis_ = a;
    stride_ = new_array.skip_vec[axis_];
    axis_size_ = new_array.shape[axis_];
    total_number_of_axes_ = new_array.get_size() / new_array.shape[axis_];
    current_axes_ = 0;
    start_ = 0;
  }

  constexpr MdStaticAxisReference(const MdStaticArray<T> &new_array,
                                  const usize a,
                                  const usize current_axis_init) {
    arr_ptr_ = &new_array;
    axis_ = a;
    stride_ = new_array.skip_vec[axis_];
    axis_size_ = new_array.shape[axis_];
    total_number_of_axes_ = new_array.get_size() / new_array.shape[axis_];
    current_axes_ = current_axis_init;
    const usize dim_skip = stride_ * axis_size_;
    start_ = (current_axis_init / stride_) * (dim_skip) +
             current_axis_init % stride_;
  }

  constexpr __always_inline T &operator[](const usize index) const {
    return arr_ptr_->array_[start_ + stride_ * index];
  }

  inline bool switch_to_next_axis_index() {
    ++current_axes_;

    if (current_axes_ >= total_number_of_axes_) {
      return false;
    }

    ++start_;

    if (current_axes_ % stride_ == 0) {
      start_ += (stride_ * (axis_size_ - 1));
    }

    return true;
  }

  friend std::ostream &operator<<(std::ostream &op,
                                  const MdStaticAxisReference &arr) {
    op << '[';

    for (usize index = 0; index < arr.get_size(); ++index) {
      op << arr[index];
      if (index < arr.get_size() - 1) {
        op << ", ";
      }
    }

    op << ']';
    return op;
  }

  constexpr inline usize get_size() const { return axis_size_; }

  constexpr inline usize get_axis_index() const { return current_axes_; }

  constexpr inline usize get_total_axes() const {
    return total_number_of_axes_;
  }
};

template <typename T> using Axis = MdStaticAxisReference<T>;

#endif
