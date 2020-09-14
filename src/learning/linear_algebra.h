/*
 *  Winter is a UCI chess engine.
 *
 *  Copyright (C) 2016 Jonas Kuratli, Jonathan Maurer, Jonathan Rosenthal
 *  Copyright (C) 2017-2019 Jonathan Rosenthal
 *
 *  Winter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Winter is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * linear_algebra.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Jonathan Rosenthal
 */

#ifndef LEARNING_LINEAR_ALGEBRA_H_
#define LEARNING_LINEAR_ALGEBRA_H_

#include "../general/types.h"
#include <cmath>
#include <iostream>

template <typename type, size_t length>
struct Vec {
  Vec() {}

  Vec(type val) {
    for (size_t i = 0; i < length; ++i) {
      values[i] = val;
    }
  }

  Vec(const Vec<type, length/2> &vec_to_crelu) {
    const size_t size = vec_to_crelu.size();
    for (size_t i = 0; i < size; ++i) {
      values[i] = std::max(vec_to_crelu[i], static_cast<type>(0));
      values[i+size] = std::max(-vec_to_crelu[i], static_cast<type>(0));
    }
  }

  type sum() const {
    type t = 0;
    for (size_t i = 0; i < length; ++i) {
      t += values[i];
    }
    return t;
  }

  type normL1() const {
    type res = 0;
    for (size_t i = 0; i < length; ++i) {
      res += std::abs(values[i]);
    }
    return res;
  }

  type normL2() const {
    type res = 0;
    for (size_t i = 0; i < length; ++i) {
      res += values[i] * values[i];
    }
    return std::sqrt(res);
  }

  inline size_t size() const {
    return length;
  }

  void print() const {
    std::cout << "(" << values[0];
    for (size_t i = 1; i < length; ++i) {
      std::cout << "," << values[i];
    }
    std::cout << ")";
  }
  Vec<type, length>& operator=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] = rhs.values[i];
    }
    return *this;
  }

  inline Vec<type, length>& operator+=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i < length; ++i) {
      values[i] += rhs[i];
    }
    return *this;
  }
  
  inline Vec<type, length>& relu() {
    for (size_t i = 0; i < length; ++i) {
      values[i] = std::max(values[i], static_cast<type>(0));
    }
    return *this;
  }

  /// Non-standard PReLU activation assumes alpha <= 1
  inline Vec<type, length>& ns_prelu(const Vec<type, length> &alpha) {
    for (size_t i = 0; i < length; ++i) {
      values[i] = std::max(values[i], values[i] * alpha[i]);
    }
    return *this;
  }

  /// Non-standard PReLU activation assumes alpha <= 1
  inline Vec<type, length>& ns_prelu(const std::array<type, length> &alpha) {
    for (size_t i = 0; i < length; ++i) {
      values[i] = std::max(values[i], values[i] * alpha[i]);
    }
    return *this;
  }

  inline Vec<type, length>& sigmoid() {
    for (size_t i = 0; i < length; ++i) {
      values[i] = static_cast<type>(1) / (1 + std::exp(-values[i]));
    }
    return *this;
  }

  friend Vec<type,length> operator+(Vec<type,length> lhs, const Vec<type,length> &rhs) {
    lhs += rhs;
    return lhs;
  }
  
  inline Vec<type, length>& operator-=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i < length; ++i) {
      values[i] -= rhs[i];
    }
    return *this;
  }
  
  friend Vec<type,length> operator-(Vec<type,length> lhs, const Vec<type,length> &rhs) {
    lhs -= rhs;
    return lhs;
  }
  
  inline Vec<type, length>& operator/=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i < length; ++i) {
      values[i] /= rhs[i];
    }
    return *this;
  }
  
  friend Vec<type,length> operator/(Vec<type,length> lhs, const Vec<type,length> &rhs) {
    lhs /= rhs;
    return lhs;
  }
  
  inline Vec<type, length>& operator*=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i < length; ++i) {
      values[i] *= rhs[i];
    }
    return *this;
  }
  
  friend Vec<type,length> operator*(Vec<type,length> lhs, const Vec<type,length> &rhs) {
    lhs *= rhs;
    return lhs;
  }
  
  inline Vec<type, length>& operator*=(const int rhs) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] *= rhs;
    }
    return *this;
  }
  
  inline Vec<type, length>& operator*=(const double rhs) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] *= rhs;
    }
    return *this;
  }
  
  inline Vec<type, length>& operator/=(const int rhs) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] /= rhs;
    }
    return *this;
  }
  
  inline Vec<type, length>& operator/=(const double rhs) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] /= rhs;
    }
    return *this;
  }

  inline Vec<type, length>& FMA(const Vec<type, length> &a, const type &b) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] += a[i] * b;
    }
    return *this;
  }

  inline Vec<type, length>& FMA(const Vec<type, length> &a, const Vec<type, length> &b) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] += a[i] * b[i];
    }
    return *this;
  }

  inline type squared_distance(const Vec<type,length> &other) {
    type s = 0;
    for (size_t i = 0; i < length; ++i) {
      s += std::pow(values[i] - other[i], 2);
    }
    return s;
  }

  inline Vec<type,length> sqrt() {
    Vec<type, length> res;
    for (size_t i = 0; i < length; ++i) {
      res[i] = std::sqrt(values[i]);
    }
    return res;
  }

  template<typename t>
  type dot(const Vec<t,length> &other) const {
    type s = 0;
    for (size_t i = 0; i < length; ++i) {
      s += values[i] * other[i];
    }
    return s;
  }

  inline type& operator[](std::size_t idx) { return values[idx]; }
  inline const type operator[](std::size_t idx) const { return values[idx]; }

  type values[length];
};

