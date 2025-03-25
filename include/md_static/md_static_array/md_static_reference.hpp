#pragma once
#ifndef _MD_ST_REFERENCE_HPP_
#define _MD_ST_REFERENCE_HPP_

#include "../../md_complex/md_complex.hpp"
#include "./md_static_array.hpp"
#include "./md_type_inference.hpp"

template <typename T> class ArraySlice {
  template <typename T1> friend class MdStaticArray;
  template <typename T1> friend class ArraySlice;

  friend struct MdArrayUtility;
  friend struct MdLinearAlgebra;
  friend struct FFT;
  friend struct Signal;

  void *operator new(usize size);
  const MdStaticArray<T> *array_reference_;
  usize size;
  usize offset;
  u16 shp_offset;

public:
  ArraySlice() {}

  ArraySlice(const MdStaticArray<T> &other, const usize offst) {
    array_reference_ = &other;
    offset = offst;
    size = other.get_size() / other.shape[0];
    shp_offset = 1;
  }

  ArraySlice(const ArraySlice<T> &other, const usize offst) {
    array_reference_ = other.array_reference_;
    offset = offst;
    shp_offset = other.shp_offset + 1;
    size = other.size / (other.array_reference_->shape[shp_offset - 1]);
  }

  ArraySlice(const ArraySlice<T> &other) {
    array_reference_ = other.array_reference_;
    offset = other.offset;
    shp_offset = other.shp_offset;
    size = other.size;
  }

  /**
   * @brief Improve copy operator: support broadcasting
   */
  ArraySlice &operator=(const ArraySlice<T> &other) {
    array_reference_ = other.array_reference_;
    offset = other.offset;
    shp_offset = other.shp_offset;
    size = other.size;

    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator=(const MdStaticArray<T1> &value) {
    if (value.get_size() != size) {
      throw std::runtime_error("Assignment error between ArraySlice of size " +
                               std::to_string(size) +
                               " and MdStaticArray of size " +
                               std::to_string(value.get_size()));
    }
    // assert shape sizes are equal too

    for (usize i = 0; i < size; ++i) {
      array_reference_->array_[offset + i] = value.array_[i];
    }

    return *this;
  }

  template <typename T1> ArraySlice &operator=(const T1 other) {
    if (size == 1) {
      array_reference_->array_[offset] = other;
      return *this;
    }

    for (usize i = 0; i < size; ++i) {
      array_reference_->array_[offset + i] = other;
    }
    return *this;
  }

  inline ArraySlice operator[](const usize index) const {
    if (shp_offset >= array_reference_->shp_size) {
      char c[100];
      sprintf(c, "Expected dimension %d, found %d", shp_offset + 1,
              array_reference_->shp_size);
      throw std::runtime_error(c);
    }
    if (index >= array_reference_->shape[shp_offset]) {
      throw std::runtime_error(
          "Index out of bounds while accessing index " + std::to_string(index) +
          " ( >= " + std::to_string(array_reference_->shape[shp_offset]) + ")");
    }

    return ArraySlice<T>(
        *this, offset + index * array_reference_->skip_vec[shp_offset]);
  }

  // To do: efficient printing
  friend std::ostream &operator<<(std::ostream &op, const ArraySlice<T> &ot) {
    // op << "here " << ot.size << '\n';
    if (ot.size == 1) {
      op << ot.array_reference_->array_[ot.offset];
    } else if (ot.shp_offset + 1 == ot.array_reference_->shp_size) {
      op << '[';
      for (usize index = 0, stride = ot.offset;
           index < ot.array_reference_->shape[ot.shp_offset];
           ++index, stride += ot.array_reference_->skip_vec[ot.shp_offset]) {
        op << ot.array_reference_->array_[stride];
        if (index != ot.size - 1) {
          op << ", ";
        }
      }
      op << ']';
    } else {
      op << '[';
      for (usize index = 0; index < ot.array_reference_->shape[ot.shp_offset];
           ++index) {
        op << ot[index];
        if (index != ot.array_reference_->shape[ot.shp_offset] - 1) {
          op << ",\n";
        }
      }
      op << "]";
    }
    return op;
  }

  inline operator T() const {
    if (size > 1) {
      throw std::runtime_error("Error casting element. Size found " +
                               std::to_string(size));
    }
    return array_reference_->array_[offset];
  }

  /**
   * @brief Return constant value
   */
  template <typename T1> inline operator T1() const {
    if (size > 1) {
      throw std::runtime_error("Error casting element. Size found " +
                               std::to_string(size));
    }
    return array_reference_->array_[offset];
  }

  inline operator MdStaticArray<T>() const {
    return MdStaticArray<T>(*array_reference_);
  }

  /**
   * @brief Return MdStaticArray value
   */
  template <typename T1> inline operator MdStaticArray<T1>() const {
    return MdStaticArray<T1>(*array_reference_);
  }

  template <typename T1>
  inline auto operator+(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) + other;
  }

  inline auto operator+(const ArraySlice<T> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) +
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator+(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) + other;
  }

  template <typename T1>
  inline auto operator-(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) - other;
  }

  inline auto operator-(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] -
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset) -
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator-(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) - other;
  }

  template <typename T1>
  inline auto operator*(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) * other;
  }

  inline auto operator*(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] *
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset) *
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator*(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) * other;
  }

  template <typename T1>
  inline auto operator/(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) / other;
  }

  inline auto operator/(const ArraySlice<T> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) /
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator/(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) / other;
  }

  template <typename T1>
  inline auto operator%(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) % other;
  }

  inline auto operator%(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] /
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset) %
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator%(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) % other;
  }

  template <typename T1>
  inline auto operator&(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) & other;
  }

  inline auto operator&(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] &
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset) &
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator&(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) & other;
  }

  template <typename T1>
  inline auto operator|(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) | other;
  }

  inline auto operator|(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] |
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset) |
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator|(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) | other;
  }

  template <typename T1>
  inline auto operator^(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) ^ other;
  }

  inline auto operator^(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] ^
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset) ^
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator^(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) ^ other;
  }

  template <typename T1>
  inline auto operator<<(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) << other;
  }

  inline auto operator<<(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1,
                           array_reference_->array_[offset]
                               << other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset)
           << MdStaticArray(*other.array_reference_, other.offset,
                            other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator<<(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) << other;
  }

  template <typename T1>
  inline auto operator>>(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) >> other;
  }

  inline auto operator>>(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] >>
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset) >>
           MdStaticArray(*other.array_reference_, other.offset,
                         other.shp_offset);
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline auto operator>>(const T1 &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset) >> other;
  }

  template <typename T1>
  inline auto operator!=(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset)
        .comp_neq_internal_(other);
  }

  inline auto operator!=(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] !=
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset)
        .comp_neq_internal_(MdStaticArray(*other.array_reference_, other.offset,
                                          other.shp_offset));
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline MdStaticArray<bool> operator!=(const T1 &other) const {
    if (size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] != other);
    }
    return MdStaticArray(*array_reference_, offset, shp_offset)
        .comp_neq_iinternal_(other);
  }

  template <typename T1>
  inline auto operator==(const MdStaticArray<T1> &other) const {
    return MdStaticArray(*array_reference_, offset, shp_offset)
        .comp_eq_internal_(other);
  }

  inline MdStaticArray<bool> operator==(const ArraySlice<T> &other) const {
    if (size == 1 && other.size == 1) {
      return MdStaticArray(1, array_reference_->array_[offset] ==
                                  other.array_reference_->array_[other.offset]);
    }
    return MdStaticArray<T>(*array_reference_, offset, shp_offset)
        .comp_eq_internal_(MdStaticArray(*other.array_reference_, other.offset,
                                         other.shp_offset));
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline MdStaticArray<bool> operator==(const T1 &other) const {
    if (size == 1) {
      return MdStaticArray<bool>(1, array_reference_->array_[offset] == other);
    }
    return MdStaticArray<T>(*array_reference_, offset, shp_offset)
        .comp_eq_iinternal_(other);
  }

  template <typename T1>
  inline ArraySlice &operator+=(MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .add_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator+=(ArraySlice &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .add_self_internal_(MdStaticArray(*other.array_reference_, other.offset,
                                          other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator+=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .add_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator-=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .sub_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator-=(ArraySlice &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .sub_self_internal_(MdStaticArray(*other.array_reference_, other.offset,
                                          other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator-=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .sub_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator*=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .mul_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator*=(const ArraySlice<T> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .mul_self_internal_(MdStaticArray(*other.array_reference_, other.offset,
                                          other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator*=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .mul_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator/=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .div_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator/=(const ArraySlice<T> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .div_self_internal_(MdStaticArray(*other.array_reference_, other.offset,
                                          other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator/=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .div_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator%=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .mod_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator%=(const ArraySlice<T> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .mod_self_internal_(MdStaticArray(*other.array_reference_, other.offset,
                                          other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator%=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .mod_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator&=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .and_bit_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator&=(const ArraySlice<T> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .and_bit_self_internal_(MdStaticArray(*other.array_reference_,
                                              other.offset, other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator&=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .and_bit_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator|=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .or_bit_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator|=(const ArraySlice<T> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .or_bit_self_internal_(MdStaticArray(*other.array_reference_,
                                             other.offset, other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator|=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .or_bit_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator^=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .xor_bit_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator^=(const ArraySlice<T> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .xor_bit_self_internal_(MdStaticArray(*other.array_reference_,
                                              other.offset, other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator^=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .xor_bit_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator<<=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .lshft_bit_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator<<=(const ArraySlice<T> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .lshft_bit_self_internal_(MdStaticArray(
            *other.array_reference_, other.offset, other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator<<=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .lshft_bit_self_iinternal_(other);
    return *this;
  }

  template <typename T1>
  inline ArraySlice &operator>>=(const MdStaticArray<T1> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .rshft_bit_self_internal_(other);
    return *this;
  }

  inline ArraySlice &operator>>=(const ArraySlice<T> &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .rshft_bit_self_internal_(MdStaticArray(
            *other.array_reference_, other.offset, other.shp_offset));
    return *this;
  }

  template <typename T1, class = typename std::enable_if<
                             !std::is_same<T1, ArraySlice<T>>::value>::type>
  inline ArraySlice &operator>>=(const T1 &other) {
    MdStaticArray(*array_reference_, offset, shp_offset)
        .rshft_bit_self_iinternal_(other);
    return *this;
  }

  constexpr inline usize get_size() const { return size; }

  ~ArraySlice() { array_reference_ = nullptr; }

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

  ///////////////////////////////////////////////////////////////////////////

  template <typename T1, typename T2>
  friend inline auto operator+(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto operator-(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1>
  friend inline MdStaticArray<T1> operator-(const ArraySlice<T1> &first);

  template <typename T1, typename T2>
  friend inline auto operator*(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto operator/(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto operator%(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto operator<<(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto operator>>(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto operator&(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto operator|(const T1 &other, const ArraySlice<T2> &first);

  template <typename T1, typename T2>
  friend inline auto operator^(const T1 &other, const ArraySlice<T2> &first);
};

template <typename T1, typename T2>
inline auto operator+(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray<T2>(*first.array_reference_, first.offset,
                           first.shp_offset) +
         other;
}

template <typename T1, typename T2>
inline auto operator-(const T1 &other, const ArraySlice<T2> &first) {
  return -other + MdStaticArray<T2>(*first.array_reference_, first.offset,
                                    first.shp_offset);
}

template <typename T1, typename T2>
inline auto operator*(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray<T2>(*first.array_reference_, first.offset,
                           first.shp_offset)
      .mul_iinternal_(other);
}

template <typename T1, typename T2>
inline auto operator/(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray(*first.array_reference_, first.offset, first.shp_offset)
      .div_iointernal_(other);
}

template <typename T1, typename T2>
inline auto operator%(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray<T2>(*first.array_reference_, first.offset,
                           first.shp_offset)
      .mod_iointernal_(other);
}

template <typename T1, typename T2>
inline auto operator&(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray<T2>(*first.array_reference_, first.offset,
                           first.shp_offset)
      .and_bit_iinternal_(other);
}

template <typename T1, typename T2>
inline auto operator|(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray<T2>(*first.array_reference_, first.offset,
                           first.shp_offset)
      .or_bit_iinternal_(other);
}

template <typename T1, typename T2>
inline auto operator^(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray<T2>(*first.array_reference_, first.offset,
                           first.shp_offset)
      .xor_bit_iinternal_(other);
}

template <typename T1, typename T2>
inline auto operator<<(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray<T2>(*first.array_reference_, first.offset,
                           first.shp_offset)
      .lshft_bit_iointernal_(other);
}

template <typename T1, typename T2>
inline auto operator>>(const T1 &other, const ArraySlice<T2> &first) {
  return MdStaticArray<T2>(*first.array_reference_, first.offset,
                           first.shp_offset)
      .rshft_bit_iointernal_(other);
}

template <typename T>
inline MdStaticArray<T> operator-(const ArraySlice<T> &first) {
  return MdStaticArray<T>(*first.array_reference_, first.offset,
                          first.shp_offset)
      .ng_internal_();
}

#endif
