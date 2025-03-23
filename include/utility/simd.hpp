#pragma once
#ifndef _SIMD_HPP_
#define _SIMD_HPP_

#include <immintrin.h>

#include <initializer_list>

#include "../md_complex/md_complex.hpp"
#include "./math.hpp"

using f32x8 = __m256;

using i32x8 = __m256i;
using i64x4 = __m256i;
using i16x16 = __m256i;
using i8x32 = __m256i;

using u32x8 = __m256i;
using u64x4 = __m256i;
using u16x16 = __m256i;
using u8x32 = __m256i;

using c64x2 = __m256d;
using c32x4 = __m256;

using f32x4 = __m128;
using f64x2 = __m128d;

using i32x4 = __m128i;
using i64x2 = __m128i;
using i16x8 = __m128i;
using i8x16 = __m128i;

using u32x4 = __m128i;
using u64x2 = __m128i;
using u16x8 = __m128i;
using u8x16 = __m128i;

using c64x1 = __m128d;
using c32x2 = __m128;

namespace F32x8 {
__always_inline f32x8 add(f32x8 first, f32x8 second) {
  return _mm256_add_ps(first, second);
}

__always_inline f32x8 sub(f32x8 first, f32x8 second) {
  return _mm256_sub_ps(first, second);
}

__always_inline f32x8 mul(f32x8 first, f32x8 second) {
  return _mm256_mul_ps(first, second);
}

__always_inline f32x8 div(f32x8 first, f32x8 second) {
  return _mm256_div_ps(first, second);
}

__always_inline f32x8 fmadd(f32x8 a, f32x8 b, f32x8 c) {
  return _mm256_fmadd_ps(a, b, c);
}

__always_inline f32x8 uni(f32 val) { return _mm256_set1_ps(val); }

__always_inline f32x8 set(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g,
                          f32 h) {
  return _mm256_set_ps(a, b, c, d, e, f, g, h);
}

__always_inline f32x8 zero() { return _mm256_setzero_ps(); }

__always_inline f32x8 fromptr(f32 *val) { return _mm256_loadu_ps(val); }

__always_inline void storeptr(f32 *val, f32x8 vec) {
  _mm256_storeu_ps(val, vec);
}

__always_inline f32 reduce_sum(f32x8 val) {
  f32 ans[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  storeptr(ans, val);
  return ans[0] + ans[1] + ans[2] + ans[3] + ans[4] + ans[5] + ans[6] + ans[7];
}

} // namespace F32x8

struct f64x4 {
private:
  __m256d internal;

public:
  f64x4() {}
  f64x4(__m256d set) : internal(set) {}
  f64x4(f64 *set) : internal(_mm256_loadu_pd(set)) {}
  f64x4(f64 set) : internal(_mm256_set1_pd(set)) {}

  f64x4 &operator=(f64 set) {
    internal = _mm256_set1_pd(set);
    return *this;
  }

  __always_inline f64x4 operator+(f64x4 other) {
    return _mm256_add_pd(internal, other.internal);
  }

  __always_inline __m256d get() { return internal; }

  static __always_inline f64x4 def() { return _mm256_set1_pd(0.0); }

  __always_inline void copy_to(f64 *ptr) { _mm256_storeu_pd(ptr, internal); }

  __always_inline f64x4 operator+(f64 other) {
    return _mm256_add_pd(internal, _mm256_set1_pd(other));
  }

  __always_inline f64x4 &operator+=(f64x4 other) {
    this->internal = _mm256_add_pd(internal, other.internal);
    return *this;
  }

  __always_inline f64x4 &operator+=(f64 other) {
    this->internal = _mm256_add_pd(internal, _mm256_set1_pd(other));
    return *this;
  }

  __always_inline f64x4 operator-(f64x4 other) {
    return _mm256_sub_pd(internal, other.internal);
  }

  __always_inline f64x4 operator-(f64 other) {
    return _mm256_sub_pd(internal, _mm256_set1_pd(other));
  }

  __always_inline f64x4 &operator-=(f64x4 other) {
    this->internal = _mm256_sub_pd(internal, other.internal);
    return *this;
  }

  __always_inline f64x4 &operator-=(f64 other) {
    this->internal = _mm256_sub_pd(internal, _mm256_set1_pd(other));
    return *this;
  }

  __always_inline f64x4 operator*(f64x4 other) {
    return _mm256_mul_pd(internal, other.internal);
  }

  __always_inline f64x4 operator*(f64 other) {
    return _mm256_mul_pd(internal, _mm256_set1_pd(other));
  }

  __always_inline f64x4 &operator*=(f64x4 other) {
    this->internal = _mm256_mul_pd(internal, other.internal);
    return *this;
  }

  __always_inline f64x4 &operator*=(f64 other) {
    this->internal = _mm256_mul_pd(internal, _mm256_set1_pd(other));
    return *this;
  }

  __always_inline f64x4 fmadd(f64x4 mul_by, f64x4 add_by) {
    return _mm256_fmadd_pd(internal, mul_by.internal, add_by.internal);
  }