#ifdef __AVX__
#include <immintrin.h>

// Sept. 10, 2020. Taken from: https://stackoverflow.com/questions/13219146/how-to-sum-m256-horizontally
inline float sum8(__m256 x) {
    const __m128 hiQuad = _mm256_extractf128_ps(x, 1);
    const __m128 loQuad = _mm256_castps256_ps128(x);
    const __m128 sumQuad = _mm_add_ps(loQuad, hiQuad);
    const __m128 loDual = sumQuad;
    const __m128 hiDual = _mm_movehl_ps(sumQuad, sumQuad);
    const __m128 sumDual = _mm_add_ps(loDual, hiDual);
    const __m128 lo = sumDual;
    const __m128 hi = _mm_shuffle_ps(sumDual, sumDual, 0x1);
    const __m128 sum = _mm_add_ss(lo, hi);
    return _mm_cvtss_f32(sum);
}

inline __m256 fmadd_ps(__m256 a, __m256 b, __m256 c) {
  //return _mm256_fmadd_ps(a, b, c);
  return _mm256_add_ps(_mm256_mul_ps(a, b), c);
}

template<size_t length>
struct Vec<float, length> {
  using type = float;
  Vec() {}
  
  Vec(float val) {
    for (size_t i = 0; i < length; ++i) {
      values[i] = val;
    }
  }
  
  inline size_t size() const {
    return length;
  }
  
  inline Vec<type, length>& operator+=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i <= length-8; i+=8) {
      __m256 v1 = _mm256_loadu_ps (&values[i]);
      __m256 v2 = _mm256_loadu_ps (&rhs.values[i]);
      _mm256_storeu_ps(&values[i], _mm256_add_ps(v1, v2));
    }
    return *this;
  }
  
  inline Vec<type, length>& operator*=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i <= length-8; i+=8) {
      __m256 v1 = _mm256_loadu_ps (&values[i]);
      __m256 v2 = _mm256_loadu_ps (&rhs.values[i]);
      _mm256_storeu_ps (&values[i], _mm256_mul_ps(v1, v2));
    }
    return *this;
  }
  
  inline Vec<type, length>& relu() {
    const __m256 zero = _mm256_set1_ps(0);
    for (size_t i = 0; i <= length-8; i += 8) {
      __m256 v1 = _mm256_loadu_ps (&values[i]);
      _mm256_storeu_ps(&values[i], _mm256_max_ps(v1, zero));
    }
    return *this;
  }
  
  inline Vec<type, length>& FMA(const Vec<type, length> &a, const type &b) {
    static_assert(length == 8 * (length / 8), "Input length is not multiple of 16");
    __m256 vb = _mm256_set1_ps (b);
    for (size_t i = 0; i <= length-8; i+=8) {
      __m256 c = _mm256_loadu_ps (&values[i]);
      __m256 va = _mm256_loadu_ps (&a.values[i]);
      _mm256_storeu_ps (&values[i], fmadd_ps (va, vb, c));
    }
    return *this;
  }

  inline Vec<type, length>& FMA(const Vec<type, length> &a, const Vec<type, length> &b) {
    static_assert(length == 8 * (length / 8), "Input length is not multiple of 16");
    for (size_t i = 0; i <= length-8; i+=8) {
      __m256 c = _mm256_loadu_ps (&values[i]);
      __m256 va = _mm256_loadu_ps (&a.values[i]);
      __m256 vb = _mm256_loadu_ps (&b.values[i]);
      _mm256_storeu_ps (&values[i], fmadd_ps (va, vb, c));
    }
    return *this;
  }
  
  template<typename t>
  float dot(const Vec<t, length> &other) const {
    static_assert(length == 8 * (length / 8), "Input length is not multiple of 16");
    __m256 c = _mm256_set1_ps(0);
    for (size_t i = 0; i <= length-8; i+=8) {
      __m256 va = _mm256_loadu_ps (&values[i]);
      __m256 vb = _mm256_loadu_ps (&other.values[i]);
      c = fmadd_ps(va, vb, c);
    }
    return sum8(c);
  }
  
  inline float& operator[](std::size_t idx) { return values[idx]; }
  inline const float operator[](std::size_t idx) const { return values[idx]; }
  
  float values[length];
};

