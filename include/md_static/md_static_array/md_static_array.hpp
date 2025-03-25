#pragma once

#ifndef _MD_STATIC_ARRAY_HPP_
#define _MD_STATIC_ARRAY_HPP_

#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

#include "../../utility/alloc.hpp"
#include "./md_type_inference.hpp"

// Todo: Create a thread pool.
static usize s_threshold_size = 10000000;
static u8 s_thread_count = 16;

template <typename T> class ArraySlice;

template <typename T> class MdStaticAxisReference;

template <typename T> class MdStaticArray {
  void init_shape(const usize *_shape, const usize _shp_size) {
    if (shape) {
      free(shape);
      skip_vec = nullptr;
    }

    if (skip_vec) {
      free(skip_vec);
      skip_vec = nullptr;
    }

    shp_size = _shp_size;
    shape = static_cast<usize *>(malloc(shp_size * sizeof(usize)));
    skip_vec = static_cast<usize *>(malloc(shp_size * sizeof(usize)));
    shape[shp_size - 1] = _shape[shp_size - 1];
    skip_vec[shp_size - 1] = 1;

    for (usize i = shp_size - 2; i < shp_size; --i) {
      shape[i] = _shape[i];
      skip_vec[i] = _shape[i + 1] * skip_vec[i + 1];
    }
  }

  void init_shape(const usize size) {
    if (shape) {
      free(shape);
      skip_vec = nullptr;
    }

    if (skip_vec) {
      free(skip_vec);
      skip_vec = nullptr;
    }

    shape = static_cast<usize *>(malloc(sizeof(usize)));
    skip_vec = static_cast<usize *>(malloc(sizeof(usize)));
    shape[0] = size;
    skip_vec[0] = 1;
    shp_size = 1;
  }

  /**
   * @brief create a temporary ArraySlice for MdStaticArray::ArraySlice.
   * This will not accessible to user.
   */
  MdStaticArray(const MdStaticArray<T> &other, const usize offset,
                const u16 shp_offset)
      : shape(nullptr), skip_vec(nullptr) {
    array_ = &other.array_[offset];
    shape = &other.shape[shp_offset];
    skip_vec = &other.skip_vec[shp_offset];
    shp_size = other.shp_size - shp_offset;
    size_ = other.size_;

    usize index = 0;

    while (index < shp_offset) {
      size_ /= other.shape[index++];
    }

    dont_free = true;
  }

  bool dont_free = false;

public:
  T *array_;
  usize *shape;
  usize *skip_vec;
  usize size_;
  u16 shp_size;

  template <typename T1> friend class ArraySlice;
  template <typename T1> friend class MdStaticAxisReference;
  template <typename T1> friend class MdStaticArray;

  friend struct MdArrayUtility;
  friend struct MdLinearAlgebra;
  friend struct MdArrayManipulate;
  friend struct FFT;
  friend struct Signal;

  friend std::ostream &operator<<(std::ostream &op, const ArraySlice<T> &ot);

  static void set_thread_count(const u8 value);

  static void set_threshold_size(const usize size);

  constexpr void init_array(const usize size) {
    if constexpr (MdTypeInfer::is_mallocable<T>::value) {
      array_ = aligned_allocate<T>(64, size);
    } else {
      array_ = new T[size];
    }

    size_ = size;
  }

  explicit MdStaticArray(const usize size) : shape(nullptr), skip_vec(nullptr) {
    init_array(size);
    init_shape(size);
  }

  MdStaticArray(const usize size, const T &value)
      : shape(nullptr), skip_vec(nullptr) {
    init_array(size);
    init_shape(size);

    for (usize index = 0; index < size; ++index) {
      array_[index] = value;
    }
  }

  MdStaticArray(const std::vector<usize> &_shape, const T &value)
      : shape(nullptr), skip_vec(nullptr) {
    usize overall_size = 1;

    for (auto &dim : _shape) {
      overall_size *= dim;
    }

    init_array(overall_size);
    init_shape(_shape.data(), _shape.size());

    for (usize index = 0; index < size_; ++index) {
      array_[index] = value;
    }
  }

  MdStaticArray() : shape(nullptr), skip_vec(nullptr) { array_ = nullptr; }

  explicit MdStaticArray(const std::vector<T> &other)
      : shape(nullptr), skip_vec(nullptr) {
    size_ = other.size();
    init_array(size_);
    init_shape(size_);

    for (usize index = 0; index < size_; ++index) {
      array_[index] = other[index];
    }
  }

  MdStaticArray(const MdStaticArray &other)
      : shape(nullptr), skip_vec(nullptr) {
    init_array(other.get_size());
    const auto shp = other.get_shape();
    init_shape(shp, other.shp_size);

    for (usize index = 0; index < size_; ++index) {
      array_[index] = other.array_[index];
    }
  }

  MdStaticArray(MdStaticArray<T> &&other) {
    array_ = other.array_;
    other.array_ = nullptr;

    size_ = other.size_;
    shp_size = other.shp_size;
    shape = other.shape;
    other.shape = nullptr;

    skip_vec = other.skip_vec;
    other.skip_vec = nullptr;

    dont_free = other.dont_free;
    other.dont_free = true;
  }

  MdStaticArray &operator=(MdStaticArray<T> &&other) {
    array_ = other.array_;
    other.array_ = nullptr;

    size_ = other.size_;
    shp_size = other.shp_size;
    shape = other.shape;
    other.shape = nullptr;

    skip_vec = other.skip_vec;
    other.skip_vec = nullptr;

    dont_free = other.dont_free;
    other.dont_free = true;

    return *this;
  }

  /**
   * @brief Assigning ArraySlice to the newly created array
   */
  MdStaticArray(const ArraySlice<T> &other);

  /**
   * @brief Casting operator
   */
  inline T operator()() const { return array_[0]; }

  /**
   * @brief Casting operator
   */
  inline operator T() const {
    if (size_ > 1) {
      throw std::runtime_error(
          "Value casted should be a single element, found with size: " +
          std::to_string(size_));
    }
    return array_[0];
  }

  /**
   * @brief Casting operator
   */
  template <typename T1> operator MdStaticArray<T1>() const {
    std::vector<usize> shp;
    for (usize index = 0; index < shp_size; ++index) {
      shp.emplace_back(shape[index]);
    }
    MdStaticArray<T1> result(shp, 0);

    for (usize index = 0; index < size_; ++index) {
      result.array_[index] = array_[index];
    }

    return result;
  }

  /**
   * @brief Assignment operator (direct vector assignment)
   */
  MdStaticArray &operator=(const std::vector<T> other) {
    skip_vec = nullptr;
    shape = nullptr;
    size_ = other.size();
    init_array(size_);
    init_shape(size_);

    for (usize index = 0; index < size_; ++index) {
      array_[index] = other[index];
    }

    return *this;
  }

  /**
   * @brief Assignment operator
   */
  MdStaticArray &operator=(const MdStaticArray &other) {
    skip_vec = nullptr;
    shape = nullptr;
    size_ = other.get_size();
    init_array(size_);
    const auto shp = other.get_shape();
    init_shape(shp, other.shp_size);

    for (usize index = 0; index < size_; ++index) {
      array_[index] = other.array_[index];
    }

    return *this;
  }

  /**
   * @brief Assigning ArraySlice to the newly created array
   */
  MdStaticArray<T> &operator=(const ArraySlice<T> &other);

  /**
   * @brief Destructor for array
   */
  ~MdStaticArray() {
    if (!dont_free) {
      if (array_ != nullptr) {
        if constexpr (std::is_fundamental<T>::value) {
          aligned_free(array_);
        } else {
          delete[] array_;
        }

        array_ = nullptr;
      }

      if (shape != nullptr) {
        free(shape);
        shape = nullptr;
      }

      if (skip_vec != nullptr) {
        free(skip_vec);
        skip_vec = nullptr;
      }
    } else {
      array_ = nullptr;
      shape = nullptr;
      skip_vec = nullptr;
    }
  }

  /**
   * @brief Get shape of the object
   */
  inline usize *get_shape() const { return shape; }

  /**
   * @brief Get shape length
   * @param void
   * @returns u16 shape of the array
   */
  inline u16 get_shape_size() const { return shp_size; }

  /**
   * @brief Check whether arrays have same structure
   * @param other array to compare
   * @return true if they have same structure, else false
   */
  template <typename T1>
  bool is_same_shape(const MdStaticArray<T1> &other) const {
    if (shp_size != other.shp_size) {
      return false;
    }

    if (get_size() != other.get_size()) {
      return false;
    }

    for (usize index = 0; index < shp_size; ++index) {
      if (shape[index] != other.shape[index]) {
        return false;
      }
    }

    return true;
  }

  MdStaticAxisReference<T> get_axis_reference(const usize axis);

  MdStaticAxisReference<T> get_nth_axis_reference(const usize axis,
                                                  const usize n);

  /**
   * @brief Add function, currently using threads
   * @param other other array (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> add_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Add function, currently using threads
   * @param other other integer (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> add_iinternal_(const T1 &other) const;

  /**
   * @brief Subtract function, currently using threads
   * @param other other array (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> sub_internal_(const MdStaticArray<T1> &) const;

  /**
   * @brief Subtract function, currently using threads
   * @param other other integer (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> sub_iinternal_(const T1 &) const;

  /**
   * @brief Subtract function, currently using threads
   * @param other other integer (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> sub_iointernal_(const T1 &) const;

  /**
   * @brief Multiplication function, currently using threads
   * @param other other array (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> mul_internal_(const MdStaticArray<T1> &) const;

  /**
   * @brief Multiplication function, currently using threads
   * @param other other integer (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> mul_iinternal_(const T1 &other) const;

  /**
   * @brief Division function, currently using threads
   * @param other other array (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> div_internal_(const MdStaticArray<T1> &) const;

  /**
   * @brief Division function, currently using threads
   * @param other other integer (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> div_iinternal_(const T1 &other) const;

  /**
   * @brief Modulo function, currently using threads
   * @param other other integer (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> div_iointernal_(const T1 &other) const;

  /**
   * @brief Modulo function, currently using threads
   * @param other other array (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> mod_internal_(const MdStaticArray<T1> &) const;

  /**
   * @brief Division function, currently using threads
   * @param other other integer (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> mod_iinternal_(const T1 &other) const;

  /**
   * @brief Modulo function, currently using threads, but the operand is to be
   * modded
   * @param other other integer (might be of different type)
   * @returns new array of current type
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> mod_iointernal_(const T1 &other) const;

  /**
   * @brief Bitwise AND function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> and_bit_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Bitwise AND function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> and_bit_iinternal_(const T1 &other) const;

  /**
   * @brief Bitwise OR function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> or_bit_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Bitwise OR function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> or_bit_iinternal_(const T1 &other) const;

  /**
   * @brief Bitwise XOR function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> xor_bit_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Bitwise XOR function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> xor_bit_iinternal_(const T1 &other) const;

  /**
   * @brief Bitwise left shift function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> lshft_bit_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Bitwise left shift function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> lshft_bit_iinternal_(const T1 &other) const;

  /**
   * @brief Bitwise left shift function between two arrays, but here
   * other is shifted using data in array
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> lshft_bit_iointernal_(const T1 &other) const;

  /**
   * @brief Bitwise right shift function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> rshft_bit_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Bitwise right shift function between two arrays
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> rshft_bit_iinternal_(const T1 &other) const;

  /**
   * @brief Bitwise right shift function between two arrays, but here
   * other is shifted using data in array
   * @param other other array to perform bitwise and
   * @returns new array
   */
  template <typename T1,
            typename T2 = typename std::remove_const<
                decltype(MdTypeInfer::eval_resultant_t<T1, T>::value)>::type>
  MdStaticArray<T2> rshft_bit_iointernal_(const T1 &other) const;

  /**
   * @brief Negative operator
   * @returns new array
   */
  MdStaticArray ng_internal_(void) const;

  /**
   * @brief Add to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1>
  void add_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Subtract to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1>
  void sub_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Multiply to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1>
  void mul_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Divide to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1>
  void div_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Modulus to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1>
  void mod_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Add to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1> void add_self_iinternal_(const T1 &other);

  /**
   * @brief Subtract to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1> void sub_self_iinternal_(const T1 &other);

  /**
   * @brief Multiply to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1> void mul_self_iinternal_(const T1 &other);

  /**
   * @brief Divide to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1> void div_self_iinternal_(const T1 &other);

  /**
   * @brief Modulo to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1> void mod_self_iinternal_(const T1 &other);

  /**
   * @brief Bitwise and to self, using multi-threading
   * @param other other vector to add
   * @returns new array
   */
  template <typename T1>
  void and_bit_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Bitwise and to self, using multi-threading
   * @param other other integer to perform bitwise AND
   * @returns new array
   */
  template <typename T1> void and_bit_self_iinternal_(const T1 &other);

  /**
   * @brief Bitwise OR to self, using multi-threading
   * @param other other vector to perform bitwise OR
   * @returns new array
   */
  template <typename T1>
  void or_bit_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Bitwise OR to self, using multi-threading
   * @param other other vector to perform bitwise OR
   * @returns new array
   */
  template <typename T1> void or_bit_self_iinternal_(const T1 &other);

  /**
   * @brief Bitwise XOR to self, using multi-threading
   * @param other other vector to perform bitwise XOR
   * @returns new array
   */
  template <typename T1>
  void xor_bit_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Bitwise XOR to self, using multi-threading
   * @param other other vector to perform bitwise XOR
   * @returns new array
   */
  template <typename T1> void xor_bit_self_iinternal_(const T1 &other);

  /**
   * @brief Left shift to self, using multi-threading
   * @param other other vector to perform bitwise XOR
   * @returns new array
   */
  template <typename T1>
  void lshft_bit_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Left shift to self, using multi-threading
   * @param other other vector to perform bitwise XOR
   * @returns new array
   */
  template <typename T1> void lshft_bit_self_iinternal_(const T1 &other);

  /**
   * @brief Right shift to self, using multi-threading
   * @param other other vector to perform bitwise XOR
   * @returns new array
   */
  template <typename T1>
  void rshft_bit_self_internal_(const MdStaticArray<T1> &other);

  /**
   * @brief Right shift to self, using multi-threading
   * @param other other vector to perform bitwise XOR
   * @returns new array
   */
  template <typename T1> void rshft_bit_self_iinternal_(const T1 &other);

  /**
   * @brief Compare current array with other
   * @param other array of values to compare
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_eq_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Check if every entry in current array is greater than other
   * @param other array of values to compare
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_g_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Check if every entry in current array is greater or equal to
   * other
   * @param other array of values to compare
   * @returns array of boolean values returning true or false
   */
  template <typename T1, typename c = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::value>
  MdStaticArray<bool> comp_geq_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Check if every entry in current array is less than other
   * @param other array of values to compare
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_l_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Check if every entry in current array is less than or equal to
   * other
   * @param other array of values to compare
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_leq_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Check if every entry in current array is not equal to other
   * @param other array of values to compare
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_neq_internal_(const MdStaticArray<T1> &other) const;

  /**
   * @brief Check if every entry in current array is equal to single value
   * @param other single value
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_eq_iinternal_(const T1 &other) const;

  /**
   * @brief Check if every entry in current array is greater than a single
   * value
   * @param other single value
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_g_iinternal_(const T1 &other) const;

  /**
   * @brief Check if every entry in current array is greater than or equal to
   * a single value
   * @param other single value
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_geq_iinternal_(const T1 &other) const;

  /**
   * @brief Check if every entry in current array is less than a single
   * value
   * @param other single value
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_l_iinternal_(const T1 &other) const;

  /**
   * @brief Check if every entry in current array is less than or equal a
   * single value
   * @param other single value
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_leq_iinternal_(const T1 &other) const;

  /**
   * @brief Check if every entry in current array is not equal to a single
   * value
   * @param other single value
   * @returns array of boolean values returning true or false
   */
  template <typename T1>
  MdStaticArray<bool> comp_neq_iinternal_(const T1 &other) const;

  template <typename T1> inline auto operator+(const T1 &other) const {
    return add_iinternal_(other);
  }

  template <typename T1>
  inline auto operator+(const MdStaticArray<T1> &other) const {
    return add_internal_(other);
  }

  template <typename T1> inline auto operator-(const T1 &other) const {
    return sub_iinternal_(other);
  }

  template <typename T1>
  inline auto operator-(const MdStaticArray<T1> &other) const {
    return sub_internal_(other);
  }

  template <typename T1>
  inline auto operator*(const MdStaticArray<T1> &other) const {
    return mul_internal_(other);
  }

  template <typename T1> inline auto operator*(const T1 &other) const {
    return mul_iinternal_(other);
  }

  template <typename T1> inline auto operator/(const T1 &other) const {
    return div_iinternal_(other);
  }

  template <typename T1>
  inline auto operator/(const MdStaticArray<T1> &other) const {
    return div_internal_(other);
  }

  template <typename T1> inline auto operator%(const T1 &other) const {
    return mod_iinternal_(other);
  }

  template <typename T1>
  inline auto operator%(const MdStaticArray<T1> &other) const {
    return mod_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_int<T1>::value>::type>
  inline auto operator&(const MdStaticArray<T1> &other) const {
    return and_bit_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_int<T1>::value>::type>
  inline auto operator&(const T1 &other) const {
    return and_bit_iinternal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_int<T1>::value>::type>
  inline auto operator|(const MdStaticArray<T1> &other) const {
    return or_bit_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_int<T1>::value>::type>
  inline auto operator|(const T1 &other) const {
    return or_bit_iinternal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_int<T1>::value>::type>
  inline auto operator^(const MdStaticArray<T1> &other) const {
    return xor_bit_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_int<T1>::value>::type>
  inline auto operator^(const T1 &other) const {
    return xor_bit_iinternal_(other);
  }

  template <typename T1>
  inline auto operator<<(const MdStaticArray<T1> &other) const {
    return lshft_bit_internal_(other);
  }

  template <typename T1> inline auto operator<<(const T1 &other) const {
    return lshft_bit_iinternal_(other);
  }

  template <typename T1>
  inline MdStaticArray<T> operator>>(const MdStaticArray<T1> &other) const {
    return rshft_bit_internal_(other);
  }

  template <typename T1>
  inline MdStaticArray<T> operator>>(const T1 &other) const {
    return rshft_bit_iinternal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator+=(const MdStaticArray<T1> &other) {
    add_self_internal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator-=(const MdStaticArray<T1> &other) {
    sub_self_internal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator*=(const MdStaticArray<T1> &other) {
    mul_self_internal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator/=(const MdStaticArray<T1> &other) {
    div_self_internal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator%=(const MdStaticArray<T1> &other) {
    mod_self_internal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator+=(const T1 &other) {
    add_self_iinternal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator-=(const T1 &other) {
    sub_self_iointernal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator*=(const T1 &other) {
    mul_self_iinternal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator/=(const T1 &other) {
    div_self_iinternal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator%=(const T1 &other) {
    mod_self_iinternal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator<<=(const MdStaticArray<T1> &other) {
    lshft_bit_self_internal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator<<=(const T1 &other) {
    lshft_bit_self_iinternal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator>>=(const MdStaticArray<T1> &other) {
    rshft_bit_self_internal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray &operator>>=(const T1 &other) {
    rshft_bit_self_iinternal_(other);
    return *this;
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator==(const MdStaticArray<T1> &other) {
    return comp_eq_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator>(const MdStaticArray<T1> &other) {
    return comp_g_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator>=(const MdStaticArray<T1> &other) {
    return comp_geq_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator<(const MdStaticArray<T1> &other) {
    return comp_l_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator<=(const MdStaticArray<T1> &other) {
    return comp_leq_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator!=(const MdStaticArray<T1> &other) {
    return comp_neq_internal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator==(const T1 &other) {
    return comp_eq_iinternal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator>(const T1 &other) {
    return comp_g_iinternal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator>=(const T1 &other) {
    return comp_geq_iinternal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator<(const T1 &other) {
    return comp_l_iinternal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator<=(const T1 &other) {
    return comp_leq_iinternal_(other);
  }

  template <typename T1, typename = typename std::enable_if<
                             MdTypeInfer::is_arith<T1>::value>::type>
  inline MdStaticArray<bool> operator!=(const T1 &other) {
    return comp_neq_iinternal_(other);
  }

  // To do: create a ArraySlice for multi-dimensional arrays.
  inline ArraySlice<T> operator[](const usize index) const {
    if (index >= shape[0]) {
      throw std::runtime_error("Index out of bounds while accessing index " +
                               std::to_string(index) +
                               " ( >= " + std::to_string(shape[0]) + ")");
    }
    return ArraySlice(*this, index * skip_vec[0]);
  }

  inline usize get_size() const { return size_; }

  friend std::ostream &operator<<(std::ostream &op,
                                  const MdStaticArray<T> &ot) {
    // op << "here " << ot.size << '\n';
    if (ot.get_size() == 1) {
      op << ot.array_[0];
    } else if (ot.get_shape_size() == 1) {
      op << '[';

      for (usize index = 0; index < ot.get_size(); ++index) {
        op << ot.array_[index];
        if (index != ot.get_size() - 1) {
          op << ", ";
        }
      }

      op << ']';
    } else {
      op << '[';

      for (usize index = 0; index < ot.get_shape()[0]; ++index) {
        op << ot[index];
        if (index != ot.get_shape()[0] - 1) {
          op << ",\n";
        }
      }

      op << ']';
    }
    return op;
  }

  template <typename T1, typename T2>
  friend inline auto &operator+=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator-=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator*=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator/=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator%=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator<<=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator>>=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator&=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator|=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto &operator^=(T1 &other, const ArraySlice<T2> &first);

  template <typename T1>
  friend inline auto operator-(const MdStaticArray<T1> ndarray);

  template <typename T1>
  friend inline MdStaticArray<T1> operator-(const ArraySlice<T1> &first);
};

#include "./md_static_array_op.hpp"

template <typename T> void MdStaticArray<T>::set_thread_count(const u8 value) {
  s_thread_count = value;
}

template <typename T>
void MdStaticArray<T>::set_threshold_size(const usize value) {
  s_threshold_size = value;
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator+(const T1 &other, const MdStaticArray<T2> &first) {
  return first + other;
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator-(const T1 &other, const MdStaticArray<T2> &first) {
  return first.sub_iointernal_(other);
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator*(const T1 &other, const MdStaticArray<T2> &first) {
  return first * other;
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator/(const T1 &other, const MdStaticArray<T2> &first) {
  return first.div_iointernal_(other);
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator%(const T1 &other, const MdStaticArray<T2> &first) {
  return first.mod_iointernal_(other);
}

template <typename T1, typename T2,
          typename std::enable_if<MdTypeInfer::is_native<T2>::value ||
                                  MdTypeInfer::is_complex<T2>::value>::value>
inline auto operator==(const T1 &other, const MdStaticArray<T2> &first) {
  return first.comp_eq_iinternal_(other);
}

template <typename T1, typename T2,
          typename std::enable_if<MdTypeInfer::is_native<T2>::value ||
                                  MdTypeInfer::is_complex<T2>::value>::value>
inline auto operator!=(const T1 &other, const MdStaticArray<T2> &first) {
  return first.comp_neq_iinternal_(other);
}

template <typename T1, typename T2>
inline auto operator>(const T1 &other, const MdStaticArray<T2> &first) {
  return first.comp_g_iinternal_(other);
}

template <typename T1, typename T2>
inline auto operator<(const T1 &other, const MdStaticArray<T2> &first) {
  return first.comp_l_iinternal_(other);
}

template <typename T1, typename T2>
inline auto operator<=(const T1 &other, const MdStaticArray<T2> &first) {
  return first.comp_leq_iinternal_(other);
}

template <typename T1, typename T2>
inline auto operator>=(const T1 &other, const MdStaticArray<T2> &first) {
  return first.comp_geq_iinternal_(other);
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator&(const T1 &other, const MdStaticArray<T2> &first) {
  return first.and_bit_iinternal_(other);
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator|(const T1 &other, const MdStaticArray<T2> &first) {
  return first.or_bit_iinternal_(other);
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator^(const T1 &other, const MdStaticArray<T2> &first) {
  return first.xor_bit_iinternal_(other);
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator<<(const T1 &other, const MdStaticArray<T2> &first) {
  return first.lshft_bit_iointernal_(other);
}

template <typename T1, typename T2,
          class = typename std::enable_if<std::is_arithmetic<T1>::value>::type>
inline auto operator>>(const T1 &other, const MdStaticArray<T2> &first) {
  return first.rshft_bit_iointernal_(other);
}

template <typename T>
inline MdStaticArray<T> operator-(const MdStaticArray<T> &first) {
  return first.ng_internal_();
}

#include "./md_static_axis_reference.hpp"
#include "./md_static_reference.hpp"

template <typename T>
MdStaticArray<T> &MdStaticArray<T>::operator=(const ArraySlice<T> &other) {
  init_array(other.size);
  init_shape(&other.array_reference_->shape[other.shp_offset],
             other.array_reference_->shp_size - other.shp_offset);

  for (usize index = 0; index < size_; ++index) {
    array_[index] = other.array_reference_->array_[other.offset + index];
  }

  return *this;
}

template <typename T>
MdStaticArray<T>::MdStaticArray(const ArraySlice<T> &other)
    : shape(nullptr), skip_vec(nullptr) {
  size_ = other.size;
  init_array(size_);
  init_shape(&other.array_reference_->shape[other.shp_offset],
             other.array_reference_->shp_size - other.shp_offset);

  for (usize index = 0; index < size_; ++index) {
    array_[index] = other.array_reference_->array_[other.offset + index];
  }
}

template <typename T1, typename T2>
inline T1 &operator+=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator += on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }
  other += first.array_reference_->array_[first.offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator-=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator -= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other -= first.array_reference_->array_[first.shp_offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator*=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator *= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other *= first.array_reference_->array_[first.offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator/=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator /= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other /= first.array_reference_->array_[first.offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator%=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator %= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other %= first.array_reference_->array_[first.offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator<<=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator <<= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other <<= first.array_reference_->array_[first.offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator>>=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator >>= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other >>= first.array_reference_->array_[first.offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator&=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator &= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other &= first.array_reference_->array_[first.offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator|=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator |= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other |= first.array_reference_->array_[first.offset];
  return other;
}

template <typename T1, typename T2>
inline T1 &operator^=(T1 &other, const ArraySlice<T2> &first) {
  if (first.get_size() > 1) {
    throw std::runtime_error(
        "Operator ^= on single element requires size to be 1, found "
        "size: " +
        std::to_string(first.get_size()));
  }

  other |= first.array_reference_->array_[first.offset];
  return other;
}

template <typename T>
__always_inline Axis<T> MdStaticArray<T>::get_axis_reference(const usize axis) {
  return Axis<T>(*this, axis);
}

template <typename T>
__always_inline Axis<T>
MdStaticArray<T>::get_nth_axis_reference(const usize axis, const usize n) {
  return Axis<T>(*this, axis, n);
}

template <typename T> using Array = MdStaticArray<T>;

#endif
