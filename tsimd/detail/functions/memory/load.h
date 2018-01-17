// ========================================================================== //
// The MIT License (MIT)                                                      //
//                                                                            //
// Copyright (c) 2017 Intel Corporation                                       //
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included in //
// in all copies or substantial portions of the Software.                     //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //
// DEALINGS IN THE SOFTWARE.                                                  //
// ========================================================================== //

#pragma once

#include "../../pack.h"

namespace tsimd {

  // load() ///////////////////////////////////////////////////////////////////

  template <typename PACK_T>
  TSIMD_INLINE PACK_T load(const void *_src);

  template <typename PACK_T>
  TSIMD_INLINE PACK_T
  load(const void *_src,
       const mask<typename PACK_T::value_t, PACK_T::static_size> &m);

  // 1-wide //

  namespace detail {
    template <typename PACK_T, typename = traits::is_pack_of_width_t<PACK_T, 1>>
    TSIMD_INLINE PACK_T load1(const void *_src)
    {
      using T = const typename PACK_T::value_t;
      return PACK_T(*((T *)_src));
    }

    template <typename PACK_T, typename = traits::is_pack_of_width_t<PACK_T, 1>>
    TSIMD_INLINE PACK_T load1(const void *_src,
                              const mask_for_pack_t<PACK_T> &mask)
    {
      using T = const typename PACK_T::value_t;
      return PACK_T(mask[0] ? *((T *)_src) : T());
    }
  }  // namespace detail

  template <>
  TSIMD_INLINE vfloat1 load(const void *_src)
  {
    return detail::load1<vfloat1>(_src);
  }

  template <>
  TSIMD_INLINE vfloat1 load(const void *_src, const vboolf1 &m)
  {
    return detail::load1<vfloat1>(_src, m);
  }

  template <>
  TSIMD_INLINE vint1 load(const void *_src)
  {
    return detail::load1<vint1>(_src);
  }

  template <>
  TSIMD_INLINE vint1 load(const void *_src, const vboolf1 &m)
  {
    return detail::load1<vint1>(_src, m);
  }

  template <>
  TSIMD_INLINE vdouble1 load(const void *_src)
  {
    return detail::load1<vdouble1>(_src);
  }

  template <>
  TSIMD_INLINE vdouble1 load(const void *_src, const vboold1 &m)
  {
    return detail::load1<vdouble1>(_src, m);
  }

  template <>
  TSIMD_INLINE vllong1 load(const void *_src)
  {
    return detail::load1<vllong1>(_src);
  }

  template <>
  TSIMD_INLINE vllong1 load(const void *_src, const vboold1 &m)
  {
    return detail::load1<vllong1>(_src, m);
  }

  // 4-wide //