template<size_t length>
struct Vec<__m256, length> {
  using type = __m256;
  static constexpr size_t arr_length = length / 8;
  Vec() {}
  
  Vec(Vec<float, length> input) {
    for (size_t i = 0; i < arr_length; ++i) {
      values[i] = _mm256_loadu_ps(&input.values[i*8]);
    }
  }
  
  inline size_t size() const {
    return length;
  }
  
  inline Vec<type, length>& operator+=(const Vec<__m256, length> &rhs) {
    for (size_t i = 0; i < arr_length; ++i) {
      values[i] = _mm256_add_ps(values[i], rhs.values[i]);
    }
    return *this;
  }
  
  /*inline Vec<type, length>& operator*=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 v1 = _mm_loadu_ps (&values[i]);
      __m128 v2 = _mm_loadu_ps (&rhs.values[i]);
      _mm_storeu_ps (&values[i], _mm_mul_ps(v1, v2));
    }
    return *this;
  }*/
  
  inline Vec<__m256, length>& relu() {
    const __m256 zero = _mm256_set1_ps(0);
    for (size_t i = 0; i < arr_length; ++i) {
      values[i] = _mm256_max_ps(values[i], zero);
    }
    return *this;
  }
  
  inline Vec<__m256, length>& FMA(const Vec<float, length> &a, const float &b) {
    __m256 vb = _mm256_set1_ps(b);
    for (size_t i = 0; i < arr_length; ++i) {
      __m256 va = _mm256_loadu_ps (&a.values[8*i]);
      values[i] = fmadd_ps (va, vb, values[i]);
    }
    return *this;
  }
  
  inline Vec<__m256, length>& FMA(const Vec<__m256, length> &a, const float &b) {
    __m256 vb = _mm256_set1_ps(b);
    for (size_t i = 0; i < arr_length; ++i) {
      values[i] = fmadd_ps (a.values[i], vb, values[i]);
    }
    return *this;
  }

  /*inline Vec<type, length>& FMA(const Vec<type, length> &a, const Vec<type, length> &b) {
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 c = _mm_loadu_ps (&values[i]);
      __m128 va = _mm_loadu_ps (&a.values[i]);
      __m128 vb = _mm_loadu_ps (&b.values[i]);
      _mm_storeu_ps (&values[i], fmadd_ps (va, vb, c));
    }
    return *this;
  }
  
  inline float& operator[](std::size_t idx) { return values[idx]; }
  inline const float operator[](std::size_t idx) const { return values[idx]; }*/
  
  float dot(const Vec<__m256, length> &other) const {
    __m256 c = _mm256_set1_ps(0);
    for (size_t i = 0; i < arr_length; ++i) {
      c = fmadd_ps(values[i], other.values[i], c);
    }
    return sum8(c);
  }
  
  inline Vec<float, length> to_simple_vec() const {
    Vec<float, length> result;
    for (size_t i = 0; i < arr_length; ++i) {
      _mm256_storeu_ps(&result.values[8*i], values[i]);
    }
    return result;
  }
  
