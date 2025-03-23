#pragma once
#ifndef _ALLOC_HPP_
#define _ALLOC_HPP_

#include <ostream>

#include "../md_static/md_static_array/md_type_inference.hpp"

template <typename T>
inline T *aligned_allocate(const usize alignment, const usize size) {
#ifdef _WIN32
  return static_cast<T *>(_aligned_malloc(size * sizeof(T), alignment));
#else
  return static_cast<T *>(aligned_alloc(alignment, size * sizeof(T)));
#endif
}

inline void aligned_free(void *ptr) {
#ifdef _WIN32
  _aligned_free(ptr);
#else
  free(ptr);
#endif
}

#endif