  __always_inline f64 reduce_sum() {
    f64 ans[4] = {0, 0, 0, 0};
    copy_to(ans);
    return ans[0] + ans[1] + ans[2] + ans[3];
  }
};

namespace F32x4 {
__always_inline f32x4 add(f32x4 first, f32x4 second) {
  return _mm_add_ps(first, second);
}

__always_inline f32x4 sub(f32x4 first, f32x4 second) {
  return _mm_sub_ps(first, second);
}

__always_inline f32x4 mul(f32x4 first, f32x4 second) {
  return _mm_mul_ps(first, second);
}

__always_inline f32x4 div(f32x4 first, f32x4 second) {
  return _mm_div_ps(first, second);
}

__always_inline f32x4 fmadd(f32x4 a, f32x4 b, f32x4 c) {
  return _mm_fmadd_ps(a, b, c);
}

__always_inline f32x4 uni(f64 val) { return _mm_set1_ps(val); }

__always_inline f32x4 set(f32 a, f32 b, f32 c, f32 d) {
  return _mm_set_ps(a, b, c, d);
}

__always_inline f32x4 zero() { return _mm_setzero_ps(); }

__always_inline f32x4 fromptr(f32 *val) { return _mm_loadu_ps(val); }

__always_inline void storeptr(f32 *val, f32x4 vec) { _mm_storeu_ps(val, vec); }

__always_inline f32 reduce_sum(f32x4 val) {
  f32 ans[4] = {0, 0, 0, 0};
  storeptr(ans, val);
  return ans[0] + ans[1] + ans[2] + ans[3];
}
} // namespace F32x4

namespace F64x2 {
__always_inline f64x2 add(f64x2 first, f64x2 second) {
  return _mm_add_pd(first, second);
}

__always_inline f64x2 sub(f64x2 first, f64x2 second) {
  return _mm_sub_pd(first, second);
}

__always_inline f64x2 mul(f64x2 first, f64x2 second) {
  return _mm_mul_pd(first, second);
}

__always_inline f64x2 div(f64x2 first, f64x2 second) {
  return _mm_div_pd(first, second);
}

__always_inline f64x2 fmadd(f64x2 a, f64x2 b, f64x2 c) {
  return _mm_fmadd_pd(a, b, c);
}

__always_inline f64x2 uni(f64 val) { return _mm_set1_pd(val); }

__always_inline f64x2 set(f64 a, f64 b) { return _mm_set_pd(a, b); }

__always_inline f64x2 zero() { return _mm_setzero_pd(); }

__always_inline f64x2 fromptr(f64 *val) { return _mm_loadu_pd(val); }

__always_inline void storeptr(f64 *val, f64x2 vec) { _mm_storeu_pd(val, vec); }

__always_inline f64 reduce_sum(f64x2 val) {
  f64 ans[2] = {0, 0};
  storeptr(ans, val);
  return ans[0] + ans[1];
}
} // namespace F64x2

namespace C32x4 {
__always_inline c32x4 add(c32x4 first, c32x4 second) {
  return _mm256_add_ps(first, second);
}

__always_inline c32x4 sub(c32x4 first, c32x4 second) {
  return _mm256_sub_ps(first, second);
}

__always_inline void storeptr(c32 *val, c32x4 vec) {
  _mm256_storeu_ps((f32 *)val, vec);
}

__always_inline c32x4 set(c32 a, c32 b, c32 c, c32 d) {
  return _mm256_set_ps(a.real, a.img, b.real, b.img, c.real, c.img, d.real,
                       d.img);
}

__always_inline c32x4 mul(c32x4 first, c32x4 second) {
  c32 ans0[4] = {0, 0, 0, 0};
  c32 ans1[4] = {0, 0, 0, 0};
  storeptr(ans0, first);
  storeptr(ans1, second);
  return set(ans0[0] * ans1[0], ans1[1] * ans0[1], ans1[2] * ans0[2],
             ans1[3] * ans0[3]);
  // c32x4 real = _mm256_mul_ps(first, second);
  // c32x4 img = _mm256_mul_ps(first, _mm256_permute_ps(second, 0b10110001));
  // c32x4 realf = _mm256_hsub_ps(real, real);
  // c32x4 imgf = _mm256_hadd_ps(img, img);
  // return _mm256_blend_ps(_mm256_permute_ps(realf, 0b01010000),
  //                        _mm256_permute_ps(imgf, 0b01010000), 0b10101010);
}

__always_inline c32x4 div(c32x4 first, c32x4 second) {
  return _mm256_div_ps(first, second);
}

__always_inline c32x4 fmadd(c32x4 a, c32x4 b, c32x4 c) {
  return add(mul(a, b), c);
}

__always_inline c32x4 uni(c32 val) {
  return _mm256_set_ps(val.real, val.img, val.real, val.img, val.real, val.img,
                       val.real, val.img);
}

__always_inline c32x4 zero() { return _mm256_setzero_ps(); }

__always_inline c32x4 fromptr(c32 *val) { return _mm256_loadu_ps((f32 *)val); }

__always_inline c32 reduce_sum(c32x4 val) {
  c32 ans[4] = {0, 0, 0, 0};
  storeptr(ans, val);
  return ans[0] + ans[1] + ans[2] + ans[3];
}
} // namespace C32x4

namespace C64x2 {
__always_inline c64x2 add(c64x2 first, c64x2 second) {
  return _mm256_add_pd(first, second);
}

__always_inline c64x2 sub(f64x4 first, c64x2 second) {
  return _mm256_sub_pd(first.get(), second);
}

__always_inline void storeptr(c64 *val, c64x2 vec) {
  _mm256_storeu_pd((f64 *)val, vec);
}

__always_inline c64x2 set(c64 a, c64 b) {
  return _mm256_set_pd(b.img, b.real, a.img, a.real);
}

__always_inline c64x2 mul(c64x2 first, c64x2 second) {
  c64x2 real = _mm256_mul_pd(first, second);
  c64x2 img = _mm256_mul_pd(first, _mm256_permute_pd(second, 0b0101));
  c64x2 realf = _mm256_hsub_pd(real, real);
  c64x2 imgf = _mm256_hadd_pd(img, img);
  return _mm256_blend_pd(realf, imgf, 0b1010);
}

__always_inline c64x2 mul_alt(c64x2 first, c64x2 second) {
  c64 ans0[2] = {0, 0}, ans1[2] = {0, 0};
  storeptr(ans0, first);
  storeptr(ans1, second);
  return set(ans0[0] * ans1[0], ans1[1] * ans0[1]);
}

__always_inline c64x2 scal_mul(c64x2 first, f64x4 second) {
  return _mm256_mul_pd(first, second.get());
}

__always_inline c64x2 div(c64x2 first, c64x2 second) {
  return _mm256_div_pd(first, second);
}

__always_inline c64x2 fmadd(c64x2 a, c64x2 b, c64x2 c) {
  return add(mul_alt(a, b), c);
}

__always_inline c64x2 uni(c64 val) {
  return _mm256_set_pd(val.img, val.real, val.img, val.real);
}

__always_inline c64x2 zero() { return _mm256_setzero_pd(); }

__always_inline c64x2 fromptr(c64 *val) { return _mm256_loadu_pd((f64 *)val); }

__always_inline c64x2 perm2x128(c64x2 a, c64x2 b, i8 ctrl) {
  return _mm256_permute2f128_pd(a, b, ctrl);
}

__always_inline c64 reduce_sum(c64x2 val) {
  c64 ans[2] = {0, 0};
  storeptr(ans, val);
  return ans[0] + ans[1];
}
} // namespace C64x2

namespace I64x4 {
__always_inline i64x4 add(i64x4 first, i64x4 second) {
  return _mm256_add_epi64(first, second);
}

__always_inline i64x4 sub(i64x4 first, i64x4 second) {
  return _mm256_sub_epi64(first, second);
}

__always_inline i64x4 uni(i64 val) { return _mm256_set1_epi64x(val); }

__always_inline i64x4 set(i64 a, i64 b, i64 c, i64 d) {
  return _mm256_set_epi64x(a, b, c, d);
}

__always_inline i64x4 zero() { return _mm256_setzero_si256(); }

__always_inline i64x4 fromptr(i64 *val) { return _mm256_loadu_epi64(val); }

__always_inline void storeptr(i64 *val, i64x4 vec) {
  _mm256_storeu_epi64(val, vec);
}

__always_inline i64 reduce_sum(i64x4 val) {
  i64 ans[8] = {0, 0, 0, 0};
  storeptr(ans, val);
  return ans[0] + ans[1] + ans[2] + ans[3];
}
} // namespace I64x4

namespace I32x8 {
__always_inline i32x8 add(i32x8 first, i32x8 second) {
  return _mm256_add_epi32(first, second);
}

__always_inline i32x8 sub(i32x8 first, i32x8 second) {
  return _mm256_sub_epi32(first, second);
}

__always_inline i64x4 mul(i32x8 first, i32x8 second) {
  return _mm256_mul_epi32(first, second);
}

__always_inline i32x8 fmadd(i32x8 a, i32x8 b, i32x8 c) {
  return _mm256_add_epi32(mul(a, b), c);
}

__always_inline i32x8 uni(i32 val) { return _mm256_set1_epi32(val); }

__always_inline i32x8 set(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g,
                          i32 h) {
  return _mm256_set_epi32(a, b, c, d, e, f, g, h);
}

__always_inline i32x8 zero() { return _mm256_setzero_si256(); }

__always_inline i32x8 fromptr(i32 *val) { return _mm256_loadu_epi32(val); }

__always_inline void storeptr(i32 *val, i32x8 vec) {
  _mm256_storeu_epi32(val, vec);
}

__always_inline i32 reduce_sum(i32x8 val) {
  i32 ans[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  storeptr(ans, val);
  return ans[0] + ans[1] + ans[2] + ans[3] + ans[4] + ans[5] + ans[6] + ans[7];
}
} // namespace I32x8

#endif