  __m256 values[arr_length];
};

#elif  __SSE__
#include <xmmintrin.h>

// Sept. 10, 2020. Taken from: https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
inline float sum4(__m128 v) {                                 
  __m128 shuf = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 3, 0, 1));
  __m128 sums = _mm_add_ps(v, shuf);
  shuf = _mm_movehl_ps(shuf, sums);
  sums = _mm_add_ss(sums, shuf);
  return _mm_cvtss_f32(sums);
}

inline __m128 fmadd_ps(__m128 a, __m128 b, __m128 c) {
  //return _mm256_fmadd_ps(a, b, c);
  return _mm_add_ps(_mm_mul_ps(a, b), c);
}

template<size_t length>
struct Vec<float, length> {
  using type = float;
  Vec() {}
  
  Vec(float val) {
    for (size_t i = 0; i < length; ++i) {
      values[i] = val;
    }
  }
  
  inline size_t size() const {
    return length;
  }
  
  inline Vec<type, length>& operator+=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 v1 = _mm_loadu_ps (&values[i]);
      __m128 v2 = _mm_loadu_ps (&rhs.values[i]);
      _mm_storeu_ps(&values[i], _mm_add_ps(v1, v2));
    }
    return *this;
  }
  
  inline Vec<type, length>& operator*=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 v1 = _mm_loadu_ps (&values[i]);
      __m128 v2 = _mm_loadu_ps (&rhs.values[i]);
      _mm_storeu_ps (&values[i], _mm_mul_ps(v1, v2));
    }
    return *this;
  }
  
  inline Vec<type, length>& relu() {
    const __m128 zero = _mm_set1_ps(0);
    for (size_t i = 0; i <= length-4; i += 4) {
      __m128 v1 = _mm_loadu_ps (&values[i]);
      _mm_storeu_ps(&values[i], _mm_max_ps(v1, zero));
    }
    return *this;
  }
  
  inline Vec<type, length>& FMA(const Vec<type, length> &a, const type &b) {
    static_assert(length == 8 * (length / 8), "Input length is not multiple of 16");
    __m128 vb = _mm_set1_ps (b);
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 c = _mm_loadu_ps (&values[i]);
      __m128 va = _mm_loadu_ps (&a.values[i]);
      _mm_storeu_ps (&values[i], fmadd_ps (va, vb, c));
    }
    return *this;
  }

  inline Vec<type, length>& FMA(const Vec<type, length> &a, const Vec<type, length> &b) {
    static_assert(length == 8 * (length / 8), "Input length is not multiple of 16");
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 c = _mm_loadu_ps (&values[i]);
      __m128 va = _mm_loadu_ps (&a.values[i]);
      __m128 vb = _mm_loadu_ps (&b.values[i]);
      _mm_storeu_ps (&values[i], fmadd_ps (va, vb, c));
    }
    return *this;
  }
  
  template<typename t>
  float dot(const Vec<t, length> &other) const {
    static_assert(length == 8 * (length / 8), "Input length is not multiple of 16");
    __m128 c = _mm_set1_ps(0);
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 va = _mm_loadu_ps (&values[i]);
      __m128 vb = _mm_loadu_ps (&other.values[i]);
      c = fmadd_ps(va, vb, c);
    }
    return sum4(c);
  }
  
  inline float& operator[](std::size_t idx) { return values[idx]; }
  inline const float operator[](std::size_t idx) const { return values[idx]; }
  
  float values[length];
};

template<size_t length>
struct Vec<__m128, length> {
  using type = __m128;
  static constexpr size_t arr_length = length / 4;
  Vec() {}
  
  Vec(Vec<float, length> input) {
    for (size_t i = 0; i < arr_length; ++i) {
      values[i] = _mm_loadu_ps(&input.values[i*4]);
    }
  }
  
  inline size_t size() const {
    return length;
  }
  
  inline Vec<type, length>& operator+=(const Vec<__m128, length> &rhs) {
    for (size_t i = 0; i < arr_length; ++i) {
      values[i] = _mm_add_ps(values[i], rhs.values[i]);
    }
    return *this;
  }
  
  /*inline Vec<type, length>& operator*=(const Vec<type, length> &rhs) {
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 v1 = _mm_loadu_ps (&values[i]);
      __m128 v2 = _mm_loadu_ps (&rhs.values[i]);
      _mm_storeu_ps (&values[i], _mm_mul_ps(v1, v2));
    }
    return *this;
  }*/
  
