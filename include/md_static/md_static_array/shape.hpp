#pragma once
#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_

#include <stdlib.h>

#include <ostream>
#include <vector>

#include "./md_type_inference.hpp"

/**
 * @brief Shape struct
 */
struct Shape {
  usize *shape;       /// Actual shape array
  usize *skip_vec;    /// Stride vector
  usize shape_size;   /// Size of shape array
  usize overall_size; /// Overall size of the master array

  constexpr Shape()
      : shape(nullptr), skip_vec(nullptr), shape_size(0), overall_size(0) {}

  Shape(usize size)
      : shape(nullptr), skip_vec(nullptr), shape_size(1), overall_size(0) {
    shape = static_cast<usize *>(malloc(sizeof(usize)));
    skip_vec = static_cast<usize *>(malloc(sizeof(usize)));
    shape[0] = size;
    skip_vec[0] = 1;
    overall_size = 1;
  }

  constexpr Shape(const std::vector<usize> &shp)
      : shape(nullptr), skip_vec(nullptr), shape_size(shp.size()),
        overall_size(0) {
    usize size = shp.size();
    shape = static_cast<usize *>(malloc(shape_size * sizeof(usize)));
    skip_vec = static_cast<usize *>(malloc(shape_size * sizeof(usize)));
    for (const i32 axis_size : shp) {
      overall_size *= axis_size;
    }

    shape[shape_size - 1] = shp[shape_size - 1];
    skip_vec[shape_size - 1] = 1;
    for (int16_t i = shape_size - 2; i >= 0; --i) {
      shape[i] = shp[i];
      skip_vec[i] = shp[i + 1] * skip_vec[i + 1];
    }
  }

  const usize operator[](const usize index) const { return shape[index]; }

  friend std::ostream &operator<<(std::ostream &op, const Shape &shp) {
    op << '(';
    for (usize index = 0; index < shp.shape_size; ++index) {
      op << shp.shape[index];
      if (index < shp.shape_size - 1) {
        op << ", ";
      }
    }
    op << ')';
    return op;
  }
};

#endif