  template <>
  TSIMD_INLINE vfloat4 load(const void *_src)
  {
#if defined(__SSE__)
    return _mm_load_ps((const float *)_src);
#else
    auto *src = (const typename vfloat4::value_t *)_src;
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vfloat4 load(const void *_src, const vboolf4 &mask)
  {
#if defined(__AVX512VL__)
    const vfloat4 zero(0);
    return _mm_mask_load_ps(zero, mask, _src);
#elif defined(__SSE__)
    return _mm_and_ps(_mm_load_ps((const float *)_src), mask);
#else
    auto *src = (const typename vfloat4::value_t *)_src;
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      if (mask[i])
        result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vint4 load(const void *_src)
  {
#if defined(__SSE__)
    return _mm_load_si128((const __m128i *)_src);
#else
    auto *src = (const typename vint4::value_t *)_src;
    vint4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vint4 load(const void *_src, const vboolf4 &mask)
  {
#if defined(__AVX512VL__)
    const vint4 zero(0);
    return _mm_mask_load_epi32(zero, mask, _src);
#elif defined(__SSE__)
    return _mm_and_si128(_mm_load_si128((const __m128i *)_src), mask);
#else
    auto *src = (const typename vint4::value_t *)_src;
    vint4 result;

    for (int i = 0; i < 4; ++i)
      if (mask[i])
        result[i] = src[i];

    return result;
#endif
  }

  // 8-wide //

  template <>
  TSIMD_INLINE vfloat8 load(const void *_src)
  {
#if defined(__AVX512__) || defined(__AVX__)
    return _mm256_load_ps((const float *)_src);
#else
    auto *src = (const typename vfloat8::value_t *)_src;
    return vfloat8(load<vfloat4>(src), load<vfloat4>(src + 4));
#endif
  }

  template <>
  TSIMD_INLINE vfloat8 load(const void *_src, const vboolf8 &mask)
  {
#if defined(__AVX512VL__)
    const vfloat8 zero(0);
    return _mm256_mask_load_ps(zero, mask, _src);
#elif defined(__AVX__)
    return _mm256_maskload_ps((const float *)_src, _mm256_castps_si256(mask));
#else
    auto *src = (const typename vfloat16::value_t *)_src;
    return vfloat8(load<vfloat4>(src, vboolf4(mask.vl)),
                   load<vfloat4>(src + 4, vboolf4(mask.vh)));
#endif
  }

  template <>
  TSIMD_INLINE vint8 load(const void *_src)
  {
#if defined(__AVX512__) || defined(__AVX__)
    return _mm256_castps_si256(_mm256_load_ps((const float *)_src));
#else
    auto *src = (const typename vint16::value_t *)_src;
    return vint8(load<vint4>(src), load<vint4>(src + 4));
#endif
  }

  template <>
  TSIMD_INLINE vint8 load(const void *_src, const vboolf8 &mask)
  {
#if defined(__AVX512VL__)
    const vint8 zero(0);
    return _mm256_mask_load_epi32(zero, mask, _src);
#elif defined(__AVX__)
    return _mm256_castps_si256(
        _mm256_maskload_ps((const float *)_src, _mm256_castps_si256(mask)));
#else
    auto *src = (const typename vint16::value_t *)_src;
    return vint8(load<vint4>(src, vboolf4(mask.vl)),
                 load<vint4>(src + 4, vboolf4(mask.vh)));
#endif
  }

  // 16-wide //

  template <>
  TSIMD_INLINE vfloat16 load(const void *_src)
  {
#if defined(__AVX512F__)
    return _mm512_load_ps((const float *)_src);
#else
    auto *src = (const typename vfloat16::value_t *)_src;
    return vfloat16(load<vfloat8>(src), load<vfloat8>(src + 8));
#endif
  }

  template <>
  TSIMD_INLINE vfloat16 load(const void *_src, const vboolf16 &mask)
  {
#if defined(__AVX512F__)
    return _mm512_mask_load_ps(_mm512_setzero_ps(), mask, (float *)_src);
#else
    auto *src = (const typename vfloat16::value_t *)_src;
    return vfloat16(load<vfloat8>(src, vboolf8(mask.vl)),
                    load<vfloat8>(src + 8, vboolf8(mask.vh)));
#endif
  }

  template <>
  TSIMD_INLINE vint16 load(const void *_src)
  {
#if defined(__AVX512F__)
    return _mm512_load_si512((const int *)_src);
#else
    auto *src = (const typename vint16::value_t *)_src;
    return vint16(load<vint8>(src), load<vint8>(src + 8));
#endif
  }

  template <>
  TSIMD_INLINE vint16 load(const void *_src, const vboolf16 &mask)
  {
#if defined(__AVX512F__)
    return _mm512_mask_load_epi32(_mm512_setzero_epi32(), mask, _src);
#else
    auto *src = (const typename vint16::value_t *)_src;
    return vint16(load<vint8>(src, vboolf8(mask.vl)),
                  load<vint8>(src + 8, vboolf8(mask.vh)));
#endif
  }

}  // namespace tsimd