  inline Vec<__m128, length>& relu() {
    const __m128 zero = _mm_set1_ps(0);
    for (size_t i = 0; i < arr_length; ++i) {
      values[i] = _mm_max_ps(values[i], zero);
    }
    return *this;
  }
  
  inline Vec<__m128, length>& FMA(const Vec<float, length> &a, const float &b) {
    __m128 vb = _mm_set1_ps(b);
    for (size_t i = 0; i < arr_length; ++i) {
      __m128 va = _mm_loadu_ps (&a.values[4*i]);
      values[i] = fmadd_ps (va, vb, values[i]);
    }
    return *this;
  }
  
  inline Vec<__m128, length>& FMA(const Vec<__m128, length> &a, const float &b) {
    __m128 vb = _mm_set1_ps(b);
    for (size_t i = 0; i < arr_length; ++i) {
      values[i] = fmadd_ps (a.values[i], vb, values[i]);
    }
    return *this;
  }

  /*inline Vec<type, length>& FMA(const Vec<type, length> &a, const Vec<type, length> &b) {
    for (size_t i = 0; i <= length-4; i+=4) {
      __m128 c = _mm_loadu_ps (&values[i]);
      __m128 va = _mm_loadu_ps (&a.values[i]);
      __m128 vb = _mm_loadu_ps (&b.values[i]);
      _mm_storeu_ps (&values[i], fmadd_ps (va, vb, c));
    }
    return *this;
  }
  
  inline float& operator[](std::size_t idx) { return values[idx]; }
  inline const float operator[](std::size_t idx) const { return values[idx]; }*/
  
  float dot(const Vec<__m128, length> &other) const {
    __m128 c = _mm_set1_ps(0);
    for (size_t i = 0; i < arr_length; ++i) {
      c = fmadd_ps(values[i], other.values[i], c);
    }
    return sum4(c);
  }
  
  inline Vec<float, length> to_simple_vec() const {
    Vec<float, length> result;
    for (size_t i = 0; i < arr_length; ++i) {
      _mm_storeu_ps(&result.values[4*i], values[i]);
    }
    return result;
  }
  
  __m128 values[arr_length];
};

#endif



template<typename t, size_t l>
Vec<t,l> operator*(Vec<t,l> lhs, const int rhs) {
  lhs *= rhs;
  return lhs;
}
template<typename t, size_t l>
Vec<t,l> operator/(Vec<t,l> lhs, const int rhs) {
  lhs /= rhs;
  return lhs;
}
template<typename t, size_t l>
Vec<t,l> operator*(const double &rhs, Vec<t,l> lhs) {
  lhs *= rhs;
  return lhs;
}
template<typename t, size_t l>
Vec<t,l> operator*(const int &rhs, Vec<t,l> lhs) { return lhs * rhs; }
template<typename t, size_t l>
Vec<t,l> operator-(Vec<t,l> score) {
  for (size_t i = 0; i < l; ++i) {
    score[i] = -score[i];
  }
  return score;
}
template<typename t, size_t l>
inline bool operator==(const Vec<t,l>& lhs, const Vec<t,l>& rhs) {
  bool res = true;
  for (size_t i = 0; i < l; ++i) {
    res = res && lhs[i] == rhs[i];
  }
  return res;
}
template<typename t, size_t l>
inline bool operator!=(const Vec<t,l>& lhs, const Vec<t,l>& rhs) { return !(lhs == rhs); }

template<typename t, size_t l>
Vec<t,l> squared_differences(const Vec<t,l> &v1, const Vec<t,l> &v2) {
  Vec<t,l> v;
  for (size_t i = 0; i < l; ++i) {
    v[i] = std::pow(v1[i] - v2[i], 2);
  }
  return v;
}

template <typename type,size_t length>
struct SqMatrix {
  SqMatrix() {
    for (size_t i = 0; i < length; ++i) {
      values[i] = Vec<type, length>(0);
    }
  }

  SqMatrix(type val) {
    for (size_t i = 0; i < length; ++i) {
      values[i] = Vec<type, length>(val);
    }
  }

  SqMatrix<type, length>& operator/=(const double rhs) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] /= rhs;
    }
    return *this;
  }

  SqMatrix<type, length>& operator+=(const SqMatrix<type, length> rhs) {
    for (size_t i = 0; i < length; ++i) {
      this->values[i] += rhs[i];
    }
    return *this;
  }

  SqMatrix<type, length> transpose() const {
    SqMatrix<type, length> mat;
    for (size_t i = 0; i < length; ++i) {
      for (size_t j = 0; j < length; ++j) {
        mat[i][j] = values[j][i];
      }
    }
    return *this;
  }

  template<typename t>
  Vec<type,length> dot(const Vec<t, length> &other) const {
    Vec<type, length> s(0);
    for (size_t i = 0; i < length; ++i) {
      s[i] = values[i].dot(other);
    }
    return s;
  }

  SqMatrix<type,length> dot(const SqMatrix<type, length> &other) {
    SqMatrix<type, length> mat = other.transpose();
    SqMatrix<type, length> result;
    for (size_t i = 0; i < length; ++i) {
      result[i] = this->dot(mat[i]);
    }
    return result.transpose();
  }

  void print() const {
    for (size_t i = 0; i < length; ++i) {
      values[i].print();
      std::cout << std::endl;
    }
  }

  Vec<type, length>& operator[](std::size_t idx) { return values[idx]; }
  const Vec<type, length> operator[](std::size_t idx) const { return values[idx]; }

  Vec<type, length> values[length];
};

template Vec<double, kPhaseVecLength>
    SqMatrix<double, kPhaseVecLength>::dot<double>(
        const Vec<double, kPhaseVecLength> &other) const;

template Vec<double, kPhaseVecLength>
    SqMatrix<double, kPhaseVecLength>::dot<NScore>(
        const Vec<NScore, kPhaseVecLength> &other) const;

template<typename type, size_t length>
struct DeterminantHelper {
  static type calculate(const SqMatrix<type, length> &mat) {
    SqMatrix<type, length - 1> h_mat;
    double det = 0;
    for (size_t i = 0; i < length; ++i) {
      int sign = 1;
      if (i % 2) {
        sign = -1;
      }
      int inc = 0;
      for (size_t u = 0; u < length; ++u) {
        if (u == i) {
          inc++;
          continue;
        }
        for (size_t v = 1; v < length; ++v) {
          h_mat[u - inc][v-1] = mat[u][v];
        }
      }
      det += sign * mat[i][0] * calculate_rec(h_mat);
    }
    return det;
  }
  static type calculate_rec(const SqMatrix<type, length-1> &mat) {
    return DeterminantHelper<type, length-1>::calculate(mat);
  }
};

template<typename type>
struct DeterminantHelper<type, 1> {
  static type calculate(const SqMatrix<type, 1> &mat) {
    return mat[0][0];
  }
};

template struct DeterminantHelper<double, 2>;

template<typename type, size_t length> type Determinant(const SqMatrix<type, length> &mat) {
  return DeterminantHelper<type, length>::calculate(mat);
}


template<typename type, size_t length> SqMatrix<type, length> SymmetricSquareMatrixInv(
      const SqMatrix<type, length> &mat) {
  double det = Determinant<type, length>(mat);
  SqMatrix<type, length> adj_mat;
  SqMatrix<type, length - 1> h_mat;
  for (size_t i = 0; i < length; ++i) {
    for (size_t j = 0; j < length; ++j) {
      int sign = 1;
      if ((i+j) % 2) {
        sign = -1;
      }
      int inc_u = 0;
      for (size_t u = 0; u < length; ++u) {
        if (u == i) {
          inc_u++;
          continue;
        }
        int inc_v = 0;
        for (size_t v = 0; v < length; ++v) {
          if (v == j) {
            inc_v++;
            continue;
          }
          h_mat[u - inc_u][v - inc_v] = mat[u][v];
        }
      }
      adj_mat[i][j] = sign * Determinant<type, length - 1>(h_mat);
    }
  }
  for (size_t i = 0; i < length; ++i) {
    for (size_t j = i+1; j < length; ++j) {
      double tmp = adj_mat[i][j];
      adj_mat[i][j] = adj_mat[j][i];
      adj_mat[j][i] = tmp;
    }
  }
  adj_mat /= det;
  return adj_mat;
}

#endif /* LEARNING_LINEAR_ALGEBRA_H_ */